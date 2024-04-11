#include "fs.h"
#include "lib.h"
#include "syscall.h"

/* Files sys data */
uint32_t dir_read_pos = 0;
uint32_t file_read_pos = 0;

/* void fs_init(uint32_t boot_block_ptr);
 * Inputs: uint32_t boot_block_ptr = address of boot block
 * Return Value: none
 * Function: Initialize file system */
void fs_init(uint32_t boot_block_ptr) {
    boot_block_start = (boot_block_t *) boot_block_ptr;
    inode_start = (inode_t *) (boot_block_start + 1);
    uint32_t num_inodes = boot_block_start->num_inodes;
    data_block_start = (uint8_t *) (boot_block_start + 1 + num_inodes);
}

/* int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry);
 * Inputs: const uint8_t* fname = file name
           dentry_t* dentry = dentry to be read 
 * Return Value: 0 on success, -1 on failure
 * Function: Fill in the dentry block */
int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry) {
    uint32_t f_len = strlen((int8_t *) fname);
    
    if (fname == NULL || dentry == NULL || f_len > MAX_FILE_NAME) {
        printf("Invalid parameters\n");
        return -1;
    }

    int i;
    dentry_t curr_dentry;
    int curr_dentry_len;
    int files = boot_block_start->num_dir_entries;

    for (i = 0; i < files; i++) {
        curr_dentry = boot_block_start->dentries[i];
        curr_dentry_len = strlen((int8_t *) curr_dentry.file_name);

        if (curr_dentry_len > MAX_FILE_NAME) {
            curr_dentry_len = MAX_FILE_NAME;
        }

        if (f_len != curr_dentry_len) {
            continue;
        }

        if (strncmp((int8_t *) fname, (int8_t *) curr_dentry.file_name, f_len) == 0) {
            read_dentry_by_index(i, dentry);
            return 0;
        }
    }
    return -1;
}

/* int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry);
 * Inputs: uint32_t index = file index
           dentry_t* dentry = dentry to be read 
 * Return Value: 0 on success, -1 on failure
 * Function: Fill in the dentry block */
int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry) {
    if (index >= MAX_FILES || dentry == NULL) {
        return -1;
    }

    dentry_t retrieved_dentry = boot_block_start->dentries[index];

    // Fill out given dentry
    strncpy(dentry->file_name, retrieved_dentry.file_name, MAX_FILE_NAME);
    dentry->file_type = retrieved_dentry.file_type;
    dentry->inode_num = retrieved_dentry.inode_num;
    
    return 0;
}

/* read_block(uint8_t *blk, uint8_t* buf, uint32_t offset, uint32_t length);
 * Inputs: uint8_t *blk = pointer to block
           uint8_t* buf = buffer to hold read data
           uint32_t offset = starting byte to read
           uint32_t length = number of bytes to read
 * Return Value: number of bytes read
 * Function: Fill in the buffer */
int32_t read_block(uint8_t *blk, uint8_t* buf, uint32_t offset, uint32_t length) {
    int i;
    int32_t bytes_read = 0;

    // Read each byte from block
    for (i = offset; i < BLOCK_SIZE; i++) {
        if (bytes_read == length) {
            return bytes_read;
        }

        memcpy(buf, blk + i, 1);
        bytes_read++;
        buf++;
    }

    return bytes_read;
}

/* read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);
 * Inputs: uint32_t inode = inode number of file
           uint32_t offset = starting position to read
           uint8_t* buf = buffer to hold read data
           uint32_t length = number of bytes to read
 * Return Value: number of bytes read
 * Function: Fill in the buffer */
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length) {
    if (inode >= MAX_FILES) {
        return -1;
    }

    inode_t *node = (inode_t *) (inode_start + inode);
    uint32_t file_len_in_bytes = node->len;
    uint32_t start_blk_index = offset / BLOCK_SIZE;
    uint32_t start_byte_in_blk = offset % BLOCK_SIZE;
    uint32_t bytes_read = 0;

    if (length > file_len_in_bytes) {
        length = file_len_in_bytes;
    }

    // Read bytes in first block 
    uint32_t blk_num = node->data_blocks[start_blk_index]; // look up block # in inode
    uint8_t *blk = (uint8_t *) (data_block_start + blk_num * BLOCK_SIZE); // locate the first byte in that block

    bytes_read = read_block(blk, buf, start_byte_in_blk, length);

    if (bytes_read == length) {
        return bytes_read;
    } 

    // Read bytes in the remaining blocks
    int i;
    for (i = start_blk_index + 1; i < MAX_DATA_BLOCKS_PER_NODE; i++) {
        blk_num = node->data_blocks[i]; // look up block # in inode
        blk = (uint8_t *) (data_block_start + blk_num * BLOCK_SIZE); // locate the first byte in that block

        bytes_read += read_block(blk, buf + bytes_read, 0, length - bytes_read);

        if (bytes_read == length) {
            return bytes_read;
        }
    }

    return bytes_read;
}

/* dir_read(int32_t fd, void* buf, int32_t nbytes);
 * Inputs: uint32_t fd = file descriptor
           void* buf = buffer to hold read data
           uint32_t nbytes = number of bytes to read
 * Return Value: 0 for successful read
 * Function: Fill in the buffer by file name */
int32_t dir_read(int32_t fd, void* buf, int32_t nbytes) {
    pcb_t *curr_pcb = get_current_pcb();
    int8_t buffer[MAX_FILE_NAME + 1];

    if (curr_pcb->fd_array[fd].file_position > boot_block_start->num_dir_entries - 1) {
        return 0;
    }

    if (read_dentry_by_index(curr_pcb->fd_array[fd].file_position, &test_dentry) == 0) {
        strncpy(buf, test_dentry.file_name, MAX_FILE_NAME);
        strncpy(buffer, test_dentry.file_name, MAX_FILE_NAME);

        curr_pcb->fd_array[fd].file_position++;

        return strlen(buffer);
    } else {
        return 0;
    }
}



/* dir_write(int32_t fd, void* buf, int32_t nbytes);
 * Inputs: uint32_t fd = file descriptor
           void* buf = buffer to hold written data
           uint32_t nbytes = number of bytes to read
 * Return Value: -1 
 * Function: Write n bytes from buffer to file */
int32_t dir_write(int32_t fd, const void* buf, int32_t nbytes) {
    return -1;
}

/* dir_open(const uint8_t* dirname);
 * Inputs: dirname = file name
 * Return Value: 0 on success, -1 on failure
 * Function: Open a directory */
int32_t dir_open(const uint8_t* dirname) {
    if (read_dentry_by_name(dirname, &test_dentry) == 0) {
        return 0;
    } 

    return -1;
}

/* dir_close(int32_t fd);
 * Inputs: fd = file descriptor
 * Return Value: 0 
 * Function: Close a directory */
int32_t dir_close(int32_t fd) {
    return 0;
}

/* file_read(int32_t fd, void* buf, int32_t nbytes);
 * Inputs: uint32_t fd = file descriptor
           void* buf = buffer to hold read data
           uint32_t nbytes = number of bytes to read
 * Return Value: 0 for successful read
 * Function: Fill in the buffer by file data */
int32_t file_read(int32_t fd, void* buf, int32_t nbytes) {
    //read_data(fd, offset, buf, nbytes); 
    return 0;
}

/* file_write(int32_t fd, void* buf, int32_t nbytes);
 * Inputs: uint32_t fd = file descriptor
           void* buf = buffer to hold written data
           uint32_t nbytes = number of bytes to read
 * Return Value: -1 
 * Function: Write n bytes from buffer to file */
int32_t file_write(int32_t fd, const void* buf, int32_t nbytes) {
    return -1;
}

/* file_open(const uint8_t* dirname);
 * Inputs: dirname = file name
 * Return Value: 0 on success, -1 on failure
 * Function: Open a file */
int32_t file_open(const uint8_t* filename) { 
    if (read_dentry_by_name(filename, &test_dentry) == 0) {
        return 0;
    } 

    return -1;
}

/* file_close(int32_t fd);
 * Inputs: fd = file descriptor
 * Return Value: 0 
 * Function: Close a file */
int32_t file_close(int32_t fd) {
    return 0;
}
