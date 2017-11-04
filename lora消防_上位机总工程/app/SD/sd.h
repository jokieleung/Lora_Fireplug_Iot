#ifndef __SD_H
#define __SD_H



#include "stm32f10x.h"

/* ���ȫ�ֱ��� */
extern uint8_t SD_TYPE;	   //����SD��������

/* ���忨������ */
#define SD_TYPE_ERR     ((uint8_t)0X00)
#define SD_TYPE_MMC     ((uint8_t)0X01)
#define SD_TYPE_V1      ((uint8_t)0X02)
#define SD_TYPE_V2      ((uint8_t)0X04)
#define SD_TYPE_V2HC    ((uint8_t)0X06)

/* ����IO�� */
#define SD_CS_SET GPIO_SetBits(GPIOG, GPIO_Pin_14)
#define SD_CS_CLR GPIO_ResetBits(GPIOG, GPIO_Pin_14)

/* SD��ָ��� */  	   
#define SD_CMD0    0       //����λ
#define SD_CMD1    1
#define SD_CMD8    8       //����8 ��SEND_IF_COND
#define SD_CMD9    9       //����9 ����CSD����
#define SD_CMD10   10      //����10����CID����
#define SD_CMD12   12      //����12��ֹͣ���ݴ���
#define SD_CMD16   16      //����16������SectorSize Ӧ����0x00
#define SD_CMD17   17      //����17����sector
#define SD_CMD18   18      //����18����Multi sector
#define SD_CMD23   23      //����23�����ö�sectorд��ǰԤ�Ȳ���N��block
#define SD_CMD24   24      //����24��дsector
#define SD_CMD25   25      //����25��дMulti sector
#define SD_CMD41   41      //����41��Ӧ����0x00
#define SD_CMD55   55      //����55��Ӧ����0x01
#define SD_CMD58   58      //����58����OCR��Ϣ
#define SD_CMD59   59      //����59��ʹ��/��ֹCRC��Ӧ����0x00

/* ����ȫ�ֺ��� */
int8_t SD_Init(void);
int8_t SD_ReadCapacity(uint32_t *capacity);
int8_t SD_ReadDisk(uint8_t *buf, uint32_t sector, uint8_t num);
int8_t SD_WriteDisk(uint8_t *buf, uint32_t sector, uint8_t num);











#endif
