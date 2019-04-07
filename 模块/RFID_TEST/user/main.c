#include "stdio.h"
#include "stm32f10x.h"
#include "delay.h"
#include "bsp_rfid_rc522.h"
void USART_Config(void);
void PcdAntennaOff ( void );
void PcdAntennaOn ( void );
 
 
int main ( void )
{
  /*�δ�ʱ�ӳ�ʼ��*/
	DelayInit ();   
	
  /*USART1 ����ģʽΪ 115200 8-N-1���жϽ��� */
	USART_Config ();  
	
  /*RC522ģ����������ĳ�ʼ������*/
	RC522_Init ();     
 
	printf ( "WF-RC522 Test\n" );
 
  printf("Please put the IC card on WF-RC522 antenna area ..." );
 
	RC522_Init();
	
  while ( 1 )
  {
    /*IC�����	*/
    IC_test ();
  }	
}

 
