#include "bsp.h"


/** @addtogroup Optimized I2C examples
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;
/* Buffer of data to be received by I2C1 */
uint8_t Buffer_Rx1[255];
/* Buffer of data to be transmitted by I2C1 */
uint8_t Buffer_Tx1[255] = {0x5, 0x6,0x8,0xA};
/* Buffer of data to be received by I2C2 */
uint8_t Buffer_Rx2[255];
/* Buffer of data to be transmitted by I2C2 */
uint8_t Buffer_Tx2[255] = {0xF, 0xB, 0xC,0xD};
extern __IO uint8_t Tx_Idx1 , Rx_Idx1;
extern __IO uint8_t Tx_Idx2 , Rx_Idx2;

/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/



/**
  * @brief  Main program
  * @param  None
  * @retval : None
  */
int main(){	
	bsp_init();		//�ײ�Ӳ����ʼ��
  
/* Use I2C1 as Master which is communicating with I2C1 of another STM32F10x device */

	
while(1){
	I2C_Master_BufferWrite(I2C1, Buffer_Tx1,120,Interrupt, 0x28);	//���жϷ�ʽд
  I2C_Master_BufferRead(I2C1,Buffer_Rx1,120,Polling, 0x28);			//����ѯ��ʽ��
	//�ж��ļ�stm32f10x_it.c�����ͨ������SLAVE_DMA_USE��ѡ��ʹ��DMA�������������
}	

}
