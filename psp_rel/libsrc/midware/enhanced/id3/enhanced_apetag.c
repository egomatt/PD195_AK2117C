/********************************************************************************
*                              ASDK 3320
*                            Module: ape Info parse
*                 Copyright(c) 2001-2012 Actions Semiconductor,
*                            All Rights Reserved.
*
* History:
*      <author>			<time>           <version >             <desc>
*    huangxiaojia     2012-5-21 9:42     1.0             build this file
********************************************************************************/
#pragma name(MW_EH_ID3_APE)

#pragma renamecode(MW_EH_ID3_APE)

#include <actos.h>
#include <decC.h>
#include <enhanced.h>
#include <stringH.h>
/********************************************************************************
********Put variable declarations here.
********************************************************************************/
uint16	far	ape_bufindex;				//当前TempBuf未处理字符的索引
uint32  far	ape_filepos;				//当前文件的精确指针
uint32	far	DataFrameBegin;	
uint32	far	ApeTagBegin;
uint8	far ApeVersion;
#ifdef _PC_
uint8*	far	ape_TempBuf;
#else
uint8	far	ape_TempBuf[512];
#endif
uint8	far	ape_TagBuf[64];
HANDLE 	far ape_TagHandle;

const char ape_ID3TITLETable[] = "TitleArtisAlbumGenreTrack"; 

/********************************************************************************
Put Functions declarations here.
********************************************************************************/
static BOOL	FI_FRead(uint8* databuf, uint16 length);
static BOOL	FI_FSeek(uint32 new_filepos);
static BOOL	check_file_head(void);
static BOOL	check_version(void);
static BOOL AnalyzeCurrent(void);
static BOOL	AnalyzeOld(void);
static BOOL	GetApeTag(ID3Info_ext_t* pID3Info);
static BOOL	SearchTagPos(uint32 LastSeekItem);
static uint8 check_item_name(uint8* buffer);

#pragma ROM(huge)
/*
********************************************************************************
*             ApeGetInfo
* Description :	   获取歌曲属性以及ID3
* Arguments   :  pFlacframeoffset:数据帧相对于文件头的偏移
                 Totaltime: 计算歌曲时间
                 formatParameter: 歌曲属性
* Returns     : 0  成功
* Notes       :
********************************************************************************
*/
BYTE sApeGetID3Info(const char *filename, ID3Info_ext_t *ID3Info)
{
    ape_bufindex = 0;
    ape_filepos = 0;

    ape_TagHandle = FS_FOpen(filename, FS_OPEN_NORMAL);//打开文件 
    if(ape_TagHandle == NULL)
    {
        return	CHECK_NOPASS;
    }
    
    if(FS_FRead(ape_TempBuf, 1, ape_TagHandle) == 0) //读一PAGE数据
    {
        FS_FClose(ape_TagHandle);
        return CHECK_NOPASS;
    }

    if(check_file_head() == FALSE)
    {
        FS_FClose(ape_TagHandle);
        return CHECK_NOPASS;    
    }

    if (check_version() == FALSE)
    {
        FS_FClose(ape_TagHandle);	
        return CHECK_NOPASS;
    }

    if (ApeVersion == 1)
    {
        AnalyzeOld();
    }
    else
    {
        AnalyzeCurrent();
    }

    GetApeTag(ID3Info);

    FS_FClose(ape_TagHandle);

    return CHECK_PASS;
}

/********************************************************************************
* Description : 检查最前面四个字节是否为"MAC "
*
* Arguments  :
* Returns     :
*            TRUE: 成功,
*            FALSE: 失败.
* Notes       :
*
********************************************************************************/
static BOOL check_file_head(void)
{
    uint8 head_byte[4];

    FI_FRead(head_byte,4);

    if ((head_byte[0]!= 'M')||(head_byte[1]!= 'A')||(head_byte[2]!= 'C')||(head_byte[3]!= ' '))
    {
        return FALSE;
    }
    return TRUE;
}

/********************************************************************************
* Description : 计算版本号并返回其新旧版本
*
* Arguments  :
*           pApeInfo用于存储信息的数据结构
* Returns     :
*            TRUE: 成功,
*            FALSE: 失败.
* Notes       :
*
********************************************************************************/
BOOL check_version(void)
{
    uint8 version_buf[2];
    uint16 version_number;

    FI_FRead(version_buf, 2);
    version_number = version_buf[0] + ((uint16) version_buf[1]<<8);

    if (version_number >= 3980)
    {
        FI_FRead(version_buf , 2);
        ApeVersion = 2;
    }
    else if (version_number <= 3950)
    {
        ApeVersion = 0;
        return FALSE;
    }
    else
    {
        ApeVersion = 1;
    }
    return TRUE;
}
/********************************************************************************
* Description : 解析新版APEv2的信息
*
* Arguments  :
*             pApeInfo用于存储信息的数据结构
* Returns     :
*            TRUE: 成功,
*            FALSE: 失败.
* Notes       :
*
********************************************************************************/
BOOL AnalyzeCurrent(void)
{
    ApeInfo_t ApeInfo, *pApeInfo;
    pApeInfo = &ApeInfo;

    FI_FRead(ape_TagBuf,4);
    pApeInfo->DescriptorSize = ape_TagBuf[0];

    FI_FRead(ape_TagBuf,(uint16)(pApeInfo->DescriptorSize)-12);

    pApeInfo->HeaderSize	 = ape_TagBuf[0];
    pApeInfo->SeekTableSize  = ape_TagBuf[4] + ((uint16) ape_TagBuf[5]<<8);
    pApeInfo->HeaderDataSize = ape_TagBuf[8];
    pApeInfo->FrameDataSize  = ape_TagBuf[12] + ((uint16) ape_TagBuf[13]<<8) + ((uint32)ape_TagBuf[14]<<16) 
                               + ((uint32)ape_TagBuf[15]<<24);

    FI_FRead(ape_TagBuf,(uint16)pApeInfo->HeaderSize);

    pApeInfo->BlocksPerFrame  = ape_TagBuf[4] + ((uint16) ape_TagBuf[5]<<8) + ((uint32)ape_TagBuf[6]<<16) 
                                + ((uint32)ape_TagBuf[7]<<24);
    pApeInfo->BlocksFinalFrame= ape_TagBuf[8] + ((uint16) ape_TagBuf[9]<<8) + ((uint32)ape_TagBuf[10]<<16) 
                                + ((uint32)ape_TagBuf[11]<<24);
    pApeInfo->TotalFrames	  = ape_TagBuf[12] + ((uint16) ape_TagBuf[13]<<8) + ((uint32)ape_TagBuf[14]<<16) 
                                + ((uint32)ape_TagBuf[15]<<24);
    pApeInfo->Bits			  = ape_TagBuf[16];
    pApeInfo->Channels		  = ape_TagBuf[18];
    pApeInfo->SampleRate	  = ape_TagBuf[20] + ((uint16) ape_TagBuf[21]<<8) + ((uint32)ape_TagBuf[22]<<16) 
                                + ((uint32)ape_TagBuf[23]<<24); 

    DataFrameBegin	= ape_filepos + pApeInfo->SeekTableSize + pApeInfo->HeaderDataSize;
    ApeTagBegin		= DataFrameBegin + pApeInfo->FrameDataSize;

    return TRUE;
}
/********************************************************************************
* Description : 解析旧版APEv2的信息
*
* Arguments  :
*            
* Returns     :
*            TRUE: 成功,
*            FALSE: 失败.
* Notes       :
*
********************************************************************************/
BOOL AnalyzeOld(void)
{
    ApeInfo_t ApeInfo, *pApeInfo;
    pApeInfo = &ApeInfo;

    FI_FRead(ape_TagBuf,34);
    pApeInfo->Bits			   = (ape_TagBuf[2] & 1) ? 8 : ((ape_TagBuf[2] & 8) ? 24 : 16);
    pApeInfo->Channels		   = ape_TagBuf[4];
    pApeInfo->SampleRate	   = ape_TagBuf[6] + ((uint16) ape_TagBuf[7]<<8) + ((uint32)ape_TagBuf[8]<<16)
                                 + ((uint32)ape_TagBuf[9]<<24); 
    pApeInfo->HeaderDataSize   = ape_TagBuf[10];
    pApeInfo->BlocksPerFrame   = 0x48000;
    pApeInfo->TotalFrames	   = ape_TagBuf[18] + ((uint16) ape_TagBuf[19]<<8) + ((uint32)ape_TagBuf[20]<<16) 
                                 + ((uint32)ape_TagBuf[21]<<24); 
    pApeInfo->BlocksFinalFrame = ape_TagBuf[22] + ((uint16) ape_TagBuf[23]<<8) + ((uint32)ape_TagBuf[24]<<16) 
                                 + ((uint32)ape_TagBuf[25]<<24);
    pApeInfo->SeekTableSize	   = pApeInfo->TotalFrames * 4;

    DataFrameBegin	= ape_filepos + pApeInfo->SeekTableSize + pApeInfo->HeaderDataSize;
    SearchTagPos(DataFrameBegin - 4);

    return TRUE;
}
/********************************************************************************
* Description : 搜索旧版本tag起始位置
*
* Arguments  :
*            seek表的最后一个帧地址
* Returns     :
*            TRUE: 成功,
*            FALSE: 失败.
* Notes       :
*
********************************************************************************/
BOOL SearchTagPos(uint32 LastSeekItem)
{
    uint32	LastFramePos;
    uint8	i;
	
    if (FI_FSeek(LastSeekItem) == FALSE)
    {
        return FALSE;
    }

    FI_FRead(ape_TagBuf, 4);
    LastFramePos = ape_TagBuf[0] + ((uint16) ape_TagBuf[1]<<8) + ((uint32)ape_TagBuf[2]<<16) 
                   + ((uint32)ape_TagBuf[3]<<24);
    FI_FSeek(LastFramePos);

    while (FI_FRead(ape_TagBuf, 64) == TRUE)
    {
        for (i=0; i<64; i++)
        {
            if (ape_TagBuf[i] == 'A')
            {
                if ((ape_TagBuf[i+1] == 'P')&&(ape_TagBuf[i+2] == 'E')&&
                    (ape_TagBuf[i+3] == 'T')&&(ape_TagBuf[i+4] == 'A')&&(ape_TagBuf[i+5] == 'G'))
                {
                    break;
                }
            }
        }
        if (i < 64)
        {
            ApeTagBegin = ape_filepos + i - 64;
            return TRUE;
        }
    }

    return FALSE;
}
/********************************************************************************
* Description : 解析APETag的id3信息
*
* Arguments  :
*            用于存储id3的数据结构
* Returns     :
*            TRUE: 成功,
*            FALSE: 失败.
* Notes       :
*
********************************************************************************/
BOOL GetApeTag(ID3Info_ext_t* pID3Info)
{
    uint8 	ItemNumber;
    uint8 	ItemLen;
    uint8 	ItemSort;
    uint8	buf_size;
    uint8* 	ID3StructAddr;
    uint32	ID3BufAddr;
    uint8 	i,j;
	
    if (ApeTagBegin == 0)
    {
        return FALSE;
    }

    if (FI_FSeek(ApeTagBegin) == FALSE) 
    {
        return FALSE;
    }

    FI_FRead(ape_TagBuf,32);

    if ((ape_TagBuf[0] != 'A')||(ape_TagBuf[1] != 'P')||(ape_TagBuf[2] != 'E')||(ape_TagBuf[3] != 'T')
        ||(ape_TagBuf[4] != 'A')||(ape_TagBuf[5] != 'G')||(ape_TagBuf[6] != 'E')||(ape_TagBuf[7] != 'X'))
    {
        return FALSE;
    }

    ItemNumber = ape_TagBuf[16];

    while (ItemNumber)
    {
        FI_FRead(ape_TagBuf,16);
        ItemLen = ape_TagBuf[0];
        ItemSort = check_item_name(ape_TagBuf+8);
        ItemSort --;

        i = 8;
        while (ape_TagBuf[i])
        {
            i++;
        }
        i++;

        if ((i + ItemLen) >= 16)
        {
            FI_FRead(ape_TagBuf + 16, i + ItemLen -16);
        }
        else
        {
            ape_bufindex -= 16 - i - ItemLen;
            ape_filepos -= 16 - i - ItemLen;		//边界暂不考虑
        }

        if (ItemSort < 5)
        {
            ID3StructAddr = (uint8*)pID3Info + ItemSort*5;
            if(ID3StructAddr[0] > 0)
            {   
//              *ID3StructAddr = ItemLen; 
                buf_size = ID3StructAddr[0];
                for(j=1; j<5; j++)
                {
                    ID3BufAddr = *(ID3StructAddr + j) + (ID3BufAddr<<8);
                }
                ID3StructAddr = (uint8*)ID3BufAddr;
                ID3StructAddr[0] = 3;
                for(j=0; (j<ItemLen)&&(j<buf_size-1); j++)
                {
                    //BUFFER[ItemSort][j] = ape_TagBuf[i+j];  
                    ID3StructAddr[j+1] = ape_TagBuf[i+j]; 
                }
            }
        }
        ItemNumber--;
    }

    return TRUE;
}

/********************************************************************************
* Description : 匹配ID3条目名
*
* Arguments  : 
*
* Returns     :
*
* Notes       :
*
********************************************************************************/ 
uint8 check_item_name(uint8* buffer)
{
    uint8 i,j=0;

    for(i=0; i<25; i++)
    {
        if(buffer[j] == ape_ID3TITLETable[i])
        {
            j++;
            if(j == 5)
            {
                i = (i+1)/5;
                break;    
            }   
        }
        else
        {
            j = 0;
        }   
    }
    if(i == 2)
    {
        if(buffer[j] != 't')
        {
            i = 25;
        } 
    }
    if(i == 25)
    {
        i = 0;   
    }
    return i;
}  
/********************************************************************************
* Description : 精确文件读取READ
*
* Arguments  :
*             databuf:读取文件数据的buf
*             length:读取的长度，字节为单位
* Returns     :
*            TRUE: 成功,
*            FALSE: 失败.
* Notes       :
*
********************************************************************************/
static BOOL FI_FRead(uint8* databuf, uint16 length)
{
    uint16 i = 0;

    ClearWatchDog();
    while ((ape_bufindex < 512) && (i < length))
    {
        databuf[i] = ape_TempBuf[ape_bufindex];
        i++;
        ape_bufindex++;
        if (ape_bufindex == 512)
        {
#ifdef _PC_
            if (fl.Read(ape_TempBuf, 512) == 0)
            {
                return FALSE;
            }
#else
            if (FS_FRead(ape_TempBuf, 1, ape_TagHandle) == 0)
            {
                return FALSE;
            } 
#endif
            ape_bufindex = 0;
        }
    }

    ape_filepos += (uint32)length;
    return TRUE;
}
/********************************************************************************
* Description : 重新定位指针并读取当前扇区
*
* Arguments  :  定位后filepos的值
* Returns     :
*            TRUE: 成功,
*            FALSE: 失败.
* Notes       :
*
********************************************************************************/
static BOOL FI_FSeek(uint32 new_filepos)
{
    ape_bufindex = new_filepos%512;
    if((new_filepos>>9) != (ape_filepos>>9))
    {
        ape_filepos  = new_filepos - ape_bufindex;
#ifdef _PC_
        if (fl.Seek(ape_filepos, CFile::begin) != ape_filepos)
        {
            return false;
        }
        if (fl.Read(ape_TempBuf,512) == 0)
        {
            return false;
        }
#else
        if(FS_FSeek((int32)(ape_filepos>>9), 0, ape_TagHandle) == 0)
        {
            return FALSE;
        }
        if(FS_FRead(ape_TempBuf, 1, ape_TagHandle) == 0)
        {
            return FALSE;
        }
#endif
    }
    ape_filepos = new_filepos;
    return TRUE;
}