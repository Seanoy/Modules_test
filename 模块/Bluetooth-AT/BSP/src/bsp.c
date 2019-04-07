#include "bsp.h" 


void RCC_Configuration(void);	/* 初始化系统时钟 */
void GPIO_Configuration(void);	/* IO口的配置*/
void NVIC_Configuration(void);	/* 优先级配置 */
void USART1_Configuration(void);	/* 串口配置 */
void TIM2_Configuration(void);	/* 定时器 */
void TIM2_Configuration(void);
void TIM4_Configuration(void);	/* 定时器 */
void TIM4_Configuration(void);	/* PWM配置 */
void EXTI_Configuration(void);	/* 外部中断配置 */


void  BSP_Init (void)
{
	RCC_Configuration();	/* 初始化系统时钟 */
	GPIO_Configuration();	/* IO口配置 */
	USART1_Configuration();	/* 串口 */
	TIM2_Configuration();	/* 定时器配置 */
	TIM2_Configuration();
  TIM4_Configuration();	/* 定时器配置 */
	TIM4_Configuration();	/* PWM配置 */
	EXTI_Configuration();	/* 外部中断配置  */
	NVIC_Configuration();	/* 优先级 */
}
/*********************************************************************************************************
* 名    称 ：RCC_Configuration
* 功能描述 : 初始化系统时钟
* 输入参数 : none
* 返回参数 : none.
* 作    者 : 
* 修    改 ：（日期、修改人名、修改原因）
* 特殊说明 : （特殊功能说明，例如：有参数检查等）
*********************************************************************************************************/
void RCC_Configuration(void)
{
	SystemInit(); 

	/*APB1 36MHz 配置timer时会倍频为72mhz，具体有哪些在此时钟上可跳进去查看*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 |RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4
						, ENABLE);
	
	/*APB2 72MHz 配置timer时会倍频为144mhz，具体有哪些在此时钟上可跳进去查看*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB
  						| RCC_APB2Periph_GPIOC| RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE
						| RCC_APB2Periph_USART1, ENABLE);
}

/*********************************************************************************************************
* 名    称 ：GPIO_Configuration
* 功能描述 : 初始化IO口配置
* 输入参数 : none
* 返回参数 : none.
* 作    者 : 
* 修    改 ：（日期、修改人名、修改原因）
* 特殊说明 : （特殊功能说明，例如：有参数检查等）
******************* **************************************************************************************/ 
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
    /* LED灯的配置 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			 //通用推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //输出模式最大速度50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			 //通用推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 //输出模式最大速度50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* 按键的配置 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			//设置成上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   //输入模式最大速度50MHZ
	GPIO_Init(GPIOC, &GPIO_InitStructure);						    
}
/*********************************************************************************************************
* 名    称 ：USART1_Configuration
* 功能描述 : 初始化串口1
* 输入参数 : none
* 返回参数 : none.
* 作    者 : 
* 修    改 ：（日期、修改人名、修改原因）
* 特殊说明 : （特殊功能说明，例如：有参数检查等）
*********************************************************************************************************/
void USART1_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		  //浮空输入模式	   
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			  //复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 38400;						//速率115200bps
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//数据位8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//停止位1位
	USART_InitStructure.USART_Parity = USART_Parity_No;				//无校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //无硬件流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式
	/* Configure USART1 */
	USART_Init(USART1, &USART_InitStructure);							//配置串口参数函数
	/* Enable USART1 Receive and Transmit interrupts */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                    //使能接收中断
	/* Enable the USART1 */
	USART_Cmd(USART1, ENABLE);	                  //使能串口
}

/*******************************************************************************
* Function Name  : TIM2_Configuration
* Description    : 定时器 1ms
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		 //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	        	
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = (100- 1);					//arr  设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = (7200-1);	 			 	//分频	psc
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				 	//时钟分割，一般写0
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	 	//设置为向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel1*/
	TIM_OCInitStructure.TIM_Pulse = 0;						// 初始值
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;			   //pwm模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//使能通道
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	//高电平
	
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);	                  
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);			//使能或者失能TIMx在CCR3上的预装载寄存器
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);	                  
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);	                  
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);			//使能或者失能TIMx在CCR3上的预装载寄存器
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);	                  
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

	
	TIM_ARRPreloadConfig(TIM4, ENABLE);  /* 中断使能 */
	/* TIM3 enable counter */
	TIM_Cmd(TIM4, ENABLE);	/* 使能 */
}


/*********************************************************************************************************
* 名    称 ：TIM3_Configuration
* 功能描述 : 初始化定时器  PWM
* 输入参数 : none
* 返回参数 : none.
* 作    者 : 
* 修    改 ：（日期、修改人名、修改原因）
* 特殊说明 : （特殊功能说明，例如：有参数检查等）
*********************************************************************************************************/
void TIM2_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;	                   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		 //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	        	
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = (100- 1);					//arr  设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = (7200-1);	 			 	//分频	psc
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				 	//时钟分割，一般写0
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	 	//设置为向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel1*/
	TIM_OCInitStructure.TIM_Pulse = 0;						// 初始值
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;			   //pwm模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//使能通道
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	//高电平
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);	                  
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);			//使能或者失能TIMx在CCR3上的预装载寄存器
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);	                  
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);	                  
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);			//使能或者失能TIMx在CCR3上的预装载寄存器
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);	                  
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

	
	TIM_ARRPreloadConfig(TIM2, ENABLE);  /* 中断使能 */
	/* TIM3 enable counter */
	TIM_Cmd(TIM2, ENABLE);	/* 使能 */
}

/*******************************************************************************
* Function Name  : TIM4_Configuration
* Description    : 定时器
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/


/*******************************************************************************
* Function Name  : TIM5_Configuration
* Description    : 编码器模式						   PA0 PA1
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM5_Configuration(void)
{
	
}
/*******************************************************************************
* Function Name  : TIM6_Configuration
* Description    : pid定时处理
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM6_Configuration(void)
{ 
	
}
/*******************************************************************************
* Function Name  : TIM7_Configuration
* Description    : 定时器
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM7_Configuration(void)
{		
}

/*******************************************************************************
* Function Name  : EXTI_Configuration		   中断配置
* Description    : None
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI_Configuration(void)
{
 
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
    //GPIOC.5 中断线以及中断初始化配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                        //设置成上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   
	GPIO_Init(GPIOC, &GPIO_InitStructure);                               //以上是对于需要用到的I/O口的配置

  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource5);          //选择gpioB11口作为外部中断使用
  	EXTI_InitStructure.EXTI_Line=EXTI_Line5;                            //使能外部中断线5 
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	                //外部中断
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;             //下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;                           //中断线使能
  	EXTI_Init(&EXTI_InitStructure);	
}

/*********************************************************************************************************
* 名    称 ：NVIC_Configuration
* 功能描述 : 初始化中断优先级
* 输入参数 : none
* 返回参数 : none.
* 作    者 : 
* 修    改 ：（日期、修改人名、修改原因）
* 特殊说明 : （特殊功能说明，例如：有参数检查等）
*********************************************************************************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	            //只需修改一次
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	   	//串口中断 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		   //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			   //子优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//使能
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;			/* TIM2中断 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		//抢占优先级2，    
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			    //子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能 
	NVIC_Init(&NVIC_InitStructure);
		
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级2， 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 

}



