#include "stm32f10x.h"
#include "stdio.h"
#include "bsp.h"
void USART_Config(void);
extern void  BSP_Init (void);
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...);
int fputc(int ch,FILE  *f);
char *itoa(int value, char *string, int radix);
int x=0;		//用于接收蓝牙的信息


int main(void)
{
	
	BSP_Init();

	while(1)
	{
			if(x == 1)
			{
          TIM2->CCR1 = 0;
          TIM2->CCR2 = 99;
				  TIM2->CCR3= 0;
          TIM2->CCR4 = 99;
				  TIM4->CCR1 = 0;
          TIM4->CCR2 = 99;
				  TIM4->CCR3= 0;
          TIM4->CCR4 = 99;
			}	
     if(x == 2)
			{
          TIM2->CCR1 = 0;
          TIM2->CCR2 = 0;
				  TIM2->CCR3 = 0;
          TIM2->CCR4 = 0;
				  TIM4->CCR1 = 0;
          TIM4->CCR2 = 0;
				  TIM4->CCR3 = 0;
          TIM4->CCR4 = 0;
			}
			if(x == 3)
			{
          TIM2->CCR1 = 99;
          TIM2->CCR2 = 0;
				  TIM2->CCR3 = 99;
          TIM2->CCR4 = 0;
				  TIM4->CCR1 = 99;
          TIM4->CCR2 = 0;
				  TIM4->CCR3 = 99;
          TIM4->CCR4 = 0;
			}
     if(x == 4)
			{
          TIM2->CCR1 = 0;
          TIM2->CCR2 = 99;
				  TIM2->CCR3 = 99;
          TIM2->CCR4 = 0;
				  TIM4->CCR1 =0;
          TIM4->CCR2 =99;
				  TIM4->CCR3 = 99;
          TIM4->CCR4 = 0;
			}
     if(x == 5)
			{
          TIM2->CCR1 = 99;
          TIM2->CCR2 = 0;
				  TIM2->CCR3 = 0;
          TIM2->CCR4 = 99;
				  TIM4->CCR1 = 99;
          TIM4->CCR2 = 0;
				  TIM4->CCR3 = 0;
          TIM4->CCR4 = 99;
			}													
	}	

}
/*********************************************************
										printf重定义
**********************************************************/
int fputc(int ch,FILE  *f)
{
	USART_SendData(USART1,(u8)ch);
	while (USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
	return ch;
}

/********************************************以下不要擅自乱修改*****************************************/
/******************************************************
		格式化串口输出函数
        "\r"	回车符	   USART_OUT(USART1, "abcdefg\r")   
		"\n"	换行符	   USART_OUT(USART1, "abcdefg\r\n")
		"%s"	字符串	   USART_OUT(USART1, "字符串是：%s","abcdefg")
		"%d"	十进制	   USART_OUT(USART1, "a=%d",10)
**********************************************************/
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...)
{ 

	const char *s;
    int d;
    char buf[16];
    va_list ap;
    va_start(ap, Data);

	while(*Data!=0){				                          //判断是否到达字符串结束符
		if(*Data==0x5c){									  //'\'
			switch (*++Data){
				case 'r':							          //回车符
					USART_SendData(USARTx, 0x0d);	   

					Data++;
					break;
				case 'n':							          //换行符
					USART_SendData(USARTx, 0x0a);	
					Data++;
					break;
				
				default:
					Data++;
				    break;
			}
			
			 
		}
		else if(*Data=='%'){									  //
			switch (*++Data){				
				case 's':										  //字符串
                	s = va_arg(ap, const char *);
                	for ( ; *s; s++) {
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
            	case 'd':										  //十进制
                	d = va_arg(ap, int);
                	itoa(d, buf, 10);
                	for (s = buf; *s; s++) {
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
				default:
					Data++;
				    break;
			}		 
		}
		else USART_SendData(USARTx, *Data++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
	}
}
/******************************************************
		整形数据转字符串函数
        char *itoa(int value, char *string, int radix)
		radix=10 标示是10进制	非十进制，转换结果为0;  

	    例：d=-379;
		执行	itoa(d, buf, 10); 后
		
		buf="-379"							   			  
**********************************************************/
char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;
    return string;

} 
