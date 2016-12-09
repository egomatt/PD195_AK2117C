#ifndef _DRIVER_H
#define _DRIVER_H

#include "sysdef.h"
#include "actapi.h"
#include "TypeExt.h"


//****************************
//  ������������
#define DRV_VM          0x00    //�����ڴ�����
#define DRV_STG         0x01    //�û��洢������
#define DRV_KY          0x02    //��������
#define DRV_UI          0x03    //��ʾ����
#define DRV_FS          0x04    //�ļ�ϵͳ����
#define DRV_SDFF        0x05    //SD����������
#define DRV_I2C         0x06    //I2C����
#define DRV_SDRAM       0x07    //USB Host����
#define DRV_MFS			0x08    //MFS for data exchange
//****************************
// �û��洢������
#define STG_NAF         0x00            //nand type flash
#define STG_SMC         0x10            //smc
#define STG_MMC         0x20            //mmc
#define STG_UHOST       0x50            //���U��
#define STG_CFC         0x30            //cfc
#define STG_HD          0x40            //hd
#define STG_NAF_SB      0x00 | STG_NAF  //small block nand type flash
#define STG_NAF_MB      0x01 | STG_NAF  //small block nand type flash
#define STG_NAF_LB      0x02 | STG_NAF  //small block nand type flash    644
#define STG_NAF_LB1     0x03 | STG_NAF  //large block 1 nand type flash  648
#define STG_NAF_LB2     0x04 | STG_NAF  //large block 2 nand type flash  645  Toshiba MLC Flash(284)
#define STG_NAF_LB3     0x05 | STG_NAF  //large block 3 nand type flash  024
#define STG_NAF_LB4     0x06 | STG_NAF  //large block 4 nand type flash  285  Samsung MLC Flash(284)
#define STG_NAF_LB5     0x07 | STG_NAF  //large block 5 nand type flash  286  Hinix   MLC Flash(284)
#define STG_NAF_LB6     0x08 | STG_NAF  //large block 6 nand type flash  287  Micron  MLC Flash(284)
#define STG_NAF_LB7     0x09 | STG_NAF  //large block 7 nand type flash  288  ST      MLC Flash(284)
#define STG_NAF_LB8     0x0A | STG_NAF  //large block 7 nand type flash  289  Samsung MLC 2RB(284)
#define STG_NAF_LB9     0x0B | STG_NAF  //large block 7 nand type flash  28A  Hinix   MLC 2RB(284)
#define STG_NAF_LBA     0x0c | STG_NAF  //LBA nand type flash            28B  Toshiba MLC 2RB(284)

#ifdef __C251__

//��������ͷ��Ϣ

typedef struct
{
        int8    filetype;               //�ļ����� 'D'
        uint8    drvtype;                //��������
        uint16    RcodeStartAddr;         //��פ�ڴ�������ʼ��ַ
        uint16    RcodeLength;            //��פ�ڴ����γ���
        uint16    Rcode2StartAddr;         //��פ�ڴ�������ʼ��ַ
        uint16    Rcode2Length;            //��פ�ڴ����γ���
         uint16    RdataStartAddr;         //��פ�ڴ�������ʼ��ַ
        uint16    RdataLength;            //��פ�ڴ����γ���
        uint16    InitAPI;                //������ʼ������api
        uint16    ExitAPI;                //�����رճ���api
        uint32   BnkAOffset;             //��������banka�������ļ��еĵ�ַ
        uint32   BnkBOffset;             //��������bankb�������ļ��еĵ�ַ
} DRV_HEADINFO; 
 

//��װ��������
//para: �����ļ����������������
//ret: ��������
 int8 DRV_Install(const int8 *filename, uint8 param);

//ж����������
//para: ��������(��ǰ�涨��)
//ret:  true/false
 bool DRV_UnInstall(uint8 drivertype);

//̽������洢�豸�Ƿ����
//para: ������������
//ret:  ��������(��ǰ�涨��) 0xff means no ud detected��0x20 if(UDNo==1) means has card, 0xfe if(UDNo==1) means capacity err
 uint8 DRV_DetectUD(uint8 UDNo);

//��������洢�豸�Ƿ����
//para: �����������ţ�0-f��������ģʽ��ͨ��=0��
//ret:  true/false
 bool DRV_ActiveUD(uint8 UDNo,uint8 workmod);


//̽���ļ�ϵͳ�����Ƿ���ڣ������ڣ����ط���״̬���ļ�ϵͳ���ͣ��Ƿ��ʽ����
//para: �߼���������
//  'A','B'  --------------------------����
//  'C','D','E','F'--------------------on board flash
//  'H','I','J','K'--------------------�ƶ��洢�豸
//  ��������
//ret:  bit7: 0/1 �Ƿ���ڣ�
//      bit6: 0/1 ��ʽ����
//      bit5-bit0 fs���� 0: fat12 1: fat16,2 fat32
 int8 DRV_DetectDrive(int8 letter);


//�����ļ�ϵͳ�����Ƿ���ڣ������ڣ����ط���״̬���ļ�ϵͳ���ͣ��Ƿ��ʽ����
//para: �߼��������ţ�����ģʽ��ͨ��=0��
//ret:  true/false
 bool DRV_ActiveDrive(int8 letter,uint8 workmod);


//Mtp�м����ļ�ϵͳ�����Ƿ���ڣ������ڣ����ط���״̬���ļ�ϵͳ���ͣ��Ƿ��ʽ����
//para: �߼��������ţ�����ģʽ��ͨ��=0��
//ret:  true/false
 bool DRV_MActiveDrive(int8 letter,uint8 workmod);


//������������ʽ����֧��FAT16��FAT32��ʽ����֧�ֵ��̸�ʽ������ʽ���������������������ײ���ʼ����
//������MBR��ֻ��DBR����֧�ַ���
//DiskACapacity: ���̵���������������ʾ
//type:��ʽ�����ļ�ϵͳ���ͣ�1��ʾFAT16��2��ʾFAT32
//ret:  true/false
 bool DRV_FormatDrive(uint32 DiskACapacity,int8 type,int8 cardornot);

//ret: high uint8=WorkMode:����ģʽ��0=MODE_FAST,1=MODE_SLOW��  low uint8=UDNo:�����������ţ�0-f��
 uint16 DRV_GetActiveUD(void);


 bool DRV_FFInstall(const int8 *filename, uint8 param);



/*
*********************************************************************************************************
*Function    ��active drive and initialize FS        2008-9-19 14:45
*
*Description��used specially for data copying
*
*Arguments    : letter   ---- drive letter, such as C--Flash, H--Card, L--HideDisk.
*             workmode ---- high uint8 is slow mode or fast mode,for data copying, default fast mode, MODE_FAST��that is 0x00.
*                           low uint8 is source disk or destination disk.
*
*Rerurns    ��0            failed
*              1            successful
*
*Other        :
*********************************************************************************************************
*/
 bool DRV_SActiveDrive(int8 letter, uint16 workmode);


#endif /*  __C251__ */

#ifdef __A251__

#define API_DRV_Install         0x0000 + API_DRV_BaseNoB
#define API_DRV_UnInstall       0x1000 + API_DRV_BaseNoB
#define API_DRV_DetectUD        0x2000 + API_DRV_BaseNoB
#define API_DRV_GetActiveUD     0x4000 + API_DRV_BaseNoB
#define API_DRV_ActiveUD        0x3000 + API_DRV_BaseNoB

#define API_DRV_DetectDrive     0x0001 + API_DRV_BaseNoB
#define API_DRV_ActiveDrive     0x1001 + API_DRV_BaseNoB
#define API_DRV_MActiveDrive    0x2001 + API_DRV_BaseNoB    //for mtp use
#define API_DRV_SActiveDrive    0x3001 + API_DRV_BaseNoB
#define API_DRV_FFInstall       0x4001 + API_DRV_BaseNoB

#define API_DRV_CheckCard       0x0002 + API_DRV_BaseNoB
#define API_DRV_CheckContain    0x1002 + API_DRV_BaseNoB

#define API_DRV_FormatDrive     0x0003 + API_DRV_BaseNoB
#define API_DRV_FormatFat16     0x1003 + API_DRV_BaseNoB
#define API_DRV_FormatFat32     0x0004 + API_DRV_BaseNoB

mDRV_Install    macro
        MOV         WR8,#API_DRV_Install
		LCALL	 RSTBankAPI
        endm
mDRV_UnInstall  macro
        MOV         WR8,#API_DRV_UnInstall
		LCALL	 RSTBankAPI
        endm
mDRV_DetectUD  macro
        MOV         WR8,#API_DRV_DetectUD
		LCALL	 RSTBankAPI
        endm                  
mDRV_GetActiveUD  macro
        MOV         WR8,#API_DRV_GetActiveUD
		LCALL	 RSTBankAPI
        endm    
mDRV_ActiveUD  macro
        MOV         WR8,#API_DRV_ActiveUD
		LCALL	 RSTBankAPI
        endm           
mDRV_FFInstall  macro
        MOV         WR8,#API_DRV_FFInstall
		LCALL	 RSTBankAPI
        endm  
        
mDRV_DetectDrive  macro
        MOV         WR8,#API_DRV_DetectDrive
		LCALL	 RSTBankAPI
        endm           
mDRV_DRV_ActiveDrive  macro
        MOV         WR8,#API_DRV_ActiveDrive
		LCALL	 RSTBankAPI
        endm                     
        
mDRV_CheckCard      macro
        MOV         WR8,#API_DRV_CheckCard
		LCALL	 RSTBankAPI
        endm
mDRV_CheckContain      macro
        MOV         WR8,#API_DRV_CheckContain
		LCALL	 RSTBankAPI
        endm  
        
mDRV_FormatDrive      macro
        MOV         WR8,#API_DRV_FormatDrive
		LCALL	 RSTBankAPI
        endm      
mDRV_FormatFat16      macro
        MOV         WR8,#API_DRV_FormatFat16
		LCALL	 RSTBankAPI
        endm
mDRV_FormatFat32      macro
        MOV         WR8,#API_DRV_FormatFat32
		LCALL	 RSTBankAPI
        endm                         
        
#endif /* __A251__ */

#endif /*_DRIVER_H */
