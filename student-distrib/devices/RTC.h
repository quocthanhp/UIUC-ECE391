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
#define STATUSC 0x8C

/* init RTC chip */
void RTC_INIT(void);

/* RTC freqency changing function */
void rtc_interrupt(void);

/* Cp2 syscalls RTC driver*/
int32_t rtc_read(int32_t fd, void* buf, int32_t nbytes);
int32_t rtc_write(int32_t fd, const void* buf, int32_t nbytes);
int32_t rtc_open(const uint8_t* filename);
int32_t rtc_close(int32_t fd);
/* chnage frequencey */
int32_t rtc_freq(int32_t freq);

#endif

