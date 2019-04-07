#include "stm32f10x.h"
#include "stdio.h"

void USART_Config(void);
void delay(void);
void HMISendstart(void);
void HMISends(char *buf1);
void HMISendb(u8 k);	//发送结束符0xff0xff0xff


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
		

	void HMISends(char *buf1)		  //字符串发送函数
{
	u8 i=0;
	while(1)
	{
	 if(buf1[i]!=0)
	 	{
			USART_SendData(USART1,buf1[i]);  //发送一个字节
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET){};//等待发送结束
		 	i++;
		}
	 else 
	 return ;

		}
	}
void HMISendb(u8 k)		         //字节发送函数
{		 
	u8 i;
	 for(i=0;i<3;i++)
	 {
	 if(k!=0)
	 	{
			USART_SendData(USART1,k);  //发送一个字节
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET){};//等待发送结束
		}
	 else 
	 return ;

	 } 
} 

 /*
内容：						地址开始位置：								
密码 						5
开门成功提示			40
成员数						50
成员名						100 120 140	...
成员名长度 			115	135	155 ...	 
*/
int main(){	
	int mem_arr_length,mem_length=0;//成员数 成员名长度
	int i,k;
	u8 ctemp;
	u8 mems[][10]={"seanOY","LiLei","HanMeiMei","DonalDuck"};//成员名最长为10字节
	mem_arr_length=sizeof(mems)/sizeof(mems[0]);
	USART_Config();
	HMISendstart(); 											//为确保串口HMI正常通信
while(1){	

	
	printf("wepo \"25503093\",5");  			//开始存密码
	HMISendb(0xff);		  									//发送结束符 0XFF 0XFF 0XFF
	delay();
	
	printf("wepo \"unlock\",40");					//发送提示信息给下位机
	HMISendb(0xff);
	delay();
	
	printf("wepo \"%d\",50",mem_arr_length);	//发送成员数给屏幕
	HMISendb(0xff);
	delay();
	
	for(i=mem_arr_length;i>0;i--)
	{
		k=100+20*(mem_arr_length-i);
		printf("wepo \"%s\",%d",mems[mem_arr_length-i],k);//按照mems数组顺序依次存放在100 120 140 ... 100+20*mem_arr_length
		HMISendb(0xff);																//发送结束符 0XFF 0XFF 0XFF
		delay();																			//给一定延时给设备写数据
		
		//判断成员名长度
		for(ctemp=mems[mem_arr_length-i][mem_length]	;(ctemp>96&&ctemp<123)|	(ctemp<91&&ctemp>64)|	(ctemp>47&&ctemp<58);)
		{
			mem_length++;
			ctemp=mems[mem_arr_length-i][mem_length];
		}
		printf("wepo \"%d\",%d",mem_length,k+15);			//发送每个成员名字的长度 放在115	135	155 ...	100+20*mem_arr_length+15
		HMISendb(0xff);																//发送结束符 0XFF 0XFF 0XFF
		delay();
		mem_length=0;//清零长度
	}
	

 
}	

}




