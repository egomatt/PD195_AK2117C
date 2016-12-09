/*
 ******************************************************************************
 *                               AS211A
 *                            Module: music
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: stop status module
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xusong    2010-07-15 15:00     1.0             build this file
 ******************************************************************************
 */
#include  "ap_music.h"

#pragma name(AP_UI_STP)
extern uint8 earphoneinflag;

uint16 ui_stop_loop(void);


//======================================================================================

uint16 ui_stop_loop(void)
{
    bool err_skip_flag = FALSE;
	uint16 scrollcount=0;
    uint8 key;
    uint8 modekeystore = NULL;
    uint8  KeyTone_bak;
    bool g_fileErrorCnt;
    uint8 pause_flag;
	//uint8  sleeptime_bak;
//    bool VOLKEY_with;
	uint8 time_to_step = 0;
//	slider_t slider;
    while (1)
    {
        /*���Զ�����ʱ������Ӧ�ȼ�*/
        ClearWatchDog();
        g_result = ap_get_message();
        if (g_result == NULL)
        {
            break;
        }
        else if (g_result == AP_MSG_STANDBY)
        {
            return RESULT_STANDBY;
        }
        else if (g_result == AP_MSG_USB_PLUGIN)
        {
            return RESULT_UDISK;
        }
        /*����������ڽ���ο��˳�����ʾ�ʹ���忨�γ������⣬2008-04-08 by Mars */
        //else if (g_result == AP_MSG_SD_OUT)
        //{
        //    if (g_music_vars.location.disk == 'H')
        //    {
        //        g_music_vars.location.disk = 'C';
        //        DisplayErrMsg(CARDPULL);//��ʾ���γ�
        //        PlayStatus=StopSta;
        //        return RESULT_CURR_UI;
        //    }
        //}
        else
        {
        }
    }
    while (1)
    {
        ClearWatchDog();
        key = ap_get_message();
        if (g_from_browser != 0)
        {
            key = (AP_KEY_PLAY | AP_KEY_UP);
            g_from_browser = FALSE;
        }
        if (need_draw!=0)
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
        
        if(!Pedo_ui_draw)
   	    {
           if(scrollcount++>2000) //matt120306#2
           {
                scrollcount = 0;
		        SetAsciiFont(FONT_TYPE_LARGE);
                if(ScrollFlag)
                    ScrollString(scroll_buf, FALSE, tag_is_unicode&0x7f, &region_scroll);
                if(ScrollFlag2)
                    ScrollString2(scroll_buf2, FALSE, tag_is_unicode&0x7f, &region_scroll2);
                UpdateScreen(NULL);
			    SetAsciiFont(FONT_TYPE_DEFAULT);
          }
   	    } 
   	    
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
    			
    			ui_show_batt(GetBattery());
                //UpDisplayFlag = UpDisplayFlag | up_musicbattery;
                //need_draw = TRUE;//���ͼ��ˢ��
                break;
                
            
            case AP_KEY_PLAY | AP_KEY_LONG:
                return RESULT_STANDBY;//�ػ�
                
            
            case AP_KEY_PLAY | AP_KEY_UP:
    			if(Pedo_ui_draw)
        			{
        				if(Pedo_work_flag)
        				{
        					sKY_SetGsensor(0,0,0,Pedo_Swim_flag);
        					Pedo_work_flag = FALSE;
        					pause_flag = TRUE;
        				}
        				else if(Swim_work_flag)
        				{
        					sKY_SetGsensor(0,0,0,Pedo_Swim_flag);
        					Swim_work_flag = FALSE;
        					pause_flag = TRUE;
        				}
        				else
        				{
        					if(Pedo_Swim_flag)
        					{
        					    Swim_work_flag = TRUE;
        					    Pedo_work_flag = FALSE;
        					    sKY_SetGsensor(0,0,0,0);
        					    sKY_SetGsensor(1,0,0,Pedo_Swim_flag);
        					    pause_flag = FALSE;
        					}
        					else
        					{
                                Pedo_work_flag = TRUE;
        					    Swim_work_flag = FALSE;
        					    sKY_SetGsensor(0,0,0,1);
        					    sKY_SetGsensor(1,0,0,Pedo_Swim_flag);
        					    pause_flag = FALSE;
        					}
        				}
        				Pedo_RefreshDataScreen(Pedo_Swim_flag, pause_flag);
                    }
    			else
        		{
        			PlayStatus = PlaySta;
                    if (g_Openparam.typeerror == 1)
                    {
                        g_fileErrorCnt++;
                        g_musicamount = FSELGetTotal();
                        if (g_fileErrorCnt >= g_musicamount)
                        {
                        DisplayErrMsg(MFMTERR);//��ʾ��ʽ��
                        SetFullInterface();
                        need_draw = TRUE;
                          PutSysMsg(AP_KEY_NULL);
                          err_skip_flag = TRUE;
                          break;
                        }
                        else
                        {
                          DisplayErrMsg(MFMTERR);//��ʾ��ʽ��
                          SetFullInterface();
                          need_draw = TRUE;
                        PutSysMsg(AP_KEY_NEXT);
                        err_skip_flag = TRUE;
                        break;
                        }
                    }
                    if (g_music_vars.MusicOpenMod == Nor_open)
                    {
                        StartIntro(TRUE);
                    }
                    else
                    {
                        StartIntro(FALSE);
                    }
                     UpDisplayFlag = UpDisplayFlag | up_musictotaltime | up_musicbitrate;
                     need_draw = TRUE;
                    if (!musicpSendCommand(MC_PLAY, (void*) (g_music_vars.MusicOpenMod)))
                    {
                        ErrReport(PlayStatus);// playStatus ��δ�ı�
                        SetFullInterface();
                        need_draw = TRUE;
                        PutSysMsg(AP_KEY_NEXT);
                        err_skip_flag = TRUE;
                        break;
                    }
                    g_music_vars.MusicOpenMod = 0;
                    OutPutVolume(g_music_vars.vol_display);
                    /*���ΰ������������˵��Զ����ر�־*/
                    KeyTone_bak = g_comval.KeyTone;
        			/* no keytone */ 
                    g_comval.KeyTone = 0;
                    return_to_play = FALSE;
                    if (g_comval.ClassDFlag!=0)
                    {
#ifndef PC
                        earphoneinflag = 1; //�����������Զ��л�cld
#endif
        		        }
        		        
        				//======== ����ʱ���ػ�
        		        // sleeptime_bak=g_comval.StandbyTime;
        				// if(g_comval.playpowerofflag)
        				//   g_comval.StandbyTime=0;
        				// ap_message_init(&g_comval);//��ʼ��STANDBY ʱ��
        				//========

        				 #if(MACHINE_MODEL == 1)
                         if(PlayStatus == PlaySta)
                            g_comval.MTPFormatType = 17;
                         else
                            g_comval.MTPFormatType = 0;
                         #endif
         
        		         g_result = ui_play();

        		         #if(MACHINE_MODEL == 1)
                         if((PlayStatus == PlaySta) && (g_result != RESULT_MAIN))
                            g_comval.MTPFormatType = 17;
                         else
                            g_comval.MTPFormatType = 0;
                         #endif
        		         
        		         //g_comval.StandbyTime=sleeptime_bak;
        				 g_comval.KeyTone = KeyTone_bak;
        				 //ap_message_init(&g_comval);

                    if (g_comval.KeyTone == 0)
                    {
#ifndef PC
                        earphoneinflag = 0;
#endif
                        DisableCLD();
                    }
                    ap_message_init(NULL);//��ʼ��STANDBY ʱ��
                    if ((g_result != 0) && (g_result != RESULT_UI_STOP))    //matt120405#1 for play stop
                    {
                        uint16 i;
                        i = g_result;
                        if (PlayStatus != StopSta)
                        {
#ifdef OPT_Sup_FadeOut_fun
                            if (!g_FCexitflag||g_currentMusicType==Music_WMA)
                            { //��Ӹ�����¼����Աȷ�������������
                                FadeOut(FALSE);
                            }
#endif
                            BackupCurrentTime();
                            musicpSendCommand(MC_STOP, NULL);
                            musicpSendCommand(MC_BACKUP, NULL);
                            g_music_vars.MusicOpenMod = Nor_Break_open;//��֤�˳���Ӷϵ����
                            if(g_result==RESULT_MUSIC_PLAY||g_result==RESULT_VOICE_PLAY)
        				    ClearTimePara();
                        }
                        g_result = i;
                    }
                    else
                    {
                        while (ap_get_message() != 0)
                        {
                        	; //�����Ϣ��
                        }  

                        if((PlayStatus != PauseSta) && (g_result == 0))    //matt120405#1 for play stop
                        {
                            SetFullInterface();
                            need_draw = TRUE;
                            PutSysMsg(AP_KEY_NEXT);
                            err_skip_flag = TRUE;
                            break;
                        }
                        else
                            g_result = 0;
                    }
                    g_FCexitflag = FALSE;
                    need_draw = TRUE;
                    UpDisplayFlag = UpDisplayFlag | up_musicbattery;
                    return g_result;
        		}
    			break;
    			

			/*case AP_KEY_AB | AP_KEY_HOLD:
			if (g_MusicType == FSEL_TYPE_MUSIC)
    		{
    			Pedo_ui_draw = 1;
    			while (ap_get_message() != 0)
	        	{
	            	;
	        	}    
				break;
			}*/	

			
			case AP_KEY_AB | AP_KEY_LONG: //����¼��
//			    if (g_MusicType == FSEL_TYPE_VOICE)
//    		    {			
        		    return RESULT_REC_NOSTART;
//			    }
//			    else
//				    break;

            /*
			case AP_KEY_AB | AP_KEY_UP:
    			if(Pedo_ui_draw)
    			{
    				sKY_SetGsensor(2,0,0);
    	            Pedo_total_sec = 0;
    		        Pedo_total_step = 0;
    		        test_up = 0;
    		        test_down = 0;
    		        need_draw = TRUE;
    			}
    			else
    			{
    				g_result = ChangeVolume();			
    				if (g_result == RESULT_UDISK)
                	{
                   		return g_result;
                	}
    				volume_callback(g_music_vars.vol_display);
    				SetFullInterface();
    				
    			}
    			need_draw = TRUE;
                break;
    			*/
    			
			
            case AP_KEY_MODE:
                modekeystore = key;//Ҫ�Ȱ���AP_KEY_MODE������up������Ч
                break;

            case AP_KEY_MODE | AP_KEY_UP: //�̰�MODE
    			if(Pedo_ui_draw)
    			{
    			    g_result = Pedo_menu(Pedo_Swim_flag);
    			    need_draw = TRUE;
    			    if (g_result == RESULT_IGNORE)
                    {       
                        g_result = RESULT_CURR_UI;
                    }
                    if (g_result == RESULT_RETURN_TO_MAIN)
                    {
                  		//sKY_SetGsensor(0,0,0);
        				//Pedo_work_flag = FALSE;
        				Pedo_ui_draw = FALSE;
        				need_draw = TRUE;
        				SetFullInterface();
        				break;
    				}
    				if(g_result!=NULL)
    			        return g_result;
    			}     
    			else
    			{
                    if (modekeystore != AP_KEY_MODE)
                    { //Ҫ�Ȱ���AP_KEY_MODE������up������Ч
                        modekeystore = NULL;
                        break;
                    }
                    musicpSendCommand(MC_CLOSE, NULL);
                 //   g_result = ui_stop_menu();
        		    g_result =ui_play_menu(FALSE);
                    SetFullInterface();
                    need_draw = TRUE;

                    if (g_result == RESULT_IGNORE)
                    {       
                        g_result = RESULT_CURR_UI;
                        //break;
                    }
                    else if (g_result == RESULT_MUSIC_PLAY||g_result == RESULT_VOICE_PLAY)
                    {
						if (g_MusicType == FSEL_TYPE_MUSIC)
                		{
                    		if(*(dir_layer_buff + 73) == 0x5a)
                    		{
                        		FS_GetOrSetDir_Exfat(dir_layer_buff, 0, EXFAT_GET_DIRLAYER);
                        VMWrite(dir_layer_buff, VM_AP_MUSIC + 0x400, 74);
                    		}
                		}
                		else
                		{
                    		if(*(dir_layer_buff + 73) == 0x5a)
                    		{
                        		FS_GetOrSetDir_Exfat(dir_layer_buff, 0, EXFAT_GET_DIRLAYER);
                        VMWrite(dir_layer_buff, VM_AP_VOICE + 0x400, 74);
                    		}
                		}
                        ClearTimePara();//��0ʱ�估�ϵ�
                        g_result = RESULT_CURR_UI;
                    }
                    else
                    {
                    }
    	            /*if(g_del_cur_flag == TRUE)
    	            {
    	                ClearTimePara();//��0ʱ�估�ϵ�
    					g_del_cur_flag = FALSE;
    	            }*/
                    PlayStatus = StopSta;
                    /*
                     if(g_result != RESULT_MAIN||g_result!=RESULT_STANDBY)
                     {

                     return RESULT_CURR_UI;
                     }*/

                    return g_result;//�ȼ������0ʱ����
    			}
    			break;

			
            case AP_KEY_PREV | AP_KEY_UP:
                if(Pedo_ui_draw)
    			{
    				//sKY_SetGsensor(0,0,0);
    				//Pedo_work_flag = FALSE;
    				Pedo_ui_draw = FALSE;
    				need_draw = TRUE;
    				SetFullInterface();
    				return_to_play = 1;
    			}    
                while (ap_get_message() != 0)
                {
                	;//do nothing
                }
                break;

            case AP_KEY_PREV:
            case AP_KEY_PREV | AP_KEY_HOLD:
			    if(Pedo_ui_draw)
                    break;
			    else
                {
                    musicpSendCommand(MC_CLOSE, NULL);
                    g_result = ChangeMusic(Music_Prev, Stop_key);
                    if (g_result == FALSE) //ѡ���ļ�����
                        return g_result;
                    GetInformation(); //ȡ�ļ���Ϣ
                    if (g_Openparam.typeerror == 0)
                    {
                        musicpSendCommand(MC_OPEN, (void*) &g_Openparam);//����ɲ���
                        g_result = musicpSendCommand(MC_LOAD, NULL);
                    }
                    SetFullInterface();
                    UpDisplayFlag = UpDisplayFlag & clr_musicBackGDPic;
                    need_draw = TRUE;
                    PlayStatus = StopSta;

                    //��������HOLD��Ϣ,��ֹ����ͣ�˻�������
                    while (ap_get_message() != 0)
                    {
                    	;
                    }                    
                    break;
                }

          
            case AP_KEY_NEXT | AP_KEY_UP:
                while (ap_get_message() != 0)
                {
                	;
                }                
                break;

            case AP_KEY_NEXT:
            case AP_KEY_NEXT | AP_KEY_HOLD:
    			if(Pedo_ui_draw)
    			{
    				sKY_SetGsensor(2,0,0,Pedo_Swim_flag);
    				if(Pedo_Swim_flag)
                    {
    	                Swim_total_sec = 0;
    		            Swim_total_step = 0;
    		        } 
    				else
    				{
    	                Pedo_total_sec = 0;
    		            Pedo_total_step = 0;
    		        }    
    		        //test_up = 0;
    		        //test_down = 0;
    		        need_draw = TRUE;
    			}
    			else
                {				
    	            musicpSendCommand(MC_CLOSE, NULL);
    	            g_result = ChangeMusic(Music_Next, Stop_key);
    	            if (g_result == FALSE) //ѡ���ļ�����
    	            {
    	                err_skip_flag = FALSE;
    	            	return g_result;
    	            }               
    	            GetInformation(); //ȡ�ļ���Ϣ
    	            if (g_Openparam.typeerror == 0)
    	            {
    	                musicpSendCommand(MC_OPEN, (void*) &g_Openparam);
    	                g_result = musicpSendCommand(MC_LOAD, NULL);
    	                //CaltimeFlag = FALSE;//�ȴ���ʱ��
    	                //WaitForCaltimeCNT = 0;
    	            }
    	            SetFullInterface();
    	            UpDisplayFlag = UpDisplayFlag & clr_musicBackGDPic;
    	            PlayStatus = StopSta;
    	            need_draw = TRUE;
    	            while (ap_get_message() != 0)
    	            {
    	            	;
    	            }  
    	            
    	            if(err_skip_flag)
    	            {
    	                err_skip_flag = FALSE;
                        PutSysMsg(AP_KEY_PLAY | AP_KEY_UP);
                    }
    	            break;
    			}
    			
			
            case AP_KEY_VOLSUB:
            case AP_KEY_VOLADD:
                if(Pedo_ui_draw)
                    break;
                else
                {
        			g_result = ChangeVolume();			
        			if (g_result == RESULT_UDISK)
                    {
                    	return g_result;
                    }
        			volume_callback(g_music_vars.vol_display);
        			SetFullInterface();
        			need_draw = TRUE;
    			}
                break;


            default:
                key = ap_handle_hotkey(key);
                if (key == RESULT_REDRAW)
                {
                    SetFullInterface();
                    need_draw = TRUE;
                }
                else if (key == RESULT_SD_OUT)
                {
                    g_music_vars.location.disk = 'C';
                    SetFullInterface();
                    need_draw = TRUE;
                    PlayStatus = StopSta;
                    return RESULT_CURR_UI;

                }
                else if (key != 0)
                {
                    return key;
                }
                break;
        } /*end of switch(key)*/

    } /*end of while(1)*/
    return 0;
}
