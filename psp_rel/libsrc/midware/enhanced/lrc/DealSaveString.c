/*
 *******************************************************************************
 *                lyric sequencer get functions
 *                    enhanced library
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 ע�⽫Z80��С�˸�Ϊ51�Ĵ��
 ȷ��ָ����16λ
 *
 *******************************************************************************
 */
#include    "ReadLyricFile.h"
#include    <filesys.h>
#include    <stringH.h>
#include	<enhanced.h>
#include	<lang_id.h>

#pragma name(MW_EH_LRC_SUB3)

extern LrcTime_t * pTimeLabel;
extern uint8 TimeRecordCounter;//һ����TimeRecordCounter+1����ǩ ��ʾ��������һ������
extern lyricSaveFormat_T lyricSave;
extern BOOL AddLinkCharFlag;//����ʱҪ�����ӷ���־ 00:����,  0ffh: �� �������Ӧָ��ǰһ���ַ�
extern uint8 * pLyricStrBufferOffest;
extern uint16 PointColCounter;//�����˵��������������
extern uint8 HalfChineseTempSavRam;//������ִ����ݴ�
extern uint8 LyricLineCounter;//�������������,��¼��ǰ��ʶ������������ڶ�LRC�ļ�����ʱ�õ���
extern uint8 No1DataNotStaConFlag;//��һ���ַ��ǡ�[����־����������  //00h: "][" �����  0ffh: "]x"�����
extern languageLrc_t LyricLanguageFlag;//���Ա�־       { 01h: Ӣ�� || 02h: ���� }
extern uint8 LanguageFlag;//LanguageFlag = CharSet���ַ�����
extern uint8 SameStringCNT;//���ʱ���ǩ����һ���ַ���������(Ϊ1ʱ��ʾ��2��ʱ���ǩ����һ���ַ���������)
extern uint8 DivScreenPageCNT;//����������  03-9-12 16:21
extern uint8 * pDataBufferOffest;
extern uchar DataBuffer[];
extern uint16 g_LCD_COL_TIMES;//���ÿ������� cailizhen 2011-3-9 10:05

returnDealString_t DealSaveString(uchar charData);
returnDealString_t dealChineseCharLow(uchar charData);
returnDealString_t dealAsciiChar(uchar charData);
returnDealString_t dealChineseCharHigh(uchar charData);
uchar GetAsciiCol(void);
void AddLinkChar(uchar charData);
bool isEnglishChar(uchar cData);
void DoSomeThingForDivScrren(bool isChineseChar);

#pragma renamecode(MW_EH_LRC_SUB3)

void DoSomeThingForDivScrren(bool isChineseChar)
{
    uchar i;

    //����ʱ���ǩ
    memcpy(pTimeLabel, pTimeLabel - (SameStringCNT + 1), (SameStringCNT + 1) * sizeof(LrcTime_t));

    //����ʱ����ʱ���ǩ��Ҫ���Ŵ���ָ���LRCbuffer�ĵ�ַ
    DivScreenPageCNT++;//DivScreenPageCNT = 0 ��ʾ��һ�� 1 �ڶ���
    for (i = 0; i < (SameStringCNT + 1); i++)
    {
        ClearWatchDog();
        pTimeLabel->p1s &= 0x0f;
        pTimeLabel->p1s |= DivScreenPageCNT << 4;//���������ӵ�����ĸ���λ������λ��ʾ0��900����

        if (FALSE != isChineseChar)
        {
            *(WORD *) (&(pTimeLabel->buffaddr_High)) = (WORD) (pLyricStrBufferOffest - 2);
        }
        else
        {
            if (FALSE != AddLinkCharFlag)
            {
                //pTimeLabel->buffaddr_High = (uchar)(((WORD)(pLyricStrBufferOffest-2)& 0xff00)>>8);
                //pTimeLabel->buffaddr_Low= (uchar)((WORD)(pLyricStrBufferOffest-2)& 0x00ff);
                *(WORD *) (&(pTimeLabel->buffaddr_High)) = (WORD) (pLyricStrBufferOffest - 2);

            }
            else
            {
                //pTimeLabel->buffaddr_High = (uchar)(((WORD)(pLyricStrBufferOffest-1)& 0xff00)>>8);
                //pTimeLabel->buffaddr_Low= (uchar)((WORD)(pLyricStrBufferOffest-1)& 0x00ff);
                *(WORD *) (&(pTimeLabel->buffaddr_High)) = (WORD) (pLyricStrBufferOffest - 1);

            }
        }

        pTimeLabel++;
        TimeRecordCounter++;
    }

    AddLinkCharFlag = 0;
    LyricLineCounter = 0;

}

void AddLinkChar(uchar charData)
{
    //���ϻ��з���pLyricStrBufferOffest�Ѿ���1��
    bool isEnglishFlag;
    uchar tempData;

    AddLinkCharFlag = FALSE;
    isEnglishFlag = isEnglishChar(charData);
    if (FALSE != isEnglishFlag)
    {
        if (FALSE != isEnglishChar(*(pLyricStrBufferOffest - 2)))
        {
            tempData = *(pLyricStrBufferOffest - 2);
            if (FALSE != isEnglishChar(*(pLyricStrBufferOffest - 3)))
            {
                //AB||C---->A-||BC
                *(pLyricStrBufferOffest - 2) = '-';

            }
            else
            {
                //xA||C---->' '||AC
                *(pLyricStrBufferOffest - 2) = ' ';
            }
            *(pLyricStrBufferOffest++) = tempData;
            PointColCounter += GetAsciiCol();
            AddLinkCharFlag = TRUE;
            return;
        }
        else
        {
            //A3||B ��������ӷ�
            return;
        }
    }
    else
    {
        //AB||3 ��������ӷ�
        return;
    }
}

bool isEnglishChar(uchar cData)
{
    if (((cData >= 'A') && (cData <= 'Z')) || ((cData >= 'a') && (cData <= 'z')))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

uchar GetAsciiCol(void)
{
    //return (LyricLanguageFlag == English_LRC ? ASCII_COL : CHINESE_COL/2);
    return ASCII_COL;
}

returnDealString_t dealChineseCharHigh(uchar charData)
{
    HalfChineseTempSavRam = charData;
    return READ_BUFFER_AGAIN;
}

returnDealString_t dealChineseCharLow(uchar charData)
{
    if (English_LRC == LyricLanguageFlag)
    {
        LyricLanguageFlag = Chinese_LRC;
        return READ_FILE_AGAIN;
    }
    else
    {
        PointColCounter += CHINESE_COL;
        if (PointColCounter >= (g_LCD_COL_TIMES + 1))
        {
            //����һ����
            if (LyricLineCounter == (LCD_DIS_CHINESE_LINE - 1))
            {
                //����
                *(pLyricStrBufferOffest++) = NEXT_SCREEN;
                *(pLyricStrBufferOffest++) = HalfChineseTempSavRam;
                *(pLyricStrBufferOffest++) = charData;
                DoSomeThingForDivScrren(TRUE);
                PointColCounter = CHINESE_COL;
                LyricLineCounter = 0;
            }
            else
            {
                //δ��������Ҫ����,���Ĵ������迼�����ӷ�
                *(pLyricStrBufferOffest++) = NEXT_ROW;
                *(pLyricStrBufferOffest++) = HalfChineseTempSavRam;
                *(pLyricStrBufferOffest++) = charData;
                LyricLineCounter++;
                PointColCounter = CHINESE_COL + ICON_WIDTH_DOT;
            }

        }
        else
        {
            //û�г���һ��
            *(pLyricStrBufferOffest++) = HalfChineseTempSavRam;
            *(pLyricStrBufferOffest++) = charData;
        }
    }

    HalfChineseTempSavRam = 0;
    if (pLyricStrBufferOffest < (uchar*) (lyricSave.LyricStrBuffer + CONTENT_TABLE_SIZE - 1))
    {
        return READ_BUFFER_AGAIN;
    }
    else
    {
        return STRING_BUFFER_OVERFLOW;
    }

}

returnDealString_t dealAsciiChar(uchar charData)
{
    uchar asciiColumn;
    uchar LineMax;

    asciiColumn = GetAsciiCol();
    HalfChineseTempSavRam = 0;
    PointColCounter += asciiColumn;

    if (PointColCounter >= (g_LCD_COL_TIMES + 1))
    {
        //����һ����
        if (English_LRC == LyricLanguageFlag)
        {
            LineMax = LCD_DIS_ENGLISH_LINE; //Ӣ��ģʽ�´����ֽ��ַ�
        }
        else
        {
            LineMax = LCD_DIS_CHINESE_LINE;
        }

        if (LyricLineCounter == (LineMax - 1))
        {

            //����
            *(pLyricStrBufferOffest++) = NEXT_SCREEN;
            PointColCounter = GetAsciiCol();
            AddLinkChar(charData);
            *(pLyricStrBufferOffest++) = charData;
            DoSomeThingForDivScrren(FALSE);
            //AddLinkCharFlag������ĺ����Ѿ�������
            LyricLineCounter = 0;
        }
        else
        {
            //δ����������
            if (LyricLineCounter == (LineMax - 2))
            {
                //�����һ����
                PointColCounter = ICON_WIDTH_DOT + asciiColumn;
            }
            else
            {
                PointColCounter = asciiColumn;
            }

            *(pLyricStrBufferOffest++) = NEXT_ROW;
            //���ӷ�����
            AddLinkChar(charData);
            AddLinkCharFlag = 0;//�Ѿ�����
            *(pLyricStrBufferOffest++) = charData;
            LyricLineCounter++;
        }

    }
    else //δ��һ��
    {
        *(pLyricStrBufferOffest++) = charData;
    }

    if (pLyricStrBufferOffest < (uchar*) (lyricSave.LyricStrBuffer + CONTENT_TABLE_SIZE - 1))//���һ���ַ�Ҫ���Ϸ���
    {
        return READ_BUFFER_AGAIN;
    }
    else
    {
        return STRING_BUFFER_OVERFLOW;
    }

}

//==========����Ҫ������ַ�����ʽ���Ӻ���===========
//˵������buffer�ж�����ÿ��BYTE���ݶ�Ҫ�������´�����ÿ��
//���ݰ������ʽ��������µ����ݸ�ʽ���������ݸ�ʽ��״̬����
//�������������ʾ��
//------------------------------------------------
//input: a: ��Buffer���ַ�ֵ
//input:
//output:
//      a=1:  x
//      a=2:  �ٶ�BUFFER
//      a=3:  ���¶��ļ�
//      a=4:  �ռ䳬����Ҫ�˳�
//      a=5:  x
//-----------------------------------------------

returnDealString_t DealSaveString(uchar charData)
{
    No1DataNotStaConFlag = 0xff; //��һ���ַ����� [
    if (HalfChineseTempSavRam != 0)
    {
        //��һ���ֽ������ĵĸ�λ���ִ����λ
        return dealChineseCharLow(charData);
    }
    else
    {
        if ((LanguageFlag == ENGLISH) || (LanguageFlag >= FRENCH))
        {
            //isAsciiChar
            return dealAsciiChar(charData);
        }
        else
        {
            if (charData < 0x81)
            {
                //one byte encode, for exam, english
                return dealAsciiChar(charData);
            }
            else
            {
                //two bytes encode, for exam, chinese
                return dealChineseCharHigh(charData);
            }
        }
        //        else if (LanguageFlag == JAPANESE || LanguageFlag == KOREAN || LanguageFlag == CHINESE_TRADITIONAL )
        //        {
        //            //���� Big5 ��� ̨�� ����
        //            //
        //            if (charData < 0x81)
        //            {
        //                //ascii
        //                return dealAsciiChar(charData);
        //            }
        //            else
        //            {
        //                //chinese
        //                return dealChineseCharHigh(charData);
        //            }
        //        }
        //        else    //����GB2312 CHINESE_SIMPLIFIED
        //        {
        //            if (charData < 0xa0)
        //            {
        //                //ascii
        //                return dealAsciiChar(charData);
        //            }
        //            else
        //            {
        //                //chinese
        //                return dealChineseCharHigh(charData);
        //            }
        //        }

    }
}

/********************************************************************************
 * Description : ReadID3FromBufAgain
 *
 * Arguments  :

 * Returns     : ���ID3����Ϣ����ʱ���ǩ��¼
 ���� 0 ��ʾ�ɹ���ȡ��ID3�����ط����ʾ��Ҫ���¶�ȡID3����Ӣ���л�Ϊ����


 * Notes       :  Only support LRC file now.
 *
 ********************************************************************************/

uchar ReadID3FromBuf(void)
{
    //��¼��һ��ʱ���ǩ����һ����ʽΪ[0 0 0 ����׵�ַ��λ  ����׵�ַ��λ]
    uchar charData;
    returnDealString_t result;
    pTimeLabel = (LrcTime_t *) lyricSave.LyricTimeAddrBuffer;
    pLyricStrBufferOffest = lyricSave.LyricStrBuffer;
    pDataBufferOffest = DataBuffer;

    pTimeLabel->min = 0;
    pTimeLabel->sec = 0;
    pTimeLabel->p1s = 0;
    //pTimeLabel->buffaddr = (WORD)pLyricStrBufferOffest;
    *(WORD *) (&(pTimeLabel->buffaddr_High)) = (WORD) (pLyricStrBufferOffest);

    pTimeLabel++;
    TimeRecordCounter++;

    //��ID3����Ϣ����LyricStrBuffer��������ʱ���ǩ
    HalfChineseTempSavRam = 0;
    charData = *(pDataBufferOffest++);
    //ID3��ǩ�Ѿ�������������Ϣ��֮���ÿո� �����ý�������
    while (charData != NULL)
    {
        ClearWatchDog();
        result = DealSaveString(charData);
        if (result == READ_BUFFER_AGAIN)
        {
            charData = *(pDataBufferOffest++);
        }
        else if (result == READ_FILE_AGAIN)
        {
            return READ_FILE_AGAIN;
        }
        else //����overflow
        {
            //empty
        }
    }
    DivScreenPageCNT = 0;
    *(pLyricStrBufferOffest++) = NEXT_SCREEN;
    LyricLineCounter = 0;
    return 0;
}
