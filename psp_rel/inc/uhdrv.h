/********************************************************************************
*                              AS212A
*                            Module: USB Host
*                 Copyright(c) 2001-2010 Actions Semiconductor,
*                            All Rights Reserved. 
*
* History:         
*      <author>    <time>           <version >             <desc>
*       xiaomaky   2009-12-4 13:54     1.0                build this file 
********************************************************************************/



#ifndef _UHDRV_H
#define _UHDRV_H

#include "string.h"
#include "typeext.h"
#include "port.h"
#include "irq.h"
#include "message.h"
#include "uddrv.h"

/* error  meaning */
#define GENERAL_OK                     0x00
#define GENERAL_ERROR                  0x01
#define DEVICE_NOT_IN                  0x02
#define ENUMERATION_FAIL               0x03
#define BULKSTART_FAIL                 0x04
#define ERROR_NOT_SUPPORT_DEVICE       0X05
#define ERROR_SETUP_OVERTIME           0X06
#define ERROR_CONTROL_IN_DATA          0X07
#define ERROR_STATUS_OVERTIME          0X08
#define ERROR_NOT_SUPPORT_PROTOCOL     0X09
#define ERROR_UNIT_READY_OVERTIME      0X0A
#define ERROR_IN_DATA_WRONG            0X0B
#define ERROR_LUN_BLOCK_SIZE           0X0C
#define ERROR_BUS_RESET_OVERTIME       0X0D

#define ERROR_TRANSFER_BUSY            0X80
#define ERROR_IRQ_OCCUR                0X81
#define ERROR_EP_BUFFER_NOT_EMPTY      0X82  /* not busy, but some endpoint sub-buffer is not empty */
#define ERROR_OTG_STATE                0X83
#define ERROR_EP_STALL                 0X84
#define ERROR_NO_EP_HANDSHAKE		   0X85	

#define ERROR_PARAMETER_IN             0XF0
#define ERROR_MIDIUM_NOT_PRESENT	   0XF1
#define ERROR_MIDIA_CHANGED	   		   0XF2


/* function number  */
#define API_UH_SectorRead              API_UD_SectorRead    //0x00
#define API_UH_SectorWrite             API_UD_SectorWrite   //0x01
//#define API_UH_WordRead                0x02
//#define API_UH_DwordRead               0x03
#define API_UH_Update                  API_UD_Update        //0x04
#define API_UH_GetMaxLun               0x05
//#define API_UH_TestLunReady            0x06
//#define API_UH_TestUdiskReady          0x07
#define API_UH_GetCurrentLun           0x08
//#define API_UH_SetCurrentLun           0x09

//BANK A
#define API_UH_EnableCon               0x0000+API_STG_BaseNoA
#define API_UH_DisableCon              0x1000+API_STG_BaseNoA

#define API_UH_Fs_Data_Bak             0x0001+API_STG_BaseNoA
#define API_UH_Fs_Data_Restore         0x1001+API_STG_BaseNoA

//BANK B
#define API_UH_Init                    API_UD_Init          //0x0000+API_STG_BaseNoB
#define API_UH_Exit                    API_UD_Exit          //0x0001+API_STG_BaseNoB
#define API_UH_GetCap                  API_UD_GetCap        //0x0002+API_STG_BaseNoB                   
#define API_UH_GetSectorSize           API_UD_GET_SCTR_SIZE //0x1002+API_STG_BaseNoB
#define API_UH_SetLUN                  0x0003+API_STG_BaseNoB
#define API_UH_ClearStall              0x0004+API_STG_BaseNoB               


#ifdef __C251__


/*!
 *  structure of read or write info   \n
 *  \li SectorAddress: logical block address by 512BYTE.
 *  \li SectorNumber : sector number.
 *  \li RAMAddress   : RAM address.
 * 
 */
/***********************************************************
�������ƣ�  UH_Init
�������ܣ�  uhost�����ĳ�ʼ����������DRV_Install��ʱ����ã�DRV_Install
            ֮��Ҳ������ʱ���ã�������ȡusb����״̬��ص���Ϣ
�������룺  mode:   not used
�������أ�  0x01       success;
            0xff    û�в���miniA��
            0xfe    ������miniA�ߣ�����û�в���u�̣�����u��û������
            0xfd    �豸��֧��
            //0xfc    ���ݴ������˵���������ܲ���----------��ʱ��ʵ��
            ����    resverd
*************************************************************/
//non_banked byte UH_Init(unsigned char mode);
uint8 UH_Init(char mode);

/***********************************************************
�������ƣ�  UH_Exit
�������ܣ�  uhost�������˳���������DRV_UnInstall��ʱ�����
�������룺
�������أ�
*************************************************************/
void UH_Exit(void);

/***********************************************************
�������ƣ�  UH_SectorRead
�������ܣ�  ����������
�������룺  shro:   �μ�struct UD_RW
�������أ�  0       success;
            0xff    ��ַ��������粻���ڻ�Խ��ĵ�ַ
            0xfe    U���ϱ��˶�ʧ��
            ����    resverd
*************************************************************/
uint8 UH_SectorRead(UD_RW *shro);

/***********************************************************
�������ƣ�  UH_SectorWrite
�������ܣ�  ����д����
�������룺  shro:   �μ�struct UD_RW
�������أ�  0       success;
            0xff    ��ַ��������粻���ڻ�Խ��ĵ�ַ
            0xfe    U���ϱ���дʧ��
            ����    resverd
*************************************************************/
uint8 UH_SectorWrite(UD_RW *shro);

/***********************************************************
�������ƣ�  UH_GetMaxLun
�������ܣ�  ��ȡ������Ŀ
�������룺
�������أ�  0��15   ������Ŀ������Э��涨ȡֵ��Χ��0��15��
                    ע������3������0��3�����ĸ���
            ����    resverd
*************************************************************/
uint8 UH_GetMaxLun(void);

/***********************************************************
�������ƣ�  UH_TestLunReady
�������ܣ�  ���Զ�Ӧ������û�м��ش���
�������룺  lun     �����Եķ�����
�������أ�  0       �÷��������˴���
            0xfa       �÷���û�м��ش���
            0xfc       �����lun������
            ����    resverd
*************************************************************/
//uint8 UH_TestLunReady(unsigned char lun);

/***********************************************************
�������ƣ�  UH_GetCap
�������ܣ�  ��ö�Ӧ���̵Ĵ�С
�������룺  lun     ��Ӧ���̵ķ�����
�������أ�  �����Ĵ�С����512 byteΪ��λ��һ��block����512��u�̣�uhost�����Ὣ��ת������512 byteΪ��λ
            0       error
*************************************************************/
uint32 UH_GetCap(void);

/***********************************************************
�������ƣ�  UH_GetSectorSize
�������ܣ�  ���u��һ��Sector�Ĵ�С
�������룺  lun     ��Ӧ���̵ķ�����
�������أ�  u��һ��Sector�Ĵ�С���������n����һ��Sector�Ĵ�С��n*512 byte
            0       error
*************************************************************/
uint8 UH_GetSectorSize(void);

/***********************************************************
�������ƣ�  UH_TestUdiskReady
�������ܣ�  ���Ե�ǰ����״̬
�������룺
�������أ�  0       �ɹ���miniA���룬u��Ҳ���룬�����ӡ���ʱ���Խ��ж����õ������Ȳ���
            0xff    û�в���miniA��
            0xfe    ������miniA�ߣ�����û�в���u�̣�����u��û������
            0xfd    �豸��֧��
            ����    resverd
ע���Ὣ�����������UH_Init�ļ򻯰棬��Ҫͨ����ѯһЩ�����־λ�����ض�Ӧ��ֵ
*************************************************************/
//uint8 UH_TestUdiskReady(void);

/***********************************************************
�������ƣ�  UH_GetCurrentLun
�������ܣ�  ��ȡ��ǰ����Ĵ��̷�����
�������룺
�������أ�  0��15   ��ǰ����Ĵ��̷�����
            ����    resverd
*************************************************************/
uint8 UH_GetCurrentLun(void);

/***********************************************************
�������ƣ�  UH_SetCurrentLun
�������ܣ�  �趨��ǰ����Ĵ��̷����ţ�������趨�Ļ���Ĭ��Ϊ0
�������룺  lun     ��Ӧ���̵ķ�����
�������أ�  0       success
            0xfc       �����lun������
            0xfd    �豸��֧��
            ����    resverd
*************************************************************/
uint8 UH_SetCurrentLun(unsigned char lun);

/***********************************************************
�������ƣ�  UH_WordRead
�������ܣ�  ��ȡĳһ��lba��ĳ��ƫ������˫�ֽ�����
�������룺  UD_RW
            typedef struct
            {
                dword   lba;        //word���������ڵĵ�ַ
                char    reserv;     //δ��
                void    byteaddr;   //word�������ڵ��ֽ�ƫ��
                byte    reserv;     //δ��
            }UD_RW;
�������أ�  0       success
            1       0xff    ��ַ��������粻���ڻ�Խ��ĵ�ַ
            ����    resverd
*************************************************************/
//int16 UH_WordRead(UD_RW *rwaddr);

/***********************************************************
�������ƣ�  UH_DwordRead
�������ܣ�  ��ȡĳһ��lba��ĳ��ƫ���������ֽ�����
�������룺  UD_RW
            typedef struct
            {
                dword   lba;        //word���������ڵĵ�ַ
                char    reserv;     //δ��
                void    byteaddr;   //word�������ڵ��ֽ�ƫ��
                byte    reserv;     //δ��
            }UD_RW;
�������أ�  0       success
            1       0xff    ��ַ��������粻���ڻ�Խ��ĵ�ַ
            ����    resverd
*************************************************************/
//int16 UH_DwordRead(UD_RW *rwaddr);


/***********************************************************
�������ƣ�  UH_Update
�������ܣ�  �պ�����FS_FClose()�ر�һ���ļ�ʱ������
�������룺
�������أ�
*************************************************************/
void UH_Update(void);


/*
message.h��
#define MSG_UH_IN               0xc0   //U�̲���
#define MSG_UH_OUT              0xc1   //U�̰γ�
#define MSG_UH_UDISK_OUT        0xc2   //�����еĿ��γ�
*/


#endif /* __C251__ */

#endif /*_UHDRV_H */











