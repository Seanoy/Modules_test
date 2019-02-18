#include "stm32f10x.h"
#include "stdio.h"

void USART_Config(void);
void delay(void);
void HMISendstart(void);
void HMISends(char *buf1);
void HMISendb(u8 k);	//���ͽ�����0xff0xff0xff


	void delay(){
	int i,j;
		for(i=6000;i>0;i--)
		for(j=2000;j>0;j--);
	}
	
	
void HMISendstart(void)
	{
	 	delay();
		HMISendb(0xff);
		delay();
	}
		

	void HMISends(char *buf1)		  //�ַ������ͺ���
{
	u8 i=0;
	while(1)
	{
	 if(buf1[i]!=0)
	 	{
			USART_SendData(USART1,buf1[i]);  //����һ���ֽ�
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET){};//�ȴ����ͽ���
		 	i++;
		}
	 else 
	 return ;

		}
	}
void HMISendb(u8 k)		         //�ֽڷ��ͺ���
{		 
	u8 i;
	 for(i=0;i<3;i++)
	 {
	 if(k!=0)
	 	{
			USART_SendData(USART1,k);  //����һ���ֽ�
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET){};//�ȴ����ͽ���
		}
	 else 
	 return ;

	 } 
} 

 /*
���ݣ�						��ַ��ʼλ�ã�								
���� 						5
���ųɹ���ʾ			40
��Ա��						50
��Ա��						100 120 140	...
��Ա������ 			115	135	155 ...	 
*/
int main(){	
	int mem_arr_length,mem_length=0;//��Ա�� ��Ա������
	int i,k;
	u8 ctemp;
	u8 mems[][10]={"seanOY","LiLei","HanMeiMei","DonalDuck"};//��Ա���Ϊ10�ֽ�
	mem_arr_length=sizeof(mems)/sizeof(mems[0]);
	USART_Config();
	HMISendstart(); 											//Ϊȷ������HMI����ͨ��
while(1){	

	
	printf("wepo \"25503093\",5");  			//��ʼ������
	HMISendb(0xff);		  									//���ͽ����� 0XFF 0XFF 0XFF
	delay();
	
	printf("wepo \"unlock\",40");					//������ʾ��Ϣ����λ��
	HMISendb(0xff);
	delay();
	
	printf("wepo \"%d\",50",mem_arr_length);	//���ͳ�Ա������Ļ
	HMISendb(0xff);
	delay();
	
	for(i=mem_arr_length;i>0;i--)
	{
		k=100+20*(mem_arr_length-i);
		printf("wepo \"%s\",%d",mems[mem_arr_length-i],k);//����mems����˳�����δ����100 120 140 ... 100+20*mem_arr_length
		HMISendb(0xff);																//���ͽ����� 0XFF 0XFF 0XFF
		delay();																			//��һ����ʱ���豸д����
		
		//�жϳ�Ա������
		for(ctemp=mems[mem_arr_length-i][mem_length]	;(ctemp>96&&ctemp<123)|	(ctemp<91&&ctemp>64)|	(ctemp>47&&ctemp<58);)
		{
			mem_length++;
			ctemp=mems[mem_arr_length-i][mem_length];
		}
		printf("wepo \"%d\",%d",mem_length,k+15);			//����ÿ����Ա���ֵĳ��� ����115	135	155 ...	100+20*mem_arr_length+15
		HMISendb(0xff);																//���ͽ����� 0XFF 0XFF 0XFF
		delay();
		mem_length=0;//���㳤��
	}
	

 
}	

}




