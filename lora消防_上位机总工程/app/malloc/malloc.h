#ifndef __MALLOC_H
#define __MALLOC_H

/* �������õ�ͷ�ļ� */
#include"stm32f10x.h"

/* �����ڴ��������ڴ�����ȣ�IS62WV512��1MB�ֽڣ�������Ҫһ����������� */
/* ���б�һ��ҪС��1MB��  */
#define MEMORY_MAX_SIZE   200*1024  		//������Ϊ200KB

/* ����Ĺ���� */
#define MEMORY_BLOCK_SIZE 32                //�ڴ�鳤��

/* �ڴ�����ĳ��� = ���ڴ�����ȣ�MEMORY_MAX_SIZE�� / 32�� */
#define MAP_TABLE_SIZE    (MEMORY_MAX_SIZE / 32)


/* ����һ���ṹ�壬���������ڴ�״̬ */

typedef struct 
{
	uint8_t managementReady;	   //�ڴ����״̬	
	uint8_t *memoryBaseAddr;       //�ڴ���׵�ַ
	uint16_t *managementMap;	   //�ڴ�����
	uint16_t memoryFree;		   //�����ڴ��(û���õ����ڴ�)

} MemoryTypeDef;

/* ����һ���ڴ�״̬�ṹ�� */
extern MemoryTypeDef Memory;

/* ����ȫ�ֺ������ⲿ���ã�*/
void MEMORY_Init(void); 
void *malloc(uint32_t size);
void free(void *pro);






#endif
