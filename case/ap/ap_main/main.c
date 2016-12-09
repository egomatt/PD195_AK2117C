/*
******************************************************************************* *                               AS211A
*                              AS211A
*                              Module: main ap
*                 Copyright(c) 2002-2010 Actions Semiconductor,
*                            All Rights Reserved. 
*
* Descripton: the main module of main menu
*
* History:         
*      <author>    <time>           <version >             <desc>
*       xusong    2010-06-29 15:00     1.0             build this file 
 *******************************************************************************
 */
#include "ap_main.h"
#pragma name(AP_MAIN)
extern uint8 UDiskOperationInfo;

/*ȫ�ֱ�������*/
uint8 standby_flag = 0; /*�жϴ�standby�������Ƿ����������Ϣ�ı�����0��Ҫ�����1����Ҫ*/
uint8 ap_id[MAIN_ITEMCOUNT];/*���ap�ṹ��Ϣ�����飬�������ļ��ж���*/
bool with_fm = FALSE;       /*֧��FM ģ��ı���*/
uint32 item_offset[8];   /*��ȡ�����ļ�������ƫ�Ƶ�����*/
comval_t g_comval;      /*�洢ϵͳ������ȫ�ֱ���*/
vm_cfg_t g_modifyval;   /*�洢֧�ֹ̼�modify���ܲ�����ȫ�ֱ���*/
function_menu_t menu;

/*������������*/
const uint8 FwcfgName[] = "FWCFG.BIN";
const uint16 inactive_id[MAIN_ITEMCOUNT] =
{ IM_PHASE_01MUSIC_01,IM_PHASE_03MIC_01, IM_PHASE_04RADIO_01,IM_PHASE_08PEDO_01,IM_PHASE_09GAME_01,IM_PHASE_05SETUP_01,0,0,0,0,0};   //pedometer
const uint16 main_text[MAIN_ITEMCOUNT] =
{ MAIN1,MAIN3,MAIN4,PEDO,SWIMFIT,MAIN5,0,0,0,0,0};   //pedometer

/*Bank �����ݶεĶ���*/
uint8 far sd_read_buf[512];

void draw_logo(BOOL wait_key_up);
bool check_ap_result(int8 check_result, int8 *result_ap, int8 ap_total);
void OpenPA(uint8 volume);
void OpenDAC(void);

/*
 *******************************************************************************
 * Description : ��ʼ��Modify���ñ���
 *
 * Arguments   :   ap_result,��� AP�����Ϣ������ָ��
 *
 * Returns     :   NONE
 *
 * Notes       :
 *
 *******************************************************************************
 */
void ReadModifyInfor(void) //read modify information
{
    uint8 i, j = 0;
    VMRead(&g_modifyval, VM_AP_FWCFG, sizeof(vm_cfg_t));
#ifndef PC
    if (g_modifyval.Magic != MAGIC_CFG) //read modify information from FWCFG.BIN
#endif
    {
        SD_FILE *fwcfgfp;
        fwcfgfp = SD_FOpen(FwcfgName, MODE_READ);
        /*��ȡ�����ļ�ͷ��Ϣ*/
        SD_FRead(fwcfgfp, sd_read_buf, sizeof(sd_read_buf)); //��ȡ�����ļ�ͷ��Ϣ

        item_offset[0] = ((uint32) (sd_read_buf[18])) * 0x100 + sd_read_buf[19];
        item_offset[1] = ((uint32) (sd_read_buf[50])) * 0x100 + sd_read_buf[51];
        item_offset[2] = ((uint32) (sd_read_buf[82])) * 0x100 + sd_read_buf[83];
        item_offset[3] = ((uint32) (sd_read_buf[114])) * 0x100 + sd_read_buf[115];
        item_offset[6] = ((uint32) (sd_read_buf[178])) * 0x100 + sd_read_buf[179];

        SD_FSeek(fwcfgfp, SEEK_SET, item_offset[0]); //��ȡ������ͷ��Ϣ
        SD_FRead(fwcfgfp, sd_read_buf, 512); //��ȡ��ֵ��������Ϣ

        item_offset[4] = ((uint32) sd_read_buf[2]) * 0x100 + sd_read_buf[3];
        item_offset[5] = ((uint32) sd_read_buf[7]) * 0x100 + sd_read_buf[8];

        /*��ȡ��ֵ�������Ϣ*/
        memcpy(&g_modifyval.Autolock, &sd_read_buf[16], sizeof(value_set_t));
        memcpy(&g_modifyval.ContrInfor, &sd_read_buf[64], sizeof(value_set_t));
        memcpy(&g_modifyval.BacklightTime, &sd_read_buf[112], sizeof(value_set_t));
        memcpy(&g_modifyval.PowerOfftime, &sd_read_buf[160], sizeof(value_set_t));
        memcpy(&g_modifyval.SleepTime, &sd_read_buf[208], sizeof(value_set_t));
        memcpy(&g_modifyval.Volume, &sd_read_buf[256], sizeof(value_set_t));
        memcpy(&g_modifyval.VolDefault, &sd_read_buf[304], sizeof(value_set_t));
        memcpy(&g_modifyval.RecordGain, &sd_read_buf[352], sizeof(value_set_t));

        SD_FSeek(fwcfgfp, SEEK_SET, item_offset[0] + (uint32)8*sizeof(value_t)); //��ȡ������ͷ��Ϣ
        SD_FRead(fwcfgfp, sd_read_buf, 512); //��ȡ��ֵ��������Ϣ
        memcpy(&g_modifyval.Stride, &sd_read_buf[16], sizeof(value_set_t));   //pedometer
        memcpy(&g_modifyval.PedoUnit, &sd_read_buf[64], sizeof(value_set_t));
        memcpy(&g_modifyval.Weight, &sd_read_buf[112], sizeof(value_set_t));
        memcpy(&g_modifyval.ArmLen, &sd_read_buf[160], sizeof(value_set_t));
        memcpy(&g_modifyval.PoolLen, &sd_read_buf[208], sizeof(value_set_t));
        memcpy(&g_modifyval.Record_Mode, &sd_read_buf[256], sizeof(value_set_t));

        /*��ȡ�ַ���������Ϣ*/
        SD_FSeek(fwcfgfp, SEEK_SET, item_offset[5]);
        SD_FRead(fwcfgfp, sd_read_buf, 512);
        memcpy(g_modifyval.DevVesion, sd_read_buf, sizeof(g_modifyval.DevVesion));
        memcpy(g_modifyval.FirmwareDate, &sd_read_buf[64], sizeof(g_modifyval.FirmwareDate));

        /*��ȡ����������Ϣ*/
        SD_FSeek(fwcfgfp, SEEK_SET, item_offset[1]);
        SD_FRead(fwcfgfp, sd_read_buf, sizeof(key_cfg_t));
        g_modifyval.KeyInfor.Total = ((key_cfg_t *) (sd_read_buf))->TotalKey;
        memcpy(g_modifyval.KeyInfor.Table, ((key_cfg_t *) (sd_read_buf))->KeyValue, 16);

        /*��ȡap��������Ϣ*/
        SD_FSeek(fwcfgfp, SEEK_SET, item_offset[2]);
        SD_FRead(fwcfgfp, sd_read_buf, 512);
        for (i = 0, j = 0; i < sd_read_buf[0]; i++)
        {
            if (((ap_infor_t *) (&sd_read_buf[16 + 32 * i]))->Status != 0)
            {
                g_modifyval.ApInfor.Table[j] = ((ap_infor_t *) (&sd_read_buf[16 + 32 * i]))->Index;
                if (g_modifyval.ApInfor.Table[j] == RESULT_RADIO)
                {
                    with_fm = TRUE;
                }
				/*�ҳ�Active �����*/
                if(g_modifyval.ApInfor.Table[j] == sd_read_buf[1])
                {							 
                   g_modifyval.ApInfor.Active = j;
                }    
                j++;
            }
        }
        g_modifyval.ApInfor.Total = j;

        /*��ȡ����������Ϣ*/
        SD_FSeek(fwcfgfp, SEEK_SET, item_offset[3]);
        SD_FRead(fwcfgfp, sd_read_buf, sizeof(lan_set_t));
        memcpy(&g_modifyval.LanInfor, sd_read_buf, sizeof(lan_set_t));

        /*��ȡ����������Ϣ*/
        SD_FSeek(fwcfgfp, SEEK_SET, item_offset[6]);
        SD_FRead(fwcfgfp, sd_read_buf, 512);
        g_modifyval.CardFlag = ((func_item_t *) (&sd_read_buf[16]))->State;
        g_modifyval.EarphoneFlag = ((func_item_t *) (&sd_read_buf[64]))->State;
        g_modifyval.ClassDFlag = ((func_item_t *) (&sd_read_buf[112]))->State;
        g_modifyval.KeyToneFlag = ((func_item_t *) (&sd_read_buf[160]))->State;
        
        g_modifyval.PedoFlag = ((func_item_t *) (&sd_read_buf[208]))->State;   //pedometer
        if(g_modifyval.PedoFlag) 
        {
            g_modifyval.ApInfor.Total = j+2;
            g_modifyval.ApInfor.Table[j] = 5; 
            g_modifyval.ApInfor.Table[j+1] = 6; 
        }
        
        g_modifyval.Magic = MAGIC_CFG;
        SD_FClose(fwcfgfp);
        VMWrite(&g_modifyval, VM_AP_FWCFG, sizeof(g_modifyval));

    }
    memcpy(sKY_GetKeyTabAddress(), g_modifyval.KeyInfor.Table, g_modifyval.KeyInfor.Total);
    memcpy(ap_id, g_modifyval.ApInfor.Table, sizeof(ap_id));
    ClearWatchDog();
}
/*
 ********************************************************************************
 * Description : ��ʼ��ϵͳ���ñ���
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void read_var(void)
{
    BOOL result;
    uint8 tmp_buf[32];
    ReadModifyInfor();
    result = VMRead(&g_comval, VM_SYSTEM, sizeof(g_comval));
    if (g_comval.magic != MAGIC_COMVAL)
    {
 				memset(tmp_buf,0,0x20);
    		tmp_buf[0]=0xff;
    		tmp_buf[16]=0xff;
     		VMWrite(tmp_buf, VM_FS_FLAG, sizeof(tmp_buf)); 
UDiskOperationInfo&=0xfb;
        GetDefaultComval(&g_comval);
        g_comval.BatteryType = BATT_TYPE_LITHIUM;
        g_comval.Lightmode = 1;
        g_comval.KeyTone = 0;

        g_comval.DisplayContrast = g_modifyval.ContrInfor.DefValue;
        g_comval.LightTime = g_modifyval.BacklightTime.DefValue;
        g_comval.LockTime = g_modifyval.Autolock.DefValue;
        g_comval.StandbyTime = g_modifyval.PowerOfftime.DefValue;
        //g_comval.SleepTime = g_modifyval.SleepTime.DefValue;

        g_comval.VolumeMax = g_modifyval.Volume.DefValue;    
        g_comval.VolumeDef = g_modifyval.VolDefault.DefValue;
        g_comval.Volume_G = g_comval.VolumeDef;
        if(g_comval.VolumeDef > g_comval.VolumeMax)
        {
            g_comval.VolumeDef = g_comval.VolumeMax;
        }    
        g_comval.langid = g_modifyval.LanInfor.Active;
        g_comval.BackLightMax = g_modifyval.ContrInfor.Max;
        g_comval.BackLightDef = g_modifyval.ContrInfor.DefValue;
        
		g_comval.Stride= g_modifyval.Stride.DefValue;   //pedometer
		g_comval.PedoUnit = g_modifyval.PedoUnit.DefValue; 
		g_comval.Weight = g_modifyval.Weight.DefValue; 
		g_comval.ArmLen = g_modifyval.ArmLen.DefValue; 
		g_comval.PoolLen = g_modifyval.PoolLen.DefValue; 
		g_comval.PedoFlag= g_modifyval.PedoFlag; 
		g_comval.Record_Mode = g_modifyval.Record_Mode.DefValue;
		
		g_comval.rec_bitrate=0;//0/1/2 8k/12k/16k  32bits/48bits/64bits
		
        g_comval.playpowerofflag = g_modifyval.SleepTime.DefValue;
		
        g_comval.ClassDFlag = g_modifyval.ClassDFlag;
        g_comval.EarphoneFlag = g_modifyval.EarphoneFlag;
        g_comval.SuppCard = g_modifyval.CardFlag;
        g_comval.RecordGain = g_modifyval.RecordGain.DefValue;
        result = VMWrite(&g_comval, VM_SYSTEM, sizeof(g_comval));

    }
    /*�忨֧�ֿ���ͨ��modify�����޸�*/
    SuppCardFlag = g_comval.SuppCard;
    if (g_comval.BatteryType == BATT_TYPE_LITHIUM)
    {
        low_battery_value = LITHIUM_LOW;
    }
    ClearWatchDog();
    ap_message_init(&g_comval);
    SetLanguage(g_comval.langid);
    SetContrast(g_comval.DisplayContrast);

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

 

void delay1(void)
{
    uint16 i, j = 0;
    for (i = 0; i < 1000; i++)
    {
        j++;
    }
}



/*void keyvalue(void)
{
		uint8 value;
		uint8 sfr_bak;
		sfr_bak = (uint8) SFR_BANK;
		SFR_BANK = BANK_PMU;
    value = LRADC1_DATA & 0x3f;
    ResShowPic(YIM_TIME_NUMBER_0 + value/10,30, 10);
    ResShowPic(YIM_TIME_NUMBER_0 + value%10, 30+10, 10);
    SFR_BANK = sfr_bak;
}*/ 

/*
 ********************************************************************************
 * Description : ������
 *
 * Arguments   :
 *
 * Returns     : �����Ϣ��0
 *
 * Notes       :
 *
 ********************************************************************************
 */

int16 main(int16 param)
{
    uint8 item_count, n = 0;
    uint8 active = 0; //��ǰ�����Ŀ
    uint8 key;
    int8 IsUDisk = 0;
    int8 UdiskFlag = 0;
    int16 result;
    int16 result_bak = 0; //�ػ�ʱ���ϴ�ִ�е�ap���ڴ�
    BOOL need_draw = TRUE;
    BOOL need_draw_all = TRUE;
    BOOL To_standby_FMRec = FALSE;
    int16 ResultCheck = 0;
    uint8 retval;
    uint16 param1;
	uint8 sfr_bak;
//	uint8 item_index;
    uint8 DisplayActive = 0;
	const region_t region={0, 0, 20, 8};//{18, 4, 136, 16};
	uint16 scrollcount=0;
    uint8 pause_flag;
	uint8 time_to_step = 0;
    uint8 tmpval = SFR_BANK;
	
    param = param;
    ClearWatchDog();
	
    //������Ƶ��������
    SFR_BANK = BANK_CMU_RMU;
    HOSC_CTL &=0xf9;                           //���������������
    HOSC_CTL |=0x06;                            //���������������
    SFR_BANK = tmpval;	
    

    if (!DRV_Install("DRV_UI.DRV", 0))
    {
        while (1)
        {
            ;//ȱʡ����
        }
    }
    if (!DRV_Install("KY_HARD.DRV", 0))
    {
        while (1)
        {
            ;//ȱʡ����
        }
    }
    read_var();
    SetLanguage(g_comval.langid);
    /*��FM ģ���Standbyģʽ*/
    if(with_fm)    
    {
        ExecAP("fmradio.ap", 2);
        read_var();
    }
    
    if(g_comval.PedoFlag)   //pedometer
    {
        sKY_SetGsensor(0,0,0,0);
        sKY_SetGsensor(0,0,0,1);
    }
    
    //LightOnOff(FALSE);
    //ClearScreen(NULL); 
    //LightOnOff(TRUE);
    res_fp = ResOpen("ui30.res");
    result = NULL;

    SetAsciiFont(FONT_TYPE_LARGE);
    while (GetSysMsg() != Msg_KeyNull)
    {
        ;//QAC comment
    }

    retval = check_5v();
    if (retval)
    {   
        //��ʼ���
        //sKY_ChargeSet(1);
        //sKY_ChargeGetandSet();
        result = RESULT_UDISK; //or RESULT_PDDRM
    }
    if(g_comval.MTPFormatType == 17)
        result = RESULT_MUSIC_PLAY;
    
    draw_logo(FALSE);

	//OpenDAC();
	//OpenPA(0);
    item_count = g_modifyval.ApInfor.Total;//FIXME g_modifyval.apinfo.ApplicationTotal;
    active = g_modifyval.ApInfor.Active;
    if(active > item_count-1)
    {
        active = 0;
    }    
	if(g_comval.KeyTone>0)
	{
		OpenDAC();
		OpenPA(g_comval.VolumeDef);	   //�а���������PA
	    if(g_comval.ClassDFlag)
		{
		   earphoneinflag =1;
		}
	}

	
#if 1	

    while (1)
    {
        ClearWatchDog();
         
        if ((result == RESULT_NULL) || (result == RESULT_MAIN))
        {
            result_bak = RESULT_NULL;
            if (need_draw)
            {
                if(Pedo_ui_draw)
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
                else
                {				
                    ui_auto_update = FALSE;

                    //show main logos
                    ClearScreen(NULL);
                    ui_show_lock_1();
                    //show active logo
                    DisplayActive = ap_id[active] - 1; //�¼���Ϣ�����1��ʼ
    				SetAsciiFont(FONT_TYPE_LARGE);
                   	//ResShowMultiString(main_text[DisplayActive], 32,16);//�����ݲ���
                   	ResShowMultiString_Scroll(main_text[DisplayActive], 0xff,48,128,1);    //
                    ResShowPic(inactive_id[DisplayActive], 42,8);
    		  		ui_show_batt(GetBattery());
                    UpdateScreen(NULL);

                    ui_auto_update = TRUE;
                }
                
                need_draw = FALSE;

                while (ap_get_message() != NULL)
                    ;

            }//need draw

            if((ScrollFlag) && (Pedo_ui_draw==0))//matt for scroll   
           	{   
           	    if(scrollcount++>0x0900)
           	    {
           	        scrollcount = 0;
                	//ScrollString(scroll_buf, FALSE,UNICODELANGUAGE,&region_scroll);
                	SetAsciiFont(FONT_TYPE_LARGE);
                    ScrollString(scroll_buf, FALSE,UNICODELANGUAGE,&region_scroll);
                    UpdateScreen(NULL);
    			    SetAsciiFont(FONT_TYPE_DEFAULT);
			    }
           	}
           	
            if (result == RESULT_MAIN)
            {
                int8 rtc_count = 0;
                do
                {
                    key = ap_get_message();
                    if (key == AP_KEY_NULL)
                    {
                    }
                    else if ((key == AP_MSG_RTC) || (key == AP_MSG_CHARGING))
                    {
                        rtc_count++;
                    }
                    else
                    {
                        rtc_count = 0;
                    }

                    //check time out
                    if (rtc_count >= 2)
                    {
                        break;
                    }
                } while ((key & AP_KEY_UP) != AP_KEY_UP);
            }
            key = ap_get_message();
//            keyvalue();
            switch (key)
            {
				case AP_MSG_RTC:
					if(g_light_flag)     //maya090323#1           
                    {
                        ui_show_batt(GetBattery());   
						UpdateScreen(&region);
                    }
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
				
                case AP_KEY_VOLADD | AP_KEY_HOLD:
                case AP_KEY_VOLADD:
                #if(MACHINE_MODEL != 1)
                case AP_KEY_NEXT | AP_KEY_HOLD:
                case AP_KEY_NEXT:
                #endif
                    if(Pedo_ui_draw == 0)
                    {
                        if (active < (item_count - 1))
                        {
                            active++;
                        }
                        else
                        {
                            active = 0;
                        }
                         
                        need_draw = TRUE;
                    }
                    break;

				
                case AP_KEY_VOLSUB | AP_KEY_HOLD:
                case AP_KEY_VOLSUB:
                #if(MACHINE_MODEL != 1)
                case AP_KEY_PREV | AP_KEY_HOLD:
                case AP_KEY_PREV:
                #endif
                    if(Pedo_ui_draw == 0)
                    { 
                        if (active > 0)
                        {
                            active--;
                        }
                        else
                        {
                            active = item_count - 1;
                        }

                        n = 0;
                        need_draw = TRUE;
                    }
                    break;
                
				case AP_KEY_PLAY | AP_KEY_UP:
//              case AP_KEY_MODE | AP_KEY_UP:
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
                        if((g_comval.PedoFlag) && (active == item_count - 3)) 
                        {
                            Pedo_ui_draw = TRUE;
                            Pedo_Swim_flag = FALSE;
                            if(Swim_work_flag) Swim_work_flag = FALSE;
                            need_draw = TRUE;
                        }
                        else if((g_comval.PedoFlag) && (active == item_count - 2)) 
                        {
                            Pedo_ui_draw = TRUE;
                            Pedo_Swim_flag = TRUE;
                            if(Pedo_work_flag) Pedo_work_flag = FALSE;
                            need_draw = TRUE;
                        }
                        else if((g_comval.PedoFlag) && (active == item_count - 1)) 
                        {
                            result = (int16) ap_id[2];  //setting
                        }
                        else
                            result = (int16) ap_id[active];
                    }    
                    break;
                
//              case AP_KEY_MODE | AP_KEY_LONG://����MODE ����test
//              break;

                case AP_KEY_AB | AP_KEY_LONG: 
                    result = RESULT_REC_NOSTART;
        	        break;
				
				#if defined STAYER || defined Greenhouse
				case AP_KEY_NEXT | AP_KEY_LONG: 
				#else
        	    case AP_KEY_NEXT | AP_KEY_UP: 
				#endif
                    if(Pedo_ui_draw)
        			{
        				sKY_SetGsensor(2,0,0,Pedo_Swim_flag);
        				if(Pedo_Swim_flag)
        				{
        	                Swim_total_sec = 0;
        		            Swim_total_step = 0;
							#if defined STAYER || defined Greenhouse
							g_comval.ArmLen = 0;
							#endif
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
        			#if(MACHINE_MODEL == 1)
                    else
        			{
                        if((g_comval.PedoFlag) && (active == item_count - 3)) 
                        {
                            Pedo_ui_draw = TRUE;
                            Pedo_Swim_flag = FALSE;
                            if(Swim_work_flag) Swim_work_flag = FALSE;
                            need_draw = TRUE;
                        }
                        else if((g_comval.PedoFlag) && (active == item_count - 2)) 
                        {
                            Pedo_ui_draw = TRUE;
                            Pedo_Swim_flag = TRUE;
                            if(Pedo_work_flag) Pedo_work_flag = FALSE;
                            need_draw = TRUE;
                        }
                        else if((g_comval.PedoFlag) && (active == item_count - 1)) 
                        {
                            result = (int16) ap_id[2];  //setting
                        }
                        else
                            result = (int16) ap_id[active];
                    } 
        			#endif
        			break;

        	    case AP_KEY_PREV | AP_KEY_UP: 
                    if(Pedo_ui_draw)
        			{
                        need_draw = TRUE;
                        Pedo_ui_draw = FALSE;
                        result = RESULT_NULL;
        			}
        			break;

                case AP_KEY_MODE | AP_KEY_UP: //�̰�MODE
                //case AP_KEY_MODE | AP_KEY_LONG: 
                //case AP_KEY_PREV | AP_KEY_UP: 
        			if(Pedo_ui_draw)
        			{
        			    result = Pedo_menu(Pedo_Swim_flag);
                        need_draw = TRUE;
                        if (result == RESULT_IGNORE)
                        {
                           //Pedo_ui_draw = FALSE;
                  		   result =  0;
                  		}
                  		else if (result == RESULT_RETURN_TO_MAIN)
                  		{
                  		   Pedo_ui_draw = FALSE;
                  		   result =  0;
                  		}
        			}  
        			break;
        	    
                
                default:
                result = ap_handle_hotkey(key);
                if (result == RESULT_REDRAW)
                {
                    result = RESULT_NULL; //remember to clear result
                    need_draw = TRUE;
                    need_draw_all = TRUE;
                }
                if (result == RESULT_MAIN)
                {
                    Pedo_ui_draw = FALSE;
                    need_draw = TRUE;
                    need_draw_all = TRUE;
                    result = RESULT_NULL;
                }
            }
        }

        else if (result == RESULT_POWERON)
        {
            ClearScreen(NULL);
		//������ΪTURE��ҪΪ����standby�ﳤ��PLAY�������Ժ󲻻ᷢUP��Ϣ��
		//����������������ap_message2�оͻᷢUP��Ϣ����
	      		blnInPlayLong=TRUE; 
						draw_logo(FALSE);	//matt090924#1 for power on halt
//            result = result_bak;
						result = RESULT_MAIN;	//matt for power on
						active = 0;
            if(result == RESULT_RECORD) 
						result = RESULT_REC_NOSTART;
            if (result == RESULT_RECORD)
            {
                result = RESULT_REC_NOSTART;
            }
            need_draw = TRUE;
        }
        else
        {
            ResClose(res_fp);
            
            switch (result)
            {
                case RESULT_MUSIC:
				result_bak = result;
                result = ExecAP("music.ap", 0);
                break;

                case RESULT_TELBOOK:
                result_bak = result;
                result = ExecAP("TelBook.ap", 0);//TelBook.ap//music.ap
                break;

                case RESULT_MUSIC_PLAY:
				result_bak = RESULT_MUSIC;
                result = ExecAP("music.ap", 1);
                break;

                case RESULT_VIEW: // DSC����Ƭ
                result_bak = result;
                result = ExecAP("photo.ap", 0);
                break;

                case RESULT_VIEW_PLAY: // DSC����Ƭ
                result_bak = RESULT_VIEW;
                result = ExecAP("photo.ap", 1);
                break;

                case RESULT_READER: // �ı��Ķ�
                result_bak = result;
                result = ExecAP("reader.ap", 0);
                break;

                case RESULT_READER_PLAY: // �ı��Ķ�
                result_bak = RESULT_READER;
                result = ExecAP("reader.ap", 1);
                break;

                case RESULT_MOVIE: //amv play
                result_bak = result;
                result = ExecAP("amv.ap", 0);
                break;

                case RESULT_MOVIE_PLAY: //amv play
                result_bak = RESULT_MOVIE;
                result = ExecAP("amv.ap", 1);
                break;

                case RESULT_RADIO:
				param1=1;
				if (!g_comval.KeyTone)
				{ 
					OpenDAC();
					OpenPA(g_comval.VolumeDef);	   //�а���������PA
				}

			    sfr_bak = SFR_BANK;
			if(!param1)
			{
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
			    }
			} 
				 if (!g_comval.KeyTone) //key tone add by mzh 2007.3.14
                {
					DisableDAC();
		        }
                result_bak = result;
                delay1();
                result = ExecAP("fmradio.ap", 1-param1);
                break;

                case RESULT_REC_NOSTART: //����¼��
               result_bak = result;
                result = ExecAP("record.ap", RESULT_RECORD);
                break;

                case RESULT_RECORD: //��ʼ¼��
                result_bak = RESULT_REC_NOSTART;
                result = ExecAP("record.ap", 1);
                break;

                case RESULT_FMREC_START:
                case RESULT_FMREC_NOSTART:
                result_bak = result;    
                if(To_standby_FMRec == TRUE)
                {
                    ExecAP("fmradio.ap",3); /*�ȿ���FM����*/
                    To_standby_FMRec = FALSE;
                }    
                result = ExecAP("record.ap", result);
                if (result == RESULT_MAIN) //�ж��Ƿ���FM¼������mode���˳�,���򷵻�FM����.
                {
                    result = RESULT_RADIO;
                }
                else if(result == RESULT_STANDBY)
                {
                    To_standby_FMRec = TRUE;
                    ExecAP("fmradio.ap", 2);     //ִ��standbyǰ��FM����standbyģʽ
                }                        
                break;

                case RESULT_SYSTEM:
                result_bak = result;
                result = ExecAP("setting.ap", 0);
                break;

                case RESULT_SYSTEM_ADFU:
                result_bak = result;
                result = ExecAP("setting.ap", 1);
                break;

                case RESULT_TESTER:
                //result = ExecAP("test.ap", 0);
                break;

                case RESULT_UDISK:
                if(g_comval.PedoFlag)   //pedometer
                {
                    sKY_SetGsensor(0,0,0,0);
                    sKY_SetGsensor(0,0,0,1);
                }
                    
                IsUDisk = 1;//FOR TTEST
                usbplugflag = USB_PLUGIN_FINISH;
                
                /*��ʼ���*/
                sKY_ChargeSet(1);
                sKY_ChargeGetandSet();


                earphone_on(TRUE); //FALSE
                    
  
                result = ExecAP("usbdisk.ap", 0);
                usbplugflag = USB_PLUGIN_FINISH;
				if(g_comval.EarphoneFlag==0)
				{
					DisablePA();
				}	
				else
				{
					DisablePADDV();
				}
                if (result == RESULT_MAIN)
                {
                    while (1)
                    {
                        int16 standbymsg;
                        standbymsg = GetSysMsg();
                        if (standbymsg == Msg_KeyShortUp)
                        {
                            break;
                        }
                        if (standbymsg == Msg_KeyLongUp)
                        {
                            break;
                        }
                    }
                    result = RESULT_NULL;
                }
                UdiskFlag = 1; //�����Ƿ�����ѡ�񲥷��б��ѡ��
                break;

                case RESULT_VOICE:
                result_bak = result;
                result = ExecAP("music.ap", 2);
                break;

                case RESULT_VOICE_PLAY:
                result_bak = RESULT_VOICE;
                result = ExecAP("music.ap", 3);
                break;

                case RESULT_STANDBY:
                if(g_comval.PedoFlag)   //pedometer
                {
                    sKY_SetGsensor(0,0,0,0);
                    sKY_SetGsensor(0,0,0,1);
                }
                result = ExecAP("standby.ap", 0);
                standby_flag = 1;
                if (result == RESULT_UDISK) /*zhangxs modify for hold check err  2008.02.26*/
                {
                    usbplugflag = USB_PLUGIN_FINISH;
                    while (ap_get_message() != NULL)
                    {
                        ; /*clear message pool */
                    }
                    ap_sleep(2); /*waitting for hold key check over */
                }
                break;

                case RESULT_FIRST_BOOT:
                if(g_comval.PedoFlag)   //pedometer
                {
                    sKY_SetGsensor(0,0,0,0);
                    sKY_SetGsensor(0,0,0,1);
                }
                result = ExecAP("standby.ap", 1);
                standby_flag = 1;
                break;

                case RESULT_GAME:
                result_bak = result;
                result = ExecAP("game.ap", 0);
                break;

                case RESULT_BROWSER: //�ļ����
                result = ExecAP("browser.ap", 0);
                break;

                default:
                result = NULL;
                break;
            }
            /*֧�ֲ忨ʱ��Ӧ���˳���ж�ز忨����*/
            if (SuppCardFlag == 1)
            {
                DRV_UnInstall(DRV_STG);
            }
            if (!standby_flag)
            {
                while (ap_get_message() != NULL)
                {
                    ;//QAC comment
                }
            }
            else
            {
                standby_flag = 0;
            }
            read_var();
     		if(g_comval.KeyTone > 0)      
            {
				    if ((result != RESULT_FMREC_NOSTART) && (result != RESULT_FMREC_START))		 //�����FM����FM¼�����򲻲���DAC��PA
					{
						OpenDAC();
						OpenPA(g_comval.VolumeDef);	   //�а���������PA  
					}  
				  	    if(g_comval.ClassDFlag)
						{
							earphoneinflag =1;
						}
		
            }
            res_fp = ResOpen("ui30.res");
             
            need_draw_all = TRUE;
            need_draw = TRUE;
            n = 0;
            switch (result)
            //��������AP���صĽ����Ϣ�Ƿ���Ч
            {
                case RESULT_MUSIC:
                case RESULT_MUSIC_PLAY:
                ResultCheck = RESULT_MUSIC;
                break;

                case RESULT_MOVIE:
                case RESULT_MOVIE_PLAY:
                ResultCheck = RESULT_MOVIE;
                break;

                case RESULT_REC_NOSTART: //����¼��
                case RESULT_RECORD:
                case RESULT_FMREC_START:
                case RESULT_FMREC_NOSTART:
                ResultCheck = RESULT_REC_NOSTART;
                break;

                case RESULT_VOICE:               
                ResultCheck = RESULT_VOICE;
                break;
				case RESULT_VOICE_PLAY:
				ResultCheck=0;
				break;
                case RESULT_RADIO:
                ResultCheck = RESULT_RADIO;
                break;

                case RESULT_READER:
                case RESULT_READER_PLAY:
                ResultCheck = RESULT_READER;
                break;

                case RESULT_VIEW:
                case RESULT_VIEW_PLAY:
                ResultCheck = RESULT_VIEW;
                break;

                case RESULT_TELBOOK:
                case RESULT_GAME:
                case RESULT_BROWSER:
                case RESULT_SYSTEM:
                case RESULT_TESTER:
                ResultCheck = result;
                break;

                default:
                ResultCheck = 0; //���ж�
                break;
            } //end of swicth
            if (ResultCheck != 0)
            {
                if (!check_ap_result((int8) ResultCheck, ap_id, (int8) item_count)) //�����Ϣ������Ч��AP,����main
                {
					if(ResultCheck != RESULT_REC_NOSTART)
						result = RESULT_MAIN;
                }
            }
        }//result != 0
    }//while(1)
    //never run here
#endif    
   
}


/********************************************************************************
 * Description : ����AP���ص���Ϣ�Ƿ�����Ч��������
 *
 * Arguments   :    check_result��Ҫ����AP�����Ϣ
 *                  result_ap:   ��� AP�����Ϣ������ָ��
 *                  ap_total:    ��ЧAP������
 * Returns     :   TRUE�� AP �����Ϣ��Ч
 *                  FALSE��AP�����Ϣ��Ч
 *
 * Notes       : NONE
 *
 ********************************************************************************/
bool check_ap_result(int8 check_result, int8 *result_ap, int8 ap_total)
{
    uint8 i;
    for (i = 0; i < (ap_total - 1); i++)
    {
        if (check_result == (*result_ap))
        {
            return TRUE;
        }
        result_ap++;
    }
    return FALSE;
}

/*��PA����*/
void OpenPA(uint8 volume)
{
    uint16 i;
    pa_t paattr;
    paattr.pa_in.dacin = 0;
    paattr.pa_in.fmin = 0;
    paattr.pa_in.linein = 0;
    paattr.pa_in.micin = 0;
    paattr.volume = (int8) volume;
    if (g_comval.EarphoneFlag == 0)
    {
        EnablePA(&paattr);
    }
    else
    {
        EnablePADDV(&paattr);
    }
    for (i = 0; i < 2000; i++)
    {
    }
    paattr.pa_in.dacin = 1;
    if (g_comval.EarphoneFlag == 0)
    {
        EnablePA(&paattr);
    }
    else
    {
        EnablePADDV(&paattr);
    }

    for (i = 0; i < 2000; i++)
    {
    }
}


void OpenDAC(void)
{
	dac_t dacattr;
    dacattr.dacselect = 0;
    dacattr.rate = 22;
	
    EnableDAC(&dacattr);	
}  
