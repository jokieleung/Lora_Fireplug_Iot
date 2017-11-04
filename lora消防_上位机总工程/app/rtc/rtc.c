#include "rtc.h"

/* �·�   1  2  3  4  5  6  7  8  9  10 11 12 */
/* ����   31 29 31 30 31 30 31 31 30 31 30 31 */
/* ������ 31 28 31 30 31 30 31 31 30 31 30 31 */
const uint8_t RtcLeapMonth[12]   = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const uint8_t RtcCommonMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/* ����һ��ȫ�ֱ�������ʱ�� */
RTC_TimeTypeDef RTC_Time;

/* �����ڲ����� */
static uint8_t RTC_CheckLeapYear(uint16_t year);
static uint8_t RTC_SetTime(RTC_TimeTypeDef *time);
static void RTC_NVIC_Config(void);
static void RTC_GetTime(void);

/****************************************************************************
* Function Name  : RTC_SetClock
* Description    : ����ʱ��
* Input          : *time��Ҫ���õ�ʱ��ֵ
* Output         : None
* Return         : None
****************************************************************************/

void RTC_SetClock(RTC_TimeTypeDef *time)
{   
    RTC_EnterConfigMode();                    //��������	
    RTC_WaitForLastTask();	                  //�ȴ����һ�ζ�RTC�Ĵ�����д�������
    RTC_SetTime(time);                        //����ʱ��	
    RTC_ExitConfigMode();                     //�˳�����ģʽ  
    RTC_GetTime();         //����ʱ��
}

/****************************************************************************
* Function Name  : RTC_Config
* Description    : ��ʼ��ʱ�ӣ�����ʼ���ڲ���ʱ����Ϣ
* Input          : time��Ҫ��ʼ����ʱ��
* Output         : None
* Return         : 0����ʼ���ɹ���0xFF����ʼ��ʧ��
****************************************************************************/

int8_t RTC_Config(RTC_TimeTypeDef *time)
{
    uint32_t timeCount;

    if(BKP_ReadBackupRegister(BKP_DR1) != 0x5050)
    {
        /* ʹ��PWR��Դʱ�Ӻ�BKP������������ʱ�� */   
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

        PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ������� 
		BKP_DeInit();	                //��λ�������� 	
		RCC_LSEConfig(RCC_LSE_ON);	    //�����ⲿ���پ���(LSE),ʹ��������پ���

        /* ���ָ����RCC��־λ������񣬵ȴ����پ���LSE������ */
    	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
		{
    		timeCount++;
            if(timeCount > 0x00FFFFF)
            {
                break;
            }
		}

        /* �ⲿ������󣬷�������ʧ�� */
        if(timeCount > 0x00FFFFF)
        {
            return 0xFF;            
        }

        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);   //����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��    
		RCC_RTCCLKCmd(ENABLE);	                  //ʹ��RTCʱ��
        RTC_WaitForLastTask();	                  //�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_WaitForSynchro();		              //�ȴ�RTC�Ĵ���ͬ��  
		RTC_ITConfig(RTC_IT_SEC, ENABLE);		  //ʹ��RTC���ж�
		RTC_WaitForLastTask();              	  //�ȴ����һ�ζ�RTC�Ĵ�����д�������

		RTC_EnterConfigMode();                    //��������	
		RTC_SetPrescaler(32767);                  //����RTCԤ��Ƶ��ֵ
		RTC_WaitForLastTask();	                  //�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_SetTime(time);                        //����ʱ��	
		RTC_ExitConfigMode();                     //�˳�����ģʽ  
		BKP_WriteBackupRegister(BKP_DR1, 0X5050); //��ָ���ĺ󱸼Ĵ�����д���û��������� 
    }
    else
    {
        RTC_WaitForSynchro();	           //�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_ITConfig(RTC_IT_SEC, ENABLE);  //ʹ��RTC���ж�
		RTC_WaitForLastTask();	           //�ȴ����һ�ζ�RTC�Ĵ�����д�������
    }

    RTC_NVIC_Config();     //RCT�жϷ������ã������ж�		    				     
	RTC_GetTime();         //����ʱ��

    return 0;
}

/****************************************************************************
* Function Name  : RTC_IRQHandler
* Description    : RTCʱ�ӵ��жϺ�������������ʱ�䣬��������
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void RTC_IRQHandler(void)
{		 
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)     //�����ж�
	{							
		RTC_GetTime();                            //����ʱ��   
 	}

	if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)       //�����ж�
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);	      //�������ж�	  	   
  	}
     				  								 
	RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);  //�������ж�
	RTC_WaitForLastTask();	  	    			  //�ȴ����һ�ζ�RTC�Ĵ�����д�������			 	   	 
}

/****************************************************************************
* Function Name  : RTC_NVIC_Config
* Description    : RTC�жϵ�����
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

static void RTC_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		        //RTCȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	        //��Ӧ���ȼ�����
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		        //ʹ�ܸ�ͨ���ж�

	NVIC_Init(&NVIC_InitStructure);   //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}

/****************************************************************************
* Function Name  : RTC_SetTime
* Description    : ����RTCʱ�ӵļ�������ʼֵ
* Input          : time�����õĳ�ʼֵ��ע��������ô�2000��2100��֮�䣩
* Output         : None
* Return         : 0�����óɹ���0xFF������ʧ��
****************************************************************************/

static uint8_t RTC_SetTime(RTC_TimeTypeDef *time)
{
    uint8_t  leapYear = 0;
    uint16_t i;
    uint32_t secondCount = 0;
    
    /* ȷ��д���ʱ�䲻�������� */
    if((time->year < 2000) || (time->year > 2100)) //��2000�굽2100�꣬һ��100��
    {
        return 0xFF;             //����ʱ�޷���ʧ��
    }
   
    /* �����е����������� */
    for(i = RTC_BASE_YEAR; i<time->year; i++)
    {
        if(RTC_CheckLeapYear(i) == 0)          //������������
        {
            secondCount += RTC_LEEP_YEAR_SECOND;     
        }
        else
        {
            secondCount += RTC_COMMON_YEAR_SECOND;    
        }    
    }
    
    
    /* ���д����������껹��ƽ�� */
    if(RTC_CheckLeapYear(time->year) == 0) //���������
    {
        leapYear = 1;                     //���Ϊ����            
    }
    else
    {
        leapYear = 0;                     //���Ϊƽ��    
    }
    /* �����·�������� */
    for(i=1; i<time->month; i++)
    {
        if(leapYear == 1)
        {
            secondCount += RtcLeapMonth[i - 1] * RTC_DAY_SECOND;     
        }
        else
        {
            secondCount += RtcCommonMonth[i - 1] * RTC_DAY_SECOND;
        }       
    }
    
    /* ���е������������ */
    for(i=1; i<time->day; i++)
    {
        secondCount += RTC_DAY_SECOND;
    }
    
    /* Сʱ������ */
    secondCount += RTC_HOUR_SECOND * time->hour;
    
    /* ���ӵ����� */
    secondCount += 60 * time->minit;
    
    /* �������� */
    secondCount += time->second; 

    /* ʹ��PWR��Դʱ�Ӻ�BKP������������ʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);	  //ʹ��RTC�ͺ󱸼Ĵ������� 
	RTC_SetCounter(secondCount);	  //����RTC��������ֵ 
    
    RTC_WaitForLastTask();	          //�ȴ����һ�ζ�RTC�Ĵ�����д�������  	

	return 0;                         //���óɹ�����0
}

/****************************************************************************
* Function Name  : RTC_CheckLeapYear
* Description    : �������Ƿ�������
* Input          : year���������
* Output         : None
* Return         : 0�������ꣻ0xFF����ƽ��
****************************************************************************/

static uint8_t RTC_CheckLeapYear(uint16_t year)
{
    /* ���������ּ��㷽ʽ����һ�����ܱ��������Ҳ��ܱ�100������ */
    /* �ڶ������ܱ�100�������ܱ�400���� */

    if((year % 100) == 0) //�ܱ�100�����ģ����ܱ�400����������
    {
        if((year % 400) == 0)
        {
            return 0;     //������ 
        }
        else
        {
            return 0xFF;  //��ƽ��
        }   
    }
    else                  //���ܱ�100���������ܱ�4����������
    {
        if((year % 4) == 0)
        {
            return 0;     //������
        }
        else
        {
            return 0xFF;  //��ƽ��
        }
    }
}

/****************************************************************************
* Function Name  : RTC_GetTime
* Description    : ��ȡRTC��������ֵ��������ת��Ϊ����
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/
 
static void RTC_GetTime(void)
{
    uint8_t leapYear = 0, i = 0;
    uint32_t secondCount = 0;
    uint32_t day;

    /* ��ȡʱ�Ӽ�������ֵ */
    secondCount =  RTC->CNTH;
    secondCount <<= 16;
    secondCount |= RTC->CNTL;
    
    day = secondCount / RTC_DAY_SECOND;           //�������
    secondCount = secondCount % RTC_DAY_SECOND;   //���ʣ������

    RTC_Time.year = RTC_BASE_YEAR;

    /* ������ڼ� */
    RTC_Time.week = (day + 6) % 7;         //��Ϊ2000��1��1�������������Լ�6

    /* ������ */
    while(day >= 365)
    {
        if(RTC_CheckLeapYear(RTC_Time.year) == 0)   //������
        {
            day -= 366;                    //������366��    
        }
        else
        {
            day -= 365;                    //ƽ����365��
        }

        RTC_Time.year++;
    }

    /* ����·� */
    if(RTC_CheckLeapYear(RTC_Time.year) == 0)
    {
        leapYear = 1;                        //�����������
    }

    i = 0;
    RTC_Time.month = 1;
    while(day >= 28)
    {        
        if(leapYear == 1)
        {
            if(day < RtcCommonMonth[i]) //��������һ����
            {
                break;
            }
            day -= RtcLeapMonth[i];     //��ȥ������µ�����
        }
        else
        {
            if(day < RtcCommonMonth[i]) //��������һ����
            {
                break;
            }
            day -= RtcCommonMonth[i];   //��ȥƽ����µ����� 
        }
        RTC_Time.month++;               //�·ݼ�1
        i++;                            //�·������1
    }

    /* ������� */
    RTC_Time.day = day + 1;             //�·�ʣ�µ�������������(���ڴ�1�ſ�ʼ)

    RTC_Time.hour = secondCount / RTC_HOUR_SECOND;       //���Сʱ
    RTC_Time.minit = secondCount % RTC_HOUR_SECOND / 60; //�������
    RTC_Time.second = secondCount % RTC_HOUR_SECOND %60; //�����
 
}






















