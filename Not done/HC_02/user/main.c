#include "stm32f10x.h"
#include "stdio.h"
 
 #include "bsp_systick.h"
 
void USART_Config(void);


 

int main(){	
 
    USART_Config();
 
    while(1)
    {
				printf("hello!");
			delay_ms(300);
        //delay_ms(1000);
    }	

}


