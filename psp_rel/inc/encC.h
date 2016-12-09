#ifndef _ADPCMENC_H
#define _ADPCMENC_H

#include "codec.h"
#include "typeext.h"

typedef struct
{
	uint8 *file_name;
	uint8 disk_type;
	uint8 reserve[3];
} rec_load_param_t;

void    encInit(void);                       //encoder��ʼ������ 

void    encStart(void);                     //��ʼ���뺯�� 

uchar    encGetBitstream(void);             //��ñ�������Ƶ���ݣ��������ݷ�����Ӧ��buffer��
      
void    encPause(void);                    //��ͣ¼���ӳ���

void    encStop(void);
                                                   
void    encContinue(void);                 //����¼���ӳ���
                                                    
void    encExit(void);                     //�˳������ӳ��� 

#ifndef PC

#pragma ROM(large)      
extern void    encTimerIntSub(void);              //timer�жϴ����ӳ���                                                     
extern void    encAudioIntSub(void);                //��Ƶ�жϴ����ӳ���
//extern void    encDMAIntSub(void);
#pragma ROM(huge)

#else
extern void    encTimerIntSub(void);              //timer�жϴ����ӳ���                                                     
extern void    encAudioIntSub(void);                //��Ƶ�жϴ����ӳ���
#endif
extern encControlInfor_t g_encControlInfor;
extern encWriteFilePara_t g_encWriteFilePara;
extern encStatus_t g_encStatus;


#endif      //_ADPCMENC_H 
