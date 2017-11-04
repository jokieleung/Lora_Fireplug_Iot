#include "adx345.h"
#include "iic.h"
#include "math.h"
#include "system.h"

#define ADX345_DelayMs(x)     {SYSTICK_Delay1ms(x);}  //��ʱ����

/****************************************************************************
* Function Name  : ADX345_WriteReg
* Description    : ����ADX345�Ĵ���
* Input          : addr���Ĵ�����ַ
*                * dat����������
* Output         : None
* Return         : None
****************************************************************************/

static int8_t ADX345_WriteReg(uint8_t addr, uint8_t dat)
{
	IIC_Start();
	IIC_SendData(ADX345_ADDR); //24C02д��ַ
    if(IIC_WaitAck())
    {
        return 0xFF;
    }
    IIC_SendData(addr); 
    if(IIC_WaitAck())
    {
        return 0xFF;
    }
    IIC_SendData(dat); 
    if(IIC_WaitAck())
    {
        return 0xFF;
    }
    
    IIC_Stop();
    return 0;   
}

/****************************************************************************
* Function Name  : ADX345_ReadReg
* Description    : ��ȡADX345�Ĵ���
* Input          : addr����ȡ��ַ
* Output         : None
* Return         : ��ȡ����8λ����
****************************************************************************/

static uint8_t ADX345_ReadReg(uint8_t addr)
{
    uint8_t readValue = 0xFF;

    IIC_Start();
	IIC_SendData(ADX345_ADDR);    //24C02д��ַ
    if(IIC_WaitAck())
    {
        return readValue;
    }
    IIC_SendData(addr); 
    if(IIC_WaitAck())
    {
        return readValue;
    }

    IIC_Start();
	IIC_SendData(ADX345_ADDR + 1); //24C02����ַ
    if(IIC_WaitAck())
    {
        return readValue;       
    }
    readValue = IIC_ReceiveData(0);
    IIC_Stop();

    return readValue;
}

/****************************************************************************
* Function Name  : ADX345_ReadXYZ
* Description    : ��ȡX,Y,Z��ADֵ
* Input          : xValue��X��ı����ַ
*                * yValue��Y��ı����ַ
*                * zValue��Z��ı����ַ
* Output         : None
* Return         : 0����ȡ�ɹ���0xFF����ȡʧ��
****************************************************************************/

static int8_t ADX345_ReadXYZ(int16_t *xValue, int16_t *yValue, int16_t *zValue)
{
    uint8_t readValue[6], i;
    IIC_Start();
	IIC_SendData(ADX345_ADDR);    //24C02д��ַ
    if(IIC_WaitAck())
    {
        return 0xFF;
    }
    IIC_SendData(0x32);           //���Ͷ���ַ��X���׵�ַ��
    if(IIC_WaitAck())
    {
        return 0xFF;
    }

    IIC_Start();
	IIC_SendData(ADX345_ADDR + 1); //24C02����ַ
    if(IIC_WaitAck())
    {
        return 0xFF;       
    }

    /* ��ȡ�����ֽ����� */
    for(i=0; i<6; i++)
    {
        
        if(i == 5)        //�������һ���ֽ�ʱ������NACK
        {
            readValue[i] = IIC_ReceiveData(0);
        }
        else             //���ͽ��պ�Ӧ��
        {
            readValue[i] = IIC_ReceiveData(1); 
        }
    }
    IIC_Stop();
    /* �����ȡ�������� */
	*xValue = (uint16_t)(readValue[1] << 8) + readValue[0]; 	    
	*yValue = (uint16_t)(readValue[3] << 8) + readValue[2]; 	    
	*zValue = (uint16_t)(readValue[5] << 8) + readValue[4]; 	   

   return 0;
}

/****************************************************************************
* Function Name  : ADX345_Init
* Description    : ��ʼ��ADX345�����˶�ADX��ID
* Input          : None
* Output         : None
* Return         : 0���ɹ���0xFF��ʧ��
****************************************************************************/

int ADX345_Init(void)
{
    IIC_Config();
    if(ADX345_ReadReg(ADX_DEVID) == 0xE5)
    {
        ADX345_WriteReg(ADX_DATA_FORMAT,0x2B);//3��SPI���,13λȫ�ֱ���,��������Ҷ���,16g���� 
		ADX345_WriteReg(ADX_BW_RATE,0x0A);	  //��������ٶ�Ϊ100Hz
		ADX345_WriteReg(ADX_POWER_CTL,0x28);  //����ʹ��,����ģʽ
		ADX345_WriteReg(ADX_INT_ENABLE,0x00); //��ʹ���ж�		 
	 	ADX345_WriteReg(ADX_OFSX,0x00);       //�û���ֵ
		ADX345_WriteReg(ADX_OFSY,0x00);       //X��ƫ��
		ADX345_WriteReg(ADX_OFSZ,0x00);       //Y��ƫ��
        return 0;
    }
   
    return 0xFF; //���س�ʼ��ʧ��
}

/****************************************************************************
* Function Name  : ADX345_Adjust
* Description    : ADX345����У����
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void ADX345_Adjust(void)
{
    int32_t offx = 0, offy = 0, offz = 0;
    int16_t xValue, yValue, zValue;
    uint8_t i;

	ADX345_WriteReg(ADX_POWER_CTL, 0x00);	 //�Ƚ�������ģʽ.
	ADX345_DelayMs(100);
	ADX345_Init(); 
    ADX345_DelayMs(20);
    
    /* ��ȡʮ����ֵ */
    for(i=0; i<10; i++)
    {
        ADX345_ReadXYZ(&xValue, &yValue, &zValue);
        offx += xValue;
        offy += yValue;
        offz += zValue;
        ADX345_DelayMs(10);   //����Ƶ����100HZ��10ms����һ�����         
    }
    
    /* ���ƽ��ֵ */
    offx /= 10;
    offy /= 10;
    offz /= 10;
    
    /* ȫ�ֱ����£�ÿ�����LSBΪ3.9 mg��ƫ�ƼĴ���LSB���ķ�֮һ�����Գ���4 */
    xValue = -(offx / 4);
	yValue = -(offy / 4);
	zValue = -((offz - 256) / 4);
    
    /* ����ƫ���� */
	ADX345_WriteReg(ADX_OFSX, xValue);
	ADX345_WriteReg(ADX_OFSY, yValue);
	ADX345_WriteReg(ADX_OFSZ, zValue); 
            
}

/****************************************************************************
* Function Name  : ADX_GetXYZData
* Description    : ��ȡADX��XYZ���ֵ�����й����ݴ���
* Input          : xValue��X��ı����ַ
*                * yValue��Y��ı����ַ
*                * zValue��Z��ı����ַ
* Output         : None
* Return         : None
****************************************************************************/

void ADX_GetXYZData(int16_t *xValue, int16_t *yValue, int16_t *zValue)
{
    int32_t xTotal = 0, yTotal = 0, zTotal = 0;
    uint8_t i;
    
    *xValue = 0;
    *yValue = 0;
    *zValue = 0;

    /* ��ȡʮ�β���ֵ */
    for(i=0; i<10; i++)
    {
        ADX345_ReadXYZ(xValue, yValue, zValue);
        xTotal += *xValue;
        yTotal += *yValue;
        zTotal += *zValue;
        ADX345_DelayMs(10);  //����Ƶ����100HZ��10ms����һ����� 
    }
    
    /* ���ƽ��ֵ */
    *xValue = xTotal / 10;
    *yValue = yTotal / 10;
    *zValue = zTotal / 10;       
}

/****************************************************************************
* Function Name  : ADX_GetAngle
* Description    : ��ADֵת���ɽǶ�ֵ
* Input          : xValue��x��ֵ
*                * yValue��y��ֵ
*                * zValue��z��ֵ
*                * dir��0����Z��ĽǶ�;1����X��ĽǶ�;2����Y��ĽǶ�.
* Output         : None
* Return         : None
****************************************************************************/

int16_t ADX_GetAngle(float xValue, float yValue, float zValue, uint8_t dir)
{
	float temp;
 	float res = 0;

	switch(dir)
	{   
        /* ����ȻZ��ĽǶ� */
		case 0:
 			temp = sqrt((xValue * xValue + yValue * yValue)) / zValue;
 			res = atan(temp);
 			break;
        
        /* ����ȻX��ĽǶ� */
		case 1:
 			temp = xValue / sqrt((yValue * yValue + zValue * zValue));
 			res = atan(temp);
 			break;

        /* ����ȻY��ĽǶ� */
 		case 2:
 			temp = yValue / sqrt((xValue * xValue + zValue * zValue));
 			res = atan(temp);
 			break;

        default:
            break;
 	}

    res = res * 1800 / 3.14; 

	return res;
}

