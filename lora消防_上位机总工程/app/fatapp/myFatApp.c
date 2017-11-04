#include"myfatapp.h"

/****************************************************************************
* Function Name  : FATFS_GetFree
* Description    : ��ȡ�ܴ��̴�С�Ϳ��д��̴�С
* Input          : drv�����̱��
*                * total�������ȡ�����ܴ��̴�С
*                * free�������ȡ���Ŀ��д��̴�С
* Output         : None
* Return         : res����ȡ״̬��0���ɹ�����������������Ӧ�Ĵ�����Ϣ��
****************************************************************************/

uint8_t FATFS_GetFree(uint8_t *drv, uint32_t *total, uint32_t *free)
{
	FATFS *fs1;
	u8 res;
    DWORD fre_clust=0, fre_sect=0, tot_sect=0;

    /* �õ�������Ϣ�����д����� */
    res = f_getfree((const TCHAR*)drv, &fre_clust, &fs1);

    if(res == 0)  //��ʾ��ȡ�ɹ�
	{											   
	    tot_sect = (fs1->n_fatent-2) * fs1->csize;	//�õ���������
	    fre_sect = fre_clust * fs1->csize;			//�õ�����������
			   
#if _MAX_SS!=512		        //������С����512�ֽ�,��ת��Ϊ512�ֽ�
		tot_sect *= fs1->ssize/512;
		fre_sect *= fs1->ssize/512;
#endif	  

		*total = tot_sect >> 1;	//��λΪKB
		*free = fre_sect >> 1;	//��λΪKB 
	}

	return res;	
}

/****************************************************************************
* Function Name  : FATFS_ScanFiles
* Description    : ��ȡָ���ļ��������ļ������ֺͺ�׺
* Input          : dir��ɨ����ļ�·��
*                * file��������������ļ������ֺͺ�׺�ĵ�ַ
* Output         : None
* Return         : res�����ض�ȡ��״̬
****************************************************************************/

uint8_t FATFS_ScanFiles(uint8_t *dir, FileNameTypeDef *file)
{
	FRESULT res;
	FILINFO  fileInfo;
	DIR dirs;
	char *fn, i = 0; 

#if _USE_LFN
    static char lfn[_MAX_LFN + 1];   /* Buffer to store the LFN */
    fileInfo.lfname = lfn;
    fileInfo.lfsize = sizeof(lfn);
#endif
	/* ���ļ��� */
	res = f_opendir(&dirs, (const TCHAR*)dir);
	
	if (res == FR_OK)    //�ɹ����ļ��� 
	{    
		while (f_readdir(&dirs, &fileInfo) == FR_OK)   //����˳����ļ��� 
		{ 	 
			if(!fileInfo.fname[0]) 	  //����ļ���Ϊ0,���� 
			{
				break;     
			} 	
			
			/* �ж��ļ����� */								
			if((fileInfo.fattrib == AM_ARC)|(fileInfo.fattrib == AM_DIR)) 
			{

#if _USE_LFN	//Ϊ���ַ�����׼��
            	fn = *fileInfo.lfname ? fileInfo.lfname : fileInfo.fname;
#else
           	 	fn = fileInfo.fname;
#endif

				/* ��ȡ���� */
				i = 0;
	       		while((*fn != '.') && (*fn != '\0')) //��һ������������ѭ��
				{
					file->name[i] = *fn;
					i++;
					fn++;	
				}

				/* ��ȡ��׺ */
				i = 0;
				while(*fn != '\0')
				{
					file->type[i] = *fn;
					i++;
					fn++;
				} 
				file++;
			}//end of �ļ������ж���
		} //end of while (f_readdir(&dirs, &fileInfo) == FR_OK)//����˳����ļ���
	}
	return res;
}


