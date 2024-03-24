#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "fs.h"

#define PASS 1
#define FAIL 0

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15");
}


/* Checkpoint 1 tests */

/* IDT Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test(){
	TEST_HEADER;

	int i;
	int result = PASS;
	for (i = 0; i < 10; ++i){
		if ((idt[i].offset_15_00 == NULL) && 
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}

	return result;
}

// add more tests here

int divide_by_zero_test(){
	TEST_HEADER;
	int a = 0;
	int b;
	b = 1 / a;

	return(FAIL);

}

// int dereferenced_null_pointer_test(){
// 	TEST_HEADER;
// 	uint8_t isnull;
// 	uint8_t *nullptr = NULL;
// 	isnull = *nullptr;

// 	return FAIL;
// }

/* Checkpoint 2 tests */

/* File System Driver Test 
 * 
 * Read all file names in file system
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: read_dentry_by_name, read_dentry_by_index, dir_read
 * Files: fs.h/c
 */
int dir_read_test() {
	TEST_HEADER;
	clear();

	inode_t *curr_node;
	uint8_t buf[MAX_FILE_NAME + 1];
	int i;

	// for (i = 0; i < boot_block_start->num_dir_entries; i++) {
	// 	curr_dentry = boot_block_start->dentries[i];
	// 	curr_node = (inode_t *) inode_start + curr_dentry.inode_num;
		
	// 	if (read_dentry_by_name(curr_dentry.file_name, &my_dentry) == 0) {
	// 		strncpy(buf, my_dentry.file_name, MAX_FILE_NAME);
	// 		buf[MAX_FILE_NAME] = '\0';
	// 		printf("file_name: %s, file_type: %d, file_size: %d\n", buf, my_dentry.file_type, curr_node->len);
	// 	} else {
	// 		printf("fail to read %s!\n", curr_dentry.file_name, strlen(curr_dentry.file_name));
	// 		return FAIL;
	// 	}
	// }

	for (i = 0; i < boot_block_start->num_dir_entries; i++) {
		if (dir_read(0, buf, MAX_FILE_NAME) == 0) {
			curr_node = (inode_t *) inode_start + test_dentry.inode_num;
			buf[MAX_FILE_NAME] = '\0'; // for very large txt file  

			printf("file_name: %s, file_type: %d, file_size: %d\n", buf, test_dentry.file_type, curr_node->len);
		} 
		else {
			return FAIL;
		}
	}

	return PASS;
}

int read_data_small_file_test() {
	TEST_HEADER;
	clear();

	dentry_t my_dentry;
	uint32_t frame0_inode; 
	uint8_t frame0_size;

	read_dentry_by_name((const uint8_t *) "frame0.txt", &my_dentry);
	frame0_inode = my_dentry.inode_num;
	frame0_size = ((inode_t *) inode_start + my_dentry.inode_num)->len;

	// Read data
	uint8_t buf[frame0_size];
	int32_t bytes_read = read_data(frame0_inode, 0, buf, frame0_size);

	if (bytes_read != frame0_size) {
		return FAIL;
	}

	printf( (int8_t *) buf);
	printf("file_name: frame0.txt\n");
	return PASS;
}

int read_data_exec_file_test() {
	TEST_HEADER;
	clear();

	// dentry_t my_dentry;
	// uint32_t grep_inode; 
	// uint8_t grep_size;

	// read_dentry_by_name((const uint8_t *) "grep", &my_dentry);
	// grep_inode = my_dentry.inode_num;
	// grep_size = ((inode_t *) inode_start + my_dentry.inode_num)->len;

	uint32_t grep_inode = 50;
	uint32_t grep_size = 6149;
	uint8_t buf[grep_size];
	int32_t bytes_read = read_data(grep_inode, 0, buf, grep_size);

	if (bytes_read != grep_size) {
		return FAIL;
	}

	int i;
	for (i = 0; i < bytes_read; i++) {
		putc(buf[i]);
	}
	// printf( "%s\n", buf);
	printf("\nfile_name: grep\n");
	return PASS;
}

int read_data_large_file_test() {
	TEST_HEADER;
	clear();

	dentry_t my_dentry;
	uint32_t verylargefile_inode;
	uint32_t verylargefile_size;

	read_dentry_by_name((const uint32_t *) "verylargetextwithverylongname.tx", &my_dentry);
	verylargefile_inode = my_dentry.inode_num;
	verylargefile_size = ((inode_t *) inode_start + my_dentry.inode_num)->len;

	// uint32_t verylargefile_inode = 44;
	// uint32_t verylargefile_size = 5277;
	uint8_t buf[verylargefile_size];
	int32_t bytes_read = read_data(verylargefile_inode, 0, buf, verylargefile_size);

	if (bytes_read != verylargefile_size) {
		return FAIL;
	}
	//printf("bytes read %d\n", bytes_read);
	int i;
	for (i = 0; i < bytes_read; i++) {
		putc(buf[i]);
	}
	// printf( "%s\n", buf);
	return PASS;
}

/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(){
	// TEST_OUTPUT("idt_test", idt_test());
	// TEST_OUTPUT("divide by zero test", divide_by_zero_test());
	// TEST_OUTPUT("rtc_test", test_interrupts());
	//TEST_OUTPUT("dereference null test", dereferenced_null_pointer_test());
	// launch your tests here
	
	// TEST_OUTPUT("dir_read_test", dir_read_test());
	
	// TEST_OUTPUT("read_data_small_file_test", read_data_small_file_test());
	
	//TEST_OUTPUT("read_data_exec_file_test", read_data_exec_file_test());
	//read_data_exec_file_test();
	
	TEST_OUTPUT("test", read_data_large_file_test());
	//read_data_large_file_test();
}
