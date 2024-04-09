#include "syscall.h"
#include "x86_desc.h"
#include "fs.h"
#include "lib.h"
#include "terminal.h"
#include "devices/RTC.h"

uint32_t curr_pid = -1;
extern void flush_tlb();
extern void halt_function( uint32_t parent_ebp, uint32_t parent_esp, int32_t status);

/* uint32_t get_next_pid();
 * Inputs: None
 * Return Value: next process id
 * Function: Get next process id */
uint32_t get_next_pid() {
    if (curr_pid == MAX_PROCESSES - 1) {
        return -1;
    }

    uint32_t next_pid = ++curr_pid;
    return next_pid;
}

/* pcb_t *get_pcb(uint32_t pid);
 * Inputs: uint32_t pid = process id
 * Return Value: pointer to pcb struct
 * Function: Get pointer to pcb struct of process pid */
pcb_t *get_pcb(uint32_t pid) {
    // PCB starts at the top of an 8KB block (each process is assigned a block in kernel stack, first block starts at 8MB)
    // First PCB struct starts at 8MB - 8KB, second PCB starts at 8MB - 8KB - 8KB
    if (pid >= MAX_PROCESSES) {
        return NULL;
    }

    return (pcb_t *) (KERNAL_STACK - (pid + 1) * KERNEL_STACK_SIZE);
}

/* set_program_page(uint32_t pid);
 * Inputs: uint32_t pid = process id
 * Return Value: None
 * Function: Set up paging for program */
void set_program_page(uint32_t pid) {
    uint32_t pde_id = PROGRAM_VIRTUAL >> PAGE_DIR_OFFSET;
    // uint32_t pde_id = PROGRAM_PHYSICAL - (  KERNEL_STACK_SIZE); // absolute address goes incrementingly, need to start from different point
    
    page_directory[pde_id].pde_MB.isPresent = 1; 
    page_directory[pde_id].pde_MB.isPageSize = 1; /* 4MB page */  
    page_directory[pde_id].pde_MB.isGlobal = 0; /* page is a per-process page and the translations will be cleared when CR3 is reloaded*/ 
    page_directory[pde_id].pde_MB.isUserSupervisor = 1; /* user-level */ 
    page_directory[pde_id].pde_MB.isReadWrite = 1;
    page_directory[pde_id].pde_MB.pageBaseAddr = (PROGRAM_PHYSICAL + (pid * PROGRAM_SPACE)) >> PAGE_FRAME_OFFSET;

    flush_tlb();
}

/* reset_program_page(uint32_t pid);
 * Inputs: uint32_t pid = process id
 * Return Value: None
 * Function: Set up paging for program */
void reset_program_page(uint32_t pid) {
    uint32_t pde_id = PROGRAM_PHYSICAL - ((pid+1) * KERNEL_STACK_SIZE);
    
    page_directory[pde_id].pde_MB.isPresent = 0; 
    // page_directory[pde_id].pde_MB.isPageSize = 0; /* 4MB page */  
    // page_directory[pde_id].pde_MB.isGlobal = 0; /* page is a per-process page and the translations will be cleared when CR3 is reloaded*/ 
    // page_directory[pde_id].pde_MB.isUserSupervisor = 0; /* user-level */ 
    // page_directory[pde_id].pde_MB.isReadWrite = 0;
    // page_directory[pde_id].pde_MB.pageBaseAddr = (PROGRAM_PHYSICAL + (pid * PROGRAM_SPACE)) >> PAGE_FRAME_OFFSET;

    flush_tlb();
}

/* uint32_t load_program_image(const uint8_t *program_name);
 * Inputs: uint8_t *program_name = name of program to load
 * Return Value: entry point into program on success, -1 on failure
 * Function: Load program image into default program image address */
int32_t load_program_image(const uint8_t *program_name) {
    dentry_t dentry;
    uint32_t inode;
    uint32_t prog_size;
    uint32_t prog_entry;
    uint32_t prog_entry_size = 4;

    /* Look up program in fs */ 
    if (read_dentry_by_name(program_name, &dentry) == -1) {
        return -1;
    }

    inode = dentry.inode_num;
    prog_size = ((inode_t *) inode_start + inode)->len;

    /* Read program data */ 
    if (prog_size != read_data(inode, 0, (uint8_t *) PROGRAM_IMAGE_VIRTUAL, prog_size)) {
        return -1;
    }

    /* Get entry point into program (bytes 24-27) */ 
    if (prog_entry_size != read_data(inode, 24, (uint8_t *) &prog_entry, prog_entry_size)) {
        return -1;
    }

    return prog_entry;
}

/* is_valid_file(const uint8_t *file_name);
 * Inputs: const uint8_t *file_name = file name
 * Return Value: 1 if valid, 0 if invalid
 * Function: Check if a file is valid (exits in fs and is executable) */
int8_t is_valid_file(const uint8_t *file_name) {
    dentry_t dentry;
    uint8_t magic_num_size = 4;
    uint8_t magic_num[magic_num_size];
    
    /* Look up program in fs */ 
    if (read_dentry_by_name(file_name, &dentry) == -1) {
        return 0;
    }

    /* Check if program is an executable by reading the magic number (first 4 bytes) */ 
    if (magic_num_size != read_data(dentry.inode_num, 0, magic_num, magic_num_size)) {
        return 0;
    }

    if (magic_num[0] != MAGIC_0 || magic_num[1] != MAGIC_1 || magic_num[2] != MAGIC_2 || magic_num[3] != MAGIC_3) {
        // Not an executable
        return 0;
    }

    return 1;
}

/* uint8_t *parse(const uint8_t* command);
 * Inputs: const uint8_t* command = command to parse
 * Return Value: program file name
 * Function: Extract the program file name */
uint8_t buffer[128];
void parse(const uint8_t* command) {
    if (command == NULL) {
        return;
    }
    int inWord = 0;

    memset(buffer, 0, 128);
    int i = 0;
    int j = 0;
    while (command[i] != '\0') {
        if (command[i] == ' ' && inWord) {
            inWord = 0;
            //buffer[j] = '\0'; // null-terminate word
            break;
        }
        else if (command[i] != ' ') {
            buffer[j] = command[i];
            inWord = 1;
            j++;
        }
        i++;
    }
}

/* int32_t execute(const uint8_t* command);
 * Inputs: const uint8_t* command = command to execute
 * Return Value: 0 on success, -1 on failure
 * Function: Load and execute a new program */
int32_t execute(const uint8_t* command){
    if (command == NULL) {
        return -1;
    }

    

    /* TODO: CHECK FOR PARTIAL FUNCTIONALITY PROGRAM (FISH, CAT, GREP, SIGTEST) --> EXIT GRACEFULLY */

    /* Parse command to get file name of program */
    parse(command);

    /* Check file validity */
    if (!is_valid_file(buffer)) {
        return -1;
    }

    /* Get new pid */
    uint32_t pid;
    if ((pid = get_next_pid()) == -1) {
        return -1;
    }

    /* Set up paging */
    set_program_page(pid);

    /* Load file into memory */
    uint32_t prog_entry;
    if ((prog_entry = load_program_image(buffer)) == -1) {
        return -1;
    }

    /* Create PCB */
    pcb_t *prog_pcb;
    if ((prog_pcb = get_pcb(pid)) == NULL) {
        return -1;
    }

    /* Save current ebp */
    register uint32_t saved_ebp asm("ebp"); 
    register uint32_t saved_esp asm("esp"); 

    prog_pcb->ebp = saved_ebp;
    prog_pcb->esp = saved_esp;

    if (pid == 0) {
        prog_pcb->parent_id = 0; 
    } else {
        prog_pcb->parent_id = pid - 1;
    }

    prog_pcb->pid = pid;
    prog_pcb->eip = prog_entry;

    /* Set fd array */

    // How to set other read/write/open/close ?

    // First entry is for stdin (terminal read)
    prog_pcb->fd_array[0].file_operations.read = terminal_read;
    prog_pcb->fd_array[0].inode = 0; 
    prog_pcb->fd_array[0].flags = FD_BUSY; 

    // Second entry is for stdout (terminal write)
    prog_pcb->fd_array[1].file_operations.write = terminal_write;
    prog_pcb->fd_array[1].inode = 0;
    prog_pcb->fd_array[1].flags = FD_BUSY; 

    /* Modify TSS */
    tss.ss0 = KERNEL_DS;
    tss.esp0 = (KERNAL_STACK - pid * KERNEL_STACK_SIZE) - 4;


    /* Push IRET context to kernel stack (SS, ESP, EFLAGS, CS, EIP) */
    asm volatile ("                 \n\
            pushl    %%eax          \n\
            pushl    %%ebx          \n\
            pushfl                  \n\
            popl     %%eax          \n\
            orl      $0x200,%%eax   \n\
            pushl    %%eax          \n\
            pushl    %%ecx          \n\
            pushl    %%edx          \n\
            iret                    \n\
            "
            :
            : "a" (USER_DS), "b" (PROGRAM_STACK_VIRTUAL - 4), "c" (USER_CS), "d" (prog_entry) 
            : "memory"
    );

    /* return */
    return 0;
}

/* helper 
* get current pcb (void)
*/
pcb_t* get_current_pcb(void){
    return (pcb_t *) (KERNAL_STACK - (curr_pid + 1) * KERNEL_STACK_SIZE);
}

int32_t halt (uint8_t status){
    int32_t ret_val = 0;

    if (status == 255) {
        ret_val = 256;
    } else {
        ret_val = (uint32_t) status;
    }
    
    pcb_t* cur_pcb_ptr = get_current_pcb();

    if(cur_pcb_ptr->pid == 0){
       //do nothing 
       execute((const uint8_t *)"shell");
    }

    int32_t i;
    /* Close all file descriptors */ 
    for(i=0; i< FD_ARRAY_SIZE; i++){
        cur_pcb_ptr->fd_array[i].flags = 0;
    }

    /* Restore parent process */
    pcb_t* parent_pcb_ptr = get_pcb(cur_pcb_ptr->parent_id); 

    tss.ss0 = KERNEL_DS;
    tss.esp0 = KERNAL_STACK - (KERNEL_STACK_SIZE * parent_pcb_ptr->pid) - sizeof(int32_t);

    curr_pid = parent_pcb_ptr->parent_id;

    set_program_page(curr_pid);

    halt_function(parent_pcb_ptr->ebp, parent_pcb_ptr->ebp, ret_val); // assuming esp and ebp are the same
    
    return 0;
}
/* read(int32_t fd, void* buf, int32_t nbytes);
 * Inputs: uint32_t fd = file descriptor
           void* buf = buffer to hold read data
           uint32_t nbytes = number of bytes to read
 * Return Value: 0 for successful read
 * Function: Fill in the buffer by file data */

int32_t read (int32_t fd, void* buf, int32_t nbytes){
    if(fd > 8 || fd < 0) return -1;
    if(buf == NULL)return -1;
    if(nbytes < 0)return -1;

    pcb_t* curr_pcb; 
    curr_pcb = get_current_pcb();
    /* fd must be BUSY to read */
    if(curr_pcb->fd_array[fd].flags == FD_FREE) return -1; 

    return curr_pcb->fd_array[fd].file_operations.read(fd,buf,nbytes);
}

/* file_write(int32_t fd, void* buf, int32_t nbytes);
 * Inputs: uint32_t fd = file descriptor
           void* buf = buffer to hold written data
           uint32_t nbytes = number of bytes to read
 * Return Value: -1 
 * Function: Write n bytes from buffer to file */

int32_t write (int32_t fd, const void* buf, int32_t nbytes){
    if(fd > 8 || fd < 0) return -1;
    if(buf == NULL)return -1;
    if(nbytes < 0)return -1;

    pcb_t* curr_pcb; 
    curr_pcb = get_current_pcb();
    /* fd must be BUSY to write */
    if(curr_pcb->fd_array[fd].flags == FD_FREE) return -1; 

    curr_pcb->fd_array[fd].file_operations.write(fd,buf,nbytes);

    return 0;
}
/* open(const uint8_t* filename);
 * Inputs: filename = file name
 * Return Value: fd on success, -1 on failure
 * Function: Open a file */

int32_t open (const uint8_t* filename){
        pcb_t* curr_pcb = get_current_pcb();
        dentry_t dentry;
        // check the fd (if its invalid) 
        if(filename == NULL){return -1;}
        // return value on invalid defined in document
        if(read_dentry_by_name(filename, &dentry) == -1){return -1;}
        // if valid, check fd for which operation needs to be called
        int i;
        /* first two are std in/out */
        /* find first free fd slot */
        for(i = 2; i < 7; i++){
            if(curr_pcb->fd_array[i].flags == FD_FREE){
                curr_pcb->fd_array[i].inode = dentry.inode_num;
                curr_pcb->fd_array[i].file_position = 0;
                curr_pcb->fd_array[i].flags = FD_BUSY;
                // (rtc , or general file)
                switch (dentry.file_type)
                {
                case 0:
                    /* code */
                    curr_pcb->fd_array[i].file_operations.open = rtc_open;
                    curr_pcb->fd_array[i].file_operations.close = rtc_close;
                    curr_pcb->fd_array[i].file_operations.read = rtc_read;
                    curr_pcb->fd_array[i].file_operations.write = rtc_write;

                    break;
                case 1:
                    /* code */
                    curr_pcb->fd_array[i].file_operations.open = dir_open;
                    curr_pcb->fd_array[i].file_operations.close = dir_close;
                    curr_pcb->fd_array[i].file_operations.read = dir_read;
                    curr_pcb->fd_array[i].file_operations.write = dir_write;
                    break;
                case 2:
                    /* code */
                    curr_pcb->fd_array[i].file_operations.open = file_open;
                    curr_pcb->fd_array[i].file_operations.close = file_close;
                    curr_pcb->fd_array[i].file_operations.read = file_read;
                    curr_pcb->fd_array[i].file_operations.write = file_write;
                    break;
                
                default:
                    break;
                }
                break;
            }
        }
        return i;

}

/* close(int32_t fd);
 * Inputs: fd = file descriptor
 * Return Value: 0 on success, -1 on fail
 * Function: Close a file */

int32_t close (int32_t fd){
    if(fd > 7 || fd < 2) return -1;

    pcb_t* curr_pcb; 
    curr_pcb = get_current_pcb();

    /* fd is already closed return fail */
    if(curr_pcb->fd_array[fd].flags == FD_FREE) return -1; 

    curr_pcb->fd_array[fd].flags = FD_FREE;
    return 0;
}



