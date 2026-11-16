#include <lpc21xx.h>
#include "tpyes.h"
#include "RTC.h"

/*
   RTC INITIALIZATION
   LPC2129
*/

void RTC_Init(void)
{
    /* Disable and reset RTC */
    CCR = RTC_RESET;

    /* Set RTC prescaler */
    PREINT  = PREINT_VAL;
    PREFRAC = PREFRAC_VAL;

    /* Enable RTC */
    CCR = RTC_ENABLE;
}


/*------------------------------------------------
             SET TIME
------------------------------------------------*/

void SetRTCTimeInfo(u32 hour, u32 minute, u32 second)
{
    HOUR = hour;
    MIN  = minute;
    SEC  = second;
}


/*------------------------------------------------
             GET TIME
------------------------------------------------*/

void GetRTCTimeInfo(s32 *hour, s32 *minute, s32 *second)
{
    *hour   = HOUR;
    *minute = MIN;
    *second = SEC;
}


/*------------------------------------------------
             SET DATE
------------------------------------------------*/

void SetRTCDateInfo(u32 date, u32 month, u32 year)
{
    DOM   = date;
    MONTH = month;
    YEAR  = year;
}


/*------------------------------------------------
             GET DATE
------------------------------------------------*/

void GetRTCDateInfo(s32 *date, s32 *month, s32 *year)
{
    *date  = DOM;
    *month = MONTH;
    *year  = YEAR;
}


/*------------------------------------------------
             SET DAY
------------------------------------------------*/

void SetRTCDay(u32 dow)
{
    DOW = dow;
}


/*------------------------------------------------
             GET DAY
------------------------------------------------*/

void GetRTCDay(s32 *dow)
{
    *dow = DOW;
}
