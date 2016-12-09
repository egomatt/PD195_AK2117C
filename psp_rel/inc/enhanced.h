/*
 *******************************************************************************
 *                ACTOS AP
 *        Enhanced module header file
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 *******************************************************************************
 */

#ifndef _ENHANCED_H
#define _ENHANCED_H

#include "typeext.h"
#include <midware_BS_EH.h>
#include <basal.h>

// Ϊ�˺� __C251__     Added by Weimh 2010.03.10
#include "simulatorpc.h"
//#define _ACTIVE_USERPL


#ifdef __C251__

#define CP_ASCII    1         //����ע�͵���#define�Ǵ����!����ԭ��ASCII��CHS��ͬһ��������û�����ֳ���.
#define CP_CHS      0         //�ڼ���Latin1֮��,��BUG������!�ʱ�����������!       modified by ccm.
#define CP_CHT      2
#define CP_LATIN1   0xa1      //define CP_LATIN1 for supply interface of the unitochar()
//add by dongzh for id3.c

typedef struct
{
    uint8 TIT2_length;
    uint8 *TIT2_buffer;
    uint8 TPE1_length;
    uint8 *TPE1_buffer;
    uint8 TALB_length;
    uint8 *TALB_buffer;
    uint8 TENC_length;
    uint8 *TENC_buffer;
	
	uint8   GENRE_length;	
    uint8    *GENRE_buffer;	//���ɣ���� 
    uint8   TrackNum_length;
    uint8    *TrackNum_buffer; //���� 
    uint16    TrackNum;

	uint32  APIC_offset;
    uint8   APIC_type;
    bool    APIC_flag;

}ID3Info_t;

/****************add for ape flac parsing*********************/
#define ITEM_BUF_SIZE 	30
#define	TEMP_BUF_ADDR	0x0600
typedef struct
{
	uint8	Title_len;
	uint8*	Title_buf;
	uint8	Artist_len;
	uint8*	Artist_buf;
	uint8	Album_len;
	uint8*	Album_buf;
 	uint8	Genre_len;
	uint8*	Genre_buf;
	uint8	Track_len;
	uint8*	Track_buf;
}ID3Info_ext_t;

typedef struct  
{
	uint8	version;

	uint8	DescriptorSize;
	uint8	HeaderSize;
	uint16	SeekTableSize;
	uint8	HeaderDataSize;
	uint32	FrameDataSize;
	
	uint32	BlocksPerFrame;
	uint32	BlocksFinalFrame;
	uint32	TotalFrames;

	uint8	Bits;
	uint8	Channels;
	uint32	SampleRate;
}ApeInfo_t;

typedef struct  
{
	uint16	BlockSize;		//��Ϊfixed-blockʱ��ֵ
	uint32	sample_rate;
	uint8	channel;		//ͨ����  //char
	uint8	bits;			//���������������� //char
}StreamInfo_t;
/*************************************************************/
//typedef struct
//{
//	uint32  APIC_offset;
//    uint8   APIC_type;
//    bool    APIC_flag;
//}ID3Info_ext_t;

//һ������ַ���, ����ڶ���ϵͳʱ����
#define LCD_MAX_CHARS   15

#define USERPL_NUM   101*8
#define LOCATION_SIZE 40
//#define _ACTIVE_USERPL
#define USER_PLAYLIST_SIZE  128
//��ǰ����,Ŀ¼,�ļ�
typedef struct
{
    int8 disk; //'C','D','E','F'-internal, 'H','I','J','K'-externel
    int8 dirLayer;
    uint8 DirNo[8]; //dir no {1, 255, 36, 10....}
    uint16 fselTotal;
    uint16 fselFileNo;
    int32 ClusterNo; //cluster no
    int32 DirEntry;
    uint8 path[12]; //8.3+'\0'
    uint8 filename[12]; //8.3+'\0'
    uint8 *ID3buffer; //change by ybfu 2004-2-4 9:44,ָ��ID3 BUFFER��ַ.
    uint8 APE_Cur;
    uint16 PL_Cur;
}file_location_t;
typedef struct
{
    char flag[6];//"USERPL"
    WORD FirstOffset; //��һ��ƫ����
    WORD LastOffset; //���ƫ����
    WORD PlaylistCount; //Item����
    //WORD FreeHead;   //����ƫ����
}user_playlist_head_t; //��512���ֽ�������

typedef struct
{
    char DelFlag; // 1 normal 2: delete
    char disk; //'C','D','E','F'-internal, 'H','I','J','K'-externel
    BYTE DirNo[8]; //dir no {1, 255, 36, 10....}
    long ClusterNo; //cluster no
    BYTE dirLayer;
    char path[12]; //8.3+'\0'
    char filename[12]; //8.3+'\0'  //39
    WORD PrevOffset; //ǰһ��ƫ����   
    WORD NextOffset; //��һ��ƫ���� ��DelFlag = 1 ʱ ���¼����һ����¼��ƫ���� 
    //��DelFlag = 2 ʱ ���¼����һ���մص�ƫ���� 
    //��Ҫ������ɾ��ʱʹ�ã���ɾ��һ��itemʱ��Nextoffset��¼������Ϣ�����滻ͷ��Ϣ��LastOffset��ͬʱLastOffsetָ���item
    //���ITEMʱ���ȴ�LastOffsetȡ��ƫ������Ȼ���жϵ�ǰ�ļ�DelFlag������2����LastOffset = Nextoffset  
}user_playlist_t; //��64���ֽ�������
#define  NAVI_LONGNAME
#define  NAVI_FILE_TYPE_NUM   3

typedef struct
{
    char PathorFile; //0 : path info 1: folder info  2 file info
    char disk; //'C','D','E','F'-internal, 'H','I','J','K'-externel
    BYTE DirNo[8]; //dir no {1, 255, 36, 10....}
    long ClusterNo; //cluster no
    BYTE dirLayer;
    char path[12]; //8.3+'\0' �ϲ�·����
    long FileNum;
    long DirNum;
    WORD Parentoffset; //16�ı��� ֧��4G���ļ�����
    WORD DelFileNum; //999
#ifdef NAVI_LONGNAME    
    char serv[11+16-2];
#else
    char serv[11-2];
#endif    
}Location_Path_t;
typedef struct
{
    char PathorFile; //0 : path info 1: folder info  2 file info
    char filename[12]; //8.3+'\0'
    WORD Nextoffset; //16�ı��� ֧��4G���ļ�����
    char serv;
#ifdef NAVI_LONGNAME 
    char LN_Flag;
    char longname[47];
#endif    
}file_info_t;

typedef struct
{
    WORD magic;
    WORD Disk_offset;
    WORD SDCARD_offset;
}navi_vars_t;

/* ------�����ļ�tag���� ------*/
bool mp3GetID3Info(const char *filename, ID3Info_t *ID3Info, int string_id);
BYTE ApeGetID3Info(const char *filename, ID3Info_ext_t *ID3Info);
BYTE flacGetID3Info(const uint8 *filename, ID3Info_ext_t *ID3Info);

/* ------ ��ʲ��� ------*/
//bool    openlyricfile(const  char *);
//bool    readlyricfile(const  char *,lyric_con_t *);


//lyric Page Mode defines
#define LYRIC_SCREEN_128_32      1
#define LYRIC_SCREEN_262_100     2	//����320*240��5�и���� cailizhen 2011-3-9 10:13

#define LYRIC_PAGE_16_2        2
#define LYRIC_PAGE_21_4        1

//lyric language defines
#define LYRIC_CSET_SINGLE    1
#define LYRIC_CSET_DOUBLE    2
#define TOTALDIRLAYER       8

// Initialize lyric sequencer
BOOL lyricInit(uchar ScreenMode, uchar CharSet);

// load the lyric matching the music file specified by strMusicFile. position initialized.
// return the page mode , 0 means fail.
BYTE lyricLoad(const BYTE * strMusicFile, const BYTE * strID3);

// Get the next page of lyric, limited len
bool lyricGetNext( BYTE * strLyric, uchar len);

//Get the previous page of lyric, limited len
bool lyricGetPrev( BYTE * strLyric, uchar len);

//Query if lTime is out of the duration of current page.
BOOL lyricQueryOutof(uint32 lTime);
BOOL lyricGetPageTime(uint32 *CurTime, uint32 *NextTime);

//Seek for the page lyric according to specified time, limited len, position changed.
bool lyricSeekFor( char *strlyric, const uchar len, const uint32 lTime);

// Query if exist lyric matching the music file specified by strMusicFile.
bool lyricQueryExist(const BYTE * strMusicFile);

/* ------ �ļ�ѡ����� ------*/

//////file selector defines
// max select scope defines
#define MAX_SUB_DIR 8
#define FSEL_MAX_FILENUM        999
#define FSEL_MAX_DIRNUM         100
#define FSEL_MUSIC_EXTNUM       3
#define FSEL_VOICE_EXTNUM       1
#define FSEL_DSC_EXTNUM       3
#define FSEL_TXT_EXTNUM       1
#define FSEL_AMV_EXTNUM       2
#define FSEL_ALL_EXTNUM       1

// select type defines
#define FSEL_TYPE_MUSIC         0
#define FSEL_TYPE_VOICE         1
#define DIRECTORY_ALL           2
#define FSEL_TYPE_DSC       3
#define FSEL_TYPE_TXT       4
#define FSEL_TYPE_AMV       5
#define FSEL_TYPE_ALL       6
#define FSEL_TYPE_DIR       7
// select playlist or common dir
#define FSEL_TYPE_COMMONDIR     0
#define FSEL_TYPE_PLAYLIST      1
#define FSEL_TYPE_USERPL        2

// select mode defines
#define FSEL_ALL_SEQUENCE       0
#define FSEL_ALL_REPEATONE      1
#define FSEL_DIR_SEQUENCE       2
#define FSEL_DIR_REPEATALL      3
#define FSEL_ALL_REPEATALL      4
#define FSEL_DIR_RANDOM         5
#define FSEL_DIR_INTRO          6
#define FSEL_DIR_END            7

// delete type defines
#define FSEL_DEL_FILE           0
#define FSEL_DEL_DIRFILE        1

// error information defines
#define FSEL_ERR_OUTOF          1
#define FSEL_ERR_EMPTY          2
#define FSEL_ERR_SETDIR         3
#define FSEL_ERR_SETFILE        4
#define FSEL_ERR_EXCEPTION      5

#define FSEL_ERR_FS             20
#define FSEL_ERR_FAIL           21
#define FSEL_ERR_DISK           22
#define FSEL_ERR_PLAYLIST       23

//these functions is used by external user
#define FSELInit            fselInit
#define FSELPlayListEnt     fselPlayListEnt
#define FSELPlayListExit    fselPlayListExit
#define FSELGetNextFile     fselGetNextFile
#define FSELGetPrevFile     fselGetPrevFile
#define FSELGetCurFNameL    fselGetCurFNameL

#define FSELDelFile         fselDelFile
#define FSELSetMode         fselSetMode
#define FSELGetTotal        fselGetTotal
#define FSELGetNo           fselGetNo
#define FSELSetLocation     fselSetLocation
#define FSELGetLocation     fselGetLocation
#define FSELGetErrID        fselGetErrID

//these function is used by internal user
#define FSELGetNext         fselGetNextFile
#define FSELGetPrev         fselGetPrevFile
#define FSELNextDir         fselNextDir
#define FSELPrevDir         fselPrevDir
#define FSELGetByNO         fselGetByNO
// the 18 following function is used by external user
// Initialize fsel Module, Must call it before call other fsel Module functions
bool fselInit(uint8 type, uint8 mode, uint8 PlayListOrCommonDir, uint8 CardOrFlash);
bool fselGetNextFile(uint8 *filename);
bool fselGetPrevFile(uint8 *filename);
bool fselGetCurFNameL(uint8 *longfilename, uint8 iLen);
uint8 fselSetLocation(file_location_t *location);
bool fselGetLocation(file_location_t *location);
bool fselSetMode(uint8 mode);
uint16 fselGetTotal(void);
uint16 fselGetNo(void);
int8 fselGetErrID(void);

extern bool fselGetByNO(uint8 *strFile, const uint16 Num);
int GenNewName(file_location_t *loation);
uint16 FS_CD_DIR_SON(const uint8 *);
uint16 FS_CD_DIR_PARENT(void);

/* ------ UNICODE ת�� ------*/
bool unitochar(char *str,int len,unsigned char encode); //change by ybfu


BYTE mp3CheckType(const uint8 *filename, uint32 *pMp3frameoffset,
                  time_t *pTotaltime, audio_format_t *formatParameter);
BYTE wavCheckType(const uint8 *filename, uint32 *pWavframeoffset, 
                  time_t *pTotaltime, audio_format_t *formatParameter);
BYTE wmaCheckType(const uint8 *filename, uint32 *pWavframeoffset, 
                  time_t *pTotaltime, audio_format_t *formatParameter, ID3Info_t *ID3Info);
//BYTE wmaCheckType_ext(const uint8 *filename, uint32 *pWavframeoffset, 
//                 time_t *pTotaltime, audio_format_t *formatParameter, ID3Info_t *ID3Info);
BYTE ApeGetInfo(const uint8 *filename, uint32 *pApeframeoffset, time_t *pTotaltime,  
                  audio_format_t *formatParameter, ID3Info_ext_t *ID3Info);
BYTE flacGetInfo(const uint8 *filename, uint32 *pFlacframeoffset, 
                  time_t *pTotaltime, audio_format_t *formatParameter, ID3Info_ext_t *ID3Info);

#endif
#endif  /* _ENHANCED_H*/

