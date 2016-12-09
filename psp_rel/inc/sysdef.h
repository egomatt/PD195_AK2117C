/*
********************************************************************************
*                       ACTOS
*                  system define
*
*                (c) Copyright 2002-2003, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sysdef.h
* By     : Gongee
* Version: 1> v1.00     first version     2003-11-20 14:38
********************************************************************************
********************************************************************************
*/
#ifndef _SYSDEF_H
#define _SYSDEF_H

//#define SDK35FD
#define EccSupport
//#define USE_EXTERNAL_RTC
#define ColorSrcreen

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�˴�����actos�汾
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define SysVer  0x30

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�˴�����SD������
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define SD_CAP              0x2000000//0x1e00000                //system disk cap,4096KB. flash block�ı���, ��ȡ128KB
#define VM_CAP              0x080000//0x1200000                //vram & hds locationarea: 8MB. ��Ϊflash block�����ı���, ��ȡ128KB
#define MI_CAP              0x0//0x1440000                //mtp infor area cap: 0x1440 sector
#define UI_CAP              0x002000//0x02000                //usb disk infor area cap

#define DEVCERTOFFSET       0x000080                //DEVCERT��ʼƫ�ƣ�����Ϊ��λ��ռ��16K, (VM_CAP��) 0~0x7f��ϵͳVRAM
#define SYNCLISTOFFSET      0x0000a0                //SYNCLIST��ռ��0.5K
#define SECURETIMEOFFSET    0x0000a1                //SECURETIME��ռ��0.5K
#define SECCLKCHLGOFFSET    0x0000a2                //SECCLKCHLG��ռ��0.5K
#define SYNCLISTDYNAMIC     0x0000a3                //SYNCLIST��ռ��20K
#define SECCLKRESPOFFSET    0x0000a3                //SECCLKRESP��ռ��20K
#define XMLINFOOFFSET       0x0000a3                //XMLINFO��ռ��20K
#define METERCERTOFFSET     0x0000a3                //METERCERT��ռ��20K
#define METERCHLGOFFSET     0x0000a3                //METERCHLG��ռ��20K
#define METERRESPOFFSET     0x0000a3                //METERRESPOFFSET��ռ��20K
#define STATEQUERYOFFSET    0x0000a3                //METERRESPOFFSET��ռ��20K

#define HDSOFFSET           0x000100                //drm hds�ļ���ʼƫ�ƣ�����Ϊ��λ��(VM_CAP��)

#define FlashDiskOffset     0x9000                  //VM_CAP >>9 mtp infor(0x1450) start addr in ud area
#define VMFSOffset          0x8000
#define DrvOffset           0x410                  //(VM_CAP + MI_CAP + UI_CAP)>>9  usb disk start addr in ud area


#define DrvOffset0      DrvOffset                   //�˲�������usbdisk���ļ�ϵͳ��
#define DrvOffset1      0x0000                      //ƫ���ã�����ͬcheckpartition��
#define DrvOffset2      0x0000                      //�ļ�ϵͳ����������������ʹ��
#define DrvOffset3      0x0000                      //xxxxnnnn,����xxxxΪ0~3��ʾƫ�Ƶ�����(drvoffsetmod)
                                                    //nnnn��ʾud�ţ�checkpartition)�������(fs_init)

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ic �汾
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#define ICVer3935
#define ICVer3951

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// chip enable ����
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#ifdef ICVer3935
    #define BootFlashCE     0x06
    #define SMCardCE        0x01
    #define DisplayCE       0x03
    #define ATJxxxx         0x2073
#endif
#ifdef ICVer3951
    #define BootFlashCE     0x01
    #define SMCardCE        0x06
#define DisplayCE       0x05
#endif
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//����DMA��ʱ
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define    WaitState    0x03

#define Watchdog
#ifdef  Watchdog
    #define WatchdogTime        2
#endif

//Ϊ�˼ӿ쿪���ٶȣ�������˱�������У��fontlib(*.$$$)
//#define  CheckFontLib

#endif  /*_SYSDEF_H*/

