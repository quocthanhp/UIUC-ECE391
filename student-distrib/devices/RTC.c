#include "RTC.h"
#include "i8259.h"
#include "lib.h"



/* straight from osDEV */
uint32_t RTC_INIT(void){
    disable_irq(8);

    outb(STATUSB, CMOS_CMD);
    char prev = inb(CMOS_DATA);
    outb(STATUSB, CMOS_CMD);
    outb(prev | 0x40, CMOS_DATA);

    enable_irq(8);
    return 0;
}
