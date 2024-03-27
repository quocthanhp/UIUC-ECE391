#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "devices/RTC.h"
#include "devices/keyboard.h"
#include "terminal.h"

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
/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(){
	// TEST_OUTPUT("idt_test", idt_test());
	// TEST_OUTPUT("divide by zero test", divide_by_zero_test());
	// TEST_OUTPUT("rtc_test", test_interrupts());
	// TEST_OUTPUT("dereference null test", dereferenced_null_pointer_test());
	//  rtc_freq_test();
	// terminal_tests();
	// launch your tests here
}
