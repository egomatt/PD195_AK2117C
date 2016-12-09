/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_charoutrect.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00    
* Description: support unicode font
********************************************************************************
*/
#include "Drv_S7565.h"
#include "display.h"
#include "string.h"


#pragma renamecode(?PR?BBC_DW3)


////*******************************************************************************/
///*
//** FUNCTION:     Sdrv_CharOutRect
//**
//** Description:  ��ָ���������ϴ��һ���ַ�,�������򲿷ֲ���ʾ��û�г�����Ļ�Ĳ��ֽ�������ʾ��
//**
//**  input
//**     str: ��ǰ�ַ�ָ�룬�Է��ʺ����ַ���֧��̩����ʾ
//**     CRregion: Ҫ�����������
//**     colcnt:   Ҫ��Ӻ��ֵ���һ�п�ʼ���
//**     language: �Ƿ�Unicode����
//**  output
//**     char: ��ָ�������������ֵ�������ʱ������Ҫ���Ƶ��ַ��������򷵻�0
//********************************************************************************/

uint8 Sdrv_CharOutRect(uint8 *str, region_t *CRregion, int8 colcnt, uint8 language)
{
//	uint8 charmode;
//	bool putchar_result;
	uint8 fontwidth;
	uint8 ScanLen;
	uint16 ucode;
//	uint16 ThaiCode;
	uint16 i,k;
	uint8 IsUnicode = (language == UNICODELANGUAGE);
	
	if(IsUnicode)
	{
		ucode = *(uint16 *)str;
		if (ucode == 0xfffe)
		{
			ucode = 0x2000;
		}
		Sdrv_SwapIntByte(&ucode);
		ScanLen = 2;
	}
	else
	{
		//���ֺ�ASCII�Ĳ�ͬ����
		if(*str >= 0x80 &&MBCodeTwoFlag)
		{
			ucode = *(uint16 *)str;
			Sdrv_SwapIntByte(&ucode);
			ScanLen = 2;
		}
		else
		{
			ucode = *str;
			ScanLen =1;
		}
		ucode = BAPI_FontCodeMBToUni(ucode);
	}
	
	//PutCharErrFlag = RESULT_PUTCHAR_OK; //Ĭ��ΪOK
	//Hdrv_SetDrawMode(DrawModeHPic_def);    //����ʾģʽ���ó�������ʾģʽ������ֿ���ݣ�
	
	//Sdrv_GetFontPoint(code, (int)CharPointBuffer);//ȡ����
	BAPI_GetFontPoint(ucode,(uint16)CharPointBuffer); //ȡ����
	
	//��̩�����⴦��
	if((ucode>=0x0e01)&&(ucode<=0x0e7F))
	{
		ScanLen += BAPI_ThaiOverlap(str +ScanLen, IsUnicode);
	}
	
	if (CRregion->width == NULL)
	{
		return ScanLen;
	}
	
	if((AsciiSelect==0)&&((uint16)ucode<0x0080))
	{
		CharPointBuffer[32]=0x06;
	}
	if(colcnt >= CharPointBuffer[32] )                 //���������ȷ���
	{
		//Hdrv_SetDrawMode(DrawModeH_def);        //����ʾģʽ���óɺ�����ʾģ
		return ScanLen;                        //ָ����һ������
	}
	//���ַ���ȴ���8ʱ��ת��16X16�ĵ�����Ϣ
	if(CharPointBuffer[32] > 8)
	{
		fontwidth = UNICODE_WIDTH_DEF;
	}
	//���ַ���ȵ���С��8ʱ��ֻת��8X16�ĵ�����Ϣ
	else
	{
		fontwidth = UNICODE_WIDTH_DEF/2;
	}
	
	//ȷ���������
	region.x = CRregion->x;
	region.y = CRregion->y;
	TextPage = CRregion->y;
	region.width = (CRregion->width < (fontwidth-colcnt))? (CRregion->width):(fontwidth-colcnt);
	region.height = UNICODE_HEIGHT_DEF;
	
	if((AsciiSelect==0)&&((uint16)ucode<0x80))
	{
		region.height = UNICODE_HEIGHT_DEF/2;
	}
	for(k=0;k<region.height/8;k++)
	{
		for(i=0;i<region.width;i++)
		{
			// �԰�������ϣ��������ƽ���ٶԻ��Ĳ��� by cailizhen 2010-06-28
			// ����������ȥ��common�еĹ����������⴦��
			uint8 start = CharPointBuffer[32]- 1 - colcnt;
			uint8 temp = start * 2 - region.width + 1;
			if(IsArabicOrHebrew)
				CharPointBuffer[i+k*fontwidth]=
				CharPointBuffer[temp - ((CharPointBuffer[32]- 1) - (colcnt+i)) + k*fontwidth];
			else
				CharPointBuffer[i+k*fontwidth]=CharPointBuffer[colcnt+i+k*fontwidth];
		}
	}
	
	Hdrv_SetWindow(&region);
	//BlackCharToColor(fontwidth, CharPointBuffer);
	CharPointBuffer[32]=region.width;
	if(TextColumn>Display_Length)
	{
		TextColumn=CRregion->x;
	}
	if(TextColumn>=CRregion->x+CRregion->width)
	{
		TextColumn-=region.width;
	}
	else
	{
		TextColumn = CRregion->x;
	}
	//����ʱ�Դ����������͵�LCM
	//����ǰ�������ϣ������,���Ƚ��������ӳ�䴦��,Ȼ�������buffer�����ӳ����� by cailizhen
	RegionMapping(&region, CharPointBuffer[32]);
	Hdrv_BuffDataTrans(CharPointBuffer, 0);
	RegionMapping(&region, CharPointBuffer[32]);
	
	//Hdrv_SetDrawMode(DrawModeH_def);    //����ʾģʽ���óɺ�����ʾģ
	return 0;
}
