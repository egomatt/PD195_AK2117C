/*
 *******************************************************************************
 *                lyric sequencer get functions
 *                    enhanced library
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 ע�⽫Z80��С�˸�Ϊ51�Ĵ��
 *
 *******************************************************************************
 */

#pragma name(MW_EH_LRC_API1)

#include <actos.h>
#include <enhanced.h>
#include "ReadLyricFile.h"
//keil 51�Ǵ�ˣ���С�����������
//lyricNextTime lyricCurTime��������������Ҫά����

//vram lyric buf ���ݽṹ
//ʱ���ǩ
//record items:
//    *************************************************
//    *** minute|second|100ms|content table address ***
//    *** Byte0  BYTE1  BYTE2   BYTE3    BYTE4      ***
//    *************************************************
//    �Ǵ�ˣ�����byte3�Ǹ��ֽڵ�ַ��byte4�ǵ��ֽڵ�ַ
//    minuteΪ0xffʱ��ʾʱ���ǩ����

//���������Ϣ
//record items:
//    *************************************************
//    *** Line1 |0x0a|Line2 |0x0a|...|Linen |0x0d| ***
//    *** string|'\a'|string|'\a'|...|string|'\d'| ***
//    *************************************************
//               ����        ����            ����

uint16 lyricPageNo;
uint32 lyricCurTime;
uint32 lyricNextTime;
HANDLE lyricFileHandle;
uint8 LanguageFlag;
uint16 g_LCD_COL_TIMES;//���ÿ������� cailizhen 2011-3-9 10:05

extern uint8 offset_p1s;//0.1sֵ
extern uint8 offset_sec;//��ֵ
//bool lyricInitflag = FALSE;//Ϊ���ʾ�ѳ�ʼ��

extern bool ReadOffset(void);
bool readlyricfileMSA(const char* id3InfoBuffer);
extern BOOL ExplainLyricFile(const char* id3InfoBuffer);
extern bool Sort_Div_Save(void);
void setTempBufferClk(uint8 * sfrBak, uint8 *membankBak, uint8 *mscrBak);
void storeTempBufferClk(uint8 * psfrBak, uint8 *pmembankBak, uint8 *pmscrBak);
extern languageLrc_t LyricLanguageFlag;
#pragma renamecode(MW_EH_LRC_API1)

/********************************************************************************
 * Description : Initialize lyric Module
 *
 * Arguments  :
 ScreenMode:  the size of screen, only support LYRIC_SCREEN_128_32 now
 CharSet:  Character set  type of lyric, should parse lyric according to it
 * Returns     :
 TRUE: Successful
 FALSE: fail.
 * Notes       :  don't support screen mode and charactor set now.
 *
 ********************************************************************************/
#ifndef PC
bool slyricInit(uchar ScreenMode, uchar CharSet)
#else
bool lyricInit(uchar ScreenMode, uchar CharSet)
#endif
{
    // ���ø��������, Ĭ��Ϊ128
    g_LCD_COL_TIMES = 108;
    if(ScreenMode == LYRIC_SCREEN_262_100)
    {
        g_LCD_COL_TIMES = 262;
    }
    
    LanguageFlag = CharSet;
    //lyricInitflag = TRUE;
    return TRUE;
}

void setTempBufferClk(uint8 * psfrBak, uint8 *pmembankBak, uint8 *pmscrBak)
{
    *psfrBak = (uint8)SFR_BANK;
    *pmembankBak = (uint8)MemBankCtl;
    *pmscrBak = (uint8)MCSR5;

    SFR_BANK = BANK_CMU_RMU;
    MemBankCtl |= 0x08; //PCMRAM IN DATA SEGMENT
    MCSR5 |= 0x04; //pcm in dataSeg
    MCSR5 &= 0xfd;

}

void storeTempBufferClk(uint8 * psfrBak, uint8 *pmembankBak, uint8 *pmscrBak)
{
    SFR_BANK = (char)(*psfrBak);
    MemBankCtl = (char)(*pmembankBak);
    MCSR5 = (char)(*pmscrBak);
}

/********************************************************************************
 * Description : Load a lyric to lyric vram buffer.
 *
 * Arguments  :
 ��ȡ������ݵ�VRAM�����ɹ�������page mode ��Ӣ��ģʽ��������ģʽ��Ӣ��ģʽ��ȫ��1���ֽڴ���һ���ַ�
 strFile:  Dos 8.3 file name of music audio file.
 strID3:  ID3 information of the music audio file.
 * Returns     :
 page mode of the lyric,  0 means fail.

 * Notes       :  Only support LRC file now.
 *
 ********************************************************************************/
#ifndef PC
char slyricLoad(const BYTE * strFile, const BYTE * strID3)
#else
char lyricLoad(const BYTE * strFile, const BYTE * strID3)
#endif
{
    HANDLE hfile;
    BOOL result;
    uint8 srfBak, membankCtlBak, mscrBak;

    ClearWatchDog();

    setTempBufferClk(&srfBak, &membankCtlBak, &mscrBak);

    //PAGE MODE ����������һ��д��VRAM���������ﲻ����ȡ
    hfile = lyricOpenfile(strFile);
    if (hfile == 0)
    {
        storeTempBufferClk(&srfBak, &membankCtlBak, &mscrBak);
        return 0;
    }

    lyricPageNo = 0;
    lyricCurTime = 0;
    lyricNextTime = 1;
    lyricFileHandle = hfile;

    result = ReadOffset(); //��OFFSETֵ
    if (FALSE == result)
    {
        offset_p1s = 0;
        offset_sec = 0;
        //		if(!FS_FSeek(0, 0, lyricFileHandle))
        //		{
        //			FS_FClose(hfile);
        //			storeTempBufferClk(&srfBak, &membankCtlBak, &mscrBak);
        //			return FALSE;
        //		}
    }

    ClearWatchDog();
    result = readlyricfileMSA(strID3);

    if (!FS_FClose(hfile))
    {
        storeTempBufferClk(&srfBak, &membankCtlBak, &mscrBak);
        return 0;
    }

    storeTempBufferClk(&srfBak, &membankCtlBak, &mscrBak);

    return result;
}

/********************************************************************************
 * Description : Query if exist lyric of specified music audio file.
 *
 * Arguments  :
 strFile:  Dos 8.3 file name of music audio file.
 * Returns     :
 TRUE:  Exist.
 FALSE: Don't

 * Notes       :  Only support LRC file now.
 *
 ********************************************************************************/
bool lyricQueryExist(const BYTE * strFile)
{
    HANDLE hfile;

    hfile = lyricOpenfile(strFile);
    if (hfile == 0)
    {
        return FALSE;
    }
    return FS_FClose(hfile);
}

/********************************************************************************
 * Description : readlyricfileMSA
 *
 * Arguments  :

 * Returns     : ����0 ��ʾ�ø���ļ���Ч�� ����1��ʾ�ø����Ч


 * Notes       :  Only support LRC file now.
 *
 ********************************************************************************/

bool readlyricfileMSA(const char* id3InfoBuffer)
{
    bool errorFlag;

    LyricLanguageFlag = English_LRC;
    errorFlag = ExplainLyricFile(id3InfoBuffer);//right 0; error; 1
    //����0��ʾ
    if (ERROR_EXPLAIN == errorFlag)
    {
        return FALSE;//��ʾ������
    }

    //�洢��VRAM
    if (!Sort_Div_Save()) //������bank
    {
        return FALSE;//��ʾ�洢����
    }
    return TRUE;
}

