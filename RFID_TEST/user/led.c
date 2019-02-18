#include "stm32f10x.h"
/*  GPIOC PIN3 LED*  
 *  GPIOA PIN6 KEY*/
	void Delay(__IO uint32_t nCount)
{
   for(; nCount != 0; nCount--) ;
}

	void GPIO_Config(void){
		
		GPIO_InitTypeDef GPIO_InitStructure;  								   //����һ��GPIO_InitTypeDef���͵Ľṹ��
		
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC , ENABLE ); //����GPIOC������ʱ�� 
																																		
		/*   LED���  */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  						//ѡ��GPIOC PIN 3 
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    	//�������
		
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;      //ѡ�йܽ�����
		
		GPIO_Init( GPIOC , &GPIO_InitStructure );      		 		//�����������ó�ʼ��GPIOC
				
	  GPIO_SetBits( GPIOC, GPIO_Pin_3 );  								  //LED����,��ʼ��ʹ��Ĭ��Ϩ��
		
	}


void NVIC_Config(void){
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//����NVICΪ������1
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_1 );
	
	//�����ж�Դ������
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	
	//������ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	
	//���������ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	
	//ʹ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}
	
void EXIT_Key_Config(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA ,ENABLE);
	
	NVIC_Config();
	/*   �������  */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;            		//����6������������
		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //�������� 
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;    	  //ѡ�йܽ�����
		
	GPIO_Init(GPIOA, &GPIO_InitStructure);									//��ʼ������ 
		
	GPIO_ResetBits( GPIOA, GPIO_Pin_6);    									//���ð������ŵ�ƽ
	
	/*ѡ��EXTI���ź�Դ*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	
	/*EXTIΪ�ж�ģʽ*/
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	
	/*�½����ж�*/
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	
	/*ʹ���ж�*/
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
}
	
	
