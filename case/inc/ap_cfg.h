/*
*******************************************************************************
*                ACTOS AP
*        ap config header  file
*
*        (c) Copyright, Actions Co,Ld.
*             All Right Reserved
*
*  $Id: ap_cfg.h,v 1.7 2009/07/30 14:54:29 mikeyang Exp $
*******************************************************************************
*/
#ifndef AP_CFG_H
#define AP_CFG_H

#ifdef MAIN_AP
#include "cfg_MainAP.h"   
#endif

#ifdef UDISK_AP
#include "CFGUDISK_AP.h"
#endif

#ifdef MUSIC_AP
#include "cfgmusic_ap.h"
#endif

#ifdef RADIO_AP
#include "cfgRADIO_AP.h"
#endif

#define     DEFAULT_FREQ            PLL_48MHZ   //PLL_24MHZ     //matt140312#1


#define     MACHINE_MODEL           3   //'1' SC903: ��ѹ2
										//'2' PD195: 
										//'3' PD196: 
										
#define     LCD_TYPE                5   //'1' SSD1303 RiT
                                        //'2' SSD1306 UG
                                        //'3' SSD1306 UG New
                                        //'4' SSD1306 UG w/ DC-DC
                                        //'5' 132X64 sh1106g
                                        
#define     Volume_Ctl              1   //'0' 40=600mv
										//'1' 32=250mv  //����
										//'2' 27=140mv sc903
										
#define     FM_BAND                 1   //'1' 87.5~108
										//'2' 76~108    STAYER
										//'3' 76~90	    Greenhouse

//#define     STAYER                      //Stayer:
//#define     Greenhouse                   
//#define     NetOnNet                    //ui30.res���� SLAN25, �����ʾ


#if(MACHINE_MODEL==1)																				
#define     EARPHONE_ON             0   //" " = on level
#else
#define     EARPHONE_ON             1   //" " = on level
#endif
										
#define     PETOMETER_USE           1
#define     PETOMETER_METHOD        2   //'1' no HPF; '2" with HPF
#define     SWIM_USE                1   //SWIM FITNESS
/*#define     G_TYPE                1   //'0' no use, no pedometer
                                        //'1' MXC62025M
										//'2' MMA8452Q
										//'3' LIS3DH    */

#define     USB_EMI                     //USB_EMI

#define     WEIGHT_MAX              500


										
//define this flag when release      
//#define SupportCard              // ֧�ֲ忨 
//#define SupportWMAREC          //֧��WMA��ʽ¼�� 
#define Msg_CTC2HZ             //ʹ��CTC��ʱ��2HZ��Ϣ
#define UNICODE_SUPPORT       //֧��unicode�����������
#define NEWFORMATRES          //֧��Excel����Unicode�����¸�ʽRes�ļ�
//#define ALARM_CLOCK             //֧�����ӹ���
//#define AUTO_FMREC              //֧��FM��ʱ�Զ�¼������

#ifdef __C251__
/*�̼�����������ݽṹ����*/
typedef struct
{
     uint8  Total; //֧�ֵ���������,���25������
     uint8  Active;//��ǰĬ�����Ե�ID��
     uint8  Table[62]; //����ID�����飬ÿ�������Ա��ʾ��ͬ���Ե�ID��ȡֵ��ΧΪΪ0-29��0xff��ʾ����β
}lan_set_t;

typedef struct
{
     uint8  Total;      //֧�ֵİ����������������֧��16������
     uint8  Table[16];  //������Ϣֵ�������飬ÿ�������Ա��ʾ��ͬ�İ�����Ϣ
}key_set_t;

typedef struct
{
     uint8  Total;     //֧�ֵ�AP������
     uint8  Active;    //�������˵�Ĭ��ap�ı��
     uint8 Table[16]; //AP��Ӧ�����飬ÿ�������Ա�����ֽڣ���һ����ʾAP���,�ڶ�����ʾAP״̬
}ap_set_t;

typedef struct
{
    uint8  DefValue;  //Ĭ��ֵ
    uint8  Max;      //���ֵ
    uint8  Min;      //��Сֵ 
    uint8  Step;     //����  
}value_set_t;

typedef struct
{
    uint16        Magic;     
    lan_set_t     LanInfor;
    key_set_t     KeyInfor;
    ap_set_t      ApInfor;
    value_set_t   Autolock;
    value_set_t   ContrInfor;
    value_set_t   BacklightTime;
    value_set_t   PowerOfftime;
    value_set_t   SleepTime;
    value_set_t   Volume;
    value_set_t   VolDefault;
    value_set_t   RecordGain; 
    value_set_t   Stride;   //pedometer
    value_set_t   PedoUnit;
    value_set_t   Weight;
    value_set_t   ArmLen;
    value_set_t   PoolLen;  
    value_set_t   Record_Mode;
    uint8  DevVesion[32];
	uint8  FirmwareDate[32]; 
	uint8  CardFlag;      //�忨֧�֣�0����֧�ֲ忨��1��֧�ֲ忨
	uint8  EarphoneFlag;  //����ֱ����0����֧�֣�1��֧��	
	uint8  ClassDFlag;	  //D�๦�ţ��������ʱʹ�ã�0����֧�֣�1��֧��	
    uint8  KeyToneFlag;
    uint8  PedoFlag;   //pedometer
}vm_cfg_t;


/*���������ݽṹ����*/
typedef struct
{
    char   CfgMagic[4];
    uint8  TotalItem;
    char   Reserv[11];
}file_cfg_head_t;

typedef struct
{
    uint32 Offset;
    char   EName[14];
    char   CName[14];
}cfg_item_t;  // 32 bytes


/*������������ͷ��Ϣ���ݽṹ����*/
typedef struct
{
    uint32 ValueOffset;
    uint8  ValueTotal;
    uint32 StringOffset;
    uint8  StringTotal;
    char   Reserv[6];
}paraset_head_t;  //16 bytes

/*��ֵ�����������ݽṹ����*/
typedef struct
{
    uint8  DefValue;
    uint8  Max;
    uint8  Min;
    uint8  Step;
    uint8  Reserv[12];
    char   EName[16];
    char   CName[16];
}value_t;  //48 bytes

/*�ַ��������������ݽṹ����*/
typedef struct
{
    char   StringCfg[32];
    char   EName[16];
    char   CName[16];
}string_t; //64 bytes

/*���������������ݽṹ����*/
typedef struct
{
    uint8  TotalKey;
    uint8  TotalItem;
    char   Reserv[14];
    char   KeyValue[16];
}key_cfg_t; //32 bytes

typedef struct
{
    uint8  Value;
    uint8  Status; //0 ��ʾ�����õİ�����1��ʾ��������
    char   Description[14];
}key_infor_t; //32 byt

/*AP������ͷ��Ϣ���ݽṹ����*/
typedef struct
{
    uint8  Total;
    uint8  Active;
    char   Reserv[14];
}ap_cfg_head_t;  //16 bytes

/*ap ������Ϣ���ݽṹ*/
typedef struct
{
    uint8  Index;
    uint8  Status;
    char   EName[15];
    char   CName[15];
}ap_infor_t;  //32 bytes

typedef struct
{
    uint8  Total; //֧�ֵ���������,���25������
    uint8  Active;//��ǰĬ�����Ե�ID��
    uint8  Table[62]; //����ID�����飬ÿ�������Ա��ʾ��ͬ���Ե�ID��ȡֵ��ΧΪΪ0-29��0xff��ʾ����β
}lang_infor_t; //64 bytes

typedef struct
{
    uint8  Total; //��ǰ���滻�Ĺ̼��ļ�
    uint8  Reserv[15]; //����
}file_ex_t; //16 bytes

typedef struct
{
    uint8  Total; //��ǰ�������������
    uint8  Reserv[15]; //����
}func_cfg_t; //16 bytes

typedef struct
{
    uint8  State; //����֧�����״̬��0����֧�֣�1��֧�֡�
    uint8  Reserv[15]; //����
    uint8  EName[16];  //Ӣ����
    uint8  CName[16];  //������
}func_item_t; //48 bytes

#endif      /*end of #ifdef __C251__*/

#endif

