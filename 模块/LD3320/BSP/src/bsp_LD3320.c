#include "bsp_LD3320.h"

 /** 接线
 * RST      		PB6
 * CS   				PB8
 * WR/SPIS  		PB10
 * P2/SDCK  		PB13     SCK(SPI2)
 * P1/SDO   		PB14     MISO(SPI2)
 * P0/SDI   		PB15     MOSI(SPI2)
 * IRQ      		PA1      
 * MD 接高电平  	PA2
 * RD       		PA0
 * A0       		PB7*/


uint8 nLD_Mode = LD_MODE_IDLE;		//	用来记录当前是在进行ASR识别还是在播放MP3
uint8 ucRegVal;
uint8 nAsrStatus=0;	

int t;
int T=5000;
int t=0;
char sign=0; //亮暗标志
int N=2;
int status=2;   //状态位  用于区别是否需要呼叫‘小桌’ 默认处于呼叫模式

void RCC_Configuration(void)
{
  SystemInit();  
  /* Enable USART1, GPIOA, GPIOx and AFIO clocks */
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 |RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
                         RCC_APB2Periph_GPIOE, ENABLE); //开启相关的AHP外设时钟
  /* Enable USART2 clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
}


/***********************************************************
* 名    称：void LD_reset(void)
* 功    能：LD芯片硬件初始化
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void LD_reset(void)
{
	LD_RST_H();
	LD3320_delay(100);
	LD_RST_L();
	LD3320_delay(100);
	LD_RST_H();
	LD3320_delay(100);
	LD_CS_L();
	LD3320_delay(100);
	LD_CS_H();		
	LD3320_delay(100);
}

/***********************************************************
* 名    称： void LD_Init_Common(void)
* 功    能： 初始化命令
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void LD_Init_Common(void)
{

	LD_ReadReg(0x06);  
	LD_WriteReg(0x17, 0x35); 
	LD3320_delay(5);
	LD_ReadReg(0x06);  

	LD_WriteReg(0x89, 0x03);  
	LD3320_delay(5);
	LD_WriteReg(0xCF, 0x43);   
	LD3320_delay(5);
	LD_WriteReg(0xCB, 0x02);
	
	/*PLL setting*/
	LD_WriteReg(0x11, LD_PLL_11);       
	if (nLD_Mode == LD_MODE_MP3)
	{
		LD_WriteReg(0x1E, 0x00); 
		LD_WriteReg(0x19, LD_PLL_MP3_19);   
		LD_WriteReg(0x1B, LD_PLL_MP3_1B);   
		LD_WriteReg(0x1D, LD_PLL_MP3_1D);
	}
	else
	{
		LD_WriteReg(0x1E,0x00);
		LD_WriteReg(0x19, LD_PLL_ASR_19); 
		LD_WriteReg(0x1B, LD_PLL_ASR_1B);		
	    LD_WriteReg(0x1D, LD_PLL_ASR_1D);
	}
	LD3320_delay(5);
	
	LD_WriteReg(0xCD, 0x04);
	LD_WriteReg(0x17, 0x4c); 
	LD3320_delay(1);
	LD_WriteReg(0xB9, 0x00);
	LD_WriteReg(0xCF, 0x4F); 
	LD_WriteReg(0x6F, 0xFF); 
}

/***********************************************************
* 名    称：void LD_Init_ASR(void)
* 功    能：初始化ASR
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 	
void LD_Init_ASR(void)
{
	nLD_Mode=LD_MODE_ASR_RUN;
	LD_Init_Common();

	LD_WriteReg(0xBD, 0x00);
	LD_WriteReg(0x17, 0x48);	
	LD3320_delay( 5 );
	LD_WriteReg(0x3C, 0x80);    
	LD_WriteReg(0x3E, 0x07);
	LD_WriteReg(0x38, 0xff);    
	LD_WriteReg(0x3A, 0x07);
	LD_WriteReg(0x40, 0);          
	LD_WriteReg(0x42, 8);
	LD_WriteReg(0x44, 0);    
	LD_WriteReg(0x46, 8); 
	LD3320_delay( 1 );
}

/***********************************************************
* 名    称：void ProcessInt0(void)
* 功    能：识别处理函数
* 入口参数：  
* 出口参数：
* 说    明：可以利用查询方式，或中断方式进行调用
* 调用方法： 
**********************************************************/ 
void ProcessInt0(void)
{
	uint8 nAsrResCount=0;

	ucRegVal = LD_ReadReg(0x2B);

		// 语音识别产生的中断
			// （有声音输入，不论识别成功或失败都有中断）
			LD_WriteReg(0x29,0) ;
			LD_WriteReg(0x02,0) ;
	
			if((ucRegVal & 0x10) && LD_ReadReg(0xb2)==0x21 && LD_ReadReg(0xbf)==0x35)		
				{	 
						nAsrResCount = LD_ReadReg(0xba);
			
						if(nAsrResCount>0 && nAsrResCount<=4) 
						{
							nAsrStatus=LD_ASR_FOUNDOK; 				
						}
						else
					    {
							nAsrStatus=LD_ASR_FOUNDZERO;
							}	
				}
			else
			 {
					nAsrStatus=LD_ASR_FOUNDZERO;	//执行没有识别
				}

		  LD_WriteReg(0x2b, 0);
		  LD_WriteReg(0x1C,0);/*写0:ADC不可用*/
		
			LD_WriteReg(0x29,0) ;
			LD_WriteReg(0x02,0) ;
			LD_WriteReg(0x2B,  0);
			LD_WriteReg(0xBA, 0);	
			LD_WriteReg(0xBC,0);	
			LD_WriteReg(0x08,1);	 /*清除FIFO_DATA*/
			LD_WriteReg(0x08,0);	/*清除FIFO_DATA后 再次写0*/

			LD3320_delay(1);

}

/***********************************************************
* 名    称：uint8 LD_Check_ASRBusyFlag_b2(void)
* 功    能：检测 ASR 是否忙状态
* 入口参数：flag ---1：空闲状态  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
uint8 LD_Check_ASRBusyFlag_b2(void)
{
	uint8 j;
	uint8 flag = 0;
	for (j=0; j<10; j++)
	{
		if (LD_ReadReg(0xb2) == 0x21)
		{
			flag = 1;
			break;
		}
		LD3320_delay(10);		
	}
	return flag;
}
/***********************************************************
* 名    称： void LD_AsrStart(void)
* 功    能：
* 入口参数：  ASR初始化
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void LD_AsrStart(void)
{
	LD_Init_ASR();
}
/***********************************************************
* 名    称： uint8 LD_AsrRun(void)
* 功    能： ASR执行函数
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
uint8 LD_AsrRun(void)
{
	LD_WriteReg(0x35, MIC_VOL);
	LD_WriteReg(0x1C, 0x09);
	LD_WriteReg(0xBD, 0x20);
	LD_WriteReg(0x08, 0x01);
	LD3320_delay( 5 );
	LD_WriteReg(0x08, 0x00);
	LD3320_delay( 5);

	if(LD_Check_ASRBusyFlag_b2() == 0)
	{
		return 0;
	}

	LD_WriteReg(0xB2, 0xff);	
	LD_WriteReg(0x37, 0x06);
	LD_WriteReg(0x37, 0x06);
	LD3320_delay( 5);
	LD_WriteReg(0x1C, 0x0b);
	LD_WriteReg(0x29, 0x10);
	
	LD_WriteReg(0xBD, 0x00);   
	return 1;
}

/***********************************************************
* 名    称：uint8 LD_AsrAddFixed(void)
* 功    能：添加识别关键词语
* 入口参数：  
* 出口参数： flag-- 1：success
* 说    明：开发者可以学习"语音识别芯片LD3320高阶秘籍.pdf"中
						关于垃圾词语吸收错误的用法，提高识别率。
* 调用方法： 
**********************************************************/ 
uint8 LD_AsrAddFixed(void)
{
	uint8 k, flag;
	uint8 nAsrAddLength;
	#define DATE_A 10    /*数组二维数值*/
	#define DATE_B 20		/*数组一维数值*/
	 uint8  sRecog[DATE_A][DATE_B] = {
	 																			"kai deng",\
																				"guan deng",\
																				"yi kou yan qi shui",\
																				"zhi zhang",\
		                                    "sha dan",\
		                                    "huan xing mo shi"\
																				};	/*添加关键词，用户修改*/
	 uint8  pCode[DATE_A] = {
	 															CODE_LSD,\
																CODE_SS,\
																CODE_AJCF,\
																CODE_QM,\
		                            CODE_CALL,\
		                            Change_Wake\
															};	/*添加识别码，用户修改*/
	flag = 1;
	for (k=0; k<DATE_A; k++)
	{			
		if(LD_Check_ASRBusyFlag_b2() == 0)
		{
			flag = 0;
			break;
		}
		
		LD_WriteReg(0xc1, pCode[k] );
		LD_WriteReg(0xc3, 0 );
		LD_WriteReg(0x08, 0x04);
		LD3320_delay(1);
		LD_WriteReg(0x08, 0x00);
		LD3320_delay(1);

		for (nAsrAddLength=0; nAsrAddLength<DATE_B; nAsrAddLength++)
		{
			if (sRecog[k][nAsrAddLength] == 0)
				break;
			LD_WriteReg(0x5, sRecog[k][nAsrAddLength]);
		}
		LD_WriteReg(0xb9, nAsrAddLength);
		LD_WriteReg(0xb2, 0xff);
		LD_WriteReg(0x37, 0x04);
//		LD_WriteReg(0x37, 0x04);
	}	 

    return flag;
}

/***********************************************************
* 名    称： uint8 LD_GetResult(void)
* 功    能： 获取识别结果
* 入口参数：  
* 出口参数： 返回识别码 LD_ReadReg(0xc5 );
* 说    明：
* 调用方法： 
**********************************************************/ 
uint8 LD_GetResult(void)
{
	return LD_ReadReg(0xc5 );
}



/***********************************************************
* 名    称：static uint8 spi_send_byte(uint8 byte)
* 功    能： 写SPI
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
static uint8 spi_send_byte(uint8 byte)
{
	/* 循环检测发送缓冲区是否是空 */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

	/*通过SPI3外设发出数据*/
	SPI_I2S_SendData(SPI2,byte);

	/* 等待接收数据，循环检查接收数据缓冲区 */
	while (SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE) == RESET);

	/* 返回读出的数据 */
	return SPI_I2S_ReceiveData(SPI2);
}
/***********************************************************
* 名    称：void LD_WriteReg(uint8 data1,uint8 data2)
* 功    能： 写ld3320寄存器
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void LD_WriteReg(uint8 data1,uint8 data2)
{
	LD_CS_L();

	LD_SPIS_L();

	spi_send_byte(0x04);

	spi_send_byte(data1);

	spi_send_byte(data2);

	LD_CS_H();

}
/***********************************************************
* 名    称：uint8 LD_ReadReg(uint8 reg_add)
* 功    能：读ld3320寄存器
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
uint8 LD_ReadReg(uint8 reg_add)
{
	uint8 i;

	LD_CS_L();

	LD_SPIS_L();

	spi_send_byte(0x05);

	spi_send_byte(reg_add);

	i=spi_send_byte(0x00);	/*读SPI*/

	LD_CS_H();

	return(i);
}

/***********************************************************
* 名    称： void Delay_( int i)
* 功    能： 短延时
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void Delay_( int i)
 {     
    while( i--)
       {	

        }
 }	
/***********************************************************
* 名    称：	LD3320_delay(unsigned long uldata)
* 功    能：	长延时函数
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
 void  LD3320_delay(unsigned long uldata)
{
	unsigned int j  =  0;
	unsigned int g  =  0;
	for (j=0;j<5;j++)
	{
		for (g=0;g<uldata;g++)
		{
			Delay_(120);
		}
	}
}

/***********************************************************
* 名    称：	RunASR(void)
* 功    能：	运行ASR
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
uint8 RunASR(void)
{
	uint8 i=0;
	uint8 asrflag=0;
	for (i=0; i<5; i++)			//	防止由于硬件原因导致LD3320芯片工作不正常，所以一共尝试5次启动ASR识别流程
	{
		LD_AsrStart();			//初始化ASR
		LD3320_delay(100);
		if (LD_AsrAddFixed()==0)	//添加关键词语到LD3320芯片中
		{
			LD_reset();			//	LD3320芯片内部出现不正常，立即重启LD3320芯片
			LD3320_delay(50);			//	并从初始化开始重新ASR识别流程
			continue;
		}

		LD3320_delay(10);

		if (LD_AsrRun() == 0)
		{
			LD_reset();			//	LD3320芯片内部出现不正常，立即重启LD3320芯片
			LD3320_delay(50);			//	并从初始化开始重新ASR识别流程
			continue;
		}

		asrflag=1;
		break;					//	ASR流程启动成功，退出当前for循环。开始等待LD3320送出的中断信号
	}
//       USART_SendData(USART1,asrflag+0x30); /*text.....*/		
	return asrflag;
}

/***********************************************************
* 名    称：LD3320_GPIO_Cfg(void)
* 功    能：初始化需要用到的IO口
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void LD3320_GPIO_Cfg(void)
{	
	  GPIO_InitTypeDef GPIO_InitStructure;
	 //io口配置
	 {
	 	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB ,ENABLE);
		//LD_CS	/RSET/LD_RD
		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6;   //RST A0 CS
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOB,&GPIO_InitStructure);
//		 
		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2;         //PA2接MD 输出高电平
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		 
		GPIO_SetBits(GPIOA,GPIO_Pin_2);	/*MD拉高*/ 
		GPIO_SetBits(GPIOB,GPIO_Pin_7);	/*A0默认拉高*/
//	GPIO_ResetBits(GPIOA,GPIO_Pin_0);	/*RD默认拉高*/
	 }
}
/***********************************************************
* 名    称：LD3320_Spi_cfg(void)
* 功    能：配置SPI功能和端口初始化
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void LD3320_Spi_cfg(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
  //spi端口配置
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 | RCC_APB2Periph_GPIOB,ENABLE);	   //使能SPI3外设时钟
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 

	//P0/P1/P2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;   //SCK MISO MOSI 复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			//spis 片选	CSN    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	LD_CS_H();
	
	//spi功能配置
	SPI_Cmd(SPI2, DISABLE);
	/* SPI3 配置 */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						   //主模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					   //8位
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;						   //时钟极性 空闲状态时，SCK保持低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						   //时钟相位 数据采样从第一个时钟边沿开始
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							   //软件产生NSS
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;   //波特率控制 SYSCLK/128
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   //数据高位在前
	SPI_InitStructure.SPI_CRCPolynomial = 7;							   //CRC多项式寄存器初始值为7
	SPI_Init(SPI2, &SPI_InitStructure);
	/* 使能SPI3 */
	SPI_Cmd(SPI2, ENABLE);

}
/***********************************************************
* 名    称： LD3320_EXTI_Cfg(void) 
* 功    能： 外部中断功能配置和相关端口配置
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void LD3320_EXTI_Cfg(void)    //配置外部IRQ中断
{
  EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  //中断引脚配置
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	//外部中断线配置
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);    //PC1  IRQ
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger =EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  EXTI_GenerateSWInterrupt(EXTI_Line1);
		
	GPIO_SetBits(GPIOA,GPIO_Pin_1);	 //默认拉高中断引脚

	EXTI_ClearFlag(EXTI_Line1);
	EXTI_ClearITPendingBit(EXTI_Line1);
	//中断嵌套配置
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/***********************************************************
* 名    称：  EXTI1_IRQHandler(void)
* 功    能： 外部中断函数
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void EXTI1_IRQHandler(void)
{
	
	if(EXTI_GetITStatus(EXTI_Line1)!= RESET ) 
	{
 		ProcessInt0();
		EXTI_ClearFlag(EXTI_Line1);
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

/***********************************************************
* 名    称：void LED_gpio_cfg(void)
* 功    能：LED端口配置
* 入口参数：  
* 出口参数：
* 说    明：
* 调用方法： 
**********************************************************/ 
void LED_gpio_cfg(void)
{ 
		GPIO_InitTypeDef GPIO_InitStructure;
	
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	   
		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
		GPIO_ResetBits(GPIOA,GPIO_Pin_8);
		GPIO_ResetBits(GPIOD,GPIO_Pin_2);
}



void LD3320_Init(void)
{
	LD3320_GPIO_Cfg();	
	LD3320_EXTI_Cfg();
	LD3320_Spi_cfg();	 
	LED_gpio_cfg();

	LD_reset();
}


void  LD3320_main(void)
{
	uint8 nAsrRes=0,i;
	LD3320_Init();	  

	nAsrStatus = LD_ASR_NONE;		//	初始状态：没有在作ASR
	while(1)
	{

 	switch(nAsrStatus)
		{
			case LD_ASR_RUNING:       
			case LD_ASR_ERROR:		       break; 
										 
			case LD_ASR_NONE:   
											nAsrStatus=LD_ASR_RUNING;
			                           
											if (RunASR()==0)	//	启动一次ASR识别流程：ASR初始化，ASR添加关键词语，启动ASR运算
											{		
												nAsrStatus = LD_ASR_ERROR;
											}
											break;

			case LD_ASR_FOUNDOK:
											 nAsrRes = LD_GetResult( );	//	一次ASR识别流程结束，去取ASR识别结果	
                                             															
												 switch(nAsrRes)		   /*对结果执行相关操作,客户修改*/
												  {
													 case CODE_CALL:      /**唤醒语音识别模块***/
													   
													      status=Call_out_mode ;   //呼叫卤蛋 进入输入指令模式
													               for(i=0;i<3;i++)
														{
														      LED_ON;
															    LD3320_delay(5000);
															    LED_OFF;
															    LD3320_delay(5000);
														
														}   //闪灯提醒 代表小桌收到呼叫命令 进入呼叫模式成功
														break;
														
													 case Change_Wake:
														  if(status==Call_out_mode)
															{
														  		 for(i=0;i<3;i++)   //闪灯提醒 进入唤醒模式成功
														       {
														        LED_ON;
															      LD3320_delay(5000);
															      LED_OFF;
															      LD3320_delay(5000);
														
														        }  status=Wake_mode;//命令执行完成 进入睡眠模式 等待下一次呼叫
															 } 
														    break;
													  case CODE_LSD:			/*命令“开灯”*/
															/****模式：呼叫模式开灯****/
													        if(status==Call_out_mode)
															{
															    LED_ON;  status=Sleep_mode;//命令执行完成 进入睡眠模式 等待下一次呼叫
															 }   
															  
															
															/****模式：唤醒模式开灯***/
															if(status==Wake_mode) 
															{
															    LED_ON;   //执行命令
															  status=Wake_mode;   //执行完毕后 保持唤醒模式的状态
															 }
																
																						 break;
														case CODE_SS:	 /*命令“关灯”*/
															  
													   	/****模式：呼叫模式开灯****/
														     if(status==Call_out_mode)
															{
																LED_OFF;  status=Sleep_mode;//命令执行完成 进入睡眠模式 等待下一次呼叫
															}   
															  
															
															
															
															/****模式：唤醒模式关灯***/
															if(status==Wake_mode) 
															{
															  LED_OFF;   //执行命令
															  status=Wake_mode;   //执行完毕后 保持唤醒模式的状态
															 }
														
														
														
														break;
														case CODE_AJCF:		/*命令“按键触发”*/
												    //PrintCom(USART1,"“按键触发”命令识别成功\r\n"); /*text.....*/
																						break;
														case CODE_QM:		/*命令“全灭”*/
												
													
																						break;
														default:break;
													}	
												nAsrStatus = LD_ASR_NONE;
											break;
			
			case LD_ASR_FOUNDZERO:
			default:
								nAsrStatus = LD_ASR_NONE;
								break;
			}
	}
}

