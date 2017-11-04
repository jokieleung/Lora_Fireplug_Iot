#include"malloc.h"

/* �����пռ䶨��λһ�����ݣ��׵�ַ��0x68000000��ʼ������4�ֽڶ��� */
__align(4) uint8_t MemoryBase[MEMORY_MAX_SIZE] __attribute__((at(0X68000000)));

/* ����һ���ڴ����� */
uint16_t MemoryManagementMap[MAP_TABLE_SIZE] __attribute__((at(0X68000000 + MEMORY_MAX_SIZE)));

/* ����һ���ڴ�״̬�ṹ�� ,ͬʱ��ʼ�� */
MemoryTypeDef Memory = 
{
	0,						//�ڴ����״̬������
	MemoryBase,				//�ڴ���׵�ַ
	MemoryManagementMap,	//�ڴ�����
	MAP_TABLE_SIZE,			//�����ڴ��
};

/****************************************************************************
* Function Name  : MEMORY_Set
* Description    : �����ڴ����״̬
* Input          : s�������ڴ����׵�ַ
*                * length�����õĳ���
*                * dat�����õ�״̬
* Output         : None
* Return         : None
****************************************************************************/

static void MEMORY_Set(uint16_t *s, uint32_t length, uint8_t dat)
{
	while(length--)
	{
		*s = dat;
		s++;		
	}	
}

/****************************************************************************
* Function Name  : MEMORY_Init
* Description    : ��ʼ���ڴ�����������ڴ����������
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void MEMORY_Init(void)
{
	/* ������λ���� */
	MEMORY_Set(MemoryManagementMap, MAP_TABLE_SIZE, 0);
	Memory.managementReady = 1;	
}

/****************************************************************************
* Function Name  : MEMORY_Maloc
* Description    : ����һ�������ĵĿռ䣬�������ڴ�ص�ַƫ����
* Input          : size�����ٿռ�Ĵ�С
* Output         : None
* Return         : 0xFFFFFFFF����ʾ����ʧ�ܣ�������ʾ���ڴ���еĵ�ַƫ����
****************************************************************************/

static uint32_t MEMORY_Maloc(uint32_t size)
{	
	uint16_t block, emptyBlock;
	int32_t addrOffset, j;
	
	/* ��δ��ʼ�����ȳ�ʼ�� */
	if(!Memory.managementReady)
	{
		MEMORY_Init();	
	}
	
	if((size == 0) || (size > (Memory.memoryFree * MEMORY_BLOCK_SIZE)))
	{
		return 0xFFFFFFFF;                //�����ݻ򳬳����ȣ�����ʧ��
	}

	block = size / MEMORY_BLOCK_SIZE;	  //�����Ҫ���ٸ��ڴ��
	if((size % MEMORY_BLOCK_SIZE) != 0)	  //���л�����������ôҪ���һ���ڴ��
	{
		block++;
	}

	/* ����ȫ�ڴ������ҵ�һ���������ڴ�飬������ַƫ�Ʒ��� */
	emptyBlock = 0;
	for(addrOffset=MAP_TABLE_SIZE - 1; addrOffset>=0; addrOffset--)
	{
		if(Memory.managementMap[addrOffset] == 0) 
		{
			emptyBlock++;	
		}
		else
		{
			emptyBlock = 0;
		}
		if(emptyBlock == block)
		{
			
			/* ���ڴ����Ϊʹ��״̬ */
			for(j=0; j<block; j++)
			{	
				/* ���ڴ���Сд�뵽�����������У������ͷ�ʱ��ȡ�ڴ�ռ� */
				/* �Ĵ�С�� */
				Memory.managementMap[addrOffset + j] = block; 				
			}
			Memory.memoryFree -= block;    //�����ڴ����� 

			/* ƫ�Ƶ�ַҪ*�ڴ���С�����ڴ�ص�ַƫ�� */
			return (addrOffset * MEMORY_BLOCK_SIZE);
		}			
	}
	
	/* ������ɨ���궼û���ҵ����ʵ��ڴ�飬����ʧ�� */
	return 0xFFFFFFFF;	
}

/****************************************************************************
* Function Name  : MEMORY_Free
* Description    : �ͷſ��ٵ��ڴ�ռ�.
* Input          : addrOffset���ڴ�صĵ�ַƫ����
* Output         : None
* Return         : 0���ͷ���ɣ�0xFF���������ͷŻ����ͷ�ʧ��
****************************************************************************/

static uint8_t MEMORY_Free(uint32_t addrOffset)
{
	uint16_t block;

	/* ��δ��ʼ�����ȳ�ʼ�� */
	if(!Memory.managementReady)
	{
		MEMORY_Init();
		return 0xFF;	
	}
	if(addrOffset < MEMORY_MAX_SIZE) //���ڴ����
	{
		/* �Ƚ��ڴ�ص�ַƫ����ת��Ϊ�ڴ�����ƫ���� */
		addrOffset = addrOffset / MEMORY_BLOCK_SIZE;

		/* ��ȡ���ٵ��ڴ�ռ��С */
		block = Memory.managementMap[addrOffset];

		/* ���ͷ��ڴ���ڴ������ͷ� */
		MEMORY_Set(&Memory.managementMap[addrOffset], block, 0);
		Memory.memoryFree += block;    //�����ڴ������
		return 0;
	}
	else
	{
		return 0xFF;	
	}
}

/****************************************************************************
* Function Name  : malloc
* Description    : ����һ���ڴ�ռ�
* Input          : size�����ٿռ�Ĵ�С
* Output         : None
* Return         : ���ٿռ�ĵ�ַָ��
****************************************************************************/

void *malloc(uint32_t size)
{
	uint32_t addrOffset;

	addrOffset = MEMORY_Maloc(size);  //����һ�����ʵ��ڴ�ռ�
	
	if(addrOffset == 0xFFFFFFFF)      //����ʧ��
	{
		return 0;				      //����NULL����0��
	}
	
	/* ����ַƫ����ת����ָ���ַ���� */
	return (void *)((uint32_t)Memory.memoryBaseAddr + addrOffset);	
}

/****************************************************************************
* Function Name  : free
* Description    : �ͷ�ָ�����ڴ�ռ�
* Input          : pro��Ҫ�ͷŵ��ڴ�ռ�ָ���ַ
* Output         : None
* Return         : None
****************************************************************************/

void free(void *pro)
{
	uint32_t addrOffset;

	if(pro == 0)	   //��ָ��ΪNULL��ʱ��
	{
		return;
	}

	/* ��ȡ���ڴ�ռ����ڴ������ĵ�ַƫ���� */
	addrOffset = (uint32_t)pro - (uint32_t)Memory.memoryBaseAddr;

	/* �ͷ��ڴ� */
	MEMORY_Free(addrOffset);

	/* �ͷ�proָ�� */
	pro = 0;
}

//void MEMORY_Copy(uint16_t cut, uint16_t paste)
//{
//	uint16_t i;
//	uint32_t cutOffset, pasteOffset, mapData;
//	
//	//-- ��ȡ�ڴ��������ڴ��Ĵ�С --//
//	mapData = Memory.managementMap[cut];
//
//	//-- ���ڴ��ƫ�Ƶ�ַתΪ�ڴ��ƫ�Ƶ�ַ	--//
//	cutOffset = cut * MEMORY_BLOCK_SIZE;	 
//	pasteOffset = paste * MEMORY_BLOCK_SIZE;
//	
//	//-- �����ڴ���е�����,һ���ڴ��һ����MEMORY_BLOCK_SIZE����--//
//	for(i=0; i<MEMORY_BLOCK_SIZE; i++)
//	{
//		MemoryBase[pasteOffset] = MemoryBase[cutOffset];
//		pasteOffset++;
//		cutOffset++;
//	}
//
//	//-- ���ڴ������е�״̬�л� --//
//	Memory.managementMap[cut] = 0;
//	Memory.managementMap[paste] = mapData;
//}
//void MEMORY_Optimization(void)
//{
//	uint32_t emptyBlock;
//	int32_t block;
//
//	for(block=MAP_TABLE_SIZE - 1; block>=0; block--)
//	{
//		if(Memory.managementMap[block] == 0)
//		{
//			emptyBlock = block;	// ��ȡ�ڴ���ƫ����
//			while(block >= 0)
//			{
//				if(Memory.managementMap[block] != 0)
//				{
//					MEMORY_Copy(block, emptyBlock);
//					emptyBlock--;		 
//				}
//				block--;	
//			}
//		}			
//	}
//}




