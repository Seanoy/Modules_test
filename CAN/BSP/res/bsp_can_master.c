#include "bsp_can_master.h"

/*
 * CAN ����źŵı�ʾ
 * 1:���Ե�ƽ H2.5v - L2.5v = 0v
 * 0:���Ե�ƽ H3.5v - L1.5v = 2v
 */

//���ӻ�CAN���ö�һ����ֻ�������ͽ�����Щ�����

typedef unsigned char u8;

//�������ݻ�����
u8 RXBUF[5];
u8 RX_FLAG = 0;

CanTxMsg TXMessage;			//�������ݻ�����
CanRxMsg RXMessage;			//�������ݻ�����


void CAN_Master_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;												//����CAN��Ӧ����
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);	//ʹ��GPIO�͸���IOʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);				//ʹ��CAN1ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;										//RX  PB8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;								//�������루�����������������룩
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;										//RX  PB9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;						//�������	�����츴�������
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

void CAN_Master_NVIC_Configuration(void)
{	
	NVIC_InitTypeDef NVIC_InitStructure;												//����CAN��ص�Ƕ�������жϿ�����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);							//�ж����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;	//CAN1 RX0 �ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		//��ռ����Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//�����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


/*
CAN ���ж��ɷ����жϡ����� FIFO �жϺʹ����жϹ���
�����ж�������������������һ��Ϊ�յ��¼����ɡ�
���� FIFO �жϷ�Ϊ FIFO0 �� FIFO1 ���жϣ����� FIFO �յ��µı��Ļ���������¼����������жϡ�
*/

static void CAN_Master_Mode_Configuration(void)
{
	CAN_InitTypeDef CAN_InitStructure;						//����CAN1
	
	CAN_DeInit(CAN1);															//ʹCAN1����Ϊȱʡֵ
	CAN_StructInit(&CAN_InitStructure);						//CAN1�Ĵ�����ʼ��
	
	CAN_InitStructure.CAN_TTCM = DISABLE;					//�ر�ʱ�䴥��ͨ��ģʽʹ��(Time Triggered Communication Mode)
	CAN_InitStructure.CAN_ABOM = ENABLE;					//�����Զ����߹���Automatic Bus-Off Management��
	CAN_InitStructure.CAN_AWUM = ENABLE;					//ʹ���Զ�����ģʽ(Automatic Wake-Up Mode)
	CAN_InitStructure.CAN_NART = DISABLE;					//��ֹ�����Զ��ش�(No-Automatic Retransmission mode)
	CAN_InitStructure.CAN_RFLM = DISABLE;					//����FIFO����ģʽ (Receive FIFO Locked Mode)���ʱ�±��ĻḲ��ԭ�б���
	CAN_InitStructure.CAN_TXFP = DISABLE;					//���� FIFO ���ȼ�(Transmit FIFO Priority)���ȼ�ȡ���ڱ��ı�ʾ��
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	//��������ģʽ
	CAN_InitStructure.CAN_SJW = CAN_SJW_2tq;			//����ͬ����Ծ���Ϊ2��ʱ�䵥λ(CAN_Synchronisation_Jump_width )
	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;			//ʱ���1ռ��6��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;			//ʱ���2ռ��3��ʱ�䵥Ԫ
	CAN_InitStructure.CAN_Prescaler = 4;					//�����ʷ�Ƶ��
	/*
	BaudRate = Fpclk1/((tsjw+tbs1+tbs2) * brp)
	36000Kbps/(1+6+3)/4 = 900Kbps = 0.9Mbps
	*/
	CAN_Init(CAN1, &CAN_InitStructure);
	
}

/*���ǻ�Ҫ�� CAN �Ĺ������������ã�ʹ���� FIFO ֻ�����ض��ı��ġ�
��ν�Ĺ��˾��� CAN �ӿڸ����յ����ĵ� ID��ѡ���Ƿ�Ѹñ��ı��浽���� FIFO �С�*/

/*STM32 �� ID ���˷�ʽ�����֡�һ��Ϊ��ʶ���б�ģʽ������Ҫ���ձ��ĵ� ID �г�һ
����Ҫ���� ID ���б��е�ĳһ����ʶ����ȫ��ͬ�ſ��Խ��գ��������Ϊ��������
����һ�ֳ�Ϊ��ʶ������ģʽ�����ѿɽ��ձ��� ID ��ĳ��λ��Ϊ�б��⼸λ����Ϊ
����λ�����԰������ɹؼ���������ֻҪ����λ���ؼ��֣���ͬ�ͷ���Ҫ��*/

static void CAN_Master_Filter_Configuration(void){
	CAN_FilterInitTypeDef CAN_FilterInitStructure;																			//����CAN������
	
	CAN_FilterInitStructure.CAN_FilterNumber = 0;																				//�������� 0  It ranges from 0 to 13. 
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;											//�����ڱ�ʶ������λģʽ
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;										//������λ��Ϊ32λ��
	CAN_FilterInitStructure.CAN_FilterIdHigh = (((u32)0x00001314<<3)&0xFFFF0000)>>16;		//Ҫ���˵�ID�ĸ�16λ
	CAN_FilterInitStructure.CAN_FilterIdLow = (((u32)0x00001314<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF;//Ҫ���˵�ID�ĵ�16λ
	/*#define CAN_Id_Extended             ((uint32_t)0x00000004)  Extended Id 		IDEλΪ����λ���ұ���Ϊ����֡
		#define CAN_RTR_Data                ((uint32_t)0x00000000)  Data frame 			RTRλΪ����λ				*/
//	��ֵʱ��ʶ�� 0x0000 1314 ������λ����Ϊ�Ĵ����ĵ���λ����Ϊ����λ��RTR λ�� IDE λ
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xFFFF;							//Specifies the filter mask number or identification number
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0xFFFF;								//ֵΪ 1 �ļĴ���λ��������λ��ֵΪ0 �ļĴ���λ������Ƚϡ�
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;	//Specifies the FIFO (0 or 1) which will be assigned to the filter
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;							//ʹ�ܹ����� Enable or disable the filter.
  CAN_FilterInit(&CAN_FilterInitStructure);														//��ʼ��������
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);														//CAN ͨ���ж�ʹ��
//	FIFO0 ��Ϣ�Һ��ж�ʹ�ܣ�CAN_IT_FMP0���� CAN �ӿڵ� FIFO0 �յ�����ʱ���������жϷ������д� FIFO0 �����ݵ��ڴ档
}

void CAN_Master_Configuration(void)
{
	CAN_Master_GPIO_Configuration();
	CAN_Master_NVIC_Configuration();
	CAN_Master_Mode_Configuration();
	CAN_Master_Filter_Configuration();
}

//��������ͱ���  ���Ҫ�����жϵĻ�	data[]������Ҫ��0xABCD
void CAN_Master_SetMsg(uint8_t data[])
{	
//	TXMessage.StdId = 0x00;				//��������ʹ����չID�����Բ���Ҫ������ֵ
	TXMessage.ExtId = 0x1314;				//ʹ����չ��ID
	TXMessage.IDE = CAN_ID_EXT;			//��չģʽ
	TXMessage.RTR = CAN_RTR_DATA;		//���͵������� Specifies the type of frame for the received message.
	TXMessage.DLC = 2;							//���ݳ���Ϊ2�ֽ�
	TXMessage.Data[0] = data[0];		// Contains the data to be received. It ranges from 0 to 0xFF. 
	TXMessage.Data[1] = data[1];	

}

void CAN_Master_SendMsg(void)
{
	CAN_Transmit(CAN1, &TXMessage);	//���ͱ���
}
	

void USB_LP_CAN1_RX0_IRQHandler(void)					//�жϺ�������startup_stm32f10x_xx.s��ɿ���
{
	CAN_Receive(CAN1, CAN_FIFO0, &RXMessage);		
	if((RXMessage.ExtId == 0x1314) && 					// �Ƚ��Ƿ��Ƿ��͵����ݺ� ID 
		(RXMessage.IDE == CAN_ID_EXT) && 
		(RXMessage.DLC == 2) && 
		(RXMessage.Data[1] | RXMessage.Data[0]<<8)==0xDCBA)		//�жϽ��ܵ������Ƿ�Ϊ0XDCBA
	{
		RX_FLAG = 0;			//���ճɹ�
	}
	else
		RX_FLAG = 0xff;		//����ʧ��	
}
	
	