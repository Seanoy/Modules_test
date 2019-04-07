#include "bsp.h" 


void RCC_Configuration(void);	/* ��ʼ��ϵͳʱ�� */
void GPIO_Configuration(void);	/* IO�ڵ�����*/
void NVIC_Configuration(void);	/* ���ȼ����� */
void USART1_Configuration(void);	/* �������� */
void TIM2_Configuration(void);	/* ��ʱ�� */
void TIM2_Configuration(void);
void TIM4_Configuration(void);	/* ��ʱ�� */
void TIM4_Configuration(void);	/* PWM���� */
void EXTI_Configuration(void);	/* �ⲿ�ж����� */


void  BSP_Init (void)
{
	RCC_Configuration();	/* ��ʼ��ϵͳʱ�� */
	GPIO_Configuration();	/* IO������ */
	USART1_Configuration();	/* ���� */
	TIM2_Configuration();	/* ��ʱ������ */
	TIM2_Configuration();
  TIM4_Configuration();	/* ��ʱ������ */
	TIM4_Configuration();	/* PWM���� */
	EXTI_Configuration();	/* �ⲿ�ж�����  */
	NVIC_Configuration();	/* ���ȼ� */
}
/*********************************************************************************************************
* ��    �� ��RCC_Configuration
* �������� : ��ʼ��ϵͳʱ��
* ������� : none
* ���ز��� : none.
* ��    �� : 
* ��    �� �������ڡ��޸��������޸�ԭ��
* ����˵�� : �����⹦��˵�������磺�в������ȣ�
*********************************************************************************************************/
void RCC_Configuration(void)
{
	SystemInit(); 

	/*APB1 36MHz ����timerʱ�ᱶƵΪ72mhz����������Щ�ڴ�ʱ���Ͽ�����ȥ�鿴*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 |RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4
						, ENABLE);
	
	/*APB2 72MHz ����timerʱ�ᱶƵΪ144mhz����������Щ�ڴ�ʱ���Ͽ�����ȥ�鿴*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB
  						| RCC_APB2Periph_GPIOC| RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE
						| RCC_APB2Periph_USART1, ENABLE);
}

/*********************************************************************************************************
* ��    �� ��GPIO_Configuration
* �������� : ��ʼ��IO������
* ������� : none
* ���ز��� : none.
* ��    �� : 
* ��    �� �������ڡ��޸��������޸�ԭ��
* ����˵�� : �����⹦��˵�������磺�в������ȣ�
******************* **************************************************************************************/ 
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
    /* LED�Ƶ����� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			 //ͨ���������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //���ģʽ����ٶ�50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			 //ͨ���������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //���ģʽ����ٶ�50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* ���������� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			//���ó���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   //����ģʽ����ٶ�50MHZ
	GPIO_Init(GPIOC, &GPIO_InitStructure);						    
}
/*********************************************************************************************************
* ��    �� ��USART1_Configuration
* �������� : ��ʼ������1
* ������� : none
* ���ز��� : none.
* ��    �� : 
* ��    �� �������ڡ��޸��������޸�ԭ��
* ����˵�� : �����⹦��˵�������磺�в������ȣ�
*********************************************************************************************************/
void USART1_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		  //��������ģʽ	   
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			  //�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 38400;						//����115200bps
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//����λ8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//ֹͣλ1λ
	USART_InitStructure.USART_Parity = USART_Parity_No;				//��У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //��Ӳ������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ
	/* Configure USART1 */
	USART_Init(USART1, &USART_InitStructure);							//���ô��ڲ�������
	/* Enable USART1 Receive and Transmit interrupts */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                    //ʹ�ܽ����ж�
	/* Enable the USART1 */
	USART_Cmd(USART1, ENABLE);	                  //ʹ�ܴ���
}

/*******************************************************************************
* Function Name  : TIM2_Configuration
* Description    : ��ʱ�� 1ms
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM4_Configuration(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;	                   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		 //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	        	
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = (100- 1);					//arr  ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = (7200-1);	 			 	//��Ƶ	psc
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				 	//ʱ�ӷָһ��д0
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	 	//����Ϊ���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel1*/
	TIM_OCInitStructure.TIM_Pulse = 0;						// ��ʼֵ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;			   //pwmģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//ʹ��ͨ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	//�ߵ�ƽ
	
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);	                  
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);			//ʹ�ܻ���ʧ��TIMx��CCR3�ϵ�Ԥװ�ؼĴ���
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);	                  
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);	                  
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);			//ʹ�ܻ���ʧ��TIMx��CCR3�ϵ�Ԥװ�ؼĴ���
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);	                  
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

	
	TIM_ARRPreloadConfig(TIM4, ENABLE);  /* �ж�ʹ�� */
	/* TIM3 enable counter */
	TIM_Cmd(TIM4, ENABLE);	/* ʹ�� */
}


/*********************************************************************************************************
* ��    �� ��TIM3_Configuration
* �������� : ��ʼ����ʱ��  PWM
* ������� : none
* ���ز��� : none.
* ��    �� : 
* ��    �� �������ڡ��޸��������޸�ԭ��
* ����˵�� : �����⹦��˵�������磺�в������ȣ�
*********************************************************************************************************/
void TIM2_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;	                   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		 //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	        	
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = (100- 1);					//arr  ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = (7200-1);	 			 	//��Ƶ	psc
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				 	//ʱ�ӷָһ��д0
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	 	//����Ϊ���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel1*/
	TIM_OCInitStructure.TIM_Pulse = 0;						// ��ʼֵ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;			   //pwmģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//ʹ��ͨ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	//�ߵ�ƽ
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);	                  
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);			//ʹ�ܻ���ʧ��TIMx��CCR3�ϵ�Ԥװ�ؼĴ���
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);	                  
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);	                  
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);			//ʹ�ܻ���ʧ��TIMx��CCR3�ϵ�Ԥװ�ؼĴ���
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);	                  
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

	
	TIM_ARRPreloadConfig(TIM2, ENABLE);  /* �ж�ʹ�� */
	/* TIM3 enable counter */
	TIM_Cmd(TIM2, ENABLE);	/* ʹ�� */
}

/*******************************************************************************
* Function Name  : TIM4_Configuration
* Description    : ��ʱ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/


/*******************************************************************************
* Function Name  : TIM5_Configuration
* Description    : ������ģʽ						   PA0 PA1
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM5_Configuration(void)
{
	
}
/*******************************************************************************
* Function Name  : TIM6_Configuration
* Description    : pid��ʱ����
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM6_Configuration(void)
{ 
	
}
/*******************************************************************************
* Function Name  : TIM7_Configuration
* Description    : ��ʱ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM7_Configuration(void)
{		
}

/*******************************************************************************
* Function Name  : EXTI_Configuration		   �ж�����
* Description    : None
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI_Configuration(void)
{
 
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
    //GPIOC.5 �ж����Լ��жϳ�ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                        //���ó���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   
	GPIO_Init(GPIOC, &GPIO_InitStructure);                               //�����Ƕ�����Ҫ�õ���I/O�ڵ�����

  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource5);          //ѡ��gpioB11����Ϊ�ⲿ�ж�ʹ��
  	EXTI_InitStructure.EXTI_Line=EXTI_Line5;                            //ʹ���ⲿ�ж���5 
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	                //�ⲿ�ж�
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;             //�½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;                           //�ж���ʹ��
  	EXTI_Init(&EXTI_InitStructure);	
}

/*********************************************************************************************************
* ��    �� ��NVIC_Configuration
* �������� : ��ʼ���ж����ȼ�
* ������� : none
* ���ز��� : none.
* ��    �� : 
* ��    �� �������ڡ��޸��������޸�ԭ��
* ����˵�� : �����⹦��˵�������磺�в������ȣ�
*********************************************************************************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	            //ֻ���޸�һ��
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	   	//�����ж� 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		   //��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			   //�����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//ʹ��
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;			/* TIM2�ж� */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		//��ռ���ȼ�2��    
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			    //�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ�� 
	NVIC_Init(&NVIC_InitStructure);
		
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�2�� 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); 

}



