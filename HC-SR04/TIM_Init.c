#include "stm32f10x.h"


	void TIMx_NVIC_Config(void){
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//����NVICΪ������1
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_1 );
	
	//�����ж�Դ��TIM3
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	
	//������ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	
	//���������ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	
	//ʹ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}
 

 void TIM_Config(void){
	 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	 
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
	 
	TIM_TimeBaseStructure.TIM_Period = 5000;
	 
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	 
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	 
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
 
	TIM_ARRPreloadConfig(TIM3,ENABLE);
	
	TIM_Cmd(TIM3,DISABLE);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	TIMx_NVIC_Config();
	

}