#ifndef __BSP_H
#define __BSP_H


#include "stm32f10x.h"
#include "stdio.h"


#include "bsp_iic.h"

void bsp_init(void);
void USART_Config(void);		//用于调试输出


#endif
