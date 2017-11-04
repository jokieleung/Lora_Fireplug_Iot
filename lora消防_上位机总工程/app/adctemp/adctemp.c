#include "adctemp.h"

/****************************************************************************
* Function Name  : ADCTEMP_Config
* Description    : ��ʼ���ڲ�ADC
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void ADCTEMP_Config(void)
{    
	ADC_InitTypeDef  ADC_InitStructure;

	/* ��ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);           //72M/6=12,ADC ���ʱ�Ӳ��ܳ��� 14M
    
    ADC_DeInit(ADC1);  //������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

   	/* ��ʼ��ADC���� */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;          //������ʽΪ����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	            //��ʹ��ɨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;         //��ʹ������ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//�������ģʽ
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;      //���ݶ��뷽ʽ���Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;        //˳����й���ת����ADC ͨ������Ŀ1 

	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_TempSensorVrefintCmd(ENABLE);   //�����ڲ��¶ȴ�����
    ADC_Cmd(ADC1, ENABLE);              //��ADCʹ��

	ADC_ResetCalibration(ADC1);                  //������λУ׼    
	while(ADC_GetResetCalibrationStatus(ADC1));  //�ȴ���λУ׼���� 

	ADC_StartCalibration(ADC1);                  //����ADУ׼ 
	while(ADC_GetCalibrationStatus(ADC1));       //�ȴ�У׼����        
   
}

/****************************************************************************
* Function Name  : ADCTEMP_GetAdc
* Description    : ��ȡ�¶ȴ�������ADֵ
* Input          : None
* Output         : None
* Return         : ���ض�ȡ����ADֵ
****************************************************************************/

static uint16_t ADCTEMP_GetAdc(void)
{
    uint16_t value;

    /* �¶ȴ�������ͨ��16���������˳��ֵΪ1������ʱ��Ϊ 239.5���ڡ� */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_239Cycles5 );  
    
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);        //ʹ��ָ����ADC1 �����ת������
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	/* ��ȡADC1�������ת����� */	
	value = ADC_GetConversionValue(ADC1); 
    
    return value;
}

/****************************************************************************
* Function Name  : ADCTEMP_GetTemp
* Description    : ��ȡ�¶�ֵ
* Input          : None
* Output         : None
* Return         : ���ض�ȡ�����¶�ֵ���Ŵ�100����������λС���㣩
****************************************************************************/

int16_t ADCTEMP_GetTemp(void)
{
    uint32_t adValue = 0;
    int16_t tempValue;
    float adVoltage;
    uint8_t time;
    
    /*  ��ȡʮ��Ȼ�����ƽ��ֵ */
    for(time = 0; time < 10; time++)
    {
        adValue += ADCTEMP_GetAdc();
    }
    adValue = adValue / 10;

    /* ��ADֵת��Ϊ��ѹֵ */
    adVoltage = (float)adValue * 3.3 / 4096;

    /* ���ݹ�ʽ����¶�ֵ */
    adVoltage = ((float)(ADCTEMP_Vsence25 - adVoltage) / ADCTEMP_AvgSlope) + 25; 
    tempValue = adVoltage * 100;   //������λС����

    return tempValue;
}







