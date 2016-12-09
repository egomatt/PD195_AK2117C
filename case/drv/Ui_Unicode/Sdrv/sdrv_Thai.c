/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_Thai.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00    
* Description: support unicode font
********************************************************************************
*/
#include "Drv_S7565.h"
#include "display.h"
#include "string.h"


#pragma renamecode(?PR?BBC_Thai)
 
/*
*******************************************************************************
** FUNCTION:    Sdrv_FontCodeMBToUni
**
** PURPOSE:     ��������������ַ�ת��ΪUnicode
**
** PARAMETERS:
**  input
**      de=����    
**  output
**      de= unicode ����
** DESCRIPTION:
****************************************************************************
*/
uint16 Sdrv_FontCodeMBToUni(uint16 ucode)
{
	uint16  UniCode;
	SD_RW sdrwfont;
	//�����ASCII�Ĳ�ͬ����
	if (ucode>= 0x80)
	{
		/*����ж�����Ե�ת��*/
		sdrwfont.faddr=MBToUniTabAddr+(((uint32)ucode)<<1);
		sdrwfont.srampage=0x00;
		sdrwfont.sramaddr=(uint16)&UniCode;
		SD_Read(&sdrwfont,2);
		Sdrv_SwapIntByte(&UniCode);
	}
	else
	{//ASCII
		UniCode =  ucode;
	}
	return  UniCode;
}

/*
*******************************************************************************
** FUNCTION:    Sdrv_ThaiOverlap
**
** PURPOSE:     �����ǰ��̩���ַ�����ͽ����ŵ�̩���ַ��ص�����ô�����ص�����
**
** PARAMETERS:
**  input
**      str: ��ǰ�ַ�ָ��,�Է��ʽ��Ӻ�����ַ�
**      IsUnicode: �ַ����Ƿ�ʹ��Unicode����
**  output
**      char: �����ص����ַ���
** DESCRIPTION:
****************************************************************************
*/
int8 Sdrv_ThaiOverlap(int8 * str, int8 IsUnicode)
{
	int8 width1,width2;
	int8 i,j,k;
	uint16 Thai_code;
	int8 ScanLen=0;
	
	while(1)
	{
		Thai_code = (IsUnicode? (*(uint16 *)(str+ScanLen)): (*(str+ScanLen)));
		if(IsUnicode)
		{
		    Sdrv_SwapIntByte(&Thai_code);
		}
		if(Thai_code==0x00)
			break;
		//����ж�����Ե�ת��
		if(!IsUnicode)
			Thai_code = Sdrv_FontCodeMBToUni(Thai_code);
		if((Thai_code==0x0e31)||((Thai_code<=0x0e3A)&&(Thai_code>=0x0e34))
		        ||((Thai_code<=0x0e4E)&&(Thai_code>=0x0e47)))
		{
			ScanLen += (IsUnicode? 2: 1);
			width1= CharPointBuffer[32];
			if(width1<=8)
			{
				width1 =UNICODE_WIDTH_DEF/2;
			}
			else
			{
				width1 =UNICODE_WIDTH_DEF;
			}
			
			//Sdrv_GetFontPoint(Thai_code,(unsigned int)AuxiliaryBuffer);
			BAPI_GetFontPoint(Thai_code, (uint16)AuxiliaryBuffer);
			width2 = AuxiliaryBuffer[32];
			if(width2<=8)
			{
				width2 =UNICODE_WIDTH_DEF/2;
			}
			else
			{
				width2 =UNICODE_WIDTH_DEF;
			}
					
			for(i=0,j=0,k=0; i<width2; i++)
			{
			    if(AuxiliaryBuffer[i]==0xff)
			    {
			        AuxiliaryBuffer[i] = 0;
			        AuxiliaryBuffer[i+width2] = 0;
			        if(k==1)
			        {
			            k = i+1;
			        }    
			        continue;
			    }    
			    else
			    {
			        k = 1;
                }
			}
			AuxiliaryBuffer[32]=k;
			
			if(width1>=width2)
			{
				for(i=0;i<AuxiliaryBuffer[32];i++)
				{
					CharPointBuffer[i]|=AuxiliaryBuffer[i];
					CharPointBuffer[i+width1]|=AuxiliaryBuffer[i+width2];
				}
			}
			else
			{
				for(i=0;i<CharPointBuffer[32];i++)
				{
					AuxiliaryBuffer[i]|=CharPointBuffer[i];
					AuxiliaryBuffer[i+width2]|=CharPointBuffer[i+width1];
				}
				memcpy(CharPointBuffer,AuxiliaryBuffer,33);
			}
		}
		else
		{
			break;
		}
	}
	
	return ScanLen;
}

