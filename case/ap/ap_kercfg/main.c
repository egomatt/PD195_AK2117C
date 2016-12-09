#include "kapcfg.h"
#include "stringH.h"


#pragma name(KCFG_M)
extern unsigned long systemtime;
extern unsigned long gwtime;
void power_control(void);
extern void          _nop_     (void);

uint8 InitDateTime;
uint32 rtctime;
date_t date;
time_t time;
kval_t kval;

int16 main(int16 param)
{  
	uint8 sfr_bak;
      param=param; 
   	ClearWatchDog();
    if( !DRV_Install("vm_nanf.drv",0) )
    {
        while(1) {;}
    }

	VMRead(&kval,VM_KERNEL,sizeof(kval_t));
    if (kval.magic == MAGIC_KVAL)
    {
        //�Ƿ�����б�
        if (IsNotPowerDown() == 0) //����  0������   ��0��û����
        {
            InitDateTime = 1; //��Ҫ��ʼ��
        }
        else
        {
            InitDateTime = 0; //����Ҫ��ʼ��
        }
    }
    else //�������һ�ο���
    {
        InitDateTime = 1; //��Ҫ��ʼ��
    }
    if (InitDateTime == 1)
    {         
        kval.rtcresetflag = 0x55; //���õ����־
        date.year = 2010;
        date.month = 10;
        date.day = 29;
        time.hour = 1;
        time.minute = 0;
        time.second = 0;
        TM_SetGWDate(&date, 0); //���޸�gwtime  systemtime��gwtime����ϵͳ��ʼֵ
        TM_SetGWTime(&time); //���޸�gwtime  systemtime��gwtime����ϵͳ��ʼֵ

        date.year = 2010;
        TM_SetDate(&date, 0); //���޸�systemtime   systemtime��gwtime����ϵͳ��ʼֵ
        TM_SetTime(&time); //���޸�systemtime   systemtime��gwtime����ϵͳ��ʼֵ
        //��systemtime �� gwtimeд�ص�VRAM��
        kval.magic = MAGIC_KVAL;
        kval.systemtime = systemtime; //��TM_SetDate���޸�
        kval.gwtime = gwtime; //��TM_SetGWDate���޸�
        VMWrite(&kval, VM_KERNEL, sizeof(kval_t));
		sfr_bak = SFR_BANK;
		SFR_BANK = BANK_RTC;
		RTC_CTL0|=0x80;
		RTCFlush();
		SFR_BANK = sfr_bak;
   		ClearWatchDog();
    }
    else
    {
  
    }
   		ClearWatchDog();

    //��VRAM��ȡ��systemtime �� gwtime
    systemtime = kval.systemtime;
    gwtime = kval.gwtime;

	//power control
	SetVDD(VDDSET_1_6V);
	SetPLL(DEFAULT_FREQ);
	power_control();
    enable_interrupt();
    return 0xffff;
 
}


void power_control(void)
{
    uint8 sfr_bak;
    sfr_bak = SFR_BANK;

 	SFR_BANK = BANK_CMU_RMU;
	MRCR3|=0x08;
	_nop_();
	_nop_();
 	_nop_();
 	_nop_();
	MRCR2|=0x08;
	_nop_();
	_nop_();

	SFR_BANK = BANK_PMU;
	VDD_DCDC_CTL = (VDD_DCDC_CTL & 0xC1)|0x16;
	BDG_CTL|=0x40;
	EFUSE_CTL|=0x40;
	SFR_BANK = BANK_USB;
	USB_PHYCTRL|=0xC0;
	USB_Efuse_Ref|= 0x40;
 	SFR_BANK = BANK_CMU_RMU;
	MRCR1 &= 0xf7;
	MCSR2 &= 0xe0;
	MCSR3 &= 0xbf;
	MCSR5 &= 0xe7;

    SFR_BANK = sfr_bak;
}

 