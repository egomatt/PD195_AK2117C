/*
 ******************************************************************************
 *                               AS211A
 *                            Module: music
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: display module of music
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xusong    2010-07-15 15:00     1.0             build this file
 ******************************************************************************
 */
#include "ap_music.h"
#pragma name(AP_SUB_DIS)
const uint16 EQ_PIC[8] =
{ IM_EQ_001_NORMAL, IM_EQ_003_ROCK, IM_EQ_002_POP, IM_EQ_004__CLASS, IM_EQ_009_SOFT, IM_EQ_005_JAZZ, IM_EQ_008_DBB, IM_EQ_007_USER };
const uint16 LOOP_PIC[7] =
{ IM_MUSIC_REPEAT_000_NORMAL, IM_MUSIC_REPEAT_001_REPEAT1, IM_MUSIC_REPEAT_004_FOLDER_NORMAL,IM_MUSIC_REPEAT_006_FOLDER_REPEATSHUFFLE,
  IM_MUSIC_REPEAT_002_REPEATALL, IM_MUSIC_REPEAT_003_REPEATSHUFFLE, IM_MUSIC_REPEAT_007_FOLDER_SHUFFLE}; 
const uint16 SONG_NUM[11]=
{SYSFONT_SMALL_0,SYSFONT_SMALL_1,SYSFONT_SMALL_2,SYSFONT_SMALL_3,SYSFONT_SMALL_4,
 SYSFONT_SMALL_5,SYSFONT_SMALL_6,SYSFONT_SMALL_7,SYSFONT_SMALL_8,SYSFONT_SMALL_9,SYSFONT_SMALL_SLASH};

void ui_show_data_h(uint8 *buf, uint8 x, uint8 y);
 

void ui_show_a(uint16 id)
{
    ResShowPic(id, POSITION_A, 48);
}

void ui_show_type(uint16 id)
{
    ResShowPic(id, 20, 0);
}
//id =0, clear the icon
void ui_show_c(uint16 id)
{
     ResShowPic(id, 0, 0);
}
 
void ui_show_d(const uint8 *string)
{
    uint8 i;
    for(i=0;i<9;i++)
    {
      ResShowPic(SONG_NUM[*(string+i)-0x30],0+i*6,48);
    }
	
}

void ui_show_f(const uint8 *string/*, uint8 part*/)
{
/*    uint8 i; 
    for(i=0;i<5;i++)
  	{
        ResShowPic(TIME_NUM[*(string+i)-0x30],22+5*i,8);
  	}*/
	uint8 id;    
	id=*string-0x30;    
	ResShowPic(SYSFONT_SMALL_0+id,90,48);    
	ResShowPic(ICON_MY_COLON,90+6,48);    
	string++;    
	id= *string-0x30;    
	ResShowPic(SYSFONT_SMALL_0+id,90+9,48);    
	string++;    id= *string-0x30;    
	ResShowPic(SYSFONT_SMALL_0+id,90+15,48);    
	ResShowPic(ICON_MY_COLON,90+21,48);    
	string++;    
	id= *string-0x30;    
	ResShowPic(SYSFONT_SMALL_0+id,90+24,48);    
	string++;    
	id= *string-0x30;    
	ResShowPic(SYSFONT_SMALL_0+id,90+30,48);
    
}


void ui_show_d_pic(uint16 id/*, uint8 type*/)
{    
  
     ResShowPic(id, 44, 0); 
}

void ui_show_loop(uint16 id)
{
	 ResShowPic(id, 61, 0);
}


void  ui_show_ef(const uint8 *string/*, uint8 part*/)
{ 
	uint8 id;    
	id=*string-0x30;    
	ResShowPic(SYSFONT_SMALL_0+id,22,16);    
	ResShowPic(ICON_MY_COLON,22+6,16);    
	string++;    
	id= *string-0x30;    
	ResShowPic(SYSFONT_SMALL_0+id,22+9,16);    
	string++;    id= *string-0x30;    
	ResShowPic(SYSFONT_SMALL_0+id,22+15,16);    
	ResShowPic(ICON_MY_COLON,22+21,16);    
	string++;    
	id= *string-0x30;    
	ResShowPic(SYSFONT_SMALL_0+id,22+24,16);    
	string++;    
	id= *string-0x30;    
	ResShowPic(SYSFONT_SMALL_0+id,22+30,16);
}

/*��ʾ��������Ϣ*/
//�����ʵ�Сͼ��(���ڱȱ�׼ASCIIС������Ҫ���һ��ͼ��)
const uint16 NUM_PIC[10] =
{IM_MUSIC_MP3_NUMBER_0,IM_MUSIC_MP3_NUMBER_1,IM_MUSIC_MP3_NUMBER_2,IM_MUSIC_MP3_NUMBER_3,IM_MUSIC_MP3_NUMBER_4,
 IM_MUSIC_MP3_NUMBER_5,IM_MUSIC_MP3_NUMBER_6,IM_MUSIC_MP3_NUMBER_7,IM_MUSIC_MP3_NUMBER_8,IM_MUSIC_MP3_NUMBER_9};
void Show_Bitrate(void)
{
    audio_format_t pFormat;    
    if (PlayStatus == PauseSta)
    {
        //�״ν���ʱ��ֹͣʱ��ͨ��checktype��ȡ�����ʣ�
    }
    else if (PlayStatus != StopSta)
    {   
        musicpSendCommand(MC_GETAUDIOFMT, (void*) &pFormat);   
        if ((pFormat.bit_rate[0] < '0') || (pFormat.bit_rate[0] > '9') || (pFormat.bit_rate[1] < '0')
                || (pFormat.bit_rate[1] > '9') || (pFormat.bit_rate[2] < '0') || (pFormat.bit_rate[2] > '9'))
        {
            pFormat.bit_rate[0] = '9';
            pFormat.bit_rate[1] = '9';
            pFormat.bit_rate[2] = '9';
        }
        TimeRateData_sav.Bitrate_sav[0] = (uint8) pFormat.bit_rate[0]; //�������ڸ����Ա�ʱ��ʾ
        TimeRateData_sav.Bitrate_sav[1] = (uint8) pFormat.bit_rate[1];
        TimeRateData_sav.Bitrate_sav[2] = (uint8) pFormat.bit_rate[2];
    }
    ResShowPic(NUM_PIC[TimeRateData_sav.Bitrate_sav[0] - 0x30], POSITION_B, 8);
    ResShowPic(NUM_PIC[TimeRateData_sav.Bitrate_sav[1] - 0x30], POSITION_B+5, 8);
    ResShowPic(NUM_PIC[TimeRateData_sav.Bitrate_sav[2] - 0x30], POSITION_B+10, 8);
}
/*
 ********************************************************************************
 *           void RefreshMainScreen( void )
 *
 * Description : ����ʾ��ˢ�º���,ֻ������Ҫʱ,�ŵ��ô˺���
 *
 * Arguments   : void
 *
 * Returns     : void
 *
 * Notes       :
 *
 ********************************************************************************
 */
void RefreshMainScreen(void)
{
    uint8 temp_buffer[9];
    uint8 strWidth;
    //region_t scrollregion,scrollregion1; 

    ui_auto_update = FALSE;

    ui_show_lock_1();
    
    SetAsciiFont(FONT_TYPE_LARGE);
    /*ˢ�±�����ͼ��*/
    if (UpDisplayFlag & up_musicbitrate)//&&(RePlayStatus == NormalPlay))//
    {
        Show_MusicType();
        Show_Bitrate();  
    }
    /*ˢ��ѭ��ģʽͼ���Ŀ¼ͼ��*/
    if ((UpDisplayFlag & up_musicloopmode) != 0)
    {
		#if(MACHINE_MODEL==1)
        if(g_music_vars.repeat_mode!=FSEL_DIR_RANDOM)
            ui_show_loop(LOOP_PIC[g_music_vars.repeat_mode]);	
        else
        {
        	ResShowPic(IM_MUSIC_REPEAT_004_FOLDER_NORMAL, 61, 0);
	        ResShowPic(IM_MUSIC_REPEAT_003_REPEATSHUFFLE, 74, 0);
        }
        #else
        if(g_music_vars.repeat_mode == FSEL_DIR_RANDOM)  	
        {
        	//ResShowPic(IM_MUSIC_REPEAT_004_FOLDER_NORMAL, 74, 0);
	       ResShowPic(IM_MUSIC_REPEAT_003_REPEATSHUFFLE, 61, 0);
        }
        else if(g_music_vars.repeat_mode == FSEL_ALL_SEQUENCE)
            ResShowPic(IM_MUSIC_REPEAT_000_NORMAL, 61, 0);	
        else if(g_music_vars.repeat_mode == FSEL_ALL_REPEATONE)
            ResShowPic(IM_MUSIC_REPEAT_001_REPEAT1, 61, 0);		
        else if(g_music_vars.repeat_mode == FSEL_ALL_REPEATALL)
            ResShowPic(IM_MUSIC_REPEAT_002_REPEATALL, 61, 0);	
        else if(g_music_vars.repeat_mode == FSEL_DIR_SEQUENCE)
        {
            ResShowPic(IM_MUSIC_REPEAT_000_NORMAL, 61, 0);
            ResShowPic(IM_MUSIC_REPEAT_004_FOLDER_NORMAL, 74, 0);
        }	
        else if(g_music_vars.repeat_mode == FSEL_DIR_REPEATALL)
        {
            ResShowPic(IM_MUSIC_REPEAT_002_REPEATALL, 61, 0);
            ResShowPic(IM_MUSIC_REPEAT_004_FOLDER_NORMAL, 74, 0);
        }	
        else if (g_music_vars.repeat_mode == FSEL_DIR_INTRO)
        {
            ResShowPic(IM_MUSIC_REPEAT_007_FOLDER_SHUFFLE, 61, 0);
            //ResShowPic(IM_MUSIC_REPEAT_004_FOLDER_NORMAL, 74, 0);
        }
        #endif
    }
    /*ˢ����Ŀ�ź�AB����״̬*/
    if ((UpDisplayFlag & up_musicsequence) != 0)
    {
        itoa4(g_musicsequence, &temp_buffer[0]);
		temp_buffer[4]=0x3a;
        itoa4(g_musicamount, &temp_buffer[5]);
        ui_show_d(&temp_buffer[0]);
        if (UpDisplayFlag & up_musicsequence)
        {
            switch (RePlayStatus)
            {
                case NormalPlay:
                break;
                case WaitSetAPointForAB:
                    ui_show_loop(IM_MUSIC_REPEATA);
                break;

                case WaitSetBPointForAB:
                    ui_show_loop(IM_MUSIC_REPEATAB);
                break;
               /* case ABLoopPlay:
                    ui_show_loop(IM_MUSIC_REPEATAB);
					
                break;*/
              /*
                case FollowRec:
                if (g_currentMusicType != Music_Wav)
                   ui_show_d_pic(MRPLMIC, 0);
                else
                   ui_show_d_pic(MRPLMIC, 1);
                break;

                case FollowAB:
                if (g_currentMusicType != Music_Wav)
                    ui_show_d_pic(MRPLFL, 0);
                else
                   ui_show_d_pic(MRPLFL, 1);
                break;

                case CompareAB:
                if (g_currentMusicType != Music_Wav)
                   ui_show_d_pic(MRPLFSPK, 0);
                else
                   ui_show_d_pic(MRPLFSPK, 1);
                break;

                case ComparePlay:
                if (g_currentMusicType != Music_Wav)
                    ui_show_d_pic(MRPLSPK, 0);
                else
                    ui_show_d_pic(MRPLSPK, 1);
                break;
            */
                default:
                break;
            } //switch(RePlayStatus)
    	}    
    }
    
    /*ˢ��EQģʽ*/
    if ((UpDisplayFlag & up_musiceqmode) != 0)
    {
        if (g_music_vars.eq_type < 8) /*Normal EQ*/
        {
            ui_show_d_pic(EQ_PIC[g_music_vars.eq_type]);
        }
        else /*SRS EQ*/
        {
            if ((g_currentMusicType == Music_MP3) || (g_currentMusicType == Music_WMA) || (g_currentMusicType
                    == Music_WMV))
            {
                if (g_music_vars.SRSModeFlag == 0)//��ʾ����ͼ��
                {
                    // ui_show_c(MEQSRS);
                }

                else
                {
                    //ui_show_c(MEQUSER);
                }
            }
            else
            {
                    //ui_show_c(EQ_PIC[0]);
            }
        }
    }
    /*ˢ�µ�ǰʱ��*/
    if ((UpDisplayFlag & up_musiccurrenttime) != 0)
    { //ע������ʱҪ�ָ��ϴβ��ŵ���ʱ��
        if ((RePlayStatus != FollowRec) && (RePlayStatus != ComparePlay))
        {
            //matt140206#2
            //ShowNowTime();
            ResShowPic(IM_MUSIC_TOTALTIME_BG,0 ,56);
            temp_buffer[0] = 0x30;
            temp_buffer[1] = 0x30;
            temp_buffer[2] = 0x30;
            temp_buffer[3] = 0x30;
            temp_buffer[4] = 0x30;
            ui_show_f(temp_buffer);
        }
    }
    if ((UpDisplayFlag & up_musictotaltime) != 0)
    {
        /*
        itoa2(TimeRateData_sav.TotalTime_sav[0], temp_buffer);
        if (g_currentMusicType != Music_Wav)
            temp_buffer[0] = ' ';
        temp_buffer[2] = ':';
        itoa2(TimeRateData_sav.TotalTime_sav[1], temp_buffer + 3);
        temp_buffer[5] = ':';
        itoa2(TimeRateData_sav.TotalTime_sav[2], &temp_buffer[6]);
        temp_buffer[8] = 0x00;
        if (g_currentMusicType != Music_Wav)
            ui_show_f(temp_buffer, 1);
        else
            ui_show_ef(temp_buffer, 1);
        */
    }
    /*ˢ�µ�ص���*/
    if ((UpDisplayFlag & up_musicbattery) != 0)
    {
        ui_show_batt(GetBattery());
		
		 switch(PlayStatus)
		 { 
		  
            case  StopSta: 
              ui_show_c(IM_MUSIC_STATUS_STOP);
			  break;
            case  PlaySta: 
              ui_show_c(IM_MUSIC_STATUS_PLAY);
			   break;
		    case  PauseSta: 
		  	  ui_show_c(IM_MUSIC_STATUS_PAUSE);
			  break;
		    case  FFPlaySta: 
		   	  ui_show_c(IM_MUSIC_STATUS_FORWARD);
			  break;
		    case  FBPlaySta: 
		  	   ui_show_c(IM_MUSIC_STATUS_BACKWARD);
			  break;
		    default:	
		  	  ui_show_c(IM_MUSIC_STATUS_STOP);
			  break;
		 }

	}
   
    /*ˢ��Ŀ¼����ID3/�ļ�����Ϣ*/
    if ((UpDisplayFlag & up_musicinformation) != 0)
    {
#if 1        
        SetAsciiFont(FONT_TYPE_LARGE);
        //matt120306#2
        if(tag_is_unicode & 0x80)
        {
            region_scroll.x = 20;
            region_scroll.y = 16;
            region_scroll.width = 108;
            region_scroll.height = 16;
            memcpy(scroll_buf, g_TagInfoBuf,92);
            ScrollString(scroll_buf, TRUE, tag_is_unicode&0x7f, &region_scroll);
            strWidth = GetStringLength(scroll_buf, 92, tag_is_unicode&0x7f);
            if(strWidth > region_scroll.width)
            {
                ScrollFlag = TRUE; 
                //strcat(scroll_buf,"  ");
            }
            else
                ScrollFlag = FALSE; 
        }
        else
        {
            region_scroll.x = 20;
            region_scroll.y = 16;
            region_scroll.width = 108;
            region_scroll.height = 16;
            memcpy(scroll_buf, g_TagInfoBuf,30);
            ScrollString(scroll_buf, TRUE, tag_is_unicode, &region_scroll);
            strWidth = GetStringLength(scroll_buf, 30, tag_is_unicode);
            if(strWidth > region_scroll.width)
            {
                ScrollFlag = TRUE; 
                //strcat(scroll_buf,"  ");
            }
            else
                ScrollFlag = FALSE; 
                
            #ifdef OPT_Sup_LRC_fun//֧�ָ����ʾ����
	        //if (!g_lyricflag)
	        #endif
		    {
            region_scroll2.x = 20;
            region_scroll2.y = 32;
            region_scroll2.width = 108;
            region_scroll2.height = 16;
            memcpy(scroll_buf2, &g_TagInfoBuf[30],30);
            ScrollString2(scroll_buf2, TRUE, tag_is_unicode, &region_scroll2);
            strWidth = GetStringLength(scroll_buf2, 30, tag_is_unicode);
            if(strWidth > region_scroll2.width)
            {
                ScrollFlag2 = TRUE;  
                //strcat(scroll_buf2,"  ");
            }
            else
                ScrollFlag2 = FALSE;
            }
        }
//        SetAsciiFont(FONT_TYPE_SMALL);

#endif       
		if (UpDisplayFlag & up_lyricflag)
        {
            if (g_lyricflag)
            {
                //ResShowPic(IM_MUSIC_ICON_LRC,88,8);		
            }
            else
            {
                //ResShowPic(IM_MUSIC_ICON_LRC_CLEAR,88,8); 		
            }
        }
    }

	UpdateScreen(NULL);
    ui_auto_update = TRUE;

    UpDisplayFlag = 0x00;
    return;
}

//-----------��ʾ��������ͼ��-----------
void Show_MusicType(void)
{
    switch (g_currentMusicType)
    {
        case Music_MP3:

        ui_show_type(IM_MUSIC_MP3);
        break;
        case Music_WMA:
        ui_show_type(IM_MUSIC_WMA);
        break;
//       case Music_WMV:
//        ui_show_type(MSC_WMA);
//        break;
        case Music_Wav:
        ui_show_type(IM_MUSIC_WAV);
        break;
        case Music_APE:
        ui_show_type(IM_MUSIC_APE);
        break;
        case Music_FLAC:
        ui_show_type(IM_MUSIC_FLAC);
        break;
        default:
        ui_show_type(IM_MUSIC_MP3);
        break;
    }

     ResShowPic(IM_MUSIC_ANI_00, 0, 16);
     ResShowPic(IM_MUSIC_ANI_01, 0, 32);
}

//-----------��ʾ������Ϣ---------------
void DisplayErrMsg(uint16 ErrMsg)
{
    ClearScreen(NULL);
    SetAsciiFont(FONT_TYPE_SMALL);
    ResShowMultiString(ErrMsg, 255, 24);
    UpdateScreen(NULL);
    while (ap_get_message() != NULL)
        ; //�����Ϣ��
    ap_sleep(4);
    SetStopInterface();
    need_draw = TRUE;
}
/*void DisplayMK(void)
{
	SetTextPos(0, 32);
	PutS("meter",-1);
	//UpdateScreen(NULL);
	SetTextPos(0, 40);
	PutS("kcal",-1);

    UpdateScreen(NULL);
}*/


 
