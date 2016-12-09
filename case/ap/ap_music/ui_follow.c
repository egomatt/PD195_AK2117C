/*
 ******************************************************************************
 *                               AS211A
 *                            Module: music
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: follow compare and record play module
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xusong    2010-07-15 15:00     1.0             build this file
 ******************************************************************************
 */
#include "ap_music.h"

#pragma name(AP_UI_FOL)

#define DEF_RECLOWBATT 5//���������¼���ĵ�ѹ
//�ж��ǵ͵�Ĵ���������ѹֵ����DEF_RecLowBattֵ�����������ֵʱȷ��Ϊ�͵�
#define DEF_RECLOWBATTIME 5

void DealForExitRec(void); //�˳�¼��ʱ�Ĵ���
void Show_time_FC(void);
void OpenMusic(void);
/*
 ********************************************************************************
 *             uint16 ui_followRec( void )
 *
 * Description : ����--------¼����״̬
 *
 * Arguments   : void
 *
 * Returns     : int
 *               RESULT_UI_STOP�� ��ʾҪ����ֹͣ״̬����ʱ����module�ѹر�
 *               key: �Ƚ���Ϣ
 *               NULL: ��ʾ�����˳�
 *
 * Notes       : �ڴ�״̬�£���play��stop״̬����mode��play;��NEXT���Ա�״̬��
 *
 ********************************************************************************
 */
uint8 IsLowPower(uint8 power)
{
    /*﮵�ص͵��жϱ�׼*/
    if(check_5v())
	{
	    return 0;
	}
    if (power > LITHIUM_LOW )
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

uint16 ui_followRec(void)
{
    uint8 key;
    uint8 LowBatt_CNT = 0;//�͵������
    uint16 result;
    uint32 DiskSpace;

    if (++Replaytimes_CNT > g_music_vars.replaytimes)
    {
        FS_FRemove(FollowReadTempFileName); //��������ļ���ɾ��
        OpenMusic();
        RePlayStatus = ABLoopPlay;
        UpDisplayFlag = UpDisplayFlag | up_MPRLmode;
        need_draw = TRUE;
        return NULL;
    }

    FS_FRemove(FollowReadTempFileName); //��������ļ���ɾ��
    DiskSpace = FS_GetSpace(1);

    //������̿ռ�����100k���ܸ���
    if (DiskSpace <= 200)
    {
        DisplayErrMsg(MWRIERR);
        OpenMusic();
        RePlayStatus = ABLoopPlay;
        SetFullInterface();
        need_draw = TRUE;
        return NULL;
    }
    // ��������㹻�����˳���
    if (IsLowPower(GetBattery()) != 0)
    {
        DisplayErrMsg(MPWROUT);
        OpenMusic();
        RePlayStatus = NormalPlay;
        UpDisplayFlag = UpDisplayFlag | up_MPRLmode;
        need_draw = TRUE;
        return NULL;
    }

    //ʹ�ؼ����ܳ���
    ui_run_realtime = TRUE;
    wavrSendCommand(MC_OPEN, NULL);
    if (!wavrSendCommand(MC_LOAD, (void*) FollowReadTempFileName))
    {
        switch (FS_GetInfo())
        {
            case FS_INFO_SPACE_OUT: //6      ��ʾ�ļ�������Ŀ¼����û�д��̿ռ䣬����д���ݻ�����չ��Ŀ¼
            DisplayErrMsg(MWRIERR);
            break;
            case FS_INFO_DIR_OUT: //7      �ļ�������Ŀ¼Ŀ¼����
            DisplayErrMsg(MDIROUT);
            break;

            default:
            break;
        }
        OpenMusic();
        RePlayStatus = ABLoopPlay;
        UpDisplayFlag = UpDisplayFlag | up_MPRLmode;
        need_draw = TRUE;
        return NULL;
    }

    //�ڿ�ʼǰ���尴��,����ſ�ʼ��Ӧ����
    while (ap_get_message() != 0)
    {
        ;//do nothing
    }
    if (!wavrSendCommand(MC_RECORD, NULL))
    {
        DealForExitRec();
        RePlayStatus = FollowAB;
        //need_draw = TRUE;
        return NULL;
    }
    UpDisplayFlag = UpDisplayFlag | up_MPRLmode;
    need_draw = TRUE;
    g_2HZ_CNT = 0;

    while (1)
    {
        result = wavrProceed();

        //if( wavrProceed() == 1 )
        if (result == 1)
        {
            if ((DiskSpace--) <= 200)
            {
                DealForExitRec();
                DisplayErrMsg(MWRIERR);
                RePlayStatus = FollowAB;
                //  SetFullInterface();
                return_to_play = 1;
                return NULL;
            }
        }
        else if (result == -1)
        {
            DealForExitRec();
            DisplayErrMsg(MREADERR);
            OpenMusic();
            RePlayStatus = NormalPlay;
            UpDisplayFlag = UpDisplayFlag | up_MPRLmode;
            need_draw = TRUE;
            return NULL;
        }
        else
        {

        }
        if (need_draw != 0)
        {
            Show_time_FC(); //��ʾ������
            RefreshMainScreen();
            need_draw = FALSE;
        }
        key = ap_get_message();

        switch (key)
        {
            case AP_MSG_STANDBY:
            break;

            case AP_KEY_PLAY | AP_KEY_UP: //����stop״̬
            DealForExitRec();//�ر�¼��ģ��
            RePlayStatus = NormalPlay;
            g_FCexitflag = TRUE;
            OpenMusic();
            SetFullInterface();
            need_draw = TRUE;
            return RESULT_UI_STOP2; //ѡ�ز��ź�ص�ֹͣ����

            case AP_KEY_AB | AP_KEY_UP:
            case AP_KEY_NEXT | AP_KEY_UP: //����Ա�״̬
            DealForExitRec();//�ر�¼��ģ��
            RePlayStatus = CompareAB;
            return NULL;

            case AP_KEY_MODE | AP_KEY_UP: //������������״̬
            DealForExitRec();//�ر�¼��ģ��
            OpenMusic();
            RePlayStatus = NormalPlay;
            SetFullInterface();
            need_draw = TRUE;
            return NULL;

            case AP_KEY_PREV | AP_KEY_UP: //����ab����״̬
            DealForExitRec();//�ر�¼��ģ��
            OpenMusic();
            RePlayStatus = ABLoopPlay;
            SetFullInterface();
            need_draw = TRUE;
            return NULL;

            case AP_MSG_RTC:
            g_result = GetBattery();
            if (IsLowPower(g_result) != 0)
            {
                if ((++LowBatt_CNT) > DEF_RECLOWBATTIME)
                {
                    DealForExitRec();
                    DisplayErrMsg(MPWROUT);
                    OpenMusic();
                    //RePlayStatus = ABLoopPlay;
                    RePlayStatus = NormalPlay;
                    UpDisplayFlag = UpDisplayFlag | up_MPRLmode;
                    need_draw = TRUE;
                    return NULL;
                }
            }
            else
            {
                LowBatt_CNT = 0;
            }

            UpDisplayFlag = UpDisplayFlag | up_musiccurrenttime;
            Show_time_FC();
            ui_show_batt( GetBattery());
            break;

            default:
            key = ap_handle_hotkey(key);
            if (key == RESULT_REDRAW)
            {
                SetFullInterface();
                return_to_play = 1;
                need_draw = TRUE;
            }
            else if (key != 0)
            {
                DealForExitRec();
                RePlayStatus = NormalPlay;
                g_FCexitflag = TRUE;
                OpenMusic();
                SetFullInterface();
                need_draw = TRUE;
                return key;
            }
            else
            {
            }

            break;
        } //key
        if (g_2HZ_CNT > ABPointTimeSize) //check if time out

        {
            DealForExitRec();
            RePlayStatus = FollowAB;
            return NULL;
        }
    } //while(1)
    return NULL;
} //ui_followRec

/*
 ********************************************************************************
 *             uint16 ui_followAB( void )
 *
 * Description : ����--------A-B������״̬
 *
 * Arguments   : void
 *
 * Returns     : int
 *               RESULT_UI_STOP�� ��ʾҪ����ֹͣ״̬����ʱ����module�ѹر�
 *               key: �Ƚ���Ϣ
 *               NULL: ��ʾ�����˳�
 *
 * Notes       : �ڴ�״̬�£���play��stop״̬����mode��play;��NEXT���Ա�״̬��
 *
 ******************************************************** ************************
 */

uint16 ui_followAB(void)
{
    uint16 key;
    wma_play_status_t status_buf;
    if (!ReturnNotInit)
    {
        OpenMusic();
    }
    ReturnNotInit = FALSE;
    UpDisplayFlag = UpDisplayFlag | up_MPRLmode;
    need_draw = TRUE;

    while (1)
    {
        if (need_draw != 0)
        {
            Show_time_FC(); //��ʾ������
            RefreshMainScreen();
            need_draw = FALSE;
        }

        key = ap_get_message();

        switch (key)
        {
            case AP_MSG_STANDBY:
            break;

            case AP_KEY_PLAY | AP_KEY_UP:
            g_result = DealPlayKey();
            if (g_result != 0)
            {
                return g_result;
            }
            return RESULT_UI_STOP;

            case AP_KEY_AB | AP_KEY_UP:
            case AP_KEY_NEXT | AP_KEY_UP:
            ReturnNotInit = TRUE; //Ҫ�����Ƚ�ABʱ��Ҫ��ʼ��
            RePlayStatus = CompareAB;
            return NULL;

            case AP_KEY_MODE | AP_KEY_UP:
            RePlayStatus = NormalPlay;
            return NULL;

            case AP_KEY_PREV | AP_KEY_UP:
            RePlayStatus = ABLoopPlay;
            return NULL;

            case AP_KEY_VOLSUB:
            case AP_KEY_VOLADD:
            case AP_KEY_VOL:
            need_draw = TRUE;
            break;

            case AP_MSG_RTC:
            UpDisplayFlag = UpDisplayFlag | up_musiccurrenttime;
            Show_time_FC(); //��ʾ������
            ShowNowTime();
            ui_show_batt( GetBattery());//need_draw = TRUE;
            break;

            default:
            key = ap_handle_hotkey(key);
            if (key == RESULT_REDRAW)
            {
                SetFullInterface();
                return_to_play = 1;
                need_draw = TRUE;
            }
            else if (key != 0)
            {
                return key;
            }
            else
            {
            }

            break;
        }

        musicpSendCommand(MC_GETSTATUS, (void*) &status_buf);
        if (status_buf.signal == SIGNAL_REACH_B)
        {
            Show_time_FC(); //��ʾ������
            RefreshMainScreen();
            need_draw = FALSE;
            musicpSendCommand(MC_STOP, NULL);
            musicpSendCommand(MC_CLOSE, NULL);
            RePlayStatus = FollowRec;
            return NULL;
        }
    }
    return NULL;
}
