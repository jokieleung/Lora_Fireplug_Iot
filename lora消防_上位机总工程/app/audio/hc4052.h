#ifndef __HC4052_H
#define __HC4052_H

#include "stm32f10x.h"

/* ����ͨ�� */
#define AUDIO_MP3_CHANNEL  ((uint8_t)0x01)
#define AUDIO_FM_CHANNEL   ((uint8_t)0x02)

/* ����IO */
#define MC_A_CLR GPIO_ResetBits(GPIOF, GPIO_Pin_10)
#define MC_A_SET GPIO_SetBits(GPIOF, GPIO_Pin_10)

#define MC_B_CLR GPIO_ResetBits(GPIOF, GPIO_Pin_11)
#define MC_B_SET GPIO_SetBits(GPIOF, GPIO_Pin_11)

/* ����ȫ�ֺ��� */
void HC4052_Init(void);
void HC4052_OutputSelect(uint8_t channel);













#endif
