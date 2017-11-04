#ifndef _KEY_H
#define _KEY_H

#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "delay.h"


#define KEY0  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)//读取按键0
#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//读取按键1
#define KEY2  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//读取按键2 
#define KEY3  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)//读取按键3

 

#define KEY0_PRES 	1	//KEY0按下
#define KEY1_PRES		2	//KEY1按下
#define KEY2_PRES		3	//KEY2按下
#define KEY3_PRES   4	//KEY_UP按下(即WK_UP/KEY_UP)


void KEY_Init(void);//IO初始化
u8 KEY_Scan(void);  	//按键扫描函数			

#endif
