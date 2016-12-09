/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_puts.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00    
* Description: support unicode font
********************************************************************************
*/
#include "Drv_S7565.h"
#include "display.h"
#include "string.h"

#pragma renamecode(?PR?BBC_PUTS)

//ȡ��Ļ����
void Sdrv_GetTextPos(uint8 *x,uint8 *y)
{
	*x = region.x;
	*y = region.y;
}

//������Ļ����
void Sdrv_SetTextPos(uint8 x,uint8 y)
{
	if(x<Display_Length)
	{
		region.x=x;
		TextColumn=x;
	}
	if(y<Display_Height)
	{
		TextPage=y;
		region.y=y;
	}
}

//����������ϣ������������
void MakeArabicHebrewLink(uint8 *str, uint8 IsUnicode)
{
	char totallength;
	char cur_length;

	totallength = BAPI_TurnArabicToUn(str, IsUnicode, -1);
	while(totallength>0)
	{
		cur_length = BAPI_CopyStr(AuxiliaryBuffer,str,RowWordsCountMax,1);
		memcpy(str, AuxiliaryBuffer, cur_length*2);
		str += cur_length*2;
		totallength -= cur_length;
	}
	*(int *)str = 0x00;
}

// ���ַ���������ʾ���
int8 CommonScan(uint8 *str, int8 IsUnicode, int8 strlen,uint8 IsConvert)//matt120802#1
{
	int8 ScanLen=0;
	int8 ScanLenbk;
	uint16 ucode;
	int8 result;
	int8 lineNum_sav,rowNum_sav;//�кż�¼
	uint8 ucode_0xfffe = 0;
	
	lineNum_sav = region.y;
	rowNum_sav = region.x;
	StrHeight = UNICODE_HEIGHT_DEF;
	
	while (1)
	{
		//��ȡ��ǰ�ַ�����
		ucode = (IsUnicode? (*(uint16 *)(str+ScanLen)): *(str+ScanLen));
		    
		if(IsUnicode)
		{
		    ucode = (*(uint16 *)(str+ScanLen));
		    if (ucode == 0xfffe)
		    {
		        ucode_0xfffe = 1;
		    }
			else
		    {
		        ucode_0xfffe = 0;
		    }
		    if(IsConvert == 0)
		    {
		        
		        Sdrv_SwapIntByte(&ucode);
		    }
		}
		if((ucode ==0x00)||(strlen==0x00)||(ucode =='\r'))
		{
			break;
		}
		//���д���
		if (ucode == '\n')
		{
			region.x = rowNum_sav;
			region.y += StrHeight;
			TextColumn = region.x;
			TextPage = region.y;
			lineNum_sav = region.y;//���浱ǰ�ж���y������
			StrHeight = UNICODE_HEIGHT_DEF;
			if(IsUnicode)
			{
				ScanLen++;
				ScanLen++;
			}
			else
			{
				ScanLen++;
			}
			(uint8)strlen--;//������ֱ�Ӽ�(��Ϊ0xffʱ��ʾ��ʾȫ��)
			continue;
		}
		ScanLenbk = ScanLen;	//���ݵ�ַ����putcharʧ�ܺ�ָ�
		if(IsUnicode)
		{
			ScanLen ++;
			ScanLen ++;
		}
		else
		{
			//�����ASCII�Ĳ�ͬ����
			//����ж�����Ե�ת��
			//���պ����������ֽڱ�ʾ����,���඼����һ���ֽ�
			if (ucode >= 0x80 && MBCodeTwoFlag)
			{
				ucode = *(uint16 *)(str+ScanLen);
				Sdrv_SwapIntByte((uint16*) &ucode);
				ScanLen++;
				ScanLen++;
			}
			else //ASCII
			{
				ucode = *(str+ScanLen);
				ScanLen++;
			}
			ucode = BAPI_FontCodeMBToUni(ucode);
			StrHeight = UNICODE_HEIGHT_DEF;//�ѵ�ǰ�еĸ߶���Ϊ���ֵĸ߶�
		}
		
		//������ַ�
		if (ucode_0xfffe == 0)
		{
			result = Sdrv_PutChar(str+ScanLen, IsUnicode, ucode);	//str+ScanLenָ������ַ�,���ڴ���̩��ĵ���
		}
		else
		{
			result = 0;
		}

		if (result==-1)
		{
			return ScanLenbk;
		}
		ScanLen += result;
		(uint8)strlen--;//������ֱ�Ӽ�(��Ϊ0xffʱ��ʾ��ʾȫ��)
		region.y = lineNum_sav;//�ָ���ǰ�ж�y��������
	}
	
	return ScanLen;
}

////*********************************************************************************/
///*
//** FUNCTION:     Sdrv_PutS
//**
//** Description:  ����Ļ�����һ��ANSI�ַ������紮�ĳ��ȳ�����Ļ���������ֲ���ʾ
//**                   ָ��ͣ�����һ������ʾ���ֵ���һ���ַ����Ͻ�,�����س���
//**                   ��βʱ�˳�����������ʱ����һ�п�ʼ��ʾ
//**  input
//**     str: �ַ�����ַ ;
//**     strlen: �ַ����ĳ���
//**
//**  output
//**     none
//*************************************************************************************/
int8 * Sdrv_PutS(int8 *str, int8 strlen)
{
	int8 *str_bk=str;
	int8 IsUnicode = FALSE;
	int8 len = (char)strlen;
	int8 ScanLen;
	uint8 IsSwap = 0 ;
	// ���Ӷ԰�������ϣ������Ĵ���
	if(IsArabicOrHebrew)
	{
		// ���ڿռ䣬������һ������ܴ���26���ַ�
		if(len > RowWordsCountMax)
			len = RowWordsCountMax;
		memcpy(AuxiliaryBuffer, str, len);
		AuxiliaryBuffer[len] = '\0';
		len = BAPI_TurnArabicToUn(AuxiliaryBuffer, FALSE, (int8)len);
		str = AuxiliaryBuffer;
		IsSwap = 1;
		len = BAPI_CopyStr(AuxiliaryBuffer,str,len,IsSwap);
		IsUnicode = TRUE;
		
	}
	ScanLen = CommonScan(str, IsUnicode, (int8)len,IsSwap);
	//����ǰ�������ϣ�����򷵻��븨���ַ����ռ�֮��Ӧ��Դ�ַ�����λ��
	return str_bk + (IsUnicode? ScanLen/2: ScanLen);
}
////*********************************************************************************/
///*
//** FUNCTION:     Sdrv_PutUnS
//**
//** Description:  ����Ļ�����һ��unicode�ַ������紮�ĳ��ȳ�����Ļ���������ֲ���ʾ
//**                   ָ��ͣ�����һ������ʾ���ֵ���һ���ַ����Ͻ�,�����س���
//**                   ��βʱ�˳�����������ʱ����һ�п�ʼ��ʾ
//**  input
//**     str: �ַ�����ַ ;
//**     strlen: �ַ����ĳ���
//**
//**  output
//**     none
//*************************************************************************************/
int8 * Sdrv_PutUnS(int8 *str, int8 strlen)
{
	int8 *str_bk=str;
	int8 len = (int8)strlen;
	uint8 IsSwap = 0 ;
	
	// ���Ӷ԰�������ϣ������Ĵ���
	if(IsArabicOrHebrew)
	{
	    
		len = BAPI_CopyStr(AuxiliaryBuffer,str,(int8)len,IsSwap);
		str = AuxiliaryBuffer;
		IsSwap = 1 ;
	}
	//����ǰ�������ϣ�����򷵻��븨���ַ����ռ�֮��Ӧ��Դ�ַ�����λ��
	return str_bk + CommonScan(str, TRUE, strlen,IsSwap);
	
	//
}

////*********************************************************************************/
///*
//** FUNCTION:     Sdrv_PutStringExt
//**
//** Description:  ����Ļ�����һ��ANSI�ַ������紮�ĳ��ȳ�����Ļ���������ֲ���ʾ
//**                   ָ��ͣ�����һ������ʾ���ֵ���һ���ַ����Ͻ�,�����س���
//**                   ��βʱ�˳�����������ʱ����һ�п�ʼ��ʾ
//**  input
//**     str: �ַ�����ַ ;
//**     strlen: �ַ����ĳ���
//**	    IsUnicode:�Ƿ�Unicode�ַ���
//**     mode:����ģʽ
//**          0--��ͬ��Puts 1--�԰�������ϣ�������Խ���ת��
//**          2--�԰�������ϣ����������� 4--�԰�������ϣ��������ת���������
//**  output
//**     char * ���ش������ĵ�ǰ�ַ�ָ��
//*************************************************************************************/
uint8 * Sdrv_PutStringExt(uint8 *str, int8 strlen, int8 IsUnicode, int8 mode)//matt120802#1
{
	switch(mode)
	{
		#if 0
	case 0x00://��ͬ��puts����putuns
		if(IsUnicode)
		{
			return Sdrv_PutUnS(str, strlen);
		}
		else
		{
			return Sdrv_PutS(str, strlen);
		}
		#endif
	case 0x01://�����԰�������ϣ�����ַ������мӹ�����
		if(IsArabicOrHebrew)
		{
			MakeArabicHebrewLink(str, IsUnicode);
		}
		return str;
	case 0x02://��������ַ���,�ǰ�������ϣ��������µ�ͬ��0x00ģʽ,
		//�ڰ�������ϣ������������û���Ⱦ���0x01����,���ڷ���дģʽ
		return str + CommonScan(str, IsUnicode, strlen,0);
	case 0x04://����0x01ģʽ����0x02ģʽ
		if(IsArabicOrHebrew)
		{
			MakeArabicHebrewLink(str, IsUnicode);		
			IsUnicode = TRUE;
		}
		return str + CommonScan(str, IsUnicode, strlen,0);
	default:
		return str;
	}
}

