#ifndef RTC_H
#define RTC_H

#include <lpc21xx.h>
#include "tpyes.h"

/* RTC clock configuration */
#define FOSC        12000000
#define CCLK        (5 * FOSC)
#define PCLK        (CCLK / 4)

#define PREINT_VAL  ((int)(PCLK / 32768) - 1)
#define PREFRAC_VAL (PCLK - ((PREINT_VAL + 1) * 32768))

/* RTC Control */
#define RTC_ENABLE  (1 << 0)
#define RTC_RESET   (1 << 1)
#define RTC_CLKSRC  (1 << 4)

/* Days */
#define SUN  0
#define MON  1
#define TUE  2
#define WED  3
#define THU  4
#define FRI  5
#define SAT  6

/* RTC functions */
void RTC_Init(void);

void SetRTCTimeInfo(u32 hour, u32 minute, u32 second);
void GetRTCTimeInfo(s32 *hour, s32 *minute, s32 *second);

void SetRTCDateInfo(u32 date, u32 month, u32 year);
void GetRTCDateInfo(s32 *date, s32 *month, s32 *year);

void SetRTCDay(u32 dow);
void GetRTCDay(s32 *dow);

#endif
