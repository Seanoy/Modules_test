#include "bsp.h"

void BTN1_PRESS_DOWN_Handler(void* btn)
{
	printf("��������\r\n");
}

void BTN1_PRESS_UP_Handler(void* btn)
{
	printf("��������\r\n");
}

void BTN1_PRESS_REPEAT_Handler(void* btn)
{
	printf("�ظ�����\r\n");
}

void BTN1_SINGLE_Click_Handler(void* btn)
{
	printf("��������\r\n");
}

void BTN1_DOUBLE_Click_Handler(void* btn)
{
	printf("˫������\r\n");
}

void BTN1_LONG_RRESS_START_Handler(void* btn)
{
	printf("�ﵽ����ʱ����ֵʱ����һ��\r\n");
}

void BTN1_LONG_PRESS_HOLD_Handler(void* btn)
{
	printf("�����ڼ�һֱ����\r\n");
}

struct Button btn1;

void button_Init(void)//�����͵�ƽ��Ӧ
{			
	GPIO_InitTypeDef GPIO_InitStructure;    
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOB , ENABLE ); 																													
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;  						 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    	 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      
	GPIO_Init( GPIOB , &GPIO_InitStructure );    
}

uint8_t read_button1_GPIO() 
{
	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0);
}

int main()
{
	int timer_ticks=0;
	BSP_init();
	button_Init();
	button_init(&btn1, read_button1_GPIO, 0);

	button_attach(&btn1, PRESS_DOWN,       BTN1_PRESS_DOWN_Handler);
	button_attach(&btn1, PRESS_UP,         BTN1_PRESS_UP_Handler);
	button_attach(&btn1, PRESS_REPEAT,     BTN1_PRESS_REPEAT_Handler);
	button_attach(&btn1, SINGLE_CLICK,     BTN1_SINGLE_Click_Handler);
	button_attach(&btn1, DOUBLE_CLICK,     BTN1_DOUBLE_Click_Handler);
	button_attach(&btn1, LONG_RRESS_START, BTN1_LONG_RRESS_START_Handler);
	button_attach(&btn1, LONG_PRESS_HOLD,  BTN1_LONG_PRESS_HOLD_Handler);
	
	button_start(&btn1);
	
	//make the timer invoking the button_ticks() interval 5ms.
	//This function is implemented by yourself.
	//__timer_start(button_ticks, 0, 5); 
	
	while(1) 
	{
	    if(timer_ticks == 15000) {//�����ʱ�ӽ�����Ҫ����ʵ�ʵ���
        timer_ticks = 0;
        
        button_ticks();
    }
	timer_ticks++;
	}
}









