#include "rtc.h"
_calendar_obj calendar;    //ʱ�ӽṹ��
//ƽ�����·����ڱ�
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
/*rtc�ж���������*/
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void RTC_Configuration(void)

{
    /* ʹ��PWR��BKPʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP,ENABLE);
    /* ʹ�ܶԺ󱸼Ĵ����ķ��� */ 
    PWR_BackupAccessCmd(ENABLE);
    /* ��λBKP�Ĵ��� */ 
    BKP_DeInit();
    /* ʹ��LSE */ 
    RCC_LSEConfig(RCC_LSE_ON);
    /*�ȴ�������� */
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) {}
    /* �� RTCʱ������ΪLSE���32.768KHZ�ľ���*/ 
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    /* ʹ��RTC Clock */ 
    RCC_RTCCLKCmd(ENABLE);
    /* �ȴ�ͬ�� */ 
    RTC_WaitForSynchro();
    /* �ȴ���RTC�Ĵ�������д�������*/             
    RTC_WaitForLastTask();
    /* ������Ԥ��Ƶֵ: ����RTCʱ������Ϊ1s */
    RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1)*/
    /* �ȴ���RTC�Ĵ�������д������� */
    RTC_WaitForLastTask();
    /* ʹ��RTC���ж� */ 
    RTC_ITConfig(RTC_IT_SEC, ENABLE);
    /* �ȴ���RTC�Ĵ�������д������� */         
    RTC_WaitForLastTask();
		}
void RTC_Init(void)
{
    /*����ǵ�һ������ʱ�ӣ���ִ��RCC_Configuration()��������*/
    if(BKP_ReadBackupRegister(BKP_DR1)!=0x1016)
    {
            RTC_Configuration();
            RTC_Set(2019,1,26,18,38,30);
            GPIO_SetBits(GPIOD, GPIO_Pin_13);//����D1
            BKP_WriteBackupRegister(BKP_DR1, 0x1016);//��ִ�еĺ󱸼Ĵ�����д���û���������
    }
    else
    {
        RTC_WaitForSynchro();//�ȴ�RTC�Ĵ���ͬ�����
        RTC_ITConfig(RTC_IT_SEC, ENABLE);//ʹ��RTC���ж�
        RTC_WaitForLastTask();//�ȴ����һ�ζ�RTC�Ĵ�����д�������
        GPIO_SetBits(GPIOG, GPIO_Pin_14);//����D2
    }
    NVIC_Configuration();
    RTC_Get();//����ʱ��
}

u8 Is_Leap_Year(u16 pyear)
{
    if(pyear%4==0)//�������ܱ�4����
    {
        if(pyear%100==0)
        {
            if(pyear%400==0)    return 1;//�����00��β����Ҫ�ܱ�400����
            else    return 0;
        }
        else
            return 1;
    }
    else
        return 0;
}
/*
*����ʱ��
*�������ʱ��ת��Ϊ����
*��1970��1��1��Ϊ��׼
*1970~2099��Ϊ�Ϸ����
����ֵ��0���ɹ�������������
*/
u8 RTC_Set(u16 year,u8 mon,u8 day,u8 hour,u8 min,u8 sec)
{
    u16 t;
    u32 secCount=0;
    if(year<1970||year>2099)
        return 1;//3?�䨪
    for(t=1970;t<year;t++)    //��������ݵ��������
    {
        if(Is_Leap_Year(t))//����
            secCount+=31622400;//�����������
        else
            secCount+=31536000;    
    }
    mon-=1;//�ȼ���һ����������������������5��10�գ���ֻ��Ҫ��ǰ4���µ��������ټ���10�죬Ȼ�����������
    for(t=0;t<mon;t++)
    {
        secCount+=(u32)mon_table[t]*86400;//�·����������
        if(Is_Leap_Year(year)&&t==1)
            secCount+=86400;//���꣬2�·�����һ���������
    }
    
    secCount+=(u32)(day-1)*86400;//��ǰ�����ڵ���������ӣ���һ�컹û���꣬����-1��
    secCount+=(u32)hour*3600;//Сʱ������
    secCount+=(u32)min*60;//����������
    secCount+=sec;
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR    | RCC_APB1Periph_BKP,ENABLE);
//    PWR_BackupAccessCmd(ENABLE);
    RTC_SetCounter(secCount);//����RTC��������ֵ
    RTC_WaitForLastTask();    //�ȴ����һ�ζ�RTC�Ĵ�����д�������
    RTC_Get();//����ʱ��
    return 0;
}

/*
�õ���ǰ��ʱ��
�ɹ�����0�����󷵻�����
*/
u8 RTC_Get(void)
{
        static u16 dayCount=0;
        u32 secCount=0;
        u32 tmp=0;
        u16 tmp1=0;
        secCount=RTC_GetCounter();
        tmp=secCount/86400;//�õ�����
        if(dayCount!=tmp)//����һ��
        {
            dayCount=tmp;
            tmp1=1970;//��1970�꿪ʼ
            while(tmp>=365)
            {
                if(Is_Leap_Year(tmp1))//������
                {
                    if(tmp>=366)    
                        tmp-=366;//�������������
                    else
                    {
                    //    tmp1++;
                        break;
                    }
                }
                else
                    tmp-=365;//ƽ��
                tmp1++;
            }
            calendar.w_year=tmp1;//�õ����
            tmp1=0;
            while(tmp>=28)//����һ����
            {
                if(Is_Leap_Year(calendar.w_year)&&tmp1==1)//��������������ѭ��2��
                {
                    if(tmp>=29)    
                        tmp-=29;
                    else
                        break;
                }
                else
                {
                    if(tmp>=mon_table[tmp1])//ƽ��
                        tmp-=mon_table[tmp1];
                    else
                        break;
                }
                tmp1++;
            }
            calendar.w_month=tmp1+1;//�õ��·ݣ�tmp1=0��ʾ1�£�����Ҫ��1
            calendar.w_date=tmp+1;    //�õ����ڣ���Ϊ��һ�컹û���꣬����tmpֻ����ǰһ�죬������ʾ��ʱ��Ҫ��ʾ��������
        }
        tmp=secCount%86400;//�õ�������
        calendar.hour=tmp/3600;//Сʱ
        calendar.min=(tmp%3600)/60;//����
        calendar.sec=(tmp%3600)%60;//��
        return 0;
}
/*
RTCʱ���ж�
ÿ�봥��һ��
*/
void RTC_IRQHandler(void)
{         
    if (RTC_GetITStatus(RTC_IT_SEC) != RESET)//�����ж�
    {                            
        RTC_Get();//����ʱ��
      
     }
    if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)//�����ж�
    {
        RTC_ClearITPendingBit(RTC_IT_ALR);//�������ж�        
  }                                                    
    RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);//�������ж�
    RTC_WaitForLastTask();                                                   
}
