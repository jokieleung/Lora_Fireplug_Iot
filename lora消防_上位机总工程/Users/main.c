

#include "public.h"

#include "gui.h"
#include "led.h"
#include "stdio.h"
#include "usart.h"
//LORA部分头文件
#include "drv_delay.h"
#include "drv_uart.h"

//WIFI部分头文件
#include "SysTick.h"
#include "esp8266_drive.h"
#include "sta_tcpclent_test.h"

//uint8_t g_As62_rx_buffer[ 100 ] = { 0 };

uint8_t jiaoyan[]={"jiaoyan"};//{'a', 's', 'h', 'i', 'n', 'i', 'n', 'g'};
uint8_t distance_jiaoyan[]={"distance"};//8
uint8_t location_jiaoyan[]={"longitude"};//9
uint8_t presure_jiaoyan[]={"presure"};

/****************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/
u8 rx_str[80]={0};
//uint8_t distance_rx_str[100]={0},presure_rx_str[100]={0},location_rx_str[100]={0}; //latitude_rx_str[100]={0},longitude_rx_str[100]={0},
//lora接收声明
//void distance_rx_lora(void);
//void location_rx_lora(void);
//void presure_rx_lora(void);

//void distance_rx_lora(){
//	u8 i;
//	uint8_t length;	
//		length=drv_uart_rx_bytes( distance_rx_str );		
//		if( 0 != length )
//			{
//			
//				for( i = 0; i < 8; i++ )
//				{
//					if( distance_rx_str[ i ] != distance_jiaoyan[ i ] )	
//					{
//						break;
//					}
//				}
//				if( 8 == i )			
//				{

//					ESP8266_STA_TCPClient_Send_distance_To_OneNet();
//			}
//		}
//}

//void location_rx_lora(){
//		u8 i;
//		uint8_t length;	
//		length=drv_uart_rx_bytes( location_rx_str );		
//		if( 0 != length )
//			{
//			
//				for( i = 0; i < 9; i++ )
//				{
//					if( location_rx_str[ i ] != location_jiaoyan[ i ] )	
//					{
//						break;
//					}
//				}
//				if( 9 == i )			
//				{

//					ESP8266_STA_TCPClient_Send_location_To_OneNet();
//			}
//		}
//	
//}

//void presure_rx_lora(){
//	u8 i;
//	uint8_t length;	
//		length=drv_uart_rx_bytes( presure_rx_str );		
//		if( 0 != length )
//			{
//			
//				for( i = 0; i < 7; i++ )
//				{
//					if( presure_rx_str[ i ] != presure_jiaoyan[ i ] )	
//					{
//						break;
//					}
//				}
//				if( 7 == i )			
//				{

//					ESP8266_STA_TCPClient_Send_presure_To_OneNet();
//			}
//		}
//}

void rx_lora_prime(){
	u8 length,i;
	length=drv_uart_rx_bytes( rx_str );		
	if( 0 != length )
			{
			
				for( i = 0; i < 7; i++ )
				{
					if( rx_str[ i ] != jiaoyan[ i ] )	
					{
						break;
					}
				}
				if( 7 == i )			
				{
					u8 j;
					//-------------------校验是否是距离数据------------
					//---------------------------------------------
					for( j = 0+8; j < 7+8; j++ )		
				{
					if( rx_str[ j ] != distance_jiaoyan[ j-8 ] )	
					{
						break;
					}
				}
				if(j==7+8)				//距离数据
				{
					ESP8266_STA_TCPClient_Send_distance_To_OneNet();
					
				}else{
				//-------------------校验是否是水压数据------------
					//---------------------------------------------
				for( j = 0+8; j < 7+8; j++ )		
				{
					if( rx_str[ j ] != presure_jiaoyan[ j-8 ] )	
					{
						break;
					}
				}
				if(j==7+8)				//压力数据
				{
					ESP8266_STA_TCPClient_Send_presure_To_OneNet();
				
				}else{
				//-------------------校验是否是location数据------------
					//---------------------------------------------
				for( j = 0+8; j < 7+8; j++ )		
				{
					if( rx_str[ j ] != location_jiaoyan[ j-8 ] )	
					{
						break;
					}
				}
				if(j==7+8)				//location数据
				{
					ESP8266_STA_TCPClient_Send_location_To_OneNet();
				
				}//else;
			}
		}
			}
			}
}

int main(void)
{
//	int i;

//	uint8_t rx_str[100]={0};

//	uint8_t length;
	Systick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	USART1_Init(115200);
	
	//WIFI初始化
	ESP8266_Init(115200);
	ESP8266_STA_TCPClient_Init();

	printf("WIFI初始化完毕\r\n");
	
	//TFT彩屏初始化
//	TFT_Init();	  
//	printf("TFT_Init完毕\r\n");
//	TFT_ClearScreen(BLACK);	 //清屏
//	printf("TFT_ClearScreen完毕\r\n");
	
	//LORA串口初始化	
	drv_uart_init( 9600 );

	printf("LORA串口初始化完毕	\r\n");
	

	while(1)
	{
//		u16 h=0;
//		h++;
		printf("while(1)循环中	\r\n");
//		if (h>=9){//;h=0;}
		rx_lora_prime();
//		distance_rx_lora();
//		location_rx_lora();
//		presure_rx_lora();
//		length=drv_uart_rx_bytes( rx_str );		
//		if( 0 != length )
//			{
//			
//				for( i = 0; i < 7; i++ )
//				{
//					if( rx_str[ i ] != jiaoyan[ i ] )	
//					{
//						break;
//					}
//				}
//				if( 7 == i )			
//				{
//					u8 char_to_int=0,last=8,j,x=0;
//					u16 temp=1;
//					
//					TFT_ClearScreen(BLACK);
//					GUI_Show12ASCII(80,20,rx_str,YELLOW,BLACK);
//					
//					while(rx_str[last]!='\r'){last++;}
//					for(j=8;j<last;j++){
//						//u8 ;
//						temp=1;
//						for(x=(last-1)-j;x>0;x--) {temp*=10;}
//						char_to_int+=((rx_str[j]-'0')*temp);						
//					}

//					ESP8266_STA_TCPClient_Send_Data_To_OneNet(char_to_int);	//rx_str[9]
//				}
//			}
		
	
		//led_display();	//LED闪烁		
	}
}

//		sprintf(tx_str,"%s\r\n",rx_str);
//		drv_uart_tx_bytes( tx_str, 5 );
//		sprintf(lth,"%d",length);
//	  GUI_Show12Char(80,100,&length,YELLOW,BLACK);
	//	GUI_Show12Char(80,110,&length,YELLOW,BLACK);


//void latitude_rx_lora(){
//		u8 i;
//	uint8_t length;	
//		length=drv_uart_rx_bytes( latitude_rx_str );		
//		if( 0 != length )
//			{
//			
//				for( i = 0; i < 8; i++ )
//				{
//					if( latitude_rx_str[ i ] != latitude_jiaoyan[ i ] )	
//					{
//						break;
//					}
//				}
//				if( 8 == i )			
//				{
//					u8 char_to_int=0,last=9,j,x=0;
//					u16 temp=1;
//					
//					TFT_ClearScreen(BLACK);
//					GUI_Show12ASCII(80,20,latitude_rx_str,YELLOW,BLACK);
//					
////					while(latitude_rx_str[last]!='\r'){last++;}
////					for(j=9;j<last;j++){
////						//u8 ;
////						temp=1;
////						for(x=(last-1)-j;x>0;x--) {temp*=10;}
////						char_to_int+=((latitude_rx_str[j]-'0')*temp);						
////					}
//			}
//		}
//}