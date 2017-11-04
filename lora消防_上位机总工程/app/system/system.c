#include "system.h"

#ifdef USE_UCOSII_OS
#include "os_includes.h"
#endif

/****************************************************************************
* Function Name  : SYSTEM_SetClock
* Description    : �����ⲿʱ����ΪPLL���룬��������Ӧ��ʱ��Ƶ�ʡ�ע��ʱ��Ƶ��
*                * Ҫ��8�ı������ú���ֻ�ṩ32MHZ��72MHZ�����á�
* Input          : freq��Ƶ�ʲ������ò���ֻ������Ϊ8�ı�������4����9����
* Output         : None
* Return         : -1����ʾ����ʧ�ܣ�ʱ�ӷ��س�ʼ��״̬
*                * 0����ʾ��ʼ���ɹ�
****************************************************************************/

int8_t SYSTEM_SetClock(uint8_t freq)
{
    ErrorStatus HSEStartUpStatus;
    uint32_t pllMul;

    /* ѡ��Ƶϵ�� */
    switch(freq)
    {
        case(32):
            pllMul = RCC_PLLMul_4;
            break;
        case(40):
            pllMul = RCC_PLLMul_5;
            break;
        case(48):
            pllMul = RCC_PLLMul_6;
            break;
        case(56):
            pllMul = RCC_PLLMul_7;
            break;
        case(64):
            pllMul = RCC_PLLMul_8;
            break;
        case(72):
            pllMul = RCC_PLLMul_9;
            break;
        default:
            RCC_DeInit();
            return -1;
    }
        
    /* ��λRCC */
    RCC_DeInit();
    
    /* ʹ�ܴ�HSE�ⲿ����ʱ�� */
    RCC_HSEConfig(RCC_HSE_ON);

    /* �ȴ�HSEʱ�Ӿ��� */
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
    
    /* ��HSEʱ�Ӿ��� */
    if (HSEStartUpStatus == SUCCESS)
    {
        /* ʹ�ܿ���FLASH ,�������ȥ����FLASH��������ݣ�Ҫ�ǲ����ó������� */
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        
        /* FLASH����2����ʱ���� */
        FLASH_SetLatency(FLASH_Latency_2); 
       
        /* ����AHB���ߵ�ʱ�ӷ�Ƶ */
        RCC_HCLKConfig(RCC_SYSCLK_Div1);        //����Ƶ
        
        /* ���ø���ʱ�ӵ�ʱ�ӷ�Ƶ�����72MHZ�� */
        RCC_PCLK2Config(RCC_HCLK_Div1);         //����Ƶ
        
        /* ���õ���ʱ�ӵ�ʱ�ӷ�Ƶ�����36MHZ�� */
        RCC_PCLK1Config(RCC_HCLK_Div2);         //����Ƶ
        
        /* ����PLL��ʱ������ͱ�Ƶ�� */
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, pllMul);//ѡ��HSE����Ƶ���룬��ƵpllMul��
        
        /* ���� PLL */ 
        RCC_PLLCmd(ENABLE);
        
        /* �ȴ�PLLʱ�Ӿ��� */
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {
        }

        /* ѡ��PLLʱ����Ϊ���� */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

        /* �ȴ�ϵͳʱ���л���� */
        while(RCC_GetSYSCLKSource() != 0x08)
        {
        }
    }
    else              //��HSEʱ�ӿ�ʼʧ�ܣ���������ʧ��
    {
        return -1;  
    }

    return 0;         //�������óɹ�
}

/****************************************************************************
* Function Name  : NVIC_Config
* Description    : ����NVIC�ж�ϵͳ��ռ���Ⱥʹ����ȵ����ã�ע:������Ϊ3λ��ռ
*                * ���ȣ�1λ�����ȡ���
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void NVIC_Config(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);    
}

#ifndef USE_UCOSII_OS
/****************************************************************************
* Function Name  : SYSTICK_Init
* Description    : ��ʼϵͳ�δ�ʱ���������ö�ʱʱ�䡣
*                * �����жϺ�����stm32f10x_it.c��139�С�
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void SYSTICK_Config(void)
{
    /* ʱ����72MHZʱ�����Լ���72��(���Ϊ16777215)������1us */
	/* ��Ҫ��Ϊ������ʱ�� */
	if(SysTick_Config(72) == 0)//�����ɹ�����0
	{
		/* ����������ú�����ʱ��Ĭ����ֱ�Ӵ򿪼������ģ����ڰ����ص��� */
		/* �ر��ж� */	
		 SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);		 	
	}    
}

/****************************************************************************
* Function Name  : SYSTICK_Delay1us
* Description    : ��ʱ������
* Input          : us����ʱ��ʱ��
* Output         : None
* Return         : None
****************************************************************************/

void SYSTICK_Delay1us(uint16_t us)
{
	uint32_t countValue;

	SysTick->LOAD  = (us * 72) - 1;             //������װ��ֵ, 72MHZʱ
	SysTick->VAL   = 0;	                        //���������
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;	//�򿪼�ʱ��

	do
	{
		countValue = SysTick->CTRL; 	
	}
	while(!(countValue & (1 << 16)));			//�ȴ�ʱ�䵽��

	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//�رռ�����	
}

/****************************************************************************
* Function Name  : SYSTICK_Delay1ms
* Description    : ��ʱ������
* Input          : ms����ʱ��ʱ��
* Output         : None
* Return         : None
****************************************************************************/

void SYSTICK_Delay1ms(uint16_t ms)
{
	uint32_t countValue;

	SysTick->LOAD  = 72000 - 1;                 //������װ��ֵ, 72MHZʱ��ʱ1ms
	SysTick->VAL   = 0;	                        //���������
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;	//�򿪼�ʱ��

	do
	{
		countValue = SysTick->CTRL;
		if(countValue & (1 << 16))              //����1ms�Ǽ�����1
		{
			ms--;
		} 	
	}
	while(ms);			                        //�ȴ�ʱ�䵽��

	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//�رռ�����	
}

/****************************************************************************
* Function Name  : SYSTICK_Delay1s
* Description    : ��ʱ������
* Input          : s����ʱ��ʱ��
* Output         : None
* Return         : None
****************************************************************************/

void SYSTICK_Delay1s(uint16_t s)
{
	uint32_t countValue;

	s *= 5; 
	SysTick->LOAD  = 72000 * 200 - 1;           //������װ��ֵ, 72MHZʱ��ʱ200ms
	SysTick->VAL   = 0;	                        //���������
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;	//�򿪼�ʱ��

	do
	{
		countValue = SysTick->CTRL;
		if(countValue & (1 << 16))              //����200ms�Ǽ�����1
		{
			s--;
		} 	
	}
	while(s);			                        //�ȴ�ʱ�䵽��

	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//�رռ�����	
}

#else


/****************************************************************************
* Function Name  : SYSTICK_Delay1us
* Description    : ��ʱ������
* Input          : us����ʱ��ʱ��
* Output         : None
* Return         : None
****************************************************************************/

void SYSTICK_Delay1us(uint16_t us)
{
    uint32_t oldValue, newValue, ticks, reloadValue, countValue = 0;

    /* ��ȡ��װֵ */
    reloadValue = SysTick->LOAD;
    
    /* ������ʱ��Ҫ���ٸ��δ��������õ���ʱ����ʱ��Ϊ72MHZ�Ļ��������õ� */
    ticks = us * 72;

    oldValue = SysTick->VAL;  //�ս���ʱ�ļ�����ֵ
    while(1)
    {
        newValue = SysTick->VAL;
        if(newValue != oldValue)
        {
            if(newValue < oldValue)    //�δ�������ǵ���������
            {
                countValue += oldValue - newValue;
            }
            else
            {
                countValue += reloadValue - newValue + oldValue;
            }
            if(countValue >= ticks)
            {
                break;
            }    
        }
    }   	
}

/****************************************************************************
* Function Name  : SYSTICK_Delay1ms
* Description    : ��ʱ������
* Input          : ms����ʱ��ʱ��
* Output         : None
* Return         : None
****************************************************************************/

void SYSTICK_Delay1ms(uint16_t ms)
{
    if(OSRunning != 0)
    {
        OSTimeDlyHMSM(0, 0, 0, ms); 
    }
    else
    {
        while(ms--)
        {
            SYSTICK_Delay1us(1000);    
        }    
    }   	
}

/****************************************************************************
* Function Name  : SYSTICK_Delay1s
* Description    : ��ʱ������
* Input          : s����ʱ��ʱ��
* Output         : None
* Return         : None
****************************************************************************/

void SYSTICK_Delay1s(uint16_t s)
{
    OSTimeDlyHMSM(0, 0, s, 0);    	
}

#endif

