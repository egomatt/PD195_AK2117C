/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_init.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00    
* Description: support unicode font
********************************************************************************
*/
#include "Drv_S7565.h"
#include  "HDrv_S7565.h"
#include "display.h"
#include "string.h"


 

#pragma userclass(near=BBD_IE)
SD_FILE * near fp;
#pragma userclass(near=default)

#pragma userclass(hconst=BBS_IE)

#ifdef STAYER
const uint8 far FontLibName[] ="UNI-MSG.BIN";
#else
const uint8 far FontLibName[] ="UNICODE.BIN";
#endif
const uint8 far FontAscLibName[]="ASCII.bin";
const uint8 far MBtoUnicTabName[][13]=
    {"V936GBK.TBL",  /*��������codepage table*/
     "V950BIG.TBL",  /*��������codepage table*/
     "V1252.TBL",      /*Ӣ��,��ŷ��ϵ codepage table*/
     "V932JIS.TBL",   /*���� codepage table*/
     "V949KOR.TBL", /*���� codepage table*/
     "V1257.TBL",      /*���޵غ���ϵ*/
     "V1251.TBL",      /*������ϵ*/
     "V1254.TBL",      /*��������ϵ*/
     "V1255.TBL",      /*ϣ������*/
     "V874.TBL",       /*̩��*/
     "V1250.TBL",       /*��ŷ����ŷ��ϵ*/
     "V1256.TBL",      /*��������ϵ*/
     "V1253.TBL",      /*ϣ���� codepage table*/
     "V863.TBL" /*Ų����*/
    };
#pragma userclass(hconst=default)
    

#pragma renamecode(?PR?BBC_IE)

bool Sdrv_LanguageSet(uint8 lang_id);
bool Sdrv_Init(uint8 mode)
{
    uint8 *pRSTDisplayAPI; //RSTDisplayAPI���
    int *pDrvEntrance; //DrvEntrance���
    
	fp = SD_FOpen(FontLibName,MODE_READ);
	if (fp == NULL)
	{
		 
		return 0;
	}
	FontLibAddr = fp->startaddr;
	SD_FClose(fp);
	
	fp = SD_FOpen(FontAscLibName,MODE_READ);
	if (fp == NULL)
	{
		 
		return 0;
	}
	AsciiFontLibAddr = fp->startaddr;
	SD_FClose(fp);
	
	//BufPage=0x07;           //BufferĬ��λ��Zram2
	//��װ����ʱ�������³�ʼ���ж�����
    //********** ��ʾ�ж������ĸ�ֵ ********
	pRSTDisplayAPI = (uint8*) (0xff0000 | RSTDisplayAPI);
    *pRSTDisplayAPI = 0x02; //��"LJMP"����RSTDisplayAPI��ڵ�ַ
    pRSTDisplayAPI++;
    pDrvEntrance = (int*) pRSTDisplayAPI;
    *pDrvEntrance = (int) &(Drv_Entrance);
    
	Sdrv_LanguageSet(0);  //Ĭ����������
	if(mode==0)
	{
		Hdrv_HardInit();
	}
	return 1;
	
}

void Sdrv_Exit(void)
{
	return;
}

bool Sdrv_LanguageSet(uint8 lang_id)
{
	unsigned char table_index = 0;
	//MBCodeTwoFlag������ʶ��ǰ���������Ƿ���2���ֽ�����ʾһ������
	//IsArabicOrHebrew������ʶ��ǰ�����Ƿ���������ϣ�������������Լ����������ط������ж�
	MBCodeTwoFlag = 0;
	IsArabicOrHebrew = 0;
	
	switch(lang_id)
	{
	case  CHINESE_SIMPLIFIED:
		table_index = 0;
		MBCodeTwoFlag = 1;
		
		break;
		
	case   CHINESE_TRADITIONAL:
		table_index = 1;
		MBCodeTwoFlag = 1;
		break;
		
	case  ENGLISH:
	case  FRENCH:
	case  GERMAN:
	case  ITALIAN:
	case  DUTCH:
	case  PORTUGUESE:
	case  SPANISH:
	case  SWEDISH:
	case  INDONESIAN:
		table_index = 2;
		break;
		
	case   JAPANESE:
		table_index = 3;
		MBCodeTwoFlag = 1;
		break;
		
	case   KOREAN:
		table_index = 4;
		MBCodeTwoFlag = 1;
		break;
		
    case LITHUANIAN:  //white110118#1 for canyon
    case ESTONIAN:    //white110118#1 for canyon
	case LATVIAN:     //white110118#1 for canyon
	case  CZECH:
	case  POLISH:
		table_index = 5;
		break;
		
	case  RUSSIAN:
       	case BULGARIAN:
       	case UKRAINIAN:
		table_index = 6;
		break;
		
	case  TURKISH:
		table_index = 7;
		break;
		
	case  HEBREW:
		table_index = 8;
		IsArabicOrHebrew = 1;
		break;
		
	case  THAI:
		table_index = 9;
		break;
		
    case DANISH:
    case HUNGARIAN:
    case ROMANIAN:
    case  CROATIA:
    case  SLOVAK:
    case SERBIAN:
		table_index = 10;
		break;
		
	case  ARABIC:
		table_index = 11;
		IsArabicOrHebrew = 1;
		break;
		
	case GREEK:
		table_index = 12;
		break;

    case NORWEGIAN:
		table_index = 13;
		break;
		
	default:
		table_index = 0;
		break;
	}
	
	//����ȫ�ֱ�����������
	Language_ID = lang_id;
	
	/*��ʼ��������Ե�Unicode��ת����ľ��*/
	fp = SD_FOpen(MBtoUnicTabName[table_index],MODE_READ);
	if (fp == NULL)
	{
		 
		return FALSE;
	}
	MBToUniTabAddr=fp->startaddr;
	SD_FClose(fp);
	return TRUE;
}
