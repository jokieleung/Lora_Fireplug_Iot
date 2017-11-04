#include"bmp.h"
#include"gui.h"
#include"ff.h"

/****************************************************************************
* Function Name  : BMP_ReadHeader
* Description    : ����ȡ�������麯��ת��λBPM�ļ���Ϣ�ṹ�����͡��������ڴ�
*                * ��������Ĵ洢��ʽ��ṹ�岻ͬ������Ҫת��������SD��ȡ����
*                * �ļ���Ϣ��С��ģʽ����λ�ǵ��ֽڣ���λ�Ǹ��ֽڣ������ǳ���
*                * �������෴���Խ�����ת��������
* Input          : header��Ҫת��������
*                * bmp��ת���ɵĽṹ��
* Output         : None
* Return         : None
****************************************************************************/

void BMP_ReadHeader(uint8_t *header, BMP_HeaderTypeDef *bmp)
{

	bmp->fileHeader.bfType = ((*header) << 8) | (*(header + 1));
	header += 2;
	
	bmp->fileHeader.bfSize = ((*(header + 3)) << 24) | ((*(header + 2)) << 16) |
	                         ((*(header + 1)) << 8) | (*header);
	header += 8;

	bmp->fileHeader.bfOffBits = ((*(header + 3)) << 24) | ((*(header + 2)) << 16) |
	                            ((*(header + 1)) << 8) | (*header);
	header += 4;

	bmp->infoHeader.bitSize = ((*(header + 3)) << 24) | ((*(header + 2)) << 16) |
	                          ((*(header + 1)) << 8) | (*header);
	header += 4;

	bmp->infoHeader.biWidth = ((*(header + 3)) << 24) | ((*(header + 2)) << 16) |
	                          ((*(header + 1)) << 8) | (*header);
	header += 4;

	bmp->infoHeader.biHeight = ((*(header + 3)) << 24) | ((*(header + 2)) << 16) |
	                           ((*(header + 1)) << 8) | (*header);
	header += 6;

	bmp->infoHeader.biBitCount = ((*(header + 1)) << 8) | (*header);
	                         
	header += 2;

	bmp->infoHeader.biCompression = ((*(header + 3)) << 24) | ((*(header + 2)) << 16) |
	                                ((*(header + 1)) << 8) | (*header);
	header += 4;

	bmp->infoHeader.biSizeImage = ((*(header + 3)) << 24) | ((*(header + 2)) << 16) |
	                              ((*(header + 1)) << 8) | (*header);
	header += 4;

	bmp->infoHeader.biXPelsPerMeter = ((*(header + 3)) << 24) | ((*(header + 2)) << 16) |
	                                  ((*(header + 1)) << 8) | (*header);
	header += 4;

	bmp->infoHeader.biYPelsPerMeter = ((*(header + 3)) << 24) | ((*(header + 2)) << 16) |
	                                  ((*(header + 1)) << 8) | (*header);
}

/****************************************************************************
* Function Name  : BMP_ShowPicture
* Description    : ��ʾBMP��ʽ��ͼƬ
* Input          : dir��Ҫ��ʾ��ͼƬ·��������
* Output         : None
* Return         : None
****************************************************************************/

void BMP_ShowPicture(uint8_t *dir)
{
	FRESULT res;
	FIL fsrc;
	UINT  br;

	uint8_t buffer[250], rgb;
	BMP_HeaderTypeDef bmpHeader;

	uint16_t a, x, y, color, width, xCount;
	float xRatio, yRatio;

	/* ����Ļˢ��ɫ */
	TFT_ClearScreen(0x0000);
    
	/* ��Ҫ��ȡ���ļ� */
	res = f_open(&fsrc, (const TCHAR*)dir, FA_READ);
		
	if(res == FR_OK)   //�򿪳ɹ�
    { 
		/* ��ȡBMP�ļ����ļ���Ϣ */
        res = f_read(&fsrc, buffer, sizeof(buffer), &br);
		
		/* ��������������ݷ��뵽�ṹ�����У�������� */
		BMP_ReadHeader(buffer, &bmpHeader);

		/* �ж�ͼƬ�Ĵ�С����TFT�Ĵ�С�Ļ���������С */
		if(((TFT_XMAX + 1) < bmpHeader.infoHeader.biWidth) ||
		   ((TFT_YMAX + 1) < bmpHeader.infoHeader.biHeight))
		{
			/* �����С���� */
			xRatio = (float)(TFT_XMAX + 1) / (float)bmpHeader.infoHeader.biWidth;
			yRatio = (float)(TFT_YMAX + 1) / (float)bmpHeader.infoHeader.biHeight;
	
	        /* ���ֻ��һ�߳���TFT���Ĵ�С�Ļ������������ֲ�������С */
			if(xRatio > 1)
			{
				xRatio = 1;
			}
			if(yRatio > 1)
			{
				yRatio = 1;
			}
		}
		else //���ͼƬ��СС��TFT��
		{
			xRatio = 1;
			yRatio = 1;
		}

		/* BMPͼƬ�Ŀ��������ݱ�����4�ı��������������ô���䲹�� */
		/* �����������ж�BMPͼƬ�ĺ�������ת���ݡ� */
		if(bmpHeader.infoHeader.biWidth % 4)
		{
			width = bmpHeader.infoHeader.biWidth * 3 / 4;
			width += 1;
			width *= 4;	
		}		
		else
		{
			width = bmpHeader.infoHeader.biWidth * 3;	
		}

		/* ��ʼ��Ӧ�õ�ֵ */
		x = 0; 
		y = 0;
		rgb = 0;
		xCount = 0;
		a = bmpHeader.fileHeader.bfOffBits;    //ȥ���ļ���Ϣ�ſ�ʼ����������
     	while(1)  
     	{      
			/* SD����ȡһ�����ݵĳ��� */
      	  	while(a < 250)
			{				
				/* ����ȡ����24λɫת��Ϊ16λɫ */
				switch (rgb) 
				{
					case 0:				  
						color = buffer[a] >> 3; //B
						break ;	   
					case 1: 	 
						color += ((uint16_t)buffer[a] << 3) & 0X07E0;//G
						break;	  
					case 2 : 
						color += ((uint16_t)buffer[a] << 8) & 0XF800;//R	  
						break ;
                    default:
                        break;			
				}
				a++;
				rgb++;

				/* �����ȡ��һ�����ص㣬��д��TFT������ */
				if(rgb == 3)
				{
				    /* ����Ҫд��ĵ� */
					TFT_SetWindow(((float)x * xRatio + 0.5), ((float)y * yRatio + 0.5), 
					              ((float)x * xRatio + 0.5), ((float)y * yRatio + 0.5));
					TFT_WriteData(color);
					rgb =0;
					x++;       //X����+1					
				}

				/* ����һ����ȡ�˶�������ֵ */
				xCount++;
				if(xCount >= width)	   //�������һ�е������ˣ���ô������ʾ
				{	
					xCount = 0;
					x = 0;
					y++;
				}				
			}

			/* ������ȡͼƬ���� */
			res = f_read(&fsrc, buffer, sizeof(buffer), &br);
			a = 0;

			/* �ж��ַ��ȡ��ᣬ���������ѭ�� */
            if (res || br < sizeof(buffer))
			{
				break;    // error or eof 
			}

        } 
    }
	 
    f_close(&fsrc);  //�����Ǵ򿪣������½��ļ���һ���ǵùر�
}
