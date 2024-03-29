#ifndef _SYSCALL_H
#define _SYSCALL_H

#include "types.h"

#define FD_ARRAY_SIZE   8

/* Virtual address of user-level program */
#define PROGRAM_VIRTUAL         0x8000000  // 128MB
#define PROGRAM_IMAGE_VIRTUAL   0x0804800

/* Physical address of user-level program */
#define PROGRAM_PHYSICAL    0x800000 // 8MB

#define PROGRAM_SPACE       0x400000 // 4MB

#define PAGE_DIR_OFFSET     22
#define PAGE_FRAME_OFFSET   12

typedef struct file_operations {
    int32_t (*read) (int32_t fd, void* buf, int32_t nbytes);
    int32_t (*write) (int32_t fd, const void* buf, int32_t nbytes);
    int32_t (*open) (const uint8_t* filename);
    int32_t (*close) (int32_t fd);
} file_operations_t;


typedef struct file_descriptor {
    file_operations_t *file_operations;
    uint32_t inode;
    uint32_t file_position;
    uint32_t flags;
} file_descriptor_t ;


typedef struct pcb {
    uint32_t pid;
    uint32_t parent_id;
    uint32_t ebp;
    uint32_t eip;
    file_descriptor_t *fd_array[FD_ARRAY_SIZE];
} pcb_t;


#endif
