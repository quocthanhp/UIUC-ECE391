#include "syscall.h"
#include "x86_desc.h"
#include "fs.h"
#include "lib.h"


uint32_t curr_pid = -1;
extern void flush_tlb();


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
    
    page_directory[pde_id].pde_MB.isPresent = 1; 
    page_directory[pde_id].pde_MB.isPageSize = 1; /* 4MB page */  
    page_directory[pde_id].pde_MB.isGlobal = 0; /* page is a per-process page and the translations will be cleared when CR3 is reloaded*/ 
    page_directory[pde_id].pde_MB.isUserSupervisor = 1; /* user-level */ 
    page_directory[pde_id].pde_MB.pageBaseAddr = (PROGRAM_PHYSICAL + (pid * PROGRAM_SPACE)) >> PAGE_FRAME_OFFSET;

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
uint8_t *parse(const uint8_t* command) {
    if (command == NULL) {
        return NULL;
    }

    uint8_t *buf;
    uint8_t *fn;
    fn = buf;
    int inWord = 0;

    while (*command != '\0') {
        if (*command == ' ' && inWord) {
            inWord = 0;
            *buf = '\0'; // null-terminate word
            break;
        }
        else if (*command != ' ') {
            memcpy(buf, command, 1);
            inWord = 1;
            buf++;
        }
        command++;
    }
    *buf = '\0'; // For CP3
    return fn;
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
    uint8_t *program = parse(command);

    /* Check file validity */
    if (!is_valid_file(program)) {
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
    if ((prog_entry = load_program_image(program)) == -1) {
        return -1;
    }

    /* Create PCB */
    pcb_t *prog_pcb;
    if ((prog_pcb = get_pcb(pid)) == NULL) {
        return -1;
    }

    prog_pcb->parent_id = 0; 
    prog_pcb->pid = pid;
    prog_pcb->eip = prog_entry;

    /* Save current ebp */
    register uint32_t saved_ebp asm("ebp"); 
    prog_pcb->ebp = saved_ebp;

    /* Modify TSS */
    tss.esp0 = KERNAL_STACK - pid * KERNEL_STACK_SIZE;

    /* Push IRET context to kernel stack (SS, ESP, EFLAGS, CS, EIP) */
    asm volatile ("                 \n\
            pushl    %0             \n\
            pushl    %1             \n\
            pushfl                  \n\
            pushl    %2             \n\
            pushl    %3             \n\
            "
            :
            : "r" (USER_DS), "r" (PROGRAM_STACK_VIRTUAL), "r" (USER_CS), "r" (prog_entry) 
            : "memory"
    );

    /* IRET */
    asm volatile ("iret");

    /* return */
    return 0;
}



int32_t halt (uint8_t status){}


int32_t read (int32_t fd, void* buf, int32_t nbytes){
    if(fd > 8 || fd < 0) return -1;
    if(buf == NULL)return -1;
    if(nbytes < 0)return -1;

    pcb_t* curr_pcb = get_current_pcb();
    /* fd must be BUSY to read */
    if(curr_pcb->fd_array[fd]->flags == FD_FREE) return -1; 

    curr_pcb->fd_array[fd]->file_operations->read(fd,buf,nbytes);

    return 0;
}

int32_t write (int32_t fd, const void* buf, int32_t nbytes){
    if(fd > 8 || fd < 0) return -1;
    if(buf == NULL)return -1;
    if(nbytes < 0)return -1;

    pcb_t* curr_pcb = get_current_pcb();
    /* fd must be BUSY to write */
    if(curr_pcb->fd_array[fd]->flags == FD_FREE) return -1; 

    curr_pcb->fd_array[fd]->file_operations->write(fd,buf,nbytes);

    return 0;
}

int32_t open (const uint8_t* filename){
        pcb_t* curr_pcb = get_current_pcb();

        // check the fd (if its invalid) 
        // return value on invalid defined in document
        // if valid, check fd for which operation needs to be called
        // (rtc , or general file)
        // check what kind of file has been called by checking their respective flags (executable blah blah blah)
}

int32_t close (int32_t fd){
    if(fd > 8 || fd < 0) return -1;
    pcb_t* curr_pcb = get_current_pcb();
    /* fd is already closed return fail */
    if(curr_pcb->fd_array[fd]->flags == FD_FREE) return -1; 

    curr_pcb->fd_array[fd]->flags == FD_FREE;
    return 0;
}

/* helper */
pcb_t* get_current_pcb(void){
    return (pcb_t *) (KERNAL_STACK - (curr_pid + 1) * KERNEL_STACK_SIZE);
}
