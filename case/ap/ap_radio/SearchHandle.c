/*
 *******************************************************************************
 *                ACTOS AP
 *        main ap of sdk 3.0
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *       History Record:
 *         1.����Զ���̨ʱ������˸�����⣬by Mars��2008-04-15
 *******************************************************************************
 */

#include "ap_radio.h"

#pragma name(AP_SEARCH)

uint8 SearchCount = 0; //��¼�ֶ���̨�Ĵ���
uint8 StationFlag = 1; //�Ƿ���������Ƶ�η�Χ�ڵĵ�̨,1Ϊ��,0Ϊ��
uint16 Freq_bak; //��ʼ����Ƶ��ı���ֵfm_play_status_t FMStatus; //��ǰ��̨��Ϣ,8 bytes
extern fm_play_status_t FMStatusUS; //US��̨��Ϣ�ݴ�,8 bytes
extern fm_play_status_t FMStatusJP; //Japan��̨��Ϣ�ݴ�,8bytes
extern fm_play_status_t FMStatus;

extern FMRADIO_param_t radio_param;
extern uint8 Freqtab[10];
extern uint8 Updateflag;
extern uint8 FMFreNumOld; //�ϴ���ʾƵ�����ֵ�λ��
extern fm_play_status_t m_FMPlaystatus;
extern BOOL fm_VolOpFlag; 
extern uint8 Prs_nor;
extern uint8 fmconnect[2];
extern slider_t slider;
extern uint8 QND_GetRSSI(); 
extern uint16 FMStationTab[60];
extern uint8 fmdataflag[3];
extern uint8 nopresetflag;
void ShowFreqInFM(void); //FM��������ʾ
bool Search_Show(FM_direct_t Direct); //�Զ���̨ʱ����ʾ
const uint8 nopreset[]="No Preset";
/*
 ********************************************************************************
 *             void ui_show_volfm(uint8 volume)
 *
 * Description : ��ʾ������Ϣ
 *
 * Arguments   :    ��ǰ����ֵ
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void read_var(void)
{
	uint16 k;
    uint8 buf[4] = { "FM" };

	if (memcmp(fmdataflag, buf, 2) != 0)
    {
        for (k = 0; k < 60; k++)    //matt140724#1
        {
            FMStationTab[k] = 0;
        }
        memcpy(fmdataflag, buf, 3); //FM BUFFER��־����ֵ
        //US��JP�ݴ�������ֵ
        FMStatusUS.freq=FM_BAND_US_BOTTOM;
        FMStatusUS.mode = 0;
		
		#ifdef Greenhouse
		FMStatusUS.vol_display = g_comval.Volume_G;
        FMStatusJP.vol_display = g_comval.Volume_G;
        FMStatusUS.volume = g_comval.Volume_G;
        FMStatusJP.volume = g_comval.Volume_G;
		#else
        FMStatusUS.vol_display = g_comval.VolumeDef;
        FMStatusJP.vol_display = g_comval.VolumeDef;
        if (g_comval.VolumeMax != 0)
        {
            FMStatusUS.volume =  (FMStatusUS.vol_display * MAXVOLUME) / g_comval.VolumeMax;
            FMStatusJP.volume =  (FMStatusJP.vol_display * MAXVOLUME) / g_comval.VolumeMax;
        }
        else
        {
            FMStatusUS.volume = FMStatusUS.vol_display;
            FMStatusJP.volume = FMStatusJP.vol_display;
        }
		#endif
		
        FMStatusUS.station = 0;
        FMStatusUS.station_count = 0;
        FMStatusUS.stereo_status = 1;
        FMStatusJP.freq=FM_BAND_JP_BOTTOM;
        FMStatusJP.mode = 1;
        FMStatusJP.station = 0;
        FMStatusJP.station_count = 0;
        FMStatusJP.stereo_status = 1;

        #if(FM_BAND==1)
            memcpy(&FMStatus, &FMStatusUS, sizeof(FMStatus));
        #else
            memcpy(&FMStatus, &FMStatusJP, sizeof(FMStatus));
        #endif
        
        // sfr_bak = SFR_BANK;
        // SFR_BANK = BANK_AUIP;
        //DAC_ANALOG |= 0xc0; //ʹ��DAC��PA�����ó�ʼ����Ϊ24
        // DAC_ANALOG |= 0x18;
        // SFR_BANK = sfr_bak;
    }
}

/*void ui_show_volfm(uint8 volume) //��ʾ������Ϣ      zxs add for show fm vol
{
//     uint8 temp[3];
   	 uint16 i,nCnt;
//   itoa_2(volume, temp);   //matt for volume
//   ResShowPic(PIC_ID_VOL1+temp[0]-0x30,PIC_X_VOL1,PIC_Y_VOL1);
//   ResShowPic(PIC_ID_VOL1+temp[1]-0x30,PIC_X_VOL2,PIC_Y_VOL2);

	 nCnt =((uint16)volume*34)/g_comval.VolumeMax;
   
   for(i = 0; i < nCnt; i++)
   {
       ResShowPic(MUVOLFG,PIC_X_F_VOLFG + i,PIC_Y_F_VOLFG);
   }
	 for(i = nCnt; i < 34; i++)
   {
       ResShowPic(MUVOLBG,PIC_X_F_VOLFG + i,PIC_Y_F_VOLFG);
   }
}*/
/*
 ********************************************************************************
 *             uint16 SearchHandle(uint8 Direct)
 *
 * Description : �ֶ���̨
 *
 * Arguments   :    Direct,Next��Last����.
 *
 * Returns     : 	����п�ʶ����ȼ���Ϣ,  ���ؽ����Ϣ
 ���û�п�ʶ����ȼ���Ϣ,  ����0
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint16 SearchHandle(uint8 Direct)
{
    uint8 key;
    uint8 count = 0; //RTC��Ϣ����
    bool need_draw=TRUE,need_draw_all=FALSE;
    uint8 Searchflag = 1;
    StationFlag = 1;
    //SearchCount = 0; //Ӳ����̨����
    Freq_bak = FMStatus.freq;
    //fmSendCommand(MC_OPEN, SEARCHLEVEL);
    while (1)
    {
        if (Direct != 0)
        {
            if (Direct == 10) //NEXT��������
            {
                if ((FMStatus.freq + 1 > FM_BAND_US_TOP) && (FMStatus.mode == 0))
                {
                    FMStatus.freq = FM_BAND_US_BOTTOM;
                }
                else if ((FMStatus.freq + 1  > FM_BAND_JP_TOP/*900*/) && (FMStatus.mode == 1))  //suber
                {
                    FMStatus.freq = FM_BAND_JP_BOTTOM;
                }
                else
                {
                    FMStatus.freq += 1;
                }
				if(FMStatus.freq == Freq_bak)
                {
                    fmSendCommand(MC_SETFREQ, FMStatus.freq); 
                    if (FMStatus.volume != 0)
                    {
					    fmSendCommand(MC_MUTE,releaseMUTE);
                    }
                    fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
					ShowFreqInFM();
                    return 0;    // searched total band & no station searched
                }
                if (Search_Show(UP) == 1)
                {
                    return 0;
                }
                else
                    ShowFreqInFM();
            }
            else if (Direct == 20) //LAST��������
            {
				if ((FMStatus.freq - 1  < FM_BAND_US_BOTTOM) && (FMStatus.mode == 0))
                {
                    FMStatus.freq = FM_BAND_US_TOP;
                }
                else if ((FMStatus.freq - 1  < FM_BAND_JP_BOTTOM) && (FMStatus.mode == 1))
                {
                    FMStatus.freq = FM_BAND_JP_TOP; //900;  //suber
                }
                else
                    {
                   FMStatus.freq -= 1; 
                }
                if(FMStatus.freq == Freq_bak)
                {
                    fmSendCommand(MC_SETFREQ, FMStatus.freq); //�����ζ��Ѳ���̨,��Ƶ�����õ�֮ǰ��ʼ������Ƶ��
                    if (FMStatus.volume != 0)
                    {
                        fmSendCommand(MC_MUTE, releaseMUTE);
                    }
                    fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
					ShowFreqInFM();
                    return 0;    // searched total band & no station searched
                }
                if (Search_Show(DOWN) == 1)
                {
                    return 0;
                }
                else
                    ShowFreqInFM();
            }
            else
            {
                if (Direct == 50)
                {
                    Direct = 0;
                    return 0;
                }
            }
        }  
        
        /*if(need_draw)                //��ʾ
        {          
            ShowFreqInFM();                        
            need_draw=FALSE;           
        }*/
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
            case AP_KEY_AB | AP_KEY_UP: //����ʱ����ӦREC
            //do nothing
            break;
            case AP_MSG_RTC: //2HZ�жϵ�������Ļ�ػ���Ϊ�˵����ʾ����
            count++;
            //if (count == 2)
            {
                ui_show_batt(GetBattery()); //��ص�����ʾ
                count = 0;
            }
            break;
/*            case AP_KEY_NEXT | AP_KEY_UP: //NEXT����ʱ��NEXT����ֹͣ����
            if (Searchflag == 1)
            {
                Searchflag = 0;
//            		ResShowPic(MNUM0+WFreq/1000,0+8,50);
//								ResShowPic(MNUM0+WFreq/100%10,0+18,50);
//								ResShowPic(MNUM0+WFreq/10%10,0+28,50);
//								ResShowPic(MNUM0+WFreq%10,0+38,50);
            }
            else
            {
                if (Direct == 10)
                {
                    goto Direct50;
                   	if(FMStatus.volume)
                    fmSendCommand(MC_MUTE,releaseMUTE);                    
                }
            }

            break;
            case AP_KEY_PREV | AP_KEY_UP: //LAST����ʱ��LAST����ֹͣ����
            if (Searchflag == 1)
            {
                Searchflag = 0;
            }
            else
            {
                if ((Direct==20)||(Direct==10))
                {
                    Direct50: Direct = 50;
                    if (FMStatus.volume != 0)
                    {
                        fmSendCommand(MC_MUTE, releaseMUTE);
                    }
                }
            }
            break;
*/
            default: //�ȼ�����
            key = ap_handle_hotkey(key);
            if (key == RESULT_REDRAW)
            {
                  need_draw_all=TRUE;
                  need_draw = TRUE;
//                  Need_refresh=0;	//matt for FM hold display
            }
            else
            {
                if (key != 0)
                {
                    return key;
                }
            }
        }
    }
    return key;
}

/*
 ********************************************************************************
 *             bool Search_Show(FM_direct_t Direct)
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
bool Search_Show(FM_direct_t Direct)
{
    bool Search_Flag;
    uint16 i;
    FMStatus.stereo_status = 1;
    FMStatus.station = 0;
    fmSendCommand(MC_MUTE, SetMUTE);
    fmSendCommand(MC_SETVOLUME, FMStatus.volume);
    fmSendCommand(MC_RESTORE, (uint16) & radio_param);
    Search_Flag = fmSendCommand(MC_SEARCH, Direct);
    if (Search_Flag == 1) //��̨�ɹ���MUTE�ͷ�
    {
    	
/*        if ((FMStatus.mode == 1) && (FMStatus.freq > 140)) //���ѵ���Ƶ�����90MHZ����������һ����̨
        {
            Search_Flag = FALSE;
            StationFlag = 0;
            Search_Flag = fmSendCommand(MC_SEARCH, Direct);
        }
        if (Search_Flag == 1)
        {
            StationFlag = 1;
            if (FMStatus.volume != 0)
            {*/
                fmSendCommand(MC_MUTE, releaseMUTE);
//            }
            fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
            ShowFreqInFM();

            g_CNT = 0;
            while (g_CNT < 3)
            {
                ; //�ȴ�2��
            }
    }
					
	
//    Updateflag = 1;

    
    for (i = 0; i < 3999; i++)
    {

    }
//#endif
    return Search_Flag;
}
void ShowFreqNum(void)
{
	uint8 buf[4];
	uint32 i;
    uint32 k;
    uint8 h;
	//Ƶ�ʻ��㼰ת����ASCII��
    i=FMStatus.freq;
    i=i*100;
    if((i%100)>49)
    {
        i=i/100+1;
    }
    else
        i=i/100;
    k=i/10;
    h=i%10;
    ResShowPic(IM_RADIO_CLEAR_FRENUM,0,40);
    //��ʾ��̨Ƶ��
    if(k<100)
    {
        itoa_3(k,buf);
        itoa_1(h,&buf[3]);
//        ResShowPic(PIC_ID_FMNUMBG,PIC_X_FMNUMBG,PIC_Y_FMNUMBG);
    }
    else
    {
        itoa_3(k,buf);
        itoa_1(h,&buf[3]);
        ResShowPic(IM_RADIO_NUMBER_0+buf[0]-0x30,26,40);
    }
    ResShowPic(IM_RADIO_NUMBER_0+buf[1]-0x30,26+9,40);
    ResShowPic(IM_RADIO_NUMBER_0+buf[2]-0x30,26+18,40);
    ResShowPic(IM_RADIO_NUMBER_FULLSTOP,26+27,40);
    ResShowPic(IM_RADIO_NUMBER_0+buf[3]-0x30,26+36,40);
	ResShowPic(IM_FM_MHZ,26+48,40);
}
void ShowSignalST(void)
{
	uint8 strength=0;
	strength=QND_GetRSSI();
	if(strength >= 40)
	{
		strength -= 40;
	}
    else
        strength = 0;
	if(strength <= 5)
	{
		ResShowPic(ICON_MY_RSSI6,64,16);
	}
	else if(strength <= 10)
	{
		ResShowPic(ICON_MY_RSSI5,64,16);
	}
	else if(strength <= 15)
	{
		ResShowPic(ICON_MY_RSSI4,64,16);
	}
	else if(strength <= 20)
	{
		ResShowPic(ICON_MY_RSSI3,64,16);
	}
	else if(strength <= 25)
	{
		ResShowPic(ICON_MY_RSSI2,64,16);
	}
	else if(strength <= 30)
	{
		ResShowPic(ICON_MY_RSSI1,64,16);
	}
	else
	{
		ResShowPic(ICON_MY_RSSI0,60,16);
	}
}
/*
 ********************************************************************************
 *             void ShowFreqInFM()
 *
 * Description : ��ʾ��̨��������
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void ShowFreqInFM(void)
{
    uint8 buf[2];

    ui_auto_update = FALSE;
    
    ClearScreen(NULL);

    ui_show_lock_1();
    
	if(Prs_nor)
	{
		ResShowPic(IM_RADIO_PRESET,0,16);		
		if(FMStatus.station_count > 0)        //��ʾ��̨��
    	{
			if(FMStatus.station != 0)
			{
				ShowStationCount(FMStatus.station);
				ResShowPic(SYSFONT_SMALL_SLASH,0+12,32);
				itoa2(FMStatus.station_count,buf);
				ResShowPic(SYSFONT_SMALL_0+buf[0]-0x30,0+18,32);
				ResShowPic(SYSFONT_SMALL_0+buf[1]-0x30,0+24,32);
				ShowFreqNum();
			}
			else
			    ShowFreqNum();
		}
    	else
    	{
			SetAsciiFont(FONT_TYPE_LARGE);
			ResShowMultiString(SFNONUM, 255, 40);
			Prs_nor = 0;
			nopresetflag = 1;
    	}
	}
	else
	{    
		ResShowPic(IM_RADIO_MESSAGE_BK, 0, 40);
		ResShowPic(IM_RADIO_NORMAL,0,16);
		ShowFreqNum();
	}
	if(FMStatus.stereo_status==0)        
    {                
        ResShowPic(IM_RADIO_STEREO,40,16);        //��ʾ������
    }
    else
    {
        ResShowPic(IM_RADIO_MONO,40,16);
    }
    ui_show_batt(GetBattery());
	      
    ShowSignalST(); 
	UpdateScreen(NULL); 

    ui_auto_update = TRUE;
    ClearWatchDog(); //�忴�Ź�
}
/*
 ********************************************************************************
 *             void    volume_callback(uint16 Vol)
 *
 * Description : �������õ�CALLBACK����
 *
 * Arguments   :    ��ǰ�����õ�����ֵ
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void volume_callback(uint16 Vol)
{
    //ʵʱ�ı�������ֵ
    FMStatus.vol_display = (uint8) Vol;
    /*
    if (g_comval.VolumeMax != 0)
    {
        FMStatus.volume = FMStatus.vol_display * MAXVOLUME / g_comval.VolumeMax;
    }
    */
    FMStatus.volume = (uint8) Vol ;//* SOFTVOL_MAX / g_comval.VolumeMax;
    fmSendCommand(MC_SETVOLUME, FMStatus.volume);
    if (FMStatus.volume == 0)
    {
        fmSendCommand(MC_MUTE, SetMUTE);
    }
    else
    {
        fmSendCommand(MC_MUTE, releaseMUTE);
    }
    m_FMPlaystatus.vol_display = FMStatus.vol_display;
}
/*
 ********************************************************************************
 *             uint16 ChangeVolume(void)
 *
 * Description : ����������ʾ����
 *
 * Arguments   :
 *
 * Returns     : 	����п�ʶ����ȼ���Ϣ,  ���ؽ����Ϣ
 ���û�п�ʶ����ȼ���Ϣ,  ����0
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint16 ChangeVolume(void)
{
	uint8 g_result;
    //init slider
   	ClearScreen(NULL);
    //UpdateScreen(NULL);
    //init slider
    //ResShowPic(T_FM, PIC_X_FMTITLE, PIC_Y_FMTITLE);
    slider.max = g_comval.VolumeMax;
    slider.min = 0;
    slider.step = 1;
    slider.title = NULL; //VOLCHANGE;
    slider.value = (int16) FMStatus.vol_display;
    g_result = ui_slider(&slider, volume_callback);
    FMStatus.vol_display = slider.value;
	FMStatus.volume = FMStatus.vol_display;//*28/g_comval.VolumeMax;	//matt091130#2	for volume 
	fmSendCommand(MC_SETVOLUME, FMStatus.volume);
	//slider.datas = NULL;
    //slider.total = 0;
	//slider.wUnitID = NULL;
    //return ui_slider(&slider, volume_callback);
    return NULL;
}
/*
 ********************************************************************************
 *             void DealFadeIn(uint8 Volume)
 *
 * Description : �������봦����
 *
 * Arguments   :    ��ǰ����
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void DealFadeIn(uint8 Volume)
{
    uint8 i, j;
    uint16 nop = 0;
    for (i = 0; i <= Volume; i++)
    {
        for (j = 0; j < 100; j++)
        {
            nop++;
        }
        SetPAVolume(i);
    }
    ClearWatchDog();
}
/*
 ********************************************************************************
 *             void delay1(void)
 *
 * Description : ��ʱ����
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void delay1(void)
{
    uint16 i;
    for (i = 0; i < 1000; i++)
    {
        ; //����ʱ������ʱ
    }

}
/*
 ********************************************************************************
 *             void DealFadeInOut(void)
 *
 * Description : �˳�FMʱ������������
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void DealFadeInOut(void)
{
    uint8 vol_cnt;
    vol_cnt = FMStatus.volume;
    while (vol_cnt > 0)
    {
        fmSendCommand(MC_SETVOLUME, vol_cnt);
        fmSendCommand(MC_MUTE, releaseMUTE);
        vol_cnt--;
        delay1();
    }

}
