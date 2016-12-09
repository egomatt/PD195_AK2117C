/*
 *******************************************************************************
 *                ACTOS AP
 *        ap common lib, slider control file
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 *  $Id: ui_slider.c,v 1.1.1.1 2006/05/22 03:23:59 selina Exp $
 *******************************************************************************
 */

#include "ap_common.h"

#pragma name(COM_UI_SLIDER)

#define   SLIDER_X    0  
#define   SLIDER_Y    32
//Ϊ�˲���bank�л���ʹ�� ResShowPic()
const  uint8 i[]=":";
extern res_entry_t res_entry;
extern region_t res_region;
#ifdef  UNICODE_SUPPORT
extern uint32   pic_entry_offset;
extern uint32   string_entry_offset; 
#endif
//ֻ���������ʹ��
uint16 slider_ResShowPic(uint16 id, uint8 x, uint8 y)
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
   
    //if(ui_auto_update) UpdateScreen(&res_region);
    return 1;
}
void ui_show_slider(uint8 vol,uint8 volmax,uint8 flag)
{  
    uint16 i,nCnt;
    uint16 num;//,num2;
   
    if(volmax==g_comval.BackLightMax)
    {
       //slider_ResShowPic(IM_SBAR_CONT_BG,16,32);//SLIDER_X, SLIDER_Y);
       nCnt =((uint16)vol*98) / volmax;
       num=98;
       //num2=98/volmax;;
    }
	else
   	{
       //slider_ResShowPic(IM_SBAR_BG,16,32);//SLIDER_X, SLIDER_Y);
       nCnt =((uint16)vol*98) / volmax;
       num=98;
       //num2=98/volmax;
   	}
   	flag = flag;
    //if(flag==0)    //first
   	{
       if(volmax==g_comval.BackLightMax)
       {
       		slider_ResShowPic(IM_SBAR_CONT_BG,13,48);//SLIDER_X, SLIDER_Y);
       }
       else
       {
       		slider_ResShowPic(IM_SBAR_BG,13,48);//SLIDER_X, SLIDER_Y);
       }
       for(i = 0; i < nCnt; i++)
       {
           slider_ResShowPic(IM_SBRA,15 + i ,48);
       }   
       //for(i = nCnt; i < 64; i++)
       for(i = nCnt; i < num; i++)
       {
           slider_ResShowPic(IM_SBAR_CLEAR,15 + i,48);
       }
    }
    /*
    if(flag==0x01)    //up
    {
        while(num2--)
       	{
       		slider_ResShowPic(IM_SBRA,15 + nCnt + num2 ,48);
       	}
    }
    else if(flag==0x10)   //down
    {
       	while(num2--)
       	{
            slider_ResShowPic(IM_SBAR_CLEAR,15 + nCnt - num2,48);
       	}
    }
    */
}
/*
********************************************************************************
* Description : �����˿ؼ�
*
* Arguments   : slider, �����˽ṹ
        callback, �����˵Ļص�����,=NULL ��ʾû�лص�����
        z: ѡ��ʹ��PREV ��NEXT ������VOLADD ��VOLSUB ����������
            z=0:  ʹ��VOLADD ��VOLSUB
            z=1:  ʹ��PREV ��NEXT ��
*
* Returns     : �����Ϣ, ����0
*
* Notes       : �ؼ�����û���ػ�����, ��Ϊ�ؼ������ػ�����
*
********************************************************************************
*/
uint16 ui_slider(slider_t *slider, const void (*callback)(uint16 res_id))
{
//    uint16 start, end;        //x����
    uint8 buf[10];    //ע����ܺ����� <<-0000>>
    uint8 *p;    //��ʽ��bufʱʹ��
    uint8 key;
	uint8 ChageStop=1;
	uint8 i=0;
    BOOL need_draw=TRUE;
    BOOL value_changed=TRUE;    //ֵ�Ƿ�ı�, ��Ҫcallback
    region_t bar_region;//={16,32,64,4};        //={x, y, 68, 8};    //�ؼ���������{60, 56, 68, 8}
    region_t num_region;//={42,16,16,16};        //={x, y-8, 68, 8};    //��������{60, 48, 68, 8}
    uint16 value;    //to keep slider->value
    uint8 hold_state=0;    //��ס״̬,Ϊ�˼ӿ�ؼ��ı���ٶ�

	uint8 up_down_flag=0;
                //0:normal; 1:holding up, 2:holding down
    bar_region.x = 13;//SLIDER_X;
    bar_region.y =48;// SLIDER_Y;
    bar_region.width = 102;
    bar_region.height = 8;

    num_region.x = 56;//SLIDER_X;
    num_region.y = 40;////SLIDER_Y-8;
    num_region.width = 16;
    num_region.height = 8;
    
    while(1)
    {
        if(need_draw)
        {
            ui_auto_update = FALSE;
            
            //clear screen
            ClearScreen(&bar_region);
            ClearScreen(&num_region);
            //show pic           
            ui_show_batt(GetBattery());
			if(slider->max==g_comval.BackLightMax)
			    ui_show_slider(slider->value,g_comval.BackLightMax,up_down_flag);
			else
			    ui_show_slider(slider->value,g_comval.VolumeMax,up_down_flag);
//            start = SLIDER_X;//+1;
//            end = /*(SLIDER_X -1) + */
//                SLIDER_BAR_WIDTH * (slider->value - slider->min) / (slider->max - slider->min);
////            ASSERT(end < SCR_WIDTH);
//            //�ֶ���ʾ����
//            while(start+SLIDER_BG_WIDTH <= end)
//            {
//                slider_ResShowPic(IM_SBRA, start, SLIDER_Y);
//                start += SLIDER_BG_WIDTH;
//            }
//            // �ڱ����ľ�ͷ����ʾ�ϰ�
////            slider_ResShowPic(SSLIDER, start, SLIDER_Y);
//            //format text <-xxxx>
            p = buf;
            if(slider->value < 0)
            {
//                value = -(slider->value);
                *p++ = '-';    //��ʾ����
            }
            else
            {
                value = slider->value;
            }
            
            if(value >= 1000)
            {
                itoa4(value, p);
                p += 4;
            }
            else if(value >= 100)
            {
                itoa3(value, p);
                p += 3;
            }
            else if(value >= 10)
            {
                itoa2(value, p);
                p += 2;
            }
            else
            {
                itoa1(value, p);
                p++;
            }
            *p = 0;
            //show text <-xxxx>    
            SetAsciiFont(FONT_TYPE_DEFAULT);
            SetTextPos(num_region.x+(num_region.width - (BYTE)get_text_width(buf)) / 2, num_region.y);
            ClearScreen(&num_region);
            PutS(buf, -1);
			SetAsciiFont(FONT_TYPE_LARGE);
			if(slider->max==g_comval.BackLightMax)
			    ResShowMultiString(CONTRAST,255,16);
			else
			    ResShowMultiString(CHANGEVO,255,16);
			    
            UpdateScreen(NULL);    //&num_region);
            ui_auto_update = TRUE;
			
            need_draw = FALSE; 
             
        }//need draw
            
        //handle message
        key = ap_get_message();
        switch( key)
        {
        case AP_MSG_RTC:
//			if(value_changed == FALSE)
//			{
				i++;
				if(i > 4)
				{
			      if(slider->max!=g_comval.BackLightMax)
					return 0;
				}				
//			}
//			else
//			{
//				i = 0;
//				ChageStop = 1;
//			}
            ui_show_batt( GetBattery());
            break;
		
		#if(MACHINE_MODEL != 1)
        case AP_KEY_NEXT:
        case AP_KEY_NEXT | AP_KEY_HOLD:
        #endif
        case AP_KEY_VOLADD:
        case AP_KEY_VOLADD | AP_KEY_HOLD:
//            if(z==0) break;
			ChageStop = 0;
            if((slider->value + slider->step) <= slider->max)
            {
                slider->value += slider->step;
                i = 0;
                value_changed = TRUE;
                need_draw = TRUE;
				up_down_flag=0x01;    //up
            }
            
            break;

        #if(MACHINE_MODEL != 1)    
        case AP_KEY_PREV:
        case AP_KEY_PREV | AP_KEY_HOLD:
        #endif
        case AP_KEY_VOLSUB:
        case AP_KEY_VOLSUB | AP_KEY_HOLD:
//            if(z==0) break;
			if(slider->value==0)
				break;//===================longjin  
			
            if((slider->value - slider->step) >= slider->min)//��С��0 ��Ҫ������һ�δ���
            {
                slider->value -= slider->step;
                i = 0;
                value_changed = TRUE;
                need_draw = TRUE;
				up_down_flag=0x10;     //down
            }
            break;

        //hold �������Ƽ��ٿ���
/*        case AP_KEY_NEXT | AP_KEY_HOLD:
        
//            if(z==0) break;
            if(hold_state == 0) hold_state = 1;

            break;


        case AP_KEY_PREV | AP_KEY_HOLD:
       
//            if(z==0) break;
            if(hold_state == 0) hold_state = 2;
            break;
            
            
        case AP_KEY_VOLADD:
            if(z==1) break;
            if((slider->value + slider->step) <= slider->max)
            {
                slider->value += slider->step;
                value_changed = TRUE;
                need_draw = TRUE;
            }            
            break;
            
        case AP_KEY_VOLSUB:
            if(z==1) break;
            if((slider->value - slider->step) >= slider->min)
            {
                slider->value -= slider->step;
                value_changed = TRUE;
                need_draw = TRUE;
            }            
            break;

        case AP_KEY_VOLADD | AP_KEY_HOLD:
            if(z==1) break;
            if(hold_state == 0) hold_state = 1;
            break;
        case AP_KEY_VOLSUB | AP_KEY_HOLD:
            if(z==1) break;
            if(hold_state == 0) hold_state = 2;        
            break;*/

/*        case AP_KEY_NEXT | AP_KEY_UP:
        case AP_KEY_VOLADD | AP_KEY_UP:
        case AP_KEY_PREV | AP_KEY_UP:
        case AP_KEY_VOLSUB | AP_KEY_UP:
            hold_state = 0;
            
            break;
*/            
            
//        case AP_KEY_MODE | AP_KEY_UP:
        case AP_KEY_PLAY | AP_KEY_UP:
        #if(MACHINE_MODEL == 1)
        case AP_KEY_NEXT | AP_KEY_UP:
        #endif
            return 0;        //ok and return

		case AP_KEY_MODE | AP_KEY_UP:       //masda20110527
		#if(MACHINE_MODEL == 1)
        case AP_KEY_PREV | AP_KEY_UP:     
        #endif
			if(slider->max==g_comval.BackLightMax)
			    return RESULT_IGNORE;
			else
			    return 0;        //ok and return
			
        case AP_MSG_WAIT_TIMEOUT:
            if(ui_auto_select) return 0;
            break;
            
        default:
            key = ap_handle_hotkey(key);
            if(key == RESULT_REDRAW) //need_draw = TRUE;
            {
                //ֱ���˳�����Ϊ�ؼ������ػ�����
                return 0;
            }
            else if(key != 0) 
            {
                return key;
            }
        }

//        if(hold_state == 1)        //holding up
//        {
//            if((slider->value + slider->step) <= slider->max)
//            {
//                slider->value += slider->step;
//                i = 0;
//                value_changed = TRUE;
//                need_draw = TRUE;
//                while(key != NULL)
//                {
//                    if((key & AP_KEY_UP) != NULL)
//                        hold_state = 0;
//                    key = ap_get_message();
//                }       
//            }
//            else hold_state = 0;    //disable holding  
//        }
//        else if(hold_state == 2)    //holding down
//        {
//        
//            if(slider->value==0)
//			 				 hold_state=0;//longjin 2011-1-11
//			else
//			{	
//            	if((slider->value - slider->step) >= slider->min)
//            	{
//                	slider->value -= slider->step;
//                	i = 0;
//               	 	value_changed = TRUE;
//                	need_draw = TRUE;
//               	 	//while(key != NULL && (key & AP_KEY_UP) == NULL)key = ap_get_message();
//                	while(key != NULL)
//                	{ 
//                    	if((key & AP_KEY_UP) != NULL)
//                        	hold_state = 0; 
//                    	key = ap_get_message();
//                	}
//            	}
//            	else hold_state = 0;    //disable holding   
//          	}        
//        }

        //handle callback
        if(callback != NULL && value_changed)
        {
            callback(slider->value);
            value_changed = FALSE;
        }
        
    }//while(1)
	return 0;
}

