#ifndef __ESP8266_H
#define __ESP8266_H

typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;

void esp_rst(void);
void esp_cwmode_def(uint8_t mode)	;
void esp_cwsap_def(uint8_t* ssid, uint8_t* pwd, uint8_t chl, uint8_t ecn);
void esp_cipmux(uint8_t mode);
void esp_cipserver(uint8_t mode, uint16_t port);
void esp_cipsend(uint8_t link_ID, uint8_t length);
void esp_usart_def(uint32_t baudrate, uint8_t databits, uint8_t stopbits, uint8_t parity, uint8_t flow_control);

#endif
