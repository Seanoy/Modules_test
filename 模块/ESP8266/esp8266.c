#include "esp8266.h"
#include "stdio.h"
#include "stm32f10x_usart.h"
 
char buff[40];
 
//每一句后面都要加上回车换行\r\n
/******************************************
函数名：esp_rst
函数参数：无
函数作用：重启模块
*******************************************/
void esp_rst(void)
{
	sprintf(buff,"AT+RST");
	printf ( "%s\r\n",buff );
}


/******************************************
函数名：esp_usart_def
函数参数：
<baudrate> UART波特率
<databits> 数据位
5：5 bit 数据位
6：6 bit 数据位
7：7 bit 数据位
8：8 bit 数据位
<stopbits> 停止位
1：1 bit 停止位
2：1.5 bit 停止位
3：2 bit 停止位
<parity> 校验位
0：None
1：Odd
2：EVEN
<flow control> 流控
0：不使能流控
1：使能RTS
2：使能CTS
3：同时使能 RTS 和 CTS
函数作用：设置波特率
*******************************************/
void esp_usart_def(uint32_t baudrate, uint8_t databits, uint8_t stopbits, uint8_t parity, uint8_t flow_control)
{
	sprintf(buff,"AT+UART_DEF=%u,%u,%u,%u,%u", baudrate, databits, stopbits, parity, flow_control);
	printf ( "%s\r\n",buff );

}



/******************************************
函数名：esp_cwmode_def 
函数参数：
<mode>
1 : station mode
2 : softAP mode
3 : softAP + station mode
函数作用：设置wifi模式
*******************************************/
void esp_cwmode_def(uint8_t mode)
{
	sprintf(buff,"AT+CWMODE_DEF=%u", mode);
	printf ( "%s\r\n",buff );
}


/******************************************
函数名：esp_cwsap_def
函数参数：
<ssid> 字符串参数， 目标AP的SSID
<pwd> 字符串参数， 密码最长64字节ASCII
函数作用：设置ESP8266 station需连接的AP 
*******************************************/
void esp_cwsap_def(uint8_t* ssid, uint8_t* pwd, uint8_t chl, uint8_t ecn)
{
	sprintf(buff,"AT+CWSAP_DEF=\"%s\",\"%s\",%u,%u", ssid, pwd, chl, ecn);
	printf ( "%s\r\n",buff );
}


/******************************************
函数名：esp_cipmux
函数参数：
<mode>
0 ：单连接模式
1 ：多连接模式
函数作用：设置多连接
*******************************************/
void esp_cipmux(uint8_t mode)
{
	sprintf(buff,"AT+CIPMUX=%u", mode);
	printf ( "%s\r\n",buff );
}


/******************************************
函数名：esp_cipserver
函数参数：
<mode>
0 ：关闭 server
1 ：开启 server
<port> 端口号 默认为 333
函数作用：
*******************************************/
void esp_cipserver(uint8_t mode, uint16_t port)
{
	sprintf(buff,"AT+CIPSERVER=%u,%u", mode, port);
	printf ( "%s\r\n",buff );
}

/******************************************
函数名：esp_cipsend
函数参数：
<link ID> 网络连接ID号(0~4)用于多连接情况
<length> 数字参数，表明发送数据的长度，最大长度为 2048
函数作用：发送指定长度的数据到对应设备
*******************************************/
void esp_cipsend(uint8_t link_ID, uint8_t length)
{
	sprintf(buff,"AT+CIPSEND=%u,%u", link_ID, length);
	printf ( "%s\r\n",buff );
}


