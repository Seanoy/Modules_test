#include "stm32f10x.h"
#include "stdio.h"
#include "rtc.h"
 #include "bsp_systick.h"
 
void USART_Config(void);


 

int main(){	
u8 t=0;
    USART_Config();
    RTC_Init();
    while(1)
    {
        if(t!=calendar.sec)
        {
            t=calendar.sec;
            printf("\r\n now is %d �� %d �� %d �� %d ʱ %d �� %d �� \r\n ",
					calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);
        }
        //delay_ms(1000);
    }	

}
