#ifndef TRAFFIC_H
#define TRAFFIC_H

#include "tpyes.h"

/*==================================================
              TRAFFIC LED CONNECTION
==================================================*/

/* Direction 1 */
#define D1_RED       14
#define D1_YELLOW    15
#define D1_GREEN     16

/* Direction 2 */
#define D2_RED       17
#define D2_YELLOW    18
#define D2_GREEN     19

/* Direction 3 */
#define D3_RED       20
#define D3_YELLOW    21
#define D3_GREEN     22

/* Direction 4 */
#define D4_RED       23
#define D4_YELLOW    24
#define D4_GREEN     25


/*==================================================
                    MODES
==================================================*/

#define NORMAL_MODE   1
#define PEAK_MODE     2
#define NIGHT_MODE    3
#define CUSTOM1_MODE  4
#define CUSTOM2_MODE  5


/*==================================================
                DEFAULT TIMINGS
==================================================*/

#define NORMAL_GREEN   10
#define NORMAL_YELLOW   3

#define PEAK_GREEN     20
#define PEAK_YELLOW     3

#define NIGHT_GREEN     5
#define NIGHT_YELLOW    2


/*==================================================
                    FUNCTIONS
==================================================*/

void Traffic_Init(void);

void Traffic_All_Red(void);


/* AUTO MODE */

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
    u32 c2_yellow
);


/* MANUAL MODE */

void Manual_Mode(void);

#endif
