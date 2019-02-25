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
//SPI��������
void SPI_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ������ʱ��
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
//SPI���ú���
void SPI_Configuration(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//ʹ��SPI1ʱ��
	
	//SPI��ʼ��
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;		//Ԥ��Ƶֵ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;													//���ݲ����ڵڶ�������
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;														//ʱ����idleΪ�ߵ�ƽ
	SPI_InitStructure.SPI_CRCPolynomial = 7;															//CRCУ�����Ķ���ʽ������λ����
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;											//֡��ʽΪ8λ
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		//˫��ȫ˫��
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;										//�������ȴ�����ֽ�Ϊ�����Чλ	(the Most Significant Bit)
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;													//����ģʽ
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;															//�������NSS�ź�
	
	SPI_Init(SPI1,&SPI_InitStructure);																		//��ʼ��SPI1
	SPI_Cmd(SPI1, ENABLE);																								//ʹ��SPI1
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

//SPI�ǻ��νṹ���ж�����д
//����ʱ��Ҫע��һ�����⣬��Ϊ��ģʽ��û���ṩʱ�ӵģ�������ģʽ�±���Ҫ�ڽ��յ�ͬʱ�ṩʱ�ӡ�
//�취���Ƿ���һ���ֽ���ʵ�֣���Ϊ��������˵�ģ�����һ���ֽھ���ζ���յ�һ���ֽڣ������д��ȫһ����ֻҪ�Ѷ��������ֽڱ��漴�ɡ�







