#ifndef __RTC_H
#define __RTC_H

#include "stm32f10x.h"

/* ����ʱ�ӵĽṹ�� */
typedef struct{
uint8_t  second;
uint8_t  minit;
uint8_t  hour;
uint8_t  week;
uint8_t  day;
uint8_t  month;
uint16_t year;
}RTC_TimeTypeDef;

/* ����ʱ����Ϣ */
extern RTC_TimeTypeDef RTC_Time;

/* ����ʱ������ */
#define RTC_BASE_YEAR      (uint16_t)2000                  //�����С����Ϊ2000��

#define RTC_LEEP_YEAR_SECOND    (uint32_t)(366 * 24 * 60 * 60)  //��������
#define RTC_COMMON_YEAR_SECOND  (uint32_t)(365 * 24 * 60 * 60)  //ƽ������
#define RTC_DAY_SECOND          (uint32_t)(24 * 60 * 60)        //һ�������
#define RTC_HOUR_SECOND         (uint32_t)(60 * 60)             //һ��Сʱ������



/* ����ȫ�ֺ��� */
int8_t RTC_Config(RTC_TimeTypeDef *time);
void RTC_SetClock(RTC_TimeTypeDef *time);
























#endif

