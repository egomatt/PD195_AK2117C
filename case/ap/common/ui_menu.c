/*
*******************************************************************************
*                ACTOS AP
*        ap common lib, menu ui file 
*
*        (c) Copyright, Actions Co,Ld.                             
*             All Right Reserved                          
* 
*******************************************************************************
*/ 
#include "ap_common.h"

#pragma name(COM_UI_MENU)

#define RowNum  1  //ÿһҳ�Ĳ˵�ѡ��ĸ���
#define Point_picwidth 8//

#define MenuStringStartPosition  24

/*ע�⣭���º��������ܷ��ʵ���Щ���� */
static const uint8 menu_pos[5] = {POSITION_MENU1, POSITION_MENU2, POSITION_MENU3, 
        POSITION_MENU4, POSITION_MENU5};

static const region_t title_region=
{0,0,64,16};//�˵�ͷ
static const region_t menu_region=
{0,32,128,16};//�˵�ѡ��
/*
********************************************************************************
* Description : ���ܲ˵��ؼ�
*
* Arguments   : menu, ���ܲ˵�����
        callback, ���ܲ˵��Ļص�����,=NULL ��ʾû�лص�����
*
* Returns     : �����Ϣ, ����0
*
* Notes       :ͳһ���ô�����:8*16��16*16
*
********************************************************************************
*/
uint16 ui_function_menu(function_menu_t *menu, const void (*callback)(uint16 res_id))
{
//    region_t region;
//    uint16 i;
    uint16 max_group;   
    uint16 cur_group;   
    uint16 n;    
    uint8 key;
	uint16 scrollcount=0;
	uint16  scrollcount2=0;
    bool need_draw=TRUE,need_draw_all=TRUE;
    bool value_changed=TRUE;
//    SetAsciiFont(FONT_TYPE_LARGE);
    while(1)
    {

		if(need_draw_all)
		{
    		 ui_auto_update = FALSE;
    		    
    		 SetAsciiFont(FONT_TYPE_LARGE);
             ClearScreen(NULL);
    //		 ResShowMultiString_Scroll2(menu->menuhead, Point_picwidth,0,100);//�����ݲ���
    //		 UpdateScreen(&title_region);
    		 
    		 ResShowPic(ICON_MY_MENU_BOX,0,56);
    		 ResShowPic(ICON_MENU_BOX_RIGHT,55,16);
    		 ResShowPic(ICON_MENU_BOX_LEFT,0,16);
    		 
    		 //UpdateScreen(NULL);  //matt140210#1
    		 ui_auto_update = TRUE;
    		 
             need_draw_all=FALSE;
    		 need_draw=TRUE;
		}
        if(need_draw)
        {
			ui_auto_update = FALSE;

			ui_show_lock_1();
			
			ClearScreen(&menu_region);
			 
            max_group = menu->total / RowNum;
            if((menu->total % RowNum) == 0) max_group--;//����ҳ������
            cur_group = menu->active / RowNum;
            n = cur_group*RowNum;// �ҳ����ʼ��һ��ı��   
//				if(n+i==menu->active)
//				continue;//��ǰ���
				
//            ResShowMultiString(menu->string[n+i],i*7+12, 8);
//			  ResShowMultiString(menu->total, uint8 x, uint8 y)
			ResShowPic(SYSFONT_SMALL_0 + (menu->active + 1)/10, 20, 16);
            ResShowPic(SYSFONT_SMALL_0 + (menu->active + 1)%10, 20+6, 16);
           	ResShowPic(SYSFONT_SMALL_SLASH, 20+12, 16);
            ResShowPic(SYSFONT_SMALL_0 + (menu->total/10), 20+18, 16);
            ResShowPic(SYSFONT_SMALL_0 + (menu->total%10), 20+24, 16);
			SetAsciiFont(FONT_TYPE_LARGE);
            //ResShowPic(MENUDOTA,0,((menu->active % RowNum)*16+16)); //���ָʾ��
            ResShowMultiString_Scroll(menu->string[menu->active], 255,32,128,0);//�����ݲ���
//           ResShowMultiString(menu->string[menu->active], 255,16);
//            SetAsciiFont(FONT_TYPE_DEFAULT);
			ui_show_batt(GetBattery());
			
            UpdateScreen(NULL);
    		ui_auto_update = TRUE;
    		 
            need_draw = FALSE;
        }
        
		//================
		//========
        if(ScrollFlag)
   	    {
           if(scrollcount++>20) //matt120306#1
           {
             scrollcount = 0;
		     SetAsciiFont(FONT_TYPE_LARGE);
		     /*
			 region_scroll.x = 0;
			 region_scroll.y = 16;
			 region_scroll.width =96;
			 region_scroll.height=16;
			 */
             ScrollString(scroll_buf, FALSE,UNICODELANGUAGE,&region_scroll);
             UpdateScreen(NULL);
			 SetAsciiFont(FONT_TYPE_DEFAULT);
           }
   	    } 
		//=============
		
        key = ap_get_message();
        switch( key)
        {
        case AP_KEY_VOLADD:
        case AP_KEY_VOLADD | AP_KEY_HOLD:
        #if(MACHINE_MODEL != 1)
        case AP_KEY_NEXT:
        case AP_KEY_NEXT | AP_KEY_HOLD:
        #endif
            if(menu->active < menu->total-1)
            {
                menu->active++;
                value_changed = TRUE;
                need_draw= TRUE;
            }
            else
            {
                menu->active = 0;
                value_changed = TRUE;
                need_draw = TRUE;
            }
            break;
            
        case AP_KEY_VOLSUB:
        case AP_KEY_VOLSUB | AP_KEY_HOLD:  
        #if(MACHINE_MODEL != 1)
        case AP_KEY_PREV:
        case AP_KEY_PREV | AP_KEY_HOLD:
        #endif
            if(menu->active > 0)
            {
                menu->active--;
                value_changed = TRUE;
                need_draw = TRUE;
            }
            else
            {
                menu->active = menu->total-1;
                value_changed = TRUE;
                need_draw = TRUE;
            }
            break;
            
        case AP_KEY_MODE | AP_KEY_UP:    //ȡ���˳�
        #if(MACHINE_MODEL == 1)
        case AP_KEY_PREV | AP_KEY_UP:
        #endif
            return RESULT_IGNORE; 
            
        case AP_KEY_PLAY | AP_KEY_UP:    //ȷ���˳�
        #if(MACHINE_MODEL == 1)
        case AP_KEY_NEXT | AP_KEY_UP:
        #endif
            return 0;
            
        case AP_MSG_WAIT_TIMEOUT:
            //if(ui_auto_select)  return 0;
            //this is recommoned by johnlee, 2004/03/02
            if(ui_auto_select)  return RESULT_IGNORE; 
            break;  
            
        case AP_MSG_RTC:
            ui_show_batt( GetBattery());
            break;   
            
        default:
            key = ap_handle_hotkey(key);
            if(key == RESULT_REDRAW) need_draw_all = TRUE;
            else if(key != 0) return key;
        }
         
            if(callback != NULL && value_changed)
            {
	            callback(menu->active);
	            value_changed = FALSE;
            }
        
    }//while (1)
    return 0;
    
}

