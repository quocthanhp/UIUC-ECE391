#ifndef _SYSCALL_H
#define _SYSCALL_H

#include "types.h"

#define FD_ARRAY_SIZE   8
#define MAX_PROCESSES   2

/* Virtual address of user-level program */
#define PROGRAM_VIRTUAL       0x8000000  // 128MB
#define PROGRAM_IMAGE_VIRTUAL 0x8048000
#define PROGRAM_STACK_VIRTUAL 0x8400000

/* Physical address of user-level program */
#define PROGRAM_PHYSICAL 0x800000 // 8MB

/* Total space for a program */
#define PROGRAM_SPACE 0x400000 // 4MB

/* Block that contains PCB at the top and kernel stack at the bottom */
#define KERNAL_STACK 0x800000 // 8MB 
#define KERNEL_STACK_SIZE 0x2000 //8KB

/* Paging offset */
#define PAGE_DIR_OFFSET   22
#define PAGE_FRAME_OFFSET 12

/* Bytes for magic number in executable */
#define MAGIC_0 0x7F
#define MAGIC_1 0x45
#define MAGIC_2 0x4C
#define MAGIC_3 0x46  

/* status of fd "flags" */
#define FD_FREE 0
#define FD_BUSY 1

typedef struct file_operations {
    int32_t (*read) (int32_t fd, void* buf, int32_t nbytes);
    int32_t (*write) (int32_t fd, const void* buf, int32_t nbytes);
    int32_t (*open) (const uint8_t* filename);
    int32_t (*close) (int32_t fd);
} file_operations_t;


typedef struct file_descriptor {
    file_operations_t file_operations;
    uint32_t inode;
    uint32_t file_position;
    uint32_t flags;       /* use FD_FREE/BUSY */
} file_descriptor_t ;


typedef struct pcb {
    uint32_t pid;
    uint32_t parent_id;
    uint32_t ebp;
    uint32_t eip;
    uint32_t esp;
    file_descriptor_t fd_array[FD_ARRAY_SIZE];
} pcb_t;

int32_t halt (uint8_t status);
int32_t execute (const uint8_t* command);
int32_t read (int32_t fd, void* buf, int32_t nbytes);
int32_t write (int32_t fd, const void* buf, int32_t nbytes);
int32_t open (const uint8_t* filename);
int32_t close (int32_t fd);
/* cp4 syscalls */
int32_t getargs (uint8_t* buf, int32_t nbytes);
int32_t vidmap (uint8_t** screen_start);
int32_t set_handler (int32_t signum, void* handler_address);
int32_t sigreturn (void);

pcb_t* get_current_pcb(void);

#endif
