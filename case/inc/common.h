/*
 ********************************************************************************
 *                       common
 *               ap common head file
 *
 *                (c) Copyright 2002-2003, Actions Co,Ld.
 *                        All Right Reserved
 *
 * File   : ap_common.h
 * By     : phchen
 * Version: 1> v1.00     first version     2004-08-20 11:35
 ********************************************************************************
 * $Id: common.h,v 1.1.1.1 2008/01/11 07:28:06 rowena Exp $
 ********************************************************************************
 */
#ifndef _COMMON_H
#define _COMMON_H

#include  "display.h"
#include  "enhanced.h"
#include  "ui_res.h"
#include "actos.h"

//���������Ϣ����ʾλ��
#define PIC_MESSAGEBOX_X   9
#define PIC_MESSAGEBOX_Y  38

#define PIC_CARDERR_X      9
#define PIC_CARDERR_Y     32
// ����ÿһ�����͵�����ļ���չ���ĸ���
#define  MUSIC_EXT_COUNT        4
#define  VOICE_EXT_COUNT        2
#define  DSC_EXT_COUNT            2
#define  NEW_MAX_FILE_NUM        999

// LCD���йصĶ���
// LCD�ֱ��ʵĶ���, ���пؼ��Ĳ�����������������ȷ��
// ������Ļ�ĸ߶ȺͿ��.
#define  LCD_WIDTH            Display_Length
#define  LCD_HIGH             Display_Height

// �ֿ�,��ʾ�߶ȵĶ���
#define  CN_CHAR_WIDTH            16
//#define  LARGE_CHAR_HEIGHT        16
//#define  LARGE_CHAR_WIDTH        8
//#define  CHAR_HEIGHT            8
//#define  CHAR_WIDTH                 6
//#define        TCHAR_WIDTH        8

// ������Ӣ���еĸ߶�
#define  ROW_EN_HEIGHT            10
#define  ROW_CN_HEIGHT            18

// ����ȫ͸��
#define  CLARITY            0xff
#define  NOCLARITY            0x0

// �˵�ͷ�ؼ��йصĶ���
typedef struct
{
    uint16 title;
    uint16 headpic;
    uint8 menu_grade[3]; // "1-1-1"
} menuhead_t;

//��������� define
#define  MENU_HEAD_HIGH        19          //�˵�ͷ�ؼ��ĸ߶�
#define  MENU_HEAD_WIDTH       LCD_WIDTH  //�˵�ͷ�ؼ��Ŀ��
#define  MENU_HEAD_S_POS_X     255        //�ַ�����ʼ����
#define  MENU_HEAD_S_POS_Y     130
#define  MENU_HEAD_G_POS_Y     ((MENU_HEAD_HIGH-CHAR_HEIGHT-1)/2) // �˵���������
//���󱨾���ؼ�
#define  MSG_WIDTH            100    // ��ֵ����С��LCD_WIDTH, ����80
#define  MSG_HIGH             60    //��ֵ����С��LCD_HIGH, ����30
#define  MSG_POS_X            ((LCD_WIDTH - MSG_WIDTH)/2)
#define  MSG_POS_Y            ((LCD_HIGH - MSG_HIGH)/2)
#define  MSG_RIGHT            (MSG_POS_X + MSG_WIDTH)
#define  MSG_BOTTOM           (MSG_POS_Y + MSG_HIGH)
#define  MSG_STR_Y            ((LCD_HIGH - 12)/2)
//#define  menuhead_high_def        19        // �˵�ͷ�ؼ��ĸ߶�
//#define  menuhead_width_def        LCD_WIDTH      // �˵�ͷ�ؼ��Ŀ��
#define  MENUHEAD_S_POS_X_DEF        5        // �ַ�����ʼ����
#define  MENUHEAD_S_POS_Y_DEF        110
//#define  menuhead_g_pox_y_def        ((menuhead_high_def - CHAR_HEIGHT - 1) / 2)    // �˵���������
// ���󱨾���ؼ�

//#define  msg_width_def            100    // ��ֵ����С��LCD_WIDTH, ����80
//#define  msg_high_def            60    //��ֵ����С��LCD_HIGH, ����30
//#define  msg_pox_x_def            ((LCD_WIDTH - msg_width_def) / 2)
//#define  msg_pox_y_def            ((LCD_HIGH - msg_high_def) / 2)
//#define  msg_right_def            (msg_pox_x_def + msg_width_def)
//#define  msg_bottom_def            (msg_pox_y_def + msg_high_def)
//#define  msg_str_y_def            (LCD_HIGH-12) / 2

//��ͨ�˵��ؼ�(һ��)
/*
typedef struct
{
    uint16 total; //�ܹ��м���item
    uint16 active; //�״̬�ı��
    const uint16 *title; //�ַ���id ����
    uint8 grade[3];
} function_menu_t;
*/
//��������� define
#define  MENU_PTR_X_DEF  0
#define  MENU_STR_X_DEF       255
#define  MENU_BCK_X_DEF       0    //menu_str_x_def-4
#define  MENU_BCK_WIDTH         76
//#define  menu_ASCIIlength    12
#define  MENU_HEAD_X_DEF       0
#define  MENU_HEAD_Y_DEF       128
//��������� define
#define  MENU_PTR_X         10
#define  MENU_STR_X         32
#define  MENU_BCK_X         9    //menu_str_x_def-4
//#define  MENU_BCK_WIDTH     76
#define  MENU_ASCII_LENGTH  12

//�������ؼ�

typedef struct
{
    //menuhead_t *menuhead;
    uint16 min;
    uint16 max;
    uint16 value;
} progress_t;

//��������� define
#define  PROGRESS_HEIGHT     10      //�����˵ĸ߶����ظ���
#define  PROGRESS_WIDTH      (LCD_WIDTH / 4 * 3)
#define  PROGRESS_POS_Y      6

//�����˿ؼ�
typedef struct
{
    uint16 title;
    uint16 min;
    uint16 max;
    uint16 value;
    uint16 step; //����,ÿ��+/-��ֵ
} slider_t;

//�Ử������������� define
//#define  slider_high_def              19          //���廬���˵ĸ߶����ظ���
#define  SLIDER_WIDTH_DEF           120-28
#define  SLIDER_POX_X_DEF           14
#define  SLIDER_POX_Y_DEF           44
#define  SLIDER_CURSOR_Y_DEF      44

// ��������
//#define  slider_pic_width_def        76
//#define  slider_pic_height_def       60

//��������� define
//#define  list_str_x_def       4
//#define  list_str_y_def        (LCD_HIGH-18)

// �ļ������
//#define  explorer_head_str_x_def        2
//#define  explorer_head_str_y_def    112
#define  EXPLORER_PIC_X_DEF             0
#define  EXPLORER_STR_X_DEF             255
//#define  explorer_row_high_def        15
#define  BROWER_ICON_X_DEF        6
#define  EXPLORER_ROW_COUNT_DEF         4
//#define  explorer_str_len_def        ((LCD_WIDTH - explorer_str_x_def) / (LARGE_CHAR_WIDTH * 2) * 2)

// �ļ����ɾ����������
#define     EXPLORE_ALL            1    // ����ָ���ļ����͵�һ��Ŀ¼���ļ�.
#define     EXPLORE_DIR            2    // ����ָ���ļ����͵�һ��Ŀ¼
#define     EXPLORE_FILE        3    // ��Ŀ¼��ָ���ļ����͵��ļ�
#define     SEL_DIR_ONLY        4    // ����һ��Ŀ¼
#define            DEL_JPG        8        // ɾ��ͼ���ļ�
#define            DEL_MUSIC        9        // ɾ�������ļ�
#define            DEL_VOICE        10        // ɾ��¼���ļ�
#define            DEL_ALL            11    // ɾ�������ļ�.
#define        DEL_TXT        12
#define        DEL_AMV        13    //delete amv file
// Picture def change

// pop up list
typedef struct
{
    //region_t region;
    const uint16 *strid;
    uint16 total; //�˵�ѡ�����
    uint16 active; //���
    uint16 headid; //�˵�����id
} pop_up_list_t;

uint16 ui_popup_list(pop_up_list_t *list,  void(*callback)(uint16 value));
//�ɹ�����0
// �������Է��� RESULT_IGNORE
// ����ֵ���ȼ�

uint16 ui_err_msg(uint16 msgID);//��ʾ���5���������˳�
uint16 ui_show_msg(uint16 msgID);//��ʾ��Ϣ�������˳�
//�ɹ�����0
// ����ֵ���ȼ�

//uint16 ui_menu(function_menu_t *menu , const void(*callback)(uint16 value));
//�ɹ�����0
// �������Է��� RESULT_IGNORE
// ����ֵ���ȼ�
//uint16 ui_menu2(function_menu_t *menu , const void(*callback)(uint16 value));


extern uint16 ui_show_progress(const progress_t *progress);
//extern void ui_show_guage(uint16 step, uint8 para);

//int ui_filelist(file_location_t *location, int type, int string_id);
//int ui_explore(file_location_t *location, int type, int string_id);
// �ļ�·��ֱ�ӷ��� location����
// �����ȼ����򷵻��ȼ���ֵ
// �ɹ�����0
// �������Է��� RESULT_IGNORE


// ������ȫ�ֱ����Ķ���
//extern uint8 file_name[12];
//extern uint8 dir_name[30];
//��������
//extern uint8 *str_pt;        //�ַ�����ǰָ��
//extern uint8 *strhead_pt;   //�ִ���ַ��¼
//extern uint8 scroll_cnt;


// �����Ǹ����ؼ����Ӻ����Ķ���
uint16 rm_dir_from_buf(uint16 n);
extern uint8 read_idm_data(uint8 *name, uint16 count);
extern uint8 write_idm_data(uint8 *name, uint16 count);
//extern void init_dir_buf();
//extern uint8 get_dir_name(uint8 *file, uint16 num);
//extern void get_dir_name2(uint8 *file, uint16 num);
//extern uint16 init_sel_dir_buf(void);
//extern uint8 del_all_my_file(uint16 type, uint8 string_id);
extern void reinit_idm_buf(uint16 top, uint16 bottom, uint16 total_dir, uint16 string_id);
void show_active_list(uint16 old_active, uint16 active, uint16 top);
//uint8 show_popup_list(uint16 string_id);
//int ui_explore_sub(file_location_t *location, int type, int string_id);
//uint16 init_active(uint16 string_id);
//int init_active_dironly(file_location_t  *location);
//void reset_cur_list_bg_color(int i, int tmp);
//void get_dir_long_name(file_location_t *location, int string_id);
void redraw_window(void);
void Clearexplore(void);
void ui_show_data(uint8 *buf, uint8 x, uint8 y);

#endif
