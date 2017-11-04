#include "hc4052.h"

/****************************************************************************
* Function Name  : HC4052_Init
* Description    : ��ʼ��4052��Ƶѡ���ѡ��IO��
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void HC4052_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);

	/*  ѡ���ΪPF10��PF11 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;

	GPIO_Init(GPIOF, &GPIO_InitStructure); 
}

/****************************************************************************
* Function Name  : HC4052_OutputSelect
* Description    : ѡ��HC4052������ͨ����ѡ������������MP3��
* Input          : channel��ѡ����Ҫʹ�õ�ͨ��
* Output         : None
* Return         : None
****************************************************************************/

void HC4052_OutputSelect(uint8_t channel)
{
	/* ��������������� */
	switch(channel)
	{
		case(AUDIO_FM_CHANNEL):
			MC_A_SET;
			MC_B_CLR;
			break;
		
		case(AUDIO_MP3_CHANNEL):
			MC_A_CLR;
			MC_B_CLR;
			break;
		
		default:
			break;
	}
}









