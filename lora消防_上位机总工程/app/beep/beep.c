#include "beep.h"
#include "system.h"

/****************************************************************************
* Function Name  : BEEP_Config
* Description    : ��ʼ����������IO����
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void BEEP_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;               //����һ���ṹ�������������ʼ��GPIO

    /* ����GPIOʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    /*  ����GPIO��ģʽ��IO�� */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;          //ѡ����Ҫ���õ�IO��
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //���ô�������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //�����������ģʽ

    /* ��ʼ��GPIO */
    GPIO_Init(GPIOB, &GPIO_InitStructure);	
                       
}

/****************************************************************************
* Function Name  : BEEP_Work
* Description    : ���������һ��
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void BEEP_Work(void)
{
    uint16_t i;
    uint8_t j;

    for(j=0; j<50; j++)           //����һ��ʱ���PWM����ʹ������������һ��
    {
        PBout(5) = 1;             //ͨ�����Ƕ����λ���������PB5������ߵ�ƽ
        for(i=0; i<4500; i++);    //��ʱ����PWM��Ƶ��
        PBout(5) = 0; 
        for(i=0; i<4500; i++);   
    }
}

