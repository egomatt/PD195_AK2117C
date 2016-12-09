/*
*********************************************************************************************************
*                                       ACTOS GL5101
*
*
*                           (c) Copyright 2002-2008, Actions Co.,Ltd.
*                                   All Right Reserved
*
* File   : otg.h
* By     : zhouxl
* Version: v1.00     first version     2008-1-24 10:57
*********************************************************************************************************
*/

#ifndef _OTG_H
#define _OTG_H

#include "actos.inc"
#include "time.h"
#include "message.h"
#include "irq.h"
#include "string.h"
#include "device.h"

//�Ƿ�֧��USB Host����
//#define         USB_HOST_SUPPORT
#define FULL_SPEED_ENABLE              0x01


#ifndef USB_HOST_SUPPORT 
   #define USB_HOST_SUPPORT_FLAG 0x00
#else
   #define USB_HOST_SUPPORT_FLAG 0x01
#endif       

//ע���Timer�жϺ�
#define         USB_DETECT_TIMER_INDEX_ADDR             0xff05c0
//USB��⺯��Timer�ж���ڵ�ַ
#define         USB_DETECT_ENTRY_ADDR                   0xff05c1
//Timer�ж���ڵ�ַ�����Memory��ַ
#define         TIMER_ENTRY_MEM_ADDR                  	0x502


#define UIV_OTGIRQ                     0xd8

//bit map for HOST_Glbal
#define GL_ENUMERATED                  0x02
#define GL_SUPPORTED                   0x04
#define GL_DEVICE_IN                   0x08
#define GL_CONTROLLER_ENABLED          0x10
#define GL_DRIVER_INITED               0x40
#define GL_DP_PULLHIGH                 0x80

#ifdef __C251__
extern uint8 HOST_Glbal;
extern uint8 UH_MAX_LUN;

/*
*********************************************************************************************************
*                                           usb_detectfnc_reg
*
* Description: ����USB������Timer�ж�.
*
* Arguments  : None.
*
* Returns    : None.
*
* Note(s)    : USB������ͨ��400ms Timer�ж϶�ʱ��ѯ��ʽ����,���������е��뵽Zram3��.
*********************************************************************************************************
*/
/*
void usb_detectfnc_reg(void)
{
	uint8 i,timeindex;
	uint16* data_p;
	uint32 vector;

	data_p = (uint16*)TIMER_ENTRY_MEM_ADDR;

	for(i=1;i<7;i++)
	{
		timeindex = i;
		if((*data_p) == USB_DETECT_ENTRY_ADDR)
		{
			break;
		}
		data_p += 2;		
	}

	if(i > 0x06)
	{
		vector = USB_DETECT_ENTRY_ADDR;
		timeindex = TM_SetTimer((int16)vector, 0x08);	
	}

	*((uint8*)USB_DETECT_TIMER_INDEX_ADDR) = timeindex;	
}
*/

/*
*********************************************************************************************************
*                                           usb_detectfnc_dereg
*
* Description: ע�����ص�Timer�ж��ϵ�USB������.
*
* Arguments  : None.
*
* Returns    : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
/*
void usb_detectfnc_dereg(void)
{
	 uint8 index,*data_p;
	 index = *((uint8*)USB_DETECT_TIMER_INDEX_ADDR);
	 TM_KillTimer(index);
	 data_p = USB_DETECT_TIMER_INDEX_ADDR;
	 *data_p = 0x00;	
}
*/

/*
*********************************************************************************************************
*                                           usb_plugin_detect
*
* Description: USB�߲�����,D+,D-���źű仯��������PC�����U��.
*
* Arguments  : None.
*
* Returns    : None.
*
* Note(s)    : �˶γ�����뱣֤��ַ���ض�λ,����ʹ��jp, callָ��.
*********************************************************************************************************
*/
/*
void usb_plugin_detect(void)   						 
{							  
	uint8 sfrbak;			   
	
	sfrbak = SFR_BANK;
	SFR_BANK = BANK_USB;
		
	//check if D+ pull high, indicate Udisk plug-in
	if ((USBSTATUS&0x08)!=0x00)
	{
		//last DP status,check if USB DP status changed
		if((HOST_Glbal&GL_DP_PULLHIGH) == 0x00)
		{
			HOST_Glbal = HOST_Glbal | GL_DP_PULLHIGH;
			USB_CON_ENABLE();	
		}			
	}
	else
	{
		HOST_Glbal = HOST_Glbal & (~GL_DP_PULLHIGH);	
	}

	SFR_BANK = sfrbak;
}
*/
#endif

#ifdef __A251__
/*
*********************************************************************************************************
*                                           mUSB_DETECTFNC_REG
*
* Description: ����USB������Timer�ж�.
*
* Arguments  : None.
*
* Returns    : None.
*
* Note(s)    : USB������ͨ��400ms Timer�ж϶�ʱ��ѯ��ʽ����,���������е��뵽Zram3��.
*********************************************************************************************************
*/
mUSB_DETECTFNC_REG macro
		mov		r11,#0x00
		mov		wr2,#TIMER_ENTRY_MEM_ADDR
		mov     wr4,#WORD0 USB_DETECT_ENTRY_ADDR
check_reg_usb_timer:
		add		r11,#0x01
		mov		wr6,@wr2				
		cmp		wr6,wr4
		je		had_reg_usb_timer
	    add		wr2,#0x0004
		cmp		r11,#0x06
		jne		check_reg_usb_timer

		mov     wr6,#WORD0 USB_DETECT_ENTRY_ADDR  //ע��80ms��ѯ�ж�
		mov		r11,#0x08
		mTM_SetTimer

had_reg_usb_timer:
		mov		wr2,#WORD0 USB_DETECT_TIMER_INDEX_ADDR
        mov		wr0,#WORD2 USB_DETECT_TIMER_INDEX_ADDR
		mov		@dr0,r11			
        endm


/*
*********************************************************************************************************
*                                           mUSB_DETECTFNC_DEREG
*
* Description: ע�����ص�Timer�ж��ϵ�USB������.
*
* Arguments  : None.
*
* Returns    : None.
*
* Note(s)    : None.
*********************************************************************************************************
*/
mUSB_DETECTFNC_DEREG macro
		mov		wr6,#WORD0 USB_DETECT_TIMER_INDEX_ADDR
        mov		wr4,#WORD2 USB_DETECT_TIMER_INDEX_ADDR
		mov		r11,@dr4
        mTM_KillTimer
		mov		r11,#0x00
		mov		wr6,#WORD0 USB_DETECT_TIMER_INDEX_ADDR
        mov		wr4,#WORD2 USB_DETECT_TIMER_INDEX_ADDR
		mov		@dr4,r11
        endm


/*
*********************************************************************************************************
*                                           mUSB_PLUGIN_DETECT
*
* Description: USB�߲�����,D+,D-���źű仯��������PC�����U��.
*
* Arguments  : None.
*
* Returns    : None.
*
* Note(s)    : �˶γ�����뱣֤��ַ���ض�λ,����ʹ��jp, callָ��.
*********************************************************************************************************
*/

mUSB_PLUGIN_DETECT  macro
		push	SFR_BANK
		mov		SFR_BANK,#BANK_USB
		
		mov		r0,USBSTATUS			//check if D+ pull high, indicate Udisk plug-in
		anl		r0,#0x08
		cjne	r0,#0x08,usb_not_plugin	
		
        mov		r0,edata(HOST_Glbal)
		anl		r0,#GL_DP_PULLHIGH		//last DP status
		cjne	r0,#0x00,usb_detect_ret			//USB DP status not changed

		mov		r11,edata(HOST_Glbal)
		orl		r11,#GL_DP_PULLHIGH
		mov		edata(HOST_Glbal),r11

		mUSB_CON_ENABLE
		jmp		usb_detect_ret
									
usb_not_plugin:
		mov		r11,edata(HOST_Glbal)
		anl     r11,#(~GL_DP_PULLHIGH)
		mov		edata(HOST_Glbal),r11
usb_detect_ret:	
		pop		SFR_BANK
		ret						
		endm
#endif /* __A251__ */


#endif  //#ifndef _OTG_H