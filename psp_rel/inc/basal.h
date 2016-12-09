#ifndef _BASAL_H
#define _BASAL_H

#ifdef __C251__
#include <typeext.h>
#include <mmcmd.h>
#include <time.h>
#include <midware_BS_EH.h>

#define TYPE_MP3    0
#define TYPE_WMA    1
#define TYPE_PCM    2
#define TYPE_FLAC   4
#define TYPE_APE    6

#define PLAYING_STOP            0x00
#define PLAYING_PLAYING         0x01
#define PLAYING_PAUSE           0x02
#define PLAYING_AB              0x03
#define PLAYING_ABPAUSE         0x04
#define PLAYING_SEARCHING       0x05
#define PLAYING_REACH_END    0x06
#define PLAYING_REACH_A        0x07
#define PLAYING_REACH_B        0x08
#define PLAYING_SEACH_FAIL    0x09
#define PLAYING_SEACH_SUCC    0x0A
#define PLAYING_WAIT_A     0x0B
#define PLAYING_WAIT_B    0x0D

#define PLAYING_ERROR        0x0C
#define PLAYING_REACH_HEAD   0x0E
#define PLAYING_DRMSONG      0x0f

#define RECORDING_STOP          0x00
#define RECORDING_RECORDING     0x01
#define RECORDING_PAUSE         0x02
#define RECORDING_WAITING       0x03
#define RECORDING_EXCEPTION     0x04
#define RECORDING_OUT_SPACE    0x05
#define RECORDING_DISK_ERROR    0x06
#define RECORDING_VOR_RECORDING    0x07
#define RECORDING_VOR_WAITING    0x08

#define SIGNAL_NON 0x00
#define SIGNAL_REACH_V 0x01
#define SIGNAL_REACH_B 0x02
//#define SIGNAL_CLEAR_B 0x03


#define FADENON_DEF 0
#define FADEIN_DEF  0x10
#define FADEOUT_DEF 0x20 
#define FADEOUT2_DEF 0x30 

#define BANK_ENABLE  0x01
#define BANK_DISABLE 0xfe

#define _GPIO_TEST 

typedef struct
{
    char mute:1;
    char unmute:1;
    char fm:1;
    char linein:1;
    char mic:1;
}mix_t;

/*
 typedef struct
 {
 char mic:1;    
 char fm:1;
 char linein:1;
 char dac:1;
 char reserved:4;
 }input_t;
 */
typedef uint8 input_t;
#define ENC_MIC 0x00
#define ENC_FM 0x01
#define ENC_LINEIN 0x01
#define ENC_MIX 0x03
typedef struct
{
    input_t input;
    BOOL mix;
}resource_t;

typedef struct
{
    BYTE MS;
    BYTE TimeHour;
    BYTE TimeMinute;
    BYTE TimeSecond;
}Full_time_t;

typedef struct
{
    uchar status;
    uchar gain;
    uchar avrDelay;
    uint16 avrLevel;
    resource_t source;
    time_t total_time;

}mp3_rec_status_t;
typedef mp3_rec_status_t wav_rec_status_t;

typedef enum
{
    none,
    rock,
    pop,
    classic,
    soft,
    jazz,
    dbb,
    user,
    SRS= 0x4f
}eq_t;

typedef struct
{
    uchar status;//����״̬��0��������0�쳣
    uchar signal; //�ź�����0һ��״̬��2����B�㣬3���B��
    uchar volume;//����
    time_t total_time;//������ʱ��
    eq_t eq;//��ǰEQ
    signed char speed;//AK211XӲ����֧�֣�����
}mp3_play_status_t;

typedef mp3_play_status_t wma_play_status_t;
typedef mp3_play_status_t music_play_status_t;
typedef mp3_play_status_t ogg_play_status_t;
typedef mp3_play_status_t ape_play_status_t;
typedef mp3_play_status_t play_status_t;

typedef struct
{
    BYTE switchflag;//�������¸���(Ϊ1��λ��ʾ��Ӧ��򿪣�Ϊ0ʱ��ʾ�ر�)
    int RemainPlayCnt;//���¿ɲ��ŵĴ��� 
    int originalPlayCnt;//ԭʼ�ɲ��Ŵ���       

    date_t BeginDate;//��ʼ��������  
    time_t BeginTime;

    date_t ExpDate;//��ֹ��������(2004-12-30)
    time_t ExpTime;

    int RemainTime;//(��һ���ù�֮�����õ�Сʱ������ͬһ�׸��ǹ̶�ֵ)
    int RemainStoreTime;//���¿ɴ洢��ʱ��    (����)//���ڱ����˽ӿڣ�����ʱ����
    char DealOnClkBck;//0: ʱ�ӻص�ʱɾ�� 1��ʱ�ӻص�ʧЧ  
}licensinf_t;

typedef enum
{
    INF_NORMALWMA, //��ͨ��WMA [0]
    INF_DRMHVLICENSE, //��DRM��license [1]
    INF_DRMNOLICENSE //��DRM����license [2]
}drm_mod_e;

typedef struct
{
    drm_mod_e drmmode;
    licensinf_t drminf;
}wma_inf_t;

typedef enum
{
    Nor_open, //��ͷ��ʼ��
    Nor_Break_open, //���ϴζϵ����ʼ��
    ABPlay_open //�ظ����ϴε�AB����
}Open_mod_t;

typedef enum
{
    ClrABPoint, //���AB��,�ص���ͨ����
    SetAPoint, //�趨A��
    SetBPoint, //�趨B��
    ReturnApoint, //�����ص�A��
    StopBPlay, //ֹͣB�㲥��
    ABFinished //AB������
}SetABMode_t;

typedef struct
{
    uint32 BreakPointAddr; // �ϵ��Ӧ���ļ�λ�ã���λ��btye
    uint32 PacketCnt; // �ϵ��Ӧ��Packet��Ŀ
    uint32 PacketSize; // �ϵ��Ӧ��Packet��С                                                
    uint8 SynHdrData1;//�ϵ��Ӧ��ͬ����Ϣ1
    uint8 SynHdrData2;//�ϵ��Ӧ��ͬ����Ϣ2
    uint16 CurBitrate; // �ϵ��Ӧ�ı�����
}decBreakPointInfor_t;

typedef struct
{
    uint32 breakPT;//�ϵ�λ��
    uint32 ApointSave;//A��λ��
    uint32 BpointSave;//B��λ��
    decBreakPointInfor_t breakPointInfor;
    //BYTE breakPTTime[15];//MP3�ϵ�ʱ��    
}breakPT_t;

typedef struct
{
    uint8* filename; //�ļ�����ַ
    breakPT_t* BreakPTSave; //�ϵ������ַ
    BYTE typeerror; //�쳣�ļ���ʽ 0�� �����ļ���1���쳣�ļ�����Ҫ�����쳣�ļ���Ԥ���ж�
    uint32 musicframeoffset; //mp3�ļ���һ֡��ʼλ��
    time_t musictotaltime; //mp3��ʱ��
    uint8 FadeInTime; //���õ���ʱ�䣬0Ϊ��֧�ֵ���
    uint8 SoftVolumeMax; //�������������������0Ϊ����Ӳ������
    uint8 SampleRate;//������
}Open_param_t;

typedef struct
{
    uint8 bits;//���������������� //char
    uint8 sample_rate;//����������   //char
    uint8 channel;//ͨ����  //char
    uint8 bit_rate[4];//������  //char  
}audio_format_t;

typedef struct
{
    uchar state;
    uchar volume;
    uchar vor_time;
    time_t total_time;
    signed char speed;
}wav_play_status_t;

typedef struct
{
    uchar status; // (1:playing ; 2:pause ; 3(searching).
    uint16 freq; //��ǰ��̨��Ƶ�ʡ���λΪ1k (���λ��Ϊ1)
    uchar station; //��ǰ��̨��Ӧ�ĵ�̨����š�
    uchar station_count; //��̨���еı����̨����
    uchar mode; //0 (US-Europe) /1 (Japan) ;?
    uchar volume; //������
    uchar stereo_status;
    uchar vol_display;
    uchar fmrssi;        //�ź�ǿ��	
}fm_play_status_t;

typedef struct
{
    bool existflag;
    BYTE playmode;
    int apeMusicCurNum;
    int apeMusicMaxNum;
    time_t curPartStartTime;
    time_t curPartEndTime;
    time_t curTime;
    char curMusicName[31];
    char curArtistName[31];
}cuefileinfo;

typedef enum
{
    WRITE_WRONG = -1,
    WRITE_NONE = 0,
    WRITE_ONE = 1,
    WRITE_HEAD = 8
}retWavrProceed_e;

/*
 typedef enum                                         
 {                                                    
 _FS_8K=0,       //8k sample rate
 _FS_32K=6        //32k sample rate
 }samplerate_t;    

 typedef enum
 {
 _BT_8K=0,
 _BT_32K=3,
 _BT_64K=7,
 _BT_128K=11,
 _BT_256K=16
 }bitrate_t;

 typedef enum
 {       
 ENCODE_WAV_ADPCM=0,
 ENCODE_WAV_PCM
 }encmode_t;

 typedef enum
 {
 SINGLE_CH = 1, 
 DUAL_CH   = 2         
 }channelnum_t;

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
typedef enum
{
    _FS_8K=0, //8k sample rate            
    _FS_11K025, //11.025k sample rate       
    _FS_12K, //12k sample rate           
    _FS_16K, //16k sample rate           
    _FS_22K05, //22.05k  sample rate       
    _FS_24K, //24k sample rate           
    _FS_32K, //32k sample rate           
    _FS_44K1, //44.1k sample rate         
    _FS_48K, //48k sample rate           
    _FS_96K //96k sample rate           
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
    ENCODE_WAV_ADPCM=0,
    ENCODE_WAV_PCM
}wav_encmode_t;

typedef enum
{
    SINGLE_CH = 1,
    DUAL_CH = 2
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
BOOL apSendCommand(BYTE cmd, BYTE music_type, void* param);

#define mp3pSendCommand(cmd,param) apSendCommand((cmd),TYPE_MP3,(void *)(param))
#define wmapSendCommand(cmd,param) apSendCommand((cmd),TYPE_WMA,(void *)(param))
#define wavpSendCommand(cmd,param) apSendCommand((cmd),TYPE_PCM,(void *)(param))
#define apepSendCommand(cmd,param) apSendCommand((cmd),TYPE_APE,(void *)(param))
#define flacpSendCommand(cmd,param) apSendCommand((cmd),TYPE_FLAC,(void *)(param))

retWavrProceed_e rProceed(void);
BOOL rSendCommand(BYTE cmd, BYTE music_type, void* param);
#define mp3rSendCommand(cmd,param) rSendCommand((cmd),TYPE_MP3,(param))
#define mp3rProceed rProceed
#define wavrSendCommand(cmd,param) rSendCommand((cmd),TYPE_PCM,(param))
#define wavrProceed rProceed
#define wmarSendCommand(cmd,param) rSendCommand((cmd),TYPE_WMA,(param))
#define wmarProceed rProceed
#define recordSendCommand(cmd,param) rSendCommand((cmd),TYPE_PCM,(param))
#define recordrProceed rProceed

#endif   
#endif  

