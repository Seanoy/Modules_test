#include "includes.h"
 
void Bsp_Init(void)
{
	delay_init();								   			//SysTick��ʼ�� 1ms�ж�һ��
	
	LED_GPIO_Init();										//����LED��ʼ��
}

