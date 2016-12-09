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

#pragma name(AP_UI_FC)
//�����ʵ�Сͼ��(���ڱȱ�׼ASCIIС������Ҫ���һ��ͼ��)
const uint16 NUM_PIC1[] =
{SYSFONT_SMALL_0,SYSFONT_SMALL_1,SYSFONT_SMALL_2,SYSFONT_SMALL_3,SYSFONT_SMALL_4,
 SYSFONT_SMALL_5,SYSFONT_SMALL_6,SYSFONT_SMALL_7,SYSFONT_SMALL_8,SYSFONT_SMALL_9};

void DealForExitRec(void); //�˳�¼��ʱ�Ĵ���
void Show_time_FC(void);
void OpenMusic(void);
/*
 ********************************************************************************
 *             uint16 ui_follow_compare( void )
 *
 * Description : �����Ա���ѭ��
 *
 * Arguments   : void
 *
 * Returns     : int
 *               RESULT_UI_STOP�� ��ʾҪ����ֹͣ״̬����ʱ����module�ѹر�
 *               key: �Ƚ���Ϣ
 *               NULL: ��ʾ�����˳�
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint16 ui_follow_compare(void)
{
    uint16 result;

    {
        mp3_play_status_t status_buf;
        uint16 i;
        i = g_ABPointTimeSize / 10; //�� 10%
        if (i < 2)
        {
            i = 2;
        }
        ABPointTimeSize = g_ABPointTimeSize + 2 + i;//2Ϊ���鲹��ֵ

        musicpSendCommand(MC_GETSTATUS, (void*) (&status_buf));
        //�������ڸ����Ա���ʾ
        TimeRateData_sav.TotalTime_sav[0] = status_buf.total_time.hour;
        TimeRateData_sav.TotalTime_sav[1] = status_buf.total_time.minute;
        TimeRateData_sav.TotalTime_sav[2] = status_buf.total_time.second;
        ifSelBuffToltalTimeFlag = TRUE;
    }

    ReturnNotInit = FALSE; //�´���Ҫ��ʼ��
    Replaytimes_CNT = 0;

    musicpSendCommand(MC_STOP, 1);//1��ʾMP3 �˳�ʱ���ݵ���A��ʱ��
    musicpSendCommand(MC_BACKUP, (void*) &g_music_vars.BreakPTSave);
    musicpSendCommand(MC_CLOSE, NULL);

    RePlayStatus = FollowRec;

    //UpDisplayFlag = UpDisplayFlag | up_MPRLmode;
    while (1)
    {
        switch (RePlayStatus)
        //״̬����
        {
            case WaitSetAPointForAB://����A��
            case NormalPlay://��ͨ����
            musicpSendCommand(MC_SETA_B, ClrABPoint);//clear_a b
            return NULL;

            case FollowRec://����¼��
            result = ui_followRec();
            if (result != 0)
            {
                return result;
            }
            break;

            case FollowAB://����AB
            result = ui_followAB();
            if (result != 0)
            {
                return result;
            }
            break;

            case ComparePlay://�ԱȲ���
            result = ui_comparePlay();
            if (result != 0)
            {
                return result;
            }
            break;

            case CompareAB: //�Ա�AB
            result = ui_compareAB();
            if (result != 0)
            {
                return result;
            }
            break;

            case ABLoopPlay: //����
            return NULL;

            default:
            break;
        }
    }
    return NULL;
}

/*
 ********************************************************************************
 *             void DealForExitRec( void )
 *
 * Description :
 *
 * Arguments   : void
 *
 * Returns     : void
 *
 * Notes       :
 *
 ********************************************************************************
 */
void DealForExitRec(void)
{
    wav_rec_status_t pStatus;

    wavrSendCommand(MC_STOP, NULL);

    do
    {
        wavrProceed();
        wavrSendCommand(MC_GETSTATUS, (void*) (&pStatus));
        if (RECORDING_DISK_ERROR == pStatus.status)
        {
            DisplayErrInfor(MREADERR);//��ʾ���̴�
            break;
        }
    } while (pStatus.status != RECORDING_STOP);

    wavrSendCommand(MC_CLOSE, NULL);

    //ʹ�ؼ����Գ���
    ui_run_realtime = FALSE;
}
/*
 ********************************************************************************
 *             void Show_time_FC( void )
 *
 * Description : ˢ�µ�ǰʱ�䣬ˢ����ʱ�䣬ˢ�±�����
 *
 * Arguments   : void
 *
 * Returns     : void
 *
 * Notes       :
 *
 ********************************************************************************
 */
void Show_time_FC(void)
{
    uint8 temp_buffer[9];
    // ˢ�µ�ǰʱ��
    if ((UpDisplayFlag & up_musiccurrenttime) != 0)
    {
        time_t time;
        if (RePlayStatus == FollowRec)//����ʱ��ʾʱ��
        {
            wavrSendCommand(MC_GETTIME, (void*) &time);
             
            
            itoa2(time.hour, temp_buffer);
            if (g_currentMusicType != Music_Wav)
                temp_buffer[0] = ' ';
            temp_buffer[2] = ':';
            itoa2(time.minute, temp_buffer + 3);
            temp_buffer[5] = ':';
            itoa2(time.second, &temp_buffer[6]);
            temp_buffer[8] = 0x00;
//            if (g_currentMusicType != Music_Wav)
                ui_show_f(temp_buffer);
//            else
//               ui_show_ef(temp_buffer, 0);
        }
        else if (RePlayStatus == ComparePlay)//�Աȷ�ʱ��ʾʱ��
        {
            Full_time_t time_buf;
            if (wavpSendCommand(MC_GETTIME, (void*) &time_buf) != 0)
            {
                 
                
                itoa2(time_buf.TimeHour, temp_buffer);
                if (g_currentMusicType != Music_Wav)
                    temp_buffer[0] = ' ';
                temp_buffer[2] = ':';
                itoa2(time_buf.TimeMinute, temp_buffer + 3);
                temp_buffer[5] = ':';
                itoa2(time_buf.TimeSecond, &temp_buffer[6]);
                temp_buffer[8] = 0x00;
//                if (g_currentMusicType != Music_Wav)
                    ui_show_f(temp_buffer);
//                else
//                    ui_show_ef(temp_buffer, 0);
            }
        }
        else//����״̬��ʾʱ��(����MUSIC����)
        {
            Full_time_t time_buf;
            if (musicpSendCommand(MC_GETTIME, (void*) &time_buf) != 0)
            {
                g_music_vars.BreakPTDisTime.hour = time_buf.TimeHour;
                g_music_vars.BreakPTDisTime.minute = time_buf.TimeMinute;
                g_music_vars.BreakPTDisTime.second = time_buf.TimeSecond;
            }
            else
            {
                //��δ����ȷȡ��ʱ�䣬���Ȳ�ˢ����ʾ
                UpDisplayFlag &= clr_musiccurrenttime;
            }

        }
        // UpDisplayFlag ^= up_musiccurrenttime;//���ˢ�±�־

    }

    //ˢ�±�����
    if ((UpDisplayFlag & up_musicbitrate) != 0)
    {
        if ((RePlayStatus == FollowRec) || (RePlayStatus == ComparePlay))
        {
            //            Show_MusicType();//����ʾ��������ͼ��

            UpDisplayFlag = UpDisplayFlag & clr_musicbitrate;//���ˢ�±�־
        }
    }

    //ˢ����ʱ��
    if (UpDisplayFlag & up_musictotaltime)
    {
        if ((RePlayStatus == FollowRec) || (RePlayStatus == ComparePlay))
        {

            itoa2(TimeRateData_sav.TotalTime_sav[0], temp_buffer);
            if (g_currentMusicType != Music_Wav)
                temp_buffer[0] = ' ';
            temp_buffer[2] = ':';
            itoa2(TimeRateData_sav.TotalTime_sav[1], temp_buffer + 3);
            temp_buffer[5] = ':';
            itoa2(TimeRateData_sav.TotalTime_sav[2], &temp_buffer[6]);
            temp_buffer[8] = 0x00;
//            if (g_currentMusicType != Music_Wav)
                ui_show_f(temp_buffer);
//            else
//                ui_show_ef(temp_buffer, 1);
        }
        else
        {
             

            itoa2(TimeRateData_sav.TotalTime_sav[0], temp_buffer);
            if (g_currentMusicType != Music_Wav)
                temp_buffer[0] = ' ';
            temp_buffer[2] = ':';
            itoa2(TimeRateData_sav.TotalTime_sav[1], temp_buffer + 3);
            temp_buffer[5] = ':';
            itoa2(TimeRateData_sav.TotalTime_sav[2], &temp_buffer[6]);
            temp_buffer[8] = 0x00;
//            if (g_currentMusicType != Music_Wav)
                ui_show_f(temp_buffer);
//            else
//                ui_show_ef(temp_buffer);
        }
        //RePlayStatus ^= up_musictotaltime;

        //UpDisplayFlag ^= up_musictotaltime;//���ˢ�±�־
        UpDisplayFlag &= clr_musictotaltime;//���ˢ�±�־

    }

    //ˢ�±�����
    if (UpDisplayFlag & up_musicbitrate)
    {
        //audio_format_t pFormat;
        //region_t region_b = {POSITION_B, 0, 21, 16};

        if ((RePlayStatus == FollowRec) || (RePlayStatus == ComparePlay))
        {
            Show_MusicType();//����ʾ��������ͼ��

            ResShowPic(NUM_PIC1[TimeRateData_sav.Bitrate_sav[0] - 0x30], POSITION_B + 1 + 2, 48);
            ResShowPic(NUM_PIC1[TimeRateData_sav.Bitrate_sav[1] - 0x30], POSITION_B + 6 + 3, 48);
            ResShowPic(NUM_PIC1[TimeRateData_sav.Bitrate_sav[2] - 0x30], POSITION_B + 12 + 3, 48);
            UpDisplayFlag = UpDisplayFlag & clr_musicbitrate;//���ˢ�±�־
        }
    }

    return;
}
/*
 ********************************************************************************
 *             void OpenMusic( void )
 *
 * Description :
 *
 * Arguments   : void
 *
 * Returns     : void
 *
 * Notes       :
 *
 ********************************************************************************
 */
void OpenMusic(void)
{
    g_music_vars.MusicOpenMod = ABPlay_open;
    musicpSendCommand(MC_OPEN, (void*) &g_Openparam);
    musicpSendCommand(MC_LOAD, NULL);
    musicpSendCommand(MC_SETPOWERCOUNT, 5); //set power out level count=2^4=16
    musicpSendCommand(MC_PLAY, (void*) g_music_vars.MusicOpenMod);
}
