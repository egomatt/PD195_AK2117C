/*
 ******************************************************************************
 *                               AS211A
 *                            Module: music
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: lyric display module
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xusong    2010-07-15 15:00     1.0             build this file
 ******************************************************************************
 */
#include "ap_music.h"

#pragma name(AP_UIA_LRC)

const region_t LRC_ICON_REG =
{ LyricIcon_Pos_X, LyricIcon_Pos_Y, 12, 8 };
const region_t regionTop =
{ 20, 32, 108, 16 };

#define ExproTimeDef 5//浏览的延时值
extern uint8 g_introCNT;
uint8 far LyricStrBuf[88]; //
uint8 far RT_buf[4]; //当前时间
void DisInFBHead(uint8 DisMod);

bool musicpSendCommand_lyric(uint8 cmd, void* param)
{
    switch (cmd)
    {
        case MC_GETTIME:
        case MC_GETSTATUS:
        case MC_GETAUDIOFMT:
        case MC_FADEINOUT:
		case MC_GETPOWERVALUE:
        return wavpSendCommand(cmd, param);
        default:
        return musicpSendCommand_bank(cmd, param);
    }
    return FALSE;
}
void GetNowTime(void)
{
    Full_time_t curtime_buf;
    if (!musicpSendCommand_lyric(MC_GETTIME, (void*) &curtime_buf))
    {
        *(uint32 *) RT_buf = 0;
        return;
    }
    //以下三句是把时间毫秒,秒,分，合成LONG形与表中数据进行比较（只比较大小）
    *(uint16*) &RT_buf[2] = curtime_buf.MS;
    //时间格式|MIN|SEC|MS_L|MS_H| //取出实时时间表
    RT_buf[1] = curtime_buf.TimeSecond;
    RT_buf[0] = curtime_buf.TimeMinute;
#ifdef PC
    // PC 环境先转换高低端
    SIM_ChgLongPC(RT_buf);
#endif
    return;
}

void PutLRC(uint8 * str, uint8 line)
{
    bool change_flag = FALSE;
    SetAsciiFont(FONT_TYPE_LARGE);
    SetTextPos(0, line * 16);
    PutStringExt(str, -1, TRUE, 0x01);
    while (1)
    {

        if (*(word*) str == 0x0000)
        {
            break;
        }
        else
        {
            //这里只做输出 by cailizhen
            str = PutStringExt(str, -1, TRUE, 0x02);
        }

        if ((*(word*) str != 0x0000) && (*(word*) str != 0x000d))
        {
            change_flag = !change_flag;
            SetTextPos(0, line * 32 + change_flag * 16);
            //str=str_save;
        }
        else
        {
            break;
        }
    }
}

void DisplayLyricStr(uint8 * str) //显示驱自动换行
{

//    ClearScreen(NULL);
//	ResShowPic(IM_MUSIC_BG,0,0);
	ClearScreen(&regionTop);
    SetAsciiFont(FONT_TYPE_LARGE);
#if 0
    PutLRC(str,0);
#else
    SetTextPos(20, 32);
    PutS(str,-1);    
#endif
//    InvertRegion(&regionTop);
    //ui_show_batt(GetBattery());
    UpdateScreen(&regionTop);
/*
    if(lyricGetNext(LyricStrBuf, sizeof(LyricStrBuf)))
   	{
#if 0        
        PutLRC(str,1);
#else
        SetTextPos(0, 32);
        PutS(str,-1);         
#endif        
        UpdateScreen(&regionTop);
        lyricGetPrev(LyricStrBuf, sizeof(LyricStrBuf));
    }

*/


}



uint16 ui_lyric(void)
{
    uint8 key;
    uint8 result;
    bool KeyUpFlag = FALSE;
    bool ExproFlag = FALSE; //true: 在浏览模式
    wma_play_status_t status_buf;
    uint32 CurTime = 0, NextTime = 1;//同歌词load命令

    //先刷屏,关闭背光时间
    //VMRead(&g_comval, VM_SYSTEM, sizeof(g_comval));
    //g_comval.LightTime = 0;
    //ap_message_init(&g_comval);
    g_introCNT = 0;
    //need_draw = TRUE;

    while (1)
    {
        do
        {
            //if (need_draw != 0)
            {
                SetPLL(PLL_80MHZ);//matt140206#2
                GetNowTime();
                //ResShowPic(IM_MUSIC_BG,0,0);
                lyricSeekFor(LyricStrBuf, sizeof(LyricStrBuf), *(uint32 *) RT_buf);
                lyricGetPageTime(&CurTime, &NextTime);
                DisplayLyricStr(LyricStrBuf);
                //need_draw = FALSE;
                SetPLL(DEFAULT_FREQ);//matt140206#2
            }
			
			return NULL;
			
            do
            {
                key = ap_get_message(); //读键值
                result = ap_handle_hotkey(key);
                if (result == RESULT_UDISK)
                {
                    return result;
                }
                //这里是处理快退大文件时可能丢掉KEYUP键的问题
            } while (((PlayStatus == FFPlaySta) || (PlayStatus == FBPlaySta)) && (key != 0) && (key != (AP_KEY_PREV
                    | AP_KEY_UP)) && (key != (AP_KEY_NEXT | AP_KEY_UP)));

            switch (key)
            {
                case AP_MSG_STANDBY: //standby 不响应
                break;

                case AP_KEY_AB | AP_KEY_UP: //rec 不响应
                break;

                case AP_MSG_RTC:
                /* 浏览模式下切换歌曲*/
				/*
                if ((g_music_vars.repeat_mode == Intro)&&((g_introCNT++) >= 20))
                {
                    g_introCNT = 0;
                    g_result = DealMusicFileEnd(1);
                    if (g_result == RESULT_UI_STOP)
                    {
                        return g_result;
                    }
                }
                if ((g_music_vars.repeat_mode == Intro)&&(!g_lyricflag))
                {
                    return NULL;
                }
                ui_show_batt(GetBattery());*/
                break;

                case AP_KEY_PLAY | AP_KEY_UP:
#ifdef OPT_Sup_FadeOut_fun
                FadeOut(FALSE);
#endif
                BackupCurrentTime();
                musicpSendCommand_lyric(MC_STOP, NULL);
                musicpSendCommand_lyric(MC_BACKUP, (void*) &g_music_vars.BreakPTSave);
                musicpSendCommand_lyric(MC_CLOSE, NULL);
                SetFullInterface()
                g_music_vars.MusicOpenMod = Nor_Break_open;
                return RESULT_UI_STOP; //退回上一层

                case AP_KEY_VOLADD | AP_KEY_UP: //下一屏歌词
                if (lyricGetNext(LyricStrBuf, sizeof(LyricStrBuf)) != 0)
                {
                    DisplayLyricStr(LyricStrBuf); //显示歌词内容
                }
                ExproFlag = TRUE;
                g_2HZ_CNT = 0;
                break;

                case AP_KEY_VOLSUB | AP_KEY_UP: //上一屏歌词
                if (lyricGetPrev(LyricStrBuf, sizeof(LyricStrBuf))!=0)
                {
                    DisplayLyricStr(LyricStrBuf); //显示歌词内容
                }
                ExproFlag = TRUE;
                g_2HZ_CNT = 0;
                break;

                case AP_KEY_MODE | AP_KEY_UP:
                if (!KeyUpFlag)
                {
                    KeyUpFlag = TRUE;
                    break;
                }
                return NULL;

                case AP_KEY_NEXT | AP_KEY_LONG: //快进
                PlayStatus = FFPlaySta;
                musicpSendCommand_lyric(MC_STEP, 4);
                break;

                case AP_KEY_PREV | AP_KEY_LONG: //快退
                PlayStatus = FBPlaySta;
                musicpSendCommand_lyric(MC_STEP, -4);
                break;

                case AP_KEY_NEXT | AP_KEY_UP: //快进
                if (PlayStatus == PlaySta)
                {
                    FadeOut(FALSE);
                    g_result = ToNextMusic(Play_key); //选下一首
                    if (g_result != 0)
                    {
                        return g_result;
                    }
                    if (!g_lyricflag)
                    {
                        return NULL;
                    }
                }
                else
                {
                    musicpSendCommand_lyric(MC_STEP, 0);
                    PlayStatus = PlaySta;
                }
                break;

                case AP_KEY_PREV | AP_KEY_UP:
                if (PlayStatus == PlaySta)
                {
                    g_result = PrevMusicInPlay();
                    if (g_result != 0)
                    {
                        return g_result;
                    }

                    if (!g_lyricflag)
                    {
                        return NULL;
                    }
                }
                else
                {
                    musicpSendCommand_lyric(MC_STEP, 0);
                    PlayStatus = PlaySta;
                }
                break;

                case AP_KEY_MODE | AP_KEY_HOLD: //MODE键没抬起时不响应
                if (!KeyUpFlag)
                {
                    continue;
                }
                break;

                case AP_MSG_SD_OUT:
                if (g_music_vars.location.disk == 'H')
                {
                    musicpSendCommand_lyric(MC_STOP, NULL);
                    musicpSendCommand_lyric(MC_CLOSE, NULL);
                    g_music_vars.location.disk = 'C';
                    DisplayErrMsg(CARDPULL);//显示卡拔出
                    return RESULT_UI_STOP;
                }
                break;

                default:
                key = ap_handle_hotkey(key);
                if (key == RESULT_REDRAW)
                {
                    need_draw = TRUE;
                }
                else if (key == RESULT_AUTO_BACK)
                {
                }
                else if (key != 0)
                {
                    return key;
                }
                else
                {
                }
                break;
            } //switch(key)
            musicpSendCommand_lyric(MC_GETSTATUS, (void*) (&status_buf));

            if (status_buf.status == PLAYING_REACH_END)
            {
                g_result = DealMusicFileEnd(1);

                if (g_result == RESULT_UI_STOP)
                {
                    return RESULT_UI_STOP;
                }
                if (!g_lyricflag)
                {
                    return NULL;
                }
            }

            if (status_buf.status == PLAYING_REACH_HEAD)
            {
                if (PlayStatus == PlaySta)
                {
                    DisInFBHead(1);//到头显示
                    SelectCurMusic();
                }
                else
                {
                    g_result = DealMusicFileEnd(1);
                    if (g_result == RESULT_UI_STOP)
                    {
                        return RESULT_UI_STOP;
                    }
                    if (!g_lyricflag)
                    {
                        return NULL;
                    }
                }
            }

            if (g_2HZ_CNT > ExproTimeDef)
            {
                ExproFlag = FALSE;
            }

        } while ((ExproFlag && (g_2HZ_CNT <= ExproTimeDef)) || (PlayStatus == FFPlaySta) || (PlayStatus == FBPlaySta));

        GetNowTime();
        if (((*(uint32 *) RT_buf) >= NextTime) || ((*(uint32 *) RT_buf) < CurTime))
        {
            lyricSeekFor(LyricStrBuf, sizeof(LyricStrBuf), *(uint32 *) RT_buf);
            lyricGetPageTime(&CurTime, &NextTime);
            DisplayLyricStr(LyricStrBuf);
        }

    }
    //while(1)
    return 1;
} //ui_lyric


