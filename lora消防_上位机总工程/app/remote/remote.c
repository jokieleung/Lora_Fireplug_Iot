#include "remote.h"
#include "system.h"

/* �������ֵ */
uint8_t RemoteCode[4];

#define REMOTE_Delay1us(x) SYSTICK_Delay1us(x)
#define REMOTE_Delay1ms(x) SYSTICK_Delay1us(x)

/****************************************************************************
* Function Name  : REMOTE_Init
* Description    : ��ʼ��������յ�IO���ⲿ�ж�
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void REMOTE_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* ����GPIOʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    /*  ����GPIO��ģʽ��IO�� */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;         //ѡ����Ҫ���õ�IO��
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //���ô�������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;      //������������ģʽ

    /* ��ʼ��GPIO */
    GPIO_Init(GPIOG, &GPIO_InitStructure);	          

/***************************************************************************/
/**************************** �ж����� *************************************/
/***************************************************************************/

	/* ����NVIC������ע��һ��ֻ�ܿ���һλ�����ܼ�λ���һ���*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//������ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;       //���ô����ȼ�Ϊ0��	
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;     //�������ⲿ�ж�0ͨ���ж�ʹ��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		     //ʹ�ܡ�

    NVIC_Init(&NVIC_InitStructure);

	/* ѡ��EXTI */
	GPIO_EXTILineConfig(EXTI_GPIO_REMOTE, EXTI_PIN_REMOTE); 

    /* �����ⲿ�жϵ�ģʽ */
	EXTI_InitStructure.EXTI_Line = LINE_REMOTE;			    //���ô��ⲿ�ж�ͨ��
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;			    //��ʹ��
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     //�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���

    /*��ʼ��EXTI*/
	EXTI_Init(&EXTI_InitStructure);
}

/****************************************************************************
* Function Name  : EXTI15_10_IRQHandler
* Description    : ������յ��ⲿ�жϺ���
* Input          : None
* Output         : RemoteCode����ȡ���������������
* Return         : None
****************************************************************************/

void EXTI15_10_IRQHandler(void)
{
    uint8_t num, bit, timeCount;
    uint16_t timeOut;

    if (EXTI_GetITStatus(EXTI_Line15))
	{
        EXTI_ClearITPendingBit(EXTI_Line15); //����жϱ�־

        /* ��ʱ7ms�ټ�⣬ȷ���ǽ��յ���ȷ���ź� */
        REMOTE_Delay1ms(2);
        if(REMOTE_IN == 0)
        {
            timeOut = 0;
            /* �ȴ�9ms�ĵ͵�ƽ�������ȥ */
            while(REMOTE_IN == 0)
            {
                REMOTE_Delay1us(100);
                timeOut++;
                if(timeOut > 1000)  //100ms�ȴ���ʱ
                {
                    return;
                }    
            }

            /* ��ȷ�ȵ�9ms�ĵ͵�ƽ�������ȥ */
            if(REMOTE_IN == 1)
            {
                /* �ȴ�4.5ms�ĸߵ�ƽ������ȥ */
                timeOut = 0;
                while(REMOTE_IN == 1)
                {
                    REMOTE_Delay1us(100);
                    timeOut++;
                    if(timeOut > 1000)   //100ms�ȴ���ʱ
                    {
                        return;
                    }    
                }

                /* ��ȷ�ȵ�4.5ms�ĸߵ�ƽ��ȥ */
                for(num=0; num<4; num++)      //�������ݹ���4��
                {
                    for(bit=0; bit<8; bit++)  //һ������8λ
                    {
                        timeOut = 0;
                        while(REMOTE_IN == 0) //��ƽ�ź�ǰ�涼��560us�ĵ͵�ƽ��ǰ׺���ȴ����ȥ
                        {
                            REMOTE_Delay1us(80);
                            timeOut++;
                            if(timeOut > 200)   //16ms�ȴ���ʱ
                            {
                                return;
                            }        
                        }
                        
                        timeOut = 0;         //���Ե�ƽ�źŵĸߵ�ƽʱ�䳤��
                        timeCount = 0;       //�����ʱ
                        while(REMOTE_IN == 1)
                        {
                            REMOTE_Delay1us(100);
                            timeOut++;
                            timeCount++;
                            if(timeOut > 500)  //50ms�ȴ���ʱ
                            {
                                return;
                            }                            
                        }//end of �����߼���ƽ�ĸߵ�ƽ

                        RemoteCode[num] >>= 1;
                        if(timeCount >=  10)   //��ʱ����560us�����߼���1��
                        {
                            RemoteCode[num] |= 0x80;        
                        }    
                    }//end of һ�����ݵĽ���    
                }//end of ����4������    
            }//end of ȷ��9ms�������ȥ
        }//end of ȷ�ϲ��Ǹ����ź�          
	}               
}


