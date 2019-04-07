#include "stm32f10x.h"
#include "stdio.h"


void delay_init(void);
void USART_Config(void);
void GPIO_Config(void); 
void TIM_Config(void);
float Hcsr04GetLength(void )  ;


void delay_ms(u16 nms);
void delay_us(u32 nus);
 
float s=0.0; 
 
int main(void){
	USART_Config();
	printf("USART_Config");
	delay_init();
	printf("delay_init");
	TIM_Config();
	printf("TIM_Config");
	GPIO_Config();
	printf("GPIO_Config");
	while(1){
		s=Hcsr04GetLength();
		printf("²âµÄ¾àÀëÎª£º%f\n",s);
		delay_ms(1000);
	}
}
