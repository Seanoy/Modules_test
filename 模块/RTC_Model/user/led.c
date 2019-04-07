#include "stm32f10x.h"
/*  GPIOC PIN3 LED*  
 *  GPIOA PIN6 KEY*/
	void Delay(__IO uint32_t nCount)
{
   for(; nCount != 0; nCount--) ;
}

	void GPIO_Config(void){
		
		GPIO_InitTypeDef GPIO_InitStructure;  								   //定义一个GPIO_InitTypeDef类型的结构体
		
		RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC , ENABLE ); //开启GPIOC的外设时钟 
																																		
		/*   LED相关  */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  						//选择GPIOC PIN 3 
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    	//推挽输出
		
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;      //选中管脚速率
		
		GPIO_Init( GPIOC , &GPIO_InitStructure );      		 		//按照上述设置初始化GPIOC
				
	  GPIO_SetBits( GPIOC, GPIO_Pin_3 );  								  //LED共阳,初始化使其默认熄灭
		
	}


void NVIC_Config(void){
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//配置NVIC为优先组1
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_1 );
	
	//配置中断源：按键
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	
	//配置抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	
	//配置子优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	
	//使能中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}
	
void EXIT_Key_Config(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA ,ENABLE);
	
	NVIC_Config();
	/*   按键相关  */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;            		//引脚6用作按键输入
		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //浮空输入 
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;    	  //选中管脚速率
		
	GPIO_Init(GPIOA, &GPIO_InitStructure);									//初始化按键 
		
	GPIO_ResetBits( GPIOA, GPIO_Pin_6);    									//重置按键引脚电平
	
	/*选择EXTI的信号源*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	
	/*EXTI为中断模式*/
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	
	/*下降沿中断*/
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	
	/*使能中断*/
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
}
	
	
