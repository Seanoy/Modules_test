#include "bsp.h"

void bsp_init(void){
	USART_Config();
	printf("USART Config Succeed!");
	RCC_Configuration();		 	//配置RCC
	printf("RCC Config Succeed!");
  GPIO_Configuration();		 	//配置GPIO
	printf("GPIO Config Succeed!");
  //USART1_Configuration();	 	//配置串口1
  I2C_GPIO_Config();		 		//配置IIC使用端口
	printf("IIC Config Succeed!");
  Delayms(10);				 			//延时
  Init_MPU6050();		     		//初始化MPU6050
	printf("MPU6050 Init Succeed!");
}

