#include "lcd_driver.h"

/****************************************************************************
*��������TFT_GPIO_Config
*��  �룺��
*��  ������
*��  �ܣ���ʼ��TFT��IO�ڡ�
****************************************************************************/	  

void TFT_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ��ʱ��ʹ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE  
	                      | RCC_APB2Periph_GPIOG, ENABLE);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	/* FSMC_A10(G12) ��RS��G0��*/
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 
	                              | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_8 
								  | GPIO_Pin_9 | GPIO_Pin_10 |GPIO_Pin_11
								  | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14
								  | GPIO_Pin_15 );

	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9
	                               | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12
								   | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/****************************************************************************
* Function Name  : TFT_FSMC_Config
* Description    : ��ʼ��FSMC
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void TFT_FSMC_Config(void)
{
	/* ��ʼ������ */
	FSMC_NORSRAMInitTypeDef        FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  FSMC_NORSRAMTiming;

	/* ���ö�дʱ�򣬸�FSMC_NORSRAMInitStructure���� */
	/* ��ַ����ʱ�䣬3��HCLK���� */
	FSMC_NORSRAMTiming.FSMC_AddressSetupTime = 0x02;

	/* ��ַ����ʱ�䣬1��HCLK���� */
	FSMC_NORSRAMTiming.FSMC_AddressHoldTime = 0x00;

	/* ���ݽ���ʱ�䣬6��HCLK���� */
	FSMC_NORSRAMTiming.FSMC_DataSetupTime = 0x05;

	/* ���ݱ���ʱ�䣬1��HCLK���� */
	FSMC_NORSRAMTiming.FSMC_DataLatency = 0x00;

	/* ���߻ָ�ʱ������ */
	FSMC_NORSRAMTiming.FSMC_BusTurnAroundDuration = 0x00;
	
	/* ʱ�ӷ�Ƶ���� */
	FSMC_NORSRAMTiming.FSMC_CLKDivision = 0x01;

	/* ����ģʽ������ڵ�ַ/���ݲ�����ʱ��ABCDģʽ�����𲻴� */
	FSMC_NORSRAMTiming.FSMC_AccessMode = FSMC_AccessMode_B;

	/*����FSMC_NORSRAMInitStructure������*/
	/* FSMC���ĸ��洢�飨bank��������ʹ�õ�һ����bank1�� */
	/* ͬʱ����ʹ�õ���bank����ĵ� 4��RAM�� */
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;

	/* ��������ʹ��SRAMģʽ */
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;

	/* ʹ�õ����ݿ��Ϊ16λ */
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;

	/* ����дʹ�ܴ� */
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;

	/* ѡ����չģʽʹ�ܣ������ö���д�ò�ͬ��ʱ�� */
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable;
	
	/* ���õ�ַ�����ݸ���ʹ�ܲ��� */
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	
	/* ���ö�дʱ�� */
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMTiming;
	
	/* ����дʱ�� */
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_NORSRAMTiming;

	/* ��FSMC��ʱ�� */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	 
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 

	/*!< Enable FSMC Bank1_SRAM Bank */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);	
}

/****************************************************************************
* Function Name  : TFT_WriteCmd
* Description    : LCDд������
* Input          : cmd��д���16λ����
* Output         : None
* Return         : None
****************************************************************************/

void TFT_WriteCmd(uint16_t cmd)
{
	TFT->TFT_CMD = cmd<<8 ;
}

/****************************************************************************
* Function Name  : TFT_WriteData
* Description    : LCDд������
* Input          : dat��д���16λ����
* Output         : None
* Return         : None
****************************************************************************/

void TFT_WriteData(u16 dat)
{
	TFT->TFT_DATA = dat<<8;
}

void TFT_WriteData_Color(u16 color)
{
	TFT->TFT_DATA = color&0xff00;
	TFT->TFT_DATA = color<<8;
}

/****************************************************************************
* Function Name  : TFT_Init
* Description    : ��ʼ��LCD��
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void TFT_Init(void)
{
	uint16_t i;

	TFT_GPIO_Config();
	TFT_FSMC_Config();

	//************* Start Initial Sequence **********//	
	TFT_WriteCmd(0xCB);  
	TFT_WriteData(0x39); 
	TFT_WriteData(0x2C); 
	TFT_WriteData(0x00); 
	TFT_WriteData(0x34);  
	TFT_WriteData(0x02);   

	TFT_WriteCmd(0xCF);  
	TFT_WriteData (0x00); 
	TFT_WriteData (0xa2); //c1
	TFT_WriteData (0Xf0);  //30
	 
	TFT_WriteCmd(0xE8);  
	TFT_WriteData (0x84); 	 //85
	TFT_WriteData (0x11); 	 //00
	TFT_WriteData (0x7a); 	 //79
	
	
	TFT_WriteCmd(0xEA);  
	TFT_WriteData (0x66);  //00
	TFT_WriteData (0x00); 
	
	TFT_WriteCmd(0xED);  
	TFT_WriteData (0x55); 	//64
	TFT_WriteData (0x01); 	//03
	TFT_WriteData (0X23); 	//12
	TFT_WriteData (0X01); 	//81
	 
	TFT_WriteCmd(0xF7);  
	TFT_WriteData (0x20); 	//20
	
	
	TFT_WriteCmd(0xC0);    //Power control 
	TFT_WriteData (0x1c);   //VRH[5:0] 	//1c
	 
	TFT_WriteCmd(0xC1);    //Power control 
	TFT_WriteData (0x13);   //SAP[2:0];BT[3:0] 	//13
	 
	TFT_WriteCmd(0xC5);    //VCM control 
	TFT_WriteData (0x23); 
	TFT_WriteData (0x3F); 
	 
	TFT_WriteCmd(0xC7);    //VCM control2 
	TFT_WriteData(0xA5); 
	
	TFT_WriteCmd(0xB1);  
	TFT_WriteData (0x00); 
	TFT_WriteData (0x17); 
	
	TFT_WriteCmd(0x3A);  
	TFT_WriteData (0x55); 
	
	TFT_WriteCmd(0xB6);  
	TFT_WriteData (0x0A);
	TFT_WriteData (0xa2);  //a2
	TFT_WriteData (0x27);
	TFT_WriteData (0x00); 
	 
	TFT_WriteCmd(0x36);    // Memory Access Control 
	TFT_WriteData (0x08); 	  //08	BGR
	 
	TFT_WriteCmd(0xF2);    // 3Gamma Function Disable 
	TFT_WriteData (0x02); 	//00
	 
	TFT_WriteCmd(0x26);    //Gamma curve selected 
	TFT_WriteData (0x01); 
	 
	TFT_WriteCmd(0xE0);    //Set Gamma 
	TFT_WriteData (0x0F); 
	TFT_WriteData (0x14); 
	TFT_WriteData (0x13); 
	TFT_WriteData (0x0C); 
	TFT_WriteData (0x0E); 
	TFT_WriteData (0x05); 
	TFT_WriteData (0x45); 
	TFT_WriteData (0x85); 
	TFT_WriteData (0x36); 
	TFT_WriteData (0x09); 
	TFT_WriteData (0x14); 
	TFT_WriteData (0x05); 
	TFT_WriteData (0x09); 
	TFT_WriteData (0x03); 
	TFT_WriteData (0x00); 
	 
	TFT_WriteCmd(0XE1);    //Set Gamma 
	TFT_WriteData (0x00); 
	TFT_WriteData (0x24); 
	TFT_WriteData (0x26); 
	TFT_WriteData (0x03); 
	TFT_WriteData (0x0F); 
	TFT_WriteData (0x04); 
	TFT_WriteData (0x3F); 
	TFT_WriteData (0x14); 
	TFT_WriteData (0x52); 
	TFT_WriteData (0x04); 
	TFT_WriteData (0x10); 
	TFT_WriteData (0x0E); 
	TFT_WriteData (0x38); 
	TFT_WriteData (0x39); 
	TFT_WriteData (0x0F); 

	TFT_WriteCmd(0x2A );
	TFT_WriteData(0x00);
	TFT_WriteData(0x00);
	TFT_WriteData(0x00);
	TFT_WriteData(0xEF);

	TFT_WriteCmd(0x2B );
	TFT_WriteData(0x00);
	TFT_WriteData(0x00);
	TFT_WriteData(0x01);
	TFT_WriteData(0x3F);
	 
	TFT_WriteCmd(0x11);    //Exit Sleep 
	for(i=50000; i>0; i--);	 //��ʱ
	for(i=50000; i>0; i--);
	for(i=50000; i>0; i--);
	for(i=50000; i>0; i--);
	for(i=50000; i>0; i--);
	for(i=50000; i>0; i--);
	TFT_WriteCmd(0x29);    //Display on
}

/****************************************************************************
* Function Name  : TFT_SetWindow
* Description    : ����Ҫ�����Ĵ��ڷ�Χ
* Input          : xStart��������ʼX����
*                * yStart��������ʼY����
*                * xEnd�����ڽ���X����
*                * yEnd�����ڽ���Y����
* Output         : None
* Return         : None
****************************************************************************/

void TFT_SetWindow(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd)
{
 	TFT_WriteCmd(0x2A);

    TFT_WriteData(xStart/256);
    TFT_WriteData(xStart%256);
    TFT_WriteData(xEnd/256);
    TFT_WriteData(xEnd%256);

    TFT_WriteCmd(0x2b);
    TFT_WriteData(yStart/256);
    TFT_WriteData(yStart%256);
    TFT_WriteData(yEnd/256);
    TFT_WriteData(yEnd%256);
    TFT_WriteCmd(0x2c); 
}

/****************************************************************************
* Function Name  : TFT_ClearScreen
* Description    : ��LCD��������Ӧ����ɫ
* Input          : color��������ɫ
* Output         : None
* Return         : None
****************************************************************************/
	  
void TFT_ClearScreen(uint16_t color)
{
 	uint16_t i, j ;

	TFT_SetWindow(0, 0, TFT_XMAX, TFT_YMAX);	 //��������
  	for(i=0; i<TFT_XMAX+1; i++)
	{
		for (j=0; j<TFT_YMAX+1; j++)
		{
			TFT_WriteData_Color(color);
		}
	}
}
