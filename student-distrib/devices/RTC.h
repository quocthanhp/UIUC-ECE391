/* RTC.h - Defines used in interactions with the RTC chip 
* 
*
*/


#ifndef _RTC_H
#define _RTC_H
#include "../types.h"

/* rtc ports */
#define CMOS_CMD 0x70
#define CMOS_DATA 0x71
/* rtc regs */
#define STATUSA 0x8A
#define STATUSB 0x8B
#define STATUSC 0x0C

/* init RTC chip */

void RTC_INIT(void);

/* RTC freqency changing function */

void rtc_interrupt(void);

#endif

