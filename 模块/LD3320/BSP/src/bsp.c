#include "bsp.h"

void bsp_init(void)
{
	USART_Config();
	RCC_Configuration();
	LD3320_Init();
}


