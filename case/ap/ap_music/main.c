/*
 ******************************************************************************
 *                               AS211A
 *                            Module: music
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: main module of music
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xusong    2010-07-15 15:00     1.0             build this file
 ******************************************************************************
 */
#include "ap_music.h"
/*********ģ����ⲿ����*********************************************/
extern kval_t kval;
extern uint8 earphoneinflag;

#pragma name(AP_MAIN)
/*�����ζ���*/
const uint8 ResFileName[] = "ui30.res";
//const region_t mainscreensize =
//{ 0, 0, 128, 16 };

/*ȫ�����ݶζ���*/
uint8 SRSModeFlag = 0;
bool g_introCntMode;
bool need_draw;
bool g_FCexitflag;
bool g_lyricflag = FALSE;//���޸�ʱ�־,true��ʾ��
bool g_ScrollFlag = FALSE; //����ʱ�жϵ�ʱ�������־
bool g_2HZFlag = TRUE;//2HZ�жϵ���־
bool g_PAOpenFlag = FALSE; //��־PA�Ŀ���״̬

bool ifSelBuffToltalTimeFlag = FALSE;//TRUE���� buffer ��ʱ������
bool g_from_browser = FALSE;
bool CaltimeFlag = FALSE; //true: ��������ʱ��

bool ReturnNotInit; //TRUE:��ʾ��һ�����ò��ó�ʼ��,
uint8 Replaytimes_CNT; //
uint16 ABPointTimeSize;

uint8 g_TagInfoBuf[92];//���ڷ�TAG��Ϣ��BUF
uint8 g_OldSecTempBackup;//��¼��ǰ���ֵ,���ڹ۲���ı仯,�Ӷ������仯�ź���
uint8 g_introCNT;
int8 MusicTimer_Sav;
int8 Timer_2HZNo_Sav;

uint8 g_DiskStateFlag = 0;//b7 = 1:flash ���Ѹ�ʽ��
uint8 fsel_mode_change_flag = FALSE;
uint8 g_ScrollTimeCNT = 0;//������Ƶ������

//uint8 g_LDBFlag = 0;//bit<0> ��ʾ ����Ƿ���unicode ��bit<1>����ʾldb��ʽ
uint8 g_MusicType = FSEL_TYPE_MUSIC;
//uint8 PlayMenuactive = 0;//���Ų˵��Ļ��
uint8 StopMenuactive = 0;//ֹͣ�˵��Ļ��
uint16 UpDisplayFlag = 0; //��Ӧ��λ��ʾҪ������ʾ�����ݡ�
uint16 g_musicamount = 0;//��ǰĿ¼��������
uint16 g_musicsequence = 0;//��ǰ�������
uint16 g_2HZ_CNT;//2hz �жϼ�����(0.5���һ)
uint16 g_ABPointTimeSize;//��¼AB�㳤��
uint16 g_result;//��ʾ16λ����
Open_param_t g_Openparam;

comval_t g_comval; //ϵͳ����,ap���ܸ���
music_vars_t g_music_vars;//��AP�Ĺ�������
Replay_status_t RePlayStatus = NormalPlay; //��ͨ����ģʽ
Play_status_t PlayStatus = StopSta; //ֹͣ״̬
Music_type_t g_currentMusicType = Music_NON;//��־��ǰ���ֵ�����(mp3/wma/asf/wmv)
uint8  g_ReplayTimeCNT; //��¼��������
uint8  g_ABMinTimeCNT = 0;//������Сʱ��������
uint8  return_to_play = 0; //Ϊ1��ʾ���������淵�ز��Ž���
uint8  tag_is_unicode;
TimeRateData_sav_t TimeRateData_sav;
void (*fun_decrypt_hook)(void *, uint32);
//uint8 temp_buffer[9] =
//{ 0 };

/*�ļ��ڵ��ú�������*/
void read_var(void);
void openDAC(void);
void openDAC(void)
{
    dac_t dacattr;
    dacattr.dacselect = 0;
    dacattr.rate = FS_44K1;//Ĭ��
    EnableDAC(&dacattr);
}

int16 main(int16 param)
{
    int16 main_result;
    SD_FILE* g_Resfp_Sav;
    g_DiskStateFlag = 0;
    g_FCexitflag = FALSE;
    /*param����1����ʱΪVoiceģʽ*/
    if (param > 1)
    {
        param -= 2;
        g_MusicType = FSEL_TYPE_VOICE;
    }
    if (param != 0)
    {
        g_from_browser = TRUE;
    }
    /* ���빫������ */
    read_var();

    #ifdef Greenhouse
	g_music_vars.vol_display = g_comval.Volume_G;
	#endif
	
    /*����Դ�ļ�,�ڶ��빫������֮�󣬸���language id��ʾ��ȷ���ַ�����Դ*/
    g_Resfp_Sav = ResOpen(ResFileName);
    ui_auto_update = TRUE;
    /*��ʾ���Ժ�*/
    show_waiting();
    //��ʼ��ȫ�ֱ���
    
    //====================================
	{
//	 char  buf1[5];
	 //ClearScreen(NULL);
 /*    itoa4(g_music_vars.location.ClusterNo,buf1);
     SetTextPos(0,0);
	 PutS(buf1, -1);
	 itoa4(g_music_vars.location.DirEntry,buf1);
     SetTextPos(0,32);
	 PutS(buf1, -1);*/
	 //ResShowPic(IM_WAITING,0,0);
     //UpdateScreen(NULL);
/*	 ap_sleep(2);
	 ap_sleep(2);
	 ap_sleep(2);
	 ap_sleep(2);
	 ap_sleep(2)*/;
	}
	//====================================
	
    g_music_vars.location.ID3buffer = g_TagInfoBuf;
    g_Openparam.filename = g_music_vars.location.filename;//�ļ�����ַ
    g_Openparam.BreakPTSave = &g_music_vars.BreakPTSave;//���ݲ�����ַ
    g_Openparam.typeerror = 0;//Ĭ��Ϊ������ʽ�ļ�
    g_Openparam.SoftVolumeMax = 0;
    g_Openparam.FadeInTime = 0x03;
    memset(&TimeRateData_sav, 0, sizeof(TimeRateData_sav_t));
    g_music_vars.fselmod = FSEL_TYPE_COMMONDIR;//ÿ�ν��붼����ΪĿ¼ѡ����

#ifdef OPT_Sup_LRC_fun
    lyricInit(LYRIC_SCREEN_128_32, g_comval.langid);//��ʼ����ʽ�����
#endif
    Timer_2HZNo_Sav = (int8) TM_SetClock((int) Music_2HZint, 1);//�� 2HZ ��ʱ
    MusicTimer_Sav = (int8) TM_SetTimer((int) Music_timeint, 2);//�� 20ms ��ʱ,����ˢ��ID3������Ϣ
    openDAC();
    OpenPA();
    if ((g_comval.ClassDFlag!=0) && (g_comval.KeyTone > 0)) //Ӳ��֧��classD
    {
#ifndef PC
        earphoneinflag = 1; //����ֱ����CLD���Զ��л�
#endif
    }
    SetLanguage((uint8) g_comval.langid);

	fun_decrypt_hook = NULL;
    //fun_decrypt_hook = data_decrypt;

    if (InitFileSelector() != 0)
    {   
        main_result = (int16)ui_stop();// =============> �������
    }
    else
    {
        main_result = RESULT_NULL;
    }
    /*����������ʱ���ر�PA*/
    if (!g_comval.KeyTone) //key tone add by mzh 2007.3.14
    {
        if (g_comval.EarphoneFlag == 0)
        {
            DisablePA();
        }
        else
        {
            DisablePADDV();
        }

        if ((g_comval.ClassDFlag!=0) && (g_comval.KeyTone == 0)) //Ӳ��֧��classD
        {
            DisableCLD();
#ifndef PC
            earphoneinflag = 0;
#endif
        }
        DisableDAC();
    }
    TM_KillTimer(MusicTimer_Sav);//�ر� 20ms ��ʱ
    TM_KillClock(Timer_2HZNo_Sav);//�ر� 2hz ��ʱ
    ResClose(g_Resfp_Sav);
    g_music_vars.location.fselTotal = 0;
    if (g_MusicType == FSEL_TYPE_MUSIC)
    {
        VMWrite(&g_music_vars, VM_AP_MUSIC, sizeof(g_music_vars));
        if(*(dir_layer_buff + 73) == 0x5a)
        {
            FS_GetOrSetDir_Exfat(dir_layer_buff, 0, EXFAT_GET_DIRLAYER);
            VMWrite(dir_layer_buff, VM_AP_MUSIC + 0x400, 74);
        }
    }
    else
    {
        VMWrite(&g_music_vars, VM_AP_VOICE, sizeof(g_music_vars));
        if(*(dir_layer_buff + 73) == 0x5a)
        {
            FS_GetOrSetDir_Exfat(dir_layer_buff, 0, EXFAT_GET_DIRLAYER);
            VMWrite(dir_layer_buff, VM_AP_VOICE + 0x400, 74);
        }
    }
	
	#ifdef Greenhouse
	g_comval.Volume_G = g_music_vars.vol_display;
	#endif
	
    //#if((MACHINE_MODEL == 2)||(MACHINE_MODEL==3))
	VMWrite(&g_comval, VM_SYSTEM, sizeof(g_comval));
    //#endif
    return main_result;
}

/*
 *****************************************************************************
 ** ����:
 ** �ӿ�:
 ** ����: �� play ״̬����������ı���
 ** �������:
 ** �������:
 ** ʹ��˵��:
 *******************************************************************************
 */
void ErrReport(Play_status_t status)
{
    ap_sleep(2);
    if ((g_music_vars.location.disk == 'H') && (DRV_DetectUD(1) != 0x20))
    {
        DisplayErrMsg(CARDPULL);//��ʾ���γ�
    }
    else
    {
        DisplayErrMsg(MFMTERR);//��ʾ��ʽ��
    }
    if ((status != StopSta) && (status != PauseSta))//����״̬��ֹͣdsp
    {
        musicpSendCommand(MC_STOP, NULL);
    }
    musicpSendCommand(MC_CLOSE, NULL);
}

/*
 ********************************************************************************
 *             void read_var(void)
 *
 * Description : ��ϵͳ������ap�ֲ�����
 *
 * Arguments   : void
 *
 * Returns     : void
 *
 * Notes       :
 *
 ********************************************************************************
 */
void read_var(void)
{
    int16 result;

    result = VMRead(&g_comval, VM_SYSTEM, sizeof(g_comval));
    if (g_comval.magic != MAGIC_COMVAL)
    {
        GetDefaultComval(&g_comval);
    }
    ap_message_init(&g_comval);
    if (g_MusicType == FSEL_TYPE_MUSIC)
    {
        result = VMRead(&g_music_vars, VM_AP_MUSIC, sizeof(g_music_vars));
    }
    else
    {
        result = VMRead(&g_music_vars, VM_AP_VOICE, sizeof(g_music_vars));
    }
#ifdef PC
        g_comval.VolumeMax = 40;
#endif
	//g_comval.VolumeMax = 32;
    if (g_music_vars.magic != MAGIC_MUSIC)
    {
        g_music_vars.magic = MAGIC_MUSIC;
		#ifdef Greenhouse
	    g_music_vars.vol_display = g_comval.Volume_G;
        g_music_vars.volume = g_comval.Volume_G;//(g_music_vars.vol_display * SOFTVOL_MAX) / g_comval.VolumeMax;
		#else
        g_music_vars.vol_display = g_comval.VolumeDef;
        g_music_vars.volume = g_comval.VolumeDef;//(g_music_vars.vol_display * SOFTVOL_MAX) / g_comval.VolumeMax;
		#endif
		
        g_music_vars.eq_type = 0;
        g_music_vars.replaytimes = 5;
        g_music_vars.replaygap = 1;
        g_music_vars.repeat_mode = FSEL_ALL_SEQUENCE;
        g_music_vars.play_rate = 0;
        g_music_vars.location.disk = 'C';
        g_music_vars.fselmod = FSEL_TYPE_COMMONDIR;
        g_music_vars.MusicOpenMod = Nor_open;
        g_music_vars.SRSMod.Wow = 8;
        g_music_vars.SRSMod.Bass = 8;
        g_music_vars.SRSMod.Srs = 5;
        g_music_vars.SRSMod.Center = 4;
        g_music_vars.SRSMod.Definition = 6;
        g_music_vars.SRSMod.Speakersize = 1;
        g_music_vars.SRSMod.Limiter = 0;
        g_music_vars.SRSMod.GainSetting = 30;
        g_music_vars.SRSMod.IsNewFlag = 1;
        g_music_vars.SRSModeFlag = 0;
#ifdef OPT_Sup_FselFolder_fun
        g_music_vars.fselget_type = Normal;
#endif
#ifdef OPT_Sup_FFRevSpeed_fun
        g_music_vars.FFRevSpeed = 1;
#endif
#ifdef OPT_Sup_FFRevSound_fun
        g_music_vars.FFRevSound = 0;
#endif
#ifdef OPT_Sup_FadeInOutTime_fun
        g_music_vars.FadeInOutTime = 5;
#endif
        strcpy(g_music_vars.location.path, FS_CD_ROOT);
        ClearTimePara();
    }
    //#if((MACHINE_MODEL==2)||(MACHINE_MODEL==3))
    if ((g_from_browser != 0) && (g_comval.MTPFormatType != 17))
    //#else
    //if (g_from_browser != 0)
    //#endif
    {
        browser_vars_t temp_browser_vars;
        VMRead(&temp_browser_vars, VM_AP_BROWSER, sizeof(browser_vars_t));
        if(*(dir_layer_buff + 73) == 0x5a)
        {
            VMRead(dir_layer_buff, VM_AP_BROWSER + 0x400, 73);
            VMWrite(dir_layer_buff, VM_AP_MUSIC + 0x400, 74);
        }
        memcpy(&g_music_vars.location, &temp_browser_vars.location, sizeof(file_location_t));
        g_music_vars.location.APE_Cur = 0;
        ClearTimePara();
    }
#ifdef OPT_Sup_FadeInOutTime_fun
    if(g_music_vars.FadeInOutTime> 15)
    {
        g_music_vars.FadeInOutTime = 10;
    }
#endif

    if(g_comval.PedoFlag)   //pedometer
    {
        //if(Pedo_Swim_flag)
        {
            sKY_BankGsensorGetStep(&Swim_total_step,&Swim_total_sec,1);
            Swim_work_flag = sKY_BankGsensorGetADC(6,1);
        }
        //else
        {
            sKY_BankGsensorGetStep(&Pedo_total_step,&Pedo_total_sec,0);
            Pedo_work_flag = sKY_BankGsensorGetADC(6,0);
        }
        Pedo_Swim_flag = Swim_work_flag;
    }

}

void ClearTimePara(void)
{
    g_music_vars.MusicOpenMod = Nor_open;
    g_music_vars.BreakPTSave.breakPT = 0; //��ʼ�ϵ�Ϊ0
    g_music_vars.BreakPTDisTime.hour = 0;
    g_music_vars.BreakPTDisTime.minute = 0;
    g_music_vars.BreakPTDisTime.second = 0;
    g_music_vars.BreakPTSave.ApointSave = 0; //��ʼ�ϵ�Ϊ0
    g_music_vars.BreakPTSave.BpointSave = 0; //��ʼ�ϵ�Ϊ0
    StartIntro(TRUE)//��Ϊʱ����0�ˣ��������Ҫ��ͷ��ʼ
}

void OpenPA(void)
{

    pa_t pa =
    { 0 };

    if (!g_PAOpenFlag)
    {
        g_PAOpenFlag = TRUE;//��־PA�Ѵ�

        pa.pa_in.dacin = 1;//Ĭ��
        pa.volume = 0;
        if (g_comval.EarphoneFlag == 0)
        {
            EnablePA(&pa);
        }
        else
        {
            EnablePADDV(&pa);
        }

    }
    //openDAC();
}

/*
 ****************************************************************************************
 *Description:����ֹͣ���Ž������
 *   RESULT_CURR_UI: ��Ҫ���ڵ�ǰ����
 *   RESULT_UI_STOP: ��UI_STOP���棨�����ھ���UI_STOP���棬����ͬ�ϣ�
 *   RESULT_LAST_UI: ����һ������
 *********************************************************************************************
 */

uint16 ui_stop(void)
{   
    ap_message_init(NULL);//��ʼ��STANDBY ʱ��
    SetFullInterface();
    need_draw = TRUE;
    if (g_MusicType == FSEL_TYPE_MUSIC)
    {
        if(*(dir_layer_buff + 73) == 0x5a)
        {
            VMRead(dir_layer_buff, VM_AP_MUSIC + 0x400, 73);
        }
    }
    else
    {
        if(*(dir_layer_buff + 73) == 0x5a)
        {
            VMRead(dir_layer_buff, VM_AP_VOICE + 0x400, 73);
        }
    }
    do
    {
        /*��ͣ״̬��ȥ�����ļ���Ϣ���ӿ���ͣ�ٶ�*/
        if (PlayStatus != PauseSta)
        {  
            {
                if (UpdateFileSelector() == 0)
                {
                    return 0;
                }

                g_musicamount = FSELGetTotal();
                g_musicsequence = FSELGetNo();
            }	 
            FSELSetMode(g_music_vars.repeat_mode);
            make_dirname();
            GetInformation(); //ȡ�ļ���Ϣ
           // PlayStatus = PauseSta; //Ϊ��ͣʱ����ȥ��ȡ��ǰʱ�䣬ʹ��֮ǰ����Ķϵ�ʱ�䣬
         PlayStatus=StopSta;
        }
	
        if (g_Openparam.typeerror == 0)
        {
            musicpSendCommand(MC_OPEN, (void*) &g_Openparam);
            if (!musicpSendCommand(MC_LOAD, NULL))
            {
                musicpSendCommand(MC_CLOSE, NULL);
                g_Openparam.typeerror = 1;
            }
            else
            {
                CaltimeFlag = TRUE;
            }

        }
        else
        {
            CaltimeFlag = TRUE;
        }
        musicpSendCommand(MC_SETPOWERCOUNT, 5);

        g_result = ui_stop_loop();
        if (g_result == RESULT_SD_OUT)
        {
            g_music_vars.location.fselTotal = 0;
            g_result = RESULT_UI_STOP2;
        }

    } while ((g_result == RESULT_CURR_UI) || (g_result == RESULT_UI_STOP) || (g_result == RESULT_UI_STOP2) || (g_result
            == RESULT_MUSIC_PLAY));//��������ui_stop����
    return g_result;//�������ص���һ��
}
