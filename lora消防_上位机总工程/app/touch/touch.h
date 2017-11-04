#ifndef __TOUCH_H
#define __TOUCH_H

/* ����ͷ�ļ� */
#include"stm32f10x.h"
#include "system.h"

/* ������������ */
typedef struct 
{
	uint16_t x;
	uint16_t y;
    uint16_t lcdx;
    uint16_t lcdy;
} TouchTypeDef;

typedef struct{
    uint8_t posState;   
    int8_t xOffset;
    int8_t yOffset; 
    float xFactor;
    float yFactor;
} PosTypeDef;

#define TOUCH_ADJ_OK          'Y'              //��ʾ����У������׼����
#define TOUCH_ADJ_ADDR        6112800          //У��������FLASH�е��׵�ַ
/* ����У���������� */
#define LCD_ADJX_MIN (10)                      //��ȡ�ĸ������СXֵ
#define LCD_ADJX_MAX (TFT_XMAX - LCD_ADJX_MIN) //��ȡ�ĸ�������Xֵ
#define LCD_ADJY_MIN (10)                      //��ȡ�ĸ������СYֵ
#define LCD_ADJY_MAX (TFT_YMAX - LCD_ADJY_MIN) //��ȡ�ĸ�������Yֵ

#define LCD_ADJ_X (LCD_ADJX_MAX - LCD_ADJY_MIN)//��ȡ����Ŀ��
#define LCD_ADJ_Y (LCD_ADJY_MAX - LCD_ADJY_MIN)//��ȡ����ĸ߶�

#define TOUCH_READ_TIMES 5     //һ�ζ�ȡ����ֵ�Ĵ���

#define TOUCH_X_CMD      0xD0  //��ȡX������
#define TOUCH_Y_CMD      0x90  //��ȡY������
#define TOUCH_MAX        20    //Ԥ�ڲ�ֵ
#define TOUCH_X_MAX      4000  //X�����ֵ
#define TOUCH_X_MIN      100   //X����Сֵ
#define TOUCH_Y_MAX      4000  //Y�����ֵ
#define TOUCH_Y_MIN      100   //Y����Сֵ

/* CSƬѡ�ź� */
#define TOUCH_CS_CLR GPIO_ResetBits(GPIOD, GPIO_Pin_6);
#define TOUCH_CS_SET GPIO_SetBits(GPIOD, GPIO_Pin_6);

/* PEN�˿� */
#define TOUCH_PEN PDin(7)

extern TouchTypeDef TouchData;

void TOUCH_Init(void);
uint8_t TOUCH_Scan(void);

#endif
