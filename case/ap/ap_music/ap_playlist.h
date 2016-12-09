/*
 *******************************************************************************
 *                        SDK30F_D1
 *                     music player header  file
 *
 *                (c) Copyright 2002, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File: ap_playlist.h
 * Version: 1> v1.00     first version    by Mars   2009-2-13 9:28
 *******************************************************************************
 */
#ifndef _UI_PLAYLIST_H
#define _UI_PLAYLIST_H
#include "actos.h"//ϵͳͷ�ļ�
#include "mmcmd.h"//ģ�������������
#include "basal.h"//basal module ͷ�ļ�
#include "enhanced.h"//enhanced module ͷ�ļ�
#include "ap_common.h"//�ؼ�ͷ�ļ�

/*�����궨��*/
#define  VM_ALBUM_PICINFO    (GetVMCap()*512 - 0x200000)

#define  PLAYLISTFILENAME      "ASDKMM  LIB"
#define  PLAYLISTMAGIC         "ASDKMMLIB"
#define  FILEINFOR             "FILEINFOR"
#define  ARTISTNAME            "ARTISTS"
#define  ALBUMNAME             "ALBUMS"
#define  GENRENAME             "GENRES"
#define   ALLSONGNAME          "ALLSONGS"
#define   HEADITEMSIZE            16
#define   IDMSTARTADDRESS    0x4000     // IDM/IPM�ڴ���ʼ��ַ
#define   IDMPAGESIZE              0x4000     // IDM/IPMÿ��ҳ��������
#define   FILEINDEXADDRESS         0x6000  //�洢�ļ������
#define   FILEINFORSIZE       256                 //ÿ�������ļ��洢���������Ĵ�С
#define   MusicExtNum            9
#define   ID3ITEMSIZE           32
#define   SORTITEMSIZE       8

#define   TOTALFILEMAX             4096
#define   ONEPAGEFILENUM       1024
#define   IPMPAGEFILENUM        (IDMPAGESIZE/FILEINFORSIZE)

#define   ListDisplayNum  5              //һ�������ʾ���б���Ŀ
#define   OneSectorListNum         64      //ÿ�������洢���б���Ŀ
#define   OneSectorListIndexNum    256      //ÿ�������洢���б���Ŀ
/*���ݽṹ����*/
typedef struct
{
    char ID3TagFlag; //�����ļ��Ƿ����ID3��TAG��Ϣ�ı�־��1Ϊ�У�0Ϊ��
    char ArtistString[ID3ITEMSIZE]; //���������� 32 bytes
    char AlbumString[ID3ITEMSIZE * 2]; //ר������  64bytes
    char GenreString[ID3ITEMSIZE]; //��������      32 bytes
    char TitleString[ID3ITEMSIZE * 2]; //����&�ļ�����      64 bytes
    char TrackString[8]; //track num string
    unsigned int TrackNum; //ר����Ŀ��
    unsigned long DirClusterNo; //�ļ���Ŀ¼��Ŀ¼�״غ�
    char FileName[12]; //�ļ�������Ϣ
} music_list_infor_t;

typedef struct
{
    char HeadItemName[12]; //�б�����
    unsigned int ItemNum; //�б����
    unsigned int ItemOffset; //�б�λ��
} list_head_item_t;

typedef struct
{
    unsigned int ListIndex;
    unsigned int ListChildNum;
    unsigned int ListTotal;
    unsigned int FileIndex;
} ID3_list_item_t;

typedef enum
{
    AllSong, // ȫ������
    Album, //ר��
    Artist, //������
    Genre//����
} list_type_t;

typedef enum
{
    Playlist, //�����б�����
    Filelist  //�ļ��б�����
} music_list_type_t;

/*�ⲿ��������*/
extern handle list_fp;
extern uint8 ScrollFlag; //������־λ
extern uint8 scroll_buf[]; //����buffer��
extern region_t region_scroll; //��������
extern const WORD menulist_bck_pic[];

extern list_type_t g_list_type; //�����б�����
extern uint g_playlist_total; //�����б�����
extern uint g_playlist_offset; //�����б��������ļ�������ƫ��
extern uint g_playlist_num; //�����б�����

extern uint g_filelist_total; //�б����ļ�����
extern uint g_filelist_start; //�����б�����ʼ�ļ�������
extern uint g_filelist_num; //�б��е��ļ����

extern uint g_filelist_index; //�ļ��������������е����
extern uint g_filelist_offset; //�������������ļ��е�ƫ������

/*�ⲿ��������*/
extern int show_music_list(const WORD headid);
extern int show_music_file(const WORD head_id);
extern void ui_show_list(uint active, uint old_active, list_type_t musiclist_type, bool scroll_flag);

#endif
