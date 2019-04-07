#ifndef __BSP_H
#define __BSP_H


#include "stm32f10x.h"
#include "stdio.h"
#include "stdint.h"

#include "bsp_LD3320.h"
#include "bsp_delay.h"
#include "bsp_sys.h"

void USART_Config(void);
void bsp_init(void);	

#endif
