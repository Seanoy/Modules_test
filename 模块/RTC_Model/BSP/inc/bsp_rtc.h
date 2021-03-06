#ifndef __BSP_RTC_H
#define __BSP_RTC_H
#include "stm32f10x.h"

typedef struct //时间结构体
{
    vu8 hour;
    vu8 min;
    vu8 sec;            
    //公历年月日周
    vu16 w_year;
    vu8  w_month;
    vu8  w_date;
    vu8  week;     
}_calendar_obj;                     

extern _calendar_obj calendar;
void NVIC_Configuration(void);
void RTC_Configuration(void);
void RTC_Init(void);
u8 Is_Leap_Year(u16 pyear);
u8 RTC_Set(u16 year,u8 mon,u8 day,u8 hour,u8 min,u8 sec);
u8 RTC_Get(void);

#endif

