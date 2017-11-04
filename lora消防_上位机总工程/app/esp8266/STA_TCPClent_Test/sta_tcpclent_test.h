#ifndef _sta_tcpclent_test_H
#define _sta_tcpclent_test_H


#include "system.h"


#define User_ESP8266_SSID	  "fzs"	      //要连接的热点的名称
#define User_ESP8266_PWD	  "liangzujie"	  //要连接的热点的密码

#define User_ESP8266_TCPServer_IP	  "183.230.40.33"	  //要连接的服务器的IP
#define User_ESP8266_TCPServer_PORT	  "80"	  //要连接的服务器的端口


extern volatile uint8_t TcpClosedFlag;  //定义一个全局变量


void ESP8266_STA_TCPClient_Init(void);

void ESP8266_STA_TCPClient_Send_Data_To_OneNet(u16 val);
void ESP8266_STA_TCPClient_Send_location_To_OneNet(void);
void ESP8266_STA_TCPClient_Send_distance_To_OneNet(void);
void ESP8266_STA_TCPClient_Send_presure_To_OneNet(void);
#endif
