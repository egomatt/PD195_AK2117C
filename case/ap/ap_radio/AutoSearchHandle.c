/*
 *******************************************************************************
 *                ACTOS AP
 *        main ap of sdk 3.0
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *        History Record:
 1.ȥ��ˢ�±�����ʾ��������̨ʱ�Ľ�����˸��, by Mars,2008-04-15
 *******************************************************************************
 */

#include "ap_radio.h"

#pragma name(AP_AUTOSEARCH)

extern uint16 Freq_bak;
extern    slider_t     slider; 
extern    comval_t    F_comval;
extern uint16 FMStationTab[60];  // ǰ30����US, ��30����JP.
extern fm_play_status_t FMStatus;
extern FMRADIO_param_t radio_param;
extern uint8 Freqtab[10];
extern uint8 fmconnect[2];
extern uint8 FMFreNumOld; //�ϴ���ʾƵ�����ֵ�λ��
extern uint8    Updateflag;
extern uint8 Prs_nor;
extern uint16 FMAutoTab[30]; // ȫ�Զ���̨,Ԥ��̨.
//extern bool Need_refresh;
extern void SequenceFreqlist(uint16* buf, uint16 curfreq, uint8 totalnum);
extern void DealFadeIn(uint8 Volume);
extern void ShowFreqNum(void);
extern void ShowSignalST(void);
void SaveStation(void);

/*
 ********************************************************************************
 *             void ui_show_chdata(uint8 *buf, uint8 x, uint8 y)
 *
 * Description : ��ʾ��̨������
 *
 * Arguments   : void
 *
 * Returns     : void
 *
 * Notes       :
 *
 ********************************************************************************
 */
/*void ui_show_chdata(uint8 *buf, uint8 x, uint8 y) //zxs add for show CH number
{
    while (1)
    {
        if (*buf != 0)
        {
            ResShowPic(MNUM0 + *buf - 0x30, x, y);
        }
        else
        {
            break;
        }
        ClearWatchDog();
        buf++;
        x += (uint8) (PIC_X_CHNUM1 - PIC_X_CHNUM0);
    }
}*/	   
/*
 ********************************************************************************
 *             void ui_show_FMdata(uint8 *buf, uint8 x, uint8 y)
 *
 * Description : ��ʾ��̨Ƶ��
 *
 * Arguments   : void
 *
 * Returns     : void
 *
 * Notes       :
 *
 ********************************************************************************
 */
/*void ui_show_FMdata(uint8 *buf, uint8 x, uint8 y)
{
    uint8 FMFreNum = (uint8) strlen(buf);
    if (FMFreNum != FMFreNumOld)
    {
        ResShowPic(PIC_ID_FMNUMBG,PIC_X_FMNUMBG,PIC_Y_FMNUMBG);
        FMFreNumOld = FMFreNum;
    }
    while (1)
    {
        if (*buf != 0)
        {
            ResShowPic(FMNUM0 + *buf - 0x30, x, y);
        }
        else
        {
            break;
        }
        ClearWatchDog();
        buf++;
        x += (uint8) (PIC_X_F2RATE - PIC_X_F1RATE);
    }
}*/
/*
 ********************************************************************************
 *             void ShowStationCount(uint16 count)
 *
 * Description : ��ʾ��̨��.
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void ShowStationCount(uint8 count)
{
		uint8 buf[4]; 
    
    if(count > 0)        //��ʾ��̨��
    { 
        //*itoa_2(count,buf);
        buf[0]=count/10;
        buf[1]=count%10;
     	ResShowPic(SYSFONT_SMALL_0+buf[0],0,32);
        ResShowPic(SYSFONT_SMALL_0+buf[1],0+6,32);
		ResShowPic(SYSFONT_SMALL_SLASH,0+12,32);
		ResShowPic(SYSFONT_SMALL_0+buf[0],0+18,32);
		ResShowPic(SYSFONT_SMALL_0+buf[1],0+24,32);
    }
}
/*
 ********************************************************************************
 *             void CleanStationCount(void)
 *
 * Description : �����̨��
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void CleanStationCount(void)
{
//    ResShowPic(PIC_ID_FMCHBG,PIC_X_FMCHBG,PIC_Y_FMCHBG);
}

/*
 ********************************************************************************
 *             void ShowFreqInFM2()
 *
 * Description : ��ʾȫ�Զ���̨����
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void ShowFreqInFM2(void)
{
//    uint8 buf[4];
//    uint8 FMTab[5];
     ClearScreen(NULL);             
    if(FMStatus.stereo_status==0)        
    {                
        ResShowPic(IM_RADIO_STEREO,40,16);        //��ʾ������
    }
    else
    {
        ResShowPic(IM_RADIO_MONO,40,16);
    }
	ResShowPic(IM_RADIO_PRESET,0,16);	
	if((FMStatus.station_count>0)&&(FMStatus.station>0))        //��ʾ��̨��
    {
		ShowStationCount(FMStatus.station);

	}
	ShowFreqNum();
    ui_show_batt(GetBattery());
	   
    ShowSignalST();         
	Prs_nor = 1;
    UpdateScreen(NULL);                             //ˢ�� 
    ClearWatchDog(); //�忴�Ź�
}

/*
 ********************************************************************************
 *             bool Search_Show2(FM_direct_t Direct)
 *
 * Description : ������һ���ڵ�̨����û�е��µ�̨.
 *
 * Arguments   : Direct,Next��Last����.
 *
 * Returns     : TRUE:�ѵ���һ���µ�̨,FALSE:û���ѵ�.
 *
 * Notes       :
 *
 ********************************************************************************
 */
bool Search_Show2(FM_direct_t Direct)
{
    bool Search_Flag;
    uint16 i;
    uint16 temp;

    FMStatus.stereo_status = 1;
    FMStatus.station = 0;
//    fmSendCommand(MC_MUTE,SetMUTE);
//    fmSendCommand(MC_SETVOLUME,FMStatus.volume);
    fmSendCommand(MC_RESTORE, (uint16) & radio_param);
    Search_Flag = fmSendCommand(MC_SEARCH, Direct);
    if (Search_Flag == 1) //��̨�ɹ���MUTE�ͷ�
    {
    	fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
        fmSendCommand(MC_GETFREQ, (uint16) & temp);
        for (i = 0; i < AutoTabNum; i++) // ����Ƿ��ڵ�̨����
        {
            if (FMAutoTab[i] == temp)
            {
                FMStatus.stereo_status = 1;
                Search_Flag = FALSE;
                break;
            }
        }
//		fmSendCommand(MC_GETSTATUS,(int)&FMStatus);
        /*if ((FMStatus.mode == 1) && (FMStatus.freq > 140))
        {
            Search_Flag = FALSE;
        }*/
        ShowFreqInFM2();
    }
/*#ifdef HARDWARE_SEEK
    if(Search_Flag==1)
    {
        CleanStationCount();
        ShowFreqInFM2();
        ShowStationCount(AutoTabNum + 1);
        g_CNT=0;
        while(g_CNT==0)
        {
            ; //�ȴ�0.5��
        }
    }
#else*/
    
    if (Search_Flag == 1)
    {
        ShowStationCount(AutoTabNum + 1);
        g_CNT = 0;
        while (g_CNT < 2)
        {
            ; //�ȴ�1��
        }
        CleanStationCount();
    }
    else
    {
    	for(i=0;i<0x100;i++)
    	{
//         	ClearWatchDog(); //�忴�Ź�
    	}
	}
//#endif
    return Search_Flag;
}

/*
 ********************************************************************************
 *             bool RestartSearch(uint8 level)
 *
 * Description : ��������ֵ,���¿�ʼ����.
 *
 * Arguments   : ����ֵ
 *
 * Returns     : TRUE:��������,FALSE:�ж�����,��������.
 *
 * Notes       :
 *
 ********************************************************************************
 */
bool RestartSearch(uint8 level)
{
    if (level <= 2)
    {
        return FALSE;
    }
    //fmSendCommand(MC_OPEN, level); //����������������ֵ����ʼ��������
    fmSendCommand(MC_OPEN, level); //����������������ֵ����ʼ��������

#ifndef HARDWARE_SEEK
    fmSendCommand(MC_RESTORE, (uint16) & radio_param);
#endif
    return TRUE;
}
/*
 ********************************************************************************
 *             uint16 AutoSearchHandle(uint8 Direct)
 *
 * Description : ȫ�Զ�����
 *
 * Arguments   : Direct,Next��Last����.
 *
 * Returns     : ����п�ʶ����ȼ���Ϣ,  ���ؽ����Ϣ
 ���û�п�ʶ����ȼ���Ϣ,  ����0
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint16 AutoSearchHandle(uint8 Direct)
{
    uint8 key = 0;
    uint8 count = 0; //RTC��Ϣ��������
    bool need_draw=TRUE,need_draw_all=TRUE;
    uint8 Search_Level = SEARCHLEVEL;
    bool loop = TRUE;
    uint8 Search_Loop = 1; //��̨�Ĵ���
    uint8 temp;

//    Need_refresh = 1;
    AutoTabNum = 0; // ȫ�Զ�����,�Ӽ�ֵ��ʼ.
    Freq_bak = FMStatus.freq;
    memset(FMAutoTab, 0, 60);        //AutoSearch ���������ʼ�� //matt140724#1
    fmSendCommand(MC_BACKUP, (uint16) & radio_param);
    fmSendCommand(MC_OPEN, SEARCHLEVEL);
    if (FMStatus.mode == 0)
    {
        if (Direct == 10) // USƵ��
        {
            FMStatus.freq = FM_BAND_US_BOTTOM;
        }
        else
        {
            FMStatus.freq = FM_BAND_US_TOP;
        }
    }
    else
    { // JPƵ��
        if (Direct == 10)
        {
            FMStatus.freq = FM_BAND_JP_BOTTOM;
        }
        else
        {
            FMStatus.freq = FM_BAND_JP_TOP; //900;  //suber
        }
    }
//#ifdef HARDWARE_SEEK
//    fmSendCommand(MC_SETFREQ,FMStatus.freq);
//#endif
    temp = FMStatus.station_count;
    while (loop == TRUE)
    {
        if(need_draw_all)
        {
            ResShowPic(IM_RADIO_BG,0,0);     
            need_draw_all=FALSE;
            need_draw=TRUE;            
        }
        if(need_draw)                //��ʾ
         {
         		ShowFreqInFM2();
         		need_draw=FALSE;
         } 
        if (Direct != 0)
        {
            if (Direct == 10) //NEXT��������
            {
/*#ifdef HARDWARE_SEEK			    //Ӳ��SEEK����
                if(loop==TRUE)
                {
                    if(Search_Show2(UP))
                    {
                        SaveStation();
                        need_draw = TRUE;
                    }
                    else
                    {
                        FMStatus.freq=0;
                        Search_Level-= 3; // �����������ޣ��Բ�ͬ��������.
                        Search_Loop--;
                        if(Search_Loop==0)
                        {
                            loop = FALSE;
                            key = 0;
                        }
                        else
                        {
                            loop = RestartSearch(Search_Level);
                            if(loop)
                            {
                                fmSendCommand(MC_SETFREQ,FMStatus.freq);
                            }
                        }
                    }
                }

#else*/
                if (((FMStatus.freq + 1) > FM_BAND_US_TOP) && (FMStatus.mode == 0)) //�������
                {
                    FMStatus.freq = FM_BAND_US_BOTTOM;
                    Search_Level -= 3; // �����������ޣ��Բ�ͬ��������.
                    Search_Loop--;
                    if (Search_Loop == 0)
                    {
                        loop = FALSE;
                        key = 0;
                    }
                    else
                    {
                        loop = RestartSearch(Search_Level);
                    }
                }
                else if(((FMStatus.freq+1)>FM_BAND_JP_TOP)&&(FMStatus.mode==1))
                    {
                        FMStatus.freq=FM_BAND_JP_BOTTOM;
                    Search_Level -= 3; // �����������ޣ��Բ�ͬ��������.
                    Search_Loop--;
                    if (Search_Loop == 0)
                    {
                        loop = FALSE;
                        key = 0;
						#ifndef Greenhouse
                        fmSendCommand(MC_SETFREQ, FMStatus.freq); //suber
						#endif
                    }
                    else
                    {
                        loop = RestartSearch(Search_Level);
                    }
                }
                else
                {
                    FMStatus.freq += 1;
                    need_draw = TRUE;
                }
                if ((loop) && (Search_Show2(UP)))
                {
					SaveStation();
					DealFadeIn(FMStatus.volume);
                    need_draw = TRUE;
                }
				/*else //�����޵�̨
				{
					if (Search_Loop == 1)
					{
						loop = FALSE;
                        key = 0;
					}
					else if (Search_Loop == 2) //�ƺ������ã������޴����󣬹��Ȳ����������
					{
						if (FMStatus.mode == 0)
						{
							FMStatus.freq = FM_BAND_US_BOTTOM;
						}
						else
						{
							FMStatus.freq = FM_BAND_jp_BOTTOM;
						}
					}
				}*/
//#endif
            }
            else if (Direct == 20) //LAST��������
            {
/*#ifdef HARDWARE_SEEK						  //Ӳ��SEEK����
                if(loop==TRUE)
                {
                    if(Search_Show2(DOWN))
                    {
                        SaveStation();
                        need_draw = TRUE;
                    }
                    else
                    {
                        if(FMStatus.mode == 0)
                        {
                            FMStatus.freq=FM_BAND_US_BOTTOM;
                        }
                        else
                        {
                            FMStatus.freq=FM_BAND_jp_BOTTOM;
                        }
                        Search_Level-=3; // �����������ޣ��Բ�ͬ��������.
                        Search_Loop--;
                        if(Search_Loop==0)
                        {
                            loop = FALSE;
                            key = 0;
                        }
                        else
                        {
                            loop = RestartSearch(Search_Level);
                            if(loop)
                            {
                                fmSendCommand(MC_SETFREQ,FMStatus.freq);
                            }
                        }
                    }
                }
#else*/
                if (((FMStatus.freq-1)<FM_BAND_US_BOTTOM) && (FMStatus.mode == 0)) //�������
                {
                    FMStatus.freq = FM_BAND_US_TOP;
                    Search_Level -= 3; // �����������ޣ��Բ�ͬ��������.
                    Search_Loop--;
                    if (Search_Loop == 0)
                    {
                        loop = FALSE;
                        key = 0;
                    }
                    else
                    {
                        loop = RestartSearch(Search_Level);
                    }
                }
                else if (((FMStatus.freq-1)<FM_BAND_JP_BOTTOM) && (FMStatus.mode == 1))
                {
                    FMStatus.freq = FM_BAND_JP_TOP;
                    Search_Level -= 3; // �����������ޣ��Բ�ͬ��������.
                    Search_Loop--;
                    if (Search_Loop == 0)
                    {
                        loop = FALSE;
                        key = 0;
                    }
                    else
                    {
                        loop = RestartSearch(Search_Level);
                    }
                }
                else
                {
                    FMStatus.freq -= 1;
                    need_draw = TRUE;
                }
                if ((loop) && (Search_Show2(DOWN)))
                {									
                    SaveStation();
					DealFadeIn(FMStatus.volume);
                    need_draw = TRUE;
                }
//#endif
            }
            else
            {
                if (Direct == 50)
                {
                    Direct = 0;
                    loop = 0;
                    key = 0;
                }
            }
        }
		
         if(loop && need_draw)                //��ʾ   
         {
         		ShowFreqInFM2();
         		need_draw=FALSE;
         }  
		
        if (AutoTabNum >= 30) // ���ѵ�30����̨,�����˳�.
        {
            loop = FALSE;
            key = 0;
        }
        
        if (loop == TRUE)
        {
/*#ifdef HARDWARE_SEEK
            do
            {
                key = ap_get_message();
            }while (key == AP_MSG_RTC);
#else*/
            key = ap_get_message();
//#endif
            switch (key)
            {
                case AP_MSG_STANDBY:
                //do nothing
                break;
                
                case AP_MSG_RTC: //2HZ�жϵ�������Ļ�ػ���Ϊ�˵����ʾ����
                if(g_rtc_counter <= g_light_time - 1) 
                {
                	g_rtc_counter=0;
                }
                /*
				if(FMStatus.stereo_status==0)        
    			{                
        			ResShowPic(IM_RADIO_STEREO,40,8);        //��ʾ������
    			}
    			else
    			{
        			ResShowPic(IM_RADIO_MONO,40,8);
    			}	
                count++;
                if (count == 2)*/
                {
                    ui_show_batt(GetBattery()); //��ص�����ʾ
                    count = 0;
                }
                break;
                
                case AP_KEY_PLAY | AP_KEY_UP:
                case AP_KEY_MODE | AP_KEY_UP: // �����̨, ���ز��Ž���.
                loop = FALSE;
                key = RESULT_IGNORE;
                break;
                
                default: //�ȼ�����
                key = ap_handle_hotkey(key);
                if (key == RESULT_REDRAW)
                {
                      need_draw_all=TRUE;
                    	need_draw = TRUE;
//                    	Need_refresh=1;	//matt for FM hold display
                }
                else
                {
                    if (key != 0)
                    {
                        loop = FALSE;
                    }
                }
				break;
            } // end switch(key)
        } // end if(loop)
		ClearWatchDog(); //�忴�Ź�
    } // end while(loop)

    
    if (AutoTabNum > 0)
    {

        memcpy(radio_param.param1, FMAutoTab, AutoTabNum * 2);
        if (temp < AutoTabNum) // �������е�̨
        {
            FMStatus.station_count = AutoTabNum;
        }
        else
        {
            if (key == 0)
            {
                FMStatus.station_count = AutoTabNum;
            }
            if (key == RESULT_IGNORE) // ���ǲ��ֵ�̨
            {
                if (temp != 0)
                {
                    FMStatus.station_count = temp;
                }
            }
        }
        FMStatus.freq = FMAutoTab[0];
        FMStatus.station = 0;
        fmSendCommand(MC_RESTORE, (uint16) & radio_param);
    }

    if (key == RESULT_IGNORE)
    {
        key = 0;
    }

    if (FMStatus.station_count > 0x00) // û���ѵ���̨�˳������.   //matt140724#1
    {
        fmSendCommand(MC_STEP, UP);
        for (temp = 0; temp < 0xff; temp++) // �ӳ�
        {
        }
        fmSendCommand(MC_BACKUP, (uint16) & radio_param);
    }/*
	else
	{
		FMStatus.freq = Freq_bak;
        fmSendCommand(MC_SETFREQ, FMStatus.freq); //�����ζ��Ѳ���̨,��Ƶ�����õ�֮ǰ��ʼ������Ƶ��
        fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
        //ShowFreqInFM2();
        Updateflag = 1;
	}*/
	
    return key;
}
/*
 ********************************************************************************
 *             void SaveStation(void)
 *
 * Description : �����������ĵ�̨
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void SaveStation(void)
{
    uint8 i;
    uint16 k;
    fmSendCommand(MC_GETFREQ, (uint16) & k); // �����̨
    //FMAutoTab[AutoTabNum] = k;
    AutoTabNum++;
    SequenceFreqlist(FMAutoTab, k, AutoTabNum);	
    for (i = AutoTabNum; i < 30; i++)
    {
        if (radio_param.param1[i] == k) // ɾ���ظ��ĵ�̨.
        {
            radio_param.param1[i] = 0;
        }
    }

}

