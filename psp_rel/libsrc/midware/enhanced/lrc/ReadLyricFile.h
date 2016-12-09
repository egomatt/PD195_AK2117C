#ifndef         _readlyricfile_H
#define         _readlyricfile_H

//#include        "chip_cfg.h"
#include        <sysdef.h>
#include        <actos.h>

#define MINUTE_INDEX 0
#define SECONDE_INDEX 1
#define MILSECONDE_INDEX 2
#define CONTENT_IDNEX 3
#define MAX_TIME 0xff000000

typedef struct
{
    uchar min;
    uchar sec;
    uchar p1s;
    uchar buffaddr_High;
    uchar buffaddr_Low;
} LrcTime_t;

#ifdef ColorSrcreen

//===================��ʷ���ʱ�������(��ʾ����ʱ������ܳ������ֵ)============עҪ�ñ�׼ʱ��д��
#define         MAX_ONE_SCREEN_TIME_MIN        0      //����ָ������   ��0-59�� 
#define         MAX_ONE_SCREEN_TIME_SEC        5      //����ָ�������� ��0-59�� 
#define         MAX_ONE_SCREEN_TIME_100MS      5      //����ָ����100������ 9 ��ʾ 900ms��0-9��
//=======================================================================

//<><><><><><><><><><><><><><><><><><> [ UP26A36_WMA ] <><><><><><><><><><><><><><><><><><><><><><>
#define         LCD_DIS_CHINESE_LINE      1    //Ҫ����LCD����ʾ����Ϊ���� 
#define         LCD_DIS_ENGLISH_LINE      1	//LCD_DIS_CHINESE_LINE*2  
#define         CHINESE_COL             16  //12	//16   //���ַ��������
#define         CHINESE_LINE            16  //12	//16  //���ַ��������
#define         ASCII_COL               8   //6 //Ascii ���������
#define         ASCII_LINE              16  //8 //Ascii ���������
#define         ICON_WIDTH_DOT           0  //ͼ�����Ŀ��(up26a33Ϊ12)
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>


//define every table address in VRAM
#define LYRIC_FILENAME_ADDR VM_LYRIC
#define LYRIC_FILENAME_SIZE 100
#define LYRIC_PAGEMODE_ADDR  LYRIC_FILENAME_ADDR + LYRIC_FILENAME_SIZE
#define LYRIC_PAGEMODE_SIZE 1
#define TIME_TABLE_ADDR     LYRIC_PAGEMODE_ADDR + LYRIC_PAGEMODE_SIZE
#define TIME_TABLE_SIZE     0x800-LYRIC_FILENAME_SIZE-LYRIC_PAGEMODE_SIZE
#define CONTENT_TABLE_ADDR  TIME_TABLE_ADDR+TIME_TABLE_SIZE
#define CONTENT_TABLE_SIZE  0x1000
#define TIME_RECORD_SIZE   5

#ifdef __C251__
//bool readlyricfileMSA(const char *);
HANDLE lyricOpenfile(const BYTE *shortfilename);
#endif

#else    //#ifdef ColorSrcreen
//===================��ʷ���ʱ�������(��ʾ����ʱ������ܳ������ֵ)============עҪ�ñ�׼ʱ��д��
#define         MAX_ONE_SCREEN_TIME_MIN        0                   //����ָ������   ��0-59��                     //
#define         MAX_ONE_SCREEN_TIME_SEC        5                   //����ָ�������� ��0-59��                     //��ʾ5.5��
#define         MAX_ONE_SCREEN_TIME_100MS      5                   //����ָ����100������ 9 ��ʾ 900ms��0-9��      //
//=======================================================================
#ifdef         UNICODE_SUPPORT

//<><><><><><><><><><><><><><><><><><> [ UP26A36_WMA ] <><><><><><><><><><><><><><><><><><><><><><>
#define         LCD_DIS_CHINESE_LINE      1                    //Ҫ����LCD����ʾ����Ϊ����(һ��ASCII����Ϊ���ֵ�2��)(up26a33 Ϊ2��)
#define         LCD_DIS_ENGLISH_LINE      1 //LCD_DIS_CHINESE_LINE*2  //Ҫ����LCD����ʾӢ��Ϊ����(һ��ASCII����Ϊ���ֵ�2��)(up26a33 Ϊ4��)
#define         CHINESE_COL             16                   //���ַ��������
#define         CHINESE_LINE            16                   //���ַ��������
#define         ASCII_COL               8                    //Ascii ���������
#define         ASCII_LINE              16                    //Ascii ���������
#define         ICON_WIDTH_DOT           12                   //ͼ�����Ŀ��(up26a33Ϊ12)
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

#else
//<><><><><><><><><><><><><><><><><><> [ UP26A36_WMA ] <><><><><><><><><><><><><><><><><><><><><><>
#define         LCD_DIS_CHINESE_LINE      1                    //Ҫ����LCD����ʾ����Ϊ����(һ��ASCII����Ϊ���ֵ�2��)(up26a33 Ϊ2��)
#define         LCD_DIS_ENGLISH_LINE      LCD_DIS_CHINESE_LINE*2  //Ҫ����LCD����ʾӢ��Ϊ����(һ��ASCII����Ϊ���ֵ�2��)(up26a33 Ϊ4��)
#define         CHINESE_COL             16                   //���ַ��������
#define         CHINESE_LINE            16                   //���ַ��������
#define         ASCII_COL               6                    //Ascii ���������
#define         ASCII_LINE              8                    //Ascii ���������
#define         ICON_WIDTH_DOT           12                   //ͼ�����Ŀ��(up26a33Ϊ12)
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
#endif

//define every table address in VRAM
#define LYRIC_FILENAME_ADDR VM_LYRIC
#define LYRIC_FILENAME_SIZE 100
#define LYRIC_PAGEMODE_ADDR  LYRIC_FILENAME_ADDR + LYRIC_FILENAME_SIZE
#define LYRIC_PAGEMODE_SIZE 1
#define TIME_TABLE_ADDR     LYRIC_PAGEMODE_ADDR + LYRIC_PAGEMODE_SIZE
#define TIME_TABLE_SIZE     (0x800-LYRIC_FILENAME_SIZE-LYRIC_PAGEMODE_SIZE)
#define CONTENT_TABLE_ADDR  TIME_TABLE_ADDR+TIME_TABLE_SIZE
#define CONTENT_TABLE_SIZE  0x1000
#define TIME_RECORD_SIZE   5

#ifdef __C251__
//bool readlyricfileMSA(const char *);
HANDLE lyricOpenfile(const BYTE *shortfilename);
#endif

#endif  //#ifdef ColorSrcreen
typedef struct lyricSaveFormat
{
    UCHAR lyricFileName[LYRIC_FILENAME_SIZE];
    UCHAR lyricPageMode;//[LYRIC_PAGEMODE_SIZE];
    UCHAR LyricTimeAddrBuffer[TIME_TABLE_SIZE];
    UCHAR LyricStrBuffer[CONTENT_TABLE_SIZE];
} lyricSaveFormat_T;

#endif//_readlyricfile_H
