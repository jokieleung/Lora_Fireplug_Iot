#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"

//#define USE_EXTI    //ʹ���ⲿ�ж�

/* ����Ҫ���õİ���ʹ�õ�PIN�� */
#define GPIO_Pin_KEY (GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4)

/* ������ȡ */
#define KEY0 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)
#define KEY1 GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2)
#define KEY2 GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)
#define KEY3 GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)

/* ���尴���ļ�ֵ */
#define KEY_UP    0x01
#define KEY_DOWN  0x02
#define KEY_LEFT  0x04
#define KEY_RIGHT 0x08
#define KEY_ALL   0x0F
/* ����ȫ�ֺ��� */
void KEY_Config(void);
uint8_t KEY_Scan(void);
void KEY_NVIC_Config(uint8_t key);





#endif
