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
#pragma name(MW_EH_LRC_API2)

#include <actos.h>
#include <enhanced.h>
#include "ReadLyricFile.h"
//#include "debug.h"
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

const int16 m_mstable[10] =
{ 0, 100, 200, 300, 400, 500, 600, 700, 800, 900 };

extern uint16 lyricPageNo;
extern uint32 lyricCurTime;
extern uint32 lyricNextTime;

BOOL lyricGetByNo(char * strLyric, uchar len, uint32 *ptime, uint16 No);
bool VMReadU(char *pbuf, uint16 address, uint16 len);

#pragma renamecode(MW_EH_LRC_API2)
/********************************************************************************
 * Description : Get a lyric page by lyric page No
 *
 * Arguments  : ��ȡָ��һҳ��ʵ�strLyric��
 strLyric:   lyric page string.
 len:         max length of lyric page string.
 ptime:	the time of lyric page.
 No:		lyric page No
 * Returns     :
 TRUE: Successful
 FALSE: fail.
 ��ȡ��ʱ�� ���ӣ�BYTE�� ʱ��(BYTE) ���� WORD
 * Notes       :  None.    strLyricΪ��ʱ�ǻ�ȡʱ��
 *
 ********************************************************************************/
BOOL lyricGetByNo(char * strLyric, uchar len, uint32 *ptime, uint16 No)
{

    uint8 TimeRec[TIME_RECORD_SIZE];
    uint16 pReadPos;

    ClearWatchDog();
    if ((len == 0) || (No == 0))
    {
        return FALSE;
    }

    if (!VMReadU(TimeRec, (TIME_TABLE_ADDR) + (No - 1) * (TIME_RECORD_SIZE), TIME_RECORD_SIZE))
    {
        return FALSE;
    }

    //minute == 0xff means the last record
    if (0xff == TimeRec[MINUTE_INDEX])
    {
        return FALSE;
    }

    //long��4���ֽ�,����Z80��С��
    *((int16 *) ptime) = *((int16 *) &TimeRec[MINUTE_INDEX]); //�Ѿ�ȡ�������Լ���
    *(((int16 *) ptime) + 1) = m_mstable[(TimeRec[MILSECONDE_INDEX]) & 0x0f];
    #ifdef PC
	SIM_ChgLongPC((uint8 *)ptime);
    #endif
    //���strLyric�ǿգ�������ú���ֻ�����ȡʱ�䣬���追����ҳ���
    if (strLyric == NULL)
    {
        return TRUE;
    }

    // read content
    pReadPos = *((uint16 *) (&TimeRec[CONTENT_IDNEX]));
    if (!VMReadU(strLyric, (CONTENT_TABLE_ADDR) + pReadPos, len))
    {
        return FALSE;
    }

    return TRUE;
}

/********************************************************************************
 * Description : Seek for specifying lyric page by time
 *
 * Arguments  :  ����ʱ������ȡ��ʣ�������ҳ�Ÿ����ڸ�ʱ���
 ���ʱ������ʾҳ�ŵĹ�ϵ
 ��� ��һҳ ʱ���� 01 02 3
 �ڶ�ҳ ʱ���� 02 03 4
 ����ҳ ʱ���� 03 05 6
 ���ڻ�õ�ʱ���� 02 5 3 ��Ӧ������ʾ�ڶ�ҳ������
 strLyric:   lyric page string.
 len:         max length of lyric page string.
 lTime:	the time of specifying lyric page.
 * Returns     :
 TRUE: Successful
 FALSE: fail.
 * Notes       :  None.
 *
 ********************************************************************************/
#ifndef PC
bool slyricSeekFor(char *strLyric, const uchar len, const uint32 lTime)
#else
bool lyricSeekFor( char *strLyric, const uchar len, const uint32 lTime)
#endif
{
    uint16 n;
    uint8  get_result;
    uint32 time;

    ClearWatchDog();
    if ((len == 0) || (lTime >= MAX_TIME))
    {
        return FALSE;
    }

    if (lTime == 0)
    {
        lyricPageNo = 0;
        return lyricGetNext(strLyric, len);
    }

    //��ʾ�Ѿ�������ҳ������1��ʼ����������2����ʾ�Ѿ����˵ڶ�ҳ��

    n = lyricPageNo;
    if (lTime >= lyricNextTime)
    {
        do
        {
            n++;
            get_result = lyricGetByNo(NULL, 1, &time, n);
        } while ((get_result) && (lTime >= time) 
                   && (n < ((TIME_TABLE_SIZE) / (TIME_RECORD_SIZE))));


        //Ӧ������ʾ��һҳ
        if (!lyricGetByNo(strLyric, len, &lyricCurTime, n - 1))
        {
            return FALSE;
        }

        lyricPageNo = n - 1;
        if(get_result == FALSE)
        {          
            lyricNextTime = MAX_TIME;		
        }
        else
        {        
            lyricNextTime = time;
        }
    }
    else if (lTime < lyricCurTime) //���˲���ʱ��������������
    {
        if (n <= 1)//�ȵ�һҳ��ʾ��ʱ��С��ʱ������޷���ʾ��
        {
            return FALSE;
        }

        do
        {
            n--;
            if (!lyricGetByNo(strLyric, len, &time, n))
            {
                return FALSE;	
            }                
        } while ((n > 1) && (lTime < time));

        lyricCurTime = time;
        lyricPageNo = n;

        if (!lyricGetByNo(NULL, 1, &time, lyricPageNo + 1))
        {
            lyricNextTime = MAX_TIME;
        }
        else
        {
            lyricNextTime = time;
        }
    }
    else
    {
        return lyricGetByNo(strLyric, len, &time, lyricPageNo);
    }
    return TRUE;

}

/********************************************************************************
 * Description : Query if a time is out of the duration of current lyric page
 *
 * Arguments  :  ��ѯ��Ҫ��ʾ��ʱ���Ƿ񳬳���������ʾ�ķ�Χ��
 lTime:	the time to queryed
 * Returns     :
 TRUE:  the time is out of.
 FALSE: don't
 * Notes       :  the function would be called frequent, should fast.
 *
 ********************************************************************************/
#ifndef PC
BOOL slyricQueryOutof(uint32 lTime)
#else
BOOL lyricQueryOutof(uint32 lTime)
#endif
{
    return ((lTime >= lyricNextTime) || (lTime < lyricCurTime));
}

#ifndef PC
BOOL slyricGetPageTime(uint32 *CurTime, uint32 *NextTime)
#else
BOOL lyricGetPageTime(uint32 *CurTime, uint32 *NextTime)
#endif
{
    *CurTime = lyricCurTime;
    *NextTime = lyricNextTime;
    return TRUE;
}
/*
 void setTempBufferClk1(uint8 * psfrBak, uint8 *pmembankBak, uint8 *pmscrBak)
 {
 *psfrBak = SFR_BANK;
 *pmembankBak = MemBankCtl;
 *pmscrBak = MCSR5;

 SFR_BANK = BANK_CMU_RMU;
 MemBankCtl |= 0x08; //PCMRAM IN DATA SEGMENT
 MCSR5 |=0x04; //pcm in dataSeg
 MCSR5 &=0xfd;

 }

 void storeTempBufferClk1(uint8 * psfrBak, uint8 *pmembankBak, uint8 *pmscrBak)
 {
 SFR_BANK = *psfrBak;
 MemBankCtl =*pmembankBak;
 MCSR5 = *pmscrBak;
 }
 */

/********************************************************************************
 * Description : VM Reader fixed the sector bound limits
 *
 * Arguments  :
 pbuf:  the buf to store reading data
 address: the address of reading data
 len:		the length of reading data
 * Returns     :
 TRUE:  successful
 FALSE: fail
 * Notes       :  None
 *
 ********************************************************************************/
bool VMReadU(char *pbuf, uint16 address, uint16 len)
{
    uint16 t;

    t = 512 - (address & 0x1ff);
    if (len > t)//��Խ��ҳ
    {
        if (!VMRead(pbuf, address, t))
        {
            return FALSE;
        }
        len -= t;
        pbuf += t;
        address += t;
    }

    return VMRead(pbuf, address, len);
}
