#ifndef __EEPROM_H
#define __EEPROM_H

#include "stm32f10x.h"

#define USE_I2C_HARDWARE   //ʹ��Ӳ��I2C

#ifndef  USE_I2C_HARDWARE
    #include "iic.h"
#endif

/* AT24C02��I2C��ַ */
#define EEPROM_ADDR       0xA0
#define EEPROM_PAGE_SIZE  8



/* ����ȫ�ֱ��� */

#ifdef USE_I2C_HARDWARE
    void AT24C02_Init(void);
#else
    #define AT24C02_Init() {IIC_Config();}
#endif
int8_t AT24C02_WriteBuff(uint8_t *writeBuff, uint8_t writeAddr, uint8_t writeNumByte);
int8_t AT24C02_ReadBuff(uint8_t *readBuff, uint8_t readAddr, uint8_t readNumByte);





















#endif
