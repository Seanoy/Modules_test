#ifndef __BSP_H
#define __BSP_H

#include "stm32f10x.h"
#include "stdio.h"
#include "bsp_rtc.h"
#include "bsp_systick.h"

void USART_Config(void);
void bsp_init(void);
#endif
