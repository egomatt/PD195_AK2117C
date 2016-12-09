/*
******************************************************************************* *                               AS211A
*                              AS211A
*                              Module: music ap
*                 Copyright(c) 2002-2010 Actions Semiconductor,
*                            All Rights Reserved. 
*
* Descripton: the head file of music
*
* History:         
*      <author>    <time>           <version >             <desc>
*       xusong    2010-07-14 11:00     1.0             build this file 
 *******************************************************************************
 */
#ifndef _AP_MUSIC_H
#define _AP_MUSIC_H
#include "ap_common.h"//�ؼ�ͷ�ļ�
#include "mmcmd.h"//ģ�������������
#include "ap_option.h"//AP���ܿ���ѡ��
#include "basal.h"//basal module ͷ�ļ�
#include <rand.h>

#define SOFTVOL_MAX    40
#define CARD_KERNEL 1

//���뵭��ʱ�䣬1 100ms, 2 200ms, 3 400ms, 4 800ms, 5 1.6s
#define FADEOUT_TIME_PERIOD     3 //400ms
#define FADEIN_TIME_PERIOD      2 //200ms

#define RESULT_CURR_UI NULL//�ڵ�ǰ����
#define RESULT_LAST_UI RESULT_USER1+1//����һ����
#define RESULT_UI_STOP RESULT_USER1+2//�� STOP ����
//�ڶ��ַ�ʽ�ص� STOP ����,���ַ�ʽ����ֹͣ����
#define RESULT_UI_STOP2 RESULT_USER1+3
#define RESULT_UI_PLAY  RESULT_USER1+4//�� PLAY ����
#define RESULT_LIST         RESULT_USER1+5  //�б�����Ϣ
#define RESULT_LIST_ERR     RESULT_USER1+6  //�б����ڻ���������
#define RESULT_PLAYLIST_PLAY     RESULT_USER1+7  //�����б��е��ļ���Ϣ
#define RESULT_USERLIST     RESULT_USER1+8

/*�л������������ַ�ʽ*/
#define Music_Reset_Head 0
#define Music_Reset_End 1
#define Music_Cur   2//ѡ��ǰ
#define Music_Next  3//ѡ��һ��
#define Music_Prev  4//ѡ��һ��
#define POSITION_LRCBG     (0)//�����ʾ����ͼ����
#define POSITIONY_LRCBG     (0)

#define POSITION_LRCTOP     (6)//�����ʾ�ı�����
#define POSITIONY_LRCTOP     108//(93)
#define POSITION_LRCMID     (6)//�����ʾ�ı�����
#define POSITIONY_LRCMID     70//(57)
#define POSITION_LRCBOT     (6)//�����ʾ�ı�����
#define POSITIONY_LRCBOT    23// (21)
#define SOFTVOLUMEMAX     40  //�������������
#define MENU_GROUP         5  //һ����ʾ�Ĳ˵���

//--------- [UpDisplayFlag] ---Ҫ��ˢ�±�־
#define up_musictypeflag    0x8000
#define up_lyricflag        0x4000
#define up_musicbitrate     0x2000
#define up_musicloopmode    0x1000
#define up_musicsequence    0x0800
#define up_MPRLmode         up_musicsequence
#define up_musiceqmode      0x0400
#define up_musiccurrenttime 0x0200
#define up_musictotaltime   0x0100
#define up_musicbattery     0x0080
#define up_musicinformation 0x0040
#define up_musicBackGDPic   0x0001    //ˢ�±���
//-----------------------------���ˢ�±�־
#define clr_musictypeflag    0x7fff
#define clr_lyricflag        0xbfff
#define clr_musicbitrate     0xdfff
#define clr_musicloopmode    0xefff
#define clr_musicsequence    0xf7ff
#define clr_MPRLmode         clr_musicsequence
#define clr_musiceqmode      0xfbff
#define clr_musiccurrenttime 0xfdff
#define clr_musictotaltime   0xfeff
#define clr_musicbattery     0xff7f
#define clr_musicinformation 0xffbf
#define clr_musicBackGDPic   0xfffe    //�������

//defines for ui lyric
#define LyricIcon_Pos_X 96//116
#define LyricIcon_Pos_Y 40//48

//=========������ʾ����=============
#define SetDisplayBackGDPic()       \
{                                   \
    UpDisplayFlag = UpDisplayFlag |     \
                    up_musicBackGDPic;  \
}

//=========����STOP�������==========
#define SetStopInterface()          \
{                                   \
    ui_auto_update=FALSE;               \
    UpDisplayFlag=UpDisplayFlag |       \
                  up_musictypeflag |    \
                  up_lyricflag |        \
                  up_musicbitrate |     \
                  up_musicloopmode |    \
                  up_musicsequence |    \
                  up_musiceqmode |      \
                  up_musiccurrenttime | \
                  up_musictotaltime |   \
                  up_musicbattery;      \
}

//=========����ˢ��ȫ�������=====
#define SetFullInterface()          \
{                                   \
    ClearScreen(NULL);              \
    ui_auto_update=FALSE;               \
    UpDisplayFlag=UpDisplayFlag |       \
                  up_musictypeflag |    \
                  up_lyricflag |        \
                  up_musicbitrate |     \
                  up_musicloopmode |    \
                  up_musicsequence |    \
                  up_musiceqmode |      \
                  up_musiccurrenttime | \
                  up_musictotaltime |   \
                  up_musicbattery |     \
                  up_musicBackGDPic |    \
                  up_musicinformation;  \
}

//=========����ˢ��ȫ�������=====
#define SetStopToPlayInterface() \
{                                \
    ui_auto_update=FALSE;            \
    UpDisplayFlag=UpDisplayFlag |    \
                  up_musictypeflag | \
                  up_lyricflag |     \
                  up_musicbitrate |  \
                  up_musicloopmode | \
                  up_musicsequence | \
                  up_musiceqmode |   \
                  up_musicbattery;   \
}

#define StartIntro(mode)                  \
{                                         \
    g_introCntMode = mode;                    \
    g_OldSecTempBackup                        \
        = g_music_vars.BreakPTDisTime.second; \
    g_introCNT = 0;                           \
}

/*music Ӧ�ýṹ�嶨��*/
typedef enum
{
    NormalPlay, //��ͨģʽ����ʾ(0)
    WaitSetAPointForAB, //AB�ŵĴ���A��(1)
    WaitSetBPointForAB, //AB�ŵĴ���B��(2)
    ABLoopPlay, //ABѭ������(3)
    FollowRec,//����¼��(4)
    FollowAB,//������AB(5)
    ComparePlay,//�ԱȲ���(6)
    CompareAB  //�Աȷ�AB(7)
} Replay_status_t;

typedef enum
{
    StopSta, //ֹͣ(0)
    PauseSta, //��ͣ(1)
    PlaySta, //��ͨģʽ����ʾ(2)
    FFPlaySta, //���״̬(3)
    FBPlaySta  //����״̬(4)
} Play_status_t;

typedef enum
{
    LoopNor, //��ͨѭ��
    LoopOne, //ѭ���ŵ�ǰ��
    LoopOneDir, //ѭ���ŵ�ǰĿ¼
    LoopDir, //ѭ����Ŀ¼
    LoopAll, //ѭ������
    Random, //�������
    Intro, //�������
    FolderShift,
    MaxRepeatMod
} Repeat_Mode_t;

typedef enum
{
    Music_NON = 0, Music_MP1, Music_MP2, Music_MP3, Music_WMA,
    //  Music_ASF,
    Music_WMV,
    Music_Wav,
    Music_APE,
    Music_FLAC,
    Music_TypeMax
} Music_type_t;

typedef enum
{
    Stop_key, Play_play, Play_key, list_key
} Music_sel_mod_t;

//AP_MUSICʹ�õ�ϵͳ����
typedef struct
{
    uint8 Wow;
    uint8 Bass;
    uint8 Srs;
    uint8 Center; //����
    uint8 Definition; //Ƶ��ͻ��
    uint8 Speakersize;
    uint8 Limiter;
    uint8 GainSetting;
    uint8 IsNewFlag;
} SRSMod_t; //SRS WOW ��Ч

//AP_MUSICʹ�õ�ϵͳ����
typedef struct
{
    uint16 magic; // +0
    uint8 volume; //
    Repeat_Mode_t repeat_mode; //repeat �˵������  //+1
    eq_t eq_type; //+2
    int8 play_rate; //�����ٶ� //+3
    uint8 replaytimes; //��������// +4
    uint8 replaygap; //�������//+5
    Open_mod_t MusicOpenMod; //�򿪷�ʽ +6
    file_location_t location; //+ 9
    breakPT_t BreakPTSave; //�ϵ���� +36
    time_t BreakPTDisTime; //�ϵ�ʱ�䱸�� + //
    uint8 fselmod; //�ļ�ѡ������ģʽ
    SRSMod_t SRSMod;
    uint8 SRSModeFlag;
    uint8  vol_display; //����ֵ����ʾ
} music_vars_t;

typedef struct
{ //�������ڸ���¼���ͶԱȷ�������ʾԭ����
    uint8 Bitrate_sav[3]; //������
    uint8 TotalTime_sav[3]; //��ʱ��
} TimeRateData_sav_t;

typedef struct
{
    uint16 HeadIcon;//ͷ��ͼ���IDֵ
    uint16 HeadStr;//ͷ����Ϣ��IDֵ
    uint8 *string;//��Ϣ��ʾ������ʾ���ִ���ַ
} dialog_t; //�Ի���

//===========================================================
//���������ⲿ����
//===========================================================
extern music_vars_t g_music_vars;
extern Replay_status_t RePlayStatus;
extern Play_status_t PlayStatus;
extern uint16 UpDisplayFlag;
extern uint16 g_result; //���õ�16λ��ʱ����
//extern slider_t slider;
extern Music_type_t g_currentMusicType;
extern uint16 g_musicamount;
extern uint16 g_musicsequence;
extern uint8 g_replaytimes;
extern uint8 g_replaypause;
extern uint8 g_ScrollFlag;
extern bool g_lyricflag;
extern uint16 g_2HZ_CNT;
extern bool g_2HZFlag;
extern uint16 g_ABPointTimeSize;
extern uint8 g_TagInfoBuf[92];
extern uint8 g_TagInfoBuf1[];
extern region_t scrollscreensize;
extern bool g_ifAVflag;
extern BOOL need_draw;
extern uint8 g_volume;
extern bool g_introCntMode;
extern uint8 g_OldSecTempBackup;
extern uint8 g_introCNT;
extern comval_t g_comval;
extern uint8  g_ReplayTimeCNT;
extern TimeRateData_sav_t TimeRateData_sav;
extern uint8 g_FadeInStatusFlag;
extern uint8  g_ABMinTimeCNT;
extern Open_param_t g_Openparam;
extern bool g_CardInFlag;
extern uint8 g_DiskStateFlag;
extern uint8 MotivePosion0;
extern uint8 MotivePosion1;
extern bool ifSelBuffToltalTimeFlag;
extern bool CaltimeFlag;
extern uint8 WaitForCaltimeCNT;
extern uint8 savenum;//ʱ�������saveʱ��
extern bool g_from_browser;
extern uint8  return_to_play; //�Ƿ�Ӳ˵����淵��play����ĺ���
extern bool g_FCexitflag;
extern uint8 fsel_mode_change_flag;
extern uint8 g_ScrollTimeCNT;
extern uint8  g_MusicType;

extern bool ReturnNotInit; //TRUE:��ʾ��һ�����ò��ó�ʼ��,
extern uint8 Replaytimes_CNT; //
extern uint16 ABPointTimeSize;

extern bool g_LDBFlag;
extern bool g_file_error_flag;
extern void (*fun_decrypt_hook)(void *, uint32);
extern uint8 tag_is_unicode;
//extern const region_t mainscreensize;
extern uint8 temp_buffer[9];
//extern function_menu_t menu;
//*********************************************
extern void make_dirname(void);
extern void show_volume_menu();
extern void OutPutVolume(uint8 vol);
extern void DisInFBHead(uint8 DisMod);//���˵�ͷ��ʾ
extern void Deal_Error_Playing(void); //����ʱ������
extern void SetStopToPlayInterfaceForPlay(void);//ֹͣ���浽���Ž���ת��
extern void EQMode_callback(uint8 value); //EQ���õĻص�����

extern uint16 ui_stop_loop(void);

//������������ԭ��
//--------class isr----------------
#pragma ROM(large)
void Music_timeint(void);
void Music_2HZint(void);
#pragma ROM(huge)
void data_decrypt(void *para, uint32 sector);

void volume_callback(uint16 Vol);
void speed_callback(uint16);
void eq_callback(uint16);
void srs_callback(uint16 srs);
void bass_user_callback(uint16 bass_user);
void srs_user_callback(uint16 srs_user);
void wow_user_callback(uint16 wow_user);
void center_user_callback(uint16 center_user);
void definition_user_callback(uint16 definition);
void speakersize_user_callback(uint16 speakersize);
void limiter_user_callback(uint16 limiter);
void send_user_srs();
void loop_callback(uint16);
void listloop_callback(uint16 loop);
void fselget_callback(uint16 fselget);
//--------------------------------
//-------class ui-------
uint16 ui_stop(void);
uint16 ui_play(void);
uint16 ui_lyric(void);
uint16 ui_play_menu(uint8 Playsta);//Playsta �������ĸ�����
uint16 ui_stop_menu(void);
uint16 ui_EQ_menu(void);
uint16 ui_SRS_menu(void);
uint16 ui_SRSUSER_menu(void);
uint16 ui_loop_menu(void);
uint16 ui_listloop_menu(void);
uint16 ui_follow_compare(void);
uint16 ui_followAB(void); //����AB
uint16 ui_followRec(void); //����¼��
uint16 ui_compareAB(void); //�Ƚ�AB
uint16 ui_comparePlay(void); //�Ƚϲ���
uint16 Deal_music_menu(void);
uint16 UI_Favourite(void);
uint16 music_list_menu(void);
uint16 Deal_in_menu(void);
//---------------------------

//-------class display-------
uint16 RealDisplay(void);
void ShowNowTime(void);
void RefreshMainScreen(void);
void Show_Bitrate(void); //��ʾ������
void DisplayErrMsg(uint16 ErrMsg);
void Show_MusicType(void);//��ʾ��������ͼ��ssssss
//---------------------------

//-------class functions-----
Music_type_t CheckMusicType(uint8 * filename);
uint16 play_key_next(uint16 key);
uint16 play_key_prev(void);
uint16 ToNextMusic(Music_sel_mod_t mod);
void GetInformation(void);
bool ChangeMusic(uint8 Direct, Music_sel_mod_t mod);
void SelectCurMusic(void);//ѡ��ǰ����
bool musicpSendCommand(uint8, void*);
bool musicpSendCommand_bank(uint8 cmd, void* param);
//void StartIntro( bool mode );
bool DealErrCase(void);
uint16 ChangeVolume(void);
uint16 DealMusicFileEnd(uint8 DisMod);
uint16 DealPlayKey(void);
void CheckVolumeLimit(void); //��������Ƿ񳬳���Χ
void CheckPlayrateLimit(void); //��ⲥ���ٶ��Ƿ񳬳���Χ
void CheckReplaygapLimit(void); //��⸴������Ƿ񳬳���Χ
void CheckReplaytimesLimit(void); //��⸴�������Ƿ񳬳���Χ
void BackupCurrentTime(void);
void FadeInOutFun(void);
void FadeOut(bool DisMod);
uint16 PrevMusicInPlay(void);
void ClearTimePara(void);//��0��ʾʱ��ֵ���ϵ���0
void DealForABGap(music_play_status_t *status);
void ClosePA(void);
void openPA(void);
void DisplayErrInfor(uint16 id);
uint16 InitFileSelector(void);
uint16 StartFileSelector(void);
uint16 UpdateFileSelector(void);
void ErrReport(Play_status_t status);
void OpenPA(void);
void FlashMotived(uint8 value);

uint16 DealKeyResult(uint8 key);
uint16  DealKeyForPlayStatus(uint8 key);

#endif     //_ap_music_H
