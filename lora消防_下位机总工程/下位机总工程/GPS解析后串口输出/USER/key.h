#ifndef _KEY_H
#define _KEY_H

#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "delay.h"


#define KEY0  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)//��ȡ����0
#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//��ȡ����1
#define KEY2  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//��ȡ����2 
#define KEY3  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)//��ȡ����3

 

#define KEY0_PRES 	1	//KEY0����
#define KEY1_PRES		2	//KEY1����
#define KEY2_PRES		3	//KEY2����
#define KEY3_PRES   4	//KEY_UP����(��WK_UP/KEY_UP)


void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(void);  	//����ɨ�躯��			

#endif
