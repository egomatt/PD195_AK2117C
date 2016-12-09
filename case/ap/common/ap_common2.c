/*
 *******************************************************************************
 *                ACTOS AP
 *        ap common lib file, part 2
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 *  $Id: ap_common2.c,v 1.1.1.1 2006/05/22 03:23:59 selina Exp $
 *******************************************************************************
 */

#include "ap_common.h"
#pragma name(COM_COM2)
extern uint8 ap_handle_hotkey_core(uint8 key);
//========== global field ===============
//#define d_Scroll_width  96-1
//extern res_entry_t res_entry;

/*
 ********************************************************************************
 * Description : ����ַ�������ʾ���
 *
 * Arguments   :
 *
 *
 * Returns     :
 *
 * Notes       : �ܹ�ʶ�� \r \n, ������Ϊ2��TCHAR ʶ��
 *
 ********************************************************************************
 */

uint16 get_text_width(const uint8 *string)
{
    int w = 0;

    

    while (*string != 0)
    {
        if (*string == '\r' || *string == '\n')
        {
            ;
        } //������س�����
        else if (*string < 0x80)
            w += CHAR_WIDTH;
        else
            w += TCHAR_WIDTH;

        string++;
    }
    return w;
}



/*
 ********************************************************************************
 * Description : ����ַ�������ʾ���
 *
 * Arguments   :
 *
 *
 * Returns     :
 *
 * Notes       : �ܹ�ʶ�� \r \n, ������Ϊ2��Tuint8 ʶ��
 *
 ********************************************************************************
 */

uint16 get_large_text_width(const uint8 *string)
{
    //ע��: ��� LARGE_uint8_WIDTH != TCHAR_WIDTH, ��ʹ��ע�������Ĵ���

    //ASSERT(LARGE_CHAR_WIDTH == TCHAR_WIDTH);
    return strlen(string) * 8;
}

/*
 ********************************************************************************
 * Description : ��ָ��λ����ʾ�������ַ���
 *
 * Arguments   : str_id:�ַ���ID��
 x, y, ��Ļ����,x=255 ��ʾx�������
 *
 * Returns     : �ɹ�, 1
 ʧ��, 0
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint16 ResShowMultiString(uint16 id, uint8 x, uint8 y)
{
    uint8 str_len;
    uint16 str_size;
    res_langid_entry_t res_langid_entry;

    /*��ȡ�ַ���ID������Ϣ */
    memset(scroll_buf, 0, sizeof(scroll_buf));
    str_size = sizeof(res_langid_entry_t);
    SD_FSeek(res_fp, SEEK_SET, string_entry_offset + (uint32) id * str_size); // ָ��ָ���ַ����ļ��е�λ��
    SD_FRead(res_fp, &res_langid_entry, str_size); //res_fp->&res_entry
    SIM_ChgLongPC((BYTE *) &res_langid_entry.dwOffset);
    SIM_ChgWordPC((BYTE *) &res_langid_entry.dwLength);

    /*��ȡ�ַ������ݣ�unicode���� */
    //   str_len = (uint8) (res_langid_entry.dwLength < 100) ? (res_langid_entry.dwLength) : 100;
    if (res_langid_entry.dwLength < 100)
    {
        str_len = (uint8) res_langid_entry.dwLength;
    }
    else
    {
        str_len = 100;
    }
    SD_FSeek(res_fp, SEEK_SET, res_langid_entry.dwOffset);
    SD_FRead(res_fp, scroll_buf, str_len);

    /*�Ƿ������ʾ�ַ��� */
    if (x == 255)
    {
       // SetTextPos(((uint8) SCR_WIDTH - GetStringLength(scroll_buf, (uint16) str_len / 2, 1)) / 2, y);
        region_scroll.x = ((uint8) SCR_WIDTH - GetStringLength(scroll_buf, (uint16) str_len / 2, 1))/2;
    }
    else
    {
       // SetTextPos(x, y);
        region_scroll.x = x;
    }

    /*����50���ֽ�,���ý����� */
    if (str_len == 100)
    {
        scroll_buf[100] = 0x00;
        scroll_buf[101] = 0x00;
    }
#if 1
    	region_scroll.width = SCR_WIDTH-x;//������һ����ȱ�������� �������Ƿ���������BUG
    	region_scroll.height = SCROLL_REGION_HEIGHT;
   		//region_scroll.x = x;
    	region_scroll.y = y;
    	scroll_buf[str_len - 2] = ' ';
    	scroll_buf[str_len - 1] = 0x00;
    	scroll_buf[str_len] = ' ';
    	scroll_buf[str_len + 1] = 0x00;
    	scroll_buf[str_len + 2] = 0x00;
    	scroll_buf[str_len + 3] = 0x00;    
		ScrollFlag = FALSE;
    	ScrollString(scroll_buf, TRUE,UNICODELANGUAGE , &region_scroll);// ARABICHASJOIN
    
	
#else
        PutUnS(scroll_buf,  -1); 
        if(ui_auto_update) UpdateScreen(NULL);
#endif
  
    return 1;
}

/*
 ********************************************************************************
 * Description : ��ָ��λ�ù�����ʾ�������ַ���
 *
 * Arguments   : id, ��Դ����
 x, y, ��Ļ����,x=255 ��ʾx�������
 *       scroll_width, ��������Ŀ��
 * Returns     : �ɹ�, 1
 ʧ��, 0
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint16 ResShowMultiString_Scroll(uint16 str_id, uint8 x, uint8 y, uint8 scroll_width,uint8 chance)
{
#if 1
    uint8 str_len;
    uint8 strWidth;
    uint16 str_size;
    res_langid_entry_t res_langid_entry;
    /*��ȡ�ַ���ID������Ϣ */
	strWidth=scroll_width;//ȥ������
    memset(scroll_buf, 0, sizeof(scroll_buf));
    str_size = sizeof(res_langid_entry_t);
    SD_FSeek(res_fp, SEEK_SET, string_entry_offset + (uint32) (str_id * str_size)); // ָ��ָ���ַ����ļ��е�λ��
    SD_FRead(res_fp, &res_langid_entry, str_size); //res_fp->&res_entry
    SIM_ChgLongPC((BYTE *) &res_langid_entry.dwOffset);
    SIM_ChgWordPC((BYTE *) &res_langid_entry.dwLength);

    /*��ȡ�ַ������ݣ�unicode���� */
    //    str_len = (uint8) (res_langid_entry.dwLength < 100) ? (res_langid_entry.dwLength) : (100);
    if (res_langid_entry.dwLength < 100)
    {
        str_len = (uint8) res_langid_entry.dwLength;
    }
    else
    {
        str_len = 100;
    }
    SD_FSeek(res_fp, SEEK_SET, res_langid_entry.dwOffset);
    SD_FRead(res_fp, scroll_buf, str_len);

    /*�Ƿ������ʾ�ַ��� */
    strWidth = GetStringLength(scroll_buf, (uint16) str_len / 2, 1);
    if (strWidth >= scroll_width)     //95
    {
        if (x == 255)
        {
            x = 0;
        }
        /*init scroll parameters*/
        region_scroll.width = scroll_width;//95;
        region_scroll.height = SCROLL_REGION_HEIGHT;
        region_scroll.x = x;
        region_scroll.y = y;
        scroll_buf[str_len - 2] = ' ';
        scroll_buf[str_len - 1] = 0x00;
        scroll_buf[str_len] = ' ';
        scroll_buf[str_len + 1] = 0x00;
        scroll_buf[str_len + 2] = 0x00;
        scroll_buf[str_len + 3] = 0x00;
        ScrollFlag = TRUE;
        ScrollString(scroll_buf, TRUE,UNICODELANGUAGE , &region_scroll);// ARABICHASJOIN
    }
    else
    {
        ScrollFlag = FALSE;
        if (x == 255)
        {
            x = ((uint8) SCR_WIDTH - strWidth)/2;
        }
        if(chance ==1)
        	SetTextPos(x, y);
        else 	
        	SetTextPos((SCR_WIDTH - strWidth )  / 2, y);
        PutUnS(scroll_buf, -1);//ARABICHASJOIN
    }
#endif
    return 1;
    
}

void ui_show_CardOut(void)
{
#if 0    
    DC oldDC_sav;
    GetDCStatus(&oldDC_sav);

    SetPenColor(Color_WHITE_def);
    SetBackgdColor(Color_BULE_def);
    ClearScreen(NULL);
    SetTextOutMode(1);
    ResShowMultiString(CARDPULL, 255, 80);
    ap_sleep(4);
    SetPenColor(oldDC_sav.PenColor);
    SetBackgdColor(oldDC_sav.BackgdColor);
#endif    
}
/*
 ********************************************************************************
 * Description : ��ʼ����Ϣ����
 *
 * Arguments   : comval, ϵͳ�趨ֵ, =null ��ʾֻ��ʼ���ڲ�������
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void ap_message_init(const comval_t *comval)
{

    if (comval != NULL)
    {
        g_light_time = comval->LightTime * 2;
        g_lock_time =  comval->LockTime * 2;
        g_light_mode = comval->Lightmode;
        g_sleep_time = comval->SleepTime * 60 * 2;

        g_standby_time = comval->StandbyTime * 2 * 60;
        g_contrast_num = comval->DisplayContrast;
        //        key_g_light_time = g_comval.keybacklight*2;
/*#ifdef EAR_PROTECT
        g_EarProtect_flag = comval->EarProtect_flag;
        EarProtectThreshold = comval->EarProtectThreshold;
        g_autoswitch_time = comval->auto_switchoff_time * 60 * 2;
#endif*/
    }

    g_rtc_counter = 0;
    //    g_light_flag = TRUE;
}
/*
 ********************************************************************************
 * Description : ˯��ָ����ʱ��
 *
 * Arguments   : half_sec, ʱ��, 0.5��Ϊ��λ
 *
 * Returns     : ���ָ����ʱ����û��������Ϣ����, ����0
 ���ָ����ʱ������������Ϣ����, ���ظ���Ϣ
 * Notes       : ���ָ����ʱ������������Ϣ����, ˯�ߵ�ʱ�佫< ָ����ʱ��
 *
 ********************************************************************************
 */

uint8 ap_sleep(uint16 half_sec)
{
    uint8 n = 0; //rtc counter
    uint8 key, result;

    #ifdef MUSIC_AP
    SetPLL(PLL_48MHZ);//matt140206#2
    #endif
    while (1)
    {
        key = ap_get_message();
        //sleep ʱ����timeout
        if ((key == AP_MSG_RTC) || (key == AP_MSG_CHARGING) || (key == AP_MSG_WAIT_TIMEOUT))
        {
            n++;
            if (n >= (uint8) half_sec)
            {
                result = 0;
                break;
            }
        }//lock ֻ��up, û��long/hold
        else if ((key == AP_KEY_NULL) || (key == AP_MSG_LOCK) || (key == (AP_MSG_LOCK | AP_KEY_UP)))
        {
            continue;
        }
        else
        {
            result = ap_handle_hotkey_core(key);
            break;
        }
    }
    #ifdef MUSIC_AP
    SetPLL(DEFAULT_FREQ);//matt140206#2
    #endif
    return result;
}

uint8 ui_show_lock(uint8 islock)
{
    ClearScreen(NULL);
    if(islock == 1)
        ResShowPic(ICON_KEY_HOLD,0,0);
    else
        ResShowPic(ICON_LIST_GROUND,0,0);
	UpdateScreen(NULL);
    
    //wait 2s
    return ap_sleep(4);
}

extern bool g_lockflag;
void ui_show_lock_1(void)
{
    if(g_lockflag)
        ResShowPic(ICON_KEY_LOCK,88,0);
    else
        ResShowPic(IM_LOCK,88,0);
    //UpdateScreen(NULL);
}
