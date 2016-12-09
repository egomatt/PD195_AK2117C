#include <actos.h>
#include <basal.h>
#include <decC.h>

#pragma name(MW_BS_AD_CMD)

extern HANDLE m_deFileHandle;
extern Open_param_t *m_deOpenparam;
extern play_status_t play_status;
extern bool m_deABok_sav;
extern uint16 m_deOldDspIntAddrSav;
extern time_t m_Totaltimelength;
extern uint8 m_decTimer_Sav;
extern uint16 m_deOldDmaIntAddrSav;
#ifndef PC
extern uint8 c_decTimerCnt;
#else
uint8 c_decTimerCnt;
#endif
extern bool m_deAPELastSecond;
//const signed char VPSTranTable[17]={-32,-28,-24,-20,-16,-12,-8,-4,0,8,16,24,32,40,48,56,64};

extern BOOL OpenCommand(void* param);
extern bool LoadCommand(BYTE music_type, void* param);
extern bool StopCommand(void* param);
extern bool PlayCommand(void* param);
extern bool CaltimeCommand(void* param);
extern bool BackupCommand(void);
extern bool RestoreCommand(void);
extern bool APointPlayCommand(void);
extern BYTE mp3check(void);

#define CAN_NOT_CALL_RESERVE_INTERFACE() while(1){;}

#pragma renamecode(MW_BS_AD_CMD)

#ifndef PC
BOOL sapSendCommand(BYTE cmd, BYTE music_type, void* param)
#else
BOOL apSendCommand(BYTE cmd, BYTE music_type, void* param)
#endif
{
    play_status_t *pStatus;
    audio_format_t *pFormat;

#ifdef PC
    if(DealCommandPC(cmd, music_type, param) == FALSE)
    return FALSE;
#endif

    switch (cmd)
    {
        case MC_OPEN:
        return OpenCommand(param);
        break;

        case MC_CALTIME:
        return CaltimeCommand(param);

        case MC_SETSOFTVOLUMEMAX:
        if (((uint8) param) != 0)
        {
            g_decControlInfor.SoftVolumeMax = 40;
        }
        else
        {
            g_decControlInfor.SoftVolumeMax = 0;
        }
        break;

        case MC_SET_CHANNEL:

        g_decControlInfor.ChannelSelect = (uint8) param;

        break;
        case MC_LOAD:
        return LoadCommand(music_type, param);

        case MC_PLAY:
        m_deAPELastSecond = FALSE;
        return PlayCommand(param);

        case MC_PAUSE:
        //CAN_NOT_CALL_RESERVE_INTERFACE()
        break;

        case MC_STOP:
        return StopCommand(param);

        case MC_STEP:
        if(((int8)param) < 0)
        {
            g_decControlInfor.FFRevFlag = FAST_BACKWARD;
            g_decControlInfor.FFRevSpeed = (uint8)(0 - ((int8)param));
        }
        else if(((int8)param)> 0)
        {
            g_decControlInfor.FFRevFlag = FAST_FORWARD;
            g_decControlInfor.FFRevSpeed = ((uint8)param);
        }
        else
        {
            g_decControlInfor.FFRevFlag = STEP_NORMAL;
            g_decControlInfor.FFRevSpeed = 0;
        }
        decFFRevDeal();
        break;

        case MC_FADEINOUT:
        //                if(play_status.status == PLAYING_ERROR)
        //				{
        //					return FALSE;
        //				}
        if (((uint8)param & 0xf0) == FADEIN_DEF)
        {
            g_decControlInfor.FadeInTime = (uint8)param & 0x0f;//���õ����־
            return TRUE;
        }
        else if (((uint8)param & 0xf0) == FADEOUT_DEF)
        {
            g_decControlInfor.FadeOutTime = (uint8)param & 0x0f;//���õ�����־
            return TRUE;
        }
        else
        {
                     	
        }
        break;

        case MC_CLOSE:
        if(m_deFileHandle != NULL)
        {
            FS_FClose(m_deFileHandle);
            m_deFileHandle = 0;
        }
        break;

        case MC_SETVOLUME:
        g_decControlInfor.Volume = (uint8)param;
        decSetVolume();
        break;

        case MC_SETA_B:
        switch((uint8)param)
        {
            case ClrABPoint://�������ʹ�ã�AP��signal = reach_Bʱ��⵽��������-1�ͷ�AB_FINISHED����
            if (play_status.status == PLAYING_WAIT_A)
            {
                APointPlayCommand();
            }
            play_status.status = PLAYING_PLAYING;
            g_decControlInfor.ABSetFlag = AB_CLEAR;
            play_status.signal = 0;
            m_deABok_sav = FALSE;//�˳�ABʱҪ��˱�־,�����´�ABֻ��һ��
            break;

            case SetAPoint://SET A
            g_decControlInfor.ABSetFlag = AB_SET_A;
            break;

            case SetBPoint://SET B
            g_decControlInfor.ABSetFlag = AB_SET_B;
            break;

            case ReturnApoint://����A�㲥��
            if(play_status.status == PLAYING_WAIT_A)
            {
                play_status.status = PLAYING_AB;
                APointPlayCommand();
            }
            break;
            case StopBPlay:
            decExit();
            play_status.status = PLAYING_WAIT_A;
            break;
            case ABFinished:
            m_deABok_sav = TRUE;
            break;

            default:
            break;
        }
        break;

        case MC_SETUSEREQ:
        memcpy(g_decControlInfor.EQVal, (const void *)param,sizeof(g_decControlInfor.EQVal));
        break;

        case MC_SETEQ:

        g_decControlInfor.EQFlag = (uint8)param;
        if(play_status.status == PLAYING_PLAYING)
        {
            decEffectDeal();
        }
        break;

        case MC_AUTO_AB:
        g_decControlInfor.SentDectFlag = (uint8)param;
        break;

        case MC_BACKUP:
        return BackupCommand();
        case MC_RESTORE:
        //CAN_NOT_CALL_RESERVE_INTERFACE()
        break;
        //return RestoreCommand();
        case MC_SETSPEED:
        //CAN_NOT_CALL_RESERVE_INTERFACE()        
         //play_status.speed = (uint8)param;
         //g_decControlInfor.PlaySpeed = (uint8)param;
        g_decControlInfor.EQFlag = 0x77;
        g_decControlInfor.EQVal[0] = (uint8)param;
        if(play_status.status == PLAYING_PLAYING)
        {
            decEffectDeal();
        }         
        break;

        case MC_SETCALLBACK:
        break;

        case MC_GETTIME:
        memcpy((Full_time_t *)param, &g_decCurStatus.NowTime, sizeof(DWORD));
        break;

        case MC_GETSAPLE:
        *((uint8*)param) = g_decSongInfor.SampleRate;
        break;

        case MC_GETSTATUS:
        pStatus = (play_status_t *)param;
        memcpy(&(pStatus->total_time), &m_Totaltimelength, sizeof(time_t));

        /*2013-1-9 15:02 ape ������β�˳����ж�����*/
        if((g_decControlInfor.SongType == 6) && (g_decCurStatus.NowTime != 0))
        {			
            if (g_decCurStatus.NowTime >= *(DWORD*)&g_decSongInfor.TotalPlayTime)
            {
                play_status.status = PLAYING_REACH_END;
            }
            else if ( (g_decCurStatus.NowTime == (*(DWORD*)&g_decSongInfor.TotalPlayTime - 1))
                   && (play_status.status == PLAYING_PLAYING) )
            {
                if (m_deAPELastSecond == FALSE)
                {
                    c_decTimerCnt = 0;
                    m_deAPELastSecond = TRUE;
                }
                else
                {
                    if (c_decTimerCnt >= 200)//2���Ӵ���
                    {
                        play_status.status = PLAYING_REACH_END;
                    }
                }
            }
        }


        if((play_status.status == PLAYING_REACH_END) || (play_status.status == PLAYING_REACH_HEAD))
        {
            pStatus->status = play_status.status;
            break;
        }

        play_status.volume = g_decControlInfor.Volume;
        pStatus->volume = g_decControlInfor.Volume;

        pStatus->signal = play_status.signal;//
        play_status.signal = 0;
        play_status.eq = g_decControlInfor.EQFlag;

        pStatus->status= play_status.status;
        if(play_status.status != PLAYING_REACH_END)
        {
            if(g_decCurStatus.Status >= 0xf0)
            {
                play_status.status = PLAYING_ERROR;
                while(!decExit())
                {
                    ;//qac	
                }
                TM_KillTimer((int8)m_decTimer_Sav);
                IRQ_Release(m_deOldDspIntAddrSav,IRQ_DSP);
                IRQ_Release(m_deOldDmaIntAddrSav,IRQ_DMA2);

            }
        }

        break;

        case MC_GETAUDIOFMT:
        if(play_status.status == PLAYING_ERROR)
        {
            return FALSE;
        }
        pFormat = (audio_format_t *)param;
        memcpy(pFormat->bit_rate, ((uint8 *)(&(g_decSongInfor.BitRate)))+1, (sizeof(DWORD)-1));
        pFormat->bit_rate[0] += 0x30;
        pFormat->bit_rate[1] += 0x30;
        pFormat->bit_rate[2] += 0x30;
        pFormat->bit_rate[3] = 0;
        pFormat->sample_rate = g_decSongInfor.SampleRate;
        pFormat->channel = g_decSongInfor.Channel;
        break;

        case MC_SETPOWERCOUNT:
        g_decControlInfor.EnergyOutFlag = (uint8)param;
        break;
        case MC_GETPOWERVALUE:
        *((uint8*)param) = g_decCurStatus.EnergyLevel;
        break;
        default:
        return FALSE;
    }
    return TRUE;
}
