/*
********************************************************************************
*                       ACTOS
*               device manager head file
*
*                (c) Copyright 2002-2003, Actions Co,Ld.
*                        All Right Reserved
*
* File   : device.h
* By     : Gongee
* Version: 1> v1.00     first version     2003-11-20 11:35
********************************************************************************
********************************************************************************
*/
#ifndef _ADFU_H
#define _ADFU_H
#include "typeext.h"




#define API_GetSysInfo          0x0000+API_ADFU_BaseNoA
#define API_PrepareforOnlineSwitch     0x1000+API_ADFU_BaseNoA

#ifdef __C251__

 void BackupZram2scode(void);      //U��Ҫʹ��0x4000~0x4fff�Ŀռ���buffer���ú����Ѹÿռ�ԭ�д��뱸�ݵ�0x06 page
 void RestoreZram2scode(void);     //�ָ���ʾDriver����BankB���뵽Ĭ��Page 0x07


typedef struct
{
        char filename[11];         //�ļ���8.3
        char type;                 //"H","F","B","I","A","S"
        long DownloadAddr;        //download ��ַ
        long offset;               //�ļ��еĵ�ַ
        long length;              //����byteΪ��λ��512ȡ����
        char Subtype[4];          //������
        long checksum;
}DIR_t;
typedef struct
{
        char filename[11];      //�ļ���8.3
        BYTE attr;
        BYTE reserve1[2];
        WORD version;
        long offset;            //������Ϊ��λ
        long length;            //һ�ֽ�Ϊ��λ
        BYTE reserve2[4];
        DWORD checksum;
}LDIR_t;    //32 bytes

typedef struct
{
        char AFIFlag[4];                        //={'A','F','I',' '};
        WORD VID;                                //�豸vid
        WORD PID;                                //�豸pid
        BYTE Version[4];                        //�汾��x.x.xx
        BYTE Date[4];
        BYTE reserve1[16];
        DIR_t diritem[126];                        //���126
        BYTE reserve2[28];
        long checksum;                            //afi headУ���
}AFIHead_t;        //4k

typedef struct
{
        //magic
        WORD magic;            // must equ. 0xdead
        //ϵͳ��ʼʱ��(����Ϊ��λ)
        //ϵͳ��ǰ��ʱ�����ϵͳ��ʼʱ���ϵͳtime counter��ʱ��
        unsigned long systemtime; 
        unsigned int RTCRate;   //950
        //�Աȶ�ѡ��0~31
        char DisplayContrast;
        //��������ʱ��,Ĭ��ֵΪ0ah=5��
        char LightTime;        
        //standby��ʱ
        char StandbyTime;
        //sleep��ʱ
        char SleepTime;
        //��������, ����:0, Ӣ��:1, ����:2
        char langid;
        //����ģʽ
        char ReplayMode;
        //����ģʽ
        char OnlineMode;
        
        char BatteryType;                       //modify by gongee 2004-05-24 09:21
        char FMBuildInFlag;                     //add battery select and fm select flag

}ComValforFMK_t;

typedef struct
{
        //0
        BYTE LFIFlag[4];                        //={0x55,0xaa,0xf0,0x0f};
        BYTE Version[4];                        //�汾��x.x.xx.xxxx
        BYTE Date[4];                           //xxxx.xx.xx
        WORD PID;                               //xxxx
        WORD VID;                               //xxxx
        long DIRItemCheckSum;                   //���е�240��Ŀ¼�������
        BYTE descriptor[44];                    //firmware������
        BYTE Productor[32];
        BYTE DeviceName[32];
        BYTE reserve0[128];
        //256
        BYTE inf_usbattri[8];
        BYTE inf_usbidentification[16];
        BYTE inf_usbproductver[4];
        BYTE reserve1[4];
        BYTE inf_descriptorlen;                    //must equ. 48
        BYTE inf_descriptortype;                //must equ. 3
        BYTE inf_usbdescripstr[46];
        //256+80
        ComValforFMK_t defaultinf;
        //256+80+17    
        BYTE reserve2[15];                      //����        
        //256+80+15=368
        BYTE asciilen1;
        BYTE inf_mtpmfrinfo[32];
        BYTE asciilen2;
        BYTE inf_mtpprodinfo[32];
        BYTE asciilen3;
        BYTE inf_mtpprodver[16];
        BYTE asciilen4;
        BYTE inf_mtpprodsn[16];
        WORD inf_mtpvid[2];
        WORD inf_mtppid[2];
        
        BYTE reserve3[38];                      //����
        //510
        WORD headchecksum;                      //ǰ510byteУ���
        LDIR_t diritem[240];                    //�߼�Ŀ¼��
}LFIHead_t;        //8k

typedef struct
{
        BYTE jump[4];               //��һ�����
        BYTE ADFUSFlag[4];            //={'A','D','F','U'};
        BYTE type[4];                //"3551"
        WORD version;               //x.x.xx
        BYTE date[4];               //xxxx.xx.xx
        BYTE reserve[2];
        BYTE scode[0x2400-20];
}ADFUS_t;

typedef struct
{
        BYTE jump[4];               //��һ�����
        BYTE BRECFlag[4];            //={'B','R','E','C'};
        BYTE type[4];                //"F644"/"F641"/"F321"/"O---"
        WORD version;               //x.x.xx
        BYTE date[4];               //xxxx.xx.xx
        BYTE reserve[2];
        BYTE scode[0x4000-20-32];
        BYTE BrecInfo[30];
        WORD CheckSum;
}BREC_t;

typedef struct
{
        BYTE jump[4];               //��һ�����
        BYTE FWSCFlag[4];            //={'F','W','S','C'};
        BYTE type[4];                //"F644"/"F641"/"F321"/"O---"
        WORD version;               //x.x.xx
        BYTE date[4];               //xxxx.xx.xx
        BYTE reserve[2];
        BYTE scode[0x3000-20];
}FWSCTask_t;

typedef struct
{
        BYTE jump[4];               //��һ�����
        BYTE HWSCFlag[4];            //={'H','W','S','C'};
        BYTE type[4];                //"0000"
        WORD version;               //x.x.xx
        BYTE date[4];               //xxxx.xx.xx
        BYTE reserve[2];
        BYTE scode[0x3000-20];
}HWSCTask_t;

typedef struct
{
        char FrameType[2];              //'F','W'
        WORD VID;
        WORD PID;
        long FirmwareVersion;           //�ͼ��汾��
        WORD Fwstatus;
        char Productor[32];             //������
        char DeviceName[32];            //�豸��
        BYTE FirmwareDate[4];
        WORD BrecCheckSum[2];           //NandFlash�������е�һ��Brec��У���
        long Logical_cap;               //Flash�߼�������
		BYTE dg_flash_info[32];
        BYTE resevred[40];           //��ֹ����Խ�����
}ADFU_FWScanInfo_t; //160bytes

typedef struct
{
        WORD StgInfor[4];        //Storage ��������Ϣ
        WORD CExCap[8];          //Cap of storages
}STGInfo_t;

typedef struct
{
        char FrameType[2];      //='H','W'
        WORD ICVersion;         //3935,3951
        BYTE SubVersion[2];     //A,B,C,D
        BYTE BromVersion[4];    //such as "3.0.00.8888"
        BYTE BromDate[4];       //such as "20,03,10,14"
        char BootDiskType[8];   //boot disk�Ĵ洢���� "F644"/"F641"/"F321"/"N---"
        STGInfo_t stginfo;
		BYTE flash_ID_table[64];
        WORD NafBscParAddr;
        BYTE resevred[48];
}ADFU_HWScanInfo_t;    //160bytes

typedef struct
{
		BYTE brec_code[4];
		BYTE brecflag[4];
		BYTE brec_size;	// sectors
		BYTE logo_stag; // 1 -- data in brec; 0 -- data out brec
	    WORD logo_size; // normally 40kb, 80 sectors
		WORD logo_checksum;
		BYTE sector_cnt;
		BYTE version[2];
		BYTE ReleaseDate[11];
		WORD MinDataNumPerZone2;
		BYTE BootDiskType[8];
		BYTE reserved[54];	
		BYTE reserved1[4];							
}ADFU_BrecHead_t; //96bytes


typedef struct
{
        //pc���ͨ���жϴ˱�־ȷ���ǽ����޸�ģʽ���ǽ�������ģʽ
        BYTE SysinfoFlag[8];    //'S','Y','S',' ','I','N','F','O'
        ADFU_HWScanInfo_t adfu_hwscaninfo;
        ADFU_FWScanInfo_t adfu_fwscaninfo;
		ADFU_BrecHead_t adfu_brechead;
}ADFU_SysInfo_t;


//��ȡϵͳ����Ӳ����Ϣ
 bool GetSysInfo(ADFU_SysInfo_t *sysinfo);

 bool ADFUInit(void);
 bool ADFURun(void);
 bool ADFUExit(void);
 void ADFUHandle(void);
 BYTE ADFUGetStatus(void);

#endif /* __C251__ */

#ifdef __A251__
mGetSysInfo   macro
        ld      hl,API_GetSysInfo
        rst     RSTBankAPI
        endm
#endif /* __A251__ */
#endif /*_ADFU_H */
