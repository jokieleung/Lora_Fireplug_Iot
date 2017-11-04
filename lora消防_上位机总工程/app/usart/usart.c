#include "usart.h"


void USART1_NVIC_RxConfig(void);
void USART2_NVIC_RxConfig(void);

/****************************************************************************
* Function Name  : USART1_Config
* Description    : Configurates the USART1.
* Input          : baudRate��������
* Output         : None
* Return         : None
****************************************************************************/

void USART1_Config(uint16_t baudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* ��RCCʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* ����TXD��GPIO���� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;               //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                     //�������PA9

	/* ��ʼ����������IO */
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ����RXD��GPIO���� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;          //ģ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                     //��������PA10

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ����USART�Ĳ��� */
	USART_InitStructure.USART_BaudRate = baudRate;                  //������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;     //���ݳ�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;          //1λֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;             //��Ч��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ʧ��Ӳ����
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; //�������ͺͽ���ģʽ
	
    /* ��ʼ��USART1 */
	USART_Init(USART1, &USART_InitStructure);
	
	/* ʹ��USART1 */
	USART_Cmd(USART1, ENABLE);
#ifdef USE_USART1RX_INTERRUPT  
    USART1_NVIC_RxConfig();
    USART_ITConfig(USART1, USART_IT_RXNE ,ENABLE);
#endif 		
}

/****************************************************************************
* Function Name  : USART2_Config
* Description    : Configurates the USART1.
* Input          : baudRate��������
* Output         : None
* Return         : None
****************************************************************************/

void USART2_Config(uint16_t baudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* ��RCCʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* ����TXD��GPIO���� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;               //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                     //�������PA9

	/* ��ʼ����������IO */
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ����RXD��GPIO���� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;          //ģ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;                      //��������PA10

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ����USART�Ĳ��� */
	USART_InitStructure.USART_BaudRate = baudRate;                  //������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;     //���ݳ�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;          //1λֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;             //��Ч��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ʧ��Ӳ����
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; //�������ͺͽ���ģʽ
	
    /* ��ʼ��USART1 */
	USART_Init(USART2, &USART_InitStructure);
	
	/* ʹ��USART1 */
	USART_Cmd(USART2, ENABLE);
#ifdef USE_USART2RX_INTERRUPT   
    USART2_NVIC_RxConfig();
    USART_ITConfig(USART2, USART_IT_RXNE ,ENABLE);
#endif 		
}

/****************************************************************************
* Function Name  : USART1_SetWord
* Description    : ͨ������1�����ַ���.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void USART1_SendWord(uint8_t *wd)
{	
	while(*wd)                      //����Ƿ��������Ƿ�Ϊ��
	{
		USART_SendData(USART1, *wd);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET); //�ȴ��������
		wd++;
	}		
}

/****************************************************************************
* Function Name  : USART2_SetWord
* Description    : ͨ������2�����ַ���.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void USART2_SendWord(uint8_t *wd)
{	
	while(*wd)                      //����Ƿ��������Ƿ�Ϊ��
	{
		USART_SendData(USART2, *wd);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) != SET); //�ȴ��������
		wd++;
	}		
}

/* ���Ҫʹ��printf�����Ļ�Ҫ������´��� */
/* ����ʹ�Target Options�����Targetλ��ѡ��Use MicroLIB */
#ifdef USE_PRINTF

#pragma import(__use_no_semihosting)             
/* ��׼����Ҫ��֧�ֺ��� */                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout; 
/* ����_sys_exit()�Ա���ʹ�ð�����ģʽ */    
_sys_exit(int x) 
{ 
	x = x; 
}

/****************************************************************************
* Function Name  : fputc
* Description    : ʹ��printf����Ҫ�ض������fputc����.
* Input          : ch , *f
* Output         : None
* Return         : ch
****************************************************************************/
 
int fputc(int ch, FILE *f)
{
	
	USART_SendData(USART1, (uint8_t) ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
	
	return ch;	
}
////* �Ĵ����� */
//int fputc(int ch, FILE *f)
//{      
//	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
//	USART1->DR = (u8) ch;      
//	return ch;
//}

#endif

#ifdef USE_USART1RX_INTERRUPT

/****************************************************************************
* Function Name  : USART1_NVIC_RxConfig
* Description    : ���ý����жϵ��жϵȼ����������ж�
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void USART1_NVIC_RxConfig(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* ����NVIC���� */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //��Ӧ���ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;   //��USART1��ȫ���ж�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		      //ʹ�ܡ�

    NVIC_Init(&NVIC_InitStructure);
}

/****************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : ����1���жϺ���
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/
uint8_t USART1_RX_Buff[64], USART1_RX_State;

void USART1_IRQHandler (void)
{
    uint8_t dat;

    /* �����ж�(���յ������ݱ�����0x0D 0x0A��β(���س�))��ʾ���ս��� */
    if(USART_GetITStatus(USART1, USART_IT_RXNE))
    {
        
        dat = USART_ReceiveData(USART1);
        /* û�����꣬ʹ��USART1_RX_State�����λ��������ʶ */
        if(((USART1_RX_State & 0x80) == 0) && ((USART1_RX_State & 0x3F) < 63)) 
        {   
            /* �Ѿ����յ�0x0D��ʹ��USART1_RX_State�ĵ�6λ����0λ��ʼ�������յ�0x0D�ı�ʶ */
            if(USART1_RX_State & 0x40)    
            {
                if(dat == 0x0A)    //�ڶ����ֽڽ��յ�0x0A���ս���
                {
                    USART1_RX_State |= 0x80;
                    USART1_RX_Buff[USART1_RX_State & 0x3F] = 0;   
                }
                else
                {
                    USART1_RX_State = 0; 
                }
            }
            else
            {
                if(dat == 0x0D)     //���յ�0x0D����ʶUSART1_RX_State�ĵ�6λ(��0λ��ʼ)
                {
                    USART1_RX_State |= 0x40;    
                }
                else
                {
                    USART1_RX_Buff[USART1_RX_State & 0x3F] = dat;
                    USART1_RX_State++;    
                }
            } 
        }
    }    
}

#endif

#ifdef USE_USART2RX_INTERRUPT

/****************************************************************************
* Function Name  : USART1_NVIC_RxConfig
* Description    : ���ý����жϵ��жϵȼ����������ж�
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void USART2_NVIC_RxConfig(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* ����NVIC���� */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //��Ӧ���ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;         //��USART2��ȫ���ж�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		      //ʹ�ܡ�

    NVIC_Init(&NVIC_InitStructure);
}

/****************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : ����2���жϺ���
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/
uint8_t USART2_RX_Buff[64], USART2_RX_State;

void USART2_IRQHandler (void)
{
    uint8_t dat;
    if(USART_GetITStatus(USART2, USART_IT_RXNE))
    {
        dat = USART_ReceiveData(USART2);

        /* û�����꣬ʹ��USART2_RX_State�����λ��������ʶ */
        if(((USART2_RX_State & 0x80) == 0) && ((USART2_RX_State & 0x3F) < 63)) 
        {   
            /* �Ѿ����յ�0x0D��ʹ��USART2_RX_State�ĵ�6λ����0λ��ʼ�������յ�0x0D�ı�ʶ */
            if(USART2_RX_State & 0x40)    
            {
                if(dat == 0x0A)    //�ڶ����ֽڽ��յ�0x0A���ս���
                {
                    USART2_RX_State |= 0x80;
                    USART2_RX_Buff[USART2_RX_State & 0x3F] = 0;   
                }
                else
                {
                    USART2_RX_State = 0; 
                }
            }
            else
            {
                if(dat == 0x0D)     //���յ�0x0D����ʶUSART2_RX_State�ĵ�6λ(��0λ��ʼ)
                {
                    USART2_RX_State |= 0x40;    
                }
                else
                {
                    USART2_RX_Buff[USART2_RX_State & 0x3F] = dat;
                    USART2_RX_State++;    
                }
            } 
        }
    }    
}

#endif














