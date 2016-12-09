/*
 *******************************************************************************
 *                ACTOS AP
 *        ap common lib message file, realtime part
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 *  $Id: ap_message.c,v 1.1.1.1 2006/05/22 03:23:59 selina Exp $
 *******************************************************************************
 */
//

#include "ap_common.h"

#pragma name(COM_MSG)
uint8 ap_get_message_core(uint8 key);
uint8 ap_handle_hotkey_core(uint8 key);

/*
 ********************************************************************************
 * Description : ����ϵͳ��Ϣ, ���ذ�����Ϣ, ͬʱ����/�ر���
 ap_get_message_core() �����
 *
 * Arguments   :
 *
 * Returns     : ����ap�ܹ�ʶ�����Ϣ, ���û��ϵͳ��Ϣ, ���� AP_KEY_NULL
 *
 * Notes       :
 ********************************************************************************
 */
// TEST  Addede by Weimh 2010.03.09
//#ifdef PC

//#endif

uint8 ap_get_message(void)
{
    uint8 key;
    
#ifdef Msg_CTC2HZ
    do
    {
        key = GetSysMsg();
    }while(key == MSG_RTC2HZ);
    if(key == Msg_HalfSecond)
    {
        key = MSG_RTC2HZ;
    }
#else
    key = GetSysMsg();
#endif

#ifndef STANDBY_AP   //pedometer
#if(PETOMETER_USE)
    if(g_comval.PedoFlag)
        Count_Step(1);
#endif
#endif

    if (key == Msg_KeyNull)
    {
        return AP_KEY_NULL;
    }
    else
    {
        return ap_get_message_core(key);
    }
}

/*
 ********************************************************************************
 * Description : �����ȼ���Ϣ, ���ؽ����Ϣ
 ap_handle_hotkey_core() �����
 *
 * Arguments   : key, ������Ϣ
 *
 * Returns     : ����п�ʶ����ȼ���Ϣ,  ���ؽ����Ϣ
 ���û�п�ʶ����ȼ���Ϣ,  ����0
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint8 ap_handle_hotkey(uint8 key)
{
    if (key == AP_KEY_NULL)
    {
        return 0;
    }
    else
    {
        return ap_handle_hotkey_core(key);
    }
}


//matt120315#2 
uint8 check_5v(void)
{
	uint ret,sfr_bak;
	sfr_bak=SFR_BANK; 
    SFR_BANK = BANK_PMU;
	if((SYSTEM_VOL & 0x80)||(CHG_CTL & 0x80))
    	ret=1;
    else
    	ret=0;
    SFR_BANK = sfr_bak;
    return	ret;
}   
     
uint8 earphone_in(void)
{
    return 1;
    /*
	uint ret,sfr_bak;
	sfr_bak=SFR_BANK; 
    SFR_BANK = BANK_GPIO;
    GPIOCOUTEN &= (~0x01);
	GPIOCINEN |= 0x01;
    if(GPIOCDAT & 0x01)
    	ret=0;
    else
    	ret=1;
    SFR_BANK = sfr_bak;
    return	ret;
    */
}

void earphone_on(bool on)
{
	uint8 sfr_bak;
	sfr_bak=SFR_BANK;
	SFR_BANK = BANK_GPIO;   //set GPIO Bank
    GPIOCOUTEN |= 0x10;     //ENABLE GPIOC4
    #if(EARPHONE_ON)
    if(!on)
    #else
    if(on)
    #endif
		GPIOCDAT &= ~(0x10);//clr GPIOC4 	
	else
		GPIOCDAT |= 0x10;   //set GPIOC4   	
	SFR_BANK=sfr_bak;
}