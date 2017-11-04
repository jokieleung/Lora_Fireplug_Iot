#ifndef __MYFATAPP_H
#define __MYFATAPP_H

#include"stm32f10x.h"
#include"ff.h"
#include"usart.h"

typedef struct
{
	uint8_t type[6];		   //��׺6���ֽ�
	uint8_t name[100];		   //·�����ļ�����100���ֽڣ�֧��25�����ִ�С���֣�
} FileNameTypeDef;



uint8_t FATFS_GetFree(uint8_t *drv, uint32_t *total, uint32_t *free);
uint8_t FATFS_ScanFiles(uint8_t *dir, FileNameTypeDef *file);
//uint8_t FATFS_ReadFile(void);

#endif
