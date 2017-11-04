#include "dma.h"

/****************************************************************************
* Function Name  : DMA_Config
* Description    : ��ʼ��DMA������
* Input          : DMA_CHx�����õ�ͨ��
*                * pAddr���������ַ
*                * mAddr�����������ڴ����ַ
*                * length��DMA����Ĵ�С
* Output         : None
* Return         : None
****************************************************************************/

void DMA_Config(DMA_Channel_TypeDef* DMA_CHx,uint32_t pAddr,uint32_t mAddr,uint16_t length)
{
    DMA_InitTypeDef DMA_InitStructure;

    /* ����ʱ�� */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
    
    DMA_DeInit(DMA_CHx);                    //��DMA��ͨ���Ĵ�������Ϊȱʡֵ

    /* ��ʼ������ */
	DMA_InitStructure.DMA_PeripheralBaseAddr = pAddr;    //DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = mAddr;        //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	DMA_InitStructure.DMA_BufferSize = length;          //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;   //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;            //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                    //��������������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;            //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;           //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	
    DMA_Init(DMA_CHx, &DMA_InitStructure);                 //��ʼ��
}

/****************************************************************************
* Function Name  : DMA_Enable
* Description    : ����һ��DMA����
* Input          : DMA_CHx��ʹ�õ�ͨ��
*                * length��DMA�����С
* Output         : None
* Return         : None
****************************************************************************/

void DMA_Enable(DMA_Channel_TypeDef*DMA_CHx, uint32_t length)
{ 
	DMA_Cmd(DMA_CHx, DISABLE );                        //�ر�USART1 TX DMA1 ��ָʾ��ͨ��      
 	DMA_SetCurrDataCounter(DMA1_Channel4,length);      //DMAͨ����DMA����Ĵ�С
 	DMA_Cmd(DMA_CHx, ENABLE);                          //ʹ��USART1 TX DMA1 ��ָʾ��ͨ�� 
}
