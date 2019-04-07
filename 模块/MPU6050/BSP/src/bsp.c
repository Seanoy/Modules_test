#include "bsp.h"

void bsp_init(void){
	USART_Config();
	printf("USART Config Succeed!");
	RCC_Configuration();		 	//����RCC
	printf("RCC Config Succeed!");
  GPIO_Configuration();		 	//����GPIO
	printf("GPIO Config Succeed!");
  //USART1_Configuration();	 	//���ô���1
  I2C_GPIO_Config();		 		//����IICʹ�ö˿�
	printf("IIC Config Succeed!");
  Delayms(10);				 			//��ʱ
  Init_MPU6050();		     		//��ʼ��MPU6050
	printf("MPU6050 Init Succeed!");
}

