#include "lpwr.h"

/****************************************************************************
* Function Name  : LPWR_Config
* Description    : ��ʼ��PA0���ⲿ�жϣ�����������Ƭ���Ӵ���ģʽ����
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void LPWR_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

    /* ����GPIOʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	  //PA.0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //��������

	GPIO_Init(GPIOA, &GPIO_InitStructure);	      //��ʼ��IO

/***************************************************************************/
/**************************** �ж����� *************************************/
/***************************************************************************/

	/* ����NVIC������ע��һ��ֻ�ܿ���һλ�����ܼ�λ���һ���*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//������ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;       //���ô����ȼ�Ϊ0��	
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;         //�������ⲿ�ж�0ͨ���ж�ʹ��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		     //ʹ�ܡ�

    NVIC_Init(&NVIC_InitStructure);

	/* ѡ��EXTI */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); //ѡ��PA0���ⲿ�ж�

    /* �����ⲿ�жϵ�ģʽ */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;			    //���ô��ⲿ�ж�ͨ��
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;			    //��ʹ��
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     //�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //�����ش���

    /*��ʼ��EXTI*/
	EXTI_Init(&EXTI_InitStructure);

}

/****************************************************************************
* Function Name  : LPWR_EnterStandbyMode
* Description    : �������ģʽ
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void LPWR_EnterStandbyMode(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//ʹ��PWR����ʱ��
	PWR_WakeUpPinCmd(ENABLE);                           //ʹ�ܻ��ѹܽŹ���
	PWR_EnterSTANDBYMode();	                            //���������STANDBY��ģʽ 		    
}

/****************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void EXTI0_IRQHandler(void)
{ 		    		    				     		    
	EXTI_ClearITPendingBit(EXTI_Line0);                 //���LINE10�ϵ��жϱ�־λ		  
}









