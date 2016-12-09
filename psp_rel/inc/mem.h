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
//系统参数区
#define         VM_KERNEL       0x0000    //由ap_kercfg赋初值
#define         VM_SYSTEM       0x0200   //由ap_main赋初值
#define	        VM_AP_FWCFG     0x0400  //for modify information,由ap_main赋初值    

//各应用程序参数区，每个程序1.5KB，后0.5KB用来保存exFat 全目录信息
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
//#define         VM_AP_GAME      0x4400      //game 仅1KB，不用保存exFat 全路径信息
#define         VM_AP_GAME      0x4400      //game 分配0.5K

#define         VM_RECFILE_INFO    0x4600          //用于录音掉电恢复功能，分配0.5K


//#define         VM_AP_TEL_HEAD  0x4800   //for telbook,15K
//#define         VM_AP_TEL_DATA  0x4A00
#define         VM_AP_PEDO      0x4800      
#define         VM_AP_SWIM      0x4C00 

#define         VM_AP_FMREC		VM_AP_READER //借用


//专门给文件系统使用
#define         EPTCLUSBLK_TBLADDR         0x8c00	 //0.5k
#define			VM_FS_FLAG		 0x8e00	 //0.5k 

//VM_FS_FLAG 存放簇链表信息，byte0-15存放flash盘簇链表信息
//byte16-31存放card盘簇链表信息;//byte32-47存放uhost或隐藏盘簇链表信，
//簇链信息表结构如下:
/*
***************************************************
byte0:UseEptBlkFlag
//bit7-使用簇块表管理标
//bit0-簇块表有可用BLOCK标记
//bit1-簇块表空簇块已用完标记
//bit2-簇块表是否有效标记，1有效，0无效 
//Bit3~6 resever
byte1-2:ClusPreBlock 
byte3-4:BlockClusStartNo
byte5-6:ClusBlkCount
byte7-10:TempBlockClusNum
byte10-11:SpareBlkCount
byte12-15:reserve
****************************************************
*/


//VM_FSYS 存放flash驱动空簇链表,mh由于8G以上时2k vram区存放不了，所以加在到4k
#define         VM_FSYS         0x9000	  //4k 存放flash 驱动空簇链表

//VM_FS_CARD 存放card驱动空簇链表

#define         VM_FS_CARD        0xa000	  //4k 存放card 驱动空簇链表 

#define         FS_BLOCK_CLUSTER_BITMAP    VM_FSYS  

#define         VM_LYRIC        0xB000

#define         VM_AP_TESTER    0x14000


#ifdef __C251__

//读取指定长度的数据。
//pbuf: 接受数据的Buffer 指针。
//Address: vram 地址
//Len[1~512]: 读取的长度。
//能从zram或uram中读，也能从ipm/idm中读，由port05决定
 bool VMRead(void *pbuf, uint16 address, uint16 len);             //len: 字节为单位
 bool VMExpRead(void *pbuf, uint32 address, uint16 len);         //len: 字节为单位

//将pbuf 中的数据写入VRam, 固定长度为512Byte。
//pbuf: 接受数据的Buffer 指针。
//Address: vram 地址
//由于VMWrite非常的慢，请不要频繁的调用，一般是在ap退出或对速度要求不高的地方使用。
//只能从zram或uram中写，不能从ipm/idm中写
 bool VMWrite(void *pbuf, uint16 address, uint16 len);
 bool VMExpWrite( void *pbuf, uint32 address, uint16 len);      //address单位为Bytes，len: 扇区为单位

#endif /* __C251__ */

#ifdef __A251__

#define API_VMRead      0x0001+API_VM_BaseNoA
#define API_VMWrite     0x0002+API_VM_BaseNoA
#define API_VMExpRead   0x0003+API_VM_BaseNoA
#define API_VMExpWrite  0x0004+API_VM_BaseNoA



//定义汇编调用的宏
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

//vm address, nor base vram逻辑空间先分配共20K
//系统参数区
#define         VM_KERNEL       0x0000    //分配0x40
#define         VM_SYSTEM       0x0040   //分配0x40
#define	        VM_AP_FWCFG     0x0080  //分配0x100

//各应用程序参数区，每个程序256字节，后128字节用来保存exFat 全目录信息
#define         VM_AP_MUSIC     0x0180              //分配0x100
#define		    VM_AP_AMV	    0x0280
#define         VM_AP_RECORD    0x0380
#define         VM_AP_VOICE     0x0480
#define         VM_AP_View      0x0580        
#define	        VM_AP_READER    0x0680
#define	        VM_AP_BROWSER	0x0780	

#define         VM_AP_RADIO     0x0880             // 不需要保存exFAT信息， 分配0x80

#define         VM_AP_TESTER    0x0900             // 分配0x200

//nor base 暂时不支持game和电话本应用
#define         VM_AP_SETTING   0x0B00            // 并未使用
#define         VM_AP_READER1   0x0B00           // 并未使用
#define         VM_AP_GAME      0x0B00      //game 仅1KB，不用保存exFat 全路径信息

#define         VM_AP_TEL_HEAD  0x0B00   //for telbook,15K
#define         VM_AP_TEL_DATA  0x0B00

//用于录音掉电恢复，先分配0.5K
#define         VM_RECFILE_INFO    0x0c00


//专门给文件系统使用
#define         EPTCLUSBLK_TBLADDR         0x0e00	 //0.5k
#define			VM_FS_FLAG		 0x1000	 //0.5k 

//VM_FSYS 存放flash驱动空簇链表,mh由于8G以上时2k vram区存放不了，所以加在到4k
#define         VM_FSYS         0x1200	  // 分配4k 存放flash 驱动空簇链表

//VM_FS_CARD 存放card驱动空簇链表

#define         VM_FS_CARD        0x2200	  //分配4k 存放card 驱动空簇链表 

#define         FS_BLOCK_CLUSTER_BITMAP    VM_FSYS  

#define         VM_LYRIC        0x3200                      //  分配6K




#ifdef __C251__

 bool VMRead(void *pbuf, unsigned int address, unsigned int len);

//为了和nand base兼容，此处address定义为uint32类型，实际上nor base的vram空间没那么大，函数中最好对地址做一个判断。
 bool VMExpRead(void *pbuf, uint32 address, uint16 len);

 bool VMWrite(void *pbuf, unsigned int address, unsigned int len);
 bool VMExpWrite(void *pbuf, uint32 address, uint16 len);


#endif /* __C251__ */

#ifdef __A251__


#define API_VMRead      0x0001+API_VM_BaseNoA
#define API_VMWrite     0x0002+API_VM_BaseNoA
#define API_VMExpRead   0x0003+API_VM_BaseNoA
#define API_VMExpWrite  0x0004+API_VM_BaseNoA



//定义汇编调用的宏
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


