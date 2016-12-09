/*
********************************************************************************
*                       ACTOS
*                  usb disk driver head file
*
*                (c) Copyright 2002-2003, Actions Co,Ld.
*                        All Right Reserved
*
* File   : uddrv.h
* By     : Gongee
* Version: 1> v1.00     first version     2003-11-20 11:34
********************************************************************************
********************************************************************************
*/
#ifndef _UDDRV_H
#define _UDDRV_H

#define  MODE_FAST  0x00
#define  MODE_SLOW  0x01

#include "actapi.h"
#include "TypeExt.h"
#include "sysdef.h"
#include "driver.h"
#include "port.h"
#include "irq.h"


#ifdef ICVer3951
    #define EccSupport
#endif

//¼��merge�ӿ�ʹ�õ���һ��ȫ�ֱ����ĵ�ַ���ļ�ϵͳ��������д�����ݣ�������\flash\data\data.c
#define RECODE_MERGE_LBA  0x1652  

#ifdef __A251__
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* for assemble
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define SDCardFFEntry   0x7f80

#define API_UD_SectorRead       0x00
#define API_UD_SectorWrite      0x01
#define API_UD_WordRead         0x02
#define API_UD_DwordRead        0x03
#define API_UD_Update           0x04
#define API_UD_ClosePage        0x05
#define API_UD_FlashType        0x06
#define API_UD_Get_Flash_Page_Size      0x07
// 8 and 9 have used for bred read write
#define API_UD_GetFlashBlockSize        0x0a
#define API_UD_MergeFlash                 0x0b

#define API_UD_CheckReclaim      0x18               //����Ƿ���Ҫ����reclaim

//Udisk����ʹ��
#define API_UD_GetFlashId      0x0c
#define API_UD_UpDataInit      0x0d
#define API_UD_GetChecksum     0x0e
#define API_UD_NandMbrecWrite  0x0f
#define API_UD_NandBrecWrite   0x10
#define API_UD_UpDataCheck     0x11
#define API_UD_MergeSDLog      0x12


#define API_UD_SDCardSectorRead       0x00
#define API_UD_SDCardSectorWrite      0x01
#define API_UD_SDCardWordRead         0x02
#define API_UD_SDCardDwordRead        0x03
#define API_UD_SDCardUpdate           0x04
#define API_UD_SDCardWriteInit        0x05
#define API_UD_SDCardReadInit         0x06
#define API_UD_SDCardGetCap           0x07
#define API_UD_SDCardWPDetect         0x08
#define API_UD_SDCardInit             0x0000 + API_SDCardFF_BaseNoB
#define API_UD_SDCardExit             0x0001 + API_SDCardFF_BaseNoB
#define API_UD_SDCardPhyGetCap        0x0001 + API_SDCardFF_BaseNoA

#define API_UD_Init             0x0000+API_STG_BaseNoB
#define API_UD_Exit             0x1000+API_STG_BaseNoB
#define API_FLASH_RECMERGE      0x2000+API_STG_BaseNoB

#define API_UD_SetHighFreq      0x0001+API_STG_BaseNoB
#define API_UD_SetLowFreq       0x1001+API_STG_BaseNoB

#define API_UD_GetCap           0x0002+API_STG_BaseNoB
#define API_UD_GET_SCTR_SIZE    0x1002+API_STG_BaseNoB

#define API_UD_READ_RECLAIM    0x0003+API_STG_BaseNoB

#define API_UD_PhyGetCap        0x0001+API_STG_BaseNoA
//#define API_UD_GetCap           0x0002+API_STG_BaseNoA
#define API_UD_WPDetect         0x0003+API_STG_BaseNoA
//#define API_UD_SetHighFreq      0x0005+API_STG_BaseNoA
//#define API_UD_SetLowFreq       0x1005+API_STG_BaseNoA
#define API_UD_EblSvMd          0x0006+API_STG_BaseNoA
#define API_UD_DisblSvMd        0x1006+API_STG_BaseNoA
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//for card fast driver

mUD_SDCardInit	macro
        MOV		WR8, #API_UD_SDCardInit
        LCALL  RSTBankAPI
        endm

mUD_SDCardExit  macro
        MOV  WR8,#API_UD_SDCardExit
        LCALL  RSTBankAPI
        endm



//for flash and card
mUD_SectorRead  macro
        MOV		R10, #API_UD_SectorRead
        LCALL     RSTStgAPI
        endm
mGD_SectorRead  macro
        MOV		R10, #API_UD_SectorRead
        LCALL     RSTStgAPI
        endm
        
mUD_SDCardSectorRead macro
		MOV		R10,#API_UD_SDCardSectorRead
		LCALL	FFSDCardAPI
		endm
mUD_SDCardSectorWrite macro
		MOV		R10,#API_UD_SDCardSectorWrite
		LCALL	FFSDCardAPI
		endm
		
mUD_SectorWrite macro
        MOV		R10, #API_UD_SectorWrite
        LCALL     RSTStgAPI
        endm
mGD_SectorWrite macro
        MOV		R10, #API_UD_SectorWrite
        LCALL     RSTStgAPI
        endm

mUD_WordRead  macro
        MOV		R10, #API_UD_WordRead
        LCALL     RSTStgAPI
        endm

mUD_DwordRead   macro
        MOV		R10, #API_UD_DwordRead
        LCALL     RSTStgAPI
        endm

mUD_Update      macro
        MOV		R10, #API_UD_Update
        LCALL     RSTStgAPI
        endm
mGD_Update      macro
        MOV		R10, #API_UD_Update
        LCALL     RSTStgAPI
        endm
mUD_ClosePage   macro
        MOV		R10, #API_UD_ClosePage
        LCALL     RSTStgAPI
        endm
mUD_GetFlashType  macro                 //����ֵ  ��MLC flash:0    MLC flash:1
        MOV		R10, #API_UD_FlashType
        LCALL     RSTStgAPI
        endm
        
mGet_Flash_Page_Size  macro
        MOV		R10, #API_UD_Get_Flash_Page_Size
        LCALL     RSTStgAPI
        endm
        
mUD_GetFlashBlockSize  macro
        MOV		R10, #API_UD_GetFlashBlockSize
        LCALL     RSTStgAPI
        endm
        
mUD_MergeFlash  macro
        MOV		R10, #API_UD_MergeFlash
        LCALL     RSTStgAPI
        endm    

mUD_CheckReclaim  macro
        MOV		R10, #API_UD_CheckReclaim
        LCALL     RSTStgAPI
        endm     


mUD_GetCap macro
        MOV  WR8,#API_UD_GetCap
        LCALL  RSTBankAPI
        endm
mUD_GetSctrSize macro
        MOV  WR8,#API_UD_GET_SCTR_SIZE
        LCALL  RSTBankAPI
        endm
mUD_ReadReclaim macro
        MOV  WR8,#API_UD_READ_RECLAIM
        LCALL  RSTBankAPI
        endm

mUD_WPDetect macro
        MOV  WR8,#API_UD_WPDetect
        LCALL  RSTBankAPI
        endm

mUD_Init        macro
        MOV  WR8,#API_UD_Init
        LCALL  RSTBankAPI
        endm
mUD_Exit        macro
        MOV  WR8,#API_UD_Exit
        LCALL  RSTBankAPI
        endm
        
m_MergeFlash      macro
        MOV  WR8, #API_FLASH_RECMERGE
        LCALL RSTBankAPI
        endm

mUD_SetHighFrequence        macro
        MOV  WR8,#API_UD_SetHighFreq
        LCALL  RSTBankAPI
        endm

mUD_SetLowFrequence         macro
        MOV  WR8,#API_UD_SetLowFreq
        LCALL  RSTBankAPI
        endm
////////////////////////////////////////////////
mUD_GetFlashId  macro
        MOV		R10, #API_UD_GetFlashId
        LCALL     RSTStgAPI
        endm
        
mUD_UpDataInit  macro
        MOV		R10, #API_UD_UpDataInit
        LCALL     RSTStgAPI
        endm
        
mUD_GetChecksum  macro
        MOV		R10, #API_UD_GetChecksum
        LCALL     RSTStgAPI
        endm

mUD_NandMbrecWrite  macro
        MOV		R10, #API_UD_NandMbrecWrite
        LCALL     RSTStgAPI
        endm
        
mUD_NandBrecWrite  macro
        MOV		R10, #API_UD_NandBrecWrite
        LCALL     RSTStgAPI
        endm
        
mUD_UpDataCheck  macro
        MOV		R10, #API_UD_UpDataCheck
        LCALL     RSTStgAPI
        endm

mUD_MergeSDLog  macro
        MOV		R10, #API_UD_MergeSDLog
        LCALL     RSTStgAPI
        endm

#endif /* __A251__ */


#ifdef __C251__

typedef int lw;

typedef struct
{
        uint32        lba;
        int8         reserv;
        uint8         srampage;
        uint16       sramaddr;

}UD_RW;

typedef struct
{
        int8    ceinfo;
        int8    type;
}CE_INFO;

/*cap[0~3] is for nand flash
cap[4] is for the SM card, cap[5] is for the sd/mmc card ,cap[6] is for cf card, CAP[7] is for hd card*/

typedef struct
{
        CE_INFO ce_info[4];
        uint16    cap[8];
}STG_INFO;

//added by dongzh

//**UD_WordRead**//
/*
�������ƣ�  UD_WordRead
�������ܣ�  ����ָ��������ַ����
�������룺  UD_RW *RWStruct
�������أ�  1 success; 0 fail
���ݽṹ��
        typedef struct
        {
            uint32   lba;        //word���������ڵĵ�ַ
            int8    reserv;     //δ��
            void    byteaddr;   //word�������ڵ��ֽ�ƫ��
            uint8    reserv;     //δ��
        }UD_RW;
*/
 int  UD_WordRead(UD_RW *rwaddr);



//**UD_DwordRead**//
/*
�������ƣ�  UD_DwordRead
�������ܣ�  ����ָ��������ַ����
�������룺  UD_RW *RWStruct
�������أ�  1 success; 0 fail
���ݽṹ��
        typedef struct
        {
            uint32   lba;        //dword���������ڵĵ�ַ
            int8    reserv;     //δ��
            void    byteaddr;   //dword�������ڵ��ֽ�ƫ��
            uint8    reserv;     //δ��
        }UD_RW;
*/
 long int UD_DwordRead(UD_RW *rwaddr);



//**UD_SectorRead**//
/*
�������ƣ�  UD_SectorRead
�������ܣ�  ��512 byte���ݵ�ָ���ڴ�λ��
�������룺  UD_RW *RWStruct
�������أ�  0   success;
            1   ��ַ��������粻���ڻ�Խ��ĵ�ַ
            3   ����������Ҫ���¼��
���ݽṹ��
        typedef struct
        {
            uint32   lba;        //LBA��д��ַ����512 BYTEΪ��λ
            int8    endflag;    //���Ʋ���������5A���ڽ���������д������Ϊ0
            void    *sramaddr;  //��д���ڴ��еĵ�ַ
            uint8    srampage;   //�ڴ�ҳ 0/1/2 ipml/m/h, 4/5/6 idml/m/h
        }UD_RW;
*/
 int8 UD_SectorRead(UD_RW *rwaddr);

//**UD_SectorWrite**//
/*
�������ƣ�  UD_SectorWrite
�������ܣ�  д512 byte���ݵ�ָ������ָ������
�������룺  UD_RW *RWStruct
�������أ�  0   success;
            1   ��ַ��������粻���ڻ�Խ��ĵ�ַ
            2   д��������
            3   ����������Ҫ���¼��
���ݽṹ��
        typedef struct
        {
            uint32   lba;        //LBA��д��ַ����512 BYTEΪ��λ
            int8    endflag;    //���Ʋ���������5A���ڽ���������д������Ϊ0
            void    *sramaddr;  //��д���ڴ��еĵ�ַ
            uint8    srampage;   //�ڴ�ҳ 0/1/2 ipml/m/h, 4/5/6 idml/m/h
        }UD_RW;
*/
 int8 UD_SectorWrite(UD_RW *rwaddr);

//**UD_WPDetect**//
/*
�������ƣ�  UD_WPDetect
����������  ���FLASH�Ƿ�д����
�������룺
�������أ�  0 û��д����
            1 д����
���ݽṹ��  ��
*/
 int8 UD_WPDetect(void);

//**UD_GetCap**//
/*
�������ƣ�  UD_GetCap
����������  ���FLASH����
�������룺
�������أ�  HL FLASH��������128KΪ��λ
���ݽṹ��  ��
*/
 uint16 UD_GetCap(void);
/*
*********************************************************************************************************
*                                           UD_GetSctrSize
*
* Description: ��ȡ��ǰ�������̷�һ������������(�����U��һ����������Ϊ512byte��������)
*
* Arguments  : None.
*
* Returns    : a: sector size, 2��ʾΪ1024byte, so on.
*
* Note(s)    : None.
*********************************************************************************************************
*/
uint8 UD_GetSctrSize(void);
//**UD_Update**//
/*
�������ƣ�  UD_Update
����������  д�����һ�����ݵ�FLASH
�������룺
�������أ�  ��
���ݽṹ��  ��
*/
 void UD_Update(void);


//**UD_GetFlashType**//
/*
�������ƣ�  UD_GetFlashType
����������  ȡ��flash �����Ƿ�MLC
�������룺
�������أ� MLC flash:0  MLC flash:1; 2:SPI Nor flash; 3:LBA flash; 4:card base; 5:TLC flash; 6:Toshiba 24nm flash
���ݽṹ��  ��
*/
 char UD_GetFlashType(void);  //����ֵ  ��MLC flash:0    MLC flash:1

//**Get_Flash_Page_Size**//
/*
�������ƣ�  Get_Flash_Page_Size
����������  ȡ��flash page size��С����512�ֽ�Ϊ��λ���������Ƿ�Ϊ1K��д
�������룺
�������أ�  ����ֵ   ���ֽڣ�0��512�ֽڶ�д 1��1024�ֽڶ�д ���ֽڣ�flash����page�ڵ�������512�ֽڣ���
���ݽṹ��  ��
*/
uint16 Get_Flash_Page_Size(void); 


//**Get_Flash_Block_Size**//
/*
�������ƣ�  Get_Flash_Block_Size
����������  ȡ��flash Block size��С����512�ֽ�Ϊ��λ
�������룺
�������أ�  ����ֵ   
���ݽṹ��  ��
*/
uint16 UD_GetFlashBlockSize(void); 

//**UD_MergeFlash**//
/*
�������ƣ�  
��������������merge flash
�������룺
�������أ�  ����ֵ   
���ݽṹ��  ��
*/
uint16 UD_MergeFlash(void);


//**UD_CheckReclaim**//
/*
�������ƣ�  
��������������Ƿ���Ҫ����reclaim
�������룺
�������أ�  ����ֵ   
���ݽṹ��  ��
*/
uint8 UD_CheckReclaim(void);



//**UD_Init**//
/*
�������ƣ�  UD_Init
����������  ��ʼ��FLASH���������ݽṹ
�������룺
�������أ�  ��
���ݽṹ��  ��
*/
 bool UD_Init(int8 mode);

//**UD_Exit**//
/*
�������ƣ�  UD_Exit
����������  ��д���ݽṹ��FLASH
�������룺
�������أ�  ��
���ݽṹ��  ��
*/
 void UD_Exit(void);


//**UD_ReadReclaim**//
/*
�������ƣ�  UD_ReadReclaim
����������  ����read reclaim����
�������룺
�������أ�  ��
���ݽṹ��  ��
*/
 void UD_ReadReclaim(void);

 
/******************************************************************************/
/*!                    
* \par  Description:
*	  description the function  in here
* \param[in]    ���flash id��buffer��8 bytes��
* \param[out]   none    
* \par �������һ����
* \return       none                
*******************************************************************************/ 
void UD_GetFlashId (uint8 *flashid);

/******************************************************************************/
/*!                    
* \par  Description:
*	  U������֮ǰ���ã�׼��һЩ�����ܲ����������дmbrec��brecʹ��,�����ش���Ӧ��ֹͣ����U������
* \param[in]    none
* \param[out]   none    
* \par �������һ����
* \return       1,fail
                0,success                
*******************************************************************************/
uint8 UD_UpDataInit(void);

/******************************************************************************/
/*!                    
* \par  Description:
*	  ��ȡflash��brec/mbrec��checksum
* \param[in]    set 0,mbrec
*                   1,brec
* \param[out]   none    
* \par �������һ����
* \return       ret_checksum-mbrec/brec��checksum                
*******************************************************************************/
uint16 UD_GetChecksum (uint8 set);

/*****************************************************************************
*Name         :
*Description  : ��mbrecд��flash��
*Parameter    :
   uint32 lba:       �������Ϊ��
   uint32 w_sectors: �������Ϊ��
   uchar *w_buf:     д�����ݵ� buffer��ַ
*Return       :
*Note         :mbrec�̶�д��ǰ��3������block��ֻ��Ҫ����һ�Σ��Զ����д��
*****************************************************************************/
//uchar UD_NandMbrecWrite(uint32 lba, uint32 w_sectors, uchar *w_buf);
uchar UD_NandMbrecWrite(UD_RW * udrw);

/*****************************************************************************
*Name         :
*Description  : ��brecд��flash�У�
*Parameter    :
   uint32 lba:  �߼����ַ,����ָ��������� brec��ͷ�����λ�ã�����Ϊ��λ
   uint32 w_sectors: Ҫд��������
   uchar *w_buf:    ������ݵ� buffer��ַ
*Return       :
*Note         :brec�̶�ÿ��8Kд��, �ϲ㲻�ù��ж��ٷ� BREC���²�ᰴ�ն����
               brec�ķ����Զ�������
*****************************************************************************/
uchar UD_NandBrecWrite(UD_RW * udrw);

/******************************************************************************/
/*!                    
* \par  Description:
*	  U�������˳�ǰ���ã�У��д���mbrec��brec�Ƿ�����,�����ش�����U������ʧ��
* \param[in]    none
* \param[out]   none    
* \par �������һ����
* \return       0xff,fail
                0,success                
*******************************************************************************/
uint8 UD_UpDataCheck(void); 



/******************************************************************************/
/*!                    
* \par  Description:
*	  U������������̼�����ã���SD������־�鶼merge��
* \param[in]    none
* \param[out]   none
* \par �������һ����
* \return       0xff,fail
                0,success                
*******************************************************************************/
uint8 UD_NandSDMerge(void);
 

//********************************for SD Card (��������)**************************************

//**UD_SDCardReadInit**//
/*
�������ƣ�  UD_SDCardReadInit
�������ܣ�  ��ʼ��������������������USBģʽ��ʹ��
�������룺  ������������������Ŀ
            ��������������ʼ�����ĵ�ַ
�������أ�  0   success;
���ݽṹ��
        typedef struct
        {
            uint32   lba;        //����������ʼ�����ĵ�ַ
            int8    SecNum;     //��������������Ŀ
            void    *sramaddr;  //��д���ڴ��еĵ�ַ
            uint8    srampage;   //�ڴ�ҳ 0/1/2 ipml/m/h, 4/5/6 idml/m/h
        }UD_RW;
*/

 int8 UD_SDCardReadInit(UD_RW *rwaddr);


//**UD_SDCardWriteInit**//
/*
�������ƣ�  UD_SDCardWriteInit
����������  ��ʼ������д����������ʼ������������
            1)��������ʼ��ַ��ʼ������N��BLK
            2)��������д������
            ����USBģʽ��ʹ��
�������룺  ��������д��������Ŀ
            ��������д����ʼ�����ĵ�ַ
�������أ�  0   success;
            1   ��ַ��������粻���ڻ�Խ��ĵ�ַ
            2   д��������
            3   ����������Ҫ���¼��
���ݽṹ��
        typedef struct
        {
            uint32   lba;        //����д����ʼ�����ĵ�ַ
            int8    SecNum;     //����д��������Ŀ
            void    *sramaddr;  //��д���ڴ��еĵ�ַ
            uint8    srampage;   //�ڴ�ҳ 0/1/2 ipml/m/h, 4/5/6 idml/m/h
        }UD_RW;
*/
 int8 UD_SDCardWriteInit(UD_RW *rwaddr);

//**UD_SDCardSectorRead**//
/*
�������ƣ�  UD_SDCardSectorRead
�������ܣ�  ��512 byte���ݵ�ָ���ڴ�λ��
�������룺  UD_RW *RWStruct
�������أ�  0   success;
            1   ��ַ��������粻���ڻ�Խ��ĵ�ַ
            3   ����������Ҫ���¼��
���ݽṹ��
        typedef struct
        {
            uint32   lba;        //LBA��д��ַ����512 BYTEΪ��λ
            int8    endflag;    //���Ʋ���������5A���ڽ���������д������Ϊ0
            void    *sramaddr;  //��д���ڴ��еĵ�ַ
            uint8    srampage;   //�ڴ�ҳ 0/1/2 ipml/m/h, 4/5/6 idml/m/h
        }UD_RW;
*/
 int8 UD_SDCardSectorRead(UD_RW *rwaddr);

//**UD_SDCardSectorWrite**//
/*
�������ƣ�  UD_SDCardSectorWrite
�������ܣ�  д512 byte���ݵ�ָ������ָ������
�������룺  UD_RW *RWStruct
�������أ�  0   success;
            1   ��ַ��������粻���ڻ�Խ��ĵ�ַ
            2   д��������
            3   ����������Ҫ���¼��
���ݽṹ��
        typedef struct
        {
            uint32   lba;        //LBA��д��ַ����512 BYTEΪ��λ
            int8    endflag;    //���Ʋ���������5A���ڽ���������д������Ϊ0
            void    *sramaddr;  //��д���ڴ��еĵ�ַ
            uint8    srampage;   //�ڴ�ҳ 0/1/2 ipml/m/h, 4/5/6 idml/m/h
        }UD_RW;
*/
 int8 UD_SDCardSectorWrite(UD_RW *rwaddr);

//**UD_SDCardWPDetect**//
/*
�������ƣ�  UD_SDCardWPDetect
����������  ��⿨�Ƿ�д����
�������룺
�������أ�  0 û��д����
            1 д����
���ݽṹ��  ��
*/
 int8 UD_SDCardWPDetect(void);

//**UD_SDCardGetCap**//
/*
�������ƣ�  UD_SDCardGetCap
����������  ��⿨����
�������룺
�������أ�  HL ����������128KΪ��λ
���ݽṹ��  ��
*/
 uint32 UD_SDCardGetCap(void);

//**UD_Init**//
/*
�������ƣ�  UD_Init
����������  ��ʼ��FLASH���������ݽṹ
�������룺
�������أ�  ��
���ݽṹ��  ��
*/
 bool UD_SDCardInit(int8 mode);

//**UD_SDCardExit**//
/*
�������ƣ�  UD_SDCardExit
����������  �رտ�
�������룺
�������أ�  ��
���ݽṹ��  ��
*/
 void UD_SDCardExit(void);

 void UD_SetHighFrequence(void);


 void UD_SetLowFrequence(void);



#endif /* __C251__ */

#endif /*_UDDRV_H */











