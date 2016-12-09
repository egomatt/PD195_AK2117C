/*
 ******************************************************************************
 *                               AS211A
 *                            Module: music
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: sub functions 3 of music
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xusong    2010-07-15 15:00     1.0             build this file
 ******************************************************************************
 */
#include "ap_music.h"

#pragma name(AP_SUB_FUN3)
const uint8 RootDirName[] = "ROOT";
/*
 *****************************************************************************
 ** ����: ��ʾ���� id ����Դ����
 ** �ӿ�: void DisplayErrInfor(uint16 id)
 ** ����:
 ** �������: uint16 id:Ҫ��ʾ����Դ��
 ** �������: void
 ** ʹ��˵��:
 *******************************************************************************
 */
void make_dirname(void)
{

    //uint16 result;
    //�б���ģʽ����
    fselGetLocation(&g_music_vars.location);
	FS_GetOrSetDir_Exfat(dir_layer_buff, 0, EXFAT_GET_DIRLAYER);
/*
    if (g_music_vars.location.path[0] == ':')
    {
        strcpy(dir_name, RootDirName);
    }
    else
    {
        uint8 Backbuf[32];
        FS_Backup(Backbuf, FS_BACKUP_DIRINFO, 0);//Ŀ¼���
        FS_CD("\\");
        memcpy(&dir_name[2], g_music_vars.location.path, 12);
        result = (uint16) FS_GetName(&dir_name[2], 13); //ȡ������
        if (result != 0)
        {
            //���UnicodeС��
            dir_name[0] = 0xff;
            dir_name[1] = 0xfe;
        }
        else
        {
            memcpy(dir_name, g_music_vars.location.path, 12);
            dir_name[8] = 0;
            dir_name[9] = 0;
        }
        FS_Restore(Backbuf, FS_RESTORE_DIRINFO, 0);//Ŀ¼��ָ�
    }
*/
	FS_GetOrSetDir_Exfat(dir_layer_buff, 0, EXFAT_SET_DIRLAYER);
}

void DisplayErrInfor(uint16 id)
{
    ClearScreen(NULL);//����
    ui_auto_update = TRUE;
    ResShowMultiString(id, 255, 16);//��ʾ�������ļ�
    UpdateScreen(NULL);//����
    while (ap_get_message() != NULL)
        ;//�����Ϣ��,ȷ��DELAYʱ��׼ȷ
    ap_sleep(4);
}

/******************************************************************************
 ** ����:  �ļ�ѡ������ʼ����������ʾ
 ** �ӿ�:  uint16 InitFileSelector(void)
 ** ����:  ���ļ�ѡ������ʼ������Ч�����ϣ����Ҳ������򱨴��˳�
 ** �������: void
 ** �������: 0: û����Ч���̻��������ļ�����ʼ��ʧ�ܣ�Ҫ���˳�AP
 1: ����ɳ�ʼ���ļ�ѡ��������
 ** ʹ��˵��:
 ********************************************************************************/
uint16 InitFileSelector(void)
{
    if (g_music_vars.location.disk == 'H')
    {
        /*�鿨��Ⲣ��ʼ���鿨��*/
        if (DRV_DetectUD(1) == 0x20)
        {
            if (FSELInit(g_MusicType, FSEL_ALL_SEQUENCE, g_music_vars.fselmod, 'H') != 0)
            {
                return 1;
            }
            else
            {
                DisplayErrInfor(CREADERR);//��ʾ����
            }

        }
        /*��ʼ���忨��ʧ�ܣ����Flash����*/
        g_music_vars.location.disk = 'C';

        if (FSELInit(g_MusicType, FSEL_ALL_SEQUENCE, g_music_vars.fselmod, 'C') != 0)
        {
            return 1;
        }
        else
        {
            DisplayErrInfor(MREADERR);//��ʾ���̴�
            return 0;
        }
    }
    else /*g_music_vars.location.disk == 'C' ������*/
    {
        if (FSELInit(g_MusicType, FSEL_ALL_SEQUENCE, g_music_vars.fselmod, 'C') != 0)
        {
            g_music_vars.location.disk = 'C';
            return 1;
        }
        else
        {
            DisplayErrInfor(MREADERR);//��ʾ����
        }
        /*Flash ���̳�ʼ��ʧ�ܣ���ʼ���忨��*/
        if (DRV_DetectUD(1) == 0x20)
        {
            g_music_vars.location.disk = 'H';
            if (FSELInit(g_MusicType, FSEL_ALL_SEQUENCE, g_music_vars.fselmod, 'H') != 0)
            {
                return 1;
            }
            else
            {
                DisplayErrInfor(CREADERR);//��ʾ����
                return 0;//�Ƿ�
            }
        }
        return 0;//û���˳�
    }
}

/*
 *****************************************************************************
 ** ����:
 ** �ӿ�: uint16 UpdateFileSelector(void)
 ** ����: �����ļ�ѡ����
 ** �������: void
 ** �������:
 0: ���²��ɹ���Ҫ�˳�AP
 1: ���³ɹ����ѿ���
 ** ʹ��˵��:
 *******************************************************************************
 */
uint16 UpdateFileSelector(void)
{
    uint8 result = FSELSetLocation(&g_music_vars.location);
    if (result == 1)
    {
        /*�ҵ����ļ��������Ƿ�Ŀ¼��*/
        if (g_music_vars.location.filename[0] != '\0')
        {
            return 1;
        }
        /*�ҵ�Ŀ¼�ĵ�һ���ļ�*/
        if (StartFileSelector() == 0)
        {
            return 0;
        }
	  
       return 1; 
    }

    /*result == 0*/
    ClearTimePara();//ʱ��ϵ������0��ʼ
    /*�Ҳ�����Ӧ�ļ�ʱҪ�ص�Ŀ¼��һ���ļ�*/
    g_music_vars.location.filename[0] = '\0';
    result = FSELSetLocation(&g_music_vars.location);
    if (result != 0)
    {
        /*�ҵ�ǰĿ¼�ĵ�һ���ļ�*/
        if (StartFileSelector() == 0)
        {
            return 0;
        }
    }
    else
    {
        /*�л�Ŀ¼�ʹ������¼����ļ�*/
        if (InitFileSelector() == 0)
        {
            return 0;
        }
        if (StartFileSelector() == 0)
        {
            return 0;
        }
	
    }
    return 1;
}

/******************************************************************************
 ** ����:
 ** �ӿ�:
 ** ����:
 ** �������:
 ** �������:
 ** ʹ��˵��: //ע���⺯�����ڳ�ʼ�����ļ�ѡ������set ��Ŀ¼����õ�
 ********************************************************************************/
uint16 StartFileSelector(void)
{
    if (g_music_vars.location.disk == 'C')
    {
        if (FSELGetNext(g_music_vars.location.filename) != 0)
        {
            return 1;
        }
        /*�Ӹ�Ŀ¼��ʼ���������ļ�*/
        if (InitFileSelector() == 0)
        {
            return 0;
        }
        if (FSELGetNext(g_music_vars.location.filename) != 0)
        {
            return 1;
        }

        if (g_music_vars.location.disk == 'H')
        {
            DisplayErrInfor(CFILEOUT);//��ʾ�����ļ�
            g_music_vars.location.disk = 'C';
            return 0;
        }
        DisplayErrInfor(DFILEOUT);//��ʾ�������ļ�
        /*���忨�������ļ�*/
        g_music_vars.location.disk = 'H';
        if (DRV_DetectUD(1) != 0x20)
        {
            g_music_vars.location.disk = 'C';
            return 0;
        }
        if (FSELInit(g_MusicType, FSEL_ALL_SEQUENCE, g_music_vars.fselmod, 'H') != 0)
        {
            if (FSELGetNext(g_music_vars.location.filename) != 0)
            {
                return 1;//�ҵ��ļ�
            }
            DisplayErrInfor(CFILEOUT);//��ʾ�����ļ�
        }
        return 0;
    }
    else //g_music_vars.location.disk == 'H'
    {
        if (FSELGetNext(g_music_vars.location.filename) != 0)
        {
            return 1;//�ҵ��ļ�
        }
        /*�ٲ�һ��,��Ϊ������Ŀ¼ǰ�滹���ļ�(��Ȼ���������ļ�)*/
        if (InitFileSelector() == 0)
        {
            return 0;
        }
        if (FSELGetNext(g_music_vars.location.filename) != 0)
        {
            return 1;//�ҵ��ļ�
        }

        if (g_music_vars.location.disk == 'C')
        {
            DisplayErrInfor(DFILEOUT);//��ʾ�������ļ�
            return 0; //��Ϊǰ���Ѵ� 'H' �鵽 'C' �ˣ��Ѳ��꣬δ�ҵ��ļ�
        }
        DisplayErrInfor(CFILEOUT);//��ʾ�����ļ�
        g_music_vars.location.disk = 'C';
        if (FSELInit(g_MusicType, FSEL_ALL_SEQUENCE, g_music_vars.fselmod, 'C') != 0)
        {
            if (FSELGetNext(g_music_vars.location.filename) != 0)
            {
                return 1;//�ҵ��ļ�
            }
            DisplayErrInfor(DFILEOUT);//��ʾ�������ļ�
        }
        return 0;
    }
}

