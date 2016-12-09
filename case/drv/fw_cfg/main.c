/*
*******************************************************************************
*                ACTOS AP
*        system setting ap of AS211A
*
*        (c) Copyright, Actions Co,Ld.
*             All Right Reserved
*
*  $Id: main.c,v 1.2 2010/03/29 10:48:01 Mars Exp $
*******************************************************************************
*/
#include "cfg.h"
#pragma name(CFG_MAIN)

/************************ͷ��Ϣ����**************************************/

//const char   CfgMagic[4]={"CFG"};
//const uint8  TotallItem = 5;
//const char   ReservHead1[11];
#pragma userclass(nconst = CFG_HEAD)
const  file_cfg_head_t FileHead = 
{
   "CFG",9,{0xfe,0xff,0x00}    
};

const cfg_item_t CfgItem[6] = 
{
   {(uint32)(&ParaHead),"Parameters","��������"},
   {(uint32)(&KeyCfg),"Key-press","����"},
   {(uint32)(&APCfg),"Application","Ӧ�ó���"},
   {(uint32)(&LanguageCfg),"language","����"},
   {(uint32)(&FileCfg),"File-exchange","�滻�ļ�"},
   {(uint32)(&FuncCfg),"Functions","��������"}        
};
const char ReservHead2[336];

#pragma userclass(nconst = CFG_PARA)

/******************��������********************************************/
const paraset_head_t ParaHead = 
{(uint32)ValueSet,14,(uint32)(&StringSet),2,""};

const value_t  ValueSet[14] = 
{
	#if(MACHINE_MODEL==1)
	{30,60,0,1,"","Auto Lock","�Զ���ʱ��(��)"},
    {2,15,0,1,"","contrast","�Աȶ�����"},
	#else
	#ifdef Greenhouse
	{0,60,0,1,"","Auto Lock","�Զ���ʱ��(��)"},    //30
    {15,15,0,1,"","contrast","�Աȶ�����"},
	#else
	{10,60,0,1,"","Auto Lock","�Զ���ʱ��(��)"},
    {15,15,0,1,"","contrast","�Աȶ�����"},
	#endif
	#endif
	
	#ifdef Greenhouse
	{60,60,0,1,"","backlight time","����ʱ��(��)"}, //30
    {5,60,0,1,"","Power off time","�ػ�ʱ��(����)"},//0
	#else
    {10,60,0,1,"","backlight time","����ʱ��(��)"},
    {1,60,0,1,"","Power off time","�ػ�ʱ��(����)"},
	#endif
	
    {1,1,0,1,"","AllwaysOnPlaying","�����в��ػ�"}, //g_modifyval.SleepTime

    #if(Volume_Ctl==2)
	{27,40,0,1,"","volume set","�����ȼ�"},
    {13,40,0,1,"","vol default","Ĭ������"},
    #elif(Volume_Ctl==1)
    {32,40,0,1,"","volume set","�����ȼ�"},
    {16,40,0,1,"","vol default","Ĭ������"},
    #else
    {40,40,0,1,"","volume set","�����ȼ�"},
    {24,40,0,1,"","vol default","Ĭ������"},
    #endif    
    
	{6,7,0,1,"","Record gain","¼������"},
	
    {50,200,20,1,"","GS Strid","��������"},
    {0,1,0,1,"","PedoUnit","��Ӣ��ת��"},
    {50,200,20,1,"","GS Weight","��������"},
	#if defined STAYER || defined Greenhouse
	{0,200,0,1,"","ArmLen  ","�۳�����"},
	#else
    {50,200,20,1,"","ArmLen  ","�۳�����"},
	#endif
    {50,200,5,1,"","PoolLen ","�س�����"}, 
    {0,2,0,1,"","Record mode","��¼ģʽ"},
};

const string_t StringSet[2]=
{
   {"AS211A_V1000","Device Version","�豸�汾"}, 
   #if(LCD_TYPE==1)
   {"23.02.01","Firmware Version","�̼��汾"} 
   #elif(LCD_TYPE==2)
   {"23.02.02","Firmware Version","�̼��汾"} 
   #elif(LCD_TYPE==3)
   {"23.02.03","Firmware Version","�̼��汾"} 
   #elif(LCD_TYPE==4)
   {"23.02.14","Firmware Version","�̼��汾"} 
   #elif(LCD_TYPE==5)
   {"23.02.15","Firmware Version","�̼��汾"} 
   #endif
   //�汾˵��:21.01.15--(2:2117c)(1:patch).(01:sw).(rsv)(lcd)
   //
};

#pragma userclass(nconst = CFG_KEY)


/********************��������**********************************************/
//const uint8 KeyTotal = 5;
//const uint8 TotalItem = 11;
//const char  KeyReserv[14];
//const uint8 keyValue[16]

const key_cfg_t KeyCfg = 
{
    9,11,"",
    {
    //#if(MACHINE_MODEL==1)
    //    Msg_KeyNull,Msg_KeyPlayPause,Msg_KeyREC,Msg_KeyMenu,Msg_KeyLast,Msg_KeyNext,Msg_KeyLoop,Msg_KeyVolAdd,Msg_KeyVolSub,Msg_KeyHold,Msg_KeyEQ
    //#elif((MACHINE_MODEL==2)||(MACHINE_MODEL==3))
        Msg_KeyNull,Msg_KeyPlayPause,Msg_KeyVolSub,Msg_KeyVolAdd,Msg_KeyLast,Msg_KeyNext,Msg_KeyMenu,Msg_KeyREC,Msg_KeyLoop,Msg_KeyHold,Msg_KeyEQ
    //#endif
    } 
};


const key_infor_t KeyInfor[11]=
{
    {Msg_KeyNull,0,"NULL"},
    {Msg_KeyPlayPause,1,"PLAY/PAUSE"}, //PLAY����������
    {Msg_KeyLoop,0,"LOOP"},
    {Msg_KeyVolSub,0,"VOL-"},
    {Msg_KeyVolAdd,0,"VOL+"},
    {Msg_KeyLast,0,"PREV"}, 
    {Msg_KeyNext,0,"NEXT"},
    {Msg_KeyMenu,0,"MENU"}, 
    {Msg_KeyREC,0,"REC/AB"},
    {Msg_KeyEQ,0,"EQ"}, 
    {Msg_KeyVol,0,"VOL"}
};

#pragma userclass(nconst = CFG_AP)

/********************AP ����*****************************/
//const uint8 TotalAP = 10;
//const uint8 AvtiveAP = 1;
//const char  Reserv[14];

const ap_cfg_head_t APCfg = 
{
    11,1,""    
};

const ap_infor_t APTable[11] =
{
    {RESULT_MUSIC,1,"Music","����"},
    {RESULT_MOVIE,0,"Movie","��Ӱ"},
    {RESULT_REC_NOSTART,0,"Record","¼��"},
    //#if((MACHINE_MODEL==2)||(MACHINE_MODEL==3))
    {RESULT_VOICE,0,"Voice","����"},
    //#else
    //{RESULT_VOICE,1,"Voice","����"},
    //#endif
    {RESULT_RADIO,1,"Radio","������"},
    {RESULT_VIEW,0,"Picture","ͼƬ���"},
    {RESULT_SYSTEM,1,"Setting","ϵͳ����"},    
    {RESULT_READER,0,"Reader","������"},    
    {RESULT_GAME,0,"Game","��Ϸ"},
	{RESULT_TELBOOK,0,"Tel-Book","�绰��"},
	{RESULT_BROWSER,0,"Browser","�ļ�����"}
};

#pragma userclass(nconst = CFG_LAN)
/******************��������*****************************************/
const lang_infor_t LanguageCfg = 
{   
    //10,2,
    //{
    //    CHINESE_SIMPLIFIED,CHINESE_TRADITIONAL,ENGLISH,/*JAPANESE,KOREAN,*/
    //    FRENCH,GERMAN,ITALIAN/*,DUTCH*/,PORTUGUESE,SPANISH,/*SWEDISH,CZECH,
    //    DANISH,*/POLISH,RUSSIAN/*,TURKISH,HEBREW,THAI,HUNGARIAN,ROMANIAN,
    //    ARABIC,INDONESIAN,GREEK*/ 
    //}

    #ifdef STAYER
    8,3,
    {
        CHINESE_SIMPLIFIED,CHINESE_TRADITIONAL,ENGLISH,JAPANESE,KOREAN,
        FRENCH,ITALIAN,SPANISH
    }
	#elif defined Greenhouse
	2,3,
    {
        ENGLISH,JAPANESE
    }
	#elif defined NetOnNet
    23,2,
    {
        CHINESE_SIMPLIFIED,CHINESE_TRADITIONAL,ENGLISH,JAPANESE,KOREAN,
        FRENCH,GERMAN,ITALIAN,DUTCH,PORTUGUESE,
        SPANISH,SWEDISH,CZECH,DANISH,POLISH,
        RUSSIAN,TURKISH,THAI,HUNGARIAN,ROMANIAN,
        INDONESIAN,GREEK,NORWEGIAN
    }
    #else
    22,2,
    {
        CHINESE_SIMPLIFIED,CHINESE_TRADITIONAL,ENGLISH,JAPANESE,KOREAN,
        FRENCH,GERMAN,ITALIAN,DUTCH,PORTUGUESE,
        SPANISH,SWEDISH,CZECH,DANISH,POLISH,
        RUSSIAN,TURKISH,THAI,HUNGARIAN,ROMANIAN,
        INDONESIAN,GREEK,/*NORWEGIAN,BULGARIAN,SLOVAK,UKRAINIAN,
        CROATIA,LITHUANIAN,ESTONIAN,LATVIAN,SERBIAN*/
    }
    #endif
};

/*************�ļ��滻******************/
#pragma userclass(nconst = CFG_FILE_EX)
//const uint8 FileTotal = 10;
//const char  FileExReserv[15];
const file_ex_t FileCfg = 
{
  17,""  
};

const char  FileName[][16]=
{
    "music.ap",
    "amv.ap",
    "record.ap",
    "voice.ap",
    "fmradio.ap",
    "viewpic.ap",
    "setting.ap",
    "reader.ap",
    "TelBook.ap",
    "game.ap",
    "main.ap",
    "udisk.ap",
    "standby.ap",
    "ky_hard.drv",
    "unicode.drv",
    "welcome.bin",
    "unicode.fon",
    "browser.ap"
};

/*����������*/
#pragma userclass(nconst = CFG_FUNC)
const func_cfg_t FuncCfg = 
{
 5,""   
};

const func_item_t FuncItem[5] = 
{
    {0,"","Support Card","֧�ֲ忨"},
    //#if(MACHINE_MODEL==1)
    //{1,"","Earphone","����ֱͨ"},
    //#elif((MACHINE_MODEL==2)||(MACHINE_MODEL==3))
    {0,"","Earphone","����ֱͨ"},
    //#endif
    {0,"","Class D","D�๦��"},
    {0,"","KeyTone","������"},  
    #if (PETOMETER_USE)
    {1,"","Pedometer","�Ʋ���"},
    #else
    {0,"","Pedometer","�Ʋ���"},
    #endif
};
#pragma userclass(nconst = default)
