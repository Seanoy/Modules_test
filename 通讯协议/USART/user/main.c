#include "bsp.h"

	void USART_Config(void);
void TIMx_GPIO_Config(void){
		
		GPIO_InitTypeDef GPIO_InitStructure;    //PA6×÷ÎªPWMÊä³ö
		
		RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO , ENABLE ); 
																																		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ;  						 
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    	 
		
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      
		
		GPIO_Init( GPIOC , &GPIO_InitStructure );    
	
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
	}

	void delay(void)
{
	int i =5000000;
	while(i--);	 
}
	 
int main(){	
	
	TIMx_GPIO_Config();
  USART_Config();
	
while(1){

	printf("hello!");
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	delay();
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	delay();
}	

}
