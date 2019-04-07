#include "stm32f10x.h"

#ifndef __BSP_CAN_MASTER_H
#define __BSP_CAN_MASTER_H


void CAN_Master_Configuration(void);			//从机CAN配置
void CAN_Master_SetMsg(uint8_t data[]);		//打包报文
void CAN_Master_SendMsg(void);						//发送报文
void USB_LP_CAN1_RX0_IRQHandler(void);		//主机接收中断


#endif
