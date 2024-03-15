
#include "RTC.h"
#include "i8259.h"
#include "lib.h"



/* straight from osDEV */
/* DESC : Init the RTC default rate 1024hz
*  INPUT : void
*  OUTPUT : -1/0 
*/
uint32_t RTC_INIT(void){
    /* irq is 8*/
    disable_irq(8);

    outb(STATUSB, CMOS_CMD);
    char prev = inb(CMOS_DATA);
    outb(STATUSB, CMOS_CMD);
    outb(prev | 0x40, CMOS_DATA);

    enable_irq(8);
    return 0;
}
