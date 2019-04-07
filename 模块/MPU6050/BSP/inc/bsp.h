#ifndef __BSP_H
#define __BSP_H


#include "stm32f10x.h"
#include "stdio.h"
#include  <math.h>   
#include <stdbool.h>
#include "misc.h"

#include "bsp_MPU6050.h"


void bsp_init(void);
void USART_Config(void);

#endif
