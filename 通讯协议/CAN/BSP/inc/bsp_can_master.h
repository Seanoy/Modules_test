#include "stm32f10x.h"

#ifndef __BSP_CAN_MASTER_H
#define __BSP_CAN_MASTER_H


void CAN_Master_Configuration(void);			//�ӻ�CAN����
void CAN_Master_SetMsg(uint8_t data[]);		//�������
void CAN_Master_SendMsg(void);						//���ͱ���
void USB_LP_CAN1_RX0_IRQHandler(void);		//���������ж�


#endif
