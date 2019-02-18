#include "stdio.h"
#include "stm32f10x.h"
#include "delay.h"
#include "bsp_rfid_rc522.h"
void USART_Config(void);
void PcdAntennaOff ( void );
void PcdAntennaOn ( void );
 
 
int main ( void )
{
  /*滴答时钟初始化*/
	DelayInit ();   
	
  /*USART1 配置模式为 115200 8-N-1，中断接收 */
	USART_Config ();  
	
  /*RC522模块所需外设的初始化配置*/
	RC522_Init ();     
 
	printf ( "WF-RC522 Test\n" );
 
  printf("Please put the IC card on WF-RC522 antenna area ..." );
 
	RC522_Init();
	
  while ( 1 )
  {
    /*IC卡检测	*/
    IC_test ();
  }	
}

 
