#include "stm32f10x.h"
#include <math.h>
 #include "stdio.h"
 
#define module_interval 25    //定义超声波模块接收发射端间距(单位：mm)
void delay_ms(u16 nms);
void delay_us(u32 nus);
int overflow=0;
u16 TIM=0;
float distance=0.0;
u16 msHcCount = 0;//ms计数  
void TIM3_IRQHandler(void)   //TIM3中断  
{  
        if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否  
        {  
                TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志   
                msHcCount++;  
        }  
}  
   static void OpenTimerForHc()        //打开定时器  
{  
        TIM_SetCounter(TIM3,0);//清除计数  
        msHcCount = 0;  
        TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设  
}  
   
static void CloseTimerForHc()        //关闭定时器  
{  
        TIM_Cmd(TIM3, DISABLE);  //使能TIMx外设  
}  
   
  
//获取定时器时间  
u32 GetEchoTimer(void)  
{  
        u32 t = 0;  
        t = msHcCount*1000;//得到MS  
        t += TIM_GetCounter(TIM3);//得到US  
          TIM3->CNT = 0;  //将TIM2计数寄存器的计数值清零  
           delay_ms(50);
        return t;  
}  
   
  
//一次获取超声波测距数据 两次测距之间需要相隔一段时间，隔断回响信号  
//为了消除余震的影响，取五次数据的平均值进行加权滤波。  
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
        while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == 0){};//等待接收口高电平输出  
            OpenTimerForHc();        //打开定时器  
            i = i + 1;  
            while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1) == 1);  
            CloseTimerForHc();        //关闭定时器  
            t = GetEchoTimer();        //获取时间,分辨率为1US  
            lengthTemp = (float)sqrt((t/20.0*17)*(t/20.0*17)-module_interval*module_interval/4.0)+12.0;//cm  
		
            sum = lengthTemp + sum ;  
          
    }  
        lengthTemp = sum/5000.0;  
        return lengthTemp;  
}  
  
  
