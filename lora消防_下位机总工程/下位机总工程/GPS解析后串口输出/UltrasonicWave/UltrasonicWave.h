#ifndef __UltrasonicWave_H
#define	__UltrasonicWave_H
#include "stm32f10x.h"
void UltrasonicWave_Configuration(void);               //�Գ�����ģ���ʼ��
void UltrasonicWave_StartMeasure(void);                //��ʼ��࣬����һ��>10us�����壬Ȼ��������صĸߵ�ƽʱ��


u8 Get_Distance(void);
void Distance_tx_lora(u8 distance);

#endif /* __UltrasonicWave_H */

