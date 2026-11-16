#include <LPC21xx.h>

#include "tpyes.h"
#include "delay.h"
#include "traffic.h"
#include "RTC.h"
#include "LCD.h"
#include "KPM.h"
#include "LCD_defines.h"


/*==================================================
              TRAFFIC LED CONNECTION
==================================================*/

#define D1_RED       14
#define D1_YELLOW    15
#define D1_GREEN     16

#define D2_RED       17
#define D2_YELLOW    18
#define D2_GREEN     19

#define D3_RED       20
#define D3_YELLOW    21
#define D3_GREEN     22

#define D4_RED       23
#define D4_YELLOW    24
#define D4_GREEN     25


#define TRAFFIC_MASK  (0xFFF << 14)


/*==================================================
                  TRAFFIC INIT
==================================================*/

void Traffic_Init(void)
{
    IODIR0 |= TRAFFIC_MASK;

    IOCLR0 = TRAFFIC_MASK;
}


/*==================================================
                    ALL RED
==================================================*/

void Traffic_All_Red(void)
{
    IOCLR0 = TRAFFIC_MASK;

    IOSET0 =
          (1 << D1_RED)
        | (1 << D2_RED)
        | (1 << D3_RED)
        | (1 << D4_RED);
}


/*==================================================
                 DISPLAY CURRENT TIME
==================================================*/

static void Display_Time(
    u8 mode,
    u8 dir,
    u8 state)
{
    s32 hour;
    s32 min;
    s32 sec;


    GetRTCTimeInfo(
        &hour,
        &min,
        &sec);


    /* LINE 1 */

    WRITE_LCD_CMD(GOTO_LINE1_POS0);

    StrLCD((u8 *)"TIME ");


    WRITE_LCD_DATA(
        (hour / 10) + '0');

    WRITE_LCD_DATA(
        (hour % 10) + '0');

    WRITE_LCD_DATA(':');


    WRITE_LCD_DATA(
        (min / 10) + '0');

    WRITE_LCD_DATA(
        (min % 10) + '0');

    WRITE_LCD_DATA(':');


    WRITE_LCD_DATA(
        (sec / 10) + '0');

    WRITE_LCD_DATA(
        (sec % 10) + '0');


    StrLCD((u8 *)"    ");


    /* LINE 2 */

    WRITE_LCD_CMD(GOTO_LINE2_POS0);


    if(mode == NORMAL_MODE)
    {
        StrLCD((u8 *)"NORMAL ");
    }

    else if(mode == PEAK_MODE)
    {
        StrLCD((u8 *)"PEAK   ");
    }

    else if(mode == NIGHT_MODE)
    {
        StrLCD((u8 *)"NIGHT  ");
    }

    else if(mode == CUSTOM1_MODE)
    {
        StrLCD((u8 *)"CUSTOM1");
    }

    else
    {
        StrLCD((u8 *)"CUSTOM2");
    }


    StrLCD((u8 *)" D");

    WRITE_LCD_DATA(
        dir + '0');


    if(state == 1)
    {
        StrLCD((u8 *)" GREEN");
    }

    else
    {
        StrLCD((u8 *)" YELLOW");
    }
}


/*==================================================
                  TIME TO MINUTES
==================================================*/

static u32 Time_To_Min(
    u32 hour,
    u32 min)
{
    return ((hour * 60) + min);
}


/*==================================================
                 CHECK TIME WINDOW
==================================================*/

static u8 Time_In_Window(
    u32 current,
    u32 start,
    u32 stop)
{
    /*
       Same time = 24 hour operation
    */

    if(start == stop)
    {
        return 1;
    }


    /*
       Normal window

       Example:
       08:00 -> 18:00
    */

    if(start < stop)
    {
        if((current >= start) &&
           (current < stop))
        {
            return 1;
        }

        return 0;
    }


    /*
       Overnight window

       Example:
       22:00 -> 06:00
    */

    if((current >= start) ||
       (current < stop))
    {
        return 1;
    }


    return 0;
}


/*==================================================
                 CHECK MAIN SCHEDULE
==================================================*/

static u8 Check_Main_Schedule(
    u32 start_hour,
    u32 start_min,
    u32 stop_hour,
    u32 stop_min)
{
    s32 hour;
    s32 min;
    s32 sec;

    u32 current;
    u32 start;
    u32 stop;


    GetRTCTimeInfo(
        &hour,
        &min,
        &sec);


    current =
        Time_To_Min(
            (u32)hour,
            (u32)min);


    start =
        Time_To_Min(
            start_hour,
            start_min);


    stop =
        Time_To_Min(
            stop_hour,
            stop_min);


    return Time_In_Window(
        current,
        start,
        stop);
}


/*==================================================
                  GET ACTIVE MODE
==================================================*/

static u8 Get_Active_Mode(
    u32 c1_enable,
    u32 c1_start_hour,
    u32 c1_start_min,
    u32 c1_stop_hour,
    u32 c1_stop_min,

    u32 c2_enable,
    u32 c2_start_hour,
    u32 c2_start_min,
    u32 c2_stop_hour,
    u32 c2_stop_min)
{
    s32 hour;
    s32 min;
    s32 sec;

    u32 current;


    GetRTCTimeInfo(
        &hour,
        &min,
        &sec);


    current =
        Time_To_Min(
            (u32)hour,
            (u32)min);


    /*================================================
                       CUSTOM 1
    ================================================*/

    if(c1_enable == 1)
    {
        if(Time_In_Window(
            current,

            Time_To_Min(
                c1_start_hour,
                c1_start_min),

            Time_To_Min(
                c1_stop_hour,
                c1_stop_min)))
        {
            return CUSTOM1_MODE;
        }
    }


    /*================================================
                       CUSTOM 2
    ================================================*/

    if(c2_enable == 1)
    {
        if(Time_In_Window(
            current,

            Time_To_Min(
                c2_start_hour,
                c2_start_min),

            Time_To_Min(
                c2_stop_hour,
                c2_stop_min)))
        {
            return CUSTOM2_MODE;
        }
    }


    /*================================================
                       NORMAL
                     06:00-17:00
    ================================================*/

    if(Time_In_Window(
        current,
        Time_To_Min(6,0),
        Time_To_Min(17,0)))
    {
        return NORMAL_MODE;
    }


    /*================================================
                        PEAK
                     17:00-22:00
    ================================================*/

    if(Time_In_Window(
        current,
        Time_To_Min(17,0),
        Time_To_Min(22,0)))
    {
        return PEAK_MODE;
    }


    /*================================================
                       NIGHT
                     22:00-06:00
    ================================================*/

    return NIGHT_MODE;
}


/*==================================================
                 GET GREEN TIME
==================================================*/

static u32 Get_Green_Time(
    u8 mode,
    u32 c1_green,
    u32 c2_green)
{
    if(mode == NORMAL_MODE)
    {
        return NORMAL_GREEN;
    }

    if(mode == PEAK_MODE)
    {
        return PEAK_GREEN;
    }

    if(mode == NIGHT_MODE)
    {
        return NIGHT_GREEN;
    }

    if(mode == CUSTOM1_MODE)
    {
        return c1_green;
    }

    return c2_green;
}


/*==================================================
                GET YELLOW TIME
==================================================*/

static u32 Get_Yellow_Time(
    u8 mode,
    u32 c1_yellow,
    u32 c2_yellow)
{
    if(mode == NORMAL_MODE)
    {
        return NORMAL_YELLOW;
    }

    if(mode == PEAK_MODE)
    {
        return PEAK_YELLOW;
    }

    if(mode == NIGHT_MODE)
    {
        return NIGHT_YELLOW;
    }

    if(mode == CUSTOM1_MODE)
    {
        return c1_yellow;
    }

    return c2_yellow;
}


/*==================================================
                  CHECK C KEY
==================================================*/

static u8 Check_C_Key(void)
{
    u8 key;


    if(ColScan() == 0)
    {
        key = KeyScan();


        if(key == 'C')
        {
            Traffic_All_Red();

            return 1;
        }
    }


    return 0;
}


/*==================================================
                   ONE SECOND
==================================================*/

static u8 One_Second(void)
{
    if(Check_C_Key())
    {
        return 1;
    }


    delay_s(1);


    if(Check_C_Key())
    {
        return 1;
    }


    return 0;
}


/*==================================================
                   SET GREEN
==================================================*/

static void Set_Green(
    u8 dir)
{
    Traffic_All_Red();


    if(dir == 1)
    {
        IOCLR0 = (1 << D1_RED);
        IOSET0 = (1 << D1_GREEN);
    }

    else if(dir == 2)
    {
        IOCLR0 = (1 << D2_RED);
        IOSET0 = (1 << D2_GREEN);
    }

    else if(dir == 3)
    {
        IOCLR0 = (1 << D3_RED);
        IOSET0 = (1 << D3_GREEN);
    }

    else
    {
        IOCLR0 = (1 << D4_RED);
        IOSET0 = (1 << D4_GREEN);
    }
}


/*==================================================
                  SET YELLOW
==================================================*/

static void Set_Yellow(
    u8 dir)
{
    Traffic_All_Red();


    if(dir == 1)
    {
        IOCLR0 = (1 << D1_RED);
        IOSET0 = (1 << D1_YELLOW);
    }

    else if(dir == 2)
    {
        IOCLR0 = (1 << D2_RED);
        IOSET0 = (1 << D2_YELLOW);
    }

    else if(dir == 3)
    {
        IOCLR0 = (1 << D3_RED);
        IOSET0 = (1 << D3_YELLOW);
    }

    else
    {
        IOCLR0 = (1 << D4_RED);
        IOSET0 = (1 << D4_YELLOW);
    }
}


/*==================================================
                 RUN PHASE
==================================================*/

static u8 Run_Phase(
    u8 mode,
    u8 dir,
    u8 state,
    u32 time)
{
    u32 i;


    for(i = 0; i < time; i++)
    {
        Display_Time(
            mode,
            dir,
            state);


        if(One_Second())
        {
            return 1;
        }
    }


    return 0;
}


/*==================================================
                     AUTO MODE
==================================================*/

void Auto_Mode(
    u32 start_hour,
    u32 start_min,
    u32 stop_hour,
    u32 stop_min,

    u32 c1_enable,
    u32 c1_start_hour,
    u32 c1_start_min,
    u32 c1_stop_hour,
    u32 c1_stop_min,
    u32 c1_green,
    u32 c1_yellow,

    u32 c2_enable,
    u32 c2_start_hour,
    u32 c2_start_min,
    u32 c2_stop_hour,
    u32 c2_stop_min,
    u32 c2_green,
    u32 c2_yellow)
{
    u8 mode;

    u32 green_time;
    u32 yellow_time;


    while(1)
    {
        /*==========================================
                 CHECK MAIN TRAFFIC WINDOW
        ==========================================*/

        if(Check_Main_Schedule(
            start_hour,
            start_min,
            stop_hour,
            stop_min) == 0)
        {
            Traffic_All_Red();


            WRITE_LCD_CMD(
                GOTO_LINE1_POS0);

            StrLCD(
                (u8 *)"TRAFFIC STOPPED");


            WRITE_LCD_CMD(
                GOTO_LINE2_POS0);

            StrLCD(
                (u8 *)"WAITING       ");


            if(Check_C_Key())
            {
                return;
            }


            delay_s(1);

            continue;
        }


        /*==========================================
                    FIND ACTIVE MODE
        ==========================================*/

        mode =
            Get_Active_Mode(
                c1_enable,
                c1_start_hour,
                c1_start_min,
                c1_stop_hour,
                c1_stop_min,

                c2_enable,
                c2_start_hour,
                c2_start_min,
                c2_stop_hour,
                c2_stop_min);


        /*==========================================
                    GET TIMINGS
        ==========================================*/

        green_time =
            Get_Green_Time(
                mode,
                c1_green,
                c2_green);


        yellow_time =
            Get_Yellow_Time(
                mode,
                c1_yellow,
                c2_yellow);


        /*==========================================
                     DIRECTION 1
        ==========================================*/

        Set_Green(1);


        if(Run_Phase(
            mode,
            1,
            1,
            green_time))
        {
            return;
        }


        Set_Yellow(1);


        if(Run_Phase(
            mode,
            1,
            2,
            yellow_time))
        {
            return;
        }


        /*==========================================
                     DIRECTION 2
        ==========================================*/

        Set_Green(2);


        if(Run_Phase(
            mode,
            2,
            1,
            green_time))
        {
            return;
        }


        Set_Yellow(2);


        if(Run_Phase(
            mode,
            2,
            2,
            yellow_time))
        {
            return;
        }


        /*==========================================
                     DIRECTION 3
        ==========================================*/

        Set_Green(3);


        if(Run_Phase(
            mode,
            3,
            1,
            green_time))
        {
            return;
        }


        Set_Yellow(3);


        if(Run_Phase(
            mode,
            3,
            2,
            yellow_time))
        {
            return;
        }


        /*==========================================
                     DIRECTION 4
        ==========================================*/

        Set_Green(4);


        if(Run_Phase(
            mode,
            4,
            1,
            green_time))
        {
            return;
        }


        Set_Yellow(4);


        if(Run_Phase(
            mode,
            4,
            2,
            yellow_time))
        {
            return;
        }
    }
}


/*==================================================
                   MANUAL MODE
==================================================*/

void Manual_Mode(void)
{
    u8 key;


    Traffic_All_Red();


    while(1)
    {
        if(ColScan() == 0)
        {
            key = KeyScan();


            /* DIRECTION 1 */

            if(key == '1')
            {
                Traffic_All_Red();

                IOCLR0 =
                    (1 << D1_RED);

                IOSET0 =
                    (1 << D1_GREEN);
            }


            /* DIRECTION 2 */

            else if(key == '2')
            {
                Traffic_All_Red();

                IOCLR0 =
                    (1 << D2_RED);

                IOSET0 =
                    (1 << D2_GREEN);
            }


            /* DIRECTION 3 */

            else if(key == '3')
            {
                Traffic_All_Red();

                IOCLR0 =
                    (1 << D3_RED);

                IOSET0 =
                    (1 << D3_GREEN);
            }


            /* DIRECTION 4 */

            else if(key == '4')
            {
                Traffic_All_Red();

                IOCLR0 =
                    (1 << D4_RED);

                IOSET0 =
                    (1 << D4_GREEN);
            }


            /* ALL RED */

            else if(key == '5')
            {
                Traffic_All_Red();
            }


            /* EXIT */

            else if(key == 'C')
            {
                Traffic_All_Red();

                return;
            }
        }


        delay_ms(100);
    }
}
