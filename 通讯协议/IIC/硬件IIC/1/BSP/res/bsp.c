#include "bsp.h"

void bsp_init(void)
{
	USART_Config();
	NVIC_IIC_Configuration();
	GPIO_IIC_Configuration();
  I2C_LowLevel_Init(I2C2);
  I2C_LowLevel_Init(I2C1);
	
}

