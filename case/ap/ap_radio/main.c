/*
 *******************************************************************************
 *                ACTOS AP
 *        main ap of sdk 3.0
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *       History Record:
 *      1.�������ģʽ����ʱ�����仯���ܱ�������⣬Mars��2008-04-15
 *******************************************************************************
 */
#include "ap_radio.h"

#pragma name(AP_MAIN)

extern fm_play_status_t m_FMPlaystatus;
extern uint8 EnterFlag;
extern void ui_show_volfm(BYTE volume);
uint16 g_CNT = 0;//2hz �жϼ�����(0.5���һ)
uint8 To_FMRec_standby = FALSE; //FM¼����standby����
FMRADIO_param_t radio_param;
function_menu_t menu; //�˵��ṹ��
comval_t g_comval;
uint8 Updateflag; //ˢ�±�־
uint8 value;
bool volmuteflag = 0; //������־
uint8 AutoTabNum;
uint8 Prs_nor=0;//1:prs ��̨ģʽ  0:nor ΢��ģʽ
//��̨Ƶ�ʶ��ձ�(ע:Ƶ�ʷ�Χ87M-108M��US����76M-90M��Japan��,��KΪ����)
uint16 FMStationTab[60];
fm_play_status_t FMStatus; //��ǰ��̨��Ϣ,8 bytes
fm_play_status_t FMStatusUS; //US��̨��Ϣ�ݴ�,8 bytes
fm_play_status_t FMStatusJP; //Japan��̨��Ϣ�ݴ�,8bytes
uint8 fmdataflag[3]; //��һ�ν�FM��־��3 bytes
uint8 Freqtab[10];
uint16 active = 0; //����˵���ѡ�к�
BOOL fm_VolOpFlag = FALSE;
uint8 nopresetflag = 0;
uint8 fmconnect[2] = ":";
slider_t slider;
uint8 FM_Chip = 0;  // 1:8025 2:8035

uint16 FMAutoTab[30]; // ȫ�Զ���̨,Ԥ��̨.
//bool Need_refresh = 1;
uint8 FMFreNumOld; //�ϴ���ʾƵ�����ֵ�λ��
//#define  MENU_COUNT  8
/*
uint16 string_nor[] =
{ FMENU1, FMENU16}; //FMENU15:��̨¼�� by ccm
uint16 string_pre[]=
{ FMENU2, FMENU16};

uint16 string_nor_pedo[] =   //pedometer
{ FMENU1, FMENU16, PEDO, SWIMFIT}; //FMENU15:��̨¼�� by ccm
uint16 string_pre_pedo[]=
{ FMENU2, FMENU16, PEDO, SWIMFIT};
*/
//��������
extern uint16 SearchHandle(uint8 Direct); //�Զ���������Direct=10��NEXT��=20��LAST
extern uint16 AutoSearchHandle(uint8 Direct);
extern void ShowFreqInFM(void); //FM��������ʾ
extern uint16 ChangeVolume(void);
extern bool Search_Show(FM_direct_t Direct); //�Զ���̨ʱ����ʾ
extern void DealFadeIn(uint8 Volume);
extern void DealFadeInOut(void);
extern void read_var(void);
extern void SetMfpToNand(void);
extern void FM_Wait1ms(uint16 time);
extern void ShowFreqInFM2(void);
extern void ShowSignalST(void);
extern uint16 FMdeal(void); //FMӦ�õĴ���

#define OpenFM()
/*void fm_enable(uint8 flag) //FMʹ��
{
    uint8 sfr_bak;

    sfr_bak = SFR_BANK;
    SFR_BANK = BANK_GPIO;
    GPIOAOUTEN |= 0x01 ;
    GPIOAINEN &= ~0x01;
    if (flag == TRUE)
    {
        GPIOADAT |= 0x01;
    }
    else
    {
        GPIOADAT &= ~0x01;
    }
    SFR_BANK = sfr_bak;
}*/
/*
 ********************************************************************************
 *			  uint16 main(uint16 param)
 *
 * Description : ������
 *
 * Arguments   :
 param:
 0, ����FM,���е�̨����
 1,û�ж���ʱ���˻�main AP
 2, ���FMӲ���Ƿ���ȷ����FM��standby
 3, FM¼����standby�������ȴ�FM
 *
 * Returns     : �����Ϣ��0
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint16 main(uint16 param)
{
    uint16 result = 0;
    uint8 sfr_bak;
    uint8 KeyTone_bak;
    uint8 Timer_Sav; //for timer
    uint8 HOSC_bak;
//	uint8 qn_ret;
		
    To_FMRec_standby = FALSE;
    if (param == 1)
    {
        return RESULT_MAIN;
    }
    sfr_bak = SFR_BANK;
    SFR_BANK = BANK_CMU_RMU; //�رո�Ƶ�޷�����
    HOSC_bak = HOSC_CTL; //����HOSC��ֵ
    HOSC_CTL &= (uint8) 0xf7;
    HOSC_CTL |= (uint8) 0x06;
    SFR_BANK = sfr_bak;
//	fm_enable(TRUE);
    if (param == 2) //��FMģ���standby
    {
        EnterFlag = 1; //not open PA
        fmSendCommand(MC_OPEN, 5);
        fmSendCommand(MC_MUTE, SetMUTE);
        fmSendCommand(MC_CLOSE, 0);
//        fm_enable(FALSE);
        sfr_bak = SFR_BANK;
        SFR_BANK = BANK_CMU_RMU;
        HOSC_CTL = HOSC_bak; //�ָ�HOSC��ֵ
        SFR_BANK = sfr_bak;
        return 0;
    }
    else if (param == 3) //��standby ����Ҫ��FM ¼���� �ȴ�FM
    {
        To_FMRec_standby = TRUE;
    }
    while (ap_get_message() != 0)
    {
        ; //�����Ϣ��
    }
    ap_sleep(1);

    //-------��װud����------------
    DRV_ActiveUD(0x00, MODE_SLOW);

    result = VMRead(&g_comval, VM_SYSTEM, sizeof(g_comval)); //�ȶ���ϵͳ����
    //check if valid
    if (g_comval.magic != MAGIC_COMVAL) //��һ�ζ�����ʼ��
    {

        GetDefaultComval(&g_comval);
    }
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

    res_fp = ResOpen("ui30.res"); //����Դ

    ap_message_init(&g_comval); //ϵͳ��Ϣ����RTC COUNTER����ʼ��
    result = VMRead(FMStationTab, VM_AP_RADIO, 120 + sizeof(fm_play_status_t) * 3 + 3); //�������¼���//liuf

    Timer_Sav = TM_SetClock((int16) Search_TimerInt, 1); //����2HZ��ʱ��

    EnterFlag = 0; //when fm  open,  open PA
    KeyTone_bak = g_comval.KeyTone; //���ݰ�����
    if (g_comval.KeyTone != 0)
    {
        g_comval.KeyTone = 0; //�رհ�����
    }
    result = FMdeal();
    //g_comval.KeyTone = KeyTone_bak; //�ָ�������
    //�������
    fmSendCommand(MC_BACKUP, (uint16) & radio_param);
    if (result == RESULT_RECORD) //�ػ�FM������Rec�ȼ�¼��
    {
        result = RESULT_FMREC_START;
    }
    if ((result != RESULT_FMREC_NOSTART) && (result != RESULT_FMREC_START)) //�ǵ�̨¼���˳���for FM record by ccm
    {
        //DealFadeInOut();  //matt140206#4
        fmSendCommand(MC_MUTE, SetMUTE);
        
        fmSendCommand(MC_CLOSE, 0);
//		fm_enable(FALSE);
        // CloseFMPower();
        sfr_bak = SFR_BANK;
        SFR_BANK = BANK_CMU_RMU;
        HOSC_CTL = HOSC_bak; //�ָ�HOSC��ֵ
        SFR_BANK = sfr_bak;
    }
    TM_KillClock((int8) Timer_Sav); //�رն�ʱ��
    //���ǵ�̨¼���˳�����ʱFM���ڹ��������ܶ�HOSC���в���
	
	#ifdef Greenhouse
	g_comval.Volume_G = FMStatus.vol_display;
	#endif
	
    VMWrite(FMStationTab, VM_AP_RADIO, 120 + sizeof(fm_play_status_t) * 3 + 3);
    VMWrite(&g_comval, VM_SYSTEM, sizeof(g_comval));
    ResClose(res_fp); //��Դ�ر�
    return result;
}


#if (0)
uint16 FMdeal(void)
{
    uint16 result = 0;
    uint8 key;
	uint8 modekeystore = NULL;
    //uint8 sfr_bak;
//    uint8 buf[4] =
//    { "FM" };
    //��־��need_draw���ػ� BandSwitch��Ƶ���л� FirstWait���ս�AP
    bool need_draw=TRUE/*,need_draw_all=TRUE*/,BandSwitch=TRUE,FirstWait=TRUE;  
//    uint16 k;
    // uint8 strvolume = 0; //czh bit7:flash enable flag, bit 6:flash flag, other bits: count time
    uint8 count = 0;
	uint8 nopresetnum=0;
//	uint8 buf[2];

	ResShowPic(IM_WAITING, 0, 0);
    read_var();
    g_comval.KeyTone=0;
    //clear the message pool  �������ʼ��...�������а��µļ�!
    while (ap_get_message() != AP_KEY_NULL)
    {
        ; //�����Ϣ��
    }
    
    while (1)
    {
#ifndef PC
        if (BandSwitch == TRUE) //��ʼ��
        {
            if (FMStatus.mode == 0)
            {
                radio_param.param1 = FMStationTab;
            }
            else
            {
                radio_param.param1 = &FMStationTab[60];
            }
            (fm_play_status_t *) radio_param.param2 = &FMStatus;

            fmSendCommand(MC_RESTORE, (uint16) & radio_param);
            fmSendCommand(MC_SETVOLUME, 0);
            if (!fmSendCommand(MC_OPEN, SEARCHLEVEL)) //Ӳ��������ʾ
            {
//                SetPenColor(Color_WHITE_def);
//                SetBackgdColor(Color_BULE_def);
                ClearScreen(NULL);
                ResShowMultiString(FMERROR, 255, 8);
                strcpy(fmdataflag, "  ");
                ap_sleep(3);
                return RESULT_MAIN;
            }
            fmSendCommand(MC_MUTE, SetMUTE);
            fmSendCommand(MC_PLAY, 0);
            BandSwitch = FALSE;
            FirstWait = TRUE;
        }
#endif   
		/*if(need_draw_all)
        {
            ResShowPic(IM_RADIO_BG,0,0);
            need_draw_all=FALSE;
            need_draw=TRUE;
        }*/
        
        if ((need_draw == TRUE) && (To_FMRec_standby == FALSE)) //��Ļ�ػ�
        {
            if(Pedo_ui_draw)
            {
                Pedo_RefreshMainScreen(Pedo_Swim_flag);
            }    
            else
            {
                ShowFreqInFM();
            }
            need_draw = FALSE;
        }
        
        if (FirstWait == TRUE) //���ν���AP�ȴ�1.5S�ͷ�MUTE(����)
        {
            FirstWait = FALSE;
            ap_sleep(3);
			FMStatus.volume = FMStatus.vol_display;
            fmSendCommand(MC_SETVOLUME, FMStatus.volume);
            fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
            if (FMStatus.volume != 0)
            {
                fmSendCommand(MC_MUTE, releaseMUTE);
            }
        }
        
        if (To_FMRec_standby == TRUE)
        {
            To_FMRec_standby = FALSE;
            return RESULT_FMREC_NOSTART; //���ص�FM¼��
        }

        #if(MACHINE_MODEL==2)
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
        #endif
        
        key = ap_get_message();
        switch (key)

        {
            case AP_MSG_STANDBY:
            break;
            
            case AP_MSG_RTC: //2HZ�жϵ�������Ļ�ػ���Ϊ�˵����ʾ����
                if(Pedo_ui_draw)
    			{
    			        if((Pedo_work_flag)||(Swim_work_flag))
    			        {
        			        Pedo_ui_num++;
        					if(Pedo_ui_num == 9)		
        					{			
        						Pedo_ui_num = 0;	
        					}
        					Pedo_RefreshDataScreen(Pedo_Swim_flag);
			            }
    			}
				else
				{
                    count++;
                    if (count == 4)
                    {
                        count = 0;
                        fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
                        if(FMStatus.stereo_status==0)        
                		{                
                    		ResShowPic(IM_RADIO_STEREO,40,16);        //��ʾ������
                		}
                		else
                		{
                    		ResShowPic(IM_RADIO_MONO,40,16);
                		}
                		ShowSignalST();  
                    }
                    
                    ui_show_batt(GetBattery()); //��ص�����ʾ
             	
    			    if(nopresetflag == 1)
                    {
    				    nopresetnum++;
    				    if(nopresetnum >= 4)
    				    {
    					    need_draw = TRUE;
    					    nopresetflag = 0;
    					    nopresetnum = 0;
    				    }
                    }
				}
      		    break;
				
          case AP_KEY_HOLD | AP_KEY_NEXT:
                if(Pedo_ui_draw) break;
                if(Prs_nor == 0)
        		{
                   	result = SearchHandle(10);
        			fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
        		}
        		while( ap_get_message() != AP_KEY_NULL )
                {
                    ; //�����Ϣ��
                }
                break;

            case AP_KEY_HOLD | AP_KEY_PREV:
                if(Pedo_ui_draw) break;
                if(Prs_nor == 0)
        		{
                   	result = SearchHandle(20);
                   	fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
        	  	}
    	  		while( ap_get_message() != AP_KEY_NULL );
                {
                    ; //�����Ϣ��
                }
                break;

            case AP_KEY_NEXT | AP_KEY_UP:
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
    		        break;
    			}
    			
                if(Prs_nor == 1)
    		   	{
                	fmSendCommand(MC_MUTE, SetMUTE);
                	SetPAVolume(0);
                	fmSendCommand(MC_STEP, UP);
                	if (FMStatus.volume != 0)
                	{
                    	fmSendCommand(MC_MUTE, releaseMUTE);
                	}
                	DealFadeIn(FMStatus.volume);
                	fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
    //            	Updateflag = 1;
    		   	}
    			else
    			{
    				/*itoa2(m_FMPlaystatus.station, buf);
    				ResShowPic(SYSFONT_SMALL_0 + buf[0] - 0x30, 15, 0);
    				ResShowPic(SYSFONT_SMALL_0 + buf[1] - 0x30, 15 + 8, 0);*/
    				
    				fmSendCommand(MC_STEPGRID, UP); //��̨Ƶ��΢������100KHZΪ����		
                	fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
    			}
    //            Updateflag = 1;
                need_draw = TRUE;
                break;

            case AP_KEY_PREV | AP_KEY_UP:	
                if(Pedo_ui_draw) break;
                if(Prs_nor == 1)
    		   	{
                	fmSendCommand(MC_MUTE, SetMUTE);
                	SetPAVolume(0);
                	fmSendCommand(MC_STEP, DOWN);
                	if (FMStatus.volume != 0)
                	{
                    	fmSendCommand(MC_MUTE, releaseMUTE);
                	}
                	DealFadeIn(FMStatus.volume);
                	fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
    //            	Updateflag = 1;
    		   	}
    			else
    			{
    				fmSendCommand(MC_STEPGRID, DOWN);
                	fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
    			}
                need_draw = TRUE;
                break;

			case AP_KEY_AB | AP_KEY_UP:
			case AP_KEY_VOLSUB:
            case AP_KEY_VOLADD:
			    if(Pedo_ui_draw)
        		{
        			break;
        		}  
        		else
        		{
				    ChangeVolume();
//			        need_draw_all = TRUE;
			        need_draw = TRUE;
			    }
			    break;
			    
			case AP_KEY_AB | AP_KEY_LONG:
				return RESULT_FMREC_START;
			
            case AP_KEY_MODE:
                modekeystore = key;
                break;

            case AP_KEY_MODE | AP_KEY_UP:
                if (modekeystore != AP_KEY_MODE)
                { //Ҫ�Ȱ���AP_KEY_MODE������up������Ч
                    modekeystore = NULL;
                    break;
                }
                if(Pedo_ui_draw)
        		{
        		    result = Pedo_menu(Pedo_Swim_flag);
                    need_draw = TRUE;
                    if(result == RESULT_RETURN_TO_MAIN)
                    {
                        //sKY_SetGsensor(0,0,0);
                	    //Pedo_work_flag = FALSE;
                	    Pedo_ui_draw = FALSE;
                	    need_draw = TRUE;
                	    break;
                    }
        			if(result!=NULL)
			        {
			   	        return  result;
			        }
        		}  
        		
    			if(Prs_nor == 1)
    		   	{
                	if(g_comval.PedoFlag)
                	{
                        menu.total = sizeof(string_pre_pedo) / 2; //MENU_COUNT; 
                        menu.string = string_pre_pedo;
                	}
                    else
                    {
                        menu.total = sizeof(string_pre) / 2; //MENU_COUNT; 
                        menu.string = string_pre;
                	}
                	menu.active = 0;
    				menu.menuhead= NULL; //RADIO;
    				menu.string_id=g_comval.langid;
                	//ui_auto_select = TRUE;
                	result = ui_function_menu(&menu, NULL);
                	//ui_auto_select = FALSE;
    				need_draw = TRUE;
    //				need_draw_all=TRUE;
                //��PLAY����8s��ʱ����FM��������,��ִ�е�ǰ����Ĳ˵�!
                	if ((result == RESULT_IGNORE) || (result == RESULT_AUTO_BACK))
                	{
                    	result = 0;
                    	break;
                	}
                	if (result != 0)
                	{
                    	return result;
                	}
                	active = menu.active;
                	if (menu.active == 0) //ɾ����̨
                	{
    					if (fmSendCommand(MC_DELONE, 0) != FALSE)
                    	{
                        	fmSendCommand(MC_BACKUP, (uint16) & radio_param);
                    	}
                    	fmSendCommand(MC_STEP, UP);
                    	fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
                	}
    				else  if(menu.active == 1)
                	{
                		//ShowFreqInFM2();
    					fmSendCommand(MC_DELALL, 0);
                    	fmSendCommand(MC_BACKUP, (uint16) & radio_param);
                    	fmSendCommand(MC_MUTE, SetMUTE);
                    	result = AutoSearchHandle(10);
                    	if (result == RESULT_IGNORE)
                    	{
                        	result = 0;
                    	}
                    	if (result != 0)
                    	{
                    		return result;
                    	}
                    	if (FMStatus.volume != 0)
                    	{
                        	fmSendCommand(MC_MUTE, releaseMUTE);
                    	}
                    	fmSendCommand(MC_SETVOLUME, FMStatus.volume);
                    	fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
    //					need_draw_all=FALSE;         
    				}
    				else    //pedometer
    				{
                        Pedo_ui_draw = TRUE;
                        need_draw = TRUE;
    				}
    				    
    			}
    			else
    			{		
    			    if(g_comval.PedoFlag)
                	{
                        menu.total = sizeof(string_nor_pedo) / 2; //MENU_COUNT; 
                        menu.string = string_nor_pedo;
                	}
                    else
                    {
                        menu.total = sizeof(string_nor) / 2; //MENU_COUNT; 
                        menu.string = string_nor;
                	}
                	menu.active = 0;
                	menu.menuhead= NULL; //RADIO;
                	menu.string_id = g_comval.langid;           	
                	//ui_auto_select = TRUE
                	result = ui_function_menu(&menu, NULL);
                	//ui_auto_select = FALSE;
                	need_draw = TRUE;
    //				need_draw_all=TRUE;
                //��PLAY����8s��ʱ����FM��������,��ִ�е�ǰ����Ĳ˵�!
                	if ((result == RESULT_IGNORE) || (result == RESULT_AUTO_BACK))
                	{
                    	result = 0;
                    	break;
                	}
                	if (result != 0)
                	{
                    	return result;
                	}
                	active = menu.active;
                	if (menu.active == 0) //�����̨
                	{
    					if (fmSendCommand(MC_SAVESTATION, 0) != FALSE)
                    	{
    						Prs_nor=1;
    						fmSendCommand(MC_BACKUP, (uint16) & radio_param);
    					}
    				}
                	else if(menu.active == 1)
                	{
                		fmSendCommand(MC_DELALL, 0);
                    	fmSendCommand(MC_BACKUP, (uint16) & radio_param);
                    	fmSendCommand(MC_MUTE, SetMUTE);
                    	result = AutoSearchHandle(10);
                    	if (result == RESULT_IGNORE)
                    	{
                        	result = 0;
                    	}
                    	if (result != 0)
                    	{               		
                        	return result;
                    	}
                    	if (FMStatus.volume != 0)
                    	{
                        	fmSendCommand(MC_MUTE, releaseMUTE);
                    	}
                    	fmSendCommand(MC_SETVOLUME, FMStatus.volume);
                    	fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);       
    //					need_draw_all=FALSE; 
    				}   
    				else if(menu.active == 2)    //pedometer
    				{
                        Pedo_ui_draw = TRUE;
            			Pedo_Swim_flag = FALSE;
            			if(Swim_work_flag)  Swim_work_flag = FALSE;
                        need_draw = TRUE;
    				}
    				else //if(menu.active == 3)    //swim fitness
    				{
                        Pedo_ui_draw = TRUE;	  
            			Pedo_Swim_flag = TRUE;
            			if(Pedo_work_flag)  Pedo_work_flag = FALSE;
                        need_draw = TRUE;
    				}
    				    
    			}
                /*else if (list.active == 5) //ѡ��ŷ��Ƶ��
                {
                    if (FMStatus.mode == 1)
                    {
                        memcpy(&FMStatusJP, &FMStatus, sizeof(FMStatus));
                        memcpy(&FMStatus, &FMStatusUS, sizeof(FMStatus));
                        BandSwitch = TRUE;
                    }
                }
                else if (list.active == 6) //ѡ���ձ�Ƶ��
                {
                    if (FMStatus.mode == 0)
                    {
                        memcpy(&FMStatusUS, &FMStatus, sizeof(FMStatus));
                        memcpy(&FMStatus, &FMStatusJP, sizeof(FMStatus));
                        BandSwitch = TRUE;
                    }
                }

                else
                {

                }  */
                break;
            
            case AP_KEY_PLAY | AP_KEY_UP: //������̨�����һ��
                if(Pedo_ui_draw)
        			{
        				if(Pedo_work_flag)
        				{
        					sKY_SetGsensor(0,0,0,Pedo_Swim_flag);
        					Pedo_work_flag = FALSE;
        				}
        				else if(Swim_work_flag)
        				{
        					sKY_SetGsensor(0,0,0,Pedo_Swim_flag);
        					Swim_work_flag = FALSE;
        				}
        				else
        				{
        					if(Pedo_Swim_flag)
        					{
        					    Swim_work_flag = TRUE;
        					    Pedo_work_flag = FALSE;
        					    sKY_SetGsensor(0,0,0,0);
        					    sKY_SetGsensor(1,0,0,Pedo_Swim_flag);
        					}
        					else
        					{
                                Pedo_work_flag = TRUE;
        					    Swim_work_flag = FALSE;
        					    sKY_SetGsensor(0,0,0,1);
        					    sKY_SetGsensor(1,0,0,Pedo_Swim_flag);
        					}
        				}
        				Pedo_RefreshDataScreen(Pedo_Swim_flag);
                    }
        		else
        		{
                    if(Prs_nor)
		   	            Prs_nor = 0;
    		        else
    		        {
    		   	        Prs_nor = 1;
    		   	        fmSendCommand(MC_MUTE, SetMUTE);
                	    SetPAVolume(0);
                	    fmSendCommand(MC_STEP, UP);
                	    if (FMStatus.volume != 0)
                	    {
                    	    fmSendCommand(MC_MUTE, releaseMUTE);
                	    }
                	    DealFadeIn(FMStatus.volume);
                	    fmSendCommand(MC_GETSTATUS, (uint16) & FMStatus);
    		   	    }
		   	    }
                need_draw = TRUE;
                break;
	    
            default: //�ȼ�����
                key = ap_handle_hotkey(key);
                if (key == RESULT_REDRAW)
                {
    //                  need_draw_all=TRUE;
                      need_draw = TRUE;
    //                  Need_refresh=0;	//matt for FM hold display
                }
                else if (key == RESULT_RECORD)
                {
                    return RESULT_FMREC_START;
                }
                else
                {
                    if ((key != 0) || (result != 0))
                    {
                        if (key != 0)
                        {
                            return key;
                        }
                        else
                        {
                            return result;
                        }
                    }
                }
        }
    }
    return key;
}
#endif
