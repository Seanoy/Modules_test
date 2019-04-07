/**
  ******************************************************************************
  * @file CAN/LoopBack/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version  V3.0.0
  * @date  04/06/2009
  * @brief  Main Interrupt Service Routines.
  *         This file provides template for all exceptions handler and 
  *         peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include  <stdio.h>
extern void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...);
extern int x;

void USART1_IRQHandler (void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) !=RESET)										
	{
		 char RxData;
       RxData = USART_ReceiveData(USART1);
		  if (RxData =='a')
			 {
				 x = 1;
			 }
		if (RxData =='b')
			 {
				 x = 2;
			 }
		if (RxData =='c')
			 {
				 x = 3;
			 }
		if (RxData =='d')
			 {
				 x = 4;
			 }
		if (RxData =='e')
			 {
				 x = 5;
			 }
			 
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) ==RESET);              //判断是否发送完毕
		USART_ClearFlag(USART2, USART_FLAG_RXNE);                              //清楚标志位

	}
}



/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
