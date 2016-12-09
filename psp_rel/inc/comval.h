/*
********************************************************************************
*                       ACTOS
*                  common value define head file
*
*                (c) Copyright 2002-2003, Actions Co,Ld.
*                        All Right Reserved
*
* File   : comval.h
* By     : Gongee
* Version: 1> v1.00     first version     2003-12-2 18:37
********************************************************************************
********************************************************************************
*/
#ifndef _COMVAL_H
#define _COMVAL_H

#include "sysdef.h"
#include "TypeExt.h"

 


//����magic�����ж�vm��������Ƿ�Ϸ�,uint16 ����
#define MAGIC_KVAL              0xdeed
#define MAGIC_COMVAL            0xdead
#define MAGIC_MUSIC             0xbeef
#define MAGIC_VOICE             0xfee0
#define MAGIC_RECORD            0x3d3d
#define MAGIC_FMRADIO           0xad01
#define MAGIC_SETTING           0xbaba
#define MAGIC_UDISK             0xee77
#define MAGIC_TESTER            0x9801
#define MAGIC_STANDBY           0x3935
#define MAGIC_UPGRADE           0x3951
#define MAGIC_MRECORD           0xcc00       //for M-Record by ccm
#define MAGIC_VIEWPIC		0x4a50		//for picture view
#define	MAGIC_READER		0x7478		//for reader "tx"
#define	MAGIC_AMV		0x616d		//for amv file "am"
#define    MAGIC_CFG        0x55aa        //for modify information
//language id
#ifdef PC
#define LAN_ID_SCHINESE        0           //���ļ���
#define LAN_ID_TCHINESE        2           //���ķ���
#endif
//#define LAN_ID_ENGLISH         1           //Ӣ��
//#define LAN_ID_TCHINESE        2           //���ķ���
//#define LAN_ID_JAPANESE        3           //����
//#define LAN_ID_KOREAN         4            //����
//#define LAN_ID_RUSSIAN         5            //����˹��
//#define LAN_ID_RABBINIC        6           //��������
//#define LAN_ID_THAI                7            //̩����
//#define LAN_ID_FRENCH             8           //����
//#define LAN_ID_GERMANY           9          //�¹���
//#define LAN_ID_ITALIAN             10          //�������
//#define LAN_ID_DUTCH              11          //������
//#define LAN_ID_PORTUGUESE        12          //��������
//#define LAN_ID_SPANISH             13         //�������� 
//#define LAN_ID_SWEDISH            14         //�����
//#define LAN_ID_CZECHISH           15        //�ݿ���
//#define LAN_ID_POLAND             16         //������
//#define LAN_ID_FINNISH             17        //������
//#define LAN_ID_DENISH              18         //������
//#define LAN_ID_HUNGARIAN         19         //��������
//#define LAN_ID_SLOVAKIAN         20         //˹�工����
//batt type
#define BATT_TYPE_ALKALINE    0
#define BATT_TYPE_NIH        1
#define BATT_TYPE_LITHIUM    2

#ifdef __C251__

//************************************
//ϵͳ������������
//************************************
typedef struct
{
        //magic
        uint16 magic;    //"KVAL"
        //ϵͳ��ʼʱ��(����Ϊ��λ)
        //ϵͳ��ǰ��ʱ�����ϵͳ��ʼʱ���ϵͳtime counter��ʱ��
        uint32 systemtime;
        uint32 gwtime;
        int8  rtcresetflag;//Ĭ��Ϊ������rtcresetflag=0x00�����rtc����rtcresetflag=0x55
}kval_t;

typedef struct
{
        //magic
        uint16 magic;

        //ϵͳ��ʼʱ��(����Ϊ��λ)
        //ϵͳ��ǰ��ʱ�����ϵͳ��ʼʱ���ϵͳtime counter��ʱ��
        uint32 systemtime;
        //ϵͳʱ�����
        uint16  reserve1;
        //int16 RTCRate;

        //�Աȶ�ѡ��6~21
        uint8 DisplayContrast;

        //��������ʱ��,Ĭ��ֵΪ0ah=5��
        uint8 LightTime;

        //Auto lockʱ��?
        uint8 LockTime;

        //standby��ʱ
        uint8 StandbyTime;

        //sleep��ʱ
        uint8 SleepTime;

        //��������, ����:0, Ӣ��:1, ����:2
        int8 langid;

        //����ģʽ
        uint8 ReplayMode;

        //����ģʽ
        uint8 OnlineMode;

        uint8 BatteryType;                       //modify by gongee 2004-05-24 09:21
        uint8 FMBuildInFlag;                     //add battery select and fm select flag

        //¼������,0:����¼��(V-Record),1:����¼��(M-Record). add by ccm 2004-06-08
        uint8 RecordType;

        //�������ɫ  7Color BackLight. For s2 by ccm
        uint8 BLightColor;
        
        //bit0~bit1: Udisk���к�֧��:0:no sn; 1:unified sn; 2:random sn;
        //bit4: Udisk��֤ģʽ֧��: 1:����Udisk��֤ģʽ,����ʱ��ӿ�,֧��suspend��resume��USB��֤����
        int8 udisk_setting;
        
  	    uint8 Lightmode;	//0: �䰵ģʽ 1: ���ģʽ	
        uint8 SuppCard;      //֧�ֿ�ѡ��  0:��֧��  1:֧��
        
        uint8 MTPFormatType; //��MTP�¸�ʽ����������ѡ��  0:FAT16  1:FAT32


        uint8 FMTag;
        
//        int8 EarProtect_flag;
//        int8 EarProtectThreshold;
//        int8 attenuation_flag;
//        int16  auto_switchoff_time;

		uint8 KeyTone;				//add by mzh 2007.3.14 0: no key tone 1:have key tone
        uint8 BackLightMax;  //����������ȵ���
        uint8 BackLightDef;  //Ĭ�ϱ�������

        uint8 VolumeMax;    //�������������
        uint8 VolumeDef;    //Ĭ������ֵ
        
        uint8 Volume_G;
 
		
        uint8 ClassDFlag;   //D�๦��֧��
        uint8 EarphoneFlag; //����ֱ��֧�� 
        uint8 RecordGain;
        
        
				uint8 Boxit;  //������
				uint8 Tetris;  //����˹����
				uint8 Snaks;   //̰����
				uint8 Riddle;   //���ݵ�
				uint8 playpowerofflag;
		
		uint8 rec_bitrate;//¼��������
		
		uint8 Stride;   //pedometer
        uint8 PedoUnit;   
        uint16 Weight; 
        uint8 ArmLen; 
        uint8 PoolLen; 
        uint8 PedoFlag;
        uint8 Record_Mode;
//        int8 reserve[32-30];
}comval_t;

//��ʼ�� comval  �ĺ�
#define COMVAL_INIT(val) \
do{ \
        (val).DisplayContrast = 11; \
        (val).langid = LAN_ID_SCHINESE; \
        (val).LightTime = 10; \
        (val).LockTime = 10; \
        (val).magic = MAGIC_COMVAL; \
        (val).ReplayMode = 0; \
        (val).SleepTime = 0; \
        (val).StandbyTime = 30; \
        (val).OnlineMode = 0; \
        (val).systemtime = 0L; \
        (val).BatteryType = 3; \
        (val).FMBuildInFlag = 0; \
        (val).RecordType = 0; \
        (val).BLightColor = 4; \
        (val).Onlinedev = 0; \
        (val).Stride = 0;\
        (val).PedoUnit = 0;\
        (val).Weight = 0;\
        (val).ArmLen = 0;\
        (val).PoolLen = 0;\
        (val).Record_Mode = 0;\
//        (val).EarProtect_flag = 0; \
//        (val).EarProtectThreshold = 22; \
//        (val).attenuation_flag = 0; \
//        (val).auto_switchoff_time = 0; \
}while(0)

#endif/*__C251__*/


#endif/*_COMVAL_H*/
