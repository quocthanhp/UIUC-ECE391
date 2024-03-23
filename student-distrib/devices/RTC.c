
#include "RTC.h"
#include "../i8259.h"
#include "../lib.h"

int8_t rtc_int_flag;
/* init rtc */
void RTC_INIT(void){
    // uint32_t rate = 0x10;
    
    enable_irq(8);
    rtc_int_flag = 0; // init flag to 0

    outb(STATUSB, CMOS_CMD);
    char prev = inb(CMOS_DATA);
    outb(STATUSB, CMOS_CMD);
    outb((prev) | 0x40, CMOS_DATA);
    outb(STATUSC, CMOS_CMD);
    inb(CMOS_DATA);
    
}

/* straight from osDEV */
/* DESC : set RTC default rate 1024hz
*  INPUT : void
*  OUTPUT : -1/0 
*/
/* ref: https://wiki.osdev.org/RTC */

// int8_t rtc_int_flag;  

void rtc_interrupt(void){
    /* irq is 8*/

    // outb(STATUSA, CMOS_CMD);
    // char prev = inb(CMOS_DATA);
    // outb(STATUSA, CMOS_CMD);
    // outb((prev & 0xF0) | rate, CMOS_DATA);
    rtc_int_flag = 1; // set flag

    outb(STATUSC, CMOS_CMD);
    inb(CMOS_DATA);
    printf('1');
    test_interrupts();
    send_eoi(8);
    
}

/* rtc_read
* I/O   I:  fd- file descripter
*           buf- 
*           nbytes- number of bytes read 
*       O:
*           0/-1 - success/fail
*   " For the real-time clock (RTC), this call should always return 0 "
*/
int32_t rtc_read(int32_t fd, void* buf, int32_t nbytes){
    rtc_int_flag = 0;
    while (rtc_int_flag != 1); // spin until rtc_interrupt sets the flag.
    return 0;
}
/* rtc_write  
* I/O   I:  fd- file descripter
*           buf- freq number?
*           nbytes- number of bytes 
*       O:
*           0/-1 - success/fail
In the case of the RTC, the system call should always accept only a 4-byte
integer specifying the interrupt rate in Hz, and should set the rate of 
periodic interrupts accordingly. Writes to regular files should always 
return -1 to indicate failure since the file system is read-only. 
The call returns the number of bytes written, or -1 on failure.
*/
int32_t rtc_write(int32_t fd, const void* buf, int32_t nbytes){
if(buf == NULL || nbytes != 4) return -1;  // if buff is null ptr or it is not 4 bytes return fail.
int setfrq = rtc_freq(*(int32_t*) buf);
return setfrq;

}

/* rtc_open  
* I/O   I: NONE
*       O: NONE
*           
RTC interrupt rate should be set to a default value of 2 Hz (2 interrupts per second) 
when the RTC device is opened.
*/
int32_t rtc_open(const uint8_t* filename){
    rtc_freq(2); // set to 2hz (lowest freq)
    return 0;
}

/* rtc_close  
* I/O   I: NONE
*       O: NONE
*/
int32_t rtc_close(int32_t fd){ return 0;}

/* rtc_freq
*       I:  freq - the frequency for RTC to be set
*
*       O:  
*           0/-1 - success/fail
*   ref: https://wiki.osdev.org/RTC
*....................................NOT DONE............................
*/ 
int32_t rtc_freq(int32_t freq){
    // check max min and power of 2
    if(freq < 2 || freq > 1024 || ((freq % 2) != 0)) return -1; // sanity check the input
    freq &= 0x0F;                                               // freq must be between 2 and 1024


    outb(STATUSA, CMOS_CMD);
    char prev = inb(CMOS_DATA);
    outb(STATUSA, CMOS_CMD);
    outb(((prev & 0xF0) | freq), CMOS_DATA);
    return 0;
}