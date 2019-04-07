#include "stm32f10x.h"
#include "stdio.h"

/// 重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
                /* 发送一个字节数据到USART1 */
                USART_SendData(USART1, (uint8_t) ch);
                
                /* 等待发送完毕 */
                while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);                
        
                return (ch);
}
