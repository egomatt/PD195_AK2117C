#ifndef _PROCESS_H
#define _PROCESS_H

#include "sdfs.h"
#define TWOBANK_INGROUP 1
#define API_BankFLSUH 			0x0000+API_APM_BaseNoB
#ifdef __C251__ 
//ap ͷ�ṹ
typedef struct
{
    uint8    file_type;          //'P'
    uint8    ap_type;            //0:ap_system, 1:ap_user
    int8    magic[4];            //'W', 'G', 0x19, 0x97
    uint8    major_version;      //3, for ACTOS 3.0
    uint8    minor_version;      //0
    uint32    text_offset;		
    uint16    text_length;
    uint16    text_addr;          //in ram �����
    uint32    data_offset;		
    uint16    data_length;
    uint16    data_addr;          //�г�ʼ����ȫ�ֱ���
    uint16	midstart;
    uint16    tsr_length;
    uint16    entry;              //ap ���
    uint8    entry_bank;         //ap ��ʼ�� bank ����
    uint8    banks;              //������bank, MAX 252
}ap_head_t;

 


typedef struct
{
    uint32    bank_offset;      //���ļ��ڵ�ƫ��
    uint16    bank_length;       //MAX 64 KB
    uint16    bank_addr;         //zram ��ַ
}ap_bank_head_t;

/* ap �ļ�ͷ�ṹ, ap�� APBuild Tool �� hex �ļ�����
ap_head_t        ap_head;            //32 bytes
ap_bank_head_t    ap_bank_head[0];    //8 bytes
ap_bank_head_t    ap_bank_head[1];    //8 bytes
...
ap_bank_head_t    ap_bank_head[251];    //8 bytes

//�յ�bank_head ���0xFF, ��ռ��2 Kbytes
*/

//tcb �ṹ
typedef struct
{
    SD_FILE			ap_file;			   //���浱ǰap�ļ���SD���Ŀ�ʼλ��
#ifdef	TWOBANK_INGROUP
    uint8   ap_bank[8];            //���浱ǰap bank ��
#else
    uint8   ap_bank[4];            //���浱ǰap bank ��
#endif
	uint16    midstart;

}tcb_t;

//�л��� BankNo, ���ؾɵ� bank ����
int16 BankSwitch(uint8 BankNO);
//ִ�� strName ap, ����ap�ķ���ֵ
int16 ExecAP(int8 *strName, int16 Param);
void PutTSR(void* pfunc);
void RunTSR(uint8 param);
void KillTSR(void);
void BankFlush(uint8 gindex);
#endif/*__C251__*/

#ifdef __A251__
 
mAPM_ExecAP     macro
        mov      R10,#API_ExecAP
		LCALL	 RSTFastAPI
        endm
mAPM_BankSwitch macro
        mov      R10,#API_BankSwitch
		LCALL	 RSTFastAPI
        endm
mAPM_MIDBankSwitch macro
        mov      R10,#API_MID_BankSwitch
		LCALL	 RSTFastAPI
        endm
#endif /*__A251__*/

#endif