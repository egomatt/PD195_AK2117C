/*
******************************************************************************* *                               AS211A
*                              AS211A
*                              Module: music ap
*                 Copyright(c) 2002-2010 Actions Semiconductor,
*                            All Rights Reserved. 
*
* Descripton: the head file of music
*
* History:         
*      <author>    <time>           <version >             <desc>
*       xusong    2010-07-14 11:00     1.0             build this file 
 *******************************************************************************
 */
//ͨ������ļ�������һЩ����,���ʺϲ�ͬ�ķ���ʹ��
#ifdef NetOnNet
#define OPT_Sup_LRC_fun//֧�ָ����ʾ����
#endif

#ifndef PC
//#define OPT_Sup_follow_fun//֧�ָ����Աȹ���
#endif
//#define OPT_Sup_AVReplay_fun//�Ƿ�֧���Զ���������
//#define OPT_Sup_FadeOut_fun//֧�ֵ�������
//#define OPT_Sup_FadeIn_fun//֧�ֵ��빦��
//#define OPT_Sup_FselFolder_fun//֧���ļ����л�����
//#define OPT_Sup_FFRevSpeed_fun//֧�ֿ�����˵���
//#define OPT_Sup_FFRevSound_fun//֧�ֿ����������
//#define OPT_Sup_FadeInOutTime_fun//֧�ֵ��뵭��ʱ��
//#define play_txt_in_music    //֧�����ֲ�����ͬʱ���ŵ�����
//#define LDB_LRC                         //֧��LDB�����ʾ
//#define Show_Album_picture //��ʾר��ͼƬ
//#define USE_SPECTRUM
//------------------------------
#define FollowReadTempFileName  "$$TEMP$$WAV"

#define SCROLLTIME_DEF     260/20           //20ms * 13 = 260ms//����Ƶ�ʷ�Ƶϵ��
#define ABReplayMiniTime_def 400/20     //20ms * 10 = 200ms//AB��������С����
//#define ACTIVE_USERPL                   //�б�༭
//#define MUSICEDIT                    //֧�����ֱ༭���ܣ��������գ�����ѡ�Σ�����¼������ҪӦ����¼���ʷ���
//#define MUSIC_BOOKMARK
