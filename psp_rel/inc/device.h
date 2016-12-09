#ifndef _DEVICE_H
#define _DEVICE_H

#include "comval.h"
#include "actapi.h"


#define API_GetSDCap            0x0000+API_DEVICE_BaseNoA
#define API_GetVMCap            0x1000+API_DEVICE_BaseNoA
#define API_GetMICap            0x2000+API_DEVICE_BaseNoA
#define API_GetUICap            0x3000+API_DEVICE_BaseNoA
#define API_GetExtRtcFlag       0x4000+API_DEVICE_BaseNoA
#define API_GetDrmSupportInfo   0x5000+API_DEVICE_BaseNoA
#define API_GetLfiStatus        0x5000+API_DEVICE_BaseNoA
#define API_GetLCDWidth         0x6000+API_DEVICE_BaseNoA
#define API_GetLCDHeight        0x7000+API_DEVICE_BaseNoA

#define API_GetUsbCableStatus   0x0001+API_DEVICE_BaseNoA
#define API_GetUsbAttribute     0x1001+API_DEVICE_BaseNoA
#define API_GetUsbSetupInfo     0x2001+API_DEVICE_BaseNoA
#define API_GetUsbVidPid        0x3001+API_DEVICE_BaseNoA
#define API_GetMtpVidPid        0x4001+API_DEVICE_BaseNoA
#define API_GetHideDiskCap      0x5001+API_DEVICE_BaseNoA
#define API_GetAutoRunDiskCap   0x6001+API_DEVICE_BaseNoA
#define API_JudgeUdiskOperation 0x7001+API_DEVICE_BaseNoA

#define API_GetMtpMfrInfo       0x000e+API_DEVICE_BaseNoA
#define API_GetMtpProdInfo      0x100e+API_DEVICE_BaseNoA
#define API_GetMtpProdVer       0x200e+API_DEVICE_BaseNoA
#define API_GetMtpProdSn        0x300e+API_DEVICE_BaseNoA
#define API_GetAudibleDeviceID  0x400e+API_DEVICE_BaseNoA

#define API_GetBattery          0x0002+API_DEVICE_BaseNoA

#define API_InitDSPCode         0x0003+API_DEVICE_BaseNoA

#define API_StartDsp            0x0004+API_DEVICE_BaseNoA
#define API_StopDsp             0x1004+API_DEVICE_BaseNoA
#define API_SetDspMips          0x2004+API_DEVICE_BaseNoA

#define API_GetWmaTable         0x0005+API_DEVICE_BaseNoA
#define API_GetOggTable         0x1005+API_DEVICE_BaseNoA

#define API_ADC_Enable          0x0006+API_DEVICE_BaseNoA
#define API_ADC_Disable         0x1006+API_DEVICE_BaseNoA
 
 
#define API_DAC_Enable          0x0007+API_DEVICE_BaseNoA
#define API_DAC_Disable         0x1007+API_DEVICE_BaseNoA
#define API_DAC_SetRate         0x2007+API_DEVICE_BaseNoA
#define API_DAC_GetRate         0x3007+API_DEVICE_BaseNoA
#define API_CLD_SetGain         0x4007+API_DEVICE_BaseNoA
#define API_CLD_GetGain         0x5007+API_DEVICE_BaseNoA

#define API_PA_Enable           0x0008+API_DEVICE_BaseNoA
#define API_PA_Disable          0x1008+API_DEVICE_BaseNoA
#define API_PA_SetVolume        0x2008+API_DEVICE_BaseNoA
#define API_PA_GetVolume        0x3008+API_DEVICE_BaseNoA
#define API_PADDV_Enable        0x4008+API_DEVICE_BaseNoA
#define API_PADDV_Disable       0x5008+API_DEVICE_BaseNoA
#define API_CLD_Enable          0x6008+API_DEVICE_BaseNoA
#define API_CLD_Disable         0x7008+API_DEVICE_BaseNoA


#define API_AI_Enable           0x0009+API_DEVICE_BaseNoA
#define API_AI_Disable          0x1009+API_DEVICE_BaseNoA
#define API_AI_SetGain          0x2009+API_DEVICE_BaseNoA
#define API_AI_GetGain          0x3009+API_DEVICE_BaseNoA

#define API_GetFwVersion        0x000a+API_DEVICE_BaseNoA
#define API_GetDefaultComval    0x000b+API_DEVICE_BaseNoA
#define API_GetFileHideOpt      0x200b+API_DEVICE_BaseNoA
#define API_GetMSCVolumeLabel   0x300b+API_DEVICE_BaseNoA
#define API_GetMTPVolumeLabel   0x400b+API_DEVICE_BaseNoA

#define API_GetFMTag            0x000c+API_DEVICE_BaseNoA

#define API_InitDSPCodeExp      0x000d+API_DEVICE_BaseNoA
 
//////////////////////////////////////////////////////////
#define API_UH_POWEROFF         0x000F+API_DEVICE_BaseNoA
//////////////////////////////////////////////////////////
#define API_USB_ISR_DEFAULT     0x0010+API_DEVICE_BaseNoA
#define API_USBH_CON_ENABLE     0x1010+API_DEVICE_BaseNoA
#define API_USBH_CHECK_UDISK    0x2010+API_DEVICE_BaseNoA

#define API_COM_ISR_DEFAULT     0x0011+API_DEVICE_BaseNoA
#define API_SETPLL     			0x0012+API_DEVICE_BaseNoA
#define API_SETVDD     			0x1012+API_DEVICE_BaseNoA
#define API_SPINOR_FW_UPGRD     0x0013+API_DEVICE_BaseNoA
#define FS_8K           8       //8k sample rate
#define FS_11K025       11      //11.025k sample rate
#define FS_12K          12      //12k sample rate
#define FS_16K          16      //16k sample rate
#define FS_22K05        22      //22.05k  sample rate
#define FS_24K          24      //24k sample rate
#define FS_32K          32      //32k sample rate
#define FS_44K1         44      //44.1k sample rate
#define FS_48K          48      //48k sample rate
#define FS_96K			96
#define FS_88K2			88

#define  AD_MICIN   0
#define  AD_FMIN    1
#define  AD_RESERVE 2
#define  AD_MIX		3 

#define ADC_Stereo 		 	0x0C
#define ADC_LeftChanel   	0x08
#define ADC_RightChanel  	0x04

#define ADC_LFPGain_Neg6db 	0x00
#define ADC_LFPGain_Neg3db 	0x20
#define ADC_LFPGain_0db    	0x40
#define ADC_LFPGain_3db	   	0x60

#define ADC_FIFOMCU		 0x00
#define ADC_FIFODMA		 0x10
#define ADC_FIFOAHC		 0x20

#define ADC_NOTRIM		 0x00
#define ADC_TRIM		 0x01

#define ADC_16BIT		 0x00
#define ADC_21BIT        0x80

#define ADC_FIFOLEVEL_1416 0x00
#define ADC_FIFOLEVEL_1016 0x01
#define ADC_FIFOLEVEL_0616 0x02
#define ADC_FIFOLEVEL_0216 0x03

 
#define ADC_DGain_00db    	0x00
#define ADC_DGain_06db		0x40
#define ADC_DGain_12db		0x80

#define CLD_Gain_06db      0x00
#define CLD_Gain_12db      0x40
#define CLD_Gain_18db      0x80
#define CLD_Gain_24db      0xc0 

#ifdef __C251__
#define Get_USBStatus(tmpval,retval){tmpval=SFR_BANK;SFR_BANK = BANK_USB;retval=DPDMCTRL & 0x40; SFR_BANK=BANK_PMU; retval|=(SYSTEM_VOL & 0x80);SFR_BANK=tmpval;}
/*
********************************************************************************
* Description : usb
********************************************************************************
*/
typedef struct
{
        uint16 VID;            //default��0x10d6
        uint16 PID;            //default��0x1000
}vidpid_t;
typedef struct
{
        int8 usbvendor[8];                //default��"Generic"
        int8 usbproductidentification[16];    //default��"USB DISK"�����㲹0x20
        int8 usbproductversion[4];        //default��" 1.00"
        int8 reserve[4];
}usbattri_t;
typedef struct
{
        int8 descriptorlen;             //=sizeof(usbsetupinfo_t);      //�ýṹ����
        int8 descriptortype;            //default��0x03
        uint16 unicodestring[23];        //default��"USB Mass Storage Class"��unicode��ʽ�����㲹0x20,0x00
        int8 reserve[16];
}usbsetupinfo_t;

typedef struct
{
        uint32 version;              //3.1.23.4567   => 0x31,0x23,0x45,0x67
        uint32 date;                 //2004/03/11    => 0x20,0x04,0x03,0x11
}fwversion_t;





//��ȡU���Ƿ����  bi7:DC5V״̬,1:DC5VΪ��,0��DC5VΪ��;bi6:���USB�Ƿ�����,1:��,0:��.
 uint8 GetUsbCableStatus(void);

//��ȡU�����ԣ��糧����Ϣ��������Ϣ�����ڰ汾
 bool GetUsbAttribute(usbattri_t *usbattri);

//��ȡU�̰�װ��ʾ��Ϣ
 bool GetUsbSetupInfo(int8* udisk_serial_num);

//��ȡU��VID&PID
 bool GetUsbVidPid(vidpid_t* vidpid);

//��ȡ�ͼ��汾��
 bool GetFwVersion(fwversion_t *fwver);

 bool GetDefaultComval(comval_t *dcomval);
 bool GetFMTag(int8 *FmTagAddr);
 
 //��ȡU�̾��
 bool GetMSCVolumeLabel(int8* msc_volume_label);

/*
********************************************************************************
* Description : mtp
********************************************************************************
*/
//--------------------------------------------------------------------
typedef struct
{
        uint8    InfoUniCharNum;         //Unicode�ַ��������32
        uint16    InfoData[33];            //Unicode
}mtpinfo_t;



 bool GetMtpMfrInfo(mtpinfo_t *mtpinfo);

 bool GetMtpProdInfo(mtpinfo_t *mtpinfo);

 bool GetMtpProdVer(mtpinfo_t *mtpinfo);
 bool GetMtpProdSn(mtpinfo_t *mtpinfo);
 bool GetMtpVidPid(vidpid_t* vidpid);
 void GetAudibleDeviceID(int8* id);

 int32 GetHideDiskCap(void);
 int32 GetAutoRunDiskCap(void);
 
 /*
********************************************************************************
*       JudgeUdiskOperation                       2010-12-31 17:25
*
* Description :record operations of Udisk mode, such as read, write, format and so on.
*
* Arguments   :GetorSet ---- 0, get operation status
*                            1, clear opertion status.
*
* Returns     :0    ---- read only.
*              else ---- bit0 == 1, UDisk write.
*                        bit4 == 1, MTP write.
*
* Notes       :
*
* By          : wanghaijing         2010-12-31 17:25       record UDisk operations             
********************************************************************************
*/
char JudgeUdiskOperation(char GetorSet);

void USB_Default_isr_Handle(void);
uint8 USB_CON_ENABLE(void);
void UH_Poweroff(void);
bool USBH_Check_Udisk(void);     //����1��ʾ��ǰ��U�̲��룬����0��ʾ��ǰû��U�̲���

//��ȡϵͳ����������������Ϊ��λ
 int32 GetSDCap(void);
//��ȡVram����������������Ϊ��λ
 int32 GetVMCap(void);
//��ȡMtp����������������Ϊ��λ
 int32 GetMICap(void);
//��ȡUSB ��Ϣ����������������Ϊ��λ
 int32 GetUICap(void);
//��ȡ�Ƿ�֧���ⲿRTC�ı�־
 int8 GetExtRtcFlag(void);
//��ȡLCD�Ŀ��
 int16 GetLCDWidth(void);
//��ȡLCD�ĸ߶�
 int16 GetLCDHeight(void);
 /****************************************************************************
 ��ȡLFI״̬���䷵��ֵ��byte��ʾ��ǰ�ж��ٸ�sd������byte��ÿ��bit�ֱ��ʾ��ӦSD��
����0��bit��ʾ��һ��SD�����ĺû���0�ǻ���1�Ǻá��������ǰ�ĺõ�SD������
 ****************************************************************************/
 uint16 GetFWStatus(void);
 
//16������ʾuart������ӡ���� num:����ӡ����
// void UartPutHexNum(uint16 num);

//10������ʾuart������ӡ���� num:����ӡ����
// void UartPutDecNum(uint16 num);

//uart������printf����
// void UartPrintf (const int8 *fmt, ...);
//--------------------------------------------------------------------



/*
********************************************************************************
* Description : battery
********************************************************************************
*/
//��ȡ��ǰ�ĵ���
//para: none
//ret:0 ~ 15 ����ֵ
 uint8 GetBattery(void);

/*
********************************************************************************
* Description : dsp
********************************************************************************
*/
//��ʼ��dsp���������
//para: dspcodename �����ļ�����
//      mode        =0 dsp code
//                  =1 table
//ret:  true/false
 bool DSP_InitCode(const int8 *dspcodename,int8 mode);

//��ʼ��dsp���������
//para: dspcodename �����ļ���
//      mod
//          x  x  x  x  x  x  x  x
//          |_____|  |___________|
//                |              |___bin type
//                |__________________codec type
//ret:  true/false
 bool DSP_InitCodeExp(int8 *DSPFileName,uint8 songtype, uint8 audiotype);
//��ʼ��dsp
//para: mips: 00010:24mips 00011:36mips 00100:48mips 00101:60mips 00110:72mips 00111:84mips
//01000:98m......11111:372m
//ret:  true/false
 bool StartDsp(uint8 mips);
//stop dsp
//para: void
//ret:  true/false
 bool StopDsp(void);
 bool StopDspInUSB(void);
//����dsp�ٶ�
//para: mips: 00:12mips 01:24mips 02:36mips 03:48mips 04:60mips 05:80mips
//ret:  true/false
 bool SetDspMips(uint8 mips);

//��ȡwma��table
//para: uint8 table no
//ret:  true/false
 bool SetDspMips(uint8 tableno);




/*
********************************************************************************
* Description : analog
********************************************************************************
*/
typedef unsigned char rate_t;
typedef unsigned char adin_t;  //ad  input
#define  AD_MICIN   0
#define  AD_FMIN    1
#define  AD_RESERVE 2
#define  AD_MIX		3
typedef unsigned char ain_t;
#define AI_MICGAIN 0
#define AI_FMGAIN  1

/*
typedef struct
{
        int8 micin:1;    //��λΪ1,ai mic select        
        int8 linein:1;   //��λΪ1,fm���뱻ѡ�� only support by 51
        int8 fmin:1;     //��λΪ1,linein���뱻ѡ�� only support by 51
		int8 micin20:1;  //reserve
}ain_t;
*/

typedef struct
{
        adin_t adin;    		//adc ����ѡ��
        uint8 channelmode;      //0ch: Stereo ; 08h: only left ; 04h: only Right
        rate_t rate;    		//������ѡ�� ADC Frequency Sampling:8/16/24/32/48 etc
        uint8 inputgain;        //mic: 0-7 for 20-40db, fm: 0-7 for -3.0 - +7.5db, input source gain��mic¼���Ƽ�ֵ5���Ŵ�50����fm¼���Ƽ�ֵ2�����Ŵ�Ҳ����С
		uint8 lfpgain;			//0: -6 db, 20h: -3db, 40h: 0db, 60h: +3db, adc analog gain���Ƽ�ֵ40h�����Ŵ�Ҳ����С
		uint8 fifoaccess;	   //  ; 0: MCU, 10h: DMA, 20h: AHC, 30h: AHC
		uint8 trimmingflag;	  //  ; 0: no trimming, 01h: trimming
		uint8 FSBit;		   // 0: 16-bit A/D    , 80h: 21-bit A/D
		uint8 fifolevel;	   //0: 14 of 16,01h: 10 of 16, 02h: 6 of 16, 03h: 2 of 16
		uint8 digitalgain;	  //0: 0 db, 40h: +6db, 80h: +12db, c0h: reserve, adc digital gain
}adc_t;

//��ͨ�û�ֻ��Ҫ����ǰ��5�����߼��û������ú���5����Ϊ�����û�ʹ�ã��벹�����������ĺ�����

 

//***********************************************************
typedef struct
{
        int8 dacselect; //0: internal dac 1: external dac       //2003-12-5 10:50
        rate_t rate;    //dac ������ѡ��
        int8 reserve[2];//����
}dac_t;

//***********************************************************
//      power amplifier input ��ģʽ
//       ����gl3935  ��ֻ֧��dacin : Ҳ����dac������
typedef struct
{
	int8 micin:1;   //��λΪ1,micin���뱻ѡ�� only support by 51
	int8 fmin:1;    //��λΪ1,fm���뱻ѡ�� only support by 51
	int8 linein:1;  //��λΪ1,linein���뱻ѡ�� only support by 51
	int8 dacin:1;   //��λΪ1,dac in select        
}pain_t;


typedef struct
{
        pain_t pa_in;  //����ѡ��
        int8 volume;    //����ѡ��
        int8 reserve[2];//����
}pa_t;

//����Analog analog
 bool EnableAIN(ain_t ain);
//�ر�Analog analog
 void DisableAIN(void);
//����Analog analog
 bool SetAINGain(ain_t ain, uint8 gain);
//����Analog analog
 uint8 GetAINGain(ain_t ain);

//����Audio ADC
 bool EnableADC(adc_t *adcattr);
//�ر�Audio ADC
 void DisableADC(void);
 


//�����ڲ�/�ⲿDAC
 bool EnableDAC(dac_t *dacattr);
//�ر��ڲ�/�ⲿDAC
 void DisableDAC(void);
//�ر��ڲ�/�ⲿDAC
 bool SetDACRate(rate_t rate);
//��ȡdac sample rate
 rate_t GetDACRate(void);

//�����ڲ�pa
 bool EnablePA(pa_t *paattr);
//�ر��ڲ�pa
 void DisablePA(void);
//����pa����
 bool SetPAVolume(uint8 volume);
//��ȡpa����
 uint8 GetPAVolume(void);
//����ֱ��pa
 bool EnablePADDV(pa_t *paattr);
//�ر�ֱ��pa
 void DisablePADDV(void);
 void EnableCLD(void);
//�ر�ֱ��pa
 void DisableCLD(void);

//���� CLD gain
 void SetCLDGain(uint8 gain);
//��ȡ CLD gain
 uint8 GetCLDGain(void);
// SetPLL
void SetPLL(uint8 pllsel);
// SETVDD
void SetVDD(uint8 vddsel);
void SpiNor_FW_Upgrd(void);
#endif /* __C251__ */

#ifdef __A251__
mGetUsbCableStatus   macro                      //��ȡU���Ƿ����  0:û����   ����:����
        MOV  WR8, #API_GetUsbCableStatus
        LCALL RSTBankAPI
        endm
mGetUsbAttribute   macro                        //��ȡU�����ԣ��糧����Ϣ��������Ϣ�����ڰ汾
        MOV  WR8, #API_GetUsbAttribute
        LCALL RSTBankAPI
        endm
mGetUsbSetupInfo   macro                        //��ȡU�̰�װ��ʾ��Ϣ
        MOV  WR8, #API_GetUsbSetupInfo
        LCALL RSTBankAPI
        endm
mGetUsbVidPid   macro                           //��ȡU��VID&PID
        MOV  WR8, #API_GetUsbVidPid
        LCALL RSTBankAPI
        endm
mGetBattery     macro
        ld      a,API_GetBattery
        LCALL RSTBankAPI
        endm
mInitDSPCode    macro
        MOV  WR8, #API_InitDSPCode
        LCALL RSTBankAPI
        endm
mInitDSPCodeExp macro
        MOV  WR8, #API_InitDSPCodeExp
        LCALL RSTBankAPI
        endm

//mInitDSPCodeExp2 macro
//        MOV  WR8, #API_InitDSPCodeExp2
//        LCALL RSTBankAPI
//        endm
mStartDsp       macro
        MOV  WR8, #API_StartDsp
        LCALL RSTBankAPI
        endm
mStopDsp        macro
        MOV  WR8, #API_StopDsp
        LCALL RSTBankAPI
        endm
mStopDspInUSB   macro
        MOV  WR8, #API_StopDsp
        LCALL RSTBankAPI
        endm
mSetDspMips     macro
        MOV  WR8, #API_SetDspMips
        LCALL RSTBankAPI
        endm
mGetWmaTable    macro
        MOV  WR8, #API_GetWmaTable
        LCALL RSTBankAPI
        endm
mGetOggTable    macro
        MOV  WR8, #API_GetOggTable
        LCALL RSTBankAPI
        endm


mEnableAIn      macro
        MOV  WR8, #API_AI_Enable
        LCALL RSTBankAPI
        endm
mDisableAIn     macro
        MOV  WR8, #API_AI_Disable
        LCALL RSTBankAPI
        endm
mSetAGain       macro
        MOV  WR8, #API_AI_SetGain
        LCALL RSTBankAPI
        endm
mGetAGain       macro
        MOV  WR8, #API_AI_GetGain
        LCALL RSTBankAPI
        endm
mADC_Enable     macro
        MOV  WR8, #API_ADC_Enable
        LCALL RSTBankAPI
        endm
mADC_Disable    macro
        MOV  WR8, #API_ADC_Disable
        LCALL RSTBankAPI
        endm
 
mDAC_Disable    macro
        MOV  WR8, #API_DAC_Disable
        LCALL RSTBankAPI
        endm
mDAC_SetRate    macro
        MOV  WR8, #API_DAC_SetRate
        LCALL RSTBankAPI
        endm
mDAC_GetRate    macro
        MOV  WR8, #API_DAC_GetRate
        LCALL RSTBankAPI
        endm
mPA_Enable      macro
        MOV  WR8, #API_PA_Enable
        LCALL RSTBankAPI
        endm
mPA_Disable     macro
        MOV  WR8, #API_PA_Disable
        LCALL RSTBankAPI
        endm
mPA_SetVolume   macro
        MOV  WR8, #API_PA_SetVolume
        LCALL RSTBankAPI
        endm
mPA_GetVolume   macro
        MOV  WR8, #API_PA_GetVolume
        LCALL RSTBankAPI
        endm

//mtp
mGetMtpVidPid   macro                      //��ȡU���Ƿ����  0:û����   ����:����
        MOV  WR8, #API_GetMtpVidPid
        LCALL RSTBankAPI
        endm
mGetMtpMfrInfo   macro                        //��ȡU�����ԣ��糧����Ϣ��������Ϣ�����ڰ汾
        MOV  WR8, #API_GetMtpMfrInfo
        LCALL RSTBankAPI
        endm

mGetMtpProdInfo   macro                           //��ȡU��VID&PID
        MOV  WR8, #API_GetMtpProdInfo
        LCALL RSTBankAPI
        endm

mGetMtpProdVer   macro                        //��ȡU�̰�װ��ʾ��Ϣ
        MOV  WR8, #API_GetMtpProdVer
        LCALL RSTBankAPI
        endm
mGetMtpProdSn  macro                           //��ȡU��VID&PID
        MOV  WR8, #API_GetMtpProdSn
        LCALL RSTBankAPI
        endm

mGetAudibleDeviceID  macro                      //��ȡ�豸ID
        MOV  WR8, #API_GetAudibleDeviceID
        LCALL RSTBankAPI
        endm

mGetHideDiskCap  macro                          //��ȡ�����̵�����
        MOV  WR8, #API_GetHideDiskCap
        LCALL RSTBankAPI
        endm

mGetAutoRunDiskCap  macro                       //��ȡAutoRun���̵�����
        MOV  WR8, #API_GetAutoRunDiskCap
        LCALL RSTBankAPI
        endm

mGetSDCap       macro                          //��ȡϵͳ��������
        MOV  WR8, #API_GetSDCap
        LCALL RSTBankAPI
        endm
mGetVMCap       macro                          //��ȡVram��������
        MOV  WR8, #API_GetVMCap
        LCALL RSTBankAPI
        endm
mGetMICap       macro                          //��ȡMtp��������
        MOV  WR8, #API_GetMICap
        LCALL RSTBankAPI
        endm
mGetUICap       macro                          //��ȡUSB ��Ϣ��������
        MOV  WR8, #API_GetUICap
        LCALL RSTBankAPI
        endm
mGetExtRtcFlag  macro                          //��ȡ�Ƿ�֧���ⲿRTC�ı�־
        MOV  WR8, #API_GetExtRtcFlag
        LCALL RSTBankAPI
        endm
mGetLCDWidth  macro							  //��ȡLCD�ĳ���
        MOV  WR8, #API_GetLCDWidth
        LCALL RSTBankAPI
        endm
mGetLCDHeight  macro                          //��ȡLCD�ĸ߶�
        MOV  WR8, #API_GetLCDHeight
        LCALL RSTBankAPI
        endm
mGetFWStatus  macro                          //��ȡLCD�ĸ߶�
        MOV  WR8, #API_GetLfiStatus
        LCALL RSTBankAPI
        endm

mUH_Poweroff macro
       	MOV  WR8, #API_UH_POWEROFF
        LCALL RSTBankAPI
		endm

mUSB_ISR_DEFAULT macro
       	MOV  WR8, #API_USB_ISR_DEFAULT
        LCALL RSTBankAPI
		endm
		
mUSB_CON_ENABLE macro
       	MOV  WR8, #API_USBH_CON_ENABLE
        LCALL RSTBankAPI
		endm

mUSBH_Check_Udisk macro
       	MOV  WR8, #API_USBH_CHECK_UDISK
        LCALL RSTBankAPI
		endm

mCOM_ISR_DEFAULT   macro
       	MOV  WR8, #API_COM_ISR_DEFAULT
        LCALL RSTBankAPI
        endm
        
mGetMSCVolumeLabel  macro                       //��ȡU�̵ľ��     
       	MOV  WR8, #API_GetMSCVolumeLabel
        LCALL RSTBankAPI
		endm             
        
        
#endif /* __A251__ */


#endif