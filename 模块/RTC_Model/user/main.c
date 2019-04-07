#include "bsp.h"
 

int main(){	
	u8 t=0;
	bsp_init();
    while(1)
    {
        if(t!=calendar.sec)
        {
            t=calendar.sec;
            printf("\r\n now is %d 年 %d 月 %d 日 %d 时 %d 分 %d 秒 \r\n ",
						calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);
        }
        //delay_ms(1000);
    }	

}
