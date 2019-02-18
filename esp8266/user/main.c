#include "stm32f10x.h"
#include "stdio.h"
#include "esp8266.h"
#include "stm32f10x_usart.h"
void LED_GPIO_Init(void);
void USART_Config(void);

void delay()
{
	int i,j;
	for(i=5000;i>0;i--)
	for(j=3000;j>0;j--);
}
void usart_2_string(char *s)
{
	while(*s)
		USART_SendData(USART1,*s++);
}
int main(){	
	USART_Config();
	
	esp_cwmode_def(2);
	delay();	
	delay();
	delay();
		delay();
		delay();
	esp_rst();
	delay();
	delay();
	delay();
		delay();
		delay();
		delay();
	esp_cwsap_def("seanoy", "asd123456789",5,3);
	delay();
	delay();
	delay();
		delay();
	esp_cipmux(1);
	delay();
	delay();
		delay();
	esp_cipserver(1, 5000);
	delay();
	delay();
	delay();
while(1){
//	printf("AT+CIPSEND=0,5\r\n");delay();delay();delay();delay(); 
	esp_cipsend(0, 5);
	delay();	delay();
	printf("hello\r\n");
	delay();
}	

}
