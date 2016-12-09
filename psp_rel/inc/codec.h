/*
*******************************************************************************
*                ACTOS AP
*        codec decleare file, ap donot use this file, use basal.h
*
*        (c) Copyright, Actions Co,Ld.                             
*             All Right Reserved                          
*
*******************************************************************************
*/ 

#ifndef CODEC_H
#define CODEC_H

//��������

//¼���ļ��ĸ�ʽ, riff �� wFormatTags
#define    WAVE_FORMAT_ACTIONS_PCM        0x81
#define    WAVE_FORMAT_ACTIONS_ADPCM            0x82
#define    WAVE_FORMAT_ACTIONS_AG4                0x83
#define    WAVE_FORMAT_ACTIONS_AG8                0x84

//״̬����
#define    CODEC_STATUS_OK    0        //����
#define    CODEC_STATUS_ERROR    1        //����
#define    CODEC_STATUS_READ    2        //codec��Ҫ������
#define    CODEC_STATUS_WRITE    3        //codec��Ҫд����
#define    CODEC_STATUS_REACHB    4        //���ŵ���b��
#define    CODEC_STATUS_MAXLEN    5    
#define    CODEC_STATUS_VORON        6
#define    CODEC_STATUS_VOROFF        7
#define    CODEC_STATUS_FORMAT_ERR        0xf0    //�ļ���ʽ����

#define    CODEC_STATUS_SETA            0x33    //codec�����A���ƫ��
#define    CODEC_STATUS_SETB            0x77    //codec�����B���ƫ��

#define    CODEC_STATUS_READ_BACKWD         0x22
#define    CODEC_STATUS_READ_FORWD          0x11

#define    CODEC_STATUS_WRITE_TAIL        0x00
#define    CODEC_STATUS_WRITE_HEAD        0xFF

//�����
#define    CODEC_CMD_NORMAL        0x00        
#define    CODEC_CMD_FF            0x55
#define    CODEC_CMD_RW            0xAA

#define    CODEC_CMD_SETA          0x22
#define    CODEC_CMD_SETB          0xAA
#define    CODEC_CMD_BACKA    0x88

#define CODEC_CMD_PLAY_NORMAL    0x00
#define CODEC_CMD_PLAY_RESTORE    0x01

#ifdef PC
#define __C251__
#endif


#ifdef __C251__
#ifndef PC
#include <typeext.h>
#include <time.h>
#include <basal.h>
#else
#include "typeext.h"
#include "time.h"
#include "basal.h"
#endif

//#define CODEC_TEST

#define PLAY_MODE_NORMAL 0
#define PLAY_MODE_BREAK 1
#define PLAY_MODE_CALTIME 2
#define PLAY_MODE_AB 3

#define FAST_FORWARD 0x55
#define FAST_BACKWARD 0xaa
#define STEP_NORMAL 0x00

#define AB_CLEAR 0x00
#define AB_SET_A 0x22
#define AB_SET_B 0xaa

#define DOUBLE_CHANNEL 0
#define LEFT_CHANNEL 1
#define RIGHT_CHANNEL 2

//���ݰ��˺������ָ��
typedef struct
{
    WORD read;    //!=NULL, using interrupt mode
    WORD write; //==NULL, using pooling mode  
    WORD Fread;
}codec_hook_t;

//ad,ag,wma decoder use
typedef struct
{
//    WORD ms;
    BYTE second;
    BYTE minute;
    BYTE hour;
}codec_time_t;

typedef struct
{
    BYTE Hms;
    BYTE second;
    BYTE minute;
}decInitTime_t;
typedef struct
{
    WORD    BitsreamBuff1; //comment,1page buffer, 512 bytes/page
    WORD    BitsreamBuff2; //End address of page buffer
    BYTE    SongType; //song type 0:mp3  1:wma  02:adpcm/pcm
    BYTE    PlayMode; //00:normal decode, 01:break point play,02:only compute time info,03:play from begin to end
    long    FileLen; //file length
    WORD    HookRead; //���ļ�����
}decInitPara_t;

typedef struct
{
    BYTE            SampleRate; //�����ʣ�kHZ
    BYTE            Channel; //ͨ������1����ͨ����2��˫ͨ��  
    DWORD           BitRate;  //�����ʣ����byteΪ0����3btye����Ϊ���١�ʮ����λ������λ��kbps����ʵʱ�ı�
    Full_time_t     TotalPlayTime;  //������ʱ�䣬���byteΪ0����3btye����Ϊ��ʱ���֡��� 
    DWORD           FileHDRLen;//�ļ�ͷ���ȣ���λ��btye����basal�ṩ
}decSongInfor_t;

typedef struct
{   
    BYTE            Status;//ʵʱ����״̬��0����������0���쳣
    BYTE            GetSongInfo; // 0x00:not yet; 0xff: get song info successfully
    BYTE            EnergyLevel;//��ǰ��������0-63����
    BYTE            SetenceFlag;//��ǰ�Ͼ��־��0���޶Ͼ䣬��0����⵽�Ͼ�    
    DWORD           NowTime;//��ǰ����ʱ�䣬��byteΪ0����3btye����Ϊ��ʱ���֡���
}decCurStatus_t;    

typedef struct
{
    BYTE        SongType; //��������, 0: mp3, 1: wma, 2: wav
    BYTE        PlayMode; //����ģʽ�� 0: �������ţ�1���ϵ������� 2�����������Ϣ
    BYTE        Volume;     // ��ǰ��������Ӳ�������PA����һ�£������������    
    BYTE        FFRevFlag; // ����˱�־��0���޿���ˣ�0x55�������0xaa������
    BYTE        FFRevSpeed; // ������ٶȣ�0-5�����Ƽ�ֵΪ4
    BYTE        ABSetFlag; // AB play flag 0x22:set a  0xaa:set b 0xff:deal AB play process
    BYTE        PlaySpeed; // play speed control
    BYTE        SoftVolumeMax; // ��������ģʽ��0��Ӳ��������0������������󼶱�����������
    BYTE        EQFlag;//EQ/SRS���ͣ�0������Ч��0x07���Զ���EQ��0x4f���Զ���SRS������ֵ���� 
    BYTE        EQVal[11];// EQ/SRS������ǰ7��ΪEQƵ�����������SRSģʽ��������8��Ϊ��Ч�л���־����3��Ϊ������ƣ��������
    BYTE        EnergyOutFlag;//���������־��0�����������0�����
    BYTE        SentDectFlag;//�Ͼ����־��0������⣬��0�����
    BYTE        TimeLimit; // �Ͼ���ʱ�䣬0-31����
    BYTE        EnergyLimit; // �Ͼ��������ż���0-63����
    BYTE        FadeInTime;//����ʱ�䣬0�������룬1: 0.1s�� 2��0.2s��3��0.4s��4��0.8s��5��1.6s
    BYTE        FadeOutTime; // ����ʱ�䣬0�������룬1: 0.1s�� 2��0.2s��3��0.4s��4��0.8s��5��1.6s
    BYTE        ChannelSelect;//ͨ��ѡ��0��˫ͨ����1����������2��������
    BYTE        CntrReserve;//Reserve
}decControlInfor_t;



typedef struct
{
    DWORD       ReadFileHandleHook;//�ļ����ӿڣ�basal�ṩ��codec���õĶ����ݽӿڣ�����Ϊ����ָ�� R11, 0x11: ���ļ�ͷ�����һҳ��0x22�����ļ�β�����һҳ��0x33�����ļ�������λ��
    IntAddr		ReadFileBuffAddr; //�ļ����ݻ������׵�ַ������Ϊָ��
    DWORD       FileLen;//�ļ����ȣ���λ��byte
    DWORD       ReadFileCurAddr;//��ǰҳ���ֽ����ļ���λ�ã���λ��btye
}decReadFilePara_t;



typedef struct
{
    BYTE        SongType;// ����
    BYTE        FsIndex;//������Index����76һ��
    BYTE        BitDepth;//������������Reserve
    BYTE        BTIndex;//������Index����76һ��
    BYTE        EncodeMode;//����ģʽ��Wav¼��ʱ, 0: adpcm, 1:pcm
    BYTE        ChannelNum;//ͨ������1����ͨ����2��˫ͨ��
    input_t     InputSel;//����Դ����76һ��
    BYTE        InputGain;//����Դ���棬��76һ�£��Ƽ�ֵ5
    BYTE        ADCGain;//ADC���棬0-3, �Ƽ�ֵ3
    BYTE        DSPGain;//DSP���棬0-1, �Ƽ�ֵ0
    BYTE        DelayTime;//¼����ʱ�趨����λ��20ms
    BYTE        EnergyOut;//���������־��0�����������0�����
    BYTE        AVRFlag;//��������־��0����������1�����أ�2������
    BYTE        AVRDelay;//�������ʱ�䣬0-5�룬�Ƽ�ֵ3
    uint16      AVRLevel;//��������ż����Ƽ�ֵ700
    int16       LeftOffset;//��ͨ��DC Offset����ֵ��Reserve
    int16       RightOffset;//��ͨ��DC Offset����ֵ��Reserve
    
}encControlInfor_t;


typedef struct
{
    DWORD        WriteFileHandle;//�ļ�д�ӿڣ�basal���ṩʵ�֣����������ʹ��c_encWriteFileBuffAddr��c_encWriteFileCurSize
    DWORD        FileLen;//��д���ȣ���λ��byte
    WORD        WriteFileBuffAddr;////��������buffer��ַ
    WORD        WriteFileCurSize;//buffer��С����λ��byte    
}encWriteFilePara_t;



typedef struct
{
    BYTE        Status;//ʵʱ����״̬��0����������0���쳣
    BYTE        AVRStatus;//�������״̬��0��������1������ֹͣ��2������
    WORD        EnergyLevel;//��������
    Full_time_t CurTime;//��ǰ¼��ʱ�䣬��byteΪ0����3btye����Ϊ��ʱ���֡���
}encStatus_t;



typedef struct
{
    BYTE  seek_to_head;        //��д����,����Щ�ļ�ͷ.
    char  write_length;    //��Ҫд�̵ĳ���(page)
    void *write_addr;        //��Ҫд�̵ĵ�ַ
}write_param_t;

typedef struct
{
    uchar sec;
    uchar minute;
    uchar hour;
}enc_time_t;
//�ļ���ʽ״̬/����
typedef struct
{
    BYTE type;            //��riff��ʽ�� wFormatTags ͬ
    char bits;                //������ľ���, 16,18 bit
    char sample_rate;        //������, 8/12/16/24/32/48
    char channel;            //1(mono)/2(stereo)/3(Joint Stero)/4(Dual channel)
    int bit_rate;            //8/16/24/32/40/48/56/64/80/96/112/128/144/160/192/224/256/320 //kbps, 0 ��ʾ�ɱ�λ��
    enc_time_t total_time;        //������ʱ��,¼��ʱ��ʾ��¼����ʱ��
}audio_status_control_t;



//���ſ���
typedef char * buffer_de_t;    //decoder ֻ��Ҫһ��buffer
typedef struct
{
    char volume;            //����,0~31, 0xff��ʾ mute
    char ff_rw;            //<0��ʾrewind;>0��ʾforward;=0��ʾ��������
    char seta_b;            //����A-B ����
    char speed;            //-8~8, 0Ϊ������            
    eq_t eq;                // None/ROCK/POP/CLASSIC/ SOFT/ JAZZ/DBB/USER
    buffer_de_t buffer;        //����ʹ�õ�buffer, �ռ���module�����
}play_control_t;

//¼��״̬
typedef struct
{
    char status;            //��ǰ��״̬, max length/dsp error/��Ҫд��vor on/vor off
    codec_time_t current_time;    //ʵʱ����¼��ʱ��
    write_param_t  write_param;    //д�̲���
}rec_status_t;

//¼������
typedef struct
{
    char     *bit_stream;        //����������
    char        *pcm_stream;        //����ǰ������,��adc��buffer
}buffer_en_t;
typedef struct
{
    uchar gain;            //����0(-7.5db)/1/��/7(3db)
    uchar vor_time;        // VORʱ��. 0(off)/1/2/3��/10s(max)
    resource_t source;        //¼��Դ
    char noise_dec;        //�Ƿ���Ҫ���빦��
    char noise_level;    
    char agc;                //�Ƿ�ʹ��agc(�Զ��������)
    buffer_en_t buffer;        //¼��ʹ�õ�buffer, �ռ���module�����
    char VoxPara;
}rec_control_t;

//<><><><><><><><><><><><>><><><><><><><><><>><><><><>><><><>><><>>>><><><><><><><>
//mp3 encoder
//<><><><><><><><><><><><>><><><><><><><><><>><><><><>><><><>><><>>>><><><><><><><>
/*
typedef enum                                         
{                                                    
        _FS_8K=0,       //8k sample rate            
        _FS_11K025,      //11.025k sample rate       
        _FS_12K,        //12k sample rate           
        _FS_16K,        //16k sample rate           
        _FS_22K05,      //22.05k  sample rate       
        _FS_24K,        //24k sample rate           
        _FS_32K,        //32k sample rate           
        _FS_44K1,       //44.1k sample rate         
        _FS_48K,        //48k sample rate           
        _FS_96K         //96k sample rate           
}samplerate_t;    

typedef enum
{
    _BT_8K=0,
    _BT_16K,
    _BT_24K,
    _BT_32K,
    _BT_40K,
    _BT_48K,
    _BT_56K,
    _BT_64K,
    _BT_80K,
    _BT_96K,
    _BT_112K,
    _BT_128K,
    _BT_144K,        
    _BT_160K,
    _BT_192K,
    _BT_224K,
    _BT_256K,
    _BT_320K,
    _BT_384K
}bitrate_t;

typedef enum
{       
    STEREO=0,
    JOINTSTEREO,
    DUALCHANNEL,
    MONO,
    NOISE
}encmode_t;

typedef enum
{
    SINGLE_CH = 1, 
    DUAL_CH   = 2         
}channelnum_t;

//typedef enum
//{
   // MICIN=0, 
   // LINEIN=1, 
   // FMIN=2, 
   // PAIN=3, 
   // DACIN=4        
//}inputainsel_t;

typedef enum
{
   DB_N7P5=0,
   DB_N6P0=1,
   DB_N4P5=2,
   DB_N3P0=3,
   DB_N1P5=4,
   DB_0P0=5,
   DB_1P5=6,
   DB_3P0=7
}gain_t;  
*/
typedef   struct
{
    char status;                    //��ǰ��״̬, max length/dsp error/��Ҫд��vor on/vor off
    codec_time_t  current_time;    //ʵʱ����¼��ʱ��
} mp3recstatus_t;

typedef struct
{  
        byte             VorTime;  
        input_t          SourceInput;      //input_t is defined in the basel.h   
        samplerate_t     FsIndex;         
        bitrate_t        BTIndex;        
        encmode_t        EncodeMode;
        channelnum_t     ChannelNum;        
        buffer_en_t      Buffer;
        byte             RecMode;     
		byte 			AVRLevel;			//avr func add by mzh 2007.1.18
		byte   			AVRDelay;
} mp3recinitpara_t;    

#endif
#endif    /* CODEC_H*/
   
   

