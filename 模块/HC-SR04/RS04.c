#include "stm32f10x.h"
#include <math.h>
 #include "stdio.h"
 
#define module_interval 25    //���峬����ģ����շ���˼��(��λ��mm)
void delay_ms(u16 nms);
void delay_us(u32 nus);
int overflow=0;
u16 TIM=0;
float distance=0.0;
u16 msHcCount = 0;//ms����  
void TIM3_IRQHandler(void)   //TIM3�ж�  
{  
        if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������  
        {  
                TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־   
                msHcCount++;  
        }  
}  
   static void OpenTimerForHc()        //�򿪶�ʱ��  
{  
        TIM_SetCounter(TIM3,0);//�������  
        msHcCount = 0;  
        TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����  
}  
   
static void CloseTimerForHc()        //�رն�ʱ��  
{  
        TIM_Cmd(TIM3, DISABLE);  //ʹ��TIMx����  
}  
   
  
//��ȡ��ʱ��ʱ��  
u32 GetEchoTimer(void)  
{  
        u32 t = 0;  
        t = msHcCount*1000;//�õ�MS  
        t += TIM_GetCounter(TIM3);//�õ�US  
          TIM3->CNT = 0;  //��TIM2�����Ĵ����ļ���ֵ����  
           delay_ms(50);
        return t;  
}  
   
  
//һ�λ�ȡ������������� ���β��֮����Ҫ���һ��ʱ�䣬���ϻ����ź�  
//Ϊ�����������Ӱ�죬ȡ������ݵ�ƽ��ֵ���м�Ȩ�˲���  
float Hcsr04GetLength(void )  
{  
        u32 t = 0;  
        int i = 0;  
        float lengthTemp = 0;  
        float sum = 0;  
        while(i!=5)  
        {  
				GPIO_SetBits(GPIOA,GPIO_Pin_2);
        delay_us(50);
        GPIO_ResetBits(GPIOA,GPIO_Pin_2);
        while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == 0){};//�ȴ����տڸߵ�ƽ���  
            OpenTimerForHc();        //�򿪶�ʱ��  
            i = i + 1;  
            while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == 1);  
            CloseTimerForHc();        //�رն�ʱ��  
            t = GetEchoTimer();        //��ȡʱ��,�ֱ���Ϊ1US  
            lengthTemp = (float)sqrt((t/20.0*17)*(t/20.0*17)-module_interval*module_interval/4.0)+12.0;//cm  
		
            sum = lengthTemp + sum ;  
          
    }  
        lengthTemp = sum/5000.0;  
        return lengthTemp;  
}  
  
  
