/********************************************************************************
*                              ASDK 3320
*                            Module: flac ID3Info parse
*                 Copyright(c) 2001-2012 Actions Semiconductor,
*                            All Rights Reserved.
*
* History:
*      <author>			<time>           <version >             <desc>
*    huangxiaojia     2012-5-18 9:42     1.0             build this file
********************************************************************************/
#pragma name(MW_EH_ID3_FLAC)

#pragma renamecode(MW_EH_ID3_FLAC)

#include <actos.h>
#include <decC.h>
#include <enhanced.h>
#include <stringH.h>

/********************************************************************************
********Put variable declarations here.
********************************************************************************/
uint16	far	flac_bufindex;				//当前flac_TempBuf未处理字符的索引
uint32  far	flac_filepos;				//当前文件的精确指针
uint32	far	flac_firstframepos;
#ifdef _PC_
uint8*	far	flac_TempBuf;
#else
uint8	far	flac_TempBuf[512];
#endif
uint8	far	flac_TagBuf[64];
HANDLE 	far flac_TagHandle;

const char flac_ID3TITLETable[] = "TitleArtisAlbumGenreTracknumber"; 
/********************************************************************************
Put Functions declarations here.
********************************************************************************/
static BOOL	FI_FRead(uint8* databuf, uint16 length);
static BOOL	FI_FSeek(uint32 new_flac_filepos);
static BOOL	check_file_head(void);
static BOOL parse_metadata_block(ID3Info_ext_t *pID3Info, StreamInfo_t *pStreamInfo, time_t *time_p); 
static BOOL parse_streaminfo_block(StreamInfo_t* pSrtInfo, time_t* timeInfo);
static BOOL	parse_ID3_block(ID3Info_ext_t *ID3);
static uint8 check_vendor_name(uint8* buffer);

/*
 ********************************************************************************
 *             flacGetInfo
 * Description :
 * Arguments   :  string_id is language id(0:简体 1 英文 2 繁体),用于对最后的不完整字符进行处理。
 因为最多取30个字符。而有些时候汉字里面有可能混放英文的。因此会出现最后字符只是汉字一半的情况。
 flac文件中使用UTF-8编码的，所以可以不用这个id。 
 * Returns     : ID3ExistFlag = 0 表示没有ID3信息，1表示有
 * Notes       :
 ********************************************************************************
*/
BYTE sflacGetID3Info(const uint8 *filename, ID3Info_ext_t *ID3Info)
{
    StreamInfo_t  StreamInfo;
    time_t time_p;

    flac_bufindex = 0;
    flac_filepos = 0;
#ifdef _PC_
    if (!fl.Open(filename, CFile::modeRead | CFile::typeBinary))
    {
         return CHECK_NOPASS;
    }
    fl.Read(flac_TempBuf,512);
#else
    flac_TagHandle = FS_FOpen(filename, FS_OPEN_NORMAL);//打开文件 
    if(flac_TagHandle == NULL)
    {
        return CHECK_NOPASS;
    }

    if(FS_FRead(flac_TempBuf, 1, flac_TagHandle) == 0) //读一PAGE数据
    {
        FS_FClose(flac_TagHandle);
        return CHECK_NOPASS;
    }
#endif

    if(check_file_head() == FALSE)
    {
        FS_FClose(flac_TagHandle);
        return CHECK_NOPASS;   
    }
    
    if(parse_metadata_block(ID3Info, &StreamInfo, &time_p) == FALSE)
    {
        FS_FClose(flac_TagHandle);
        return CHECK_NOPASS;
    }
    
#ifdef _PC_
    fl.Close();
#else
    FS_FClose(flac_TagHandle);
#endif
    return CHECK_PASS;    
}
/********************************************************************************
 * Description : 检查最前面四个字节是否为"fLaC"
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
    if((head_byte[0] !='f')||(head_byte[1] !='L')||(head_byte[2] !='a')||(head_byte[3] !='C'))
    {
        return FALSE; 
    } 
    return  TRUE;   
}
/********************************************************************************
 * Description : metadata block解析
 *
 * Arguments  :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************/
BOOL parse_metadata_block(ID3Info_ext_t *pID3Info, StreamInfo_t *pStreamInfo, time_t *time_p) 
{
    uint8 	last_meta_flag = 1;
    uint8 	block_head[4];
    uint32 	block_length;
    BOOL	ret;
    
    while( last_meta_flag )
    {
        FI_FRead(block_head,4);
        if((block_head[0]&0x80) != 0)
        {
            last_meta_flag = 0; 
            block_head[0] = block_head[0] & 0x7f;   
        } 
        block_length = block_head[3] + ((uint16) block_head[2]<<8) + ((uint32)block_head[1]<<16);
        if (block_head[0] == 0)
        {
            ret = parse_streaminfo_block(pStreamInfo, time_p);
            if(ret == FALSE)
            {
                return	FALSE;
            }
        }
        else if (block_head[0] == 4)
        {
            ret = parse_ID3_block(pID3Info);
            if(ret == FALSE)
            {
                return	FALSE;
            }
        }  
        else
        {   
            if(block_length > (uint32)(512- flac_bufindex))
            {
                FI_FSeek(flac_filepos+block_length); 
            }
            else
            {
                flac_bufindex = flac_bufindex + (uint16)block_length;
                flac_filepos	 = flac_filepos  + (uint16)block_length;
            }
        }
    }  
    flac_firstframepos = flac_filepos;
    return TRUE;
}

/********************************************************************************
 * Description : stream_info block 解析
 *
 * Arguments  : 
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************/
BOOL parse_streaminfo_block(StreamInfo_t* pSrtInfo, time_t* timeInfo)
{
    BOOL ret = TRUE;

	timeInfo = timeInfo;

    FI_FRead(flac_TagBuf, 34); 

    if ((flac_TagBuf[0] == flac_TagBuf[2])&&(flac_TagBuf[1] == flac_TagBuf[3]))
    {
        pSrtInfo->BlockSize = ((uint16)flac_TagBuf[0]<<8) + (flac_TagBuf[1]);
    } 
    else
    {
        pSrtInfo->BlockSize = 0;
    }

    pSrtInfo->sample_rate = (flac_TagBuf[12]>>4) + (flac_TagBuf[11]<<4) + ((uint32)flac_TagBuf[10]<<12);

    if ((pSrtInfo->sample_rate == 0)||(pSrtInfo->sample_rate > 655350))
    {
        ret = FALSE;
    } 

    pSrtInfo->channel = ((flac_TagBuf[12]>>1) & 0x07) + 1;

    pSrtInfo->bits = ((flac_TagBuf[12] & 0x01) <<4) + (flac_TagBuf[13] >> 4) + 1;

    if (pSrtInfo->bits > 24)
    {
        ret = FALSE;
    }

    return ret;
}
/********************************************************************************
 * Description : ID3解析
 *
 * Arguments  : 
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************/
BOOL parse_ID3_block(ID3Info_ext_t *ID3)
{
    uint8 vendor_len;
    uint8 vendor_times;
    uint8 vendor_NO;
    uint8 buf_size;
    uint8* ID3StructAddr;
    uint32 ID3vendorbuf = 0;
    uint8 i,j;

    FI_FRead(flac_TagBuf, 4);  
    vendor_len = flac_TagBuf[0];
    FI_FRead(flac_TagBuf, vendor_len+4);      
    vendor_times = flac_TagBuf[vendor_len];

    while(vendor_times)
    {
        vendor_times --;
        FI_FRead(flac_TagBuf, 4); 
        vendor_len = flac_TagBuf[0];
        if (vendor_len > 64)
        {
            return FALSE;
        }
        FI_FRead(flac_TagBuf, vendor_len);
        vendor_NO = check_vendor_name(flac_TagBuf);
        if(vendor_NO != 0)
        {
            vendor_NO--;
            i = 0;
            while(i < vendor_len)
            {
                    
                if(flac_TagBuf[i] == '=')
                {
                    i++;                         
                    break;   
                }
                i++;  
            } 
#ifdef _PC_
            ID3StructAddr = (uint8*)ID3 + vendor_NO*8;   
            *ID3StructAddr = vendor_len - i; 
            for(j=0; ((j<*ID3StructAddr)&&(j<VENDOR_BUF_SIZE)); j++)
            {
                BUFFER[vendor_NO][j] = flac_TagBuf[i+j];     
            }
#else
            ID3StructAddr = (uint8*)ID3 + vendor_NO*5;
            if(ID3StructAddr[0] > 0)
            {   
//              vendor_len = vendor_len - i; 
                buf_size = ID3StructAddr[0];
                for(j=1; j<5; j++)
                {
                    ID3vendorbuf = (ID3vendorbuf<<8) + *(ID3StructAddr + j);   
                }    
                ID3StructAddr = (uint8*)ID3vendorbuf; 
                ID3StructAddr[0] = 3;
                for(j=0; ((j < vendor_len - i)&&(j < buf_size - 1)); j++)
                {
                    ID3StructAddr[j+1] = flac_TagBuf[i+j];    
                }
            }
#endif
        }
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
uint8 check_vendor_name(uint8* buffer)
{
    uint8 i,j=0;
    
    for(i=0; i<25; i++)
    {
        if(buffer[j] == flac_ID3TITLETable[i])
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
    else if(i == 5)
    {
        for(i=0; i<6; i++)
        {
            if(buffer[j+i] != flac_ID3TITLETable[25+i])
            {
                i = 25;
                break;   
            }   
        }   
    }
    else
    {
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
    while ((flac_bufindex < 512) && (i < length))
    {
        databuf[i] = flac_TempBuf[flac_bufindex];
        i++;
        flac_bufindex++;
        if (flac_bufindex == 512)
        {
#ifdef _PC_
            if (fl.Read(flac_TempBuf,512) != 512)
            {
                return FALSE;
            }
#else
            if (FS_FRead(flac_TempBuf, 1, flac_TagHandle) == 0)
            {
                return FALSE;
            } 
#endif
            flac_bufindex = 0;
        }
    }

    flac_filepos += (uint32)length;
    return TRUE;
}
/********************************************************************************
* Description : 重新定位指针并读取当前扇区
*
* Arguments  :  定位后flac_filepos的值
* Returns     :
*            TRUE: 成功,
*            FALSE: 失败.
* Notes       :
*
********************************************************************************/
static BOOL FI_FSeek(uint32 new_flac_filepos)
{
    flac_bufindex = new_flac_filepos%512;
    if((new_flac_filepos>>9) != (flac_filepos>>9))
    {
        flac_filepos  = new_flac_filepos - flac_bufindex;
#ifdef _PC_
        if (fl.Seek(flac_filepos, CFile::begin) != flac_filepos)
        {
            return FALSE;
        }
        if (fl.Read(flac_TempBuf,512) == 0)
        {
            return FALSE;
        }
#else
        if(FS_FSeek((int32)(flac_filepos>>9), 0, flac_TagHandle) == 0)
        {
            return FALSE;
        }
        if(FS_FRead(flac_TempBuf, 1, flac_TagHandle) == 0)
        {
            return FALSE;
        }
#endif
    }
    flac_filepos = new_flac_filepos;
    return TRUE;
}