#include "actos.h"
#include "key.h"
#include "ap_common.h"
#include "chip_cfg.h"
#include "ap_cfg.h"
#include "math.h"

#pragma ROM(medium)

extern uint8 usb_plugin;
extern uint8 usb_unstickcount;
extern uint8 usb_stickcount;

extern uint8 usbplugflag;
extern uint8 low_battery_value;
extern uint8 batDetectCnt;
extern uint8 sysverflag;

#pragma renamecode(?PR?KY_USB)


void LowBatDetect(void);

void sKY_CheckUsbStatus(void)
{
    uint8 dpdm_status;
    uint8 sys_vol;
    uint8 usb_plugin_new;
    uint8 sfr_bak = SFR_BANK;

    SFR_BANK = BANK_USB;
    dpdm_status = DPDMCTRL & 0x40;
    SFR_BANK = BANK_PMU;
    sys_vol = (SYSTEM_VOL & 0x80) | (CHG_CTL & 0x80);
    usb_plugin_new = sys_vol;

    if (usb_plugin_new != 0)
    {
        usb_plugin_new = 1;
    }
    else
    {
        batDetectCnt++;
        if (batDetectCnt >= 10)
        {
            batDetectCnt = 0;
            LowBatDetect(); //µ÷ÓÃµÍµç¼ì²â
        }
    }

    if ((usbplugflag == 1) && (usb_plugin & usb_plugin_new))
    {
        usbplugflag = MSG_USB_STICK_FLAG;
    }

    if (usb_plugin_new != usb_plugin)
    {
        if (usb_plugin_new != 0)
        {
            usb_stickcount++;
            if (usb_stickcount > 3)
            {
                usb_plugin = usb_plugin_new;
                usb_unstickcount = 0;
                usbplugflag = MSG_USB_STICK_FLAG;//PutSysMsg(MSG_USB_STICK);
            }
        }
        else
        {
            usb_unstickcount++;
            if (usb_unstickcount > 3)
            {
                usb_plugin = usb_plugin_new;
                usb_unstickcount = 0;
                PutSysMsg(MSG_USB_UNSTICK);
            }
        }
    }

    SFR_BANK = sfr_bak;
}

void LowBatDetect(void)
{
    uint8 sfr_bak;
    uint8 bat_vol;

    sfr_bak = SFR_BANK;
    SFR_BANK = BANK_PMU;
    bat_vol = BATADC_DATA & 0x7f;

    if (bat_vol <= low_battery_value)
    {
        PutSysMsg(MSG_LOW_POWER);
    }
    SFR_BANK = sfr_bak;
}



