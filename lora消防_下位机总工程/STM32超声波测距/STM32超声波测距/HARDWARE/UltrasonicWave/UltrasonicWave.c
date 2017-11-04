/******************** (C) 1209 Lab **************************
 * �ļ���  : UltrasonicWave.c
 * ����    �����������ģ�飬UltrasonicWave_Configuration��������
             ��ʼ������ģ�飬UltrasonicWave_StartMeasure��������
			 ������࣬������õ�����ͨ������1��ӡ����         
 * ʵ��ƽ̨��Mini STM32������  STM32F103RBT6
 * Ӳ�����ӣ�------------------
 *          | PC8  - TRIG      |
 *          | PC7  - ECHO      |
 *           ------------------
 * ��汾  ��ST3.5.0
 *
 * ����    ��Lee 
*********************************************************************************/

#include "UltrasonicWave.h"
#include "usart.h"
#include "timer.h"
#include "delay.h"
#include "stdio.h"
#include "string.h"
#define	TRIG_PORT      GPIOC				//TRIG       
#define	ECHO_PORT      GPIOC				//ECHO 
#define	TRIG_PIN       GPIO_Pin_8   //TRIG       
#define	ECHO_PIN       GPIO_Pin_7		//ECHO   

uint8_t UltrasonicWave_Distance;      //������ľ���    


/*
 * ��������UltrasonicWave_Configuration
 * ����  ��������ģ��ĳ�ʼ��
 * ����  ����
 * ���  ����	
 */
void UltrasonicWave_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;	
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//�ر�jtag
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);
    
  GPIO_InitStructure.GPIO_Pin = TRIG_PIN;					 //PC8��TRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //��Ϊ�������ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
  GPIO_Init(TRIG_PORT, &GPIO_InitStructure);	                 //��ʼ������GPIO 

  GPIO_InitStructure.GPIO_Pin = ECHO_PIN;				     //PC7��ECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;		 //��Ϊ����
  GPIO_Init(ECHO_PORT,&GPIO_InitStructure);						 //��ʼ��GPIOA
	
	 //GPIOC.7	  �ж����Լ��жϳ�ʼ������
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource7);

 	 EXTI_InitStructure.EXTI_Line=EXTI_Line7;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
		
			
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//�����ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}

u8 Get_Distance(){
	u8 ERROR=0;
			TIM_SetCounter(TIM2,0);
			TIM_Cmd(TIM2, ENABLE);                                             //����ʱ��
		
			while(GPIO_ReadInputDataBit(ECHO_PORT,ECHO_PIN));	                 //�ȴ��͵�ƽ

			TIM_Cmd(TIM2, DISABLE);			                                 //��ʱ��2ʧ��
			UltrasonicWave_Distance=TIM_GetCounter(TIM2)*5*34/100;									 //�������&&UltrasonicWave_Distance<150
		
	if(UltrasonicWave_Distance>0)
		return UltrasonicWave_Distance;
	else return ERROR;
}

void Distance_tx_lora(u8 distance,uint8_t Length){
		u8 t[100];
		sprintf(t,"%d",distance);
			strcat(t,"\r\n");
			drv_uart_tx_bytes(t, Length );			//??????? ashining ???
}

//void EXTI9_5_IRQHandler(void)
//{
//	u8 t[100];
//	delay_us(10);		                      //��ʱ10us
//     if(EXTI_GetITStatus(EXTI_Line7) != RESET)
//	{
//			TIM_SetCounter(TIM2,0);
//			TIM_Cmd(TIM2, ENABLE);                                             //����ʱ��
//		
//			while(GPIO_ReadInputDataBit(ECHO_PORT,ECHO_PIN));	                 //�ȴ��͵�ƽ

//			TIM_Cmd(TIM2, DISABLE);			                                 //��ʱ��2ʧ��
//			UltrasonicWave_Distance=TIM_GetCounter(TIM2)*5*34/100;									 //�������&&UltrasonicWave_Distance<150
//		
//	if(UltrasonicWave_Distance>0)
//	{
////		#ifdef __AS62_TX_MODE__ 
//		
//			sprintf(t,"%d",UltrasonicWave_Distance);
//			strcat(t,"\r\n");
//			drv_uart_tx_bytes(t, 8 );			//??????? ashining ???
//		
//		delay_ms( 1500 );
//				
//	
//	
////		#endif
////			drv_uart_tx_bytes(( uint8_t *)UltrasonicWave_Distance, 8 );			//??????? ashining ???
////					for(t=0;t<60000;t++) {GPIO_ResetBits(GPIOA,GPIO_Pin_2);}
////					for(t=0;t<60000;t++) GPIO_SetBits(GPIOA,GPIO_Pin_2);
////		delay_ms( 1500 );
//    	
//	}
//		
//	EXTI_ClearITPendingBit(EXTI_Line7);  //���EXTI7��·����λ
//}

//}

/*
 * ��������UltrasonicWave_StartMeasure
 * ����  ����ʼ��࣬����һ��>10us�����壬Ȼ��������صĸߵ�ƽʱ��
 * ����  ����
 * ���  ����	
 */
void UltrasonicWave_StartMeasure(void)
{
  GPIO_SetBits(TRIG_PORT,TRIG_PIN); 		  //��>10US�ĸߵ�ƽ�TRIG_PORT,TRIG_PIN��������define����?
  delay_us(20);		                      //��ʱ20US
  GPIO_ResetBits(TRIG_PORT,TRIG_PIN);
	
}

/******************* (C) 1209 Lab *****END OF FILE************/
