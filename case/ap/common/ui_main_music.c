/*
 *******************************************************************************
 *                ACTOS AP
 *        ap common lib , main ui control
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 *******************************************************************************
 */

#include "ap_common.h"

#pragma name(COM_UI_MAIN)


/*const uint16 TIME_NUM[11]=
{IM_MUSIC_MP3_NUMBER_0,IM_MUSIC_MP3_NUMBER_1,IM_MUSIC_MP3_NUMBER_2,IM_MUSIC_MP3_NUMBER_3,IM_MUSIC_MP3_NUMBER_4,
 IM_MUSIC_MP3_NUMBER_5,IM_MUSIC_MP3_NUMBER_6,IM_MUSIC_MP3_NUMBER_7,IM_MUSIC_MP3_NUMBER_8,IM_MUSIC_MP3_NUMBER_9,ICON_MY_COLON};*/
//const region_t region_c  =  {POSITION_C, 0, 8, 8};
//const region_t region_ef = {POSITION_F, 0, 15+29+20, 16};
//const uint16   batt_id[4]  =
//{ BATTLVL1, BATTLVL2, BATTLVL3, BATTLVL4, BATTLVL5};// BATTLVL6, BATTLVL7, BATTLVL8, BATTLVL9*/0 }; //��Ӧ1~9��������ͼ��
//{IM_BATTERY_02,IM_BATTERY_03,IM_BATTERY_04,IM_BATTERY_05};
//void ui_show_com(const uint8 *string,const region_t *region1);
//ֻ���������ʹ��,Ϊ�����Ч�ʶ���������
#if (0)
uint16 main_ResShowPic(uint16 id, uint8 x, uint8 y)
{
    res_infor_t res_infor;
    //�������ʱ����ˢ��ͼƬ
    SD_FSeek(res_fp, SEEK_SET, pic_entry_offset + (uint32) id * RESHEADITEM);
    SD_FRead(res_fp, &res_infor, sizeof(res_infor_t));
#ifdef PC
    SIM_ChgLongPC(&res_infor.dwOffset);
    SIM_ChgLongPC(&res_infor.dwLength);
    SIM_ChgWordPC(&res_infor.wWidth);
    SIM_ChgWordPC(&res_infor.wHeight);
#endif
    /*�ж�res���͵ĺϷ���*/
    if (res_infor.bType != 0x02)
    {
        return 0;
    }
    /*�贰��������*/
    res_region.x = x;
    res_region.y = y;
    res_region.width = (uint8) res_infor.wWidth;
    res_region.height = (uint8) res_infor.wHeight;
    /*��ȡͼƬ���ݲ�ˢ��*/
    PutSDImage(&res_region, res_fp->startaddr + res_infor.dwOffset);
    UpdateScreen(&res_region);
    return 1;
}
#endif

/********************************************************************************
 * Description : ���ݻ�ȡ���ĵ�ѹֵ��ʾ��Ӧ�ĵ��ͼ��
 *
 * Arguments  :
 *            value����ǰ��ȡ���ĵ�ѹֵ
 *
 * Returns     :
 *            ��
 * Notes       :
 *
 ********************************************************************************/
void ui_show_batt(uint8 value)
{
    //uint8 cur_value;
    if (check_5v() &&( sKY_ChargeSet(0) == 0x0)) //���нӳ������USB����û�е��ʱ�����ͼ����ʾ��
	{
//		ResShowPic(BATTERY1, POS_BATTERY, POSY_BATTERY);
//        ResShowPic(batt_id[0], POSITION_BATT, 0);
		ResShowPic(IM_USB_BATTERY_01, POSITION_BATT, 0);   
    }
    else
    {
        //��5����ʾ���ͼ�꣬��Ӧ��ϵ���£�
        /*
         ��ص�ѹ��Χ
         1.       �ո�磺<=3.5V  0.05V���

         2.       һ��磺3.5-3.6V  0.05V���

         3.       ����磺3.6-3.65V   0.05V���

         4.       ����磺3.65-3.8V    0.05V���

         5         ����  3 .8V����    0.05V ���

         */
        if (check_5v() && ( sKY_ChargeSet(0) == 1))
        {
            if (cur_value < 4)
            {
                cur_value++;
            }
            else
            {
                cur_value = 0;
            }
        }
        else if (check_5v() && ( sKY_ChargeSet(0) == 2))
        {
             cur_value = 4;
        }
        else
        {
            for (cur_value = 0; cur_value < 5; cur_value++)
            {
                if (value <= batvalue[cur_value])
                {
                    break;
                }
            }
            if (cur_value >= 5)
            {
                cur_value = 4;
            }
            if(cur_value > last_value)
                cur_value = last_value;
            else
                last_value = cur_value;
        }
    	ResShowPic(IM_USB_BATTERY_01+cur_value/*batt_id[cur_value]*/, POSITION_BATT, 0);   
    }
}


void show_waiting(void)
{
    ClearScreen(NULL);//����
    SetAsciiFont(FONT_TYPE_LARGE);
    ResShowMultiString(WAITPASS, 255, 24);//��ʾ���ڳ�ʼ��
    UpdateScreen(NULL);//����
    SetAsciiFont(FONT_TYPE_DEFAULT);
}


