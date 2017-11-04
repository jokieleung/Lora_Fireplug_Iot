#ifndef __ADC_H
#define __ADC_H


#include "stm32f10x.h"

/* ����ADC�Ķ��� */
#define ADC_Chn0 0x01
#define ADC_Chn1 0x02
#define ADC_Chn2 0x04

/* ����ȫ�ֱ��� */
void ADC_Config(uint8_t adc);
uint16_t ADC_ReadChn0(void);
uint16_t ADC_ReadChn1(void);
uint16_t ADC_ReadChn2(void);































#endif
