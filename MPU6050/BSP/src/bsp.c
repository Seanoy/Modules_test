#include "bsp.h"

void bsp_init(void){
	USART_Config();
	RCC_Configuration();		 //配置RCC
  GPIO_Configuration();		 //配置GPIO
  USART1_Configuration();	 //配置串口1
  I2C_GPIO_Config();		 //配置IIC使用端口
  Delayms(10);				 //延时
  Init_MPU6050();		     //初始化MPU6050
	
}

