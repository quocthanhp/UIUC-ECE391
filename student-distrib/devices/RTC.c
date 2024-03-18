
#include "RTC.h"
#include "../i8259.h"
#include "../lib.h"



/* straight from osDEV */
/* DESC : Init the RTC default rate 1024hz
*  INPUT : void
*  OUTPUT : -1/0 
*/
/* ref: https://wiki.osdev.org/RTC */
uint32_t RTC_INIT(void){
    /* irq is 8*/
    uint32_t rate = 1024;
    disable_irq(8);

    outb(STATUSA, CMOS_CMD);
    char prev = inb(CMOS_DATA);
    outb(STATUSA, CMOS_CMD);
    outb((prev & 0xF0) | rate, CMOS_DATA);

    enable_irq(8);
    return 0;
}
