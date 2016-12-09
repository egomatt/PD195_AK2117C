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

extern uint32 systemtime;
extern uint32 gwtime;

const region_t clock_region =
{ 34, 40, 60, 16 };

//days of month
const uint8 month_days[] =
{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/*const WORD TNUM_PIC[] =
{ TNUMBER0, TNUMBER1, TNUMBER2, TNUMBER3, TNUMBER4, TNUMBER5, TNUMBER6, TNUMBER7, TNUMBER8, TNUMBER9 };
const uint8 ActiveposX[] =
{0,DAY,MONTH,YEAR,59,50,33,24}//
const uint8 ActiveposY[] =
{0,32,16,16,0,0,0,0}*/
//时间图片
/*
 ********************************************************************************
 * Description : 录音时间设定界面
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
void showmainpic(time_t time)
{
  uint8 num_high,num_low;
  uint8 i=0;
  uint8 PMflag=0; 
  if(time.hour>12)
  	{
  	 time.hour=time.hour-12;
     PMflag=1;
  	}
  
  if(PMflag==1)
  ResShowPic(TIMEPM,0, 0);
  else
  ResShowPic(TIMEAM,0, 0);	
 
  num_high=time.hour/10;
  num_low =time.hour%10;
  ResShowPic(TNUM_PIC[num_high],24+i*9, 0);
  i++; 
  ResShowPic(TNUM_PIC[num_low],24+i*9, 0);
  i++;
  num_high=time.second/10;
  num_low =time.second%10;
  ResShowPic(TIMEDOT,24+i*9,0);
  ResShowPic(TNUM_PIC[num_high],24+i*9+8, 0);
  i++;
  ResShowPic(TNUM_PIC[num_low],24+i*9+8, 0);

  
}
#endif
#if 1
uint16 show_clock(void)
{
	return 0;
}
#else
uint16 show_clock(void)
{
    uint8 bSettime = 0;
    uint8 bTimeAdd = 0xFF;
    date_t date =
    { 2004, 1, 1 };
    time_t time =
    { 12, 00, 00 };
    uint8 buf[11]; //date &  time
    uint8 key;
    uint8 set_what = 0; //0, 非设定状态, 1,year, 2,month...
    BOOL show_what = TRUE; //true, 显示, false, 不显示
    uint8 days; //本月的天数
    BOOL result;
    BOOL need_draw = TRUE;
    BOOL need_draw_backgroud = TRUE;
    BOOL set_ok = FALSE; //设置完成
    BOOL adjusting = FALSE; //正在设置,不要闪烁
    uint8 PMflag=0;
    kval_t kval;
    result = TM_GetDate(&date, 0);
    result = TM_GetTime(&time);	
	
    while (1)
    {
        //显示背景
        if (need_draw_backgroud)
        {
            ClearScreen(NULL);
            ui_auto_update = FALSE;
            ui_auto_update = TRUE;
            need_draw_backgroud = FALSE;
        }

        //显示时钟
        if (need_draw)
        {
            ClearScreen(&clock_region);
            if (bTimeAdd == 0xff)
            {
                result = TM_GetDate(&date, 0);
                result = TM_GetTime(&time);
                //系统时间没作2098限制，ap层限制
                if (date.year >= 2099) //2000~2098, 不定为2099是为了方便处理从2098跳2099能回到00
                {
                    date.year = 2000;
                    TM_SetDate(&date, 0);
                }
            }
           
            if (set_what == 1 && !show_what)
            {
                buf[0] = ' ';
                buf[1] = ' ';
                buf[2] = ' ';
                buf[3] = ' ';
            }
            else
                itoa4(date.year, buf);
            buf[4] = '/';
            if (set_what == 2 && !show_what)
            {
                buf[5] = ' ';
                buf[6] = ' ';
            }
            else
                itoa2(date.month, buf + 5);
            buf[7] = '/';
            if (set_what == 3 && !show_what)
            {
                buf[8] = ' ';
                buf[9] = ' ';
                buf[10] = 0;
            }
            else
            itoa2(date.day, buf + 8);
			
            SetTextPos((SCR_WIDTH - (BYTE) get_text_width(buf)) / 2, 40);
            PutS(buf, -1);
            //show 19:17:50
            if (set_what == 4 && !show_what)
            {
                buf[0] = ' ';
                buf[1] = ' ';
            }
            else
                itoa2(time.hour, buf);
            buf[2] = ':';
            if (set_what == 5 && !show_what)
            {
                buf[3] = ' ';
                buf[4] = ' ';
            }
            else
                itoa2(time.minute, buf + 3);
            buf[5] = ':';
            if (set_what == 6 && !show_what)
            {
                buf[6] = ' ';
                buf[7] = ' ';
                buf[8] = 0;
            }
            else
                itoa2(time.second, buf + 6);
            SetTextPos((SCR_WIDTH - (BYTE) get_text_width(buf)) / 2, 48);
            PutS(buf, -1);
            UpdateScreen(NULL);

            need_draw = FALSE;
        }

        //handle msg
        key = ap_get_message();
        switch (key)
        {
            case AP_MSG_CHARGING:
            case AP_MSG_RTC:
            show_what = !show_what;
            need_draw = TRUE;
            break;
            //case AP_KEY_VOL | AP_KEY_UP:
            case AP_KEY_VOLADD | AP_KEY_UP:
            if (set_what >= 8)
                set_what = 0;
            else
                set_what++;
            bTimeAdd = 0;
            break;
            case AP_KEY_VOLSUB | AP_KEY_UP:
            if (set_what == 0)
                set_what = 8;
            else
                set_what--;
            bTimeAdd = 0;
            break;
            case AP_KEY_NEXT: //in setting clock
            case AP_KEY_NEXT | AP_KEY_HOLD:
            adjusting = TRUE;
            bSettime = 0xff;
            switch (set_what)
            {
                case 1: //year
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
                case 3: //day
                days = month_days[date.month - 1]; //本月天数
                if ((date.year % 4 == 0) && (date.month == 2))
                    days++; //闰年+1
                if (date.day < days)
                    date.day++;
                else
                    date.day = 1;
                break;
                case 4: //hour
                if (time.hour < 23)
                    time.hour++;
                else
                    time.hour = 0;
                break;
                case 5: //min
                if (time.minute < 59)
                    time.minute++;
                else
                    time.minute = 0;
                break;
                case 6: //sec
                if (time.second < 59)
                    time.second++;
                else
                    time.second = 0;
                break;
                default: //no in setting
                bSettime = 0;
                break;
            } //set_what

            //clear the key buffer
            do
            {
                key = ap_get_message();
                if (key == (AP_KEY_NEXT | AP_KEY_UP))
                {
                    adjusting = FALSE;
                    break;
                }
            } while (key != 0);
            need_draw = TRUE;
            break;

            case AP_KEY_PREV: //in setting clock
            case AP_KEY_PREV | AP_KEY_HOLD:
            adjusting = TRUE;
            bSettime = 0xff;
            switch (set_what)
            {
                case 1: //year
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
                case 3: //day
                days = month_days[date.month - 1]; //本月天数
                if ((date.year % 4 == 0) && (date.month == 2))
                    days++; //闰年+1
                if (date.day > 1)
                    date.day--;
                else
                    date.day = days;
                break;
                case 4: //hour
                if (time.hour > 0)
                    time.hour--;
                else
                    time.hour = 23;
                break;
                case 5: //min
                if (time.minute > 0)
                    time.minute--;
                else
                    time.minute = 59;
                break;
                case 6: //sec
                if (time.second > 0)
                    time.second--;
                else
                    time.second = 59;
                break;
                default: //no in setting
                bSettime = 0;
                break;
            } //set_what

            //clear the key buffer
            do
            {
                key = ap_get_message();
                if (key == (AP_KEY_PREV | AP_KEY_UP))
                {
                    adjusting = FALSE;
                    break;
                }
            } while (key != 0);

            need_draw = TRUE;

            break;

            case AP_KEY_NEXT | AP_KEY_UP:
            case AP_KEY_PREV | AP_KEY_UP:
            adjusting = FALSE;

            break;
            case AP_MSG_WAIT_TIMEOUT: //子菜单界面在8秒后返回系统功能菜单, add by ccm 2004/07/15
            //case AP_KEY_PLAY | AP_KEY_UP:
            bSettime = 0;
            case AP_KEY_MODE | AP_KEY_UP:
            //return 0;        //退出

            key = 0;
            set_ok = TRUE;

            break;
            default:
            key = ap_handle_hotkey(key);
            if (key == RESULT_REDRAW)
            {
                need_draw_backgroud = TRUE;
                need_draw = TRUE;
            }
            else if (key != 0)
            {
                set_ok = TRUE;
            }
        } //switch(key)

        //set ok?
        if (set_ok)
            break;
    }//while(1)
    if (bSettime == 0)
    {
        return key;
    }
    else if (key != 0) //不是按mode退出的都不保存设置，如USB线插上等
    {
        return key;
    }

    SetAsciiFont(FONT_TYPE_DEFAULT);

    //re-check the date/time format
    days = month_days[date.month - 1];
    if ((date.year % 4 == 0) && (date.month == 2))
        days++; //润年
    if (date.day > days)
        date.day = days;

    //save date & time
    result = TM_SetDate(&date, 0);
    //DUMP("set date", 0);
//    ASSERT(result);
    result = TM_SetTime(&time);
    //DUMP("set time", 0);
//    ASSERT(result);

    //将systemtime 和 gwtime写回到VRAM中
    VMRead(&kval, VM_KERNEL, sizeof(kval_t));
    kval.systemtime = systemtime; //全局变量
    kval.gwtime = gwtime; //全局变量
    VMWrite(&kval, VM_KERNEL,sizeof(kval));

    return key;
}
#endif
