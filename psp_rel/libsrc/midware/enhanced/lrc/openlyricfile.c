/*
 *******************************************************************************
 *                ACTOS AP
 *        enhanced module impletementation
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 *******************************************************************************
 */

#include <actos.h>
#include <enhanced.h>
#include "ReadLyricFile.h"
#include <stringH.h>

#pragma name(MW_EH_LRC_SUB1)
uint8 far LName[LYRIC_FILENAME_SIZE+2];    //歌曲长名buffer

//extern bool lyricInitflag;//为真表示已初始化
//该段占用解码部分的buffer，暂定为9000开始的地方
#pragma userclass(near = RESULT_BUFFER)
lyricSaveFormat_T lyricSave;
#pragma userclass(near = default)

#pragma renamecode(MW_EH_LRC_SUB1)

/********************************************************************************
 * Description : Open lyric file according to specified music file.
 *
 * Arguments  :
 shortfilename:  dos 8.3 file name of music file
 pagemode:  page mode
 * Returns     :
 0:         fail
 1~2:     file handle
 0xffff:    the lyric already loaded, needn't load again.
 * Notes       :  only support 31 char length long file name now, this is limited by file system
 *
 ********************************************************************************/
/*
HANDLE lyricOpenfile(const BYTE *shortfilename)
{
    uint8 result, index;
    uint8 strLong[LYRIC_FILENAME_SIZE]; //临时放长文件名

    ClearWatchDog();
    memset(strLong, 0x0, LYRIC_FILENAME_SIZE);
    memcpy(strLong, shortfilename, 11);
    strLong[11] = 0x00;

    if (((strLong[6] == '~') || (strLong[5] == '~'))
            && ((result = FS_GetName(strLong, ((LYRIC_FILENAME_SIZE / 2) - 1))) != 0)//may true name is 1~1.mp3
            && (result <= (LYRIC_FILENAME_SIZE / 2)))
    {
        index = (result - 5) << 1; //.lrc0
        if (strLong[index] == '.')
        {
            strLong[index + 2] = 'L';
            strLong[index + 4] = 'R';
            strLong[index + 6] = 'C';
            strLong[index + 8] = 0;
            memcpy(lyricSave.lyricFileName, strLong, LYRIC_FILENAME_SIZE);
            return FS_FOpen(strLong, FS_OPEN_LN_NORMAL);
        }
        else
        {
            //短名一样， 长名可能不一样， 所以下面的语句有隐患，但可能性极低。
            memcpy(lyricSave.lyricFileName, strLong, LYRIC_FILENAME_SIZE);
            memcpy(strLong, shortfilename, 8);
            strLong[8] = 'L';
            strLong[9] = 'R';
            strLong[10] = 'C';
            strLong[11] = 0;
            return FS_FOpen(strLong, FS_OPEN_NORMAL);
        }

    }
    else
    {
        strLong[8] = 'L';
        strLong[9] = 'R';
        strLong[10] = 'C';
        strLong[11] = '\0';
        memcpy(lyricSave.lyricFileName, strLong, LYRIC_FILENAME_SIZE);
        return FS_FOpen(strLong, FS_OPEN_NORMAL);
    }
}
*/
HANDLE lyricOpenfile(const BYTE *shortfilename)
{
    uint8 strLong[12], LNameSize;
    uint8 *LNOffset;
    bool   is_flac;	
    HANDLE ret_handle;
    uint8 LGName[LYRIC_FILENAME_SIZE+2];    //歌曲长名buffer
    ClearWatchDog();

    memcpy(LGName, shortfilename, 12);
    LNameSize = FS_GetName(LGName,LYRIC_FILENAME_SIZE/2);
    if(LNameSize == 0)  //不存在长名
    {
        memset(strLong, 0, 12);
        memcpy(strLong, shortfilename, 11);
        strLong[8] = 'L';
        strLong[9] = 'R';
        strLong[10] = 'C';
        strLong[11] = '\0';
        return FS_FOpen(strLong, FS_OPEN_NORMAL);
    }
    else
    {
        is_flac = FALSE;
        LNOffset = LGName + LNameSize*2 - 10;        //取后缀名开始地址
	 //判断是否FLAC类型，当前支持的歌曲类型，只有FLAC后缀名长度不同	
	 if(((*LNOffset) == 'F') || ((*LNOffset) == 'f'))
	 {
	      LNOffset +=2;
	      if(((*LNOffset) == 'L') || ((*LNOffset) == 'l'))
	      {
	          LNOffset +=2;
		   if(((*LNOffset) == 'A') || ((*LNOffset) == 'a'))  
		   {
		        LNOffset +=2;
			 if(((*LNOffset) == 'C') || ((*LNOffset) == 'c'))  	
			 {
			         is_flac = TRUE;
			 }
		   }
	      }
	 }

	 if( is_flac)
	 {
	        LNOffset = LGName + LNameSize*2 - 10;	//取后缀名开始地址
	 }
	 else
	 {
	        LNOffset = LGName + LNameSize*2 - 8;	//取后缀名开始地址
	 }
        
       /* for(i=0; i<LNameSize; i++)
		{
		    if((LGName[i*2] == 0x2e) && (LGName[i*2+1] == 0x00))
			{
				i++;
				LNOffset = LGName + i*2;
				break;
			}
		}*/

        *LNOffset = 'L';
        *(LNOffset + 2) = 'R';
        *(LNOffset + 4) = 'C';
        *(LNOffset + 6) = 0;
        ret_handle = FS_FOpenbyLN(LGName,0x00,strLong);
        if(ret_handle == NULL)
        {
            *LNOffset = 'l';
            *(LNOffset + 2) = 'r';
            *(LNOffset + 4) = 'c';
            *(LNOffset + 6) = 0;
            ret_handle = FS_FOpenbyLN(LGName,0x00,strLong);
        }
    
        return ret_handle;
    }
}