/* RTC.h - Defines used in interactions with the RTC chip 
* 
*
*/


#ifndef _RTC_H
#define _RTC_H
#include "types.h"


#define CMOS_CMD 0x70
#define CMOS_DATA 0x071

#define STATUSA 0x8A
#define STATUSB 0x8B

/* init RTC chip */
uint32_t RTC_INIT(void);

#endif