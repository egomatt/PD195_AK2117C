/*
 *******************************************************************************
 *                ACTOS AP
 *        ap common lib file, udisk use this file too
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 *  $Id: ap_common.c,v 1.1.1.1 2006/05/22 03:23:59 selina Exp $
 *******************************************************************************
 */

#include "ap_common.h"

//========== global field ===============
#pragma name(COM_COM)
uint8 g_keylight_flag = TRUE;//�����ƿ��ر�־��//maya090412#1 for keylight
uint8 g_keylight_time = 0;//�ذ�������ʱ�䣬0.5��Ϊ��λ

uint8 g_light_flag = TRUE;//�����ƿ��ر�־
uint8 g_light_time = 0; //�ر���ʱ��,0.5��Ϊ��λ
uint8 g_light_mode = 0; //������ģʽ��0���䰵��1�����
uint8 g_lock_time = 0; //Auto lockʱ��,0.5��Ϊ��λ
uint8 g_contrast_num = 0; //�Աȶ�
//uint8 usbplugflag = 0;    //USB������
uint8 g_charge_counter = 0; //�����״̬�ļ�����������USB�Ϳ�ʼ����һ��
uint16 g_sleep_time = 0; //˯��ʱ��,0.5��Ϊ��λ
uint16 g_standby_time = 0; //�Զ��ػ�ʱ��,0.5��Ϊ��λ
uint16 g_rtc_counter = 0; //rtc ��Ϣ����
uint16 key_count = 0; //��ǰ������Ϣ�����Ĵ���
uint8 key_value = Msg_KeyNull; //��ǰ������ֵ
uint16 lowPower_counter = 0;
/*#ifdef EAR_PROTECT
uint8 g_decrease_time = 0; //�����ݼ���ʱʱ��
uint8 g_EarProtect_flag; //����ģʽ�Ƿ����ı�־
uint8 EarProtectThreshold;
uint16 g_half_hour_counter = 0;
uint16 g_autoswitch_counter = 0;
uint16 g_autoswitch_time;
#endif*/

uint8 file_name[12]; //����ļ���������
//uint8 dir_name[30];
uint32 pic_entry_offset;
uint32 string_entry_offset;
SD_FILE *res_fp;
res_entry_t res_entry;
region_t res_region;

#ifndef USB_DISK_USING
//������ز���
uint8 *str_pt;//�ַ�����ǰָ��
uint8 *strhead_pt;//�ִ���ַ��¼
uint8 scroll_cnt;
uint8 ScrollFlag;
uint8 ScrollFlag2;//�˵���������
uint8 scroll_buf[104];
region_t region_scroll; //����
//fiona add for special language
//uint8 charbuf[33];
uint8 scrollhead_width;
uint8* scrollhead_pt;//�����ִ���ַ��¼

uint8 g_menu_active = FALSE;

//�벻Ҫ��������������ڷ�IDATA0 ��,�����ܳ�ʼ��
uint8 ui_auto_update = TRUE; //�Ƿ��Զ�ˢ����Ļ
uint8 ui_auto_select = TRUE; //�Ƿ��Զ�ȷ��
uint8 ui_run_realtime = FALSE; //�ؼ���ʵʱģʽ
#endif
uint8 batvalue[5] =
{ 0x5b, 0x5f, 0x65, 0x6b, 0x75 };//{ 0x58, 0x5d, 0x5f, 0x65, 0x76 };
uint8 cur_value = 0; //��ǰ�ĵ�ص���ֵ����Ҫ���ڳ����ʾ
uint8 last_value = 10;
bool blnInPlayLong = FALSE; //��play long������
bool g_lockflag = FALSE;
bool g_lockflag2 = FALSE;
//uint8 InvertFlag;

Pedo_Record_t Pedo_record;
bool Pedo_ui_draw = FALSE;   //pedometer
bool Pedo_work_flag = FALSE;
uint8 Pedo_time_cnt = 0;
uint32 Pedo_total_sec = 0;
uint32 Pedo_total_step = 0;
//uint16 test_up = 0;
//uint16 test_down = 0;
uint8  Pedo_ui_num = 0;
//uint8 gsensor_posflag = 0;	//20110525	0:��״̬Ϊ��ֵ	1:��״̬ΪСֵ
//uint8 g_debounce = 0;

bool Pedo_Swim_flag = FALSE;   //0:pedometer    1:Swim
bool Swim_work_flag = FALSE;
uint32 Swim_total_sec = 0;
uint32 Swim_total_step = 0;

/*
 ********************************************************************************
 * Description : ����Դ�ļ�
 *
 * Arguments   : filename, ��Դ�ļ�����
 *
 * Returns     : �ɹ�,�ļ�ָ��
 ʧ��, NULL
 *
 * Notes       :
 *
 ********************************************************************************
 */
SD_FILE *ResOpen(const uint8* filename)
{
    res_head_t res_head;
    //open res file
    res_fp = SD_FOpen(filename, 0);
    if (res_fp == NULL)
    {
        return NULL;
    }
    SD_FRead(res_fp, &res_head, sizeof(res_head_t));
    SIM_ChgWordPC(&res_head.counts);
    /*�ж��ļ���ʽ�Ƿ�Ϸ� */
    if ((res_head.magic[0] != 'R') || (res_head.magic[1] != 'U') || (res_head.magic[2] != '2') || (res_head.magic[3]
            != 0x19))
    {
        SD_FClose(res_fp);
        return NULL;
    }

    /*��ȡͼƬ entry ��offset*/
    SD_FSeek(res_fp, SEEK_SET, RESHEADITEM);
    SD_FRead(res_fp, &pic_entry_offset, sizeof(pic_entry_offset));
    SIM_ChgLongPC(&pic_entry_offset);

    /*��ȡ�������������entry��ַ */
    SD_FSeek(res_fp, SEEK_SET, RESHEADITEM * 2);
    SD_FRead(res_fp, &string_entry_offset, sizeof(string_entry_offset));
    SIM_ChgLongPC(&string_entry_offset);

    /*��ȡ��ǰ����ID������entry��ַ */
    //VMRead(&g_comval, VM_SYSTEM, sizeof(g_comval));
    SD_FSeek(res_fp, SEEK_SET, string_entry_offset + (unsigned long) g_comval.langid * RESHEADITEM);
    SD_FRead(res_fp, &string_entry_offset, sizeof(string_entry_offset));
    SIM_ChgLongPC(&string_entry_offset);
    //return ok
    return res_fp;
}

/*
 ********************************************************************************
 * Description : �ر���Դ�ļ�
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void ResClose(const SD_FILE *fp)
{
    SD_FClose(fp);
}

/*
 ********************************************************************************
 * Description : ��ָ��λ����ʾͼ��
 *
 * Arguments   : id, ��Դ����
 x,y, ��Ļ����
 *
 * Returns     : �ɹ�, 1
 ʧ��, 0
 *
 * Notes       :	���¡�Ŀ¼������ṹ�����ƣ���鿴ap_common.h�ļ�
 *
 ********************************************************************************
 */
uint16 ResShowPic(uint16 pic_id, uint8 x, uint8 y)
{
    res_infor_t res_infor;
    //�������ʱ����ˢͼƬ
   /* if (!g_light_flag)
    {
        return 0;
    }
    */
    SD_FSeek(res_fp, SEEK_SET, pic_entry_offset + (uint32) (pic_id * RESHEADITEM)); //�ҵ���N��picture������Ŀ¼
    SD_FRead(res_fp, &res_infor, sizeof(res_infor_t));
    SIM_ChgLongPC(&res_infor.dwOffset);
    SIM_ChgLongPC(&res_infor.dwLength);
    SIM_ChgWordPC(&res_infor.wWidth);
    SIM_ChgWordPC(&res_infor.wHeight);

    /*�ж�res���͵ĺϷ��� */
    if (res_infor.bType != 0x02)
    {
        return 0;
    }

    /*�贰�������� */
    res_region.x = x;
    res_region.y = y;
    res_region.width = (uint8) res_infor.wWidth;
    res_region.height = (uint8) res_infor.wHeight;

    /*��ȡͼƬ���ݲ�ˢ�� */
    PutSDImage(&res_region, res_fp->startaddr + res_infor.dwOffset);
    if(ui_auto_update) UpdateScreen(&res_region);
    return 1;
}
//�Զ������ļ���ʾ���д���,�����ǰ������С�ڰ˸��ֽ�
//����ַ�����Ŀո�ȥ��,Ȼ�����ַ��������"."�ٰѺ�׺����
void Deal_ShortName(uint8 *shorName)
{
    uint8 i = 7;
    uint8 temp_name[12];
    memset(temp_name, 0x0, 12);
    while (1)
    {
        if (shorName[i] == 0x20) //����ո���
        {
            i--;
        }
        else
        {
            break;
        }
    }
    memcpy(temp_name, shorName, i + 1);
    temp_name[i + 1] = 0x2e;
    memcpy(&temp_name[i + 2], &shorName[8], 4);
    memcpy(shorName, temp_name, 12);
}


#ifdef InvertFlag
uint16 ResInvertShowPic(uint16 pic_id, uint8 x, uint8 y)
{
    return 1;
}
#endif

