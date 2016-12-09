/********************************************************************************
 *                              USDK 5102
 *                            Module: POWER MANAGER
 *                 Copyright(c) 2001-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *    wuyueqian     2009-9-07 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * file
 * brief
 * Author   wuyueqian
 * par
 *
 *      Copyright(c) 2001-2007 Actions Semiconductor, All Rights Reserved.
 *
 * version 1.0
 * date  2008/6/12
 *******************************************************************************/
#include <actos.h>
#include <basal.h>
#include <encC.h>
#include <codec.h>
#include <stringH.h>

#pragma name(MW_BS_AD_EN)

#define RECORDING_OPEN    0x20
#define RECORDING_LOAD    0x21

#define READ_ADDRESS        0x0201
#define VOR_TIME_OUT      0x01
#define RECORD_BUFFER_ADDRESS 0x0600//0xc000
#define RECORD_BUFFER_SIZE 512

#define WRITE_NONE_PAGE 0
#define WRITE_PAGE 1
//#define WRITE_HEAD 2
#define ONE_PAGE_BITNUM 9
#define INPUT_MIC 0x08
#define INPUT_LINEIN 0x40
#define INPUT_FM 0x02
#define INPUT_DAC 0x01
#define ADPCM 0
#define PCM 1
#define AVR_TURN_OFF 0
#define AVR_TURN_ON_VOICE 1
#define AVR_TURN_ON_SONG 2

#define AK211X_SUPPORT_WAV_ONLY

BYTE far m_wavRecStatus;// = RECORDING_WAITING;      //��ǰ״̬
handle far m_wavRecFileHandle;// = NULL;    //�������ļ��ľ��
uint8 far m_wavRecOldTimerNum; //timer isr ʹ�õ�timer����
uint16 far m_wavRecOldDspIsr; //����ɵ�hip isr
uint16 far m_wavRecOldDMAIsr; //����ɵ�ADC isr
uint8  far  m_needclosetimer;


char far m_wavRecFileName[12];

void encode_setAudioFormat(void* param);
bool encode_getNewFileHandle(void* param);
void encode_initAudioFormat(BYTE music_type);
void encode_getAudioFormat(void* param);
void ch_bit_rate(char *tmp);
int8 parse_bit_rate(char *ch);
void setRecordBufferClock(void);

void deal_en_exit(void);

#pragma renamecode(MW_BS_AD_EN)

/*******************************************************************************
 * Function:       Wav2rProceed
 * Description:    wav ¼������д���ļ�
 * Input��
 * Output��        0:û��д, 1:дһ������, 8:��д��WAVͷ�ļ�
 *                 -1:����,��wavrSendCommand(MC_GETSTATUS,mp3_rec_status_t *)��ȡ��ϸ��Ϣ
 * Other��         һ��дһ������(512 bytes),
 *******************************************************************************/
#ifndef PC
retWavrProceed_e srProceed(void)
#else
retWavrProceed_e rProceed(void)
#endif
//retWavrProceed_e srProceed(void)
{
    uchar status;
    BOOL result;
    status = encGetBitstream();//��ѯenc�����ݱ���
    if (WRITE_PAGE == status)
    {
        result = FS_FWrite((const char *) g_encWriteFilePara.WriteFileBuffAddr, 
                (uint8)(g_encWriteFilePara.WriteFileCurSize >> ONE_PAGE_BITNUM),    //���գ���16λ��8λ
                m_wavRecFileHandle);
        if (result == 1)
        {
            g_encWriteFilePara.FileLen += g_encWriteFilePara.WriteFileCurSize;
            return WRITE_PAGE;
        }
        else
        {
            encStop();
	     #if 0		
            encExit();
            IRQ_Release(m_wavRecOldDspIsr, IRQ_DSP);
            //IRQ_Release(m_wavRecOldDMAIsr, IRQ_DMA2);

            TM_KillTimer((int8)m_wavRecOldTimerNum);
	     #endif
	     deal_en_exit();	 
	     
            FS_FClose(m_wavRecFileHandle);
            m_wavRecStatus = RECORDING_DISK_ERROR;
	     m_needclosetimer = 0;		
            return WRITE_WRONG;
        }
    }
    else if (status == 0x02)// ���ļ�ͷд��WAV�ļ�
    {
        #if 0
        encExit();
        IRQ_Release(m_wavRecOldDspIsr, IRQ_DSP);
        //		IRQ_Release(m_wavRecOldDMAIsr, IRQ_DMA2);
        TM_KillTimer((int8)m_wavRecOldTimerNum);
	 #endif
	 deal_en_exit();	 
	 m_needclosetimer = 0;	
        result = FS_FClose(m_wavRecFileHandle);
        if (!result)
        {
            m_wavRecStatus = RECORDING_DISK_ERROR;
            return WRITE_WRONG;
        }
        m_wavRecFileHandle = FS_FOpen(m_wavRecFileName, FS_OPEN_NORMAL);
        if (NULL == m_wavRecFileHandle)
        {
            return WRITE_NONE;
        }
#ifndef PC
        //write head
        result = FS_FWrite((const char *) g_encWriteFilePara.WriteFileBuffAddr, 
                (uint8)(g_encWriteFilePara.WriteFileCurSize >> ONE_PAGE_BITNUM), 
                m_wavRecFileHandle);
#endif
        m_wavRecStatus = RECORDING_STOP;
        if (result == 1)
        {
            return WRITE_HEAD;
        }
        else
        {
            m_wavRecStatus = RECORDING_DISK_ERROR;
            return WRITE_WRONG;
        }
    }
    else
    {
        return WRITE_NONE;
    }
}
/*******************************************************************************
 * Function:       wavrSendCommand
 * Description:    wav encode modual entry
 * Input��
 * Output��
 * Other��         // ����MC_STOP֮��,��Ҫ�ȵ�wav2rProceed()����8�ſ��Է���MC_CLOSE
 *******************************************************************************/
#ifndef PC
BOOL srSendCommand(BYTE cmd, BYTE music_type, void* param)
#else
BOOL rSendCommand(BYTE cmd, BYTE music_type, void* param)
#endif
//BOOL srSendCommand(BYTE cmd, BYTE music_type, void* param)
{
    mp3_rec_status_t *wav_rec_status; // AP ��
    time_t *pTime;
    BOOL result;
    unsigned int *file_handle;	

    switch (cmd)
    {
        case MC_OPEN:
        encode_initAudioFormat(music_type);
        break;

        case MC_CLOSE:
        if ((m_wavRecStatus == RECORDING_STOP))// && (m_wavRecFileHandle != NULL))
        {
            if (NULL != m_wavRecFileHandle)
            {
            #ifdef PC
				WriteWavDataPC(m_wavRecFileHandle);
			#endif
                FS_FClose(m_wavRecFileHandle);
                m_wavRecFileHandle = NULL;
            }
            else
            {
                m_wavRecStatus = RECORDING_EXCEPTION;
                return FALSE;
            }            
        }
        else
        {
            //�쳣�����close����Ҫע���жϲ��رն�ʱ��������¼������ͻȻ�ο���
            if(m_needclosetimer != 0 )
            {
                #if 0
	         encExit();
	         IRQ_Release(m_wavRecOldDspIsr, IRQ_DSP);
	         //		IRQ_Release(m_wavRecOldDMAIsr, IRQ_DMA2);
	         TM_KillTimer((int8)m_wavRecOldTimerNum);
		  #endif
		  deal_en_exit();	 
		  m_needclosetimer = 0;	 
            }
		
            m_wavRecStatus = RECORDING_EXCEPTION;
            return FALSE;
        }
        break;

        case MC_LOAD:
        result = encode_getNewFileHandle(param);
        if (!result)
        {
            return FALSE;
        }

        encInit();         // ���л��޸�wavͷ

// UI Simulator ����Ҫд¼���ļ�ͷ
#ifndef PC
        //FS_FWrite((const char *) g_encWriteFilePara.WriteFileBuffAddr, 1, m_wavRecFileHandle);
        FS_FWrite((const char *) 0x5000, 1, m_wavRecFileHandle);
        UD_Update();
#endif
        g_encWriteFilePara.FileLen += 512;		
        m_wavRecStatus = RECORDING_WAITING;
        //BankSwitch(0xc7);
        BankSwitch(BYTE2_OF(encAudioIntSub));
        break;

        case MC_RECORD:
        if ((m_wavRecStatus == RECORDING_WAITING) && (m_wavRecFileHandle != NULL))
        {
            m_wavRecOldTimerNum = TM_SetTimer((int16) encTimerIntSub, 2);
            if (m_wavRecOldTimerNum == 0)
            {
                m_wavRecStatus = RECORDING_DISK_ERROR;
                return FALSE;
            }
            m_wavRecOldDspIsr = IRQ_Intercept((uint16) encAudioIntSub, IRQ_DSP);
            //m_wavRecOldDMAIsr = IRQ_Intercept((int)encDMAIntSub, IRQ_DMA2);

            encStart();

	     m_needclosetimer = 1; 		
        }
        else
        { // ����¼��
            if (m_wavRecStatus == RECORDING_PAUSE)
            {
                encContinue();
            }
            else
            {
                m_wavRecStatus = RECORDING_EXCEPTION;
                return FALSE;
            }
        }
        m_wavRecStatus = RECORDING_RECORDING;
        break;

        case MC_PAUSE:
        if (m_wavRecStatus == RECORDING_RECORDING)
        {
            encPause();
        }
        else
        {
            m_wavRecStatus = RECORDING_EXCEPTION;
            return FALSE;
        }
        m_wavRecStatus = RECORDING_PAUSE;
        break;

        case MC_STOP:
        encStop();//codec prepare the head data for writing
        break;

        case MC_SETGAIN:
        g_encControlInfor.InputGain = (BYTE) param;
        break;

        case MC_SETGAIN_DSP:
        if ((((BYTE) param) < 2))// && (((BYTE) param) >= 0))
        {
            g_encControlInfor.DSPGain = (BYTE) param;
        }
        break;
        case MC_SETGAIN_ADC:
        if ((((BYTE) param) < 4))// && (((BYTE) param) >= 0))
        {
            g_encControlInfor.ADCGain = (BYTE) param;
        }
        break;

        case MC_SET_RECORD_DELAY:
        g_encControlInfor.DelayTime = (BYTE) param;

        case MC_SETAVRFLAG:
        if ((((BYTE) param) < 3))// && (((BYTE) param) >= 0))
        {
            g_encControlInfor.AVRFlag = (BYTE) param;
        }
        break;
        case MC_SETAVRLEVEL:
        g_encControlInfor.AVRLevel = (WORD) param;
        break;
        case MC_SETAVRDELAY:
        g_encControlInfor.AVRDelay = (BYTE) param;
        break;

        case MC_SET_SONG_TYPE:
        g_encControlInfor.SongType = (BYTE) param;
        break;

        case MC_RECODEMODE:
        g_encControlInfor.EncodeMode = (BYTE) param;
        break;

        case MC_SETSOURCE:
        g_encControlInfor.InputSel = ((resource_t *) param)->input;
        //memcpy(&(g_encControlInfor.InputSel), &(((resource_t *)param)->input), sizeof(input_t));
        break;

        case MC_SETAUDIOFMT:
        encode_setAudioFormat(param);
        break;

        case MC_GETTIME:
        pTime = (time_t *) param;
        memcpy(pTime, &(g_encStatus.CurTime.TimeHour), sizeof(time_t));
        break;

        case MC_GETSTATUS:
        wav_rec_status = (mp3_rec_status_t *) param;
        if (g_encStatus.Status != 0x0)
        {
            wav_rec_status->status = g_encStatus.Status;
        }
        else
        {
            wav_rec_status->status = m_wavRecStatus;
        }
        wav_rec_status->gain = g_encControlInfor.InputGain;
        wav_rec_status->avrDelay = g_encControlInfor.AVRDelay;
        wav_rec_status->avrLevel = g_encControlInfor.AVRLevel;
        memcpy(&(wav_rec_status->source.input), &(g_encControlInfor.InputSel), sizeof(input_t));
        memcpy(&(wav_rec_status->total_time.hour), &(g_encStatus.CurTime.TimeHour), sizeof(time_t));
        break;

        case MC_GETAUDIOFMT:
        encode_getAudioFormat(param);
        break;

        case MC_SETPOWERCOUNT:
        g_encControlInfor.EnergyOut = *(uint8*)param;
        break;

        case MC_GETPOWERVALUE:
        *((uint16*)param) = (uint16)g_encStatus.EnergyLevel;
        break;

        case MC_GETHANDLE:
        file_handle = (unsigned int*) param;
        *file_handle = m_wavRecFileHandle;
	 break;	

        default:
        return FALSE;
        //break;
    }//switch(cmd)

    return TRUE;
}

void setRecordBufferClock(void)
{
//    uint8 sfr_save;
//    sfr_save = SFR_BANK;
//    SFR_BANK = BANK_CMU_RMU;
//    MCSR7 &= 0xE3;
//    MCSR7 |= 0x08;
//    SFR_BANK = sfr_save;
}

/*******************************************************************************
 * Function:       encode_initAudioFormat
 * Description:    ��ʼ��¼������
 * Input��
 * Output��
 * Other��         ��Ӧ MC_OPEN
 *******************************************************************************/

void encode_initAudioFormat(BYTE music_type)
{
    g_encControlInfor.InputSel = ENC_MIC;

    //music_type = music_type;
    g_encControlInfor.SongType = music_type;
    g_encControlInfor.FsIndex = _FS_8K;
    g_encControlInfor.ChannelNum = SINGLE_CH;//default is 32kbps
    //g_encControlInfor.BitDepth = 16;
    //g_encControlInfor.BTIndex = indexOf(FsIndex*BitDepth*ChannelNum/4);
    g_encControlInfor.EncodeMode = (BYTE) ADPCM;
    g_encControlInfor.InputGain = 7;//��Ӧ37dB
    g_encControlInfor.ADCGain = 2;
    g_encControlInfor.DSPGain = 0;
    g_encControlInfor.DelayTime = 15;//20*15 =300MS
    g_encControlInfor.EnergyOut = 0;//disable
    g_encControlInfor.AVRFlag = AVR_TURN_OFF;//disable
    g_encControlInfor.AVRDelay = 3;
    g_encControlInfor.AVRLevel = 700;
    //g_encControlInfor.LeftOffset = reserved;
    //g_encControlInfor.RightOffset = reserved;

    //setRecordBufferClock();
    g_encWriteFilePara.WriteFileBuffAddr = RECORD_BUFFER_ADDRESS;
    g_encWriteFilePara.WriteFileCurSize = RECORD_BUFFER_SIZE;
    g_encWriteFilePara.FileLen = 0;
    //g_encWriteFilePara.WriteFileHandle ��MC_LOAD�г�ʼ��

    m_wavRecStatus = RECORDING_WAITING;
}

#ifdef PC
typedef struct
{
	uint8 *file_name;
	uint8 disk_type;
	uint8 reserve[3];
} rec_load_param_t;
#endif

/*******************************************************************************
 * Function:       encode_getNewFileHandle
 * Description:    ����ļ��Ƿ����,������,�������ļ�.
 * Input��
 * Output��
 * Other��         ��Ӧ MC_LOAD
 *******************************************************************************/
BOOL encode_getNewFileHandle(void* param)
{
    rec_load_param_t *load_param = (rec_load_param_t *)param;
    memcpy(m_wavRecFileName, load_param->file_name, 11);
    m_wavRecFileName[11] = '\0';

    m_wavRecFileHandle = NULL;
    m_wavRecFileHandle = FS_FOpen(m_wavRecFileName, 0);
    if ((m_wavRecFileHandle != NULL) || (m_wavRecStatus != RECORDING_WAITING))
    {
        m_wavRecStatus = RECORDING_EXCEPTION;
        m_wavRecFileHandle = NULL;
        return FALSE;
    }
    //recording_waiting and m_wavRecFileHandle == null
    m_wavRecFileHandle = FS_FCreate(m_wavRecFileName);
    if (m_wavRecFileHandle == NULL)
    {
        m_wavRecStatus = RECORDING_DISK_ERROR;
        return FALSE;
    }
    
    if(load_param->disk_type == 'C')//���� ����nand ��������nand flash
    {
#ifndef PC
        //����flash merge
        UD_MergeFlash();
#endif
    }
    
    g_encWriteFilePara.WriteFileHandle = m_wavRecFileHandle;
    return TRUE;
}

/*******************************************************************************
 * Function:       parse_bit_rate
 * Description:    ת�� AP��� BIT RATE �� CODE���BIT RATE
 * Input��
 * Output��
 * Other��         //
 *******************************************************************************/
int8 parse_bit_rate(char *ch)
{
    int tmp;
    tmp = (*(ch) - '0') * 100 + (*(ch + 1) - '0') * 10 + (*(ch + 2) - '0');
    switch (tmp)
    {
        case 8:
        return _BT_8K;

        case 16:
        return _BT_16K;

        case 24:
        return _BT_24K;

        case 32:
        return _BT_32K;

        case 40:
        return _BT_40K;

        case 48:
        return _BT_48K;

        case 56:
        return _BT_56K;

        case 64:
        return _BT_64K;

        case 80:
        return _BT_80K;

        case 96:
        return _BT_96K;

        case 112:
        return _BT_112K;

        case 128:
        return _BT_128K;

        case 144:
        return _BT_144K;

        case 160:
        return _BT_160K;

        case 192:
        return _BT_192K;

        case 224:
        return _BT_224K;

        case 256:
        return _BT_256K;

        case 320:
        return _BT_320K;

        case 384:
        return _BT_384K;
        default:
        return -1;
    }
   // return -1;
}

/*******************************************************************************
 * Function:       encode_setAudioFormat
 * Description:    ���� CODE ���  g_encInitPara
 * Input��
 * Output��
 * Other��         ��Ӧ MC_SETAUDIOFMT
 *******************************************************************************/
void encode_setAudioFormat(void* param)
{
    audio_format_t *m_audio_fmt;
    m_audio_fmt = (audio_format_t *) param;

#ifndef AK211X_SUPPORT_WAV_ONLY
    int8 result;
    result = parse_bit_rate(m_audio_fmt->bit_rate);
    if(result >= 0)
    {
        g_encControlInfor.BTIndex = result;
        g_encControlInfor.FsIndex = m_audio_fmt->sample_rate;
        g_encControlInfor.ChannelNum = m_audio_fmt->channel;
    }
#else
    g_encControlInfor.FsIndex = m_audio_fmt->sample_rate;
    g_encControlInfor.ChannelNum = m_audio_fmt->channel;
#endif
}
/*******************************************************************************
 * Function:       ch_bit_rate
 * Description:    ת�� CODE ��� BIT RATE �� AP ���BIT RATE
 * Input��
 * Output��
 * Other��         //
 *******************************************************************************/
void ch_bit_rate(char *tmp)
{
    *tmp = ' ';
    *(tmp + 1) = ' ';
    *(tmp + 3) = 0;
    switch (g_encControlInfor.BTIndex)
    {
        case _BT_8K:
        *(tmp + 2) = '8';
        break;

        case _BT_16K:
        *(tmp + 1) = '1';
        *(tmp + 2) = '6';
        break;
        case _BT_24K:
        *(tmp + 1) = '2';
        *(tmp + 2) = '4';
        break;
        case _BT_32K:
        *(tmp + 1) = '3';
        *(tmp + 2) = '2';
        break;
        case _BT_40K:
        *(tmp + 1) = '4';
        *(tmp + 2) = '0';
        break;
        case _BT_48K:
        *(tmp + 1) = '4';
        *(tmp + 2) = '8';
        break;
        case _BT_56K:
        *(tmp + 1) = '5';
        *(tmp + 2) = '6';
        break;

        case _BT_64K:
        *(tmp + 1) = '6';
        *(tmp + 2) = '4';
        break;

        case _BT_80K:
        *(tmp + 1) = '8';
        *(tmp + 2) = '0';
        break;

        case _BT_96K:
        *(tmp + 1) = '9';
        *(tmp + 2) = '6';
        break;

        case _BT_112K:
        *tmp = '1';
        *(tmp + 1) = '1';
        *(tmp + 2) = '2';
        break;

        case _BT_128K:
        *tmp = '1';
        *(tmp + 1) = '2';
        *(tmp + 2) = '8';
        break;

        case _BT_144K:
        *tmp = '1';
        *(tmp + 1) = '4';
        *(tmp + 2) = '4';
        break;

        case _BT_160K:
        *tmp = '1';
        *(tmp + 1) = '6';
        *(tmp + 2) = '0';
        break;

        case _BT_192K:
        *tmp = '1';
        *(tmp + 1) = '9';
        *(tmp + 2) = '2';
        break;

        case _BT_224K:
        *tmp = '2';
        *(tmp + 1) = '2';
        *(tmp + 2) = '4';
        break;

        case _BT_256K:
        *tmp = '2';
        *(tmp + 1) = '5';
        *(tmp + 2) = '6';
        break;

        case _BT_320K:
        *tmp = '3';
        *(tmp + 1) = '2';
        *(tmp + 2) = '0';
        break;
        default:
        break;
    }
}

/*******************************************************************************
 * Function:       encode_getAudioFormat
 * Description:    ��ȡ CODE �� audio_format_t
 * Input��
 * Output��
 * Other��         ��Ӧ MC_GETAUDIOFMT
 *******************************************************************************/
void encode_getAudioFormat(void* param)
{
    audio_format_t *m_audio_fmt;

    m_audio_fmt = (audio_format_t *) param;
    m_audio_fmt->sample_rate = g_encControlInfor.FsIndex;
    m_audio_fmt->channel = g_encControlInfor.ChannelNum;
    ch_bit_rate(m_audio_fmt->bit_rate); //�﷨
}

void deal_en_exit(void)
{
    encExit();
    IRQ_Release(m_wavRecOldDspIsr, IRQ_DSP);
    //IRQ_Release(m_wavRecOldDMAIsr, IRQ_DMA2);

    TM_KillTimer((int8)m_wavRecOldTimerNum);
}

