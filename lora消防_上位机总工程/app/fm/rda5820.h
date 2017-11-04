#ifndef __RDA5820_H
#define __RDA5820_H

#include "stm32f10x.h"
/* RDA5820��I2C��ַ */
#define RDA5820_ADDR	 0x22   //RDA5820��������ַ

/* ģʽ���嶨�� */
#define FM_TX_MODE       0x01    //����ģʽ
#define FM_RX_MODE       0x00    //����ģʽ
#define FM_PA_MODE       0x08    //
#define FM_I2S_DAC_MODE  0x0C

/* RDA5820������� */
#define RDA5820_R00		0x00  //����16��λ��ID =0X5820
#define RDA5820_R02		0x02  //DHIZ[15],DMUTE[14],MONO[13],BASS[12],SEEKUP[9],SEEK[8],SKMODE[7],CLK_MODE[6:4],ANTENNA[2],SOFTRESET[1],ENABLE[0]
#define RDA5820_R03		0x03  //CHAN[15:6],TUNE[4],BAND[3:2],SPACE[1:0]
#define RDA5820_R04		0x04  //STCIEN[14](��������ж�),DE[11],I2S_ENABLE[6],GPIO3~1[5:0]
#define RDA5820_R05		0x05  //INT_MODE[15],SEEKTH[14:8](�趨�ź�ǿ��),LNA_PORT_SEL[7:6],LNA_ICSEL_BIT[5:4],VOLUME[3:0];
#define RDA5820_R0A		0x0A  //STC[14](������ɱ�־),SF[13](����ʧ�ܱ�־),ST[10](��������־),READCHAN[9:0](Ƶ��).
#define RDA5820_R0B		0x0B  //RSSI[15:9](�ź�ǿ��),FM_TRUE[8](վ���־),FM_READY[7](fm������־)
#define RDA5820_R40		0x40  //AUTO_SEEK[15](ȫ/���Զ���̨),CHAN_SCAN[14](�ѿ�/æ̨),CHIP_FUN[3:0](����ģʽ)
#define RDA5820_R41		0x41  //MEM_CLR[15](�ڴ����),RPT_TIME[14:9](RDSӦ�����),MEM_DEPTH[8:0](FIFO��С,���256)
#define RDA5820_R42		0x42  //TX_SIG_GAIN[10:8],TX_PA_GAIN[5:0](���书��).
#define RDA5820_R4A		0x4A  //MEM_INTMOD[4:0]
#define RDA5820_R4B		0x4B  //CHAN_NUM[7:0](����ȫ�Զ���̨��Ч,�����̨����).
#define RDA5820_R4C		0x4C  //TX_AUDIO_DEV[15:0]
#define RDA5820_R4D		0x4D  //TX_PILOT_DEV[15:0]
#define RDA5820_R4E		0x4E  //TX_RDS_DEV[15:0]
#define RDA5820_R53		0x53  //CHAN_BOTTON[10:0](ͨ�����Ƶ��,100kHZΪ��λ)
#define RDA5820_R54		0x54  //CHAN_TOP[10:0](ͨ�����Ƶ��,100kHZΪ��λ)
#define RDA5820_R64		0x64  //RDS_RXMOD[4:3](FIFOģʽ[10]���߼Ĵ���ģʽ[11])		 



uint8_t RDA5820_Init(void);
void RDA5820_SetMode(uint8_t mode);
void RDA5820_SetVol(uint8_t vol);
void RDA5820_SetBand(uint8_t band);
void RDA5820_SetSpace(uint8_t space);
void RDA5820_SetTxPGA(uint8_t gain);
void RDA5820_SetTxPAG(uint8_t gain);
void RDA5820_SetSeekth(uint8_t seekth);

uint8_t RDA5820_GetVol(void);
uint8_t RDA5820_GetRssi(void);
void RDA5820_SetFrequency(uint16_t freq);
uint16_t  RDA5920_GetFrequency(void);


















#endif
