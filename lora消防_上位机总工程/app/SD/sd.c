#include "sd.h"
#include "spi.h"

/* ����ȫ�ֱ��� */
uint8_t SD_TYPE;	  //����SD��������

/****************************************************************************
* Function Name  : SD_GPIO_Config
* Description    : ��ʼ��SDʹ�õ�IO��SPI.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

static void SD_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOG, ENABLE);
	
	/* SD_CS PG14/ FLASH_CS PG13 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_SetBits(GPIOG, GPIO_Pin_14);
	GPIO_SetBits(GPIOG, GPIO_Pin_13);
	
	/* ENC28J60_CS PB12 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_12); //����Ӱ��SD��

	SPI2_Config();
}

/****************************************************************************
* Function Name  : SD_WriteCmd
* Description    : ��SD������һ������.SD��һ�������ʽ����8λ���
*                * 32λ���������7λCRCЧ��λ�����һλΪ1
* Input          : cmd������
*                * dat: �������
*                * crc: CRCЧ��
* Output         : None
* Return         : r1��SD�����ص�����
*                * 0xFF����ʾ����ʧ��
****************************************************************************/

static uint8_t SD_WriteCmd(uint8_t cmd, uint32_t dat, uint8_t crc)
{	 
	uint8_t r1 = 0;
	uint16_t i = 0;

	//--��λSD��,ȡ���ϴ�Ƭѡ--//
	SD_CS_SET;
	SPI2_WriteReadData(0xFF);	 //�����ṩ8��ʱ��

	SD_CS_CLR;
	while(SPI2_WriteReadData(0xFF) != 0xFF) //�ȴ����Ƿ�׼����
	{
		i++;
		if(i > 100)
		{
			return 0xFF;	                 //�ȴ�ʧ�ܷ���
		}
	}

	//--��������--//
	SPI2_WriteReadData(cmd | 0x40);

	SPI2_WriteReadData(dat >> 24);     //����Dat�����8λ
	SPI2_WriteReadData(dat >> 16);
	SPI2_WriteReadData(dat >> 8);
	SPI2_WriteReadData(dat & 0x00FF);

	SPI2_WriteReadData(crc);
	if(cmd == SD_CMD12)		 //�����ֹͣ���ݴ����������෢һ��ʱ��
	{
		SPI2_WriteReadData(0xFF);
	}

	i = 0;
	do
	{
		r1 = SPI2_WriteReadData(0xFF);	
		i++;
		if(i > 100)
		{
			return 0xFF;
		}
	}
	while((r1 & 0x80) != 0);   //���ͳɹ������λ��0 

	return r1;
}

/****************************************************************************
* Function Name  : SD_Init
* Description    : ��ʼ��SD��.
* Input          : None
* Output         : None
* Return         : 0xFF����ʼ��ʧ��
*		         * 0����ʼ���ɹ�
****************************************************************************/

int8_t SD_Init(void)
{
	uint8_t r1, buf[4];
	uint16_t i = 0;

	SD_GPIO_Config();
	
	SPI2_SetSpeed(SPI_BaudRatePrescaler_256);
	//--��SD��ͨ��ģʽת��ΪSPIģʽ���ϵ�Ĭ������DAT0��������--//
	//--�ӵ�CMD0ʱ��CS�ź���Ч��SPIģʽ����--//
	for(i=0; i<0x0F; i++)//��ʼʱ���ȷ�������74��ʱ�ӣ�����Ǳ���ġ�
	{
		SPI2_WriteReadData(0xFF);	 //����8*16��
	}

	//����ȡ��0x01��ʱ���ʾ��ʼ���ɹ�
	i = 0;
	while(SD_WriteCmd(SD_CMD0, 0, 0x95) != 0x01)
	{
		i++;
		if(i > 100)
		{
			return 0xFF;        //��ʼ��ʧ�ܷ���0
		}
	}
		
	//--����CMD8������Ƿ�SD V2.0--//
	i = 0;
	do
	{
		i++;
		if(i > 100)	//���Ƿ��ͳ�����������ѭ����
		{
			break;
		}
		r1 = SD_WriteCmd(SD_CMD8, 0x01AA, 0x87);
	}
	while(r1 != 0x01); //����CMD8
	
	if(r1 == 0x01)     //���CMD8�л�Ӧ˵����SD V2.0Э��
	{
		//--��ȡCMD8�ķ���ֵ������Ƿ�֧�ֵ�ѹ--//
		//--��ȡCMD8�ķ���ֵ������Ƿ�֧�ֵ�ѹ--//
		for(i=0; i<4; i++)
		{
			buf[i] = SPI2_WriteReadData(0xFF);
		}
		
		//--����ѹ��֧�ֵ�ѹ�����ش���--//
		if((buf[2] != 0x01) || (buf[3] != 0xAA))
		{
			return 0xFF;
		}
		
		//--��ʼ��SD��--//
		i = 0;
		do
		{
			i++;
			if(i > 100)
			{
				return 0xFF;   //����ʧ��
			}

			SD_WriteCmd(SD_CMD55, 0, 0x01);
			r1 = SD_WriteCmd(SD_CMD41, 0x40000000, 0x01);
		}
		while(r1 != 0);

		//--�����SDHC������SD��--//
		i = 0;
		while(SD_WriteCmd(SD_CMD58, 0, 0x01) != 0)
		{
			i++;
			if(i > 100)
			{
				SD_TYPE = SD_TYPE_ERR;
				break;
			}		
		}

		//--��ȡOCR--//
		for(i=0; i<4; i++)
		{
			buf[i] = SPI2_WriteReadData(0xFF);
		}

		if(buf[0] & 0x40) 
		{
			SD_TYPE = SD_TYPE_V2HC;
		}
		else
		{
			SD_TYPE = SD_TYPE_V2;
		}	
	}
			
	else //�������SD V1.0����MMC V3
	{
		SD_WriteCmd(SD_CMD55, 0x00, 0x01);
		r1 = SD_WriteCmd(SD_CMD41, 0x00, 0x01);

		if(r1 <= 1)	   //��CMD41�л�Ӧ˵����SD V1.0
		{
			SD_TYPE = SD_TYPE_V1;	 //��V1.0��
			i = 0;
			do
			{
				if(i > 100)
				{
					return 0xFF;
				}

				SD_WriteCmd(SD_CMD55, 0x00, 0x01);
				r1 = SD_WriteCmd(SD_CMD41, 0x00, 0x01);	
			}
			while(r1 != 0);	
		}

		else		  //û�л�Ӧ˵����MMC V3
		{
			SD_TYPE = SD_TYPE_MMC;	 //��������MMC��
			i = 0;
			while(SD_WriteCmd(SD_CMD1, 0, 0x01) != 0)
			{
				i++;
				if(i > 100)
				{
					return 0xFF;
				}
			}	
		}
	}		

	SD_CS_SET;		   //ȡ��Ƭѡ
	SPI2_WriteReadData(0xFF); 
//	SPI2_SetSpeed(SPI_BaudRatePrescaler_2);

	return 0;
}

/****************************************************************************
* Function Name  : SD_ReadCapacity
* Description    : ��ȡSD�����ڴ��С.
* Input          : capacity����ȡ������ֵ����ָ��
* Output         : �ڴ��С����λΪKB
* Return         : 0xFF����ʼ��ʧ��
*		         * 0����ʼ���ɹ�
****************************************************************************/

int8_t SD_ReadCapacity(uint32_t *capacity)
{
	uint8_t csdValue[16];
	uint16_t n, i = 0;

	//--��������--//
	while(SD_WriteCmd(SD_CMD9, 0, 0x01) != 0);
	{
		i++;
		if(i > 100)
		{
			return 0xFF;  //��������ʧ��
		}
	}

	//--�ȴ���ʼ����0XFE--//
	i = 0;
	while(SPI2_WriteReadData(0xFF) != 0xFE)
	{
		i++;
		if(i > 500)
		{
			return 0xFF;
		}
	}

		
	//--��ȡ����--//
	for(i=0; i<16; i++)
	{
		csdValue[i] = SPI2_WriteReadData(0xFF);
	}

	//--��ȡ��λCRCЧ��--//
	SPI2_WriteReadData(0xFF);	  //RCC
	SPI2_WriteReadData(0xFF);
	
	//--ȡ��Ƭѡ--//
	SD_CS_SET;			  
	SPI2_WriteReadData(0xFF);

	//--SD V2.0�Ŀ�CSD��һ��������0x40--//
	if((csdValue[0] & 0xC0) == 0x40)
	{
		//--����C_SIZE����CSD[69:48]--//
		*capacity = csdValue[9] + ((uint16_t)csdValue[8] << 8) + 1;

		//--ʵ���Ͼ��ǳ���1024--//
		*capacity = (*capacity) << 10;//�õ�������			
	}
	else   
	{
		//--�ڴ��㷨�� capacity = BLOCKNR * BLOCK_LEN--//
		//--BLOCKNR = (C_SIZE + 1) * MULT��--//
		//--BLOCK_LEN = (READ_BL_LEN < 12) ��2^(READ_BL_LEN)--//

		//--����BLOCK_LEN,C_SIZE_MULT��CSD[49:47];READ_BL_LEN��CSD[83:80]--//
		n = (csdValue[5] & 0x0A) + ((csdValue[10] & 0x80) >> 7) 
			 + ((csdValue[9] & 0x03) << 1) + 2;

		//--����C_SIZE��C_SIZE��CSD[73:62]--//
		*capacity = (csdValue[8] >> 6) + ((uint16_t)csdValue[7] << 2) 
		            + ((uint16_t)(csdValue[6] & 3) << 10) + 1;
		*capacity = (*capacity) << (n - 9);//�õ�������	
	}

	return 0;		
}

/****************************************************************************
* Function Name  : SD_ReadData
* Description    : ��ȡ512����������.
* Input          : *buf�����ݻ�����
* Output         : ��ȡ������ֵ
* Return         : 0xFF����ʼ��ʧ��
*		         * 0����ʼ���ɹ�
****************************************************************************/

static int8_t SD_ReadData(uint8_t *buf)
{
	uint16_t i;

	//--�ȴ���ʼ����0XFE--//
	i = 0;
	while(SPI2_WriteReadData(0xFF) != 0xFE)
	{
		i++;
		if(i > 0x0FFF)
		{
			return 0xFF;
		}
	}
	
	//--��������--//
	for(i=0; i<512; i++)
	{
		*buf = SPI2_WriteReadData(0xFF);
		buf++;	
	}

	//--���������ٶ���λCRCЧ�飬�������ǿ��Բ���Ҫ����--//
	SPI2_WriteReadData(0xFF);
	SPI2_WriteReadData(0xFF);
	
	return 0;					  	
}
/****************************************************************************
* Function Name  : SD_ReadDisk
* Description    : ��ȡ����������.
* Input          : *buf�����ݻ�����
*                * sector����ȡ����λ��
*                * num����ȡ��������
* Output         : ��ȡ����������ֵ
* Return         : 0xFF����ʼ��ʧ��
*		         * 0����ʼ���ɹ�
****************************************************************************/

int8_t SD_ReadDisk(uint8_t *buf, uint32_t sector, uint8_t num)
{
	uint16_t i;

	if(SD_TYPE != SD_TYPE_V2HC)
	{
		sector <<= 9; //ת��λ�ֽڵ�ַ
	}
	SPI2_SetSpeed(SPI_BaudRatePrescaler_2);
	if(num == 1)
	{
		//--���Ͷ�ȡ����--//
		i = 0;
		while(SD_WriteCmd(SD_CMD17, sector, 0x01) != 0); 
		{
			i++;
			if(i > 100)
			{
				return 0xFF;  //�����޷�Ӧ��������������ʧ��
			}	
		}

		//-- �������� --//
		if(SD_ReadData(buf) != 0)
		{
			return 0xFF;
		}		
	}
	else
	{
		//--����������ȡ����--//
		i = 0;
		while(SD_WriteCmd(SD_CMD18, sector, 0x01) != 0); 
		{
			i++;
			if(i > 100)
			{
				return 0xFF;  //�����޷�Ӧ��������������ʧ��
			}	
		}
		
		//--��������--//
		while(num--)
		{
			//-- �������� --//
			if(SD_ReadData(buf) != 0)
			{
				return 0xFF;
			}
			buf += 512;			
		}
		SD_WriteCmd(SD_CMD12, 0, 0x01); //����ֹͣ�ź�
	}
	
	//--ȡ��Ƭѡ--//
	SD_CS_SET;
	SPI2_WriteReadData(0xFF);	

	return 0;
}

/****************************************************************************
* Function Name  : SD_WriteData
* Description    : д512����������.
* Input          : *buf�����ݻ�����
*                * cmd������
* Output         : None
* Return         : 0xFF����ʼ��ʧ��
*		         * 0����ʼ���ɹ�
****************************************************************************/

static int8_t SD_WriteData(uint8_t *buf, uint8_t cmd)
{
	uint16_t i;

	//-- �������ɸ�ʱ�ӣ��ȴ�SD��׼���� --//
	i = 0;
	while(SPI2_WriteReadData(0xFF) != 0xFF)
	{
		if(i > 0x0FFF)
		{
			return 0xFF;	
		}	
	}

	//--��������--//
	SPI2_WriteReadData(cmd);

	//--��ʼд������--//
	for(i = 0; i<512; i++)
	{
		SPI2_WriteReadData(*buf);
		buf++;
	}

	//--������λCRCЧ���룬��㷢--//
	SPI2_WriteReadData(0xFF);
	SPI2_WriteReadData(0xFF);

	//--��ȡ����ֵ--//
	i = SPI2_WriteReadData(0xFF);

	if((i & 0x1F) != 0x05)	  //�ж��Ƿ�д�ɹ�
	{
	 	SD_CS_SET;
		SPI2_WriteReadData(0xFF);
		return 0xFF;
	}

	return 0;
}
/****************************************************************************
* Function Name  : SD_WriteDisk
* Description    : д�������.
* Input          : *buf�����ݻ�����
*                * sector��Ҫд��������ֵ
*                * Ҫд��������
* Output         : None
* Return         : 0xFF����ʼ��ʧ��
*		         * 0����ʼ���ɹ�
****************************************************************************/

int8_t SD_WriteDisk(uint8_t *buf, uint32_t sector, uint8_t num)
{
	uint8_t i;

	if(SD_TYPE != SD_TYPE_V2HC)
	{
		sector <<= 9; //ת��λ�ֽڵ�ַ
	}
	SPI2_SetSpeed(SPI_BaudRatePrescaler_2);
	//--- ֻдһ������ ---//
	if(num == 1)
	{
		//--����д����--//
		i = 0;
		while(SD_WriteCmd(SD_CMD24, sector, 0x01) != 0); 
		{
			i++;
			if(i > 100)
			{
				return 0xFF;  //�����޷�Ӧ��������������ʧ��
			}	
		}
	
		if(SD_WriteData(buf, 0xFE) != 0)
		{
			return 0xFF;	
		}
	}

	//--- д������� ---//
	else
	{
		if(SD_TYPE == SD_TYPE_MMC)		  //�����MMC��
		{
			SD_WriteCmd(SD_CMD55, 0, 0X01);	
			SD_WriteCmd(SD_CMD23, num, 0X01); //д��ǰ�Ȳ���num���������������	
		}
		//--��������д����--//
		i = 0;
		while(SD_WriteCmd(SD_CMD25, sector, 0x01) != 0); 
		{
			i++;
			if(i > 100)
			{
				return 0xFF;  //�����޷�Ӧ��������������ʧ��
			}	
		}

		//--- ��ʼд���� ---//
		while(num--)
		{
			if(SD_WriteData(buf, 0xFC) != 0)
			{
				return 0xFF;	
			}
			buf += 512;	
		}

		//-- �������ɸ�ʱ�ӣ��ȴ�SD��׼���� --//
		i = 0;
		while(SPI2_WriteReadData(0xFF) != 0xFF)
		{
			if(i > 0x0FFF)
			{
				return 0xFF;	
			}	
		}
	
		//--���ͽ�������--//
		SPI2_WriteReadData(0xFD);

	}

	SD_CS_SET;
	SPI2_WriteReadData(0xFF);

	return 0;
}
