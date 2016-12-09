/*
 ********************************************************************************
 *                       ACTOS
 *               standby ap of sdk 3.0
 *
 *                (c) Copyright 2002-2003, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File   : main.c
 * By     : Gongee
 * Version: 1> v1.00     first version     1/12/2004 11:10PM
 ********************************************************************************
 * $Id: main.c,v 1.1.1.1 2006/05/22 03:23:59 selina Exp $
 ********************************************************************************
 */
#include "ap_common.h"
#include "actos.h"

#pragma name(AP_STANDBY)
uint8 far GPIOBuf[20];
uint8 far intBuf[2];
uint8   far PMUBuf[2];
uint8   far RTCBuf[3];
uint8   far ClockBuf[13];
uint8   far NANDBuf[4];
uint8   far USBBuf[2];
uint8   far RESETBuf[3];
uint8   far PWMBuf;
uint8   far DIODEBuf;
uint8   far temp_sfrbank;
uint8 draw_standby_logo(void);
#ifndef PC
void Enter_HardStandby(void);
void Enter_SoftStandby(void);
#endif
extern uint8 StandbyCharge(void);
void ui_standby_msg(uint16 msgID);
void time_delay(uint8 time);
uint8 time_sleep(uint16 half_sec);
uint8 Get_PowerON_Message(void);
void RecoverPara(void);
extern void _nop_(void);
void MCU_delay(void);
void LOSC_delay(void);
void SetParaToStandby(void);
void MyDI(void);
void MyEI(void);
void SetRESET(void);
void RecoverRESET(void);
void SetRTC(void);
void RecoverRTC(void);
void SetPMU(void);
void RecoverPMU(void);
void SetNAND(void);
void RecoverNAND(void);
void SetPWM(void);
void RecoverPWM(void);
void SetGPIOReg(void);
void RecoverGPIOReg(void);
void SetUSBEfuse(void);
void RecoverUSBEfuse(void);
void SetAllClock(void);
void RecoverAllClock(void);
void SetDIODE(void);
void RecoverDIODE(void);
void SetToLOSC(void);
void RecoverHOSC(void);
comval_t g_comval;
extern STG_INFO StorageInfor;
extern uint8 sysverflag;
int main(int param)
{
    uint8 result;
    uint8 retval;
    uint8 bat_value;
    uint8 rtc_count = 0;
    uint8 key = 0;
    if (param > 0)
    {
    }
	/*
    VMRead(&g_comval, VM_SYSTEM, sizeof(g_comval));
    g_comval.SleepTime = 0; //�˱���Ҫ��0��ʹ˯��ģʽ����һ�ν�һ����Ч��
    VMWrite(&g_comval, VM_SYSTEM, sizeof(g_comval));
    */

    if (check_5v()) retval = 0x40;
    else    retval = 0;
    //�����
    //��ʼ���
//    sKY_ChargeSet(1);
//    sKY_ChargeGetandSet();
    if ((retval & 0xc0) != 0)
    {
        if ((retval & 0x40) != 0)
        {
            return RESULT_UDISK;
        }
        if (sKY_ChargeSet(0) != 0x0)
        {
            res_fp = ResOpen("ui30.res");
            result = StandbyCharge(); //��ʾ������
            ResClose(res_fp);
            if (result != RESULT_IGNORE)
            {
                return result;
            }
        }
        else
        {
            if ((retval & 0x40) != 0)
            {
                return RESULT_UDISK;
            }
            else
            {
                return RESULT_MAIN;
            }
        }

    }
    else
    {
        bat_value = GetBattery();
        if (bat_value <= 0x54)
        {
            res_fp = ResOpen("ui30.res");
//            ui_standby_msg(LOWPOWER);//��ʾ�͵����
            ResClose(res_fp);
        }
    }
    
    draw_standby_logo();//��ʾ�ػ�LOGO
    
    ClearScreen(NULL); //����
    StandbyScreen(0); //�ر������
    UpdateScreen(NULL);
    time_delay(2);    //wait VCC off

#ifndef PC
    if(check_5v())  /*����ڹػ�logoʱ����USB������������*/   //matt140303#2
    {
        result = RESULT_UDISK;
        StandbyScreen(1);   //���������
        return result;
    }
 //   if (sysverflag != 0) //��������ģʽ
    {
        Enter_SoftStandby();
    }
  //  else //����Ӳ����ģʽ
    //������̧�������������Ƶ�ػ�״̬
    {
        do
        {

            key = ap_get_message();
            if (key == AP_KEY_NULL)
            {
            }
            else if ((key == AP_MSG_RTC) || (key == AP_MSG_CHARGING))
            {
                rtc_count++;
            }
            else
            {
                rtc_count = 0;
            }

            //check time out
            if (rtc_count >= 2)
            {
                break;
            }
        } while ((key & AP_KEY_UP) != AP_KEY_UP);
        Enter_HardStandby(); //����Ӳ����ģʽ�������ܵ���LOSC��Ƶ����
        while (1)
        {
            ClearWatchDog();
            result = Get_PowerON_Message();//��ȡ����PLAY����Ϣ���߲���USB�ߣ������ʱ����Ϣ
            switch (result)
            {
                case RESULT_POWERON:
                case RESULT_UDISK:
                RecoverPara();//�ָ���Ƶ��Ȼ�󿪻�
                //DMA3Init(0); //���³�ʼ��DMA3���������������
                StandbyScreen(1);//�����⣬����
                SFR_BANK = temp_sfrbank;
                break;
                default:
                break;
            }
            if (result != 0)
            {
                break;
            }
        }

    }
    return result;
#else
    RefreshDisplayPC();
    while(1)
    {
        SIM_Sleep(200);
    }
#endif

}
/********************************************************************************
 * Description : �������ؼ�S3ģʽ����ʱVCC��VDDȫ���Ͽ�
 *
 * Arguments  :
 *
 *
 * Returns     :
 *            ��
 * Notes       :
 *
 ********************************************************************************/
void Enter_SoftStandby(void)
{
    //PMU RTCVDD��Ĵ���������,��Ҫ�ⲿ��Ƶ���߷�����д��һλ��;
    //disable VCC/VDD
    uint8 count = 0;
    SFR_BANK = 0x01;
    //�رյ�PIN��Ƶ������ʡ8��9��UA�Ĺ��ġ�
    LOSC1_CTL = (uint8 ) (LOSC1_CTL & 0xfe);
    LOSC2_CTL = (uint8 ) (LOSC2_CTL & 0xfe);
    while (count < 4)
    {
        SFR_BANK = 0x05;
        SYSTEM_CTL_RTCVDD = (uint8 ) (SYSTEM_CTL_RTCVDD & 0xfe);
        time_sleep(6);
        count++;
    }

}
/********************************************************************************
 * Description : ����Ӳ���أ���S2ģʽ��ϵͳ�ܵ��ǵ�Ƶ32K
 *
 * Arguments  :
 *
 *
 * Returns     :
 *            ��
 * Notes       :
 *
 ********************************************************************************/
void Enter_HardStandby(void)
{
    temp_sfrbank = SFR_BANK;
    SetParaToStandby();
    SetToLOSC();
}
/********************************************************************************
 * Description : ��ʾ�ػ�LOGO
 *
 * Arguments  :
 *
 *
 * Returns     :
 *            ��
 * Notes       :
 *
 ********************************************************************************/
uint8 draw_standby_logo(void)
{
    uint16 i;
    uint16 result;

    #if(MACHINE_MODEL == 1)
    VMRead(&g_comval, VM_SYSTEM, sizeof(g_comval));
    res_fp = ResOpen("ui30.res");
    SetLanguage(g_comval.langid);
    ClearScreen(NULL);//����
    SetAsciiFont(FONT_TYPE_LARGE);
    ResShowMultiString(SHUTDOWN, 255, 16);
    UpdateScreen(NULL);//����
    ResClose(res_fp);
    #endif
    
    res_fp = ResOpen("end.res");
    for (i = 1; i <= 5; i++)
    {
        #if(MACHINE_MODEL == 1)
        result = ResShowPic(i, 0, 40);
        #else
        result = ResShowPic(i, 0, 0);
        #endif
        ClearWatchDog();
        if (result == 0)
        {
            ResClose(res_fp);
            return FALSE; //format error
        }
        //software timer
#ifdef PC
        RefreshDisplayPC();
        SIM_Sleep(200);
#else
        time_delay(1);
#endif

    }

    ResClose(res_fp);
    return TRUE;
}

void time_delay(uint8 time)
{
    uint16 i;
    uint32 timer;
    for (i = 0; i < 10 * time; i++)
    {
        timer = 8000;
        ClearWatchDog();
        while (timer > 0)
        {
            --timer;
        }
    }

}

uint8 time_sleep(uint16 half_sec)
{
    uint8 n = 0; //rtc counter
    uint8 key;
    while (1)
    {
        key = ap_get_message();
        if (key == AP_MSG_RTC)
        {
            n++;
            if (n >= (uint8) half_sec)
            {
                break;
            }
        }
    }
    return 0;
}
void ui_standby_msg(uint16 msgID)
{


    ClearScreen(NULL);
    ResShowMultiString(msgID, 255, 80);
	UpdateScreen(NULL);
    time_delay(8);

}

/********************************************************************************
 * Description : �ָ���Ƶ���ָ��������õ�һЩ����
 *
 * Arguments  :
 *
 *
 * Returns     :
 *            ��
 * Notes       :
 *
 ********************************************************************************/
void RecoverPara(void)
{

    RecoverHOSC();
    MyEI();
    RecoverRESET();
    RecoverUSBEfuse();
    RecoverRTC();
    RecoverNAND();
    //    RecoverPWM();
    RecoverGPIOReg();
    RecoverPMU();
    RecoverAllClock();
    RecoverDIODE();
    //�ָ���Ƶ24M��VDD1.6V
    //��Ҫ�Ƿ�ֹ�����Զ��ػ�ʱ��PLL���6M��VDDΪ1.4V����������û�лָ���
    SetPLL(DEFAULT_FREQ);
    SetVDD(VDDSET_1_6V);
}
/********************************************************************************
 * Description : ����STANDBY֮ǰ��������ؼĴ��������͹���
 *
 * Arguments  :
 *
 *
 * Returns     :
 *            ��
 * Notes       :
 *
 ********************************************************************************/
void SetParaToStandby(void)
{
    DisablePA();
    DisableDAC();
    DisableAIN();
    SetRESET();
    SetUSBEfuse();
    SetRTC();
    SetNAND();
    //    SetPWM();
    SetGPIOReg();
    SetPMU();
    SetAllClock();
    SetDIODE();
    MyDI();
}
void SetDIODE(void)
{
    SFR_BANK = 0x05;
    DIODEBuf = DIODE_CTL;
    DIODE_CTL = (uint8 ) (DIODE_CTL & 0x7f);
}
void RecoverDIODE(void)
{
    SFR_BANK = 0x05;
    DIODE_CTL = DIODEBuf;
    DIODE_CTL = (uint8 ) (DIODE_CTL | 0x80);
    MCU_delay();
}

/********************************************************************************
 * Description : ��Ƶ32K�µ���ʱ�����Ϊ3MS
 *
 * Arguments  :
 *
 *
 * Returns     :
 *            ��
 * Notes       :
 *
 ********************************************************************************/

void LOSC_delay(void)
{
    uint8 time = 0x10;
    while (1)
    {
        time--;
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        ClearWatchDog();
        if (time == 0)
        {
            break;
        }

    }
}
/********************************************************************************
 * Description : MCU24Mʱ����ʱ
 *
 * Arguments  :
 *
 *
 * Returns     :
 *            ��
 * Notes       :
 *
 ********************************************************************************/
void MCU_delay(void)
{
    uint8 time = 0xff;
    while (1)
    {
        time--;
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        ClearWatchDog();
        if (time == 0)
        {
            break;
        }
    }
}
void SetRESET(void)
{
    SFR_BANK = 0x01;
    RESETBuf[0] = MRCR1;
    RESETBuf[1] = MRCR3;
    RESETBuf[2] = MRCR2;
#ifndef PC
    if(StorageInfor.ce_info[0].type != 3) 
	{
	     //bit4 set to 1:nand flash controller,DMA5 controller&file system accelerator reset to normal
	    MRCR1 = 0x10;
	    //bit3 set to 1,this bit should be reset before USB reset bit is reset,bit2 set to 1,PWM controller reset to normal
	    MRCR3 = 0x0e;
	    //bit3 set to 1,USB&DMA6 reset to normal
	    MRCR2 = 0x08;
	}
	else  //card base
	{
	    //bit5 set to 1: SD card
	    MRCR1 = 0x20;
	    //bit3 set to 1,this bit should be reset before USB reset bit is reset,bit2 set to 1,PWM controller reset to normal
	    MRCR3 = 0x0e;
	    //bit3 set to 1,USB&DMA6 bit 2: DMA4
	    MRCR2 = 0x0C;
	}
#endif

    _nop_();
    _nop_();
    _nop_();
}
void RecoverRESET(void)
{
    SFR_BANK = 0x01;
    MRCR1 = RESETBuf[0];
    MRCR3 = RESETBuf[1];
    MRCR2 = RESETBuf[2];
    _nop_();
    _nop_();
    _nop_();
}
void SetUSBEfuse(void)
{
    SFR_BANK = 0x07;
//    USBBuf[0] = USB_Efuse_Ref;
    USBBuf[1] = USB_PHYCTRL;
//    USB_Efuse_Ref = (uint8 ) (USB_Efuse_Ref | 0x40);//bit6 set to 1, efuse is standby,and its standby current is 5UA max
    USB_PHYCTRL = (uint8 ) (USB_PHYCTRL | 0x80);//enable phy_PLLEn
    USB_PHYCTRL = (uint8 ) (USB_PHYCTRL | 0x40);//enable phy_dallallen
    _nop_();
    _nop_();
}
void RecoverUSBEfuse(void)
{
    SFR_BANK = 0x07;
//    USB_Efuse_Ref = USBBuf[0];
    USB_PHYCTRL = USBBuf[1];
    _nop_();
    _nop_();
}
void SetRTC(void)
{
    SFR_BANK = 0x0c;
    RTCBuf[0] = RTC_CTL0;
    RTCBuf[1] = WDCtl;
    RTCBuf[2] = RTC_CTL1;
    RTC_CTL0 = (uint8 ) (RTC_CTL0 & 0x7f);
    WDCtl = (uint8 ) (WDCtl | 0x08);
    WDCtl = (uint8 ) (WDCtl & 0x7f);
    RTC_CTL1 = (uint8 ) (RTC_CTL1 & 0xbf);
    RTC_CTL1 = (uint8 ) (RTC_CTL1 & 0x7f);
    _nop_();
    _nop_();
}
void RecoverRTC(void)
{
    SFR_BANK = 0x0c;
    RTC_CTL0 = RTCBuf[0];
    WDCtl = RTCBuf[1];
    RTC_CTL1 = RTCBuf[2];
    _nop_();
    _nop_();
}
void SetNAND(void)
{
    SFR_BANK = 0x06;
    NANDBuf[0] = PADDRV1;
    NANDBuf[1] = PADDRV0;
    NANDBuf[2] = PADPUPD0;
    NANDBuf[3] = PADPUPD1;
    PADDRV1 = 0x0;
    PADDRV0 = 0x0;
    PADPUPD0 = (uint8 ) (PADPUPD0 & 0xfb);
    PADPUPD0 = (uint8 ) (PADPUPD0 & 0xf7);
    PADPUPD1 = (uint8 ) (PADPUPD1 & 0x7f);
    _nop_();
    _nop_();
}
void RecoverNAND(void)
{
    SFR_BANK = 0x06;
    PADDRV1 = NANDBuf[0];
    PADDRV0 = NANDBuf[1];
    PADPUPD0 = NANDBuf[2];
    PADPUPD1 = NANDBuf[3];
    _nop_();
    _nop_();
}
/*--------------------------------PWM---------------------------------------
 //PWM��Standbyʱ�����������
 //1.PWM��������������ƣ�ֻ��Ҫ��clockԴ�е�LOSC�����ҽ�ģ��resetд"0"
 //2.�����ʱ��PWM�������ı��⣬�����Standbyʱ��PWMѡ��LOSC��PWMDUTY�Ĵ���д��0x00��
 //  PWM����������0",����һֱ���رգ��رյ�ʱ���ο�LCD��standby����
 */
void SetPWM(void)
{
    SFR_BANK = 0x06;
    PWMBuf = PWMDUTY;
    PWMDUTY = 0x0;
}
void RecoverPWM(void)
{
    SFR_BANK = 0x06;
    PWMDUTY = PWMBuf;
}
void SetGPIOReg(void)
{

    uint8 i = 0;
    SFR_BANK = 0x06;
    //GPIP_A
    GPIOBuf[i] = GPIOAOUTEN;
    i++;
    GPIOAOUTEN = 0xff;

    GPIOBuf[i] = GPIOAINEN;
    i++;
    GPIOAINEN = 0x0;

    GPIOBuf[i] = GPIOADAT;
    i++;
    GPIOADAT = 0xff;
    MCU_delay();

    //GPIP_B
    GPIOBuf[i] = GPIOBOUTEN;
    i++;
    GPIOBOUTEN = 0xff;

    GPIOBuf[i] = GPIOBINEN;
    i++;
    GPIOBINEN = 0x0;

    GPIOBuf[i] = GPIOBDAT;
    i++;
    GPIOBDAT = 0xff;

#if 0   //����GPIOC ��Ϊ���ر����ã��������ﲻ�ܶ�����в���������������һ��
    MCU_delay();
    GPIOBuf[i] = GPIOCOUTEN;
    i++;
    GPIOCOUTEN = 0xff;
    GPIOBuf[i] = GPIOCDAT;
    i++;
    GPIOCDAT = (uint8 ) (GPIOCDAT | 0xfd);
    GPIOCDAT = (uint8 ) (GPIOCDAT & 0xfd);
#endif
    MCU_delay();

    //GPIP_D
    GPIOBuf[i] = GPIODOUTEN;
    i++;
    GPIODOUTEN = 0xff;

    GPIOBuf[i] = GPIODINEN;
    i++;
    GPIODINEN = 0x0;

    GPIOBuf[i] = GPIODDAT;
    i++;
    GPIODDAT = 0xff;
    MCU_delay();

    //GPIP_E
    GPIOBuf[i] = GPIOEOUTEN;
    i++;
    //GPIOE4Ϊ����⣬��standbyʱ���뽫E4�����disable,
    //������Ͽ�ʱ��Ӳ���ϻὫE4��ֱ���������أ�
    //�Ӷ�����STANDBY��������
    GPIOEOUTEN = 0xef;

    GPIOBuf[i] = GPIOEINEN;
    i++;
    GPIOEINEN = 0x0;
    GPIOBuf[i] = GPIOEDAT;
    i++;
    //  GPIOEDAT |= 0xe8;
    //GPIOEDAT &= 0xe8;
    GPIOEDAT = 0xff;
    MCU_delay();

    //GPIP_F
    GPIOBuf[i] = GPIOFOUTEN;
    i++;
    GPIOFOUTEN = 0xff;

    GPIOBuf[i] = GPIOFINEN;
    i++;
    GPIOFINEN = 0x0;

    GPIOBuf[i] = GPIOFDAT;
    i++;
    GPIOFDAT = 0xff;
    MCU_delay();

    //GPIP_G
    MFPSEL6 = (uint8 ) (MFPSEL6 & 0xfe);
    GPIOBuf[i] = GPIOGOUTEN;
    i++;
    GPIOGOUTEN = 0xff;

    GPIOBuf[i] = GPIOGINEN;
    i++;
    GPIOGINEN = 0x0;

    GPIOBuf[i] = GPIOGDAT;
    GPIOGDAT = 0xff;
    MCU_delay();
}
void RecoverGPIOReg(void)
{

    uint8 i = 0;
    SFR_BANK = 0x06;
    GPIOAOUTEN = GPIOBuf[i];
    i++;
    GPIOAINEN = GPIOBuf[i];
    i++;
    GPIOADAT = GPIOBuf[i];
    i++;
    MCU_delay();

    GPIOBOUTEN = GPIOBuf[i];
    i++;
    GPIOBINEN = GPIOBuf[i];
    i++;
    GPIOBDAT = GPIOBuf[i];
    i++;
    MCU_delay();
#if 0
    GPIOCOUTEN = GPIOBuf[i];
    i++;
    GPIOCDAT = GPIOBuf[i];
    i++;
#endif
    MCU_delay();

    GPIODOUTEN = GPIOBuf[i];
    i++;
    GPIODINEN = GPIOBuf[i];
    i++;
    GPIODDAT = GPIOBuf[i];
    i++;
    MCU_delay();

    GPIOEOUTEN = GPIOBuf[i];
    i++;
    GPIOEINEN = GPIOBuf[i];
    i++;
    GPIOEDAT = GPIOBuf[i];
    i++;
    MCU_delay();

    GPIOFOUTEN = GPIOBuf[i];
    i++;
    GPIOFINEN = GPIOBuf[i];
    i++;
    GPIOFDAT = GPIOBuf[i];
    i++;
    MCU_delay();

    GPIOGOUTEN = GPIOBuf[i];
    i++;
    GPIOGINEN = GPIOBuf[i];
    i++;
    GPIOGDAT = GPIOBuf[i];
    MCU_delay();
}
void SetPMU(void)
{
/*
    uint8 bdg_ctl;
    SFR_BANK = 0x05;
    PMUBuf[0] = BDG_CTL;
    PMUBuf[1] = EFUSE_CTL;
    bdg_ctl = BDG_CTL;
    BDG_CTL = bdg_ctl;
    BDG_CTL = (uint8 ) (BDG_CTL | 0x40);
    BDG_CTL = (uint8 ) (BDG_CTL & 0xE0);
    MCU_delay();
    EFUSE_CTL = (uint8 ) (EFUSE_CTL | 0x40);
*/}
void RecoverPMU(void)
{
/*
    SFR_BANK = 0x05;
    BDG_CTL = PMUBuf[0];
    MCU_delay();
    EFUSE_CTL = PMUBuf[1];
*/}

void SetAllClock(void)
{
    SFR_BANK = 0x01;
    ClockBuf[0] = MCSR1;
    ClockBuf[1] = MCSR2;
    ClockBuf[2] = AUDIOPLL_CTL;
    ClockBuf[3] = MCSR3;
    ClockBuf[4] = MCSR4;
    ClockBuf[5] = MCSR5;
    ClockBuf[6] = MCSR6;
    ClockBuf[7] = MCSR7;
    ClockBuf[8] = MCSR8;
    ClockBuf[9] = MCSR9;
    ClockBuf[10] = MCSR10;
    ClockBuf[11] = MCSR11;
    ClockBuf[12] = MCSR12;

    MCSR1 = 0x0;
    MCSR2 = 0x0;
    AUDIOPLL_CTL = (uint8 ) (AUDIOPLL_CTL & 0xfe);
    MCSR3 = 0x0;
    MCSR4 = (uint8 ) (MCSR4 & 0x03);
    MCSR5 = (uint8 ) (MCSR5 & 0x20);
    MCSR6 = 0x0;
    MCSR7 = 0x0;
    MCSR8 = 0x0;
    MCSR9 = 0x0;
    MCSR10 = 0x0;
    MCSR11 = 0x0;
    MCSR12 = 0x0;
    _nop_();
    _nop_();
    _nop_();
}
void RecoverAllClock(void)
{
    SFR_BANK = 0x01;
    MCSR1 = ClockBuf[0];
    MCSR2 = ClockBuf[1];
    AUDIOPLL_CTL = ClockBuf[2];
    MCSR3 = ClockBuf[3];
    MCSR4 = ClockBuf[4];
    MCSR5 = ClockBuf[5];
    MCSR6 = ClockBuf[6];
    MCSR7 = ClockBuf[7];
    MCSR8 = ClockBuf[8];
    MCSR9 = ClockBuf[9];
    MCSR10 = ClockBuf[10];
    MCSR11 = ClockBuf[11];
    MCSR12 = ClockBuf[12];
}
void SetToLOSC(void)
{
    SFR_BANK = 0x01;
    LOSC1_CTL = (uint8 ) (LOSC1_CTL & 0xfe);
    LOSC2_CTL = (uint8 ) (LOSC2_CTL & 0xfe);
    CPUCLKCTL = (uint8 ) (CPUCLKCTL & 0xfc);
    CPUCLKCTL = (uint8 ) (CPUCLKCTL | 0x00);
    _nop_();
    _nop_();
    _nop_();
    ClearWatchDog();
    //   SetAllClock();
    LOSC_delay();
    HOSC_CTL = (uint8 ) (HOSC_CTL & 0xf7);
    HOSC_CTL = (uint8 ) (HOSC_CTL & 0xfe);
}
void RecoverHOSC(void)
{
    uint8 mcsr_val;
    SFR_BANK = 0x01;
    //    RecoverAllClock();
    HOSC_CTL = (uint8 ) (HOSC_CTL | 0x01); //HOSC Enable
    LOSC_delay();
    _nop_();
    _nop_();
    mcsr_val = (uint8 ) (CPUCLKCTL & 0xfe);//�л�HOSCģʽ
    mcsr_val = (uint8 ) (mcsr_val | 0x02); //MCU clock=24M
    CPUCLKCTL = mcsr_val;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    LOSC1_CTL = (uint8 ) (LOSC1_CTL & 0xfe); //disable LOSC
    _nop_();
    _nop_();   
}
//#endif
/********************************************************************************
 * Description : ��ȡUSB��������״̬��
 *
 * Arguments  :
 *
 *
 * Returns     :dpdm_status=0X40��ʾ�������USB�ߣ�dpdm_status=0X80Ϊ�����
 *            ��
 * Notes       :
 *
 ********************************************************************************/
uint8 getUSBStatus(void)
{
    uint8 sfr_bak;
    uint8 dpdm_status;
    sfr_bak = SFR_BANK;
    SFR_BANK = BANK_USB;
    dpdm_status = (uint8 ) (DPDMCTRL & 0x40);
    SFR_BANK = BANK_PMU;
    dpdm_status = (uint8 ) (dpdm_status | (SYSTEM_VOL & 0x80));
    SFR_BANK = sfr_bak;
    return dpdm_status;
}
/********************************************************************************
 * Description : ��ȡ��������Ϣ
 *
 * Arguments  :
 *
 *
 * Returns     :
 *            RESULT_POWERON������PLAY������
 RESULT_UDISK������USB�߿���
 RESULT_MAIN��������������
 * Notes       :
 *
 ********************************************************************************/
uint8 Get_PowerON_Message(void)
{
    uint8 temp;
    uint8 USBStatus = 0;
    USBStatus = getUSBStatus();//��ȡUSB��������Ϣ
    //��ʼ��PLAY���Ĵ���
    SFR_BANK = BANK_PMU;
    temp = SYSTEM_CTL_RTCVDD;
    temp = temp & 0x3f;
    SYSTEM_CTL_RTCVDD = temp;
    if ((PRESS_DISPLAY & 0x40) > 0) //��ⳤ��PLAY����Ϣ
    {

        SFR_BANK = BANK_PMU;
        if ((BATADC_DATA & 0x7f) < 0x51) //��⵽�͵�ʱ��������
        {
            return 0;
        }
        else
        {
            return RESULT_POWERON;
        }
    }
    else if (USBStatus > 0)
    {
        return RESULT_UDISK;
    }
    else
    {
        return 0;
    }
}
/********************************************************************************
 * Description : �ر����е��ж�
 *
 * Arguments  :
 *
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************/
void MyDI(void)
{
    intBuf[0] = IE0;
    intBuf[1] = AIE;
    IE0 = 0x0;
    AIE = 0x0;
}
/********************************************************************************
 * Description : �ָ��ж�
 *
 * Arguments  :
 *
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************/
void MyEI(void)
{
    IE0 = intBuf[0];
    AIE = intBuf[1];
}
