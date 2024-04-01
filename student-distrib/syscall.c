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

/* pcb_t *get_next_pcb();
 * Inputs: None
 * Return Value: pointer to pcb struct
 * Function: Get pointer to pcb struct of next process */
pcb_t *get_next_pcb() {
    // PCB starts at the top of an 8KB block (each process is assigned a block in kernel stack, first block starts at 8MB)
    // First PCB struct starts at 8MB - 8KB, second PCB starts at 8MB - 8KB - 8KB
    uint32_t next_pid = get_next_pid();

    if (next_pid < 0) {
        return NULL;
    }

    return (pcb_t *) (KERNAL_STACK - (next_pid + 1) * KERNEL_STACK_SIZE);
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
 * Return Value: entry point into program on success. -1 on failure
 * Function: Load program image into default program image address */
uint32_t load_program_image(const uint8_t *program_name) {
    dentry_t dentry;
    uint32_t inode;
    uint8_t *prog_buf;
    uint32_t prog_size;
    uint32_t prog_entry;

    /* Look up program in fs */ 
    if (read_dentry_by_name(program_name, &dentry) == -1) {
        return - 1;
    }

    inode = dentry.inode_num;
    prog_size = ((inode_t *) inode_start + inode)->len;

    /* Read program data */ 
    if (prog_size != read_data(inode, 0, prog_buf, prog_size)) {
        return -1;
    }

    /* Get entry point into program (bytes 24-27) */ 
    prog_entry = *((uint32_t *) (prog_buf + 24));

    /*  Copy into default program image address */
    uint8_t *prog_ptr = (uint8_t *) PROGRAM_IMAGE_VIRTUAL;
    int i;
    for (i = 0; i < prog_size; i++) {
        memcpy(prog_ptr, prog_buf, 1);
        prog_ptr++;
        prog_buf++;
    }

    return prog_entry;
}

int8_t is_valid_file(const uint8_t *file_name) {
    dentry_t dentry;
    uint8_t *magic_num;
    uint8_t magic_num_size = 4;

    /* Look up program in fs */ 
    if (read_dentry_by_name(file_name, &dentry) == -1) {
        return - 1;
    }

    /* Check if program is an executable by reading the magic number (first 4 bytes) */ 
    if (magic_num_size != read_data(dentry.inode_num, 0, magic_num, magic_num_size)) {
        return -1;
    }

    if (magic_num[0] != MAGIC_0 || magic_num[1] != MAGIC_1 || magic_num[2] != MAGIC_2 || magic_num[3] != MAGIC_3) {
        // Not an executable
        return -1;
    }

    return 0;
}

uint8_t *parse(const uint8_t* command) {
    if (command == NULL) {
        return NULL;
    }

    uint8_t *buf;
    uint8_t *program;
    program = buf;
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

    return program;
}

int32_t execute(const uint8_t* command){
    if (command == NULL) {
        return -1;
    }

    /* Parse command to get file name of program */
    uint8_t *program = parse(command);

    /* Check file validity */
    if (!is_valid_file(program)) {
        return -1;
    }

    /* Get new pid */
    uint32_t pid = get_next_pid();

    if (pid < 0) {
        return -1;
    }

    /* Set up paging */
    set_program_page(pid);

    /* Load file into memory */
    load_program_image(program);

    /* Create PCB */
    pcb_t *prog_pcb;
    if ((prog_pcb = get_next_pcb()) == NULL) {
        return -1;
    }

    /* Prepare for context switch */

    /* Push IRET context to kernel stack */

    /* IRET */

    /* return */
}



int32_t halt (uint8_t status){}
int32_t read (int32_t fd, void* buf, int32_t nbytes){}
int32_t write (int32_t fd, const void* buf, int32_t nbytes){}
int32_t open (const uint8_t* filename){}
int32_t close (int32_t fd){}
