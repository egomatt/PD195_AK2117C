/*
 ******************************************************************************
 *                               AS211A
 *                            Module: music
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: sub functions 2 of music
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xusong    2010-07-15 15:00     1.0             build this file
 ******************************************************************************
 */
#include "ap_music.h"
#define MusicExtNum 8

extern bool Mp3Get_ldb_Load(const uint8 * strFile, uint8 * strID3);
extern bool WMAGet_ldb_Load(const uint8 * strFile, uint8 * strID3);
extern void Deal_UniID3Infor(bool mp3flag, bool longnameflag, bool uni_flag);
extern void MakeShortName(uint8 *str);
extern void DealBETagBuffer(void);
extern void DealMP3ANSI(void);

#pragma name(AP_SUB_FUN2)

const uint8 ExtMusicstr[MusicExtNum][4] =
{ "MP1", "MP2", "MP3", "WMA", /*"ASF",*/"WMV", "WAV", "APE", "FLA" };

uint8 far LyricID3Buf[92];
ID3Info_t far ID3;
ID3Info_ext_t far ID3_ext; 
void DealTheFirstSpace(uint8 *String, uint8 Size);
extern void DealTagUtf8(void);
/*
 ******************************************************************************
 *             Music_type_t CheckMusicType(uint8   *filename)
 *
 * Description : 检测当前是哪种类型的MUSIC文件
 *
 * Arguments   : uint8   *filename  文件名
 *
 * Returns     : Music_type_t 文件类型
 *
 * Notes       :
 *
 ********************************************************************************
 */
Music_type_t CheckMusicType(uint8 * filename)
{
    uint8 cnt;
    uint8 ext[4];
    Music_type_t ret;
    memcpy(ext, filename + 8, 3);
    ext[0] = ((ext[0] >= 'a') && (ext[0] <= 'z')) ? (ext[0] - 'a' + 'A') : ext[0];
    ext[1] = ((ext[1] >= 'a') && (ext[1] <= 'z')) ? (ext[1] - 'a' + 'A') : ext[1];
    ext[2] = ((ext[2] >= 'a') && (ext[2] <= 'z')) ? (ext[2] - 'a' + 'A') : ext[2];
    ext[3] = 0;

    for (cnt = 0; cnt < Music_TypeMax; cnt++)
    {
        if (strcmp(ext, ExtMusicstr[cnt]) == 0)
        {
            ret = Music_NON + cnt + 1;
            if ((ret == Music_MP1) || (ret == Music_MP2))
            {
                return Music_MP3;
            }
            else
            {
                return ret;
            }
        }
    }
    return Music_NON;
}


/*
 ********************************************************************************
 *             uint16 GetInformation(void)
 *
 * Description : 读取歌曲信息,歌词
 *
 * Arguments   : void
 *
 * Returns     : int
 *
 * Notes       :
 *
 ********************************************************************************
 */
void GetInformation(void)
{
    bool unicode_flag = FALSE;
	
	#ifdef OPT_Sup_LRC_fun
    uint32 lEntry;
	#endif
	
    bool mp3_unicdoe = FALSE;
    bool long_name_flag = FALSE;
    uint8 result;
	
	#ifdef OPT_Sup_LRC_fun
    uint8 g_lyric_mod;
	#endif
	
    audio_format_t pFormat;

    //matt140206#2
    SetPLL(PLL_80MHZ);
            
    g_currentMusicType = CheckMusicType(g_music_vars.location.filename);
    memset(g_TagInfoBuf, 0, 92);
    memset(&ID3, 0, sizeof(ID3Info_t));
    memset(&ID3_ext, 0, sizeof(ID3Info_ext_t));
    ID3.TIT2_length = 30;
    ID3.TPE1_length = 30;
    ID3.TALB_length = 30;
	  ID3.GENRE_length = 0;
    ID3.TrackNum_length = 0;
	  ID3.TENC_length = 0;
	ID3_ext.Title_len = 30;
    ID3_ext.Artist_len = 30;
    ID3_ext.Album_len = 30;
    ID3.TIT2_buffer = &g_TagInfoBuf[0];
    ID3.TPE1_buffer = &g_TagInfoBuf[30];
    ID3.TALB_buffer = &g_TagInfoBuf[60];
    ID3.TENC_buffer = ID3.GENRE_buffer = ID3.TrackNum_buffer = &g_TagInfoBuf[90];
	  ID3.GENRE_buffer = &g_TagInfoBuf[90];
    ID3.TrackNum_buffer = &g_TagInfoBuf[90];	
    ID3.TENC_buffer = &g_TagInfoBuf[90];
    ID3_ext.Title_buf = &g_TagInfoBuf[0];
    ID3_ext.Artist_buf = &g_TagInfoBuf[30];
    ID3_ext.Album_buf = &g_TagInfoBuf[60];
    ID3_ext.Genre_buf = ID3_ext.Track_buf = &g_TagInfoBuf[90];
    g_Openparam.typeerror = 0;
    switch (g_currentMusicType)
    {
        case Music_WMA:
        g_Openparam.typeerror = wmaCheckType(/*g_music_vars.location.filename*/NULL, &g_Openparam.musicframeoffset,
                &g_Openparam.musictotaltime, &pFormat, &ID3);

        if ((g_Openparam.typeerror == 0)/*&&(wmaGetTAGInfo(g_music_vars.location.filename, &ID3))*/) //正常文件才操作
        {
            if ((*(uint16 *) g_TagInfoBuf != 0x0000) || (*(uint16 *) (g_TagInfoBuf + 30) != 0x0000)
                    || (*(uint16 *) (g_TagInfoBuf + 60) != 0x0000))
            {
                unicode_flag = TRUE;
            }
        }
        else
        {
            g_TagInfoBuf[0] = 0x0;
        }
/*#ifdef LDB_LRC//支持LDB歌词显示功能
        if(WMAGet_ldb_Load(g_music_vars.location.filename,g_TagInfoBuf))
        {
            g_LDBFlag |=(1<<1);
        }
        else
        {
            g_LDBFlag &=~(1<<1);
        }
#endif*/
        break;


        case Music_Wav:
        g_Openparam.typeerror = wavCheckType(/*g_music_vars.location.filename*/NULL, &g_Openparam.musicframeoffset,
                &g_Openparam.musictotaltime, &pFormat);
        break;

        
        case Music_MP3:
        g_Openparam.typeerror = mp3CheckType(/*g_music_vars.location.filename*/NULL, &g_Openparam.musicframeoffset,
                &g_Openparam.musictotaltime, &pFormat);
        BankFlush(FLUSH_BANK_BASAL | FLUSH_BANK_CODEC);
        if ((g_Openparam.typeerror == 0) && (mp3GetID3Info(/*g_music_vars.location.filename*/NULL, &ID3,
                g_comval.langid)))
        {
			DealBETagBuffer();		//处理大端unicode字符串，转换为小端 

            DealTagUtf8();
            /*
            if (((g_TagInfoBuf[0] == 0xff) && (g_TagInfoBuf[1] == 0xfe)) 
             && ((g_TagInfoBuf[30] == 0xff) && (g_TagInfoBuf[31] == 0xfe)) 
             && ((g_TagInfoBuf[60] == 0xff) && (g_TagInfoBuf[61] == 0xfe))) // is unicode ?
            {
                mp3_unicdoe = TRUE;
                unicode_flag = TRUE;
            }
            else
            {
                DealMP3ANSI();
            }
            */
            if (((!((g_TagInfoBuf[0] == 0xff) && (g_TagInfoBuf[1] == 0xfe))) && (g_TagInfoBuf[0] != 0)) 
             || ((!((g_TagInfoBuf[30] == 0xff) && (g_TagInfoBuf[31] == 0xfe))) && (g_TagInfoBuf[30] != 0)) 
             || ((!((g_TagInfoBuf[60] == 0xff) && (g_TagInfoBuf[61] == 0xfe))) && (g_TagInfoBuf[60] != 0))) // is unicode ?
            {
                DealMP3ANSI();
            }
            else
            {
                mp3_unicdoe = TRUE;
                unicode_flag = TRUE; 
            }
        }
        else
        {
            g_TagInfoBuf[0] = 0x0;
        }

/*#ifdef LDB_LRC//支持LDB歌词显示功能
        if(Mp3Get_ldb_Load(g_music_vars.location.filename,g_TagInfoBuf))
        {
            g_LDBFlag |=(1<<1);
        }
        else
        {
            g_LDBFlag &=~(1<<1);
        }
#endif*/
        break;
		
		#ifndef PC
        case Music_APE:
        //SetPLL(PLL_80MHZ);//matt140206#2
        g_Openparam.typeerror = ApeGetInfo(NULL, &g_Openparam.musicframeoffset, 
                &g_Openparam.musictotaltime, &pFormat, &ID3_ext);
        //SetPLL(DEFAULT_FREQ);
        BankFlush(FLUSH_BANK_BASAL | FLUSH_BANK_CODEC);
        
        if (g_Openparam.typeerror == 0) //正常文件才操作
        {
            DealTagUtf8();//UTF-8 转 Unicode
            if ((*(uint16 *) g_TagInfoBuf != 0x0000) 
             || (*(uint16 *) (g_TagInfoBuf + 30) != 0x0000)
             || (*(uint16 *) (g_TagInfoBuf + 60) != 0x0000))
            {
                unicode_flag = TRUE;
            }
        }
        else
        {
            g_TagInfoBuf[0] = 0x0;
        }
        break;
            
        case Music_FLAC:
        //SetPLL(PLL_80MHZ);//matt140206#2
        g_Openparam.typeerror = flacGetInfo(NULL, &g_Openparam.musicframeoffset, 
                &g_Openparam.musictotaltime, &pFormat, &ID3_ext);
        //SetPLL(DEFAULT_FREQ);
        BankFlush(FLUSH_BANK_BASAL | FLUSH_BANK_CODEC);
        
        if (g_Openparam.typeerror == 0) //正常文件才操作
        {
            DealTagUtf8();//UTF-8 转 Unicode
            if ((*(uint16 *) g_TagInfoBuf != 0x0000) 
             || (*(uint16 *) (g_TagInfoBuf + 30) != 0x0000)
             || (*(uint16 *) (g_TagInfoBuf + 60) != 0x0000))
            {
                unicode_flag = TRUE;
            }
        }
        else
        {
            g_TagInfoBuf[0] = 0x0;
        }
        break;
#endif


        default:
        g_TagInfoBuf[0] = 0x0;
        break;
    }

    if (g_Openparam.typeerror == 0)
    {
        memcpy(TimeRateData_sav.TotalTime_sav, &g_Openparam.musictotaltime, sizeof(time_t));
        if ((pFormat.bit_rate[0] < '0') || (pFormat.bit_rate[0] > '9') || (pFormat.bit_rate[1] < '0')
                || (pFormat.bit_rate[1] > '9') || (pFormat.bit_rate[2] < '0') || (pFormat.bit_rate[2] > '9'))
        {
            pFormat.bit_rate[0] = '9';
            pFormat.bit_rate[1] = '9';
            pFormat.bit_rate[2] = '9';
        }
        TimeRateData_sav.Bitrate_sav[0] = (uint8) pFormat.bit_rate[0]; //保存用于跟读对比时显示
        TimeRateData_sav.Bitrate_sav[1] = (uint8) pFormat.bit_rate[1];
        TimeRateData_sav.Bitrate_sav[2] = (uint8) pFormat.bit_rate[2];
    }
    else
    {
        memset(&TimeRateData_sav.TotalTime_sav, 0, sizeof(time_t));
        TimeRateData_sav.Bitrate_sav[0] = '0'; //保存用于跟读对比时显示
        TimeRateData_sav.Bitrate_sav[1] = '0';
        TimeRateData_sav.Bitrate_sav[2] = '0';

    }

    //if (!unicode_flag)//---处理前面空格 //matt120509#2
    //    DealTheFirstSpace(g_TagInfoBuf, sizeof(g_TagInfoBuf));
    
    //---处理没有TAG信息的情况
    tag_is_unicode = 0;
	#ifdef Greenhouse
	unicode_flag = FALSE;
    g_TagInfoBuf[0] = 0;
	#endif
    if (!unicode_flag)
    {
        if (g_TagInfoBuf[0] == 0)
        {
            memset(g_TagInfoBuf, 0, 92);
            memcpy(g_TagInfoBuf, g_music_vars.location.filename, 12);
            result = (uint8) FS_GetName(g_TagInfoBuf, 45); //取出长名   //matt140815#1 取长名时字符数错误，因为是unicode，应该是buff的一半。
            if (result != 0)
            {
                long_name_flag = TRUE;
                unicode_flag = TRUE;
            }
            else
            {
                memcpy(g_TagInfoBuf, g_music_vars.location.filename, 12);
                MakeShortName(g_TagInfoBuf);
            }
            tag_is_unicode |= 0x80;
        }
    }
    /*
    if (unicode_flag != 0)
    {
        Deal_UniID3Infor(mp3_unicdoe, long_name_flag, unicode_flag);
    }
    else
    {
        for (result = 0; result < sizeof(g_TagInfoBuf); result++)
        {
            if (g_TagInfoBuf[result] == 0)
            {
                g_TagInfoBuf[result] = ' ';
                g_TagInfoBuf[result + 1] = 0;
                break;
            }
        }

    }
    */
    if(unicode_flag )
    {
        tag_is_unicode |= UNICODELANGUAGE;
    }
    else
    {
        tag_is_unicode |= 0;
    }

#ifdef OPT_Sup_LRC_fun//支持歌词显示功能
    if(/*(!(g_LDBFlag&(1<<1)))&&*/(g_Openparam.typeerror == 0))
    {
        lEntry = FS_GetCurDirEntry();//文件项在当前目录项中的偏移
        g_lyricflag = lyricQueryExist(g_music_vars.location.filename);
        if (g_lyricflag)
        {
            if((g_TagInfoBuf[0] == 0xff) && (g_TagInfoBuf[1] == 0xfe))
            {
                memcpy(LyricID3Buf,&g_TagInfoBuf[2],sizeof(LyricID3Buf)-2);
                unitochar(LyricID3Buf, sizeof(LyricID3Buf)-2, g_comval.langid);
            }
            else
            {
                memcpy(LyricID3Buf,g_TagInfoBuf,sizeof(LyricID3Buf));
            }
            g_lyric_mod = lyricLoad(g_music_vars.location.filename, LyricID3Buf);
        }
        if (g_lyric_mod == 0)
        {
            g_lyricflag = FALSE;
        }
        FS_GetOrSetDir_Exfat(0, g_music_vars.location.ClusterNo, EXFAT_SET_CURDIR);
        FS_SetCurDirEntry(lEntry);
    }
    else if(g_Openparam.typeerror == 0) /*存在LDB格式歌词*/
    {
        g_lyricflag = TRUE;
    }
    else
    {
        g_lyricflag = FALSE;
    }
#endif //OPT_Sup_LRC_fun

    //matt140206#2
    SetPLL(DEFAULT_FREQ);
}
