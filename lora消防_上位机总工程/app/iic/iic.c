#include "IIC.h"
#include "system.h"

#define IIC_DelayUs(x) {SYSTICK_Delay1us(x);}
static void IIC_SendAck(void);
static void IIC_NoAck(void);


/****************************************************************************
* Function Name  : IIC_Config
* Description    : ��ʼ��GPIO.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void IIC_Config(void)
{
 	RCC->APB2ENR |= 1 << 3;    //��ʹ������IO PORTBʱ��
	 							 
	GPIOB->CRH &= 0XFFFF00FF;  //PB10/11 �������
	GPIOB->CRH |= 0X00003300;
		   
	GPIOB->ODR |= 3 << 10;     //PB10,11 �����
}

/****************************************************************************
* Function Name  : IIC_Start
* Description    : IIC������ʼ�ź�.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void IIC_Start(void)
{
	IIC_SDA_OUT();
	
	IIC_SDA_SET;
	IIC_SCL_SET;
	IIC_DelayUs(5);	

	IIC_SDA_CLR;	//START:when CLK is high,DATA change form high to low
	IIC_DelayUs(5);	//����ʱ��>4us
	
	IIC_SCL_CLR;    //ǯסIIC���ߣ�׼�����ͻ��������					
}

/****************************************************************************
* Function Name  : IIC_Stop
* Description    : IIC���ͽ����ź�.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void IIC_Stop(void)
{
	IIC_SDA_OUT();

	IIC_SCL_CLR;
	IIC_SDA_CLR;	//SCL�ڵ͵�ƽ�ڼ䣬SDA�仯��Ч

	IIC_SCL_SET;
	IIC_DelayUs(5);	//����ʱ��>4us

	IIC_SDA_SET;    //STOP:when CLK is high DATA change form low to high
	IIC_DelayUs(5);	//����ʱ��>4.7us
}

/****************************************************************************
* Function Name  : IIC_SendData
* Description    : IIC����һ��8λ����
* Input          : dat�����͵�����
* Output         : None
* Return         : None
****************************************************************************/

void IIC_SendData(uint8_t dat)
{
	uint8_t i;
	
	IIC_SDA_OUT();
	IIC_SCL_CLR;

	for(i=0; i<8; i++)//Ҫ����8λ�������λ��ʼ
	{
		if((dat & 0x80) == 0x80)
		{
			IIC_SDA_SET;
		}
		else
		{
			IIC_SDA_CLR;
		}
		dat <<= 1;
		IIC_DelayUs(2);

		IIC_SCL_SET;
		IIC_DelayUs(5);  //����ʱ��>4.7us
		IIC_SCL_CLR;
		IIC_DelayUs(2);
	}	
}

/****************************************************************************
* Function Name  : IIC_ReceiveData
* Description    : IIC����һ��8λ����.
* Input          : ack��1������Ӧ��0��������Ӧ��
* Output         : None
* Return         : ��ȡ��������
****************************************************************************/

uint8_t IIC_ReceiveData(uint8_t ack)
{
	uint8_t i, readValue;

	IIC_SDA_OUT();
	IIC_SDA_SET;	
	IIC_SDA_IN();
	IIC_SCL_CLR;

	for(i=0; i<8; i++)	 //����8���ֽ�,�Ӹߵ���
	{
		IIC_SCL_SET;
		IIC_DelayUs(2);
		readValue <<= 1;
		if(IIC_SDA != 0)
		{
			readValue |= 0x01;
		}		
		IIC_DelayUs(1);
		
		IIC_SCL_CLR;
		IIC_DelayUs(5);			
	}
	
	if(ack)		 //�Ƿ�Ӧ��
	{
		IIC_SendAck();
	}
	else
	{
		IIC_NoAck();
	}

	return readValue;
}

/****************************************************************************
* Function Name  : IIC_WaitAck
* Description    : �ȴ�Ӧ��.
* Input          : None
* Output         : None
* Return         : 1��Ӧ��ɹ���0��Ӧ��ʧ��
****************************************************************************/

int8_t IIC_WaitAck(void)
{
	uint32_t i;

	IIC_SDA_IN();
	IIC_SDA_SET;
	IIC_DelayUs(1);
	IIC_SCL_SET;
	

	while(IIC_SDA)//��SCL�ߵ�ƽ������£�SDA����һ���½���
	{
		 i++;
		 if(i > 0xFFFF)
		 {
		 	return 0xFF;
		 }
	}
	
	IIC_SCL_CLR;
	IIC_DelayUs(2);

	return 0;	
}

/****************************************************************************
* Function Name  : IIC_SendAck
* Description    : ����Ӧ���ź�.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

static void IIC_SendAck(void)
{	
	IIC_SCL_CLR;
	IIC_SDA_OUT();
		
	IIC_SDA_CLR;
	IIC_DelayUs(2);
	IIC_SCL_SET;
	IIC_DelayUs(2);
	IIC_SCL_CLR;		
}

/****************************************************************************
* Function Name  : IIC_NoAck
* Description    : ��Ӧ��.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

static void IIC_NoAck(void)
{	
	IIC_SCL_CLR;
	IIC_SDA_OUT();
		
	IIC_SDA_SET;
	IIC_DelayUs(2);
	IIC_SCL_SET;
	IIC_DelayUs(2);
	IIC_SCL_CLR;		
}


