#include "stm32f10x.h"
#include "stdio.h"

/// �ض���c�⺯��printf��USART1
int fputc(int ch, FILE *f)
{
                /* ����һ���ֽ����ݵ�USART1 */
                USART_SendData(USART1, (uint8_t) ch);
                
                /* �ȴ�������� */
                while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);                
        
                return (ch);
}
