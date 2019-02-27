#include "stm32f10x.h"

#ifndef __BSP_CAN_SLAVE_H
#define __BSP_CAN_SLAVE_H


void CAN_Slave_Configuration(void);			//�ӻ�CAN����
void CAN_Slave_SetMsg(uint8_t data[]);	//�������
void CAN_Slave_SendMsg(void);						//���ͱ���
void USB_LP_CAN1_RX0_IRQHandler(void);	//�ӻ������ж�


#endif
