#ifndef __ADX345_H
#define __ADX345_H


#include "stm32f10x.h"

/* ADX345��I2C��ַ */
#define ADX345_ADDR        0x3A   //ADX345��I2C��ַ

/* ADXL345��ָ��� */
#define ADX_DEVID          0x00   //����ID
#define ADX_DATA_FORMAT    0x31   //���ݸ�ʽ����
#define ADX_BW_RATE        0x2C   //�������ʼ�����ģʽ����
#define ADX_POWER_CTL      0x2D   //ʡ�����Կ���
#define ADX_INT_ENABLE     0x2E   //�ж�ʹ�ܿ���
#define ADX_OFSX           0x1E   //�û���ֵ
#define ADX_OFSY           0x1F   //X��ƫ��
#define ADX_OFSZ           0x20   //Y��ƫ��


/* ����ȫ�ֺ��� */
int ADX345_Init(void);
void ADX345_Adjust(void);
void ADX_GetXYZData(int16_t *xValue, int16_t *yValue, int16_t *zValue);
int16_t ADX_GetAngle(float xValue, float yValue, float zValue, uint8_t dir);























#endif
