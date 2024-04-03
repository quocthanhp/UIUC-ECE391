#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "devices/RTC.h"
#include "devices/keyboard.h"
#include "terminal.h"
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
int rtc_freq_test(){
	TEST_HEADER;
	int32_t test = 4;
	int32_t test2 = 16;
	int32_t test3 = 128;
	int32_t test4 = 1024;
	int32_t nbytes = 4; 
	uint8_t *buf;

	int i;
	
	clear();
	
	rtc_write(test, buf, nbytes); 
	for(i = 0; i < 10; i++ ){
		rtc_read(test, buf, nbytes);
		printf(( int8_t *) "1");
	}

	printf((int8_t *) "\n");
	 rtc_write(test2, buf, nbytes);
	for(i = 0; i < 20; i++ ){
		rtc_read(test2, buf, nbytes);
		printf(( int8_t *) "1");
	}
	
	printf(( int8_t *) "\n");
	 rtc_write(test3, buf, nbytes);
	for(i = 0; i < 80; i++ ){
		rtc_read(test3, buf, nbytes);
		printf((int8_t *) "1");
	}

	printf(( int8_t *) "\n");
	 rtc_write(test4, buf, nbytes);
	for(i = 0; i < 80; i++ ){
		rtc_read(test4, buf, nbytes);
		printf(( int8_t *) "1");
	}
	// clear();
	// return(PASS);
	return 1;
}

int terminal_tests(){

	terminal_open((uint8_t * )1);
	unsigned char buf[KEYBOARD_BUFFER_SIZE];
	int i;
	int32_t bytes_written;
	for (i = 0; i < KEYBOARD_BUFFER_SIZE; i++){
        buf[i] = '\0';    //setting the static terminal buffer to be null characters
    }

	int32_t nbytes = KEYBOARD_BUFFER_SIZE;
	while(1){
		int32_t bytes_read = terminal_read( 1 , buf , nbytes );
				bytes_written = terminal_write( 1 , buf , bytes_read );
	}
	
	
	return bytes_written;
}

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

/* File System Driver Test 
 * 
 * Read data from small file
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: read_data
 * Files: fs.h/c
 */
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

/* File System Driver Test 
 * 
 * Read data from executable file
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: read_data
 * Files: fs.h/c
 */
int read_data_exec_file_test() {
	TEST_HEADER;
	clear();

	dentry_t my_dentry;
	uint32_t grep_inode; 
	uint32_t grep_size;

	read_dentry_by_name((const uint8_t *) "grep", &my_dentry);
	grep_inode = my_dentry.inode_num;
	grep_size = ((inode_t *) inode_start + my_dentry.inode_num)->len;

	uint8_t buf[grep_size];
	int32_t bytes_read = read_data(grep_inode, 0, buf, grep_size);

	if (bytes_read != grep_size) {
		return FAIL;
	}

	int i;
	for (i = 0; i < bytes_read; i++) {
		if (buf[i] != '\0') {
			putc(buf[i]);
		}
	}
	
	printf("\nfile_name: grep\n");
	return PASS;
}

/* File System Driver Test 
 * 
 * Read data from large file
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: read_data
 * Files: fs.h/c
 */
int read_data_large_file_test() {
	TEST_HEADER;
	clear();

	dentry_t my_dentry;
	uint32_t verylargefile_inode;
	uint32_t verylargefile_size;

	read_dentry_by_name((const uint8_t *) "verylargetextwithverylongname.tx", &my_dentry);
	verylargefile_inode = my_dentry.inode_num;
	verylargefile_size = ((inode_t *) inode_start + my_dentry.inode_num)->len;

	uint8_t buf[verylargefile_size];
	int32_t bytes_read = read_data(verylargefile_inode, 0, buf, verylargefile_size);

	if (bytes_read != verylargefile_size) {
		return FAIL;
	}

	int i;
	for (i = 0; i < bytes_read; i++) {
		putc(buf[i]);	
	}

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
	// TEST_OUTPUT("dereference null test", dereferenced_null_pointer_test());
	 rtc_freq_test();
	// terminal_tests();
	// launch your tests here

	//TEST_OUTPUT("dir_read_test", dir_read_test());
	//TEST_OUTPUT("read_data_small_file_test", read_data_small_file_test());
	//TEST_OUTPUT("read_data_exec_file_test", read_data_exec_file_test());
	// TEST_OUTPUT("read_data_large_file_test", read_data_large_file_test());

}
