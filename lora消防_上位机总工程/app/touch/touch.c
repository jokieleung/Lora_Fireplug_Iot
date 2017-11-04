#include "touch.h"
#include "spi.h"
#include "flash.h"
#include "gui.h"

#define TOUCH_AdjDelay500ms() SYSTICK_Delay1ms(500)

TouchTypeDef TouchData;         //���������洢��ȡ��������
static PosTypeDef TouchAdj;     //����һ��������������У������

/****************************************************************************
* Function Name  : TOUCH_ReadData
* Description    : ������������ֵ
* Input          : cmd��ѡ��Ҫ��ȡ��X�ỹ��Y�������
* Output         : None
* Return         : ��ȡ������������ֵ
****************************************************************************/

static uint16_t TOUCH_ReadData(uint8_t cmd)
{
    uint8_t i, j;
    uint16_t readValue[TOUCH_READ_TIMES], value;
    uint32_t totalValue;

    /* SPI���ٶȲ��˹��� */
    SPI2_SetSpeed(SPI_BaudRatePrescaler_16);
    /* ��ȡTOUCH_READ_TIMES�δ���ֵ */
    for(i=0; i<TOUCH_READ_TIMES; i++)
    {   /* ��Ƭѡ */
        TOUCH_CS_CLR;
        /* �ڲ��ģʽ�£�XPT2046ת����Ҫ24��ʱ�ӣ�8��ʱ���������֮��1��ʱ��ȥ�� */
        /* æ�źţ��������12λת�������ʣ��3��ʱ���Ǻ���λ */    
        SPI1_WriteReadData(cmd); // �������ѡ��X�����Y�� 
        
        /* ��ȡ���� */
        readValue[i] = SPI1_WriteReadData(0xFF);
        readValue[i] <<= 8;
        readValue[i] |= SPI1_WriteReadData(0xFF);
        
        /* �����ݴ�����ȡ����ADֵ��ֻ��12λ�������λ���� */
        readValue[i] >>= 3;
        TOUCH_CS_SET;
    }

    /* �˲����� */
    /* ���ȴӴ�С���� */
    for(i=0; i<(TOUCH_READ_TIMES - 1); i++)
    {
        for(j=i+1; j<TOUCH_READ_TIMES; j++)
        {
            /* ����ֵ�Ӵ�С�������� */
            if(readValue[i] < readValue[j])
            {
                value = readValue[i];
				readValue[i] = readValue[j];
				readValue[j] = value;
            }   
        }       
    }
   
    /* ȥ�����ֵ��ȥ����Сֵ����ƽ��ֵ */
    j = TOUCH_READ_TIMES - 1;
    totalValue = 0;
    for(i=1; i<j; i++)     //��y��ȫ��ֵ
    {
        totalValue += readValue[i];
    }
    value = totalValue / (TOUCH_READ_TIMES - 2);
      
    return value;
}

/****************************************************************************
* Function Name  : TOUCH_ReadXY
* Description    : ��ȡ��������X��Y�����������ֵ
* Input          : *xValue�������ȡ��X����������ֵ�ĵ�ַ
*                * *yValue�������ȡ��Y����������ֵ�ĵ�ַ
* Output         : None
* Return         : 0����ȡ�ɹ���0xFF����ȡʧ��
****************************************************************************/

static uint8_t TOUCH_ReadXY(uint16_t *xValue, uint16_t *yValue)
{   
    uint16_t xValue1, yValue1, xValue2, yValue2;

    xValue1 = TOUCH_ReadData(TOUCH_X_CMD);
    yValue1 = TOUCH_ReadData(TOUCH_Y_CMD);
    xValue2 = TOUCH_ReadData(TOUCH_X_CMD);
    yValue2 = TOUCH_ReadData(TOUCH_Y_CMD);
    
    /* �鿴������֮���ֻ����ֵ��� */
    if(xValue1 > xValue2)
    {
        *xValue = xValue1 - xValue2;
    }
    else
    {
        *xValue = xValue2 - xValue1;
    }

    if(yValue1 > yValue2)
    {
        *yValue = yValue1 - yValue2;
    }
    else
    {
        *yValue = yValue2 - yValue1;
    }

    /* �жϲ�����ֵ�Ƿ��ڿɿط�Χ�� */
	if((*xValue > TOUCH_MAX) || (*yValue > TOUCH_MAX))  
	{
		return 0xFF;
	}

    /* ��ƽ��ֵ */
    *xValue = (xValue1 + xValue2) / 2;
    *yValue = (yValue1 + yValue2) / 2;

    /* �жϵõ���ֵ���Ƿ���ȡֵ��Χ֮�� */
    if((*xValue > TOUCH_X_MAX) || (*xValue < TOUCH_X_MIN) 
       || (*yValue > TOUCH_Y_MAX) || (*yValue < TOUCH_Y_MIN))
    {                   
        return 0xFF;
    }
 
    return 0; 
}

/****************************************************************************
* Function Name  : TOUCH_ReadAdjust
* Description    : ��У����ʱ���ȡУ�������������ֵ
* Input          : x��У����X����
*                * y��У����Y����
*                * *xValue�������ȡ��X����������ֵ�ĵ�ַ
*                * *yValue�������ȡ��Y����������ֵ�ĵ�ַ
* Output         : None
* Return         : 0����ȡ�ɹ���0xFF:��ȡʧ��
****************************************************************************/

static uint8_t TOUCH_ReadAdjust(uint16_t x, uint16_t y, uint16_t *xValue, uint16_t *yValue)
{
    uint8_t i;
    uint32_t timeCont;

    /* ��ȡУ��������� */
    TFT_ClearScreen(BLACK);
    GUI_DrowSign(x, y, RED);
    i = 0;
    while(1)
    {
        if(!TOUCH_ReadXY(xValue, yValue))
        {
            i++;
            if(i > 10)         //��ʱһ�£��Զ�ȡ���ֵ
            {
                GUI_DrowSign(x, y, BLACK);
                return 0;
            }
               
        }

        /* ��ʱ�˳� */
        if(timeCont > 0xFFFFFFFE)
        {   
                GUI_DrowSign(x, y, BLACK); 
                return 0xFF;
        } 
    }       
}

/****************************************************************************
* Function Name  : TOUCH_Adjust
* Description    : �����Ļ�Ƿ�У����û�еĻ�����У������У��ֵ���õ�FLASH��
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

static void TOUCH_Adjust(void)
{
    uint16_t px[2], py[2], xPot[4], yPot[4];
    float xFactor, yFactor;

    /* ����Ƿ���У������ */
    FLASH_ReadData(&TouchAdj.posState, TOUCH_ADJ_ADDR, sizeof(TouchAdj));
    if(TouchAdj.posState == TOUCH_ADJ_OK)
    {
        return;
    }
    /* ��ȡ��һ���� */
    if(TOUCH_ReadAdjust(LCD_ADJX_MIN, LCD_ADJY_MIN, &xPot[0], &yPot[0]))
    {
        return;
    }   
    TOUCH_AdjDelay500ms();

    /* ��ȡ�ڶ����� */
    if(TOUCH_ReadAdjust(LCD_ADJX_MIN, LCD_ADJY_MAX, &xPot[1], &yPot[1]))
    {
        return;
    }   
    TOUCH_AdjDelay500ms();

    /* ��ȡ�������� */
    if(TOUCH_ReadAdjust(LCD_ADJX_MAX, LCD_ADJY_MIN, &xPot[2], &yPot[2]))
    {
        return;
    }   
    TOUCH_AdjDelay500ms();

    /* ��ȡ���ĸ��� */
    if(TOUCH_ReadAdjust(LCD_ADJX_MAX, LCD_ADJY_MAX, &xPot[3], &yPot[3]))
    {
        return;
    }   
    TOUCH_AdjDelay500ms();
    
    /* �����ȡ�����ĸ�������ݣ����ϳɶԽǵ������� */
    px[0] = (xPot[0] + xPot[1]) / 2;
    py[0] = (yPot[0] + yPot[2]) / 2;
    px[1] = (xPot[3] + xPot[2]) / 2;
    py[1] = (yPot[3] + yPot[1]) / 2;

    /* ����������� */
    xFactor = (float)LCD_ADJ_X / (px[1] - px[0]);
    yFactor = (float)LCD_ADJ_Y / (py[1] - py[0]);  
    
    /* ���ƫ���� */
    TouchAdj.xOffset = (int16_t)LCD_ADJX_MAX - ((float)px[1] * xFactor);
    TouchAdj.yOffset = (int16_t)LCD_ADJY_MAX - ((float)py[1] * yFactor);

    /* �����������������ݴ���Ȼ�󱣴� */
    TouchAdj.xFactor = xFactor ;
    TouchAdj.yFactor = yFactor ;
    
    TouchAdj.posState = TOUCH_ADJ_OK;
    FLASH_WriteData(&TouchAdj.posState, TOUCH_ADJ_ADDR, sizeof(TouchAdj));            
}

/****************************************************************************
* Function Name  : TOUCH_Init
* Description    : ��ʼ��������
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void TOUCH_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* SPI��IO�ں�SPI�����ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    /* TOUCH-CS��IO������ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* TOUCH-PEN��IO������ */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;

    GPIO_Init(GPIOD, &GPIO_InitStructure);

    SPI1_Config();

    /* Ҫʹ��SPI���洢У�����������Գ�ʼ�� */
    TOUCH_Adjust(); //У�����   
}

/****************************************************************************
* Function Name  : TOUCH_Scan
* Description    : ɨ���Ƿ��д�������
* Input          : None
* Output         : TouchData����ȡ������������ֵ�Ͷ�Ӧ�Ĳ�������ֵ
* Return         : 0����ȡ�ɹ���0xFF��û�д���
****************************************************************************/

uint8_t TOUCH_Scan(void)
{
    
    if(TOUCH_PEN == 0)   //�鿴�Ƿ��д���
    {
        if(TOUCH_ReadXY(&TouchData.x, &TouchData.y)) //û�д���
        {
            return 0xFF;    
        }
        /* ������������ֵ���������������ֵ */
        TouchData.lcdx = TouchData.x * TouchAdj.xFactor + TouchAdj.xOffset;
        TouchData.lcdy = TouchData.y * TouchAdj.yFactor + TouchAdj.yOffset;
        
        /* �鿴��������ֵ�Ƿ񳬹�������С */
        if(TouchData.lcdx > TFT_XMAX)
        {
            TouchData.lcdx = TFT_XMAX;
        }
        if(TouchData.lcdy > TFT_YMAX)
        {
            TouchData.lcdy = TFT_YMAX;
        }
        return 0; 
    }
    return 0xFF;       
}

