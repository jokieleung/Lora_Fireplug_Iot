/**********************************************************
						���ߣ����زر���
						���̣�ILoveMCU.taobao.com
												
����˵����
STM32					GPS
VCC		------>	VCC
GND		------>	GND
RX1		<------	TXD


STM32					USB-TTLģ��
GND		------>	GND
TX1		------>	RXD

����ʹ��STM32��Сϵͳ��
	https://item.taobao.com/item.htm?id=523336310868
����ʹ��USB-TTLģ�飺
	https://item.taobao.com/item.htm?id=39481188174
����ʹ��GPSģ�飺
	��GPS��׼�桿
	https://item.taobao.com/item.htm?id=18982118996
	��GPS MINI�桿
	https://item.taobao.com/item.htm?id=537475465445
	������+GPS˫ģģ�顿
	https://item.taobao.com/item.htm?id=528045510604
***********************************************************/

#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
//#include "led.h"
#include "string.h"
//#include "stdio.h"
#include "main.h"

#include "drv_uart.h"
#include "UltrasonicWave.h"
#include "timer.h"

#include "key.h"
#include "exti.h"
//����
void errorLog(int num);
void parseGpsBuffer(void);
void printGpsBuffer(void);

void KEY_react(u8 key_num);

void location_tx_lora(u8* lon,u8* lat){
	//u8 lat_tx[100]="latitude ",lon_tx[100]="longitude ";//,latitude[100],longitude[100];
	u8 tmp[100];
	strcat(tmp,"jiaoyan:longitude ");
	strcat(tmp,lon);
	strcat(tmp,"; ");
	strcat(tmp,"latitude ");
	strcat(tmp,lat);
	strcat(tmp,"\r\n");

	drv_uart_tx_bytes(tmp, strlen(tmp) );
//	delay_ms(1000);
//	drv_uart_tx_bytes(lon_tx, strlen(lon_tx) );
//	delay_ms(1000);
}
void presure_tx_lora(u8* presure){
	u8 tx[100]="jiaoyan:presure ";
	//	sprintf(t,"%d",presure);
		//	strcat(t,"\r\n");
	
			strcat(tx,presure);
			strcat(tx,"\r\n");
			drv_uart_tx_bytes(tx, strlen(tx) );			//??????? ashining ???
}

void KEY_react(u8 key_num){
	if(key_num == KEY0_PRES){//distance
//		u8 distance_got=0;
//		distance_got=Get_Distance();
//    Distance_tx_lora(distance_got);
		printf("KEY0��\r\n");
	}
	if(key_num == KEY1_PRES){//location
//		u8 lon[]="114.338",lat[]="30.515";
//		
//		location_tx_lora(lon,lat);//(u8*)(u8*)
		printf("KEY1��\r\n");
		//strcpy(Save_Data.longitude,"114.338");//*Save_Data.longitude="11319.72036";
		//strcpy(Save_Data.latitude,"30.515");  //*Save_Data.latitude="2318.13307";
		//location_tx_lora(Save_Data.latitude,Save_Data.longitude);//(u8*)(u8*)
	}
	if(key_num == KEY2_PRES){//presure
//		presure_tx_lora(0.1);
			printf("KEY2��\r\n");
	}
	if(key_num == KEY3_PRES){//presure too_high ����distance too large
		printf("KEY3��\r\n");
	}
}

int main(void)
{	
	u8 KEY_NUM=0;
	
	delay_init();
	
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);				 //���ڳ�ʼ��Ϊ9600
	EXTIX_Init();//������ʼ������
	drv_uart_init(9600);		//USART3  lora
	Timerx_Init(5000,7199);   //10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms 
	UltrasonicWave_Configuration();               //�Գ�����ģ���ʼ��
	
//	clrStruct();
		
//	printf("Welcome to use!\r\n");
//	printf("ILoveMcu.taobao.com!\r\n");
	while(1)
	{

		
		
//		parseGpsBuffer();
//	printGpsBuffer();

//		delay_ms(1000);
//		delay_ms(500);
		
//		
//		delay_ms(500);
	}
}

void errorLog(int num)
{
	
	while (1)
	{
	  	printf("ERROR%d\r\n",num);
	}
}

void parseGpsBuffer()
{
	char *subString;
	char *subStringNext;
	char i = 0;
	if (Save_Data.isGetData)
	{
		Save_Data.isGetData = false;
		printf("**************\r\n");
		printf(Save_Data.GPS_Buffer);

		
		for (i = 0 ; i <= 6 ; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
					errorLog(1);	//��������
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2]; 
					switch(i)
					{
						case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//��ȡUTCʱ��
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//��ȡUTCʱ��
						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//��ȡγ����Ϣ
						case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//��ȡN/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//��ȡ������Ϣ
						case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//��ȡE/W

						default:break;
					}

					subString = subStringNext;
					Save_Data.isParseData = true;
					if(usefullBuffer[0] == 'A')
						Save_Data.isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						Save_Data.isUsefull = false;

				}
				else
				{
					errorLog(2);	//��������
				}
			}


		}
	}
}

void printGpsBuffer()
{
	if (Save_Data.isParseData)
	{
		Save_Data.isParseData = false;
		
		printf("Save_Data.UTCTime = ");
		printf(Save_Data.UTCTime);
		printf("\r\n");

		if(Save_Data.isUsefull)
		{
			Save_Data.isUsefull = false;
			printf("Save_Data.latitude = ");
			printf(Save_Data.latitude);
			printf("\r\n");


			printf("Save_Data.N_S = ");
			printf(Save_Data.N_S);
			printf("\r\n");

			printf("Save_Data.longitude = ");
			printf(Save_Data.longitude);
			printf("\r\n");

			printf("Save_Data.E_W = ");
			printf(Save_Data.E_W);
			printf("\r\n");
		}
		else
		{
			printf("GPS DATA is not usefull!\r\n");
		}
		
	}
}






