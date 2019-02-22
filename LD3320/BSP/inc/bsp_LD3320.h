#ifndef __BSP_LD3320_H
#define __BSP_LD3320_H

/************************************************************************************
**	��Ȩ���У�Copyright (c) 2005 - 2010 ICRoute INC.
** Ӳ��˵����STM32f103vet6  8MHZ  �ڲ�ʱ��Ϊ72MHZ 
** 					 LD3320 ��Դ���� 12MHZ  ��ͬ�ľ������޸�LDChip.h �еĶ���ֵ����define CLK_IN  12��
** ���˵�������ж�д�Ĵ�����ʽ�������������ʽ���޸�Reg_RW.c �ļ����ɣ�
** �޸���:  http://shop64790118.taobao.com/
** ʱ�䣺2011.10.15
** ˵�����������޸��� ICRoute��˾ ���׵�Դ���򣬰�Ȩ�������С�
************************************************************************************/

#include "stm32f10x.h"

#define uint8 unsigned char
#define uint16 unsigned int
#define uint32 unsigned long

#define LD_RST_H() GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define LD_RST_L() GPIO_ResetBits(GPIOB, GPIO_Pin_6)

#define LD_CS_H()	GPIO_SetBits(GPIOB, GPIO_Pin_8)
#define LD_CS_L()	GPIO_ResetBits(GPIOB, GPIO_Pin_8)

#define LD_SPIS_H()  GPIO_SetBits(GPIOB, GPIO_Pin_10)
#define LD_SPIS_L()  GPIO_ResetBits(GPIOB, GPIO_Pin_10)

#define LED_OFF      GPIO_SetBits(GPIOB, GPIO_Pin_9)
#define LED_ON       GPIO_ResetBits(GPIOB, GPIO_Pin_9)


//	��������״̬����������¼������������ASRʶ����������MP3����
#define LD_MODE_IDLE			0x00
#define LD_MODE_ASR_RUN		0x08
#define LD_MODE_MP3		 		0x40


//	�������״̬����������¼������������ASRʶ������е��ĸ�״̬
#define LD_ASR_NONE					0x00	//	��ʾû������ASRʶ��
#define LD_ASR_RUNING				0x01	//	��ʾLD3320������ASRʶ����
#define LD_ASR_FOUNDOK			0x10	//	��ʾһ��ʶ�����̽�������һ��ʶ����
#define LD_ASR_FOUNDZERO 		0x11	//	��ʾһ��ʶ�����̽�����û��ʶ����
#define LD_ASR_ERROR	 			0x31	//	��ʾһ��ʶ��������LD3320оƬ�ڲ����ֲ���ȷ��״̬


#define CLK_IN   	8	/* user need modify this value according to clock in */
#define LD_PLL_11					(uint8)((CLK_IN/2.0)-1)
#define LD_PLL_MP3_19			0x0f
#define LD_PLL_MP3_1B			0x18
#define LD_PLL_MP3_1D   	(uint8)(((90.0*((LD_PLL_11)+1))/(CLK_IN))-1)

#define LD_PLL_ASR_19 		(uint8)(CLK_IN*32.0/(LD_PLL_11+1) - 0.51)
#define LD_PLL_ASR_1B 		0x48
#define LD_PLL_ASR_1D 		0x1f

// LD chip fixed values.
#define RESUM_OF_MUSIC               0x01
#define CAUSE_MP3_SONG_END           0x20

#define MASK_INT_SYNC								0x10
#define MASK_INT_FIFO								0x04
#define MASK_AFIFO_INT								0x01
#define MASK_FIFO_STATUS_AFULL				0x08

//ʶ���루�ͻ��޸Ĵ���
#define CODE_LSD		1	 /*��ˮ��*/
#define CODE_SS	  	2	 /*��˸*/
#define CODE_AJCF		3	 /*��������*/
#define CODE_QM	  	4	 /*ȫ��*/
#define ludan  			5  /*����±��*/
#define Change_Wake 6 	/*ת���ɻ���ģʽ*/

//ģ��״̬ģʽ
#define Wake_mode 		1 //����ģʽ ֱ�Ӻ���ָ���ִ������
#define Call_out_mode 2 //����ģʽ����Ҫ����С�� �ſ���ִ������
#define Sleep_mode  	3 //˯��ģʽ   

#define MIC_VOL 0x43		//LD3320��ַ

void LD3320_delay(unsigned long uldata);
void LD_reset(void);
void LD_Init_Common(void);
void LD_Init_ASR(void);
void LD_ReloadMp3Data(void);
void LD_ReloadMp3Data_2(void);

uint8 LD_ProcessAsr(uint32 RecogAddr);
void 	LD_AsrStart(void);
uint8 LD_AsrRun(void);
uint8 LD_AsrAddFixed(void);
uint8 LD_GetResult(void);

void LD_ReadMemoryBlock(uint8 dev, uint8 * ptr, uint32 addr, uint8 count);
void LD_WriteMemoryBlock(uint8 dev, uint8 * ptr, uint32 addr, uint8 count);

extern uint8  nLD_Mode;

uint8 LD_ReadReg(uint8 reg_add);
void  LD_WriteReg(uint8 data1,uint8 data2);

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void designed(void);
void delay(int time);//��ʱ
void wake(void);
void LD3320_Init(void);
void LD3320_main(void);

#endif
