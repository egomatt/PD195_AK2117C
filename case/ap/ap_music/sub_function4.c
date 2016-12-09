/*
 ******************************************************************************
 *                               AS211A
 *                            Module: music
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: sub functions 4 of music
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xusong    2010-07-15 15:00     1.0             build this file
 ******************************************************************************
 */
#include "ap_music.h"
#pragma name(AP_SUB_FUN4)
uint8 temp_buffer[9] ={ 0 };


void SetStopToPlayInterfaceForPlay(void)
{
    ui_auto_update = FALSE;
    UpDisplayFlag = UpDisplayFlag/* | up_musictypeflag | up_lyricflag | up_musicbitrate | up_musicloopmode
            | up_musicsequence | up_musiceqmode */| up_musicbattery;
}

/*�����Ź����еĴ���*/
void Deal_Error_Playing(void)
{
    uint8 key;
    do
    {
        key = ap_get_message();
        if (key == AP_MSG_SD_OUT)
        {
            break;
        }
    } while (key != NULL);
    if ((key == AP_MSG_SD_OUT) && (g_music_vars.location.disk == 'H'))
    {
        DisplayErrMsg(CARDPULL);
        g_music_vars.location.disk = 'C';
    }
    else if ((g_music_vars.location.disk == 'H') && (DRV_DetectUD(1) != 0x20))
    {
        g_music_vars.location.disk = 'C';
        ap_sleep(2);
    }
    else
    {
        if(g_music_vars.location.disk == 'H')
        {
            int8 rtc_count = 0;
            do
            {
                key = ap_get_message();
                if ((key == AP_MSG_RTC) || (key == AP_MSG_CHARGING))
                {
                    rtc_count++;
                }
                
                //check time out
                if (rtc_count >= 3)
                {
                    break;
                }
            }while (key != AP_MSG_SD_OUT);
            // ����Ƿ񿨰γ�������
            if(key == AP_MSG_SD_OUT)
            {
                DisplayErrMsg(CARDPULL);
                g_music_vars.location.disk = 'C';
            }
            else
            {

	
                DisplayErrMsg(MFMTERR);//��ʾ��ʽ��

		
            }
        }
        else
        {
            DisplayErrMsg(MFMTERR);//��ʾ��ʽ��
        }
    }

    musicpSendCommand(MC_STOP, NULL);
    musicpSendCommand(MC_CLOSE, NULL);
    SetFullInterface();
    need_draw = TRUE;
}

//��ѡ��ǰ����
void SelectCurMusic(void)
{
    musicpSendCommand(MC_STOP, NULL);
    musicpSendCommand(MC_CLOSE, NULL);
    ChangeMusic(Music_Cur, Play_play); //���ŵ�ǰ����
    GetInformation(); //ȡ�ļ���Ϣ
    if (g_Openparam.typeerror == 0)
    {
        musicpSendCommand(MC_OPEN, (void*) &g_Openparam);
        musicpSendCommand(MC_LOAD, NULL);
        musicpSendCommand(MC_PLAY, NULL);
    }
}
//========================================================

/*���˵��ļ�ͷ��ʾ*/
void DisInFBHead(uint8 DisMod)
{
    if (DisMod == 0)
    {//��ͨ��ʾģʽ
    
       // if (g_currentMusicType != Music_Wav)
        temp_buffer[0] = 0;
        temp_buffer[1] = 0;
        temp_buffer[2] = 0;
        temp_buffer[3] = 0;
        temp_buffer[4] = 0;
       // SetTextPos(POSITION_F, 0);
      //  SetAsciiFont(FONT_TYPE_LARGE);
        //PutS(temp_buffer, -1); //��ͷ��ʾʱ��Ϊ0
//        if (g_currentMusicType != Music_Wav)
        ui_show_f(temp_buffer);
//       else
//            ui_show_ef(temp_buffer);
   //     SetAsciiFont(FONT_TYPE_DEFAULT);
        UpdateScreen(NULL);
    }
    else
    {//�����ʾģʽ
        region_t region;
        region.x = LyricIcon_Pos_X;
        region.y = LyricIcon_Pos_Y;
        region.width = 12;
        region.height = 8;
//        ResShowPic(LYRIC_R, LyricIcon_Pos_X, LyricIcon_Pos_Y);
        UpdateScreen(&region);
    }
    while (ap_get_message() != (AP_KEY_PREV | AP_KEY_UP))
    {
        ;//do nothing
    }
    //ChangeMusic(Music_Prev, Play_key);
    SelectCurMusic();
    ui_run_realtime = FALSE;
    UpDisplayFlag = UpDisplayFlag | up_musicbattery;
    PlayStatus = PlaySta;
    need_draw = TRUE;
}

/*������ļ�β��ʾ*/
void DisInFFEnd(uint8 DisMod)
{
	if(!Pedo_ui_draw)
    {  
    if (DisMod == 0)
    {//��ͨ��ʾģʽ
        // wma_play_status_t status_buf;
       // musicpSendCommand(MC_GETSTATUS, (int) (&status_buf));
       // itoa2(TimeRateData_sav.TotalTime_sav[0], temp_buffer);
       // if (g_currentMusicType != Music_Wav)
        //    temp_buffer[0] = ' ';
     //   temp_buffer[2] = ':';
        itoa1(TimeRateData_sav.TotalTime_sav[0], temp_buffer);
        itoa2(TimeRateData_sav.TotalTime_sav[1], &temp_buffer[1]);
        itoa2(TimeRateData_sav.TotalTime_sav[2], &temp_buffer[3]);
       // SetTextPos(POSITION_F, 0);
       // SetAsciiFont(FONT_TYPE_LARGE);
        //PutS(temp_buffer, -1); //��ͷ��βʱ��ʾ��ʱ��
//        if (g_currentMusicType != Music_Wav)
            ui_show_f(temp_buffer);
//        else
//            ui_show_ef(temp_buffer, 1);
      //  SetAsciiFont(FONT_TYPE_DEFAULT);
        UpdateScreen(NULL);
    }
    else
    {//�����ʾģʽ
        region_t region;
        region.x = LyricIcon_Pos_X;
        region.y = LyricIcon_Pos_Y;
        region.width = 12;
        region.height = 8;
//        ResShowPic(LYRIC_F, LyricIcon_Pos_X, LyricIcon_Pos_Y);
        UpdateScreen(&region);
	    }
    }
}

/*��������*/
void FadeOut(bool DisMod)
{
    music_play_status_t status_buf;
    DisMod = DisMod;
    while (1)
    {
        bool result = musicpSendCommand(MC_FADEINOUT, FADEOUT_DEF | FADEOUT_TIME_PERIOD);
        if (result != 0)
        {
            break;
        }

        musicpSendCommand(MC_GETSTATUS, (void*) (&status_buf));
        if ((status_buf.status == PLAYING_REACH_END) || (status_buf.status == PLAYING_REACH_HEAD) || (status_buf.status
                == PLAYING_ERROR))
        {
            break;
        }
        if ((status_buf.status == PLAYING_WAIT_A) || (status_buf.signal == SIGNAL_REACH_B) || (status_buf.signal
                == SIGNAL_REACH_V))
        {
            musicpSendCommand(MC_SETA_B, ClrABPoint);
            break;
        }
    }
}

void CheckVolumeLimit(void)
{
    if (g_music_vars.volume > SOFTVOL_MAX)
    {
        g_music_vars.volume = SOFTVOL_MAX;
    }

    if (g_music_vars.volume < 0)
    {
        g_music_vars.volume = 0;
    }
}

void CheckPlayrateLimit(void)
{
    if (g_music_vars.play_rate > 8)
    {
        g_music_vars.play_rate = 8;
    }
    if (g_music_vars.play_rate < -8)
    {
        g_music_vars.play_rate = -8;
    }
}

void CheckReplaygapLimit(void)
{
    if (g_music_vars.replaygap < 1)
    {
        g_music_vars.replaygap = 1;
    }
    if (g_music_vars.replaygap > 10)
    {
        g_music_vars.replaygap = 10;
    }
}

void CheckReplaytimesLimit(void)
{
    if (g_music_vars.replaytimes < 1)
    {
        g_music_vars.replaytimes = 1;
    }
    if (g_music_vars.replaytimes > 10)
    {
        g_music_vars.replaytimes = 10;
    }
}

