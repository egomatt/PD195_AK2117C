/*
 *******************************************************************************
 *                File selector
 *            enhanced library
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 *******************************************************************************
 */
#include "ap_common.h"

#pragma name(DIR_SEL_NEXT)

static const uint8 ExtMusic_N[][4] =
{ "MP3", "WMA", /*"WAV", "MP1", "MP2", "APE", "FLA"*/};
static const uint8 ExtPicture_N[][4] =
{ "JPG", "BMP", "GIF" };//, "ACT"};
static const uint8 ExtTXT_N[][4] =
{ "TXT" };
static const uint8 ExtVideo_N[][4] =
{ "AMV", "AVI" };
static const uint8 ExtVoice_N[][4] =
{ "WAV" };
static const uint8 dir_ExtAll_N[][4] =
{ "*  " };
static const uint8 dir_RootName[] =
{ ":" };
static const uint8 ExtResult[] =
{ FSEL_TYPE_MUSIC, FSEL_TYPE_VOICE, FSEL_TYPE_TXT, FSEL_TYPE_AMV, FSEL_TYPE_DSC, DIRECTORY_ALL };
static const uint8 ExtSize[] =
{ sizeof(ExtMusic_N) / 4, sizeof(ExtVoice_N) / 4, sizeof(ExtTXT_N) / 4, sizeof(ExtVideo_N) / 4,
        sizeof(ExtPicture_N)/ 4, 0 
};
static const uint32 ExtPoint[] =
{ (uint32*) ExtMusic_N, (uint32*) ExtVoice_N, (uint32*) ExtTXT_N, (uint32*) ExtVideo_N, (uint32*) ExtPicture_N,
        (uint32*) dir_ExtAll_N 
};
/********************************************************************************
 * Description : ��ʼ���ļ�ѡ�����ı���
 *
 * Arguments  : none
 * Returns     : none

 * Notes       :  None
 *
 ********************************************************************************/
void dir_Init_Next(uint8 type, uint8 mode)
{
    dir_fselType = type;
    dir_fselDirLayer = 0;
    dir_fselMode = mode;
    memset(dir_fselDirNo, 0, TOTALDIRLAYER);
    dir_fselFileNo = 0;
    dir_fselTotal = 0;
    FS_CD(":");
}
/********************************************************************************
 * Description : ��ȡ��ǰ�ļ������������ĸ���
 *
 * Arguments  : none
 * Returns     : ��ǰ�ļ����͵ĸ���

 * Notes       :  None
 *
 ********************************************************************************/
uint8 dir_GetExtSize_Next(void)
{
    uint8 i;
    ClearWatchDog();
    for (i = 0; i < sizeof(ExtResult); i++)
    {
        if (dir_fselType == ExtResult[i])
        {
            return ExtSize[i];
        }
    }
    return 0;
}

/********************************************************************************
 * Description : ��ȡ��ǰ�ļ����͵��׵�ַ
 *
 * Arguments  :  None
 * Returns     :  ��ǰ�ļ�����������׵�ַ
 * Notes       :     None
 *
 ********************************************************************************/
const uint8 *dir_GetAllExtBuf_Next(void)
{
    uint8 i;
    ClearWatchDog();
    for (i = 0; i < sizeof(ExtResult); i++)
    {
        if (dir_fselType == ExtResult[i])
        {
            return (const uint8*) ExtPoint[i];
        }
    }
    return (const uint8*) ExtPoint[4];
}

/********************************************************************************
 * Description : ������һ���ļ�
 *
 * Arguments  :
 strFile���洢�ҵ����ļ�����
 * Returns     :
 TRUE OR FALSE
 * Notes       :     None
 *
 ********************************************************************************/
uint8 dir_SelectNext(uint8* strFile)
{
    uint8 dir_type;
    //  if (dir_fselFileNo == dir_fselTotal)
    //   {
    //       return FALSE;
    //   }

    if (dir_fselFileNo == 0)
    {
        dir_type = FS_DIR_FILE;
    }
    else
    {
        dir_type = FS_DIR_FILE_CONT;
    }
    if (FS_Dir(strFile, dir_GetAllExtBuf_Next(), dir_type, dir_GetExtSize_Next()) != 0)
    {
        dir_fselFileNo++;
        ClearWatchDog();
        return TRUE;
    }
    return FALSE;
}

/********************************************************************************
 * Description : ���㵱ǰĿ¼�µ��ļ�����
 *
 * Arguments  :
 strFile���洢�ҵ����ļ�����
 * Returns     :
 TRUE OR FALSE
 * Notes       :     None
 *
 ********************************************************************************/
uint16 dir_GetFileTotalCur(void)
{
    uint16 total = 0;
    uint8 dir_type = FS_DIR_FILE;
    int32 lEntry = FS_GetCurDirEntry();
    long lDir = FS_GetCurDir();
    do
    {
        if (FS_Dir(dir_m_szTemp, dir_GetAllExtBuf_Next(), dir_type, dir_GetExtSize_Next()) != 0)
        {
            total++;
            ClearWatchDog();
        }
        else
        {
            break;
        }
        dir_type = FS_DIR_FILE_CONT;
    } while (1);
    FS_GetOrSetDir_Exfat(0, lDir, EXFAT_SET_CURDIR);
    FS_SetCurDirEntry(lEntry);
    return total;
}
/********************************************************************************
 * Description : ������һ��Ŀ¼
 *
 * Arguments  :
 strFile���洢�ҵ���Ŀ¼����
 * Returns     :
 TRUE OR FALSE
 * Notes       :     None
 *
 ********************************************************************************/
uint8 dir_fselNextDir(uint8 *strDir)
{
    uint16 wtemp;
    wtemp = dir_fselDirNo[dir_fselDirLayer];
    wtemp++;
    if (!dir_ToDirEntry(strDir, wtemp))
    {
        return FALSE;
    }
    dir_fselDirNo[dir_fselDirLayer] = (uint8) wtemp;
    return TRUE;
}

/********************************************************************************
 * Description : ��8��Ŀ¼�в����ļ�
 *
 * Arguments  :
 strFile���洢�ҵ����ļ�����
 * Returns     :
 TRUE OR FALSE
 * Notes       :     None
 *
 ********************************************************************************/
uint8 dir_SelectNext_8Layer(uint8 *strFile)
{
    //�ڵ�ǰĿ¼�������ļ�
    if (!dir_SelectNext(strFile))
    {
        //Ѱ�ҵ�ǰĿ¼�µ�����Ŀ¼
        CurrentDiragain:
        //    output8(0x4e, input8(0x4e) | 0x08);
        if (FS_Dir(dir_m_szTemp, dir_IgnoreExt4, FS_DIR_DIR, 0) != 0)
        {

            if (FS_CD(FS_CD_SON) == TRUE)
            {
                //      dir_fselTotal = dir_GetFileTotalCur();
                if (dir_fselDirNo[dir_fselDirLayer] != 0)
                {
                    dir_fselDirLayer++;
                }

                if (dir_fselDirLayer > TOTALDIRLAYER)
                {
                    dir_fselDirLayer--;
                    FS_CD(FS_CD_PARENT);
                    goto ParentDiragainNext;
                }

                dir_fselFileNo = 0;
                dir_fselDirNo[dir_fselDirLayer] = 1; //��һ����Ŀ¼

                // ��鵱ǰĿ¼�Ƿ��к��ʵ��ļ�.
                if (dir_SelectNext(strFile) != 0)
                {
                    return TRUE;
                }
                else //��Ŀ¼�µ���Ŀ¼ѭ������
                {
                    goto CurrentDiragain;
                }
            }
            else //������
            {
                dir_Init_Next(dir_fselType, dir_fselMode);
                return FALSE;
            }
            //output8(0x27, (0x27,IO27Bak);
        }
        else //Ѱ�ҵ�ǰĿ¼����һ��Ŀ¼��ƽ��Ѱ�ң�
        {
            ParentDiragainNext: if (FS_DirParNext() != 0)
            {
                if (dir_fselDirLayer > TOTALDIRLAYER)
                {
                    dir_Init_Next(dir_fselType, dir_fselMode);
                    return FALSE;
                }
                //Ѱ�ҵ�ǰĿ¼����һ��Ŀ¼��ƽ��Ѱ�ң�
                if (dir_fselNextDir(dir_m_szTemp) != 0)
                { // ���Ŀ¼���Ƿ��к��ʵ��ļ�.
                    FS_CD(FS_CD_SON);
                    //    dir_fselTotal = dir_GetFileTotalCur();
                    dir_fselFileNo = 0;
                    if (dir_SelectNext(strFile) != 0)
                    {
                        return TRUE;
                    }
                    // �����Ŀ¼.
                    goto CurrentDiragain;
                }
                else //����һ��Ŀ¼
                {
                    if (dir_fselDirLayer <= TOTALDIRLAYER)
                    {
                        dir_fselDirNo[dir_fselDirLayer] = 0;
                    }

                    dir_fselDirLayer--;
                    goto ParentDiragainNext;
                }
            }
            else
            {
                dir_Init_Next(dir_fselType, dir_fselMode);
                return FALSE;
            }
        }
    }
    return TRUE;
}

/********************************************************************************
 * Description : ������һ��Ŀ¼�е��ļ�
 *
 * Arguments  :
 strFile���洢�ҵ����ļ�����
 * Returns     :
 TRUE OR FALSE
 * Notes       :     None
 *
 ********************************************************************************/

uint8 dir_fselGetNext(uint8 *strFile)
{
    switch (dir_fselMode)
    {
        case FSEL_ALL_REPEATALL:
        {
            if (dir_SelectNext_8Layer(strFile) != 0)
            {
                return TRUE;
            }
            else
            {
                dir_Init_Next(dir_fselType, dir_fselMode);
                return dir_SelectNext_8Layer(strFile);
            }
        }
        default:
        return FALSE;
    }
}

/********************************************************************************
 * Description :���������Ų����ļ�
 *
 * Arguments  :
 strFile���洢�ҵ����ļ�����
 Num���ļ�������
 * Returns     :
 TRUE OR FALSE
 * Notes       :     None
 *
 ********************************************************************************/
uint8 dir_fselGetByNO(uint8 *strFile, const uint16 Num)
{
    if ((Num > dir_fselTotal) || (Num == 0))
    {
        return FALSE;
    }

    dir_fselFileNo = 0;
    while (Num > dir_fselFileNo)
    {
        if (!dir_SelectNext(strFile))
        {
            return FALSE;
        }
    }
    return TRUE;
}

/********************************************************************************
 * Description :��ȡ�ļ�����
 *
 * Arguments  :

 * Returns     :
 �ļ�����
 * Notes       :     None
 *
 ********************************************************************************/

uint16 dir_GetSelFileTotal(void)
{
    uint16 total;
    uint8 dir_type = FS_DIR_FILE;
    total = 0;
    do
    {
        if (FS_Dir(dir_m_szTemp, (const uint8*) dir_GetAllExtBuf_Next(), dir_type, dir_GetExtSize_Next()) != 0)
        {
            total++;
            ClearWatchDog();
        }
        else
        {
            //    return total;
            break;
        }
        dir_type = FS_DIR_FILE_CONT;
    } while (1);
    return total;
}

/********************************************************************************
 * Description :������֪��location��Ϣ���ñ���Ŀ¼
 *
 * Arguments  :
 location��Ŀ¼��Ϣ�ṹ��

 * Returns     :
 * Notes       :     None
 *
 ********************************************************************************/
uint8 dir_fselSetLocationC(file_location_t *location)
{
    //   uint8 dir_type;
    uint16 wtemp = 1;
    uint16 curdir_total_num = 0;

    dir_Init_Next(dir_fselType, dir_fselMode);

    if (location->ClusterNo == 0x0)
    {
        location->ClusterNo = FS_GetCurDir();
    }
    
    FS_GetOrSetDir_Exfat(dir_layer_buff, 0, EXFAT_SET_DIRLAYER);
    if (!FS_SetCurDir(location->ClusterNo))
    {
        return FALSE;
    }
    dir_fselDiskLetter = (uint8) location->disk;
    memcpy(dir_fselDirNo, location->DirNo, TOTALDIRLAYER);
    dir_fselDirLayer = (uint8) location->dirLayer;

    if (dir_fselDirNo[0] != 0)
    {
        FS_CD("\\");
        dir_ToDirEntry(dir_m_szTemp, dir_fselDirNo[dir_fselDirLayer - 1]);
        FS_CD(dir_m_szTemp);
        ClearWatchDog();
        if (memcmp(dir_m_szTemp, location->path, 11) != 0) //path is invalid
        {
            return FALSE;
        }
    }
    else
    {

        if (location->path[0] != ':')
        {
            return FALSE;
        }
        FS_CD(dir_RootName);
    }
    //    dir_fselTotal = dir_GetFileTotalCur();
    dir_fselFileNo = 0;

    //Select the specify file by location
    if ('\0' == location->filename[0])
    {
        return TRUE;
    }
    return TRUE;
}
/********************************************************************************
 * Description : throgh the keeped values set the file selector parament
 *
 * Arguments  :location :the keeped vlaues
 *
 * Returns     :
 *            TRUE: Successful,
 *            FALSE: Fail, detail information in dir_fselError.
 * Notes       :
 *
 ********************************************************************************/
uint8 dir_fselSetLocation(file_location_t *location)
{
    uint8 i;

    if (dir_fselPLOrCommonDir == FSEL_TYPE_COMMONDIR) //common file system
    {
        i = dir_fselSetLocationC(location);
        if (i != 0)
        {
            return i;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
}
void dir_getSelNumByFileName(file_location_t *location)
{
    uint16 cur_selnum, total_num;
    uint8 dir_type = FS_DIR_FILE;
    uint8 find_flag = 0;
    cur_selnum = 0;
    total_num = 0;
    dir_temp_record.DirEntry = FS_GetCurDirEntry();
    do
    {
        if (FS_Dir(dir_m_szTemp, (const uint8*) dir_GetAllExtBuf_Next(), dir_type, dir_GetExtSize_Next()) != 0)
        {
            total_num++;
            if (find_flag == 0)
            {
                if (memcmp(dir_m_szTemp, location->filename, 11) == 0)
                {
                    
                    dir_temp_record.DirEntry = FS_GetCurDirEntry();
                    cur_selnum = total_num;
                    find_flag = TRUE;
                }

            }
            ClearWatchDog();
        }
        else
        {
            break;
        }
        dir_type = FS_DIR_FILE_CONT;
    } while (1);
    location->fselFileNo = cur_selnum;
    location->fselTotal = total_num;
}

