/*
*******************************************************************************
*                ACTOS AP
*        system setting ap of sdk 3.0, ui part,
*    this function is too big to fit in a bank with others
*
*        (c) Copyright, Actions Co,Ld.
*             All Right Reserved
*
*  $Id: ui_clock.c,v 1.1.1.1 2006/05/22 03:23:59 selina Exp $
*******************************************************************************
*/
#include "actos.h"
#include "enhanced.h"
#include "ap_common.h"
#include "ap_setting.h"

    
#define POSITION_CLOCK_BG    30

#pragma name(AP_UI_CLK)
#if(SWIM_USE)
#if defined STAYER || defined Greenhouse
const uint16 Menu_Pudo[5]={STRIDE,WEIGHT,COUNTSET,POOLLENGTH,MODE_SEL};
#else
const uint16 Menu_Pudo[5]={STRIDE,WEIGHT,ARMLENGTH,POOLLENGTH,UNIT};
#endif
#else
const uint16 Menu_Pudo[3]={STRIDE,WEIGHT,UNIT};
#endif
const uint16 Menu_unit[2]={METRIC,IMPERIAL};
#ifdef STAYER
const uint16 Mode_unit[2]={MODE_W,MODE_R};
#else
const uint16 Mode_unit[3]={MODE_W,MODE_J,MODE_R};
#endif

extern vm_cfg_t  g_modifyval;

#if 0
extern uint32 systemtime;
extern uint32 gwtime;
extern uint8 PMflag;
extern date_t date ;
extern time_t time ;

const region_t clock_region =
{ 0, 32, 64, 16 };

//days of month
const uint8 month_days[] =
{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };


const WORD TNUM_PIC[] =
{ TNUMBER0, TNUMBER1, TNUMBER2, TNUMBER3, TNUMBER4, TNUMBER5, TNUMBER6, TNUMBER7, TNUMBER8, TNUMBER9 };

const WORD TNUM_PIC[] = {SYSFONT_SMALL_0,SYSFONT_SMALL_1,SYSFONT_SMALL_2,SYSFONT_SMALL_3,SYSFONT_SMALL_4};


const uint8 ActiveposX[] =
{ 0, 24, 40, 40, 55, 46, 29, 20 };//X����
const uint8 ActiveposY[] =
{ 0, 32, 32, 16, 0, 0, 0, 0 };//y����
#endif
/*
 ********************************************************************************
 * Description : ¼��ʱ���趨����
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */

#if 0
void Showtime(uint8 flashflag ,uint8 active)
{
	;
}
void Showdate(uint8 flashflag ,uint8 active)
{
	;
}
uint16 show_clock(void)
{
	return 0;
}


void Showtime(uint8 flashflag ,uint8 active)
{ 
  if(flashflag)
  	{ 
        if(PMflag==1)
        ResShowPic(TIMEPM,0, 0);
        else
        ResShowPic(TIMEAM,0, 0);
	  	ResShowPic(TNUM_PIC[time.minute%10],55, 0);
	  	ResShowPic(TNUM_PIC[time.minute/10],46, 0);
		ResShowPic(TIMEDOT,38,0); 
	  	ResShowPic(TNUM_PIC[(time.hour)%10],29, 0);
	  	ResShowPic(TNUM_PIC[(time.hour)/10],20, 0);
	   
  	}
    else
    {
     if(active==0)
	 ResShowPic(AM_CLEAR,0, 0);
	 else
  	 ResShowPic(NUMCLEAR,ActiveposX[active],ActiveposY[active]);
    }
}

void Showdate(uint8 flashflag ,uint8 active)
{ 
    uint8 buffer[10];
	region_t region_tmp={0,16,64,32};
	SetAsciiFont(FONT_TYPE_LARGE);
    itoa4(date.year,buffer);
	buffer[4]='/';
	itoa2(date.month,&buffer[5]);
	buffer[7]='/';
	itoa2(date.day,&buffer[8]);
    if(!flashflag)
  	{ 
  	  switch(active)
  	  {
  	  case 1:
         buffer[8]=' ';
		 buffer[9]=' ';
		break;
	  case 2:
	     buffer[5]=' ';
		 buffer[6]=' ';
		break;
	  case 3:
	     buffer[2]=' ';
		 buffer[3]=' ';
		break;
	  default:
	  	break;
  	  }
  	}
	SetTextPos(16, 16);
	PutS(buffer, 4);
	SetTextPos(8, 32);
	PutS(&buffer[4], 6);
	UpdateScreen(&region_tmp); 
	SetAsciiFont(FONT_TYPE_DEFAULT);
}

uint16 show_clock(void)
{
    uint8 bSettime = 0;
    uint8 key;
    uint8 set_what=0; //0, ���趨״̬, 1,year, 2,month...
    uint8 oldset_what=0;
    BOOL show_what = TRUE; //true, ��ʾ, false, ����ʾ
    uint8 days; //���µ�����
    BOOL result;
    BOOL need_draw = TRUE;
    BOOL set_ok = FALSE; //�������
    BOOL adjusting = FALSE; //��������,��Ҫ��˸
    kval_t kval;
	bool chargeflag=FALSE;
	ClearScreen(NULL);
	UpdateScreen(NULL);
	TM_GetDate(&date, 0);
    TM_GetTime(&time);
	if(time.hour>12)
	{
	  time.hour-=12;
      PMflag=1;
	}
	if(time.hour==0)
	time.hour=12;	
	
	ui_auto_update=TRUE;
    while (1)
    {  
        if(need_draw)
        {
             
            Showtime(show_what,set_what);
		    Showdate(show_what,set_what);
			need_draw=FALSE;
				
        }
		if(chargeflag)
		{
			Showtime(TRUE,oldset_what);
			Showdate(TRUE,oldset_what);
			chargeflag=FALSE;
		}
        key = ap_get_message();
        switch (key)
        {
            case AP_MSG_CHARGING:
            case AP_MSG_RTC:
            show_what = !show_what;
            need_draw = TRUE;
            break;
            case AP_KEY_PLAY: 
            case AP_KEY_PLAY| AP_KEY_UP:
			oldset_what=set_what;
            if (set_what >= 7)
                set_what = 0;
            else
                set_what++;
			need_draw = TRUE;
			chargeflag=TRUE;
            break;
		    case AP_KEY_NEXT :
		    case AP_KEY_NEXT | AP_KEY_HOLD:
            switch (set_what)
            {   
                case 0:
				PMflag=!PMflag;
				break;
                case 3: //year
                if (date.year < 2098)
                    date.year++;
                else
                    date.year = 2000;
                break;
                case 2: //month
                if (date.month < 12)
                    date.month++;
                else
                    date.month = 1;
                break;
                case 1: //day
                days = month_days[date.month - 1]; //��������
                if ((date.year % 4 == 0) && (date.month == 2))
                    days++; //����+1
                if (date.day < days)
                    date.day++;
                else
                    date.day = 1;
                break;
				case 7: //hour
                if ((time.hour /10)< 1)
                    time.hour+=10;
                if(time.hour>12)
					time.hour=12;
                break;
				
                case 6: //hourlow
                if (time.hour < 12)
                    time.hour++;
                else
                    time.hour = 1;
                break;
				
				case 5: //minhigh
				if(time.minute/10<5)
				 	time.minute+=10;
				 break; 
                case 4: //min
                if (time.minute < 59)
                    time.minute++;
                else
                    time.minute = 0;
                break;
                default: //no in setting
                bSettime = 0;
                break;
            } //set_what
            do
            {
                key = ap_get_message();
                if (key == (AP_KEY_NEXT | AP_KEY_UP))
                {
                    break;
                }
            } while (key != 0);
            need_draw = TRUE;
            break;

            case AP_KEY_PREV: //in setting clock
            case AP_KEY_PREV | AP_KEY_HOLD:
            bSettime = 0xff;
            switch (set_what)
            {    
               case 0:
                PMflag=!PMflag;
				break;
                case 3: //year
                if (date.year > 2000)
                    date.year--;
                else
                    date.year = 2098;
                break;
                case 2: //month
                if (date.month > 1)
                    date.month--;
                else
                    date.month = 12;
                break;
                case 1: //day
                days = month_days[date.month - 1]; //��������
                if ((date.year % 4 == 0) && (date.month == 2))
                    days++; //����+1
                if (date.day > 1)
                    date.day--;
                else
                    date.day = days;
                break;
				case 7: //hourhigh
                if ((time.hour/10)>0)
                    time.hour-=10; 
                break;
                case 6: //hourlow
                if (time.hour > 1)
                    time.hour--;
                else
                    time.hour =12;
                break;
				
				case 5: //minhigh
				 if(time.minute/10>0)
				 time.minute-=10;
				  break;
                case 4: //minlow
                if (time.minute > 0)
                    time.minute--;
                else           	
                    time.minute=59;

                break;
                default: //no in setting
                bSettime = 0;
                break;
            } //set_what
            do
            {
                key = ap_get_message();
                if (key == (AP_KEY_PREV | AP_KEY_UP))
                {
                    break;
                }
            } while (key != 0);
             need_draw = TRUE;
            break;
			
            case AP_KEY_MODE | AP_KEY_UP:
            key = 0;
            set_ok = TRUE;
            break;
            default:
            key = ap_handle_hotkey(key);
            if (key == RESULT_REDRAW)
            {
          
                need_draw = TRUE;
            }
            else if (key != 0)
            {
                set_ok = TRUE;
            }
        } //switch(key)
        if (set_ok)
            break;
    }//while(1)
    if (bSettime == 0)
    {
        return key;
    }
    else if (key != 0) //���ǰ�mode�˳��Ķ����������ã���USB�߲��ϵ�
    {
        return key;
    }

    SetAsciiFont(FONT_TYPE_DEFAULT);
	
	if(PMflag)
	{ 
	   if(time.hour==12)
	    time.hour=0;
	   else
	   	time.hour+=12;
	}
    days = month_days[date.month - 1];
    if ((date.year % 4 == 0) && (date.month == 2))
        days++; //����
    if (date.day > days)
        date.day = days;
    result = TM_SetDate(&date, 0);
    result = TM_SetTime(&time);
    //��systemtime �� gwtimeд�ص�VRAM��
    VMRead(&kval, VM_KERNEL, sizeof(kval_t));
    kval.systemtime = systemtime; //ȫ�ֱ���
    kval.gwtime = gwtime; //ȫ�ֱ���
    VMWrite(&kval, VM_KERNEL,sizeof(kval));

    return key;
}
#endif 

#if defined STAYER || defined Greenhouse
uint16 show_mode(void)
{
	uint8 result;
	uint8 tmp;

	tmp = g_comval.Record_Mode;
	menu.total = sizeof(Mode_unit)/2;;        
	menu.menuhead = STDISP;//��ʾ����
	menu.string = Mode_unit;
	menu.active = g_comval.Record_Mode;
	menu.string_id = g_comval.langid;
	//ui_auto_select = TRUE;  
	result = ui_function_menu(&menu, NULL);
	//ui_auto_select = FALSE;
	g_comval.Record_Mode = menu.active;
	return result;
}
#endif


uint16 show_unit(void)
{
	uint8 result;
	uint8 tmp;

	tmp = g_comval.PedoUnit;
	menu.total = 2;        
	menu.menuhead = STDISP;//��ʾ����
	menu.string = Menu_unit;
	menu.active = g_comval.PedoUnit;
	menu.string_id = g_comval.langid;
	//ui_auto_select = TRUE;  
	result = ui_function_menu(&menu, NULL);
	//ui_auto_select = FALSE;
	g_comval.PedoUnit=menu.active;
	if(tmp != g_comval.PedoUnit)
	{
        if(tmp)
        {
            g_comval.Stride = (uint32)g_comval.Stride*254/100;     //inch->cm
            g_comval.Weight = (uint32)g_comval.Weight*454/1000;    //lb->kg
            g_comval.ArmLen = (uint32)g_comval.ArmLen*254/100;     //inch->cm
            g_comval.PoolLen = (uint32)g_comval.PoolLen*914/1000;  //yard->m
        }
        else
        {
            g_comval.Stride = (uint32)g_comval.Stride*394/1000;     //cm->inch
            g_comval.Weight = (uint32)g_comval.Weight*2205/1000;    //kg->lb
            g_comval.ArmLen = (uint32)g_comval.ArmLen*394/1000;     //cm->inch
            g_comval.PoolLen = (uint32)g_comval.PoolLen*1094/1000;  //m->yard
        }
	}
	return result;
}

uint16 show_SW(uint8 mode)
{
	uint8 need_draw = TRUE;
	uint16 num,Max,Min;
	uint8 key;
	
	if(mode==1)
	{
		num = g_comval.Weight;	
		Max = WEIGHT_MAX;//g_modifyval.Weight.Max;
		Min = g_modifyval.Weight.Min;
	}
	else if(mode==0)
	{
		num = g_comval.Stride;	
		Max = g_modifyval.Stride.Max;
		Min = g_modifyval.Stride.Min;
	}
	else if(mode==2)
	{
		num = g_comval.ArmLen;	
		Max = g_modifyval.ArmLen.Max;
		Min = g_modifyval.ArmLen.Min;
	}
	else if(mode==3)
	{
		num = g_comval.PoolLen;	
		Max = g_modifyval.PoolLen.Max;
		Min = g_modifyval.PoolLen.Min;
	}
	
	while(1)
	{
		if(need_draw == TRUE)
		{
			ClearScreen(NULL);
			if(mode==1)
				ResShowMultiString(WEIGHT,32,16);
			else if(mode==0)
				ResShowMultiString(STRIDE,32,16);
		    else if(mode==2)
		        #if defined STAYER || defined Greenhouse
                ResShowMultiString(COUNTSET,32,16);
		        #else
				ResShowMultiString(ARMLENGTH,32,16);
				#endif
		    else if(mode==3)
				ResShowMultiString(POOLLENGTH,32,16);
			ResShowPic(YIM_TIME_NUMBER_0+num/100,32,32);
			ResShowPic(YIM_TIME_NUMBER_0+num/10%10,32+9,32);
			ResShowPic(YIM_TIME_NUMBER_0+num%10,32+18,32);
			
			SetAsciiFont(FONT_TYPE_LARGE);
			SetTextPos(64, 32);
			if(mode==1)
			{
			    if(!g_comval.PedoUnit)
				    PutS("kg", -1);
				else
				    PutS("lb", -1);
		    }
			else if(mode==0)
			{
			    if(!g_comval.PedoUnit)
				    PutS("cm", -1);
				else
				    PutS("in", -1);
		    }
		    else if(mode==2)
		    {
			    if(!g_comval.PedoUnit)
					#ifdef STAYER
					PutS("cnt", -1);
					#elif defined Greenhouse
					ResShowPic(IM_CYCLE,64,32);
					#else
				    PutS("cm", -1);
					#endif
				else
				    PutS("in", -1);
		    }
		    else if(mode==3)
		    {
			    if(!g_comval.PedoUnit)
				    PutS("m", -1);
				else
				    PutS("yd", -1);
		    }
				
			need_draw = FALSE;
			UpdateScreen(NULL);
		}
		key = ap_get_message();

        switch (key)
        {
            //case AP_MSG_WAIT_TIMEOUT:   //�Ӳ˵�������8��󷵻�ϵͳ���ܲ˵�, add by ccm 2004/07/15
            case AP_KEY_PLAY | AP_KEY_UP:
			#if(MACHINE_MODEL==1)
            case AP_KEY_NEXT| AP_KEY_UP: 
			#endif
				if(mode==1)
					g_comval.Weight = num;
				else if(mode==0)
					g_comval.Stride = num;
				else if(mode==2)
					g_comval.ArmLen = num;
			    else if(mode==3)
					g_comval.PoolLen = num;
            case AP_KEY_MODE | AP_KEY_UP:  
			#if(MACHINE_MODEL==1)  
            case AP_KEY_PREV| AP_KEY_UP: 
			#endif
            	return 0; //�˳�
            //break;
            //case AP_KEY_VOLADD:
			#if(MACHINE_MODEL==1)
			case AP_KEY_VOLADD | AP_KEY_UP:
			#else
            case AP_KEY_NEXT | AP_KEY_UP:
			#endif
            num++;
            if(num > Max)
			{
				num = Min;
			}
			need_draw = TRUE;
            break;
            //case AP_KEY_VOLADD:
			#if(MACHINE_MODEL==1)
			case AP_KEY_VOLADD | AP_KEY_HOLD:
			#else
            case AP_KEY_NEXT | AP_KEY_HOLD:
			#endif
			num+=10;
            if(num > Max)
			{
				num = Min;
			}
			need_draw = TRUE;
            break;
            //case AP_KEY_VOLSUB:
			#if(MACHINE_MODEL==1)
			case AP_KEY_VOLSUB | AP_KEY_UP:
			#else
			case AP_KEY_PREV | AP_KEY_UP:
			#endif
			if(num>0) num--;
			else
			{
			    num = Max;
			}
            if(num < Min)
			{
				num = Max;
			}
			need_draw = TRUE;
            break;
            //case AP_KEY_VOLSUB:
			#if(MACHINE_MODEL==1)
			case AP_KEY_VOLSUB | AP_KEY_HOLD:
			#else
			case AP_KEY_PREV | AP_KEY_HOLD:
			#endif
			if(num<10) num=0;
            else       num-=10;
            if(num < Min)
			{
				num = Max;
			}
			need_draw = TRUE;
            break;
  
            default:
           	key = ap_handle_hotkey(key);
            if (key == RESULT_REDRAW)
                need_draw = TRUE;
            else if (key != 0)
                return key;
        }//switch(key)						
	}
	return  0;
}

uint16 show_gusensou(void)
{
		uint8 result;
		uint8 active = 0;

		while(1)
	 	{
		     menu.total = sizeof(Menu_Pudo)/2;        
		     menu.menuhead = STDISP;//��ʾ����
		     menu.string = Menu_Pudo;
		     menu.active = active;
		     menu.string_id = g_comval.langid;
		     //ui_auto_select = TRUE;  
		     result = ui_function_menu(&menu, NULL);
		     //ui_auto_select = FALSE;
			 active=menu.active;
			 if(result!=NULL)
			 {
			   	return  result;
			 }
			 else
		 	 {
                switch(menu.active)
                {
			        case 0:
					  	 result = show_SW(0);
						 if(result!=NULL)
						 return result;
			             break; 
			             
					case 1:
					  	 result = show_SW(1);
						 if(result!=NULL)
						 return result;
					  	 break;

					#if(SWIM_USE)

                    case 2:
					  	 result = show_SW(2);
						 if(result!=NULL)
						 return result;
			             break; 
			             
					case 3:
					  	 result = show_SW(3);
						 if(result!=NULL)
						 return result;
					  	 break;
					  	 
					case 4:
						 #if defined STAYER || defined Greenhouse
						 result=show_mode();
						 #else
					 	 result=show_unit();
						 #endif
						 if(result!=NULL)
						 return result;
			             break; 
			             
					#else
					  	 
				    case 2:
					 	 result=show_unit();
						 if(result!=NULL)
						 return result;
			             break; 

			        #endif
			        
			        default:
					  	break;
                	}
		 	 }
			 		 	
	 	 }	
	 return  0;
}

