#include <LPC21xx.h>

#include "LCD_defines.h"
#include "tpyes.h"
#include "LCD.h"
#include "KPM.h"
#include "RTC.h"
#include "delay.h"
#include "traffic.h"


/*==================================================
              MAIN TRAFFIC SETTINGS
==================================================*/

/*
   Default:

   START = 00:00
   STOP  = 00:00

   00:00 -> 00:00 means 24 hour operation.
*/

u32 start_hour = 0;
u32 start_min  = 0;

u32 stop_hour  = 0;
u32 stop_min   = 0;


/*==================================================
                     CUSTOM 1
==================================================*/

u32 c1_enable = 0;

u32 c1_start_hour = 10;
u32 c1_start_min  = 0;

u32 c1_stop_hour  = 12;
u32 c1_stop_min   = 0;

u32 c1_green  = 15;
u32 c1_yellow = 3;


/*==================================================
                     CUSTOM 2
==================================================*/

u32 c2_enable = 0;

u32 c2_start_hour = 18;
u32 c2_start_min  = 30;

u32 c2_stop_hour  = 20;
u32 c2_stop_min   = 0;

u32 c2_green  = 12;
u32 c2_yellow = 3;


/*==================================================
                DISPLAY TWO DIGITS
==================================================*/

void Display2Digit(u32 value)
{
    WRITE_LCD_DATA(
        (value / 10) + '0');

    WRITE_LCD_DATA(
        (value % 10) + '0');
}


/*==================================================
                  READ NUMBER
==================================================*/

/*
   DIGITS : 0-9
   ENTER  : =
   CANCEL : C
*/

u32 Read_Number(void)
{
    u8 key;

    u32 value = 0;
    u32 count = 0;


    while(1)
    {
        while(ColScan());


        key = KeyScan();


        /* DIGIT */

        if((key >= '0') &&
           (key <= '9'))
        {
            if(count < 2)
            {
                value =
                    (value * 10) +
                    (key - '0');

                WRITE_LCD_DATA(key);

                count++;
            }
        }


        /* ENTER */

        else if(key == '=')
        {
            return value;
        }


        /* CANCEL */

        else if(key == 'C')
        {
            return 999;
        }


        delay_ms(100);
    }
}


/*==================================================
                  SET RTC TIME
==================================================*/

void Set_RTC_Time(void)
{
    u32 hour_value;
    u32 min_value;
    u32 sec_value;


    /* HOUR */

    WRITE_LCD_CMD(CLEAR_LCD);

    WRITE_LCD_CMD(
        GOTO_LINE1_POS0);

    StrLCD(
        (u8 *)"RTC HOUR:");

    WRITE_LCD_CMD(
        GOTO_LINE2_POS0);

    hour_value =
        Read_Number();


    if(hour_value == 999)
        return;


    if(hour_value > 23)
        return;


    /* MINUTE */

    WRITE_LCD_CMD(CLEAR_LCD);

    WRITE_LCD_CMD(
        GOTO_LINE1_POS0);

    StrLCD(
        (u8 *)"RTC MIN:");

    WRITE_LCD_CMD(
        GOTO_LINE2_POS0);

    min_value =
        Read_Number();


    if(min_value == 999)
        return;


    if(min_value > 59)
        return;


    /* SECOND */

    WRITE_LCD_CMD(CLEAR_LCD);

    WRITE_LCD_CMD(
        GOTO_LINE1_POS0);

    StrLCD(
        (u8 *)"RTC SEC:");

    WRITE_LCD_CMD(
        GOTO_LINE2_POS0);

    sec_value =
        Read_Number();


    if(sec_value == 999)
        return;


    if(sec_value > 59)
        return;


    /* SET RTC */

    SetRTCTimeInfo(
        hour_value,
        min_value,
        sec_value);


    /* CONFIRM */

    WRITE_LCD_CMD(CLEAR_LCD);

    WRITE_LCD_CMD(
        GOTO_LINE1_POS0);

    StrLCD(
        (u8 *)"RTC SET:");

    Display2Digit(hour_value);

    WRITE_LCD_DATA(':');

    Display2Digit(min_value);

    WRITE_LCD_DATA(':');

    Display2Digit(sec_value);


    delay_ms(1500);
}


/*==================================================
                  SET START TIME
==================================================*/

void Set_Start_Time(void)
{
    u32 value;


    /* HOUR */

    WRITE_LCD_CMD(CLEAR_LCD);

    WRITE_LCD_CMD(
        GOTO_LINE1_POS0);

    StrLCD(
        (u8 *)"START HOUR:");

    WRITE_LCD_CMD(
        GOTO_LINE2_POS0);

    value =
        Read_Number();


    if(value == 999)
        return;


    if(value <= 23)
    {
        start_hour = value;
    }


    /* MINUTE */

    WRITE_LCD_CMD(CLEAR_LCD);

    WRITE_LCD_CMD(
        GOTO_LINE1_POS0);

    StrLCD(
        (u8 *)"START MIN:");

    WRITE_LCD_CMD(
        GOTO_LINE2_POS0);

    value =
        Read_Number();


    if(value == 999)
        return;


    if(value <= 59)
    {
        start_min = value;
    }


    /* DISPLAY */

    WRITE_LCD_CMD(CLEAR_LCD);

    WRITE_LCD_CMD(
        GOTO_LINE1_POS0);

    StrLCD(
        (u8 *)"START:");

    Display2Digit(start_hour);

    WRITE_LCD_DATA(':');

    Display2Digit(start_min);


    delay_ms(1000);
}


/*==================================================
                   SET STOP TIME
==================================================*/

void Set_Stop_Time(void)
{
    u32 value;


    /* HOUR */

    WRITE_LCD_CMD(CLEAR_LCD);

    WRITE_LCD_CMD(
        GOTO_LINE1_POS0);

    StrLCD(
        (u8 *)"STOP HOUR:");

    WRITE_LCD_CMD(
        GOTO_LINE2_POS0);

    value =
        Read_Number();


    if(value == 999)
        return;


    if(value <= 23)
    {
        stop_hour = value;
    }


    /* MINUTE */

    WRITE_LCD_CMD(CLEAR_LCD);

    WRITE_LCD_CMD(
        GOTO_LINE1_POS0);

    StrLCD(
        (u8 *)"STOP MIN:");

    WRITE_LCD_CMD(
        GOTO_LINE2_POS0);

    value =
        Read_Number();


    if(value == 999)
        return;


    if(value <= 59)
    {
        stop_min = value;
    }


    /* DISPLAY */

    WRITE_LCD_CMD(CLEAR_LCD);

    WRITE_LCD_CMD(
        GOTO_LINE1_POS0);

    StrLCD(
        (u8 *)"STOP:");

    Display2Digit(stop_hour);

    WRITE_LCD_DATA(':');

    Display2Digit(stop_min);


    delay_ms(1000);
}


/*==================================================
                    CUSTOM 1
==================================================*/

void Set_Custom1(void)
{
    u32 value;


    /* ENABLE */

    WRITE_LCD_CMD(CLEAR_LCD);

    WRITE_LCD_CMD(
        GOTO_LINE1_POS0);

    StrLCD(
        (u8 *)"C1 ENABLE?");

    WRITE_LCD_CMD(
        GOTO_LINE2_POS0);

    StrLCD(
        (u8 *)"1=YES 0=NO");


    value =
        Read_Number();


    if(value == 999)
        return;


    if(value == 1)
        c1_enable = 1;

    else
        c1_enable = 0;


    if(c1_enable == 0)
        return;


    /* START HOUR */

    WRITE_LCD_CMD(CLEAR_LCD);

    WRITE_LCD_CMD(
        GOTO_LINE1_POS0);

    StrLCD(
        (u8 *)"C1 START HR:");

    WRITE_LCD_CMD(
        GOTO_LINE2_POS0);

    value =
        Read_Number();


    if(value == 999)
        return;


    if(value <= 23)
        c1_start_hour = value;


    /* START MIN */

    WRITE_LCD_CMD(CLEAR_LCD);

    WRITE_LCD_CMD(
        GOTO_LINE1_POS0);

    StrLCD(
        (u8 *)"C1 START MN:");

    WRITE_LCD_CMD(
        GOTO_LINE2_POS0);

    value =
        Read_Number();


    if(value == 999)
        return;


    if(value <= 59)
        c1_start_min = value;


    /* STOP HOUR */

    WRITE_LCD_CMD(CLEAR_LCD);

    WRITE_LCD_CMD(
        GOTO_LINE1_POS0);

    StrLCD(
        (u8 *)"C1 STOP HR:");

    WRITE_LCD_CMD(
        GOTO_LINE2_POS0);

    value =
        Read_Number();


    if(value == 999)
        return;


    if(value <= 23)
        c1_stop_hour = value;


    /* STOP MIN */

    WRITE_LCD_CMD(CLEAR_LCD);

    WRITE_LCD_CMD(
        GOTO_LINE1_POS0);

    StrLCD(
        (u8 *)"C1 STOP MN:");

    WRITE_LCD_CMD(
        GOTO_LINE2_POS0);

    value =
        Read_Number();


    if(value == 999)
        return;


    if(value <= 59)
        c1_stop_min = value;


    /* GREEN */

    WRITE_LCD_CMD(CLEAR_LCD);

    WRITE_LCD_CMD(
        GOTO_LINE1_POS0);

    StrLCD(
        (u8 *)"C1 GREEN:");

    WRITE_LCD_CMD(
        GOTO_LINE2_POS0);

    value =
        Read_Number();


    if(value == 999)
        return;


    if((value >= 1) &&
       (value <= 99))
    {
        c1_green = value;
    }


    /* YELLOW */

    WRITE_LCD_CMD(CLEAR_LCD);

    WRITE_LCD_CMD(
        GOTO_LINE1_POS0);

    StrLCD(
        (u8 *)"C1 YELLOW:");

    WRITE_LCD_CMD(
        GOTO_LINE2_POS0);

    value =
        Read_Number();


    if(value == 999)
        return;


    if((value >= 1) &&
       (value <= 99))
    {
        c1_yellow = value;
    }


    /* DONE */

    WRITE_LCD_CMD(CLEAR_LCD);

    WRITE_LCD_CMD(
        GOTO_LINE1_POS0);

    StrLCD(
        (u8 *)"CUSTOM1 SET");

    delay_ms(1000);
}


/*==================================================
                    CUSTOM 2
==================================================*/

void Set_Custom2(void)
{
    u32 value;


    /* ENABLE */

    WRITE_LCD_CMD(CLEAR_LCD);

    WRITE_LCD_CMD(
        GOTO_LINE1_POS0);

    StrLCD(
        (u8 *)"C2 ENABLE?");

    WRITE_LCD_CMD(
        GOTO_LINE2_POS0);

    StrLCD(
        (u8 *)"1=YES 0=NO");


    value =
        Read_Number();


    if(value == 999)
        return;


    if(value == 1)
        c2_enable = 1;

    else
        c2_enable = 0;


    if(c2_enable == 0)
        return;


    /* START HOUR */

    WRITE_LCD_CMD(CLEAR_LCD);

    WRITE_LCD_CMD(
        GOTO_LINE1_POS0);

    StrLCD(
        (u8 *)"C2 START HR:");

    WRITE_LCD_CMD(
        GOTO_LINE2_POS0);

    value =
        Read_Number();


    if(value == 999)
        return;


    if(value <= 23)
        c2_start_hour = value;


    /* START MIN */

    WRITE_LCD_CMD(CLEAR_LCD);

    WRITE_LCD_CMD(
        GOTO_LINE1_POS0);

    StrLCD(
        (u8 *)"C2 START MN:");

    WRITE_LCD_CMD(
        GOTO_LINE2_POS0);

    value =
        Read_Number();


    if(value == 999)
        return;


    if(value <= 59)
        c2_start_min = value;


    /* STOP HOUR */

    WRITE_LCD_CMD(CLEAR_LCD);

    WRITE_LCD_CMD(
        GOTO_LINE1_POS0);

    StrLCD(
        (u8 *)"C2 STOP HR:");

    WRITE_LCD_CMD(
        GOTO_LINE2_POS0);

    value =
        Read_Number();


    if(value == 999)
        return;


    if(value <= 23)
        c2_stop_hour = value;


    /* STOP MIN */

    WRITE_LCD_CMD(CLEAR_LCD);

    WRITE_LCD_CMD(
        GOTO_LINE1_POS0);

    StrLCD(
        (u8 *)"C2 STOP MN:");

    WRITE_LCD_CMD(
        GOTO_LINE2_POS0);

    value =
        Read_Number();


    if(value == 999)
        return;


    if(value <= 59)
        c2_stop_min = value;


    /* GREEN */

    WRITE_LCD_CMD(CLEAR_LCD);

    WRITE_LCD_CMD(
        GOTO_LINE1_POS0);

    StrLCD(
        (u8 *)"C2 GREEN:");

    WRITE_LCD_CMD(
        GOTO_LINE2_POS0);

    value =
        Read_Number();


    if(value == 999)
        return;


    if((value >= 1) &&
       (value <= 99))
    {
        c2_green = value;
    }


    /* YELLOW */

    WRITE_LCD_CMD(CLEAR_LCD);

    WRITE_LCD_CMD(
        GOTO_LINE1_POS0);

    StrLCD(
        (u8 *)"C2 YELLOW:");

    WRITE_LCD_CMD(
        GOTO_LINE2_POS0);

    value =
        Read_Number();


    if(value == 999)
        return;


    if((value >= 1) &&
       (value <= 99))
    {
        c2_yellow = value;
    }


    /* DONE */

    WRITE_LCD_CMD(CLEAR_LCD);

    WRITE_LCD_CMD(
        GOTO_LINE1_POS0);

    StrLCD(
        (u8 *)"CUSTOM2 SET");

    delay_ms(1000);
}


/*==================================================
                TRAFFIC SETTINGS
==================================================*/

void Traffic_Settings(void)
{
    u8 key;


    while(1)
    {
        WRITE_LCD_CMD(CLEAR_LCD);


        WRITE_LCD_CMD(
            GOTO_LINE1_POS0);

        StrLCD(
            (u8 *)"1-ST 2-SP 3-C1");


        WRITE_LCD_CMD(
            GOTO_LINE2_POS0);

        StrLCD(
            (u8 *)"4-C2 5-CONT");


        while(ColScan());


        key = KeyScan();


        if(key == '1')
        {
            Set_Start_Time();
        }

        else if(key == '2')
        {
            Set_Stop_Time();
        }

        else if(key == '3')
        {
            Set_Custom1();
        }

        else if(key == '4')
        {
            Set_Custom2();
        }

        else if(key == '5')
        {
            return;
        }


        delay_ms(100);
    }
}


/*==================================================
                    SELECT MODE
==================================================*/

u8 Select_Mode(void)
{
    u8 key;


    while(1)
    {
        WRITE_LCD_CMD(CLEAR_LCD);


        WRITE_LCD_CMD(
            GOTO_LINE1_POS0);

        StrLCD(
            (u8 *)"SELECT MODE");


        WRITE_LCD_CMD(
            GOTO_LINE2_POS0);

        StrLCD(
            (u8 *)"1-AUTO 2-MAN");


        while(ColScan());


        key = KeyScan();


        /* AUTO */

        if(key == '1')
        {
            WRITE_LCD_CMD(CLEAR_LCD);

            WRITE_LCD_CMD(
                GOTO_LINE1_POS0);

            StrLCD(
                (u8 *)"AUTO MODE");


            delay_ms(1000);

            return 1;
        }


        /* MANUAL */

        if(key == '2')
        {
            WRITE_LCD_CMD(CLEAR_LCD);

            WRITE_LCD_CMD(
                GOTO_LINE1_POS0);

            StrLCD(
                (u8 *)"MANUAL MODE");


            delay_ms(1000);

            return 2;
        }


        delay_ms(100);
    }
}


/*==================================================
                       MAIN
==================================================*/

int main(void)
{
    u8 mode;


    /*==============================================
                    INITIALIZATION
    ==============================================*/

    Init_LCD();

    InitKPM();

    RTC_Init();

    Traffic_Init();


    /*==============================================
                      POWER ON
    ==============================================*/

    WRITE_LCD_CMD(CLEAR_LCD);

    WRITE_LCD_CMD(
        GOTO_LINE1_POS0);

    StrLCD(
        (u8 *)"TRAFFIC SYSTEM");


    WRITE_LCD_CMD(
        GOTO_LINE2_POS0);

    StrLCD(
        (u8 *)"POWER ON");


    delay_ms(1500);


    /*==============================================
                     SET RTC
    ==============================================*/

    Set_RTC_Time();


    /*==============================================
                 TRAFFIC SETTINGS
    ==============================================*/

    Traffic_Settings();


    /*==============================================
                   SELECT MODE
    ==============================================*/

    mode = Select_Mode();


    /*==============================================
                    MAIN LOOP
    ==============================================*/

    while(1)
    {
        /*==========================================
                     AUTO MODE
        ==========================================*/

        if(mode == 1)
        {
            Auto_Mode(
                start_hour,
                start_min,
                stop_hour,
                stop_min,

                c1_enable,
                c1_start_hour,
                c1_start_min,
                c1_stop_hour,
                c1_stop_min,
                c1_green,
                c1_yellow,

                c2_enable,
                c2_start_hour,
                c2_start_min,
                c2_stop_hour,
                c2_stop_min,
                c2_green,
                c2_yellow);


            /*
               C pressed.
               Return to mode selection.
            */

            mode = Select_Mode();
        }


        /*==========================================
                    MANUAL MODE
        ==========================================*/

        else
        {
            WRITE_LCD_CMD(CLEAR_LCD);

            WRITE_LCD_CMD(
                GOTO_LINE1_POS0);

            StrLCD(
                (u8 *)"MANUAL MODE");


            WRITE_LCD_CMD(
                GOTO_LINE2_POS0);

            StrLCD(
                (u8 *)"1 2 3 4 5=C");


            delay_ms(1000);


            Manual_Mode();


            /*
               C pressed.
            */

            mode = Select_Mode();
        }
    }
}
