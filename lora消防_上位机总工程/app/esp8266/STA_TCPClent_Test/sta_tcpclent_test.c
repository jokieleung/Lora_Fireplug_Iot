#include "sta_tcpclent_test.h"
#include "SysTick.h"
#include "usart.h"
#include "esp8266_drive.h"
#include "string.h"

//extern uint8_t distance_rx_str[100],presure_rx_str[100],location_rx_str[100];//latitude_rx_str[100],longitude_rx_str[100],; 

extern u8 rx_str[80];

volatile u8 TcpClosedFlag = 0;

void ESP8266_STA_TCPClient_Send_location_To_OneNet(void)
{
	u8 res;
	char str[350]={0};
	char JSON[100]={0};
	u8 length=0;
			if(TcpClosedFlag) //检测是否失去连接
		{
			ESP8266_ExitUnvarnishSend(); //退出透传模式
			do
			{
				res = ESP8266_Get_LinkStatus();     //获取连接状态
			} 	
			while(!res);
			
			if(res == 4)                     //确认失去连接后重连
			{
				printf ( "\r\n请稍等，正在重连热点和服务器...\r\n" );
				
				while (!ESP8266_JoinAP(User_ESP8266_SSID, User_ESP8266_PWD ) );
				
				while (!ESP8266_Link_Server(enumTCP, User_ESP8266_TCPServer_IP, User_ESP8266_TCPServer_PORT, Single_ID_0 ) );
				
				printf ( "\r\n重连热点和服务器成功\r\n" );
			} 
			while(!ESP8266_UnvarnishSend());					
		}

		printf(rx_str);
		printf("\r\n");
    strcat(JSON, "{\"datastreams\":[{\"id\":\"map\",\"datapoints\":[{\"value\":{\"lon\":");
		printf(JSON);printf("\r\n");
		strcat(JSON,&rx_str[10+8]);
	printf(JSON);printf("\r\n");
		strcpy((strstr(JSON,"latitude ")-2), ",\"lat\":");
			printf(JSON);printf("\r\n");
		strcat(JSON,((strstr(rx_str,"latitude "))+9));//latitude_rx_str[9]);
			printf(JSON);printf("\r\n");
    strcat(JSON,"}}]}]}");
			printf(JSON);printf("\r\n");
		length=strlen(JSON);
		sprintf(str,"POST /devices/7207185/datapoints HTTP/1.1\r\napi-key:rn2Very6SZUCAv27kJafT1US99A=\r\nHost:api.heclouds.com\r\nContent-Length:%d\r\n\r\n",length);
		strcat(str,JSON);
			printf(str);printf("\r\n");
		
//	sprintf (str,"POST /devices/7207185/datapoints HTTP/1.1\r\napi-key:rn2Very6SZUCAv27kJafT1US99A=\r\nHost:api.heclouds.com\r\nContent-Length:63\r\n\r\n{\"datastreams\":[{\"id\":\"sys_time\",\"datapoints\":[{\"value\":%d}]}]}",val );//www.prechin.cn\r\n
	ESP8266_SendString ( ENABLE, str, 0, Single_ID_0 );
		printf ( "\r\n发送成功\r\n" );
}

void ESP8266_STA_TCPClient_Send_presure_To_OneNet(void)
{
	u8 res;
	char str[300]={0};
	char JSON[100]={0};
	u8 length=0;
			if(TcpClosedFlag) //检测是否失去连接
		{
			ESP8266_ExitUnvarnishSend(); //退出透传模式
			do
			{
				res = ESP8266_Get_LinkStatus();     //获取连接状态
			} 	
			while(!res);
			
			if(res == 4)                     //确认失去连接后重连
			{
				printf ( "\r\n请稍等，正在重连热点和服务器...\r\n" );
				
				while (!ESP8266_JoinAP(User_ESP8266_SSID, User_ESP8266_PWD ) );
				
				while (!ESP8266_Link_Server(enumTCP, User_ESP8266_TCPServer_IP, User_ESP8266_TCPServer_PORT, Single_ID_0 ) );
				
				printf ( "\r\n重连热点和服务器成功\r\n" );
			} 
			while(!ESP8266_UnvarnishSend());					
		}

    strcat(JSON, "{\"datastreams\":[{\"id\":\"presure\",\"datapoints\":[{\"value\":");
    strcat(JSON,&rx_str[8+8]);
    strcat(JSON,"}]}]}");
		
		length=strlen(JSON);
		
		sprintf(str,"POST /devices/7207185/datapoints HTTP/1.1\r\napi-key:rn2Very6SZUCAv27kJafT1US99A=\r\nHost:api.heclouds.com\r\nContent-Length:%d\r\n\r\n",length);
		strcat(str,JSON);
		
//	sprintf (str,"POST /devices/7207185/datapoints HTTP/1.1\r\napi-key:rn2Very6SZUCAv27kJafT1US99A=\r\nHost:api.heclouds.com\r\nContent-Length:63\r\n\r\n{\"datastreams\":[{\"id\":\"sys_time\",\"datapoints\":[{\"value\":%d}]}]}",val );//www.prechin.cn\r\n
	ESP8266_SendString ( ENABLE, str, 0, Single_ID_0 );
		printf ( "\r\n发送成功\r\n" );
}

void ESP8266_STA_TCPClient_Send_distance_To_OneNet(void)
{
	u8 res;
	char str[300]={0};
	char JSON[100]={0};
	u8 length=0;
			if(TcpClosedFlag) //检测是否失去连接
		{
			ESP8266_ExitUnvarnishSend(); //退出透传模式
			do
			{
				res = ESP8266_Get_LinkStatus();     //获取连接状态
			} 	
			while(!res);
			
			if(res == 4)                     //确认失去连接后重连
			{
				printf ( "\r\n请稍等，正在重连热点和服务器...\r\n" );
				
				while (!ESP8266_JoinAP(User_ESP8266_SSID, User_ESP8266_PWD ) );
				
				while (!ESP8266_Link_Server(enumTCP, User_ESP8266_TCPServer_IP, User_ESP8266_TCPServer_PORT, Single_ID_0 ) );
				
				printf ( "\r\n重连热点和服务器成功\r\n" );
			} 
			while(!ESP8266_UnvarnishSend());			
			
		}
		
		strcat(JSON, "{\"datastreams\":[{\"id\":\"ultrasonic\",\"datapoints\":[{\"value\":");
		strcat(JSON,&rx_str[9+8]);
    strcat(JSON,"}]}]}");
		length=strlen(JSON);
		sprintf(str,"POST /devices/7207185/datapoints HTTP/1.1\r\napi-key:rn2Very6SZUCAv27kJafT1US99A=\r\nHost:api.heclouds.com\r\nContent-Length:%d\r\n\r\n",length);
		strcat(str,JSON);
		
//	sprintf (str,"POST /devices/7207185/datapoints HTTP/1.1\r\napi-key:rn2Very6SZUCAv27kJafT1US99A=\r\nHost:api.heclouds.com\r\nContent-Length:63\r\n\r\n{\"datastreams\":[{\"id\":\"sys_time\",\"datapoints\":[{\"value\":%d}]}]}",val );//www.prechin.cn\r\n
	ESP8266_SendString ( ENABLE, str, 0, Single_ID_0 );
		printf ( "\r\n发送成功\r\n" );
}

void ESP8266_STA_TCPClient_Send_Data_To_OneNet(u16 val)
{
	u8 res;
	char str[200]={0};
			if(TcpClosedFlag) //检测是否失去连接
		{
			ESP8266_ExitUnvarnishSend(); //退出透传模式
			do
			{
				res = ESP8266_Get_LinkStatus();     //获取连接状态
			} 	
			while(!res);
			
			if(res == 4)                     //确认失去连接后重连
			{
				printf ( "\r\n请稍等，正在重连热点和服务器...\r\n" );
				
				while (!ESP8266_JoinAP(User_ESP8266_SSID, User_ESP8266_PWD ) );
				
				while (!ESP8266_Link_Server(enumTCP, User_ESP8266_TCPServer_IP, User_ESP8266_TCPServer_PORT, Single_ID_0 ) );
				
				printf ( "\r\n重连热点和服务器成功\r\n" );
			} 
			while(!ESP8266_UnvarnishSend());					
		}
	

	sprintf (str,"POST /devices/7207185/datapoints HTTP/1.1\r\napi-key:rn2Very6SZUCAv27kJafT1US99A=\r\nHost:api.heclouds.com\r\nContent-Length:63\r\n\r\n{\"datastreams\":[{\"id\":\"sys_time\",\"datapoints\":[{\"value\":%d}]}]}",val );//www.prechin.cn\r\n
	ESP8266_SendString ( ENABLE, str, 0, Single_ID_0 );
		printf ( "\r\n发送成功\r\n" );
}

void ESP8266_STA_TCPClient_Init(void)
{
//	u8 res;
	
//	char str[200]={0};
	
	printf ( "\r\n正在配置ESP8266请耐心等待...\r\n" );

	ESP8266_CH_PD_Pin_SetH;
	printf ( "\r\nESP8266_CH_PD_Pin_SetH...\r\n" );
	ESP8266_AT_Test();
	printf ( "\r\nESP8266_AT_Test\r\n" );
	ESP8266_Net_Mode_Choose(STA);
	printf ( "\r\nESP8266_Net_Mode_Choose\r\n" );
	while(!ESP8266_JoinAP(User_ESP8266_SSID, User_ESP8266_PWD));
	ESP8266_Enable_MultipleId ( DISABLE );
		while(!ESP8266_Link_Server(enumTCP, User_ESP8266_TCPServer_IP, User_ESP8266_TCPServer_PORT, Single_ID_0));
	
	while(!ESP8266_UnvarnishSend());
	printf ( "\r\n配置ESP8266 OK！\r\n" );
//	while(!ESP8266_Link_Server(enumTCP, User_ESP8266_TCPServer_IP, User_ESP8266_TCPServer_PORT, Single_ID_0));
//	
//	while(!ESP8266_UnvarnishSend());
//	printf ( "\r\n配置ESP8266 OK！\r\n" );
			
}



