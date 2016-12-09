/********************************************************************************
 *                              USDK 5102
 *                            Module: POWER MANAGER
 *                 Copyright(c) 2001-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      wuyueqian     2009-9-07 9:42     1.0             build this file
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
#include "basal.h"
#include "decC.h"

#ifdef PC
extern void decAudioIntSub(void);
extern void decDMAIntSub(void);
extern void decTimerIntSub(void);
#endif

#pragma name(MW_BS_AD_CMD1)

#define DECODE_BUFFER_ADDRESS_NORMAL 0x0600//0x1800
#define WRONG_DECODE_FILE_TYPE 0xf0

extern HANDLE m_deFileHandle;
extern Open_param_t*m_deOpenparam;//�������ַ

extern uint32 deBpointerSave;
extern uint32 deBreakPointSave;
extern decBreakPointInfor_t m_breakPointInfo;
extern uint32 deApointerSave;
extern uint32 deFilepages;
extern BYTE m_backupmode;
extern play_status_t play_status;
extern bool m_deABok_sav;
extern BYTE * m_deDataBuffer;
extern uint16 m_deOldDspIntAddrSav;
extern uint16 m_deOldDmaIntAddrSav;
extern time_t m_Totaltimelength;
extern uint8 m_decTimer_Sav;

bool LoadCommand(BYTE music_type, void* param);
bool StopCommand(void* param);
bool PlayCommand(void* param);
bool CaltimeCommand(void* param);
bool RestoreCommand(void);
bool BackupCommand(void);
bool APointPlayCommand(void);

#ifndef PC
#pragma ROM(large)
extern uint8 m_deRead_Data(uchar readFileDirection, DWORD position);
#pragma ROM(huge)
#else
extern uint8 m_deRead_Data(uchar readFileDirection, DWORD position);
#endif

#pragma renamecode(MW_BS_AD_CMD1)

BOOL OpenCommand(void* param)
{
    m_deOpenparam = (Open_param_t *) param;
    if (m_deOpenparam != 0)
    {
        if (m_deOpenparam->typeerror != 0)
        {
            play_status.status = PLAYING_ERROR;
            m_Totaltimelength.second = 0;
            m_Totaltimelength.minute = 0;
            m_Totaltimelength.hour = 0;
            return FALSE;
        }
        g_decControlInfor.SoftVolumeMax = 0;
        g_decControlInfor.FadeInTime = m_deOpenparam->FadeInTime;
        g_decControlInfor.FadeOutTime = 0x03;//shortest
        
        g_decSongInfor.SampleRate = m_deOpenparam->SampleRate;
    }
    return TRUE;	
}

/*************************************************************************
 * LoadCommand ��ʼ��codec�õ�ȫ�ֱ���g_decInitPara��
 * ����codec��buffer��dma�жϵ�hook�����������ļ��ĵ�һ֡���ݵ�ַ�ȵ�
 ****************************************************************************/
bool LoadCommand(BYTE music_type, void* param)
{
    BankSwitch(BYTE2_OF(m_deRead_Data));
    BankSwitch(BYTE2_OF(decAudioIntSub));

    m_deDataBuffer = (BYTE*) DECODE_BUFFER_ADDRESS_NORMAL;

    g_decReadFilePara.ReadFileHandleHook = (DWORD) m_deRead_Data;
    g_decReadFilePara.ReadFileBuffAddr = (WORD) (m_deDataBuffer);
    g_decControlInfor.SongType = music_type;

    if (m_deOpenparam != NULL)
    {
        g_decSongInfor.FileHDRLen = m_deOpenparam->musicframeoffset;
        memcpy(&m_Totaltimelength, &(m_deOpenparam->musictotaltime), sizeof(time_t));
    }
    else
    {
        g_decSongInfor.FileHDRLen = 512;
    }

    //play_status.speed=0x00;
    m_deABok_sav = FALSE;

    //g_decControlInfor.SoftVolumeMax = 41;	 //���ϲ�ȥ�趨������ÿ�ε��û��޸�֮ǰ���ù���ֵ
    //g_decControlInfor.Volume = 23;

    g_decControlInfor.ChannelSelect = 0;//˫ͨ��

    g_decControlInfor.TimeLimit = 50; //auto ab play
    g_decControlInfor.EnergyLimit = 8; //auto ab play

    play_status.status = PLAYING_STOP;

    m_backupmode = 0;

    if (m_deOpenparam != NULL)
    {
        m_deFileHandle = FS_FOpen(m_deOpenparam->filename, FS_OPEN_NORMAL);
    }
    else
    {
        m_deFileHandle = FS_FOpen((uint8*) param, FS_OPEN_NORMAL);
    }

    if (m_deFileHandle == NULL)
    {
        play_status.status = PLAYING_ERROR;
        return FALSE;
    }

    {
        uint32 EndPageLen;//���һ��PAGE���ֽ���
        EndPageLen = (uint32)FS_GetUnalignedLen(m_deFileHandle);//ȡ���һ��PAGE��BYTE��
        deFilepages = (uint32)FS_GetFileLen(m_deFileHandle);
        g_decReadFilePara.FileLen = (((uint32)deFilepages)<<9); //* 512;

        //�����ļ�β������PAGE������,һҳһҳ����
        if (EndPageLen != 0)
        {
            g_decReadFilePara.FileLen = g_decReadFilePara.FileLen - 512 + EndPageLen;
        }
        if(g_decSongInfor.FileHDRLen>=g_decReadFilePara.FileLen)
        {
            return FALSE;
        }
    }

    if (g_decReadFilePara.FileLen == 0)
    {
        play_status.status = PLAYING_ERROR;
        return FALSE;
    }

    return TRUE;

}

/*************************************************************************
 * CaltimeCommand ͨ��AUDIO IP�����������ʱ��
 *����WMA WAV MP3������ͨ�������������ʱ��
 ****************************************************************************/

bool CaltimeCommand(void* param)
{

    param = param;
    if (play_status.status == PLAYING_ERROR)
    {
        return FALSE;
    }
    g_decControlInfor.PlayMode = PLAY_MODE_CALTIME; //  codec ����������������
    decInit();

    m_deOldDspIntAddrSav = IRQ_Intercept((uint) &(decAudioIntSub), IRQ_DSP);
    m_deOldDmaIntAddrSav = IRQ_Intercept((uint) &(decDMAIntSub), IRQ_DMA2);
    m_decTimer_Sav = TM_SetTimer((int16) decTimerIntSub, 2);

    decstart();
    do
    {
        ClearWatchDog();
        if (g_decCurStatus.Status >= WRONG_DECODE_FILE_TYPE)
        {
            play_status.status = PLAYING_ERROR;
            break;
        }

    } while (g_decCurStatus.GetSongInfo != 0xff);//while(0);

    memcpy(&m_Totaltimelength, &(g_decSongInfor.TotalPlayTime.TimeHour), sizeof(time_t));

    decExit();
    TM_KillTimer((int8)m_decTimer_Sav);
    IRQ_Release(m_deOldDspIntAddrSav, IRQ_DSP);
    IRQ_Release(m_deOldDmaIntAddrSav, IRQ_DMA2);

    return TRUE;

}

/*************************************************************************
 * StopCommand ֹͣ����
 *1 g_decReadFilePara.ReadFilePages��dma hook�б���ֵ = deBreakPointSave
 ****************************************************************************/

bool StopCommand(void* param)
{
    m_backupmode = (uint8) param;
    while (!decExit())
    {
        ; //qac
    }        
    //g_decBreakPointInfor.BreakPointAddr = g_decReadFilePara.ReadFileCurAddr;//�����ļ��ϵ���Ϣ
    TM_KillTimer((int8)m_decTimer_Sav);
    IRQ_Release(m_deOldDspIntAddrSav, IRQ_DSP);
    IRQ_Release(m_deOldDmaIntAddrSav, IRQ_DMA2);

    play_status.status = PLAYING_STOP;
    return TRUE;
}

/*************************************************************************
 * PlayCommand ����
 *
 ****************************************************************************/
bool PlayCommand(void* param)
{
    if (play_status.status == PLAYING_ERROR)
    {
        return FALSE;
    }
    if (play_status.status == PLAYING_PAUSE)
    {
        decContinue();
        play_status.status = PLAYING_PLAYING;
        return TRUE;
    }

    play_status.status = PLAYING_PLAYING;//Ĭ��Ϊ����״̬
    if ((uint8) param == ABPlay_open)
    {
        RestoreCommand();
        //���deApointerSave = deFilepages��ʾ���һҳ�������ٸ�
        if ((deApointerSave < deFilepages))
        {
            play_status.status = PLAYING_AB;
            g_decControlInfor.ABSetFlag = AB_FLAG_DEAL_AB_PROCESS;//Ϊ�˽���RD ���ļ�ʱ�ͼ��öϵ��Ƿ񳬹�B����
            memcpy(&g_decBreakPointInfor, &m_breakPointInfo, sizeof(decBreakPointInfor_t));
            g_decControlInfor.PlayMode = PLAY_MODE_BREAK;
        }
        //�ص��ļ�ͷ��codec�����deApointerSave��seek������ȡ����
        FS_FSeek(0, 0, m_deFileHandle);
    }
    else if ((uint8) param == Nor_Break_open)
    {
        RestoreCommand();
        if (deBreakPointSave < deFilepages)
        {
            g_decControlInfor.PlayMode = PLAY_MODE_BREAK;
            memcpy(&g_decBreakPointInfor, &m_breakPointInfo, sizeof(decBreakPointInfor_t));
        }
        FS_FSeek(0, 0, m_deFileHandle);
    }
    else
    {
        //Ҫ��ͷ��ʼ��
        g_decControlInfor.PlayMode = PLAY_MODE_NORMAL;
        memset(&g_decBreakPointInfor, 0x0, sizeof(decBreakPointInfor_t));
        FS_FSeek(0, 0, m_deFileHandle);
    }

    if (m_deOpenparam == NULL)
    {
        g_decControlInfor.PlayMode = PLAY_MODE_AB;//���������ַΪ�����ʾΪ�����ԱȲ���
    }

    decInit();

    m_deOldDspIntAddrSav = IRQ_Intercept((uint) &(decAudioIntSub), IRQ_DSP);
    m_deOldDmaIntAddrSav = IRQ_Intercept((uint) &(decDMAIntSub), IRQ_DMA2);
    m_decTimer_Sav = TM_SetTimer((int16) decTimerIntSub, 2);
    decstart();
    return TRUE;
}

//other bank combine
/*

 bool BackupCommand(void)
 {
 m_deOpenparam->BreakPTSave->ApointSave = deApointerSave;
 m_deOpenparam->BreakPTSave->BpointSave = deBpointerSave;
 m_deOpenparam->BreakPTSave->breakPT = deBreakPointSave;
 if(m_backupmode == 0)//�ϵ���������
 {
 memcpy(&(m_deOpenparam->BreakPTSave->breakPointInfor), &g_decBreakPointInfor, sizeof(decBreakPointInfor_t));
 }
 else//AB ���������еĶϵ㲻���棬ֻ�Ǳ���A�㣬�Ժ�ϵ�������A�㿪ʼ
 {
 memcpy(&(m_deOpenparam->BreakPTSave->breakPointInfor), &m_breakPointInfo, sizeof(decBreakPointInfor_t));
 }
 return TRUE;
 }

 bool RestoreCommand(void)
 {
 deApointerSave = m_deOpenparam->BreakPTSave->ApointSave;
 deBpointerSave = m_deOpenparam->BreakPTSave->BpointSave;
 deBreakPointSave = m_deOpenparam->BreakPTSave->breakPT;
 memcpy(&m_breakPointInfo, &(m_deOpenparam->BreakPTSave->breakPointInfor), sizeof(decBreakPointInfor_t));
 return TRUE;
 }

 bool APointPlayCommand(void)
 {
 g_decControlInfor.PlayMode = PLAY_MODE_BREAK;
 memcpy(&g_decBreakPointInfor, &m_breakPointInfo, sizeof(decBreakPointInfor_t));
 FS_FSeek(0, FS_SEEK_FFROMSTART, m_deFileHandle);
 decInit();//����ʱ�䲻����
 decstart();
 return TRUE;
 }

 */
