#ifndef _FS_H
#define _FS_H

#include "types.h"

#define BLOCK_SIZE                  4096
#define B_BLOCK_RESERVED_BYTES      52
#define DENTRY_RESERVED_BYTES       24        
#define MAX_FILES                   63
#define MAX_FILE_NAME               32
#define MAX_DATA_BLOCKS_PER_NODE    ((BLOCK_SIZE / 4) - 1)                

typedef struct dentry {
    char file_name[MAX_FILE_NAME];
    uint32_t file_type;
    uint32_t inode_num;
    uint8_t  reserved[DENTRY_RESERVED_BYTES];
} dentry_t;

typedef struct boot_block {
    uint32_t num_dir_entries;
    uint32_t num_inodes;
    uint32_t num_data_blocks;
    uint8_t  reserved[B_BLOCK_RESERVED_BYTES];
    dentry_t dentries[MAX_FILES];
} boot_block_t;

typedef struct inode {
    uint32_t len;
    uint32_t data_blocks[MAX_DATA_BLOCKS_PER_NODE];
} inode_t;

void fs_init(uint32_t boot_block_ptr);
int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry);
int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry);
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

/* Interface for dir */


int32_t dir_read(int32_t fd, void* buf, int32_t nbytes);
int32_t dir_write(int32_t fd, const void* buf, int32_t nbytes);
int32_t dir_open(const uint8_t* dirname);
int32_t dir_close(int32_t fd);

/* Interface for file */
int32_t file_read(int32_t fd, void* buf, int32_t nbytes);
int32_t file_write(int32_t fd, const void* buf, int32_t nbytes);
int32_t file_open(const uint8_t* filename);
int32_t file_close(int32_t fd);

/* Base pointer */
dentry_t test_dentry;
boot_block_t *boot_block_start;
inode_t *inode_start;
uint8_t *data_block_start;

#endif
