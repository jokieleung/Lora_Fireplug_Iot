#include "rda5820.h"
#include "system.h"
#include "iic.h"

/****************************************************************************
* Function Name  : RDA5820_DelayMs
* Description    : RDA5820��ʱ����
* Input          : ms����ʱʱ��
* Output         : None
* Return         : None
****************************************************************************/
#define RDA5820_DelayMs(x) {SYSTICK_Delay1ms(x);}

/****************************************************************************
* Function Name  : RDA5820_ReadData
* Description    : RDA5820ʹ��IIC��ȡһ��16λ����
* Input          : addr����ȡ�Ĵ�����ַ
* Output         : None
* Return         : dat����ȡ��������
****************************************************************************/
		
static uint16_t RDA5820_ReadData(uint8_t addr)
{
	uint16_t dat;
	
	IIC_Start();					//������ʼ�ź�

	IIC_SendData(RDA5820_ADDR);	    //����д����
	IIC_WaitAck();					//�ȴ�Ӧ��
	IIC_SendData(addr);				//���Ͷ�ȡ��ַ
	IIC_WaitAck();					//�ȴ�Ӧ��

	IIC_Start();
	IIC_SendData(RDA5820_ADDR + 1);	//���Ͷ�����
	IIC_WaitAck();					//�ȴ�Ӧ��
	dat = IIC_ReceiveData(1);        //��ȡ��8λ	����Ӧ��
	dat <<= 8;
   	dat |= IIC_ReceiveData(0);		//��ȡ��8λ

	IIC_Stop();						//���ͽ����źţ�����Ӧ��

	return dat;
}

/****************************************************************************
* Function Name  : RDA5820_WriteData
* Description    : RDA5820ʹ��IICд��16λ����
* Input          : addr��д��Ĵ�����ַ
*                * dat��д������
* Output         : None
* Return         : None
****************************************************************************/

static void RDA5820_WriteData(uint8_t addr, uint16_t dat)
{
	IIC_Start();
	IIC_SendData(RDA5820_ADDR);	    //����д����
	IIC_WaitAck();					//�ȴ�Ӧ��
	IIC_SendData(addr);				//���Ͷ�ȡ��ַ
	IIC_WaitAck();					//�ȴ�Ӧ��
	
	IIC_SendData(dat >> 8);		    //���͸�8λ����
	IIC_WaitAck();					//�ȴ�Ӧ��
	IIC_SendData(dat & 0xFF);		//���͵�8λ����
	IIC_WaitAck();					//�ȴ�Ӧ��

	IIC_Stop();						//���ͽ����ź� 
}

/****************************************************************************
* Function Name  : RDA5820_Init
* Description    : ��ʼ��RDA5820
* Input          : None
* Output         : None
* Return         : 0xFF����ʼ��ʧ�ܣ�0����ʼ���ɹ�
****************************************************************************/

uint8_t RDA5820_Init(void)
{
	uint16_t id;

	IIC_Config();
	id = RDA5820_ReadData(RDA5820_R00);
	if(id == 0x5805)
	{
	 	RDA5820_WriteData(RDA5820_R02, 0x0002);	//��λ
		RDA5820_DelayMs(50);
	 	RDA5820_WriteData(RDA5820_R02, 0xC001);	//������,�ϵ�
		RDA5820_DelayMs(600);				    //�ȴ�ʱ���ȶ� 
	 	RDA5820_WriteData(RDA5820_R05, 0X884F);	//����ǿ��8,LNAN,1.8mA,VOL���
 	 	RDA5820_WriteData(0x07, 0X7800);		// 
	 	RDA5820_WriteData(0x13, 0X0008);		// 
	 	RDA5820_WriteData(0x15, 0x1420);		//VCO����  0x17A0/0x1420 
	 	RDA5820_WriteData(0x16, 0XC000);		//  
	 	RDA5820_WriteData(0x1C, 0X3126);		// 
	 	RDA5820_WriteData(0x22, 0X9C24);		//fm_true 
	 	RDA5820_WriteData(0x47, 0XF660);		//tx rds
        
        RDA5820_SetBand(0);                 //���÷��ͻ��߽���Ƶ�ʷ�Χ
    	RDA5820_SetSpace(2);                //����Ƶ�ʲ�����С
    	RDA5820_SetTxPGA(3);                //���������С
    	RDA5820_SetTxPAG(63);               //���书�ʴ�С
        return 0;
	}
    return 0xFF;
}

/****************************************************************************
* Function Name  : RDA5820_SetMode
* Description    : ����RDA5820���ͻ��߽���ģʽ
* Input          : mode������ģʽ
* Output         : None
* Return         : None
****************************************************************************/

void RDA5820_SetMode(uint8_t mode)
{
	uint16_t tmpreg;

	tmpreg = RDA5820_ReadData(RDA5820_R40);	 //��ȡ��ʼ����
	
	tmpreg &= 0xFFF0;  //SHIP_FUNC[3:0]
	tmpreg |= mode;	   //������ģʽ��������ͷ�ļ���
	
	RDA5820_WriteData(RDA5820_R40, tmpreg);	 //����
}

/****************************************************************************
* Function Name  : RDA5820_SetVol
* Description    : ���������Ĵ�С
* Input          : vol����С���ã�0x00~0x0F��
* Output         : None
* Return         : None
****************************************************************************/

void RDA5820_SetVol(uint8_t vol)
{
	uint16_t tmpreg;
	
	tmpreg = RDA5820_ReadData(RDA5820_R05);	 //��ȡ��ʼ����
	
	tmpreg &= 0xFFF0;     //VOLUME[3:0]
	tmpreg |= vol & 0x0F; //��min:0000~max:1111	
	
	RDA5820_WriteData(RDA5820_R05, tmpreg);	 //����	
}

/****************************************************************************
* Function Name  : RDA5820_GetVol
* Description    : ��ȡ��ǰ����
* Input          : None
* Output         : None
* Return         : ��ȡ��������ֵ�����16��
****************************************************************************/

uint8_t RDA5820_GetVol(void)
{
    uint16_t tmpreg;

    tmpreg = RDA5820_ReadData(RDA5820_R05);	 //��ȡ��ʼ����

    tmpreg &= 0x000F;

    return (uint8_t)tmpreg;
}

/****************************************************************************
* Function Name  : RDA5820_SetBand
* Description    : ���÷��ͻ����Ƶ�ʷ�Χ
* Input          : band������Ƶ�ʷ�Χband:0,87~108Mhz;1,76~91Mhz;2,76~108Mhz
* Output         : None
* Return         : None
****************************************************************************/

void RDA5820_SetBand(uint8_t band)
{
	uint16_t tmpreg;
	
	tmpreg = RDA5820_ReadData(RDA5820_R03);	 //��ȡ��ʼ����

	tmpreg &= 0xFFF3;     // BAND��R03��2:3λ 	
	tmpreg |= band << 2;  // band:0,87~108Mhz;1,76~91Mhz;2,76~108Mhz

	RDA5820_WriteData(RDA5820_R03, tmpreg);	 //����
}

/****************************************************************************
* Function Name  : RDA5820_SetSpace
* Description    : ����Ƶ�ʲ�����Сband:0,100Khz;1,200Khz;2,50Khz
* Input          : space������Ƶ�ʲ�����С
* Output         : None
* Return         : None
****************************************************************************/

void RDA5820_SetSpace(uint8_t space)
{
	uint16_t tmpreg;

	tmpreg = RDA5820_ReadData(RDA5820_R03);	 //��ȡ��ʼ����

	tmpreg &= 0XFFFC;	 //SPACE��BAND��[1:0]
	tmpreg |= space;	 //band:0,100Khz;1,200Khz;2,50Khz

	RDA5820_WriteData(RDA5820_R03, tmpreg);	 //����
}

/****************************************************************************
* Function Name  : RDA5820_SetTxPGA
* Description    : ���÷����ź�����
* Input          : gain�����������С����min:000~max:111
* Output         : None
* Return         : None
****************************************************************************/

void RDA5820_SetTxPGA(uint8_t gain)
{
	uint16_t tmpreg;

	tmpreg = RDA5820_ReadData(RDA5820_R42);	 //��ȡ��ʼ����
	
	tmpreg &= 0xF8FF;     //PGA[10:8]
	tmpreg |= gain << 8;  //��min:000~max:111	
	
	RDA5820_WriteData(RDA5820_R42, tmpreg);	 //����		
}

/****************************************************************************
* Function Name  : RDA5820_SetTxPAG
* Description    : ���÷��书��
* Input          : gain�����书�ʴ�С����min:00000~max:11111
* Output         : None
* Return         : None
****************************************************************************/

void RDA5820_SetTxPAG(uint8_t gain)
{	
	uint16_t tmpreg;

	tmpreg = RDA5820_ReadData(RDA5820_R42);	 //��ȡ��ʼ����
	
	tmpreg &= 0xFFC0;     //PA[5:0]
	tmpreg |= gain;       //��min:00000~max:11111	
	
	RDA5820_WriteData(RDA5820_R42, tmpreg);	 //����	
}

/****************************************************************************
* Function Name  : RDA5820_SetSeekth
* Description    : �����ź�ǿ����ʼֵ
* Input          : seekth������ֵSEEKTH[14:8](�趨�ź�ǿ��)
* Output         : None
* Return         : None
****************************************************************************/

void RDA5820_SetSeekth(uint8_t seekth)
{
    uint16_t tmpreg;

    tmpreg = RDA5820_ReadData(RDA5820_R05);
    
    tmpreg &= 0x80FF;        // SEEKTH[14:8](�趨�ź�ǿ��)
    tmpreg |= ((uint16_t)seekth & 0x80) << 8;

    RDA5820_WriteData(RDA5820_R05, tmpreg);	 //����
}

/****************************************************************************
* Function Name  : RDA5820_SetFrequency
* Description    : ����RDA5820Ƶ��
* Input          : freq��Ƶ�ʴ�С����Ƶ��Ϊ(freq / 100)MHZ��
* Output         : None
* Return         : None
****************************************************************************/

void RDA5820_SetFrequency(uint16_t freq)
{
	uint8_t space;
	uint16_t band, tmpreg, chan, i = 0;

	tmpreg = RDA5820_ReadData(RDA5820_R03);	 //��ȡ��ʼ����
	tmpreg &= 0x001F;    //CHAN[15:8]
	band = (tmpreg >> 2) & 0x03;
	space = tmpreg & 0x03;

	/* ѡ��channel space */
	if(space == 0)	     //���space=0,channel space is 100KHZ
	{
		space = 10;		
	}
	else if(space == 1)	 //���space=1,channel space is 200KHZ
	{
		space = 20;	
	}
	else				 //���space=2,channel space is 50KHZ
	{
		space = 5; 
	}

	/* ��ǰ���õ����Ƶ��ֵ */
	if(band == 0)                    //band=0; 87~108MHZ
	{
		band = 8700;	
	}
	else if((band == 1)||(band == 2))//band=1; 76~91MHZ	band=2; 76~108MHZ
	{
		band = 7600;
	}
	else
	{
		band = RDA5820_ReadData(RDA5820_R53); //�õ�bottomƵ��
		band *= 10;
	}
	if(freq < band)
	{
		return;
	}
	
	/* chan = (frequence - chan_bottom(khz)) / channe spacing */
	chan = (freq - band) / space;
	chan &= 0x3FF;                 //ȡ��10λ
	tmpreg |= chan << 6;
	tmpreg |= 1 << 4;

	RDA5820_WriteData(RDA5820_R03, tmpreg);	 //����Ƶ��	
	RDA5820_DelayMs(20);				 //��ʱ20ms

	while((RDA5820_ReadData(RDA5820_R0B) & (1 << 7)) == 0) //����FM ready
	{
		i++;
		if(i > 0xFFFE)
		{
			return;
		}	
	}
}

/****************************************************************************
* Function Name  : RDA5820_GetRssi
* Description    : ��ȡ�ź�ǿ�ȣ�RSSI[15:9](�ź�ǿ��)
* Input          : None
* Output         : None
* Return         : ��ȡ�����ź�ǿ��
****************************************************************************/

uint8_t RDA5820_GetRssi(void)
{
    uint16_t tmpreg;

    tmpreg = RDA5820_ReadData(RDA5820_R0B);	 //��ȡ��ʼ����
    tmpreg >>= 8;

    return (uint8_t)tmpreg;
}

/****************************************************************************
* Function Name  : RDA5920_GetFrequency
* Description    : ��ȡ����Ƶ��
* Input          : None
* Output         : None
* Return         : ��ȡ����Ƶ��
****************************************************************************/

uint16_t  RDA5920_GetFrequency(void)
{
    uint8_t space;
	uint16_t band, tmpreg, freq;

	tmpreg = RDA5820_ReadData(RDA5820_R03);	 //��ȡ��ʼ����
	
    freq = tmpreg >> 6;
	band = (tmpreg >> 2) & 0x03;
	space = tmpreg & 0x03;
    
    /* ѡ��channel space */
	if(space == 0)	     //���space=0,channel space is 100KHZ
	{
		space = 10;		
	}
	else if(space == 1)	 //���space=1,channel space is 200KHZ
	{
		space = 20;	
	}
	else				 //���space=2,channel space is 50KHZ
	{
		space = 5; 
	}

    /* ��ǰ���õ����Ƶ��ֵ */
	if(band == 0)                    //band=0; 87~108MHZ
	{
		band = 8700;	
	}
	else if((band == 1)||(band == 2))//band=1; 76~91MHZ	band=2; 76~108MHZ
	{
		band = 7600;
	}
	else
	{
		band = RDA5820_ReadData(RDA5820_R53); //�õ�bottomƵ��
		band *= 10;
	}
    freq = band + freq * space;
    
    return freq;
}



