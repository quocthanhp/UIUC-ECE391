
#include "RTC.h"
#include "../i8259.h"
#include "../lib.h"

/* init rtc */
void RTC_INIT(void){
    // uint32_t rate = 0x10;

    enable_irq(8);
    
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
void rtc_interrupt(void){
    /* irq is 8*/
    // uint32_t rate = 0x10;
    // cli();
    
    // // disable_irq(8);

    // outb(STATUSA, CMOS_CMD);
    // char prev = inb(CMOS_DATA);
    // outb(STATUSA, CMOS_CMD);
    // outb((prev & 0xF0) | rate, CMOS_DATA);

    //maybe needed?
    //confirm wtih TA
    outb(STATUSC, CMOS_CMD);
    inb(CMOS_DATA);
    printf('1');
    test_interrupts();
    // enable_irq(8);
    send_eoi(8);
    // sti();
}


