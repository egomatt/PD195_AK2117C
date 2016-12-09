/*
********************************************************************************
*                       ACTOS
*                       ACTOS
*                  time define head file
*
*                (c) Copyright 2002-2003, Actions Co,Ld.
*                        All Right Reserved
*
* File   : mem.h
* By     : Gongee
* Version: 1> v1.00     first version     12/4/2003 11:13PM
********************************************************************************
********************************************************************************
*/
#ifndef _MEM_H
#define _MEM_H

#include "actapi.h"
#include "sysdef.h"


#ifndef    USE_NOR_BASE                        // nand base / card base

//vm address
//ϵͳ������
#define         VM_KERNEL       0x0000    //��ap_kercfg����ֵ
#define         VM_SYSTEM       0x0200   //��ap_main����ֵ
#define	        VM_AP_FWCFG     0x0400  //for modify information,��ap_main����ֵ    

//��Ӧ�ó����������ÿ������1.5KB����0.5KB��������exFat ȫĿ¼��Ϣ
#define         VM_AP_MUSIC     0x0800
#define		    VM_AP_AMV	    0x0e00
#define         VM_AP_RECORD    0x1400
#define         VM_AP_VOICE     0x1a00
#define         VM_AP_RADIO     0x2000
#define         VM_AP_View      0x2600         // for picture view fuction  by jacklee
#define         VM_AP_SETTING   0x2c00
#define	        VM_AP_READER    0x3200
#define         VM_AP_READER1   0x3800
#define	        VM_AP_BROWSER	0x3e00		//for browser file
//#define         VM_AP_GAME      0x4400      //game ��1KB�����ñ���exFat ȫ·����Ϣ
#define         VM_AP_GAME      0x4400      //game ����0.5K

#define         VM_RECFILE_INFO    0x4600          //����¼������ָ����ܣ�����0.5K


//#define         VM_AP_TEL_HEAD  0x4800   //for telbook,15K
//#define         VM_AP_TEL_DATA  0x4A00
#define         VM_AP_PEDO      0x4800      
#define         VM_AP_SWIM      0x4C00 

#define         VM_AP_FMREC		VM_AP_READER //����


//ר�Ÿ��ļ�ϵͳʹ��
#define         EPTCLUSBLK_TBLADDR         0x8c00	 //0.5k
#define			VM_FS_FLAG		 0x8e00	 //0.5k 

//VM_FS_FLAG ��Ŵ�������Ϣ��byte0-15���flash�̴�������Ϣ
//byte16-31���card�̴�������Ϣ;//byte32-47���uhost�������̴������ţ�
//������Ϣ��ṹ����:
/*
***************************************************
byte0:UseEptBlkFlag
//bit7-ʹ�ôؿ������
//bit0-�ؿ���п���BLOCK���
//bit1-�ؿ��մؿ���������
//bit2-�ؿ���Ƿ���Ч��ǣ�1��Ч��0��Ч 
//Bit3~6 resever
byte1-2:ClusPreBlock 
byte3-4:BlockClusStartNo
byte5-6:ClusBlkCount
byte7-10:TempBlockClusNum
byte10-11:SpareBlkCount
byte12-15:reserve
****************************************************
*/


//VM_FSYS ���flash�����մ�����,mh����8G����ʱ2k vram����Ų��ˣ����Լ��ڵ�4k
#define         VM_FSYS         0x9000	  //4k ���flash �����մ�����

//VM_FS_CARD ���card�����մ�����

#define         VM_FS_CARD        0xa000	  //4k ���card �����մ����� 

#define         FS_BLOCK_CLUSTER_BITMAP    VM_FSYS  

#define         VM_LYRIC        0xB000

#define         VM_AP_TESTER    0x14000


#ifdef __C251__

//��ȡָ�����ȵ����ݡ�
//pbuf: �������ݵ�Buffer ָ�롣
//Address: vram ��ַ
//Len[1~512]: ��ȡ�ĳ��ȡ�
//�ܴ�zram��uram�ж���Ҳ�ܴ�ipm/idm�ж�����port05����
 bool VMRead(void *pbuf, uint16 address, uint16 len);             //len: �ֽ�Ϊ��λ
 bool VMExpRead(void *pbuf, uint32 address, uint16 len);         //len: �ֽ�Ϊ��λ

//��pbuf �е�����д��VRam, �̶�����Ϊ512Byte��
//pbuf: �������ݵ�Buffer ָ�롣
//Address: vram ��ַ
//����VMWrite�ǳ��������벻ҪƵ���ĵ��ã�һ������ap�˳�����ٶ�Ҫ�󲻸ߵĵط�ʹ�á�
//ֻ�ܴ�zram��uram��д�����ܴ�ipm/idm��д
 bool VMWrite(void *pbuf, uint16 address, uint16 len);
 bool VMExpWrite( void *pbuf, uint32 address, uint16 len);      //address��λΪBytes��len: ����Ϊ��λ

#endif /* __C251__ */

#ifdef __A251__

#define API_VMRead      0x0001+API_VM_BaseNoA
#define API_VMWrite     0x0002+API_VM_BaseNoA
#define API_VMExpRead   0x0003+API_VM_BaseNoA
#define API_VMExpWrite  0x0004+API_VM_BaseNoA



//��������õĺ�
mVMRead			macro
  		MOV WR8,#API_VMRead
        LCALL     RSTBankAPI
        endm

mVMWrite		macro
  		MOV WR8,#API_VMWrite
        LCALL     RSTBankAPI
        endm

mVMExpRead      macro
        MOV WR8,#API_VMExpRead
        LCALL     RSTBankAPI
        endm

mVMExpWrite     macro
        MOV WR8,#API_VMExpWrite
        LCALL     RSTBankAPI
        endm



#endif /* __A251__ */

#else                      // nor base

//vm address, nor base vram�߼��ռ��ȷ��乲20K
//ϵͳ������
#define         VM_KERNEL       0x0000    //����0x40
#define         VM_SYSTEM       0x0040   //����0x40
#define	        VM_AP_FWCFG     0x0080  //����0x100

//��Ӧ�ó����������ÿ������256�ֽڣ���128�ֽ���������exFat ȫĿ¼��Ϣ
#define         VM_AP_MUSIC     0x0180              //����0x100
#define		    VM_AP_AMV	    0x0280
#define         VM_AP_RECORD    0x0380
#define         VM_AP_VOICE     0x0480
#define         VM_AP_View      0x0580        
#define	        VM_AP_READER    0x0680
#define	        VM_AP_BROWSER	0x0780	

#define         VM_AP_RADIO     0x0880             // ����Ҫ����exFAT��Ϣ�� ����0x80

#define         VM_AP_TESTER    0x0900             // ����0x200

//nor base ��ʱ��֧��game�͵绰��Ӧ��
#define         VM_AP_SETTING   0x0B00            // ��δʹ��
#define         VM_AP_READER1   0x0B00           // ��δʹ��
#define         VM_AP_GAME      0x0B00      //game ��1KB�����ñ���exFat ȫ·����Ϣ

#define         VM_AP_TEL_HEAD  0x0B00   //for telbook,15K
#define         VM_AP_TEL_DATA  0x0B00

//����¼������ָ����ȷ���0.5K
#define         VM_RECFILE_INFO    0x0c00


//ר�Ÿ��ļ�ϵͳʹ��
#define         EPTCLUSBLK_TBLADDR         0x0e00	 //0.5k
#define			VM_FS_FLAG		 0x1000	 //0.5k 

//VM_FSYS ���flash�����մ�����,mh����8G����ʱ2k vram����Ų��ˣ����Լ��ڵ�4k
#define         VM_FSYS         0x1200	  // ����4k ���flash �����մ�����

//VM_FS_CARD ���card�����մ�����

#define         VM_FS_CARD        0x2200	  //����4k ���card �����մ����� 

#define         FS_BLOCK_CLUSTER_BITMAP    VM_FSYS  

#define         VM_LYRIC        0x3200                      //  ����6K




#ifdef __C251__

 bool VMRead(void *pbuf, unsigned int address, unsigned int len);

//Ϊ�˺�nand base���ݣ��˴�address����Ϊuint32���ͣ�ʵ����nor base��vram�ռ�û��ô�󣬺�������öԵ�ַ��һ���жϡ�
 bool VMExpRead(void *pbuf, uint32 address, uint16 len);

 bool VMWrite(void *pbuf, unsigned int address, unsigned int len);
 bool VMExpWrite(void *pbuf, uint32 address, uint16 len);


#endif /* __C251__ */

#ifdef __A251__


#define API_VMRead      0x0001+API_VM_BaseNoA
#define API_VMWrite     0x0002+API_VM_BaseNoA
#define API_VMExpRead   0x0003+API_VM_BaseNoA
#define API_VMExpWrite  0x0004+API_VM_BaseNoA



//��������õĺ�
mVMRead			macro
  		MOV WR8,#API_VMRead
        LCALL     RSTBankAPI
        endm

mVMWrite		macro
  		MOV WR8,#API_VMWrite
        LCALL     RSTBankAPI
        endm

mVMExpRead      macro
        MOV WR8,#API_VMExpRead
        LCALL     RSTBankAPI
        endm

mVMExpWrite     macro
        MOV WR8,#API_VMExpWrite
        LCALL     RSTBankAPI
        endm
        
#endif/* __A251__ */


#endif/*USE_NOR_BASE*/

#endif/*_MEM_H*/


