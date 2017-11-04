#include "flash.h"
#include "spi.h"
#include "malloc.h"

/* �����ڲ����ú��� */
static void FLASH_SectorErase(uint32_t eraseAddr);
static void FLASH_WriteEnable(void);
static uint8_t FLASH_CheckBusy(void);
static void FLASH_WritePage(uint8_t *writeBuff, uint32_t writeAddr, uint16_t writeByteNum);

/****************************************************************************
* Function Name  : FLASH_Init
* Description    : ��ʼ��FLASH��IO��SPI.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

uint16_t FLASH_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOG, ENABLE);
	
	/* SD_CS PG14/ FLASH_CS PG13 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_SetBits(GPIOG, GPIO_Pin_14 | GPIO_Pin_13);
	
	/* ENC28J60_CS PB12 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_12); 

    /* ��ʼ��SPI2 */
    SPI2_Config();

    /* ��ʼ��FLASHҪ��ȡоƬIDһ�� */
	return(FLASH_ReadID());   
}

/****************************************************************************
* Function Name  : FLASH_ReadID
* Description    : ��ȡFLASH���ͺ�ID��
* Input          : None
* Output         : None
* Return         : ��ȡ����ID����
****************************************************************************/

uint16_t FLASH_ReadID(void)
{
	uint16_t id = 0;
	
	FLASH_CS_CLR;            //��Ƭѡ
	
	SPI2_WriteReadData(0x90);//���Ͷ�ȡID����	    
	SPI2_WriteReadData(0x00); 	    
	SPI2_WriteReadData(0x00); 	    
	SPI2_WriteReadData(0x00);

	id |= SPI2_WriteReadData(0xFF) << 8; //��ȡ16λID 
	id |= SPI2_WriteReadData(0xFF);
		 
	FLASH_CS_SET;			 //�ر�Ƭѡ   
	return id;	
}

/****************************************************************************
* Function Name  : FLASH_ReadData
* Description    : ��ȡFLASH������.
* Input          : readBuff����ȡ������
*                * readAddr����ȡ��ַ
*                * readByteNum����ȡ���ݳ���
* Output         : None
* Return         : None
****************************************************************************/

void FLASH_ReadData(uint8_t *readBuff, uint32_t readAddr, uint16_t readByteNum)
{
    SPI2_SetSpeed(SPI_BaudRatePrescaler_2);
	FLASH_CS_CLR;    //��Ƭѡ

	/* д��ȡ���� */
	SPI2_WriteReadData(EN25X_ReadData);

	/* ����24λ��ȡ��ַ */
	SPI2_WriteReadData(readAddr >> 16);
	SPI2_WriteReadData(readAddr >> 8);
	SPI2_WriteReadData(readAddr);

	/* ��ȡ���� */
	while(readByteNum--)
	{
		*readBuff = SPI2_WriteReadData(0xFF);
		readBuff++;
	}

	FLASH_CS_SET;   //�ر�Ƭѡ
}

/****************************************************************************
* Function Name  : FLASH_CheckBusy
* Description    : ���FLASH�Ƿ�æµ.
* Input          : None
* Output         : None
* Return         : 1����æµ��0���ȴ���ʱ��
****************************************************************************/

static uint8_t FLASH_CheckBusy(void)
{
	uint8_t statusValue;
	uint32_t timeCount = 0;
	
	do
	{
		timeCount++;
		if(timeCount > 0xEFFFFFFF)				   //�ȴ���ʱ
		{
			return 0;
		}
		FLASH_CS_CLR;                              //ʹ������
		   
		SPI2_WriteReadData(EN25X_ReadStatusReg);   //���Ͷ�ȡ״̬�Ĵ�������    
		statusValue = SPI2_WriteReadData(0xFF);    //��ȡһ���ֽ�  
	
		FLASH_CS_SET;                              //ȡ��Ƭѡ
	}
	while((statusValue & 0x01) == 0x01);    	   // �ȴ�BUSYλ���

	return 1; 
}

/****************************************************************************
* Function Name  : FLASH_WriteEnable
* Description    : дʹ��
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

static void FLASH_WriteEnable(void)
{
	FLASH_CS_CLR;                               //ʹ������  
	 
    SPI2_WriteReadData(EN25X_WriteEnable);       //����дʹ�� 
	 
	FLASH_CS_SET;                               //ȡ��Ƭѡ     	      	
}

/****************************************************************************
* Function Name  : FLASH_SectorErase
* Description    : ��������
* Input          : eraseAddr��������ַ
* Output         : None
* Return         : None
****************************************************************************/

static void FLASH_SectorErase(uint32_t eraseAddr)
{	
	FLASH_WriteEnable(); //����дʹ��
	FLASH_CheckBusy();	 //ȷ��FLASH�Ƿ���æ״̬

	FLASH_CS_CLR;        //ʹ������

	SPI2_WriteReadData(EN25X_SectorErase); //��������

	SPI2_WriteReadData(eraseAddr >> 16);  //����24λ��ַ
	SPI2_WriteReadData(eraseAddr >> 8);
	SPI2_WriteReadData(eraseAddr);

	FLASH_CS_SET;                         //ȡ��Ƭѡ     	      	 
}

/****************************************************************************
* Function Name  : FLASH_ChipErase
* Description    : ��Ƭ����
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void FLASH_ChipErase(void)
{
	FLASH_CheckBusy();	 //ȷ��FLASH�Ƿ���æ״̬
	FLASH_WriteEnable(); //����дʹ��

	FLASH_CS_CLR;        //ʹ������

	SPI2_WriteReadData(EN25X_ChipErase); //��������

	FLASH_CS_SET;                         //ȡ��Ƭѡ
    FLASH_CheckBusy();	 //ȷ��FLASH�Ƿ���æ״̬
}

/****************************************************************************
* Function Name  : FLASH_WritePage
* Description    : ��յ�FLASH����д���ݣ�ע���ǿյģ���
* Input          : writeBuff��д������
*                * writeAddr��д���ַ
*                * writeByteNum��д�����ݳ���
* Output         : None
* Return         : None
****************************************************************************/

static void FLASH_WritePage(uint8_t *writeBuff, uint32_t writeAddr, uint16_t writeByteNum)
{
	uint16_t byteNum, i;

	byteNum = writeAddr % 256;	  
	byteNum = 256 - byteNum;		  //�����ҳʣ���ַ
	
	if(writeByteNum <= byteNum)	      //��д���ֽ�������ҳʣ���ַ
	{
		byteNum = writeByteNum;
	}
				
	/* ��ʼд�� */
	while(1)
	{  
		/* д������ */
		FLASH_CheckBusy();				      //ȷ��FLASH�Ƿ���æ״̬
		FLASH_WriteEnable();		          //����дʹ��
 
		FLASH_CS_CLR;                         //ʹ������ 
		
		SPI2_WriteReadData(EN25X_PageProgram); //����д����
	
		SPI2_WriteReadData(writeAddr >> 16);  //����24λ��ȡ��ַ
		SPI2_WriteReadData(writeAddr >> 8);
		SPI2_WriteReadData(writeAddr);

	    for(i=0; i<byteNum; i++)				  //ѭ��д��
		{
			SPI2_WriteReadData(*writeBuff);
			writeBuff++;	
		}

		FLASH_CS_SET;                         //ȡ��Ƭѡ 

		/* �ж��Ƿ�д�� */
		if(writeByteNum == byteNum) //���д���ֽ�������ʣ���ֽ�����ʾд�����	
		{
			break;
		}
		else	                    //���δд�����
		{
			writeAddr += byteNum;					 //д���ַƫ��
			writeByteNum = writeByteNum - byteNum;	 //���ʣ���ֽ���

			if(writeByteNum >= 256)	//���ʣ���ֽ�������256����ôһ��д��һҳ
			{
				byteNum = 256;	
			}
			else					//���ʣ���ֽ���С��256����ôһ��ȫ��д��
			{
				byteNum = writeByteNum;
			}
		}
	} 	
}

/****************************************************************************
* Function Name  : FLASH_WriteData
* Description    : ��FLASH����д���ݣ�FLASH���Բ�Ϊ�գ�
* Input          : writeBuff��д������
*                * writeAddr��д���ַ
*                * writeByteNum��д�����ݳ���
* Output         : None
* Return         : None
****************************************************************************/
#ifndef __MALLOC_H
static uint8_t flashBuff[4096]; //��ȡ����������֪��Ϊʲô���ܷŵ��Ӻ������档��
#endif

void FLASH_WriteData(uint8_t *writeBuff, uint32_t writeAddr, uint16_t writeByteNum)
{
	
	uint16_t sectorAddr, byteAddr, byte, i;

#ifdef __MALLOC_H	
    uint8_t *p;
    p = malloc(4096);                  //����һ���ڴ��
    if(p == 0)
    {
        return;
    }
#endif
    SPI2_SetSpeed(SPI_BaudRatePrescaler_2);
	sectorAddr = writeAddr / 4096;	   //������ַ
	byteAddr = writeAddr % 4096;	   //����ƫ�Ƶ�ַ
	byte = 4096 - byteAddr;            //����ʣ���ַ

	if(writeByteNum < byte)	   //���д���ֽ�С������ʣ��ռ�(С��һ������)
	{
		byte = writeByteNum;
	}
	while(1)
	{

#ifdef __MALLOC_H
        FLASH_ReadData(p, sectorAddr*4096, 4096);         //��ȡһ������������
#else
		FLASH_ReadData(flashBuff, sectorAddr*4096, 4096); //��ȡһ������������
#endif
		for(i=0; i<byte; i++)	  		//���Ҫд��ĵ�ַ�ǲ��ǿյ�
		{
#ifdef __MALLOC_H
            if(p[byteAddr + i] != 0xFF)
#else       
			if(flashBuff[byteAddr + i] != 0xFF)	//�������
#endif
			{
				break;
			} 		
		}

		if(i < byte) //���д���ַ����������
		{
			FLASH_SectorErase(sectorAddr*4096); //�����������
			
			for(i=0; i<byte; i++)				//��������
			{
#ifdef __MALLOC_H
                p[byteAddr + i] = writeBuff[i];
#else       
				flashBuff[byteAddr + i] = writeBuff[i];
#endif

			}
#ifdef __MALLOC_H
			FLASH_WritePage(p, sectorAddr*4096, 4096);
#else       
			FLASH_WritePage(flashBuff, sectorAddr*4096, 4096);
#endif		
		}
		else		 //Ҫд��ĵ�ַ��������
		{
			FLASH_WritePage(writeBuff, writeAddr, byte);			
		}

		/* �ж��Ƿ�д�� */
		if(byte == writeByteNum)		//���д���ֽ�������ʣ���ֽ�����ʾд�����
		{
			break;
		}
		else                        //���δд��
		{
			writeBuff += byte;		//����ָ��ƫ��
			writeAddr += byte;		//���д���ַƫ��
			writeByteNum -= byte;   //���д������ʣ����
			sectorAddr++;			//������ַ+1
			byteAddr = 0;           //����ƫ�Ƶ�ַΪ0

			if(writeByteNum >= 4096)
			{
				byte = 4096;	
			}
			else
			{
				byte = writeByteNum;
			}		
		}
	}
#ifdef __MALLOC_H	
    free(p);
#endif    	 				
}

