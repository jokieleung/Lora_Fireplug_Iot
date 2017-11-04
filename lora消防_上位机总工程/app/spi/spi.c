#include "spi.h"

/****************************************************************************
* Function Name  : SPI1_Config
* Description    : ��ʼ��SPI2
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void SPI1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

    /* SPI��IO�ں�SPI�����ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    /* SPI��IO������ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); //PA5.6.7����
/***************************************************************************/
/************************* ����SPI�Ĳ��� ***********************************/
/***************************************************************************/
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//ѡ��ȫ˫��SPIģʽ
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;     //����ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8λSPI
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;       //ʱ�����ոߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;      //�ڵڶ���ʱ�Ӳɼ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		  //Nssʹ���������
	/* ѡ������Ԥ��ƵΪ256 */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//�����λ��ʼ����
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	
	SPI_Cmd(SPI1, ENABLE);
	SPI_Init(SPI1, &SPI_InitStructure);
}      

/****************************************************************************
* Function Name  : SPI2_Config
* Description    : ��ʼ��SPI2
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void SPI2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

    /* SPI��IO�ں�SPI�����ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    /* SPI��IO������ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure);

/***************************************************************************/
/************************* ����SPI�Ĳ��� ***********************************/
/***************************************************************************/
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//ѡ��ȫ˫��SPIģʽ
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;     //����ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8λSPI
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;       //ʱ�����ոߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;      //�ڵڶ���ʱ�Ӳɼ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		  //Nssʹ���������
	/* ѡ������Ԥ��ƵΪ256 */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//�����λ��ʼ����
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	
	SPI_Cmd(SPI2, ENABLE);
	SPI_Init(SPI2, &SPI_InitStructure);
}      

/****************************************************************************
* Function Name  : SPI1_SetSpeed
* Description    : ����SPI1�Ĵ����ٶȡ�
* Input          : �ٶȲ����ʷ�Ƶ
* Output         : None
* Return         : None
****************************************************************************/

void SPI1_SetSpeed(uint8_t speed)
{
	SPI1->CR1 &= 0xFFC7; 
	SPI1->CR1 |= speed;
	SPI_Cmd(SPI1, ENABLE);
	
}

/****************************************************************************
* Function Name  : SPI2_SetSpeed
* Description    : ����SPI2�ķ�Ƶ�����Ըı�SPI2���ٶ�.
* Input          : Speed����Ƶ��
* Output         : None
* Return         : None
****************************************************************************/

void SPI2_SetSpeed(uint8_t Speed)
{
	
	SPI2->CR1 &= 0xFFC7; 
	SPI2->CR1 |= Speed;
	SPI_Cmd(SPI2,ENABLE);
	 		
}

/****************************************************************************
* Function Name  : SPI1_WriteReadData
* Description    : ʹ��SPI1д��һ���ֽ�����ͬʱ��ȡһ���ֽ����ݡ�
* Input          : dat��Ҫд��8λ����
* Output         : None
* Return         : ��ȡ����8λ����
****************************************************************************/

uint8_t SPI1_WriteReadData(uint8_t dat)
{
	uint16_t i = 0;

    /* �����ͻ������� */	
 	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	{
		i++;
		if(i > 10000)
		{
			return 0xFF;
		}
	}
	
    /* �������� */
   	SPI_I2S_SendData(SPI1, dat);
	
	/* �ȴ����ջ�����Ϊ�ǿ� */
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	
 	/* ����ȡ������ֵ���� */
 	return SPI_I2S_ReceiveData(SPI1);		
}

/****************************************************************************
* Function Name  : SPI2_WriteReadData
* Description    : ʹ��SPI2д��һ���ֽ�����ͬʱ��ȡһ���ֽ����ݡ�
* Input          : dat��д�������
* Output         : None
* Return         : ��ȡ��������
*                * ��ȡʧ�ܷ���0xFF
****************************************************************************/

uint8_t SPI2_WriteReadData(uint8_t dat)
{
	uint16_t i = 0;

    /* �����ͻ������� */	
 	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
	{
		i++;
		if(i > 10000)
		{
			return 0xFF;
		}
	}
	
    /* �������� */
   	SPI_I2S_SendData(SPI2, dat);
	
	/* �ȴ����ջ�����Ϊ�ǿ� */
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	
 	/* ����ȡ������ֵ���� */
 	return SPI_I2S_ReceiveData(SPI2);		
}



















