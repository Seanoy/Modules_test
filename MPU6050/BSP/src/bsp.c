#include "bsp.h"

void bsp_init(void){
	USART_Config();
	RCC_Configuration();		 //����RCC
  GPIO_Configuration();		 //����GPIO
  USART1_Configuration();	 //���ô���1
  I2C_GPIO_Config();		 //����IICʹ�ö˿�
  Delayms(10);				 //��ʱ
  Init_MPU6050();		     //��ʼ��MPU6050
	
}

