#include "key.h"
#include "usart.h"

/* �������� */
static void KEY_Delay10ms(void);

/****************************************************************************
* Function Name  : KEY_Config
* Description    : ��ʼ���������õ�IO��
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void KEY_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* ����GPIOʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    /*  ����GPIO��ģʽ��IO�� */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_KEY;        //ѡ����Ҫ���õ�IO��
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //���ô�������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;      //������������ģʽ

    /* ��ʼ��GPIO */
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    	          
    /* PA0 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;          //ѡ����Ҫ���õ�IO��
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;      //������������ģʽ

    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

#ifdef USE_EXTI
/****************************************************************************
* Function Name  : KEY_NVIC_Config
* Description    : ��ʼ���ⲿ�ж�
* Input          : key��ѡ��ʹ���ⲿ�жϵİ���
* Output         : None
* Return         : None
****************************************************************************/

void KEY_NVIC_Config(uint8_t key)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* ��ʱ��ʹ�� */
    if(key & KEY_UP)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    }
    if(key & (KEY_DOWN | KEY_LEFT | KEY_RIGHT))
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);    
    }
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    /* ����GPIO���� */    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //���ô�������
    if(key & KEY_UP)
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;      //������������ģʽ
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;          //ѡ����Ҫ���õ�IO��
    	/*��ʼ��GPIO*/
    	GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    if(key & (KEY_DOWN | KEY_LEFT | KEY_RIGHT))
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;      //������������ģʽ
        if(key & KEY_LEFT)
        {
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;          //ѡ����Ҫ���õ�IO��
        }
        if(key & KEY_DOWN)
        {
            GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_3;          //ѡ����Ҫ���õ�IO��
        }
        if(key & KEY_RIGHT)
        {
            GPIO_InitStructure.GPIO_Pin |= GPIO_Pin_4;          //ѡ����Ҫ���õ�IO��
        }
    	/*��ʼ��GPIO*/
    	GPIO_Init(GPIOA, &GPIO_InitStructure);        
    }

/***************************************************************************/
/**************************** �ж����� *************************************/
/***************************************************************************/

	/* ����NVIC������ע��һ��ֻ�ܿ���һλ�����ܼ�λ���һ���*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//������ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;       //���ô����ȼ�Ϊ0��	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		     //ʹ�ܡ�
    
    if(key & KEY_UP)
    {
        NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;         //�������ⲿ�ж�0ͨ���ж�ʹ��
        NVIC_Init(&NVIC_InitStructure);
    }
    if(key & KEY_LEFT)
    {
        NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;         //�������ⲿ�ж�0ͨ���ж�ʹ��
        NVIC_Init(&NVIC_InitStructure); 
    }
    if(key & KEY_DOWN)
    {
        NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;         //�������ⲿ�ж�0ͨ���ж�ʹ��
        NVIC_Init(&NVIC_InitStructure); 
    }
    if(key & KEY_RIGHT)
    {
        NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;         //�������ⲿ�ж�0ͨ���ж�ʹ��
        NVIC_Init(&NVIC_InitStructure); 
    }

	/* ѡ��EXTI */
    if(key & KEY_UP)
    {
	    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); //ѡ��KEY_UP���ⲿ�ж�
    }
    if(key & KEY_LEFT)
    {
	    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource2); //ѡ��KEY_UP���ⲿ�ж�
    }
    if(key & KEY_DOWN)
    {
	    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource3); //ѡ��KEY_UP���ⲿ�ж�
    }
    if(key & KEY_RIGHT)
    {
	    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4); //ѡ��KEY_UP���ⲿ�ж�
    }

    /* �����ⲿ�жϵ�ģʽ */    
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;			    //��ʹ��
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     //�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //�����ش���

    if(key & KEY_UP)
    {
	    EXTI_InitStructure.EXTI_Line = EXTI_Line0;			    //���ô��ⲿ�ж�ͨ��
        /*��ʼ��EXTI*/
	    EXTI_Init(&EXTI_InitStructure);
	}
    if(key & (KEY_DOWN | KEY_LEFT | KEY_RIGHT))
    {
        if(key & KEY_LEFT)
        {
            EXTI_InitStructure.EXTI_Line |= EXTI_Line2;
        }
        if(key & KEY_DOWN)
        {
            EXTI_InitStructure.EXTI_Line |= EXTI_Line3;
        }
        if(key & KEY_RIGHT)
        {
            EXTI_InitStructure.EXTI_Line |= EXTI_Line4;
        }
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //�½��ش���
        /*��ʼ��EXTI*/
    	EXTI_Init(&EXTI_InitStructure);
    }
}

/****************************************************************************
* Function Name  : EXTI0_IRQHandler
* Description    : �ⲿ�ж�0���жϺ���
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void EXTI0_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line0))
	{
        printf(" KEY_UP�����жϣ�\n");
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}


/****************************************************************************
* Function Name  : EXTI2_IRQHandler
* Description    : �ⲿ�ж�2���жϺ���
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void EXTI2_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line2))
	{
        printf(" KEY_LEFT�����жϣ�\n");
	}
	EXTI_ClearITPendingBit(EXTI_Line2);
}

/****************************************************************************
* Function Name  : EXTI3_IRQHandler
* Description    : �ⲿ�ж�3���жϺ���
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void EXTI3_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line3))
	{
        printf(" KEY_DOWN�����жϣ�\n");
	}
	EXTI_ClearITPendingBit(EXTI_Line3);
}

/****************************************************************************
* Function Name  : EXTI4_IRQHandler
* Description    : �ⲿ�ж�4���жϺ���
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void EXTI4_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line4))
	{
        printf(" KEY_RIGHT�����жϣ�\n");
	}
	EXTI_ClearITPendingBit(EXTI_Line4);
}

#endif

/****************************************************************************
* Function Name  : KEY_Delay10ms
* Description    : ����ʹ�õ�������ʱ������ע�⣺����ʱ����������ʱʱ���ǲ�ȷ
*                * ���ģ�������ľ���ʱ10ms��Ҫ��ȷ��ʱ��ʹ�ö�ʱ����
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

static void KEY_Delay10ms(void)
{
    uint16_t i;
    for(i=0; i<0x5FFF; i++);
}

/****************************************************************************
* Function Name  : KEY_Scan
* Description    : ����ɨ�裬ע��ð���ɨ��֧��һ�ΰ�1��������֧��ͬʱ�������
* Input          : None
* Output         : None
* Return         : keyValue�������ļ�ֵ
****************************************************************************/

uint8_t KEY_Scan(void)
{
    uint8_t keyValue = 0, timeCount = 0;

    if((KEY0 == 1) || (KEY1 == 0) || (KEY2 == 0) || (KEY3 == 0)) //����Ƿ��а�������
    {
        KEY_Delay10ms();                                      //��ʱ����

        /* ������ĸ��������� */
        if(KEY0 == 1)
        {
            keyValue = KEY_UP;
        }
        else if(KEY1 == 0)
        {
            keyValue = KEY_LEFT;
        }
        else if(KEY2 == 0)
        {
            keyValue = KEY_DOWN;
        }
        else if(KEY3 == 0)
        {
            keyValue = KEY_RIGHT;
        }
        else
        {
            keyValue = 0;
        }
        
        /* �а�������ʱ�������ּ�� */
        if(keyValue != 0)
        {
            while(((KEY0 == 1) || (KEY1 == 0) || (KEY2 == 0) || (KEY3 == 0)) && (timeCount < 150))
            {
                KEY_Delay10ms();
                timeCount++;        
            }
//            KEY_Delay10ms();//�����������г�����٣�����ɨ����ٶ�̫�죬��һ�����������������   
        }        
    }

    return keyValue;
}












