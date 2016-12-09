/*
*******************************************************************************
*                ACTOS AP
*        AG decoder header file
*
*        (c) Copyright, Actions Co,Ld.                             
*             All Right Reserved                          
*
*******************************************************************************
*/ 

#ifndef         _MP3_DE_H
#define         _MP3_DE_H
#include        <actos.h>
#include        <stringH.h> 
#include        <codec.h>



//�ӿڱ���
//FIXME:

extern  decControlInfor_t g_decControlInfor;     //codec��Ҫ�Ŀ�����Ϣ������������ˣ����뵭��
#ifndef PC
extern decSongInfor_t g_decSongInfor;  //codec�����ʱ��ͱ����ʶ����������������,audio�����ж�ʱ�����
#else    
extern  _declspec(dllimport) decSongInfor_t g_decSongInfor;  //codec�����ʱ��ͱ����ʶ����������������,audio�����ж�ʱ�����
#endif
extern  decCurStatus_t g_decCurStatus;  // codec�����жϻ����״̬song�ȣ�InitCodecType����status����g_decInit��songtype������,f1��ʾ����ʱ���жϸ���ʱ����Ϣ

extern  decReadFilePara_t g_decReadFilePara;    //����ļ�����Ϣ�������ϵ���Ϣ��codec���ݸò��������ļ���д��read hook�������������������

extern  decBreakPointInfor_t g_decBreakPointInfor;  //�ϵ������Ϣ��decode�ṹ������������ж�ʱ����ϵ�Ŀ�����Ϣ

extern  long c_decBitRate;
extern  long c_decAPEFrameSize;
extern  WORD c_decAPESampleRate;
extern  WORD c_decAPETblOffset;

//�ӿں���

extern void decInit(void);
extern bool decstart(void);
extern void decPause(void);
extern void decContinue(void);
extern bool decExit(void);
extern bool decSetVolume(void);


#ifdef CODEC_TEST
extern void codec_test(void);
#endif
#ifndef PC

#pragma ROM(large)
extern void decAudioIntSub(void);
#pragma ROM(huge)

#pragma ROM(large)
extern void decDMAIntSub(void);
#pragma ROM(huge)

#pragma ROM(large)
extern void decTimerIntSub(void);
#pragma ROM(huge)

#endif		/* #ifndef PC */
extern void decFFRevDeal(void);
extern void decEffectDeal(void);








#endif  /* _MP3_DE_H */
