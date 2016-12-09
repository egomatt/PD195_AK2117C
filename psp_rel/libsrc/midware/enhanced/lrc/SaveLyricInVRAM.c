/*
 *******************************************************************************
 *                lyric sequencer get functions
 *                    enhanced library
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 注意将Z80的小端改为51的大端
 确保指针是16位
 *
 *******************************************************************************
 */
#include    "ReadLyricFile.h"
#include    <filesys.h>
#include    <stringH.h>
#include	<enhanced.h>

#pragma name(MW_EH_LRC_SUB7)

extern LrcTime_t * pTimeLabel;
extern uint8 TimeRecordCounter;//一共有TimeRecordCounter+1条标签 表示结束的那一条不加
extern lyricSaveFormat_T lyricSave;
extern languageLrc_t LyricLanguageFlag;//语言标志       { 01h: 英语 || 02h: 汉语 }
extern uchar DataBuffer[PAGE_BYTE];////读取歌词，再做时间和歌词内容的分离

bool SaveLyricInVRAM(void);
void adjustContentOffest(void);
#pragma renamecode(MW_EH_LRC_SUB7)

void adjustContentOffest(void)
{
    uchar i;
    uint16 timeBase;
    //ClearWatchDog();
    pTimeLabel = (LrcTime_t *) lyricSave.LyricTimeAddrBuffer;
    timeBase = *(uint16 *) (&(pTimeLabel->buffaddr_High));
    for (i = 0; i < TimeRecordCounter; i++)
    {
        *(uint16 *) (&(pTimeLabel[i].buffaddr_High)) -= timeBase;
    }
}

bool SaveLyricInVRAM(void)
{
    uint16 saveNum, totalNum;
    uint8 * pSourceAddr;
    uint16 pDestAddr;
    bool result;
    TimeRecordCounter += 1; //加上最后一条
    lyricSave.lyricPageMode = (uchar) LyricLanguageFlag;
    adjustContentOffest();
    //save time label
    saveNum = LYRIC_FILENAME_SIZE + LYRIC_PAGEMODE_SIZE + TimeRecordCounter * 5;
    //fill time label
    if ((saveNum % PAGE_BYTE) != 0)
    {
        memset(lyricSave.lyricFileName + saveNum, 0, (PAGE_BYTE - (saveNum % PAGE_BYTE)));
        saveNum = (saveNum / PAGE_BYTE) + 1;
    }
    else
    {
        saveNum = (saveNum / PAGE_BYTE);
    }
    for (; saveNum < 4; saveNum++)
    {
        memset(lyricSave.lyricFileName + PAGE_BYTE * saveNum, 0, PAGE_BYTE);
    }
    totalNum = 4;//0x800

    //save content
    saveNum = *(uint16 *) (&pTimeLabel[TimeRecordCounter - 1].buffaddr_High);
    //fill content
    if ((saveNum % PAGE_BYTE) != 0)
    {
        memset(lyricSave.LyricStrBuffer + saveNum, 0, (PAGE_BYTE - (saveNum % PAGE_BYTE)));
        saveNum = (saveNum / PAGE_BYTE) + 1;
    }
    else
    {
        saveNum = (saveNum / PAGE_BYTE);
    }
    totalNum += saveNum;
    
    //save to vram
    pSourceAddr = lyricSave.lyricFileName;
    pDestAddr = (uint16) LYRIC_FILENAME_ADDR;
    result = VMExpWrite(pSourceAddr, pDestAddr, totalNum);
    if (!result)
    {
        return FALSE;
    }
    //	VMRead(DataBuffer, 0xa800, 512);
    //	VMRead(DataBuffer, 0xaa00, 512);
    return TRUE;
}
