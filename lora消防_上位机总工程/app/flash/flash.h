#ifndef __FLASH_H
#define __FLASH_H


#include "stm32f10x.h"

/* ����FLASH ʹ�õ�IO�� */
#define FLASH_CS_SET GPIO_SetBits(GPIOG, GPIO_Pin_13)
#define FLASH_CS_CLR {GPIO_ResetBits(GPIOG, GPIO_Pin_13);\
GPIO_SetBits(GPIOG, GPIO_Pin_14);GPIO_SetBits(GPIOB, GPIO_Pin_12);}

/* �������FLASH��ID */
#define EN25Q80 	0X1C13 	
#define EN25Q16 	0X1C14
#define EN25Q32 	0X1C15
#define EN25Q64 	0X1C16

/* ����ָ��� */
#define EN25X_WriteStatusReg    0x01   //д״̬�Ĵ���
#define EN25X_PageProgram		0x02   //ҳ�༭
#define EN25X_ReadData          0x03   //д����
#define EN25X_WriteDisable		0x04   //дʧ��
#define EN25X_ReadStatusReg		0x05   //��ȡ״̬�Ĵ���
#define EN25X_WriteEnable		0x06   //дʹ��
#define EN25X_SectorErase		0x20   //������
#define EN25X_ChipErase			0xC7   //����Ƭ



/* �����ⲿ���ú��� */
uint16_t FLASH_ReadID(void);
uint16_t FLASH_Init(void);
void FLASH_ChipErase(void);
void FLASH_ReadData(uint8_t *readBuff, uint32_t readAddr, uint16_t readByteNum);
void FLASH_WriteData(uint8_t *writeBuff, uint32_t writeAddr, uint16_t writeByteNum);















#endif
