/*
*******************************************************************************
*                ACTOS AP
*        system setting ap of AS211A
*
*        (c) Copyright, Actions Co,Ld.
*             All Right Reserved
*
*  $Id: main.c,v 1.2 2010/03/23 10:48:01 Mars Exp $
*******************************************************************************
*/
#include "actos.h"
#include "ap_common.h"
#include "ap_setting.h"

typedef uint16 (*show_func)(void);

#pragma name(AP_MAIN)
uint16 UpDisplayFlag = 0; //对应于更新相关选项
comval_t g_comval;
vm_cfg_t  g_modifyval;  
setting_vars_t g_setting_vars;
slider_t slider;
progress_t progress;
uint8 charge_flag; 
function_menu_t menu;

uint8 PMflag=0;
date_t date ;
time_t time ;

//usb模式 时间设置 省电设置 语言设置 显示设置 录音设置 系统工具 发射 共8个主菜单
//#if (PETOMETER_USE)
#define      MAX_SYSMENUNUM_PEDO   6    //pedometer
const uint16 main_text_pedo[MAX_SYSMENUNUM_PEDO]= 
{
    POWER,LANGUAGE,DISPLAY,AUTOLOCK,/*RECORDSETTING,*/SYSTEMTOOLS,PERSONAL
};
//#else
#define      MAX_SYSMENUNUM   5 
const uint16 main_text[MAX_SYSMENUNUM]= 
{
    POWER,LANGUAGE,DISPLAY,AUTOLOCK,/*RECORDSETTING,*/SYSTEMTOOLS
};
//#endif
const show_func entries[MAX_SYSMENUNUM_PEDO] =
{  
  show_powerdown,show_language,show_display,show_lock,/*show_record,*/system_tool,show_gusensou
};

const uint16 record_text[1]= 
{
  SETRQUA/*,SETRVAD,SETRAGC*/  
}; 
const uint16 record_qua[3]= 
{
  SETRLTIM ,SETRNOMA ,SETRHQUA 
}; 
const char FwcfgName[] = "FWCFG.BIN"; 
/*
********************************************************************************
* Description : 读入系统设定变量
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
    uint16 result;
	
	#if defined STAYER || defined Greenhouse
	BOOL flag = FALSE;
    if(g_comval.ArmLen == 0)
    {
        flag = TRUE;
    }
	#endif
	
    result = VMRead(&g_comval, VM_SYSTEM, sizeof(g_comval));
	
	#if defined STAYER || defined Greenhouse
	if(flag)
    {
       g_comval.ArmLen = 0;
    }					
	#endif
    ap_message_init(&g_comval);    
    result = VMRead(&g_modifyval, VM_AP_FWCFG,sizeof(vm_cfg_t)); 
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

int16 main(int16 param)
{  
    int16 result;
    int16 menu_active=0;    //main menu's active number,
    BOOL mc_result;

	param = param;
    mc_result= DRV_ActiveUD(0,MODE_SLOW);
    /*先读VM，在打开资源文件，保证语言的索引表正确*/
    read_var();
    //ClearScreen(NULL);
    res_fp = ResOpen("ui30.res");
    while(1)
    {
        if(g_comval.PedoFlag)
        {
            menu.total = MAX_SYSMENUNUM_PEDO;             //"+2":for MP3 recorder by ccm
            menu.string = main_text_pedo;
        }
        else
        {
            menu.total = MAX_SYSMENUNUM;             //"+2":for MP3 recorder by ccm
            menu.string = main_text;
        }
        
        menu.menuhead = MAIN5;
        menu.active = menu_active;
        menu.string_id = g_comval.langid;
        //ui_auto_select = TRUE; 
        result = ui_function_menu(&menu, NULL);
        //ui_auto_select = FALSE;
        if(result == RESULT_IGNORE)
        {     
            result = 0;
            break;
        }
        /*check hotkey*/
        else if(result != 0)
        {    
             break;
        }
        else if(menu.active == menu.total - 1)
		{
		     ;//break;    //user choose exit.
        }
         
        menu_active = menu.active;
        result = entries[menu.active](); 
        if(result == RESULT_IGNORE)
        {
            result = 0;
        }     
        if(result != 0) 
        {
            break;
        }    
    }//while(1)
    ResClose(res_fp);
    mc_result = VMWrite(&g_comval, VM_SYSTEM, sizeof(g_comval));
    return result;
}
//============录音设置
uint16 show_record(void)
{  
    uint16 result;
	uint16 menu_active=0;
	/*
	while(1)
	{
	    menu.total =1;            
	    menu.menuhead =STRECOR;
	    menu.string = record_text;
	    menu.active = menu_active;
	    menu.string_id = g_comval.langid;
	    ui_auto_select = FALSE; 
	    result = ui_function_menu(&menu, NULL);
	    ui_auto_select = TRUE;
		menu_active=menu.active;
		if(result!=0)
		{
		   	return result;
		}
		else
		{ 
			  switch(menu_active)
			  {
					case 0:
					    menu.total =3;            
				        menu.menuhead =STRECOR;
				        menu.string = record_qua;
				        menu.active = g_comval.rec_bitrate;
				        menu.string_id = g_comval.langid;
				        ui_auto_select = FALSE; 
				        result = ui_function_menu(&menu, NULL);
				        ui_auto_select = TRUE;
					    if(result==RESULT_IGNORE)
					        break;
					    else if (result!=0)
					        return result;
					    g_comval.rec_bitrate=menu.active;
					break;
                    
					default:
						break;		
			  }

		}
	}
	*/

	menu.total =3;            
	menu.menuhead =STRECOR;
	menu.string = record_qua;
	menu.active = g_comval.rec_bitrate;
	menu.string_id = g_comval.langid;
	//ui_auto_select = TRUE; 
	result = ui_function_menu(&menu, NULL);
	//ui_auto_select = FALSE;
	if (result!=0)
	    return result;
	g_comval.rec_bitrate=menu.active;
					    
  return 0;
}
