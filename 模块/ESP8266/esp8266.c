#include "esp8266.h"
#include "stdio.h"
#include "stm32f10x_usart.h"
 
char buff[40];
 
//ÿһ����涼Ҫ���ϻس�����\r\n
/******************************************
��������esp_rst
������������
�������ã�����ģ��
*******************************************/
void esp_rst(void)
{
	sprintf(buff,"AT+RST");
	printf ( "%s\r\n",buff );
}


/******************************************
��������esp_usart_def
����������
<baudrate> UART������
<databits> ����λ
5��5 bit ����λ
6��6 bit ����λ
7��7 bit ����λ
8��8 bit ����λ
<stopbits> ֹͣλ
1��1 bit ֹͣλ
2��1.5 bit ֹͣλ
3��2 bit ֹͣλ
<parity> У��λ
0��None
1��Odd
2��EVEN
<flow control> ����
0����ʹ������
1��ʹ��RTS
2��ʹ��CTS
3��ͬʱʹ�� RTS �� CTS
�������ã����ò�����
*******************************************/
void esp_usart_def(uint32_t baudrate, uint8_t databits, uint8_t stopbits, uint8_t parity, uint8_t flow_control)
{
	sprintf(buff,"AT+UART_DEF=%u,%u,%u,%u,%u", baudrate, databits, stopbits, parity, flow_control);
	printf ( "%s\r\n",buff );

}



/******************************************
��������esp_cwmode_def 
����������
<mode>
1 : station mode
2 : softAP mode
3 : softAP + station mode
�������ã�����wifiģʽ
*******************************************/
void esp_cwmode_def(uint8_t mode)
{
	sprintf(buff,"AT+CWMODE_DEF=%u", mode);
	printf ( "%s\r\n",buff );
}


/******************************************
��������esp_cwsap_def
����������
<ssid> �ַ��������� Ŀ��AP��SSID
<pwd> �ַ��������� �����64�ֽ�ASCII
�������ã�����ESP8266 station�����ӵ�AP 
*******************************************/
void esp_cwsap_def(uint8_t* ssid, uint8_t* pwd, uint8_t chl, uint8_t ecn)
{
	sprintf(buff,"AT+CWSAP_DEF=\"%s\",\"%s\",%u,%u", ssid, pwd, chl, ecn);
	printf ( "%s\r\n",buff );
}


/******************************************
��������esp_cipmux
����������
<mode>
0 ��������ģʽ
1 ��������ģʽ
�������ã����ö�����
*******************************************/
void esp_cipmux(uint8_t mode)
{
	sprintf(buff,"AT+CIPMUX=%u", mode);
	printf ( "%s\r\n",buff );
}


/******************************************
��������esp_cipserver
����������
<mode>
0 ���ر� server
1 ������ server
<port> �˿ں� Ĭ��Ϊ 333
�������ã�
*******************************************/
void esp_cipserver(uint8_t mode, uint16_t port)
{
	sprintf(buff,"AT+CIPSERVER=%u,%u", mode, port);
	printf ( "%s\r\n",buff );
}

/******************************************
��������esp_cipsend
����������
<link ID> ��������ID��(0~4)���ڶ��������
<length> ���ֲ����������������ݵĳ��ȣ���󳤶�Ϊ 2048
�������ã�����ָ�����ȵ����ݵ���Ӧ�豸
*******************************************/
void esp_cipsend(uint8_t link_ID, uint8_t length)
{
	sprintf(buff,"AT+CIPSEND=%u,%u", link_ID, length);
	printf ( "%s\r\n",buff );
}


