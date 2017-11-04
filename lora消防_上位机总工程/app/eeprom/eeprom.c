#include "eeprom.h"

/* ����ȫ�ֱ��� */
static int8_t AT24C02_WriteData(uint8_t *writeBuff, uint8_t writeAddr, uint8_t writeNumByte);
static void AT24C02_WaitEepromStandbyState(void);
      
/* ʹ��Ӳ��I2C */
#ifdef USE_I2C_HARDWARE

/****************************************************************************
* Function Name  : AT24C02_Init
* Description    : ��ʼ��GPIO��Ӳ��I2C
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void AT24C02_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef  I2C_InitStructure;

	/* ��ʱ��ʹ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	/* ����GPIO�Ĳ��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;

	GPIO_Init(GPIOB, &GPIO_InitStructure);

/***************************************************************************/
/********************* Ӳ��I2C���ò���ʼ�� *********************************/
/***************************************************************************/
	/* ��ʱ��ʹ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

	/* ����I2C�Ĳ��� */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;        //����ΪI2Cģʽ
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;//ʱ��ռ�ձ�
	I2C_InitStructure.I2C_OwnAddress1 = 0x10;		  //�����Լ��ĵ�ַ������ȡ
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	/* 7λӦ���ַ */
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 200000;        //������400KHZ

	I2C_Cmd(I2C2, ENABLE);	
	I2C_Init(I2C2, &I2C_InitStructure);
}

/****************************************************************************
* Function Name  : AT24C02_WriteData
* Description    : д��24C02������.ע�⣺��ַ�Զ���1ֻ����ͬһ������
* Input          : writeBuff��д�����ݵı����ַ
*                * writeAddr��д��ĵ�ַ 
*                * writeNumByte��д�����ݳ��ȣ�24C02��writeNumByte <= 8��
* Output         : None
* Return         : 0���ɹ���0xFF��ʧ�ܡ�
****************************************************************************/

static int8_t AT24C02_WriteData(uint8_t *writeBuff, uint8_t writeAddr, uint8_t writeNumByte)
{
	uint16_t i = 0;

	/* ���I2C�����Ƿ�æµ */
	while (I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY))
	{
		i++;
		if(i == 100000)
		{
			return 0xFF;
		}
	}

	/* ������ʼ�ź� */
	I2C_GenerateSTART(I2C2, ENABLE);

	i = 0;
	/* ���EV5�����Ƿ�����Ϊ����ģʽ */
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
	{
		i++;
		if(i == 100000)
		{
			return 0xFF;
		}
	}

	/* ����д������ַ */
	I2C_Send7bitAddress(I2C2, EEPROM_ADDR, I2C_Direction_Transmitter);//����ģʽ

	i = 0;
	/* ���EV6�������*/
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		i++;
		if(i == 100000)
		{
			return 0xFF;
		}
	}

	/* ����д��ַ */
	I2C_SendData(I2C2, writeAddr);

	i = 0;
	/* ���EV8 */
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		i++;
		if(i == 100000)
		{
			return 0xFF;
		}
	}

	while (writeNumByte--)
	{
		/* �������� */
		I2C_SendData(I2C2, *writeBuff);
		
		i = 0;
		/* ���EV8 */
		while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			i++;
			if(i == 100000)
			{
				return 0xFF;
			}
		}

		writeBuff++;
	}

	I2C_GenerateSTOP(I2C2, ENABLE);

	return 0;
}

/****************************************************************************
* Function Name  : AT24C02_WaitEepromStandbyState
* Description    : �ȴ�24C02д��ɹ�.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

static  void AT24C02_WaitEepromStandbyState(void)      
{
	uint16_t i = 0;
	__IO uint16_t SR1_Tmp = 0;
	
	do
	{
		/* Send START condition */ 
		I2C_GenerateSTART(I2C2, ENABLE);
		/*  Read I2C2 SR1 register */ 
		SR1_Tmp = I2C_ReadRegister(I2C2, I2C_Register_SR1);
		/* Send EEPROM address for write */ 
		I2C_Send7bitAddress(I2C2, EEPROM_ADDR, I2C_Direction_Transmitter);

		i++;
		if(i == 100000)
		{
			return ;
		}
	}
	while(!(I2C_ReadRegister(I2C2, I2C_Register_SR1) & 0x0002));
	
	/* Clear AF flag */ 
	I2C_ClearFlag(I2C2, I2C_FLAG_AF);
	
	/* STOP condition */ 
	I2C_GenerateSTOP(I2C2, ENABLE);  
}

/****************************************************************************
* Function Name  : AT24C02_WriteBuff
* Description    : д��24C02������
* Input          : writeBuff��д�����ݵı����ַ
*                * writeAddr��д��ĵ�ַ 
*                * writeNumByte��д�����ݳ���
* Output         : None
* Return         : 0���ɹ���0xFF��ʧ�ܡ�
****************************************************************************/

int8_t AT24C02_WriteBuff(uint8_t *writeBuff, uint8_t writeAddr, uint8_t writeNumByte)
{
	uint8_t addr = 0, buffNum = 0, byteNum = 0;

	addr = writeAddr % EEPROM_PAGE_SIZE;    //24c02һ��8λһ������		

	if(addr ==0)              //��д���ַ���õ���һ����������ʼ��ַ
	{
		buffNum = writeNumByte / EEPROM_PAGE_SIZE;	    //���������  
		byteNum = writeNumByte % EEPROM_PAGE_SIZE;		//����������

        /* ��д������������ */
		while(buffNum--)  //��д����������
		{
			if(AT24C02_WriteData(writeBuff, writeAddr, EEPROM_PAGE_SIZE))
			{
				return 0xFF;
			}
			AT24C02_WaitEepromStandbyState();
			
			writeBuff += EEPROM_PAGE_SIZE;
			writeAddr += EEPROM_PAGE_SIZE; 		
		}
		
        /* д��������������ʼд���������������ֽ� */ 
		if(byteNum != 0)  
		{
			if(AT24C02_WriteData(writeBuff, writeAddr, byteNum))
			{
				return 0xFF;
			}
			AT24C02_WaitEepromStandbyState();
		}
	
	}
	else  //else of if(addr ==0) 
	{
		addr = EEPROM_PAGE_SIZE - addr;            //����ж������ݾ͵���������ַ
        if(writeNumByte > addr)
        {
            writeNumByte = writeNumByte - addr;    //���д�����ݴ�������ʣ���ַ
            buffNum = writeNumByte / EEPROM_PAGE_SIZE; //���������  
		    byteNum = writeNumByte % EEPROM_PAGE_SIZE; //����������	
		}
        
        /* ��д��ǰ�治����������ַ������ */		
		if(AT24C02_WriteData(writeBuff, writeAddr, addr))
		{
			return 0xFF;
		}
		AT24C02_WaitEepromStandbyState();
		writeAddr += addr;     //д���ַ���
        writeBuff += addr;     //���ݵ�ַ����

        /* ��д���������� */
		while(buffNum--)  
		{
			if(AT24C02_WriteData(writeBuff, writeAddr, EEPROM_PAGE_SIZE))
			{
				return 0xFF;
			}
			AT24C02_WaitEepromStandbyState();
			
			writeBuff += EEPROM_PAGE_SIZE;
			writeAddr += EEPROM_PAGE_SIZE;	
		}
		
        /* д��������������ʼд���������������ֽ� */
		if(byteNum != 0)  
		{
			if(AT24C02_WriteData(writeBuff, writeAddr, byteNum))
			{
				return 0xFF;
			}
			AT24C02_WaitEepromStandbyState();
		}		
	}

    AT24C02_WaitEepromStandbyState();

	return 0;
}

/****************************************************************************
* Function Name  : AT24C02_ReadBuff
* Description    : ��ȡ24C02������.
* Input          : readBuff����ȡ�����ݵı����ַ
*                * readAddr����ȡ�ĵ�ַ 
*                * readNumByte����ȡ�����ݳ���
* Output         : None
* Return         : 0���ɹ���0xFF��ʧ�ܡ�
****************************************************************************/

int8_t AT24C02_ReadBuff(uint8_t *readBuff, uint8_t readAddr, uint8_t readNumByte)
{
	uint16_t i = 0;

	/* ���I2C�����Ƿ�æµ */
	while (I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY))
	{
			i++;
			if(i == 100000)
			{
				return 0xFF;
			}
	}

	/* ������ʼ�ź� */
	I2C_GenerateSTART(I2C2, ENABLE);

	i = 0;
	/* ���EV5�����Ƿ�����Ϊ����ģʽ */
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
	{
		i++;
		if(i == 100000)
		{
			return 0xFF;
		}
	}
	
	/* ����д������ַ */
	I2C_Send7bitAddress(I2C2, EEPROM_ADDR, I2C_Direction_Transmitter);//����ģʽ

	i = 0;
	/* ���EV6�������*/
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		i++;
		if(i == 100000)
		{
			return 0xFF;
		}
	}

	/* Clear EV6 by setting again the PE bit */
  	I2C_Cmd(I2C2, ENABLE);

	/* ���Ͷ�ȡ��ַ */
	I2C_SendData(I2C2, readAddr);

	i = 0;
	/* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		i++;
		if(i == 100000)
		{
			return 0xFF;
		}
	}

	/* ������ʼ�ź� */
	I2C_GenerateSTART(I2C2, ENABLE);

	i = 0;
	/* ���EV5��*/
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
	{
		i++;
		if(i == 100000)
		{
			return 0xFF;
		}
	}

	/* ���Ͷ�������ַ */
    I2C_Send7bitAddress(I2C2, EEPROM_ADDR, I2C_Direction_Receiver);

	i = 0;
    /* Test on EV6 and clear it */
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
		i++;
		if(i == 100000)
		{
			return 0xFF;
		}
	}
 
	while (readNumByte)
	{
		if(readNumByte==1)
		{
			/* ʧ��Ӧ�� */
			I2C_AcknowledgeConfig(I2C2, DISABLE);
			/* ����ֹͣ���� */
			I2C_GenerateSTOP(I2C2, ENABLE);
		}

		/* �������� */
		while(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED))
		{
			/* ��ȡ���յ������� */
			*readBuff = I2C_ReceiveData(I2C2);

			readBuff++;
			readNumByte--;
		}
	}/* end of while(readNumByte); */

	I2C_AcknowledgeConfig(I2C2, ENABLE);
	AT24C02_WaitEepromStandbyState();

	return 0;
}

#else

/****************************************************************************
* Function Name  : AT24C02_WriteData
* Description    : д��24C02������.ע�⣺��ַ�Զ���1ֻ����ͬһ������
* Input          : writeBuff��д�����ݵı����ַ
*                * writeAddr��д��ĵ�ַ 
*                * writeNumByte��д�����ݳ��ȣ�24C02��writeNumByte <= 8��
* Output         : None
* Return         : 0���ɹ���0xFF��ʧ�ܡ�
****************************************************************************/

static int8_t AT24C02_WriteData(uint8_t *writeBuff, uint8_t writeAddr, uint8_t writeNumByte)
{
	IIC_Start();
	IIC_SendData(EEPROM_ADDR); //24C02д��ַ
	if(IIC_WaitAck())
    {
        return 0xFF;
    }
	IIC_SendData(writeAddr);   //����д���ַ
	if(IIC_WaitAck())
    {
        return 0xFF;
    }

    while(writeNumByte--)
    {
    	IIC_SendData(*writeBuff);
    	if(IIC_WaitAck())
        {
            return 0xFF;
        }
        writeBuff++;
    }
	IIC_Stop();

	return 0;
}

/****************************************************************************
* Function Name  : AT24C02_WaitEepromStandbyState
* Description    : �ȴ�24C02д��ɹ�.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

static  void AT24C02_WaitEepromStandbyState(void)      
{
    uint32_t i = 0;
    do
    {
        IIC_Start();
	    IIC_SendData(EEPROM_ADDR); //24C02д��ַ
        i++;
        if(i > 0x00FFFF)
        {
            return;
        }
	}
    while(IIC_WaitAck());   
}
/****************************************************************************
* Function Name  : AT24C02_WriteBuff
* Description    : д��24C02������
* Input          : writeBuff��д�����ݵı����ַ
*                * writeAddr��д��ĵ�ַ 
*                * writeNumByte��д�����ݳ���
* Output         : None
* Return         : 0���ɹ���0xFF��ʧ�ܡ�
****************************************************************************/

int8_t AT24C02_WriteBuff(uint8_t *writeBuff, uint8_t writeAddr, uint8_t writeNumByte)
{
	uint8_t addr = 0, buffNum = 0, byteNum = 0;

	addr = writeAddr % EEPROM_PAGE_SIZE;    //24c02һ��8λһ������		

	if(addr ==0)              //��д���ַ���õ���һ����������ʼ��ַ
	{
		buffNum = writeNumByte / EEPROM_PAGE_SIZE;	    //���������  
		byteNum = writeNumByte % EEPROM_PAGE_SIZE;		//����������

        /* ��д������������ */
		while(buffNum--)  //��д����������
		{
			if(AT24C02_WriteData(writeBuff, writeAddr, EEPROM_PAGE_SIZE))
			{
				return 0xFF;
			}
			AT24C02_WaitEepromStandbyState();
			
			writeBuff += EEPROM_PAGE_SIZE;
			writeAddr += EEPROM_PAGE_SIZE; 		
		}
		
        /* д��������������ʼд���������������ֽ� */ 
		if(byteNum != 0)  
		{
			if(AT24C02_WriteData(writeBuff, writeAddr, byteNum))
			{
				return 0xFF;
			}
			AT24C02_WaitEepromStandbyState();
		}
	
	}
	else  //else of if(addr ==0) 
	{
		addr = EEPROM_PAGE_SIZE - addr;            //����ж������ݾ͵���������ַ
        if(writeNumByte > addr)
        {
            writeNumByte = writeNumByte - addr;    //���д�����ݴ�������ʣ���ַ
            buffNum = writeNumByte / EEPROM_PAGE_SIZE; //���������  
		    byteNum = writeNumByte % EEPROM_PAGE_SIZE; //����������	
		}
        
        /* ��д��ǰ�治����������ַ������ */		
		if(AT24C02_WriteData(writeBuff, writeAddr, addr))
		{
			return 0xFF;
		}
		AT24C02_WaitEepromStandbyState();
		writeAddr += addr;     //д���ַ���
        writeBuff += addr;     //���ݵ�ַ����

        /* ��д���������� */
		while(buffNum--)  
		{
			if(AT24C02_WriteData(writeBuff, writeAddr, EEPROM_PAGE_SIZE))
			{
				return 0xFF;
			}
			AT24C02_WaitEepromStandbyState();
			
			writeBuff += EEPROM_PAGE_SIZE;
			writeAddr += EEPROM_PAGE_SIZE;	
		}
		
        /* д��������������ʼд���������������ֽ� */
		if(byteNum != 0)  
		{
			if(AT24C02_WriteData(writeBuff, writeAddr, byteNum))
			{
				return 0xFF;
			}
			AT24C02_WaitEepromStandbyState();
		}		
	}

    AT24C02_WaitEepromStandbyState();

	return 0;
}

/****************************************************************************
* Function Name  : AT24C02_ReadBuff
* Description    : ��ȡ24C02������.
* Input          : readBuff����ȡ�����ݵı����ַ
*                * readAddr����ȡ�ĵ�ַ 
*                * readNumByte����ȡ�����ݳ���
* Output         : None
* Return         : 0���ɹ���0xFF��ʧ�ܡ�
****************************************************************************/

int8_t AT24C02_ReadBuff(uint8_t *readBuff, uint8_t readAddr, uint8_t readNumByte)
{

	IIC_Start();
	IIC_SendData(EEPROM_ADDR);   //д��ַ
	if(IIC_WaitAck())
    {
        return 0xFF;
    }
	IIC_SendData(readAddr);
	if(IIC_WaitAck())
    {
        return 0xFF;
    }

	IIC_Start();
	IIC_SendData(EEPROM_ADDR + 1);
	if(IIC_WaitAck())
    {
        return 0xFF;
    }
	while(readNumByte)
	{   
        readNumByte--;
		if(readNumByte != 0)       //δ������ɾͷ���Ӧ��
		{
			*readBuff = IIC_ReceiveData(1);
		}
        else                      //������ɾͷ��ͷ�Ӧ��
        {
            *readBuff = IIC_ReceiveData(0);
        }
        readBuff++;
	}

	IIC_Stop();
	return 0;
}

#endif  //end of #ifndef USE_I2C_HARDWARE

