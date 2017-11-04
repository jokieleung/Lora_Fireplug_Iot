#ifndef __LCD_DRIVER_H
#define __LCD_DRIVER_H

/* ����ͷ�ļ� */
#include"stm32f10x.h"

/* TFT��ַ�ṹ�� */
typedef struct
{
	u16 TFT_CMD;
	u16 TFT_DATA;
} TFT_TypeDef;
/* ʹ��NOR/SRAM�� Bank1.sector4,��ַλHADDR[27,26]=11 A10��Ϊ�������������� */ 
/* ע������ʱSTM32�ڲ�������һλ����! 111110=0X3E */			    
#define TFT_BASE        ((uint32_t)(0x6C000000 | 0x000007FE))
#define TFT             ((TFT_TypeDef *) TFT_BASE)

/* �������Ĵ�С */
#define TFT_XMAX 239		//����TFT���Ĵ�С
#define TFT_YMAX 319

/* ������ɫ�ĺ� */
#define WHITE          0xFFFF
#define BLACK          0x0000
#define BLUE           0x001F
#define RED            0xF800
#define MAGENTA        0xF81F
#define GREEN          0x07E0
#define CYAN           0x7FFF
#define YELLOW         0xFFE0		 //������ɫ�ĺ�

/* ����ȫ�ֱ��� */
void TFT_WriteCmd(uint16_t cmd);
void TFT_WriteData(uint16_t dat);
void TFT_WriteData_Color(u16 color);

void TFT_Init(void);
void TFT_SetWindow(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
void TFT_ClearScreen(uint16_t color);

#endif

