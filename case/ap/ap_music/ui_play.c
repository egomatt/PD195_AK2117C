/*
 ******************************************************************************
 *                               AS211A
 *                            Module: music
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: play status module
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xusong    2010-07-15 15:00     1.0             build this file
 ******************************************************************************
 */
#include "ap_music.h"
#define IntCntValDef 10

extern uint16 Pedo_DealKeyForPlayStatus(uint8 key);   //pedometer

//extern bool g_light_flag;
extern uint8 VolChangeFlag;

#pragma name(AP_UI_PLY)

/*
 ********************************************************************************
 *             uint16 ui_play( void )
 *
 * Description : ��ʾʱ�䣬����,AB��
 *
 * Arguments   : void
 *
 * Returns     : void
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint16 ui_play(void)
{
    uint8 key,time_to_step = 0;
	//uint8 time_wate;
	uint16 scrollcount=0;
    music_play_status_t status_buf;
    PlayStatus = PlaySta;
    RePlayStatus = NormalPlay;/* ����״̬ */
    need_draw = TRUE;/* ��Ҫ��screen */
    SetStopToPlayInterfaceForPlay();
#ifdef Show_Album_picture
    pic_order=0;
#endif
    while (1)
    {
        if (need_draw != 0)
        {
            if(Pedo_ui_draw == 0)
                RefreshMainScreen();
			else
			{
			    if(Swim_work_flag || Pedo_work_flag)
			    {
				    Pedo_RefreshMainScreen(Pedo_Swim_flag, 0);
				}
				else
			    {
				    Pedo_RefreshMainScreen(Pedo_Swim_flag, 1);
				}
			}
            need_draw = FALSE;
        }    

        #ifdef OPT_Sup_LRC_fun//֧�ָ����ʾ����
	    if ((g_lyricflag) && (Pedo_ui_draw == 0))
		{
		    
			g_result = ui_lyric();
			
			//   VMRead(& g_comval, VM_SYSTEM, sizeof(g_comval));
			if( g_result != 0 ) //������ȼ�,��Ҫ����.
			{
			    return g_result;
			}
		}
        #endif/* OPT_Sup_LRC_fun */
        
        /*
        if((ScrollFlag) && (Pedo_ui_draw==0))
   	    {
           if(scrollcount++>20) //matt120306#2
           {
             scrollcount = 0;
		     SetAsciiFont(FONT_TYPE_LARGE);
		     ScrollString(scroll_buf, FALSE, tag_is_unicode, &region_scroll);
		     ScrollString2(scroll_buf2, FALSE, tag_is_unicode,&region_scroll2);
             UpdateScreen(NULL);
			 SetAsciiFont(FONT_TYPE_DEFAULT);
           }
   	    } 
   	    */
        key = 1;
        /*���������˸߱����ʺʹ��ļ�ʱ����up����ʧ������*/
        do
        {
            key = ap_get_message(); //����ֵ
            if (key == AP_MSG_USB_PLUGIN)
            {
                break;
            }
            //�������״̬�������̧���˻ص��������ţ�������ϼ�
            if ((key & AP_KEY_UP) != 0)
            {
                if (PlayStatus == FBPlaySta)
                {
                    key = AP_KEY_PREV | AP_KEY_UP;
                }
                else if (PlayStatus == FFPlaySta)
                {
                    key = AP_KEY_NEXT | AP_KEY_UP;
                }
                else
                {
                }

            }
        } while (((PlayStatus == FFPlaySta) || (PlayStatus == FBPlaySta)) && (key != 0) && (key != (AP_KEY_PREV
                | AP_KEY_UP)) && (key != (AP_KEY_NEXT | AP_KEY_UP)));

        if(earphone_in()==1)  //liuf 110323
      	{
      	    earphone_on(TRUE);
      	}	
      	else
      	{
      	    earphone_on(FALSE);
      	}
      	/*
      	if(check_5v())
        {
            g_light_flag = FALSE;
        	return RESULT_UDISK;
        }
        */
        
        switch (key)
        {
            case AP_MSG_RTC:
			if(Pedo_ui_draw)
			{
			            if((Pedo_work_flag)||(Swim_work_flag))
    			        {
        			        Pedo_ui_num++;
        					if(Pedo_ui_num == 9)		
        					{			
        						Pedo_ui_num = 0;	
        					}
        					Pedo_RefreshDataScreen(Pedo_Swim_flag, 0);
			            }
			}
			break;

            case AP_MSG_WAIT_TIMEOUT:
            case AP_KEY_PLAY://������PLAY��ʱ�Ȳ�����,�ɼ���ֹͣʱ������
            case AP_KEY_NEXT://������NEXT��ʱ�Ȳ�����,�ɼ���ѡ��һ��ʱ������
            case AP_KEY_PREV://������PREV��ʱ�Ȳ�����,�ɼ���ѡ��һ��ʱ������
            key = ap_handle_hotkey(key);
            if (key == RESULT_REDRAW)
            {
                SetFullInterface();
                need_draw = TRUE; //liuf
            }
            else if (key != 0)
            {
                return key;
            }
            else
            {            	
            }
            break;
            
            case AP_KEY_NULL://û����
            case AP_KEY_MODE:
            break;

            default:
            //����������������,ֻ�ж�Ӧ�ļ����˺�Ž��루�˺����ڱ��BANK��
			if(Pedo_ui_draw)
				key = Pedo_DealKeyForPlayStatus(key);
			else
                key = DealKeyForPlayStatus(key);
            if (key != NULL)
            {
                g_result = DealKeyResult(key);
                if (g_result == RESULT_UI_STOP)
                {
                    return RESULT_UI_STOP;    //NULL    //matt120405#1 for play stop
                }
                else if (g_result != NULL)
                {
                    return g_result;
                }
                else
                {
                }

            }
            break;
        }

        musicpSendCommand(MC_GETSTATUS, (void*) (&status_buf));
        DealForABGap(&status_buf);
        if (status_buf.status == PLAYING_ERROR)
        {
						//sKY_SetGsensor(0,0,0);       //pedometer
						SetFullInterface();
						Pedo_ui_draw = FALSE;
						need_draw = TRUE; //liuf
            Deal_Error_Playing();
            return NULL;
        }
        else if (status_buf.status == PLAYING_REACH_END)
        {
						//savenum = 0;						
            /*���ŵ��ļ�βʱ������ˢ��һ�µ�ǰʱ��ͽ�����*/ 
            if (!return_to_play)
            {
           		if(!Pedo_ui_draw)
				      ShowNowTime();
            }
            g_result = DealMusicFileEnd(0);
            if (g_result != NULL)
            {
                return g_result;
            }
        }
        //ע�� ������Ϊ���һ��������������������⴦����һ������¿ɲ��ֵ�ͷ����β
        //     ��־��ͳһ�� PLAYING_REACH_END ��־�Ϳ����ˣ���Ϊ��ͨ�� PlayStatus
        //     �����־�ж��ǿ�����ǿ��ˣ��Ϳ�֪���ǵ����ļ�ͷ�����ļ�β���������
        //     ���˵���ൽͷʱֹͣ���ˣ���ʱ�ѷ�ֹͣ���������ģ����ڽ������
        //     ���������ֹͣ����Ҫ��һ��ʱ���������ʱ����е��ļ�ͷ���绹��
        //     �� PLAYING_REACH_END ��־�Ǿͷֲ����ǵ�ͷ���ǵ�β������Ҫ��һ����־
        //     PLAYING_REACH_HEAD ��ʾ�ǵ�ͷ��
        else if (status_buf.status == PLAYING_REACH_HEAD)
        {
            //savenum = 0;
            if (PlayStatus == PlaySta)
            {
                DisInFBHead(0);//��ͷ��ʾ
                SelectCurMusic();
            }
            else
            {
                g_result = DealMusicFileEnd(0); //���������ˢ�²���ȫ������
                ClearTimePara();
                if (g_result != NULL)
                {
                    return g_result;
                }
            }
        }
        else
        {
        }

        if (RePlayStatus == WaitSetBPointForAB)
        {
            if (g_comval.ReplayMode != 0)
            {
                if ((status_buf.signal == SIGNAL_REACH_V) && (g_ABMinTimeCNT == 0))//Ҫ����AB��Сʱ��
                {
                    //if (musicpSendCommand(MC_SETA_B, SetBPoint) != 0) //set_v
                    {
                        //g_AVSetFlag = TRUE;//��־����AV�ķ�ʽ����B���
                    }
                }
            }
        }
        /*�Ӳ˵�����ʱ����ˢ�±�������ˢ�¹�����Ϣ*/
        if (return_to_play == 0)
        {
            //matt140206#2
            SetPLL(PLL_80MHZ);//matt140206#2
            g_result = (uint16) RealDisplay();
            SetPLL(DEFAULT_FREQ);//matt140206#2
        }
        else
        {
            SetFullInterface();
            need_draw = TRUE;
            return_to_play = 0;
        }
        if (g_result == RESULT_UI_STOP)
        {
            return g_result;    //0    //matt120405#1 for play stop
        }
    }/* end of while(1) */
    return 0;

}
 
/*
 ********************************************************************************
 *             void RealDisplay( void )
 *
 * Description : ��ʾʱ�䣬����,AB��
 *
 * Arguments   : void
 *
 * Returns     : int
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint16 RealDisplay(void)
{
//    uint8 RANDOM_DATA;
//    region_t region_Scroll;
     
    BackupCurrentTime();
    if (g_music_vars.BreakPTDisTime.second != g_OldSecTempBackup)
    {
        g_OldSecTempBackup = g_music_vars.BreakPTDisTime.second;
        if (g_music_vars.repeat_mode == Intro)
        {
            g_introCNT++;
            if ((g_introCntMode && (g_music_vars.BreakPTDisTime.second > IntCntValDef)) || (g_introCNT > IntCntValDef))

            { //��ͷ��ʼ�����
                if ((PlayStatus != FFPlaySta) && (PlayStatus != FBPlaySta))
                {
#ifdef OPT_Sup_FadeOut_fun
                    FadeOut(FALSE);
#endif
                }
                return DealMusicFileEnd(0);
            }
        }

        //matt120315#3
        /*�����ƿ���ʱ���ˢ��*/
        if (!g_light_flag)
        {
            return 0;
        } 

        if (RePlayStatus == NormalPlay)
        {
		    if(!Pedo_ui_draw)
                Show_Bitrate();
        }
		if(!Pedo_ui_draw)
            ShowNowTime();
         
    }

    //matt120315#3
    /*�����ƿ���ʱ���ˢ��*/
    if (!g_light_flag)
    {
        return 0;
    } 
    
    //matt120306#2
	if(!Pedo_ui_draw)
	{
        //����ʱ�Ĵ���
        if ((g_ScrollFlag) && (PlayStatus == PlaySta))
        {
            g_ScrollFlag = FALSE;
            SetAsciiFont(FONT_TYPE_LARGE);
            if(ScrollFlag)
                ScrollString(scroll_buf, FALSE, tag_is_unicode&0x7f, &region_scroll);
            if(ScrollFlag2)
                ScrollString2(scroll_buf2, FALSE, tag_is_unicode&0x7f, &region_scroll2);
            UpdateScreen(NULL);
			SetAsciiFont(FONT_TYPE_DEFAULT);
			 /*
            SetAsciiFont(FONT_TYPE_LARGE);
    		region_Scroll.x = 0;
            region_Scroll.y = 24;
            region_Scroll.width = 96;
            region_Scroll.height = 16;
            memcpy(scroll_buf,g_TagInfoBuf,92);//by cailizhen
            ScrollString(scroll_buf, FALSE,tag_is_unicode,&region_Scroll);
            SetAsciiFont(FONT_TYPE_SMALL);
            */
            if ((PlayStatus != FBPlaySta) && (PlayStatus != FFPlaySta))
            {

                Show_Bitrate();
            }
            ui_show_batt(GetBattery());
            //UpdateScreen(NULL);//gongee 2004-3-18 8:48

        }
	}
	
    if (g_2HZFlag && (PlayStatus == PlaySta) && (g_currentMusicType != Music_Wav))
    {
      //  ResShowPic( ((uint16)rand())%16 + IMAGE_05, 60, 48); //��ʾ������Ƶ��
    }
    //����A����B��ʱ�Ĵ���
    if (RePlayStatus == WaitSetAPointForAB)
    {
       /* if (g_2HZFlag)
        {
     //       if (g_currentMusicType != Music_Wav)
        //        ui_show_d_pic(MRPLAB, 0);
       //     else
       //         ui_show_d_pic(MRPLAB, 1);
        }
        else
        {
        //    if (g_currentMusicType != Music_Wav)
         //       ui_show_d_pic(MRPLNOA, 0);
        //    else
          //      ui_show_d_pic(MRPLNOA, 1);
        }*/
        ui_show_loop(IM_MUSIC_REPEATA);
    }
    else if (RePlayStatus == WaitSetBPointForAB)
    {
       /* if (g_2HZFlag)
        {
      //      if (g_currentMusicType != Music_Wav)
         //       ui_show_d_pic(MRPLAB, 0);
         //   else
        //        ui_show_d_pic(MRPLAB, 1);
        }
        else
        {
          //  if (g_currentMusicType != Music_Wav)
          //      ui_show_d_pic(MRPLNOB, 0);
         //   else
          //      ui_show_d_pic(MRPLNOB, 1);
        }  //
        */
        ui_show_loop(IM_MUSIC_REPEATAB);
    }
	else if (RePlayStatus == ABLoopPlay)
	{
		if(g_2HZFlag)
		{
			ui_show_loop(IM_MUSIC_REPEATAB);
		}
		else
		{
			ui_show_loop(IM_MUSIC_PLAYTIME_BG);
//			ResShowPic(IM_MUSIC_PLAYTIME_BG, 59, 16);
		}
	}
	return NULL;
}
/*
 ********************************************************************************
 *             bool musicpSendCommand( uint8 cmd, uint16 param )
 *
 * Description : ���ݲ�ͬ���ļ�������ѡ�񷢵������MODULE��
 *
 * Arguments   : cmd ���� param ����
 *
 * Returns     : success/fail
 *
 * Notes       :
 *
 ********************************************************************************
 */
bool musicpSendCommand(uint8 cmd, void* param)
{
    switch (cmd)
    {
        case MC_GETTIME:
        case MC_GETSAPLE:
        case MC_GETSTATUS:
        case MC_GETAUDIOFMT:
        case MC_FADEINOUT:
        return wavpSendCommand(cmd, param);
        default:
        return musicpSendCommand_bank(cmd, param);
    }
    return FALSE;
}

/*
 ********************************************************************************
 *             void Show_Bitrate( void )
 *
 * Description : ��ʾ������
 *
 * Arguments   : void
 *
 * Returns     : void
 *
 * Notes       :
 *
 ********************************************************************************
 */
void BackupCurrentTime(void)
{
    Full_time_t time_buf;
    if (musicpSendCommand(MC_GETTIME, (void*) &time_buf) != 0)
    {
        g_music_vars.BreakPTDisTime.hour = time_buf.TimeHour;
        g_music_vars.BreakPTDisTime.minute = time_buf.TimeMinute;
        g_music_vars.BreakPTDisTime.second = time_buf.TimeSecond;
    }
}

#if (0)
uint16 time_ResShowPic(uint16 id, uint8 x, uint8 y)
{
    res_infor_t  res_infor;
    SD_FSeek(res_fp, SEEK_SET, pic_entry_offset+(long)id * RESHEADITEM);
    SD_FRead(res_fp, &res_infor, sizeof(res_infor_t));
    /*�ж�res���͵ĺϷ���*/
    if(res_infor.bType != 0x02) 
    {
        return 0; 
    }  
    /*�贰��������*/
    res_region.x = x;
    res_region.y = y; 
    res_region.width = (char)res_infor.wWidth;
    res_region.height = (char)res_infor.wHeight; 
    /*��ȡͼƬ���ݲ�ˢ��*/
    PutSDImage(&res_region, res_fp->startaddr + res_infor.dwOffset );                    
   
    return 1;
}
#endif

/*��ʾ��ǰ����ʱ��ͽ�������Ϣ*/
void ShowNowTime(void)
{
    uint32 nCnt;
    uint16 i,num;
    res_infor_t  res_infor1,res_infor2; //matt140206#2

/*
	music_play_status_t status_buf;
	if (ifSelBuffToltalTimeFlag || (!CaltimeFlag))     
	{         //�� buffer �����ʱ������         
		status_buf.total_time.hour = TimeRateData_sav.TotalTime_sav[0];         
		status_buf.total_time.minute = TimeRateData_sav.TotalTime_sav[1];         
		status_buf.total_time.second = TimeRateData_sav.TotalTime_sav[2];     
	}     
	else     
	{         
		musicpSendCommand(MC_GETSTATUS, (void*)(& status_buf));         
		//�ݴ����ڵ���ʱ�����ݵ� buffer         
		TimeRateData_sav.TotalTime_sav[0] = status_buf.total_time.hour;         
		TimeRateData_sav.TotalTime_sav[1] = status_buf.total_time.minute;         
		TimeRateData_sav.TotalTime_sav[2] = status_buf.total_time.second;         
		ifSelBuffToltalTimeFlag = 1;     
	}
	*/
/*	itoa1(g_music_vars.BreakPTDisTime.hour,&temp_buffer[0]);
	temp_buffer[1] = ':';
	itoa2(g_music_vars.BreakPTDisTime.minute, &temp_buffer[2]);
    temp_buffer[4] = ':';
    itoa2(g_music_vars.BreakPTDisTime.second, &temp_buffer[5]);
    temp_buffer[7] = 0x00;*/
    
    itoa_1(g_music_vars.BreakPTDisTime.hour, temp_buffer);     
	itoa_2(g_music_vars.BreakPTDisTime.minute,  & temp_buffer[1]);     
	itoa_2(g_music_vars.BreakPTDisTime.second, & temp_buffer[3]);
    ui_show_f(temp_buffer);

    //itoa_1(TimeRateData_sav.TotalTime_sav[0], temp_buffer);       
	//itoa_2(TimeRateData_sav.TotalTime_sav[1],  & temp_buffer[1]);       
    //itoa_2(TimeRateData_sav.TotalTime_sav[2], & temp_buffer[3]);       
    //ui_show_ef(temp_buffer);

   
    //matt140206#2
    SetPLL(PLL_80MHZ);
    ui_auto_update = FALSE;
    if((TimeRateData_sav.TotalTime_sav[0]==0) && (TimeRateData_sav.TotalTime_sav[1]==0) && (TimeRateData_sav.TotalTime_sav[2]==0))
        nCnt = 0;
    else
        nCnt =((uint32)126*((uint32)g_music_vars.BreakPTDisTime.hour*3600 + g_music_vars.BreakPTDisTime.minute*60 + g_music_vars.BreakPTDisTime.second))
            / ((uint32)TimeRateData_sav.TotalTime_sav[0]*3600 + TimeRateData_sav.TotalTime_sav[1]*60 + TimeRateData_sav.TotalTime_sav[2]);
    num=126;
    ResShowPic(IM_MUSIC_TOTALTIME_BG,0 ,56);

    SD_FSeek(res_fp, SEEK_SET, pic_entry_offset+(long)IM_SBRA * RESHEADITEM);
    SD_FRead(res_fp, &res_infor1, sizeof(res_infor_t));
    SD_FSeek(res_fp, SEEK_SET, pic_entry_offset+(long)IM_SBAR_CLEAR * RESHEADITEM);
    SD_FRead(res_fp, &res_infor2, sizeof(res_infor_t));
    
    /*�ж�res���͵ĺϷ���*/
    if(res_infor1.bType == 0x02) 
    {
        /*�贰��������*/
        res_region.y = 56; 
        res_region.width = (char)res_infor1.wWidth;
        res_region.height = (char)res_infor1.wHeight; 
        for(i = 0; i < nCnt; i++)
        {
            res_region.x = 1+i;
            /*��ȡͼƬ���ݲ�ˢ��*/
            PutSDImage(&res_region, res_fp->startaddr + res_infor1.dwOffset );  
        }  
    }  

    /*�ж�res���͵ĺϷ���*/
    if(res_infor2.bType == 0x02) 
    {
        /*�贰��������*/
        res_region.y = 56; 
        res_region.width = (char)res_infor2.wWidth;
        res_region.height = (char)res_infor2.wHeight; 
        for(i = nCnt; i < num; i++)
        {
            res_region.x = 1+i;
            /*��ȡͼƬ���ݲ�ˢ��*/
            PutSDImage(&res_region, res_fp->startaddr + res_infor2.dwOffset );  
        }  
    }  
    
    /*    
    for(i = 0; i < nCnt; i++)
    {
        time_ResShowPic(IM_SBRA,1 + i ,56);
    }   
    for(i = nCnt; i < num; i++)
    {
        time_ResShowPic(IM_SBAR_CLEAR,1 + i,56);
    }
    */
    
    res_region.x = 0;
    res_region.y = 56; 
    res_region.width = 128;
    res_region.height = 8; 
    UpdateScreen(&res_region);
    ui_auto_update = TRUE;
    SetPLL(DEFAULT_FREQ);
    
/*
	itoa_1(status_buf.total_time.hour, temp_buffer);       
	itoa_2(status_buf.total_time.minute,  & temp_buffer[1]);       
    itoa_2(status_buf.total_time.second, & temp_buffer[3]);       
    ui_show_ef(temp_buffer);
*/
}


//===================[����AB����״̬]====================
void DealForABGap(music_play_status_t *status)
{
    if (status->signal == SIGNAL_REACH_B)
    {
        g_ReplayTimeCNT++;
        g_2HZ_CNT = 0;

        if (g_ReplayTimeCNT < g_music_vars.replaytimes)
        {
            musicpSendCommand(MC_SETA_B, (void*) StopBPlay);
        }
        else
        {

            musicpSendCommand(MC_SETA_B, (void*) ABFinished);
            musicpSendCommand(MC_SETA_B, ClrABPoint);
            RePlayStatus = WaitSetAPointForAB;
        }
    }

    if (status->status == PLAYING_WAIT_A)
    {
        if (g_music_vars.replaygap * 2 >= g_2HZ_CNT) //PAUSEֵ
        {
            return;
        }
        musicpSendCommand(MC_SETA_B, (void*) ReturnApoint);
        RePlayStatus = ABLoopPlay;
    }

}


uint16 DealKeyResult(uint8 key)
{
    uint16 result;
    if (RESULT_SD_OUT == g_result)
    {
        g_music_vars.location.disk = 'C';
        return RESULT_UI_STOP2;
    }
    else if ((key == RESULT_UI_STOP) || (key == RESULT_LIST)) //test
    {
        if (g_Openparam.typeerror != 0)
        {
            musicpSendCommand(MC_CLOSE, NULL);
        }
        return RESULT_UI_STOP;
    }

    else if (((key == RESULT_IGNORE)||(key == RESULT_AUTO_BACK))!=0)
    {
        SetFullInterface();
        if (PlayStatus != PlaySta)
        {
            return RESULT_UI_STOP;
        }
        return NULL;
    }
    else if (key == RESULT_MAIN)
    {
        result = (int16) key;
        return result;
    }
    else
    {
        return key;
    }
}

