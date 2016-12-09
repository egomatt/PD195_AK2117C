/********************************************************************************
 *                              USDK 5102
 *                            Module: POWER MANAGER
 *                 Copyright(c) 2001-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *    wuyueqian     2009-9-07 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * file
 * brief
 * Author   wuyueqian
 * par
 *
 *      Copyright(c) 2001-2007 Actions Semiconductor, All Rights Reserved.
 *
 * version 1.0
 * date  2008/6/12
 *******************************************************************************/

#include <actos.h>
#include <enhanced.h>
#include <stringH.h>
#include <lang_id.h>

#pragma name(MW_EH_ID3)

#pragma renamecode(MW_EH_ID3)

#define ID3_BUFFER 0x0600//0x1800
extern char *TempBuf; //read file buffer

char far TagBuf[128];
bool FectchID3Info(const char *filename, ID3Info_t *ID3Info);
extern BOOL FetchID3_V2info(handle TagFileHandle, ID3Info_t *ID3Info);
void DealStringEnd(uint8 *str, int string_id);
void DealStrEndSpace(char *str); 
extern bool SelectGENRE(ID3Info_t *ID3Info,BYTE GENREindex);

/*
 ********************************************************************************
 *             mp3GetTiltle
 * Description :
 * Arguments   :  string_id is language id(0:���� 1 Ӣ�� 2 ����),���ڶ����Ĳ������ַ����д���
 ��Ϊ���ȡ30���ַ�������Щʱ���������п��ܻ��Ӣ�ĵġ���˻��������ַ�ֻ�Ǻ���һ������
 * Returns     : ID3ExistFlag = 0 ��ʾû��ID3��Ϣ��1��ʾ��
 * Notes       :
 ********************************************************************************
 */
#ifndef PC
bool smp3GetID3Info(const char *filename, ID3Info_t *ID3Info, int string_id)
#else
bool mp3GetID3Info(const char *filename, ID3Info_t *ID3Info, int string_id)
#endif
{
    bool ID3ExistFlag = FALSE;

    ClearWatchDog();

    if (!FectchID3Info(filename, ID3Info))
    {
        ClearWatchDog();
        return FALSE;
    }

    //���洦��ó��� ��Ϣ��һЩ�������
    if (ID3Info->TIT2_length != 0)
    {
        DealStringEnd(ID3Info->TIT2_buffer, string_id);
        DealStrEndSpace(ID3Info->TIT2_buffer);
        if (ID3Info->TIT2_buffer[0] != 0)
        {
            ID3ExistFlag = TRUE;
        }
    }

    if (ID3Info->TPE1_length != 0)
    {
        DealStringEnd(ID3Info->TPE1_buffer, string_id);
        DealStrEndSpace(ID3Info->TPE1_buffer);
        if (ID3Info->TPE1_buffer[0] != 0)
        {
            ID3ExistFlag = TRUE;
        }
    }

    if (ID3Info->TALB_length != 0)
    {
        DealStringEnd(ID3Info->TALB_buffer, string_id);
        DealStrEndSpace(ID3Info->TALB_buffer);
        if (ID3Info->TALB_buffer[0] != 0)
        {
            ID3ExistFlag = TRUE;
        }
    }

    if (ID3Info->TENC_length != 0)
    {
        DealStringEnd(ID3Info->TENC_buffer, string_id);
        DealStrEndSpace(ID3Info->TENC_buffer);
        if (ID3Info->TENC_buffer[0] != 0)
        {
            ID3ExistFlag = TRUE;
        }
    }

    ClearWatchDog();
    return ID3ExistFlag;

}

/*
 ********************************************************************************
 *             DealStringEnd
 *
 * Description :  ��������β�Ĳ�������������
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void DealStringEnd(uint8 *str, int16 string_id)
{
    BYTE i;
    bool half;

    if ((string_id != CHINESE_SIMPLIFIED) && (string_id != CHINESE_TRADITIONAL))
    {
        return;
    }

    if (*((uint16 *)str) == 0xfffe)
    {
        return;
    }
    if ( *str == 0x03 )
    {
        return;
    }

    //���ǿմ����ô���
    if (str[0] == 0) //0 means '\0'
    {
        return;
    }

    for (i = 0; i < 255; i++)
    {
        if (str[i] >= 0x80)
        {
            half = TRUE;
        }
        else
        {
            half = FALSE;
        }

        //�Ƿ񽫽���
        if (str[i + 1] == 0)
        {
            if (FALSE != half)
            {
                str[i] = ' ';
            }
            return;
        }

        if (FALSE != half)
        {
            i++;
            if (str[i + 1] == 0) //�պ���������
            {
                return;
            }

            half = FALSE;
        }
    }//end for

}
/*
 ********************************************************************************
 *             DealStringEnd
 *
 * Description :  �����ַ�����Ŀո������
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void DealStrEndSpace(char *str)
{
    BYTE i, j;
    /*ͳ���ַ���β���Ŀո����*/
    BYTE space_num = 0;
    /*����unicode����big endian�����,ȥ��ȫ�ǿո������*/
    if (*((WORD *) &str[0]) == 0xfffe)
    {
        WORD codeChar;
        for (i = 0, j = 0; i < 253; i += 2)
        {
            codeChar = *((WORD *) &str[i + 2]);
            if (codeChar == 0x0000)
            {
                break;
            }
            /*ֻȥ���ַ�β���Ŀո��*/
            if (codeChar != 0x2000)
            {
                j = i;
                space_num = 0;
            }
            else
            {
                space_num++;
            }
        }
        /*�����ַ�ȫ�ǿո�����*/    // ����ֻ��һ���ַ�������³���
        if ((j == 0) && (*((WORD *) &str[j + 2]) == 0x2000))
        {
            *((WORD *) &str[0]) = 0x0000;
        }
        /*ȥ���ַ���β���Ŀո�*/
        else if (j == (i - (space_num + 1) * 2))
        {
            /*β���ո�����Ϊ0�Ŵ���*/
            if (space_num != 0)
            {
                *((WORD *) &str[j + 2]) = 0x0000;
            }
        }
        else
        {
        }
    }
    else
    {

        for (i = 0, j = 0; i < 255; i++)
        {
            if (str[i] == 0)
            {
                break;
            }
            /*ֻȥ���ַ�β���Ŀո��*/
            if (str[i] != ' ')
            {
                j = i;
                space_num = 0;
            }
            else
            {
                space_num++;
            }
        }
        if ((j == 0) && (str[j] == ' '))
        {
            str[j] = 0;
        }
        /*ȥ���ַ���β���Ŀո����*/
        else if (j == (i - (space_num + 1)))
        {
            /*β���ո�����Ϊ0�Ŵ���*/
            if (space_num != 0)
            {
                str[j + 1] = 0;
            }
        }
        else
        {
        }
    }
}
/*
 ********************************************************************************
 *             FectchID3Info
 *
 * Description :
 *
 * Arguments   :  ��ȡfileName������ID3��Ϣ���ŵ�ID3Info_t���ݽṹ��
 *
 * Returns     : ID3ExistFlag = 0 ��ʾû��ID3��Ϣ��1��ʾ��
 *
 * Notes       :
 *
 ********************************************************************************
 */

bool FectchID3Info(const char *filename, ID3Info_t *ID3Info)
{
    handle TagFileHandle;//���ļ�ָ��
    int LastPageLen; //���һҳ����Ч����
    long FileLen;
    bool Fectch_ret;
    char TAGString[] = "TAG"; //ID3V1��ͷӦ����"tag"
    uint8 *pStr;
    uint16 trackNum = 0;

    ClearWatchDog();
    // TempBuf = ID3_BUFFER;
    TempBuf = GET_REAL_ADDR(ID3_BUFFER);
    TagFileHandle = FS_FOpen(filename, FS_OPEN_NORMAL);//���ļ�
    if (TagFileHandle == NULL)
    {
        return FALSE;
    }

    ID3Info->TrackNum_buffer[0]=0x0;

    Fectch_ret = FetchID3_V2info(TagFileHandle, ID3Info);

    if (ID3Info->TrackNum_buffer[0] != 0)
    {
        pStr = ID3Info->TrackNum_buffer;
        while((*pStr != '/') && (*pStr != 0))
        {
            trackNum *= 10;
            trackNum += (*pStr - 0x30);
            pStr++;
        }
        ID3Info->TrackNum = trackNum;	
    }


    if (Fectch_ret == TRUE)
    {
        FS_FClose(TagFileHandle);
        return 1;
    }


    ClearWatchDog();
    //LastPageLen���ļ����һ���ص��ֽ���
    LastPageLen = FS_GetUnalignedLen(TagFileHandle);
    //FileLen���ļ����ܴ�����ÿ����512�ֽ�
    FileLen = FS_GetFileLen(TagFileHandle);
    if(LastPageLen == 0)
    {
        FileLen++;
    }
    FS_FSeek(FileLen - 2, 0, TagFileHandle); //�ȶ�һ��PAGE
    FS_FRead(TempBuf, 1, TagFileHandle); //

    //����ǿ����Ż��ģ����ж��Ƿ�С��128�ٶ���������һ����������128����ֻ��һ��
    if (LastPageLen < 128)
    {
        //ȡ128bytes���� 128bytes��BUFFER��
        memcpy(TagBuf, TempBuf + 512 - 128 + LastPageLen, ((uint16)(128 - LastPageLen)));
        FS_FRead(TempBuf, 1, TagFileHandle);
        memcpy((TagBuf + 128 - LastPageLen), TempBuf, ((uint16)LastPageLen));
    }
    else
    {
        FS_FRead(TempBuf, 1, TagFileHandle);
        memcpy(TagBuf, TempBuf + LastPageLen - 128, 128);
    }
    FS_FClose(TagFileHandle);

    if (memcmp(TagBuf, TAGString, 3) == 0)
    {
        memcpy(ID3Info->TIT2_buffer, (char*) &TagBuf[3], ID3Info->TIT2_length - 2);
        *((BYTE *) (ID3Info->TIT2_buffer) + ID3Info->TIT2_length - 2) = 0x00; //���ﲹ�ַ�������


        memcpy(ID3Info->TPE1_buffer, (char*) &TagBuf[33], ID3Info->TPE1_length - 2);
        *((BYTE *) (ID3Info->TPE1_buffer) + ID3Info->TPE1_length - 2) = 0x00; //���ﲹ�ַ�������


        memcpy(ID3Info->TALB_buffer, (char*) &TagBuf[63], ID3Info->TALB_length - 2);
        *((BYTE *) (ID3Info->TALB_buffer) + ID3Info->TALB_length - 2) = 0x00; //���ﲹ�ַ�������

        if(ID3Info->GENRE_length != 0)
        {
#ifndef PC
            SelectGENRE(ID3Info,TagBuf[127]);
#endif
        }
        ID3Info->TrackNum = (uint16)(TagBuf[126]);
        return 1;
    }
    else
    {
        return 0;
    }

}
