/*
 ******************************************************************************
 *                               AS211A
 *                            Module: music
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: stop menu module
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xusong    2010-07-15 15:00     1.0             build this file
 ******************************************************************************
 */
#include "ap_music.h"
extern Open_param_t* m_deOpenparam;
extern uint8 StopMenuactive; //����ѡ�в˵������
extern uint16 PlaylistCurClass; //1 ~ ClassItemTotal

#pragma name(APC_UI_MNU_STOP)

/*����˵�����ַ�ID������*/
const uint16 dir_string_card[5] =
{ MSTOP1, MSTOP2, MSTOP3, MSTOP4, MSTOP5 };

const uint16 dir_string[4] =
{ MSTOP1, MSTOP3, MSTOP4, MSTOP5 };

uint16 ui_stop_menu(void)
{
    uint16 result;
    function_menu_t menu;
    bool tmpflag = FALSE;
    bool card_exist = FALSE;  
    if (DRV_DetectUD(1) == 0x20)
    {
        //�п�ʱ����ʾ
        menu.menuhead = MAIN1;
        menu.total =sizeof(dir_string_card) / 2;
        menu.string = dir_string_card;
        card_exist = TRUE;
    }
    else
    {   
        menu.menuhead = MAIN1;
        menu.total = sizeof(dir_string) / 2;
        menu.string = dir_string;
        if (StopMenuactive >= menu.total)
        {
            StopMenuactive--;
        }
    }
     
    menu.active = (uint16)StopMenuactive;
    //ui_auto_select = FALSE;
    result = ui_function_menu(&menu, NULL);
    //ui_auto_select = TRUE;
    StopMenuactive = menu.active;
    if (result != 0)
    {
        return result;
    }
    {

        //��������ǰ��Ŀ¼��ɾ��ѡ��
        if (DRV_DetectUD(1) == 0x20)
        {
            //�п������
            switch (menu.active)
            {

                case 0: //��Ŀ¼
                if ((g_music_vars.location.disk == 'H') || (g_music_vars.fselmod == FSEL_TYPE_USERPL))
                {
                    g_music_vars.fselmod = FSEL_TYPE_COMMONDIR;
                    g_music_vars.location.disk = 'C';
                    g_music_vars.location.fselTotal = 0;
                    FSELInit(g_MusicType, FSEL_ALL_SEQUENCE, FSEL_TYPE_COMMONDIR, (uint8) g_music_vars.location.disk);
                    fselGetLocation(&g_music_vars.location);
                    FS_GetOrSetDir_Exfat(dir_layer_buff, 0, EXFAT_GET_DIRLAYER);
                }
                result = ui_directory(&g_music_vars.location, g_MusicType);
                FSELSetMode(FSEL_ALL_SEQUENCE);
                if (result == RESULT_IGNORE)
                {
                    fselSetLocation(&g_music_vars.location);
	     ClearTimePara();		
                }
                break;
                case 1: //�忨Ŀ¼
                if ((g_music_vars.location.disk == 'C') || (g_music_vars.fselmod == FSEL_TYPE_USERPL))
                {
                    g_music_vars.location.disk = 'H';
                    strcpy(g_music_vars.location.path, FS_CD_ROOT);
                    g_music_vars.fselmod = FSEL_TYPE_COMMONDIR;
                    g_music_vars.location.fselTotal = 0;
                    result = FSELInit(g_MusicType, FSEL_ALL_SEQUENCE, FSEL_TYPE_COMMONDIR,
                            (uint8) g_music_vars.location.disk);
                    if (!result)
                    {
                        g_DiskStateFlag = g_DiskStateFlag & CLRCARDOKFLAG_DEF;
                        g_music_vars.location.disk = 'C';
                        DisplayErrInfor(CREADERR);//��ʾ����
                        break;
                    }
                    g_DiskStateFlag = g_DiskStateFlag | SETCARDOKFLAG_DEF;
                    fselGetLocation(&g_music_vars.location);
                    FS_GetOrSetDir_Exfat(dir_layer_buff, 0, EXFAT_GET_DIRLAYER);
                    tmpflag = TRUE;

                }
                result = ui_directory(&g_music_vars.location, g_MusicType);
                if (result == RESULT_IGNORE)
                {
                    fselSetLocation(&g_music_vars.location);
	     ClearTimePara();		
                }
                FSELSetMode(FSEL_ALL_SEQUENCE);
                if (result == 0)
                {
                    if (tmpflag != 0)
                    {
                        tmpflag = FALSE;
                    }
                    g_music_vars.location.APE_Cur = 0;
                }

                if (result == RESULT_SD_OUT)
                {
                    //DisplayErrMsg(CARDPULL);//��ʾ���γ�


                    g_music_vars.location.disk = 'C';
                    result = RESULT_IGNORE;

                }
                break;

                case 2: //ɾ���ļ�
                result = ui_delete(&g_music_vars.location, g_MusicType);
                if (RESULT_IGNORE == result)
                {
                    fselSetLocation(&g_music_vars.location);

                }
                else
                {
                    FSELGetNext(g_music_vars.location.filename);
                }
                break;
                case 3: //ɾ��ȫ��
                g_music_vars.fselmod = FSEL_TYPE_COMMONDIR;
                result = ui_delete_all(&g_music_vars.location, g_MusicType);
                g_music_vars.location.fselTotal = 0;
                break;
                default://�˳�
                result = RESULT_IGNORE;
                break;
            }
        }
        else
        {
            //�޿������
            switch (menu.active)
            {
                case 0: //��Ŀ¼
                if (g_music_vars.location.disk == 'H')
                {
                    g_music_vars.location.disk = 'C';
                    FSELInit(g_MusicType, FSEL_ALL_SEQUENCE, FSEL_TYPE_COMMONDIR, (uint8) g_music_vars.location.disk);
                    g_music_vars.fselmod = FSEL_TYPE_COMMONDIR;
                    FS_GetOrSetDir_Exfat(dir_layer_buff, 0, EXFAT_GET_DIRLAYER);
                }
                result = ui_directory(&g_music_vars.location, g_MusicType);
                FSELSetMode(FSEL_ALL_SEQUENCE);
                if (result == 0)
                {
                    g_music_vars.location.APE_Cur = 0;
                }
                break;

                case 1: //ɾ���ļ�
                result = ui_delete(&g_music_vars.location, g_MusicType);
                if (RESULT_IGNORE == result)
                {
                    fselSetLocation(&g_music_vars.location);

                }
                else
                {
                    FSELGetNext(g_music_vars.location.filename);
                }
                break;
                case 2: //ɾ��ȫ��
                FSELSetMode(FSEL_ALL_REPEATALL);
                g_music_vars.fselmod = FSEL_TYPE_COMMONDIR;
                result = ui_delete_all(&g_music_vars.location, g_MusicType);
                FSELSetMode(g_music_vars.repeat_mode);
                break;

                default://�˳�
                result = RESULT_IGNORE;
                break;
            }
        }
        return result;
    }
}

