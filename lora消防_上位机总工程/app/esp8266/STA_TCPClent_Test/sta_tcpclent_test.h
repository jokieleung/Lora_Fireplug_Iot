#ifndef _sta_tcpclent_test_H
#define _sta_tcpclent_test_H


#include "system.h"


#define User_ESP8266_SSID	  "fzs"	      //Ҫ���ӵ��ȵ������
#define User_ESP8266_PWD	  "liangzujie"	  //Ҫ���ӵ��ȵ������

#define User_ESP8266_TCPServer_IP	  "183.230.40.33"	  //Ҫ���ӵķ�������IP
#define User_ESP8266_TCPServer_PORT	  "80"	  //Ҫ���ӵķ������Ķ˿�


extern volatile uint8_t TcpClosedFlag;  //����һ��ȫ�ֱ���


void ESP8266_STA_TCPClient_Init(void);

void ESP8266_STA_TCPClient_Send_Data_To_OneNet(u16 val);
void ESP8266_STA_TCPClient_Send_location_To_OneNet(void);
void ESP8266_STA_TCPClient_Send_distance_To_OneNet(void);
void ESP8266_STA_TCPClient_Send_presure_To_OneNet(void);
#endif
