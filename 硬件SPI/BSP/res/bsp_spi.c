#include "bsp_spi.h"


#define SPI_SCLK_PORT		GPIOA
#define SPI_MISO_PORT		GPIOA
#define SPI_MOSI_PORT		GPIOA
#define SPI_CS_PORT			GPIOA

#define SPI_SCLK_PIN		GPIO_Pin_5
#define SPI_MISO_PIN		GPIO_Pin_6
#define SPI_MOSI_PIN		GPIO_Pin_7
#define SPI_CS_PIN			GPIO_Pin_4

#define SPI_CS_ENABLE			GPIO_ResetBits(SPI_CS_PORT,SPI_CS_PIN)
#define SPI_CS_DISENABLE	GPIO_SetBits(SPI_CS_PORT,SPI_CS_PIN)

/*******************************************************
*		FunctionName:	SPI_GPIO_Config
*		Function:			Configure SPI_GPIO
*		Arguments:		None
*		Return:				None
*		Author				seanOY
********************************************************/
//SPI引脚设置
void SPI_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能引脚时钟
	//SCLK configuration
  GPIO_InitStructure.GPIO_Pin = SPI_SCLK_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(SPI_SCLK_PORT, &GPIO_InitStructure);

  //MISO configuration
  GPIO_InitStructure.GPIO_Pin = SPI_MISO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(SPI_MISO_PORT, &GPIO_InitStructure);			
   
  //MOSI configuration
  GPIO_InitStructure.GPIO_Pin = SPI_MOSI_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(SPI_MOSI_PORT, &GPIO_InitStructure);    
  
  //CS configuration
  GPIO_InitStructure.GPIO_Pin = SPI_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(SPI_CS_PORT, &GPIO_InitStructure);	
	
}

/*******************************************************
*		FunctionName:	SPI_Configuration
*		Function:			Configure SPI
*		Arguments:		None
*		Return:				None
*		Author				seanOY
********************************************************/
//SPI配置函数
void SPI_Configuration(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//使能SPI1时钟
	
	//SPI初始化
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;		//预分频值
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;													//数据捕获于第二个边沿
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;														//时钟在idle为高电平
	SPI_InitStructure.SPI_CRCPolynomial = 7;															//CRC校验计算的多项式的数据位长度
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;											//帧格式为8位
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		//双线全双工
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;										//设置最先传输的字节为最高有效位	(the Most Significant Bit)
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;													//主机模式
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;															//软件控制NSS信号
	
	SPI_Init(SPI1,&SPI_InitStructure);																		//初始化SPI1
	SPI_Cmd(SPI1, ENABLE);																								//使能SPI1
}

/*******************************************************
*		FunctionName:	SPI_Writebyte
*		Function:			Write a byte
*		Arguments:		data
*		Return:				None
*		Author				seanOY
********************************************************/
void SPI_Writebyte(unsigned char data)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, data);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	SPI_I2S_ReceiveData(SPI1);
}
 
/*******************************************************
*		FunctionName:	SPI_Readbyte
*		Function:			Read a byte
*		Arguments:		data
*		Return:				None
*		Author				seanOY
********************************************************/
unsigned char SPI_Readbyte(unsigned char data)
{
while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
SPI_I2S_SendData(SPI1, data);
while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
return SPI_I2S_ReceiveData(SPI1);
}

//SPI是环形结构，有读就有写
//读的时候要注意一个问题，因为从模式是没法提供时钟的，所以主模式下必须要在接收的同时提供时钟。
//办法就是发送一个字节来实现，因为还是上面说的，发送一个字节就意味着收到一个字节，代码和写完全一样，只要把读出来的字节保存即可。







