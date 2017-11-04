#include "mp3player.h"
#include "ff.h"
#include "vs10xx.h"
#include "hc4052.h"
#include "spi.h"

/****************************************************************************
* Function Name  : MP3_Init
* Description    : ��ʼ��MP3
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

int8_t MP3_Init(void)
{
    uint16_t id;
	VS10XX_Config();
    id = VS10XX_RAM_Test();	
	if(id != 0x83FF)
	{	
		return 0xFF;
	}
	
	HC4052_Init();
	VS10XX_SineTest();
	HC4052_OutputSelect(AUDIO_MP3_CHANNEL);
	MP3_BaseSetting(0,0,0,0);
	MP3_EffectSetting(0);

	return 0;
}

/****************************************************************************
* Function Name  : MP3_AudioSetting
* Description    : ���������Ĵ�С������SCI_VOL�Ĵ���
* Input          : vol�������Ĵ�С��0~0xFF��
* Output         : None
* Return         : None
****************************************************************************/

void MP3_AudioSetting(uint8_t vol)
{
	uint16_t volValue = 0;
	
	/* 0�����������0xFE����������8�ֽڿ�����ͨ������8�ֽڿ�����ͨ�� */
	vol = 254 - vol;
	volValue = vol | (vol << 8);

	VS10XX_WriteCmd(SCI_VOL, volValue);
}

/****************************************************************************
* Function Name  : MP3_BaseSetting
* Description    : �������ã�����SCI_BASS�Ĵ���
* Input          : amplitudeH����Ƶ���� 0~15(��λ:1.5dB,С��9��ʱ��Ϊ����)
*                * freqLimitH����Ƶ���� 2~15(��λ:10Hz)
*                * amplitudeL����Ƶ���� 0~15(��λ:1dB)
*                * freqLimitL����Ƶ���� 1~15(��λ:1Khz)
* Output         : None
* Return         : None
****************************************************************************/

void MP3_BaseSetting(
	uint8_t amplitudeH, uint8_t freqLimitH,
	uint8_t amplitudeL, uint8_t freqLimitL
)
{
	uint16_t bassValue = 0;
	
	/* ��Ƶ������12 ��15λ */
	bassValue = amplitudeH & 0x0F;
	bassValue <<= 4;
	
	/* Ƶ�������� 11 ��8λ */
	bassValue |= freqLimitL & 0x0F;
	bassValue <<= 4;
	
	/* ��Ƶ������ 7 ��4 λ */
	bassValue |= amplitudeL & 0x0F;
	bassValue <<= 4;
	
	/* Ƶ�������� 3 �� 0λ */
	bassValue |= freqLimitH & 0x0F;
	
	VS10XX_WriteCmd(SCI_BASS, bassValue); 
		
}

/****************************************************************************
* Function Name  : MP3_EffectSetting
* Description    : ��Ч���á�����SCI_MODE�Ĵ���
* Input          : effect��0,�ر�;1,��С;2,�е�;3,���
* Output         : None
* Return         : None
****************************************************************************/

void MP3_EffectSetting(uint8_t effect)
{
	uint16_t effectValue;

	effectValue = VS10XX_ReadData(SCI_MODE);
	if(effect & 0x01)
	{
		effectValue |= 1 << 4;	
	}
	else
	{
		effectValue &= ~(1 << 5);
	}
	if(effect &0x02)
	{
		effectValue |= 1 << 7;
	}
	else
	{
		effectValue &= ~(1 << 7);
	}

	VS10XX_WriteCmd(SCI_MODE, effectValue);
}

/****************************************************************************
* Function Name  : MP3_PlaySong
* Description    : ����һ�׸���
* Input          : addr�����ŵ�ַ�͸������������ǵü�.mp3��׺��
* Output         : None
* Return         : None
****************************************************************************/

void MP3_PlaySong(uint8_t *addr)
{
	FIL file;
	UINT br;
	FRESULT res;
	uint8_t i, musicBuff[512];
	uint16_t k;

    /*open file*/
	res = f_open(&file, (const TCHAR*)addr, FA_READ);
 	
	VS10XX_SoftReset();
	if(res == FR_OK)
	{
		SPI1_SetSpeed(SPI_BaudRatePrescaler_8);
		while(1)
		{
			res = f_read(&file, musicBuff, sizeof(musicBuff), &br);
			k = 0;

			do
			{	
				if(VS_DREQ != 0) //VS����
				{
					VS_XDCS_CLR;
					for(i=0; i<32; i++)
					{
						SPI1_WriteReadData(musicBuff[k++]);
					}
					VS_XDCS_SET;
				}
			}
			while(k < br);
			
			if (res || (br == 0))
			{
				break;    // error or eof 
			} 
		}
		f_close(&file);  //�����Ǵ򿪣������½��ļ���һ���ǵùر�
	}		
}

