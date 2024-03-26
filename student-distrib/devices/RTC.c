
#include "RTC.h"
#include "../i8259.h"
#include "../lib.h"

int8_t rtc_int_flag;
// int32_t count = 0;
int32_t freq = 0;
// int32_t temp = 16; 
/* init rtc */
void RTC_INIT(void){
    // uint32_t rate = 0x10;
    
    
    rtc_int_flag = 0; // init flag to 0

    outb(STATUSB, CMOS_CMD);
    char prev = inb(CMOS_DATA);
    outb(STATUSB, CMOS_CMD);
    outb((prev) | 0x40, CMOS_DATA);
    // outb(STATUSC, CMOS_CMD);
    // inb(CMOS_DATA);

    outb(STATUSA, CMOS_CMD);
    prev = inb(CMOS_DATA);
    freq = 15 & 0x0F;
    outb(STATUSA, CMOS_CMD);
    outb((prev & 0xF0) | freq, CMOS_DATA);
    enable_irq(8);
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
    outb(STATUSA, CMOS_CMD);
    outb(STATUSB, CMOS_CMD);
    outb(STATUSC, CMOS_CMD);

    char prev = inb(CMOS_DATA);
    send_eoi(8);

    // outb((prev / 0xF0) | rate, CMOS_DATA);

    // if(count != temp) count++;
    // if(count == temp) {
    //     count = 0;
        
    // }

    // outb(STATUSC, CMOS_CMD);
    // inb(CMOS_DATA);
    // putc((uint8_t *) '1');
    //test_interrupts();
    
}

/* rtc_read
* I/O   I:  fd- 
*           buf- 
*           nbytes- 
*       O:
*           0/-1 - success/fail
*   " For the real-time clock (RTC), this call should always return 0 "
*/
int32_t rtc_read(int32_t fd, void* buf, int32_t nbytes){
    rtc_int_flag = 0;
    while (!rtc_int_flag){continue;} // spin until rtc_interrupt sets the flag.
    rtc_int_flag = 0; 
    return 0;
}
/* rtc_write  
* I/O   I:  fd- 
*           buf- freq number?
*           nbytes- number of bytes 
*       O:
*           0/-1 - success/fail
In the case of the RTC, the system call should always accept only a 4-byte
integer specifying the interrupt rate in Hz, and should set the rate of 
periodic interrupts accordingly. 
*
*/
int32_t rtc_write(int32_t fd, const void* buf, int32_t nbytes){
    
    if(2 > fd  || nbytes != 4 || fd > 1024)  return -1;  // if fd is out of bunds or it is not 4 bytes return fail.
    // freq = fd;
    // temp = 1024/freq;
    rtc_freq(fd);
    return nbytes;
}

/* rtc_open  
* I/O   I: NONE
*       O: NONE
*           
RTC interrupt rate should be set to a default value of 2 Hz (2 interrupts per second) 
when the RTC device is opened.
*/
int32_t rtc_open(const uint8_t* filename){
    // temp = 2;
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
*
*/ 
int32_t rtc_freq(int32_t freq){
    // check max min and power of 2 freq must be between 2 and 1024
    if(freq < 2 || freq > 1024 || ((freq % 2) != 0)) return -1; 
    char temp;
    switch (freq)
    {
    case 2:
        temp = 0x0F;
        break;
    case 4:
        temp = 0x0E;
        break;
    case 8:
        temp = 0x0D;
        break;
    case 16:
        temp = 0x0C;
        break;
    case 32:
        temp = 0x0B;
        break;
    case 64:
        temp = 0x0A;
        break;
    case 128:
        temp = 0x09;
        break;
    case 256:
        temp = 0x08;
        break;
    case 512:
        temp = 0x07;
        break;
    case 1024:
        temp = 0x06;
        break;
    
    default:
        break;
    }
    // if(freq == 2) temp = 0x0F;                                          
    // if(freq == 4) temp = 0x0E;                                          
    // if(freq == 8) temp = 0x0D;                                          
    // if(freq == 16) temp = 0x0C;                                          
    // if(freq == 32) temp = 0x0B;                                          
    // if(freq == 64) temp = 0x0A;                                          
    // if(freq == 128) temp = 0x09;                                          
    // if(freq == 256) temp = 0x08; 
    // if(freq == 512) temp = 0x07;                                         
    // if(freq == 1024) temp = 0x06;
    outb(STATUSA, CMOS_CMD);
    char prev = inb(CMOS_DATA);
    // freq = 15 & 0x0F;
    outb(STATUSA, CMOS_CMD);
    outb((prev & 0xF0) | temp, CMOS_DATA);
    // outb(STATUSA, CMOS_CMD);
    // char prev = inb(CMOS_DATA);
    // outb(STATUSA, CMOS_CMD);
    // outb((prev | temp), CMOS_DATA);
    return 0;
}
