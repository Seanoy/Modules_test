#include "bsp_can_master.h"

/*
 * CAN 差分信号的表示
 * 1:隐性电平 H2.5v - L2.5v = 0v
 * 0:显性电平 H3.5v - L1.5v = 2v
 */

//主从机CAN设置都一样，只不过发送接收有些许差异

typedef unsigned char u8;

//接收数据缓冲区
u8 RXBUF[5];
u8 RX_FLAG = 0;

CanTxMsg TXMessage;			//发送数据缓冲区
CanRxMsg RXMessage;			//接收数据缓冲区


void CAN_Master_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;												//配置CAN对应引脚
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);	//使能GPIO和复用IO时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);				//使能CAN1时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;										//RX  PB8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;								//上拉输入（浮空输入或带上拉输入）
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;										//RX  PB9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;						//推挽输出	（推挽复用输出）
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

void CAN_Master_NVIC_Configuration(void)
{	
	NVIC_InitTypeDef NVIC_InitStructure;												//配置CAN相关的嵌套向量中断控制器
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);							//中断优先级1
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;	//CAN1 RX0 中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		//抢占优先为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//子优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


/*
CAN 的中断由发送中断、接收 FIFO 中断和错误中断构成
发送中断由三个发送邮箱任意一个为空的事件构成。
接收 FIFO 中断分为 FIFO0 和 FIFO1 的中断，接收 FIFO 收到新的报文或报文溢出的事件可以引起中断。
*/

static void CAN_Master_Mode_Configuration(void)
{
	CAN_InitTypeDef CAN_InitStructure;						//配置CAN1
	
	CAN_DeInit(CAN1);															//使CAN1配置为缺省值
	CAN_StructInit(&CAN_InitStructure);						//CAN1寄存器初始化
	
	CAN_InitStructure.CAN_TTCM = DISABLE;					//关闭时间触发通信模式使能(Time Triggered Communication Mode)
	CAN_InitStructure.CAN_ABOM = ENABLE;					//启动自动离线管理（Automatic Bus-Off Management）
	CAN_InitStructure.CAN_AWUM = ENABLE;					//使用自动唤醒模式(Automatic Wake-Up Mode)
	CAN_InitStructure.CAN_NART = DISABLE;					//禁止报文自动重传(No-Automatic Retransmission mode)
	CAN_InitStructure.CAN_RFLM = DISABLE;					//接收FIFO锁定模式 (Receive FIFO Locked Mode)溢出时新报文会覆盖原有报文
	CAN_InitStructure.CAN_TXFP = DISABLE;					//发送 FIFO 优先级(Transmit FIFO Priority)优先级取决于报文标示符
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	//正常工作模式
	CAN_InitStructure.CAN_SJW = CAN_SJW_2tq;			//重新同步跳跃宽度为2个时间单位(CAN_Synchronisation_Jump_width )
	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;			//时间段1占用6个时间单元
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;			//时间段2占用3个时间单元
	CAN_InitStructure.CAN_Prescaler = 4;					//波特率分频器
	/*
	BaudRate = Fpclk1/((tsjw+tbs1+tbs2) * brp)
	36000Kbps/(1+6+3)/4 = 900Kbps = 0.9Mbps
	*/
	CAN_Init(CAN1, &CAN_InitStructure);
	
}

/*我们还要对 CAN 的过滤器进行配置，使接收 FIFO 只接收特定的报文。
所谓的过滤就是 CAN 接口根据收到报文的 ID，选择是否把该报文保存到接收 FIFO 中。*/

/*STM32 的 ID 过滤方式有两种。一种为标识符列表模式，它把要接收报文的 ID 列成一
个表，要求报文 ID 与列表中的某一个标识符完全相同才可以接收，可以理解为白名单管
理。另一种称为标识符屏蔽模式，它把可接收报文 ID 的某几位作为列表，这几位被称为
屏蔽位，可以把它理解成关键字搜索，只要屏蔽位（关键字）相同就符合要求。*/

static void CAN_Master_Filter_Configuration(void){
	CAN_FilterInitTypeDef CAN_FilterInitStructure;																			//配置CAN过滤器
	
	CAN_FilterInitStructure.CAN_FilterNumber = 0;																				//过滤器组 0  It ranges from 0 to 13. 
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;											//工作在标识符屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;										//过滤器位宽为32位。
	CAN_FilterInitStructure.CAN_FilterIdHigh = (((u32)0x00001314<<3)&0xFFFF0000)>>16;		//要过滤的ID的高16位
	CAN_FilterInitStructure.CAN_FilterIdLow = (((u32)0x00001314<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF;//要过滤的ID的低16位
	/*#define CAN_Id_Extended             ((uint32_t)0x00000004)  Extended Id 		IDE位为隐性位，且报文为数据帧
		#define CAN_RTR_Data                ((uint32_t)0x00000000)  Data frame 			RTR位为显性位				*/
//	赋值时标识符 0x0000 1314 左移三位是因为寄存器的低三位意义为保留位、RTR 位和 IDE 位
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xFFFF;							//Specifies the filter mask number or identification number
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0xFFFF;								//值为 1 的寄存器位就是屏蔽位，值为0 的寄存器位不参与比较。
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;	//Specifies the FIFO (0 or 1) which will be assigned to the filter
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;							//使能过滤器 Enable or disable the filter.
  CAN_FilterInit(&CAN_FilterInitStructure);														//初始化过滤器
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);														//CAN 通信中断使能
//	FIFO0 消息挂号中断使能（CAN_IT_FMP0）在 CAN 接口的 FIFO0 收到报文时，我们在中断服务函数中从 FIFO0 读数据到内存。
}

void CAN_Master_Configuration(void)
{
	CAN_Master_GPIO_Configuration();
	CAN_Master_NVIC_Configuration();
	CAN_Master_Mode_Configuration();
	CAN_Master_Filter_Configuration();
}

//打包并发送报文  如果要触发中断的话	data[]里数据要是0xABCD
void CAN_Master_SetMsg(uint8_t data[])
{	
//	TXMessage.StdId = 0x00;				//由于我们使用拓展ID，所以不需要对他赋值
	TXMessage.ExtId = 0x1314;				//使用拓展的ID
	TXMessage.IDE = CAN_ID_EXT;			//拓展模式
	TXMessage.RTR = CAN_RTR_DATA;		//发送的是数据 Specifies the type of frame for the received message.
	TXMessage.DLC = 2;							//数据长度为2字节
	TXMessage.Data[0] = data[0];		// Contains the data to be received. It ranges from 0 to 0xFF. 
	TXMessage.Data[1] = data[1];	

}

void CAN_Master_SendMsg(void)
{
	CAN_Transmit(CAN1, &TXMessage);	//发送报文
}
	

void USB_LP_CAN1_RX0_IRQHandler(void)					//中断函数名在startup_stm32f10x_xx.s里可看到
{
	CAN_Receive(CAN1, CAN_FIFO0, &RXMessage);		
	if((RXMessage.ExtId == 0x1314) && 					// 比较是否是发送的数据和 ID 
		(RXMessage.IDE == CAN_ID_EXT) && 
		(RXMessage.DLC == 2) && 
		(RXMessage.Data[1] | RXMessage.Data[0]<<8)==0xDCBA)		//判断接受的数据是否为0XDCBA
	{
		RX_FLAG = 0;			//接收成功
	}
	else
		RX_FLAG = 0xff;		//接收失败	
}
	
	