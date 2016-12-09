/*
 *******************************************************************************
 *                        SDK30F_D1
 *                     music player header  file
 *
 *                (c) Copyright 2002, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File: ap_music.h
 * By  : rcmai
 * Version: 1> v1.00     first version     2004-3-19 8:28
 *******************************************************************************
 *******************************************************************************
 */
#ifndef _ap_music_shuffle_H

#define _ap_music_shuffle_H

#define GroupNum 10              //����ÿ�����������
extern uint16 far FileNumArr[GroupNum]; //�������һ����Ҫ����shuffle�������ļ����
extern uint16 far CurrentfileNum; //��ǰ�Ѿ������˶����׸���
extern uint16 far Nextgroup; //��ǰ���ŵ��ڼ������һ�����
extern uint16 far TotalGroup; //���ֶ�����
extern uint8 far Currentgroupfilenum; //��ǰ���ŵĸ����ڲ�������������
extern uint8 far curgrouptotal;

BOOL shuffleAll(uint8 *strFile, bool *firstshuffle);
BOOL SelectNext(char* strFile);
uint16 GetFileTotalCur(void);
#endif     //_ap_music_shuffle_H
