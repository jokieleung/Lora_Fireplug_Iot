#include "adc.h"

/****************************************************************************
* Function Name  : ADC_Config
* Description    : ��ʼ��ADC
* Input          : adc��ѡ���ʼ����ADC�������ǿ���������������ADC���룩
* Output         : None
* Return         : None
****************************************************************************/

void ADC_Config(uint8_t adc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef  ADC_InitStructure;

/***************************************************************************/
/********************** IO��ģʽ���� ***************************************/
/***************************************************************************/

    if((adc & ADC_Chn0) == ADC_Chn0) //���ѡ���ADC0
    {
        /* ��ʱ�� */
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;          //PB0
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;      //ģ������
    	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

        GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
    if((adc & ADC_Chn1) == ADC_Chn1)
    {
        /* ��ʱ�� */
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;          //PB1
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;      //ģ������
    	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

        GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
    if((adc & ADC_Chn2) == ADC_Chn2)
    {
        /* ��ʱ�� */
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;          //PA1
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;      //ģ������
    	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

        GPIO_Init(GPIOA, &GPIO_InitStructure); 
    }

/***************************************************************************/
/*********************** ADC�������� ***************************************/
/***************************************************************************/

	/* ��ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);           //72M/6=12,ADC ���ʱ�Ӳ��ܳ��� 14M

	/* ��ʼ��ADC���� */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;          //������ʽΪ����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	            //��ʹ��ɨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;         //��ʹ������ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//�������ģʽ
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;      //���ݶ��뷽ʽ���Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;        //˳����й���ת����ADC ͨ������Ŀ1 

	ADC_Init(ADC1, &ADC_InitStructure);
    ADC_Cmd(ADC1, ENABLE);                       //��ADCʹ��

	ADC_ResetCalibration(ADC1);                  //������λУ׼    
	while(ADC_GetResetCalibrationStatus(ADC1));  //�ȴ���λУ׼���� 

	ADC_StartCalibration(ADC1);                  //����ADУ׼ 
	while(ADC_GetCalibrationStatus(ADC1));       //�ȴ�У׼����        
}

/****************************************************************************
* Function Name  : ADC_ReadChn0
* Description    : ��ȡһ��ADC0��ת�����
* Input          : None
* Output         : None
* Return         : һ��12λ��ת�����
****************************************************************************/

uint16_t ADC_ReadChn0(void)
{
    uint16_t Value;

    /* ADC0��PB0��ͨ��8���������˳��ֵΪ1������ʱ��Ϊ 239.5���ڡ� */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_239Cycles5 );  
    
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);        //ʹ��ָ����ADC1 �����ת������
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	/* ��ȡADC1�������ת����� */	
	Value = ADC_GetConversionValue(ADC1); 
    
    return Value;
}

/****************************************************************************
* Function Name  : ADC_ReadChn1
* Description    : ��ȡһ��ADC1��ת�����
* Input          : None
* Output         : None
* Return         : һ��12λ��ת�����
****************************************************************************/

uint16_t ADC_ReadChn1(void)
{
    uint16_t Value;

    /* ADC0��PB1��ͨ��9���������˳��ֵΪ1������ʱ��Ϊ 239.5���ڡ� */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_239Cycles5 );  
    
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);        //ʹ��ָ����ADC1 �����ת������
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	/* ��ȡADC1�������ת����� */	
	Value = ADC_GetConversionValue(ADC1); 
    
    return Value;
}

/****************************************************************************
* Function Name  : ADC_ReadChn2
* Description    : ��ȡһ��ADC2��ת�����
* Input          : None
* Output         : None
* Return         : һ��12λ��ת�����
****************************************************************************/

uint16_t ADC_ReadChn2(void)
{
    uint16_t Value;

    /* ADC0��PA1��ͨ��1���������˳��ֵΪ1������ʱ��Ϊ 239.5���ڡ� */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5 );  
    
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);        //ʹ��ָ����ADC1 �����ת������
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	/* ��ȡADC1�������ת����� */	
	Value = ADC_GetConversionValue(ADC1); 
    
    return Value;
}

