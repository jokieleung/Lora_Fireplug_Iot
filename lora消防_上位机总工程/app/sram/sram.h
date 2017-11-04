#ifndef __SRAM_H
#define __SRAM_H


#include"stm32f10x.h"

/* ����FSMC�ĵ�ַ */
#define Bank1_SRAM3_ADDR    ((uint32_t)(0x68000000))		

/* ����ȫ�ֺ��� */
void SRAM_Config(void);
void SRAM_WriteBuffer(uint16_t *writeBuf, uint32_t writeAddr, uint32_t length);
void SRAM_ReadBuffer(uint16_t* readBuff, uint32_t readAddr, uint32_t length);

#endif
