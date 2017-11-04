#include "can.h"

#ifdef CAN_RX0_INT_ENABLE

uint8_t CAN_RX_BUFF[8];

#endif

/****************************************************************************
* Function Name  : CAN1_NVIC_Config
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

static void CAN1_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* ʹ�ܽ��յ��жϺ��ж����ȼ� */
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);   //FIFO0��Ϣ�Һ��ж�����.	
}

/****************************************************************************
* Function Name  : CAN1_Config
* Description    : ��ʼ��CAN������������Ϊ450K
* Input          : mode������ѡ��Ҫʹ�õĹ���ģʽ����Ҫ�����ֹ���ģʽ��1������
*                * ģʽ��CAN_Mode_Normal��2��CAN_Mode_Silent ����Ĭģʽ��3����
*                * ��ģʽ��CAN_Mode_LoopBack��4����Ĭ����ģʽ��CAN_Mode_Silent
*                * _LoopBack��
* Output         : None
* Return         : None
****************************************************************************/

void CAN1_Config(uint8_t mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;

	/* ��ʼ��IO�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;        //PA12
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;     //��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;        //PA11

	GPIO_Init(GPIOA, &GPIO_InitStructure);

/***************************************************************************/
/********************* CAN���úͳ�ʼ�� *************************************/
/***************************************************************************/

	/* ��ʱ��ʹ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    /* ��ʼ��CAN�Ĳ��� */

	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);

	/* CAN ������ʼ�� */
	CAN_InitStructure.CAN_TTCM = DISABLE;    //ʧ��ʱ�䴥��ģʽ
	CAN_InitStructure.CAN_ABOM = DISABLE;    //ʧ���Զ����߹���
	CAN_InitStructure.CAN_AWUM = DISABLE;    //ʧ��˯��ģʽͨ���������
	CAN_InitStructure.CAN_NART = DISABLE;    //ʧ�ܷ��Զ��ش���ģʽ��Ҳ���ǻ��Զ��ش��䣩
	CAN_InitStructure.CAN_RFLM = DISABLE;    //ʧ�ܽ���FIFO����ģʽ�������ݻḲ�Ǿ�����
	CAN_InitStructure.CAN_TXFP = DISABLE;    //���ȼ��ɱ��ı�ʶ������ 
	CAN_InitStructure.CAN_Mode = mode;       //����ͨģʽ����չģʽ
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq; //����ͬ����Ծ��� 1 ��ʱ�䵥λ

    /* ����������, ��APB1��ʱ��Ƶ����36MHZ��ʱ�� �����ʵĹ�ʽΪ�� */
    /* ������(Kpbs) = 36M / ((CAN_BS1 + CAN_BS2 + 1) *  CAN_Prescaler) */
	CAN_InitStructure.CAN_BS1 = CAN_BS1_8tq; //ʱ��� 1 Ϊ8 ��ʱ�䵥λ 
	CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq; //ʱ��� 2 Ϊ7 ��ʱ�䵥λ
	CAN_InitStructure.CAN_Prescaler = 5;	 

	CAN_Init(CAN1, &CAN_InitStructure);

#ifdef CAN_RX0_INT_ENABLE
	CAN1_NVIC_Config();
#endif    

}

/****************************************************************************
* Function Name  : CAN1_SendMesg
* Description    : ����һ������
* Input          : id�����͵�ID��
*                * len�����͵����ݳ���(ע�ⷢ�����ݳ��Ȳ��ܳ���8���ֽ�)
*                * dat��������ݵ�ָ��
* Output         : None
* Return         : None
****************************************************************************/

void CAN1_SendMesg(uint32_t id, uint8_t len, uint8_t *dat)
{
	uint16_t i = 0;
	CanTxMsg TxMessage;

    /* һ�η���ֻ�ܷ���8���ֽ� */
    if(len > 8)
    {
        return ;
    }
	/* �������䣺���ñ�ʶ�������ݳ��Ⱥʹ��������� */
	TxMessage.StdId = (id & 0x7FF); //��׼֡ID11λ
	TxMessage.ExtId = (id >> 11);   //������չ��ʾ������չ��ʾ����29λ��
	TxMessage.RTR = CAN_RTR_DATA;   //����Ϊ����֡����Զ��֡ΪCAN_RTR_Remote��
    if((id & 0x7FF) == 0x7FF)       //����Ǳ�׼֡������չ֡����չ֡����11λ��
    {
    	TxMessage.IDE = CAN_ID_STD;	//��չID   
    }
    else
    {
    	TxMessage.IDE = CAN_ID_EXT;	//��׼ID
    }
	TxMessage.DLC = len;	        //���͵����ݳ���

	/* �����ݷ��뵽������ */
	for(i=0; i<len; i++)	         
	{
		TxMessage.Data[i] = *dat;
		dat++;	
	}
    
    /* ��ʼ�������� */
	CAN_Transmit(CAN1, &TxMessage); 
}

/****************************************************************************
* Function Name  : CAN1_Config16BitFilter
* Description    : ����CAN����16������׼ID������IDλ��ȫ����ͬ���ܹ�ͨ����
* Input          : id1��Ҫ���յ�һ��ID
*                * id2��Ҫ���յ�һ��ID
* Output         : None
* Return         : None
****************************************************************************/

void CAN1_Config16BitFilter(uint16_t id1, uint16_t id2)
{
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    uint16_t mask = 0xFFFF;

	/* CAN filter init ���μĴ�����ʼ�� */
	CAN_FilterInitStructure.CAN_FilterNumber = 1;	               //������1
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;//IDģʽ

	/* �Ĵ���������Ϊ16λ */
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = (id1 << 5);    //Ҫ���յ�ID��ʾ��1		
    CAN_FilterInitStructure.CAN_FilterIdLow =  (id2 << 5);	  //Ҫ���յ�ID��ʾ��2

	/* ����Ϊ����IDλ��Ҫ��ͬ�Ž��� */	
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (mask << 5); //MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow  = (mask << 5);
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0; //FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE; //ʹ�ܹ�����1

	CAN_FilterInit(&CAN_FilterInitStructure);
}

/****************************************************************************
* Function Name  : CAN1_Config32BitFilter
* Description    : ����һ����չID�Ľ���
* Input          : id��Ҫ���յ�ID
* Output         : None
* Return         : None
****************************************************************************/

void CAN1_Config32BitFilter(uint32_t id)
{
    uint32_t mask = 0xFFFFFFFF;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	/* CAN filter init ���μĴ�����ʼ�� */
	CAN_FilterInitStructure.CAN_FilterNumber = 1;	               //������1
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;//IDģʽ

	/* �Ĵ���������Ϊ32λ */
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = (id >> 13);    //Ҫ���յ�ID��ʾ��1		
    CAN_FilterInitStructure.CAN_FilterIdLow =  (id << 3 ) | 4;//Ҫ���յ�ID��ʾ��2

	/* ����Ϊ����IDλ��Ҫ��ͬ�Ž��� */	
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = mask >> 13;     //MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow  = (mask << 3) | 4;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0; //FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE; //ʹ�ܹ�����1

	CAN_FilterInit(&CAN_FilterInitStructure);
}

/****************************************************************************
* Function Name  : CAN1_ReceiveMesg
* Description    : ����һ������
* Input          : receiveBuff���������ݵ�����ָ��
* Output         : None
* Return         : None
****************************************************************************/

void CAN1_ReceiveMesg(uint8_t *receiveBuff)
{
	uint8_t i = 0;

	CanRxMsg RxMessage;	//���ý�������

	if((CAN_MessagePending(CAN1, CAN_FIFO0) != 0)) //���FIFO0�����Ƿ�������
	{
    	CAN_Receive(CAN1,CAN_FIFO0,&RxMessage); //��ȡFIFO0���������
    	for(i=0; i<RxMessage.DLC; i++)          //����ȡ��������λ����CAN_RXSBUF
    	{
    		*receiveBuff = RxMessage.Data[i];
    		receiveBuff++;
    	}
    }			
}

/****************************************************************************
* Function Name  : USB_LP_CAN1_RX0_IRQHandler
* Description    : FIFO0����һ�������������ݣ����8���ֽڣ�
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

#ifdef CAN_RX0_INT_ENABLE

void USB_LP_CAN1_RX0_IRQHandler(void)
{
	CAN1_ReceiveMesg(CAN_RX_BUFF);	
}

#endif

