/*
 *******************************************************************************
 *                ACTOS AP
 *        ap common lib, directory control file
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 * File: Dir_main.c
 * By  : stevenluo
 * Version: v1.00     first version     2010-04-09 12:00
 *******************************************************************************
 */

#include "ap_common.h"
extern uint16 g_musicsequence;
extern void dir_getSelNumByFileName(file_location_t *location);
#pragma name(DIR_MAIN)

/*######################################################################*/
/*#########################BANK�ڵĳ�����#############################*/
/*######################################################################*/
static const uint8 file_pos[3] =
{16,32,48};

//{ POSITION_MENU1, POSITION_MENU2, POSITION_MENU3, POSITION_MENU4, POSITION_MENU5, POSITION_MENU6 }; //�б�������λ��
//static const uint16 file_bck_pic[6] =
//{ MENUBCK6, MENUBCK5, MENUBCK4, MENUBCK3, MENUBCK2, MENUBCK1 };//�б���ı���ͼ
//static const uint16 headID_pic[8] =
//{ T_MUSIC, T_VOICE, T_BROWSER, T_PHOTO, T_EBOOK, T_MOVIE, T_BROWSER, T_RECORD };//�б�ͷ��ͼƬ��Ϣ
static const uint16 file_icon[3] =
{ IM_BROWSER_ICON_FOLDER, ICON_FILE, IM_BROWSER_ICON_MUSIC};//�б�ѡ���ͼ��
static const uint16 file_icon1[3] =
{ IM_BROWSER_ICON_FOLDER, IM_BROWSER_ICON_MUSIC, IM_BROWSER_ICON_MUSIC };//�б�ѡ���ͼ��

/*######################################################################*/
/*#########################��פ���ݶ�################################*/
/*######################################################################*/
file_record_m dir_temp_record; //���浱ǰ�б�������ļ����ͺ��ļ�����
uint8 longname_flag; //��ǰ�ļ�ѡ���Ƿ�Ϊ�������
uint8 DiskFlag = 0; //�̷����0='C',1='H'
//uint8 file_mode;
uint16 dir_total_num = 0; //��ǰĿ¼���ļ�������
uint16 file_total_num = 0; //��ǰĿ¼���˺���ļ�����
uint16 list_total_num = 0; //��ǰĿ¼�ļ��к��ļ����ܺͣ��ټ���Ŀ¼��
uint16 dir_list_active = 0; //��ǰ�б�ļ�����
uint16 old_list_active = 0;//��ǰ�б�ɵļ�����
uint16 cur_group = 0; //��ǰ�б����list_total_num�ķ����������λ��
uint16 top; //��ǰ�б�Ŀ�ʼ��
uint16 bottom; //��ǰ�б�Ľ�����
uint8 selmode = 0;//�ļ����ʱ�ļ��л������3-�л���һ�����ݣ�2-�л���һ�����ݣ�1-�ӵ�һ���ļ��л������һ����
uint8 selmode_changeflag = 0;//�ļ����ʱ�л�����ı���
uint8 need_decode=TRUE;
/*######################################################################*/
/*#########################�����#####################################*/
/*######################################################################*/

/********************************************************************************
 * Description : ���ݵ�ǰ���ļ����ͺ����������ѡ�������
 *
 * Arguments  :
 *            icon��0-�ļ���ͼ�꣬1-�ļ�ͼ�꣬2-ͼƬͼ��
 *		 index����ǰ�б��������
 * Returns     :
 *            ��
 * Notes       :
 *
 ********************************************************************************/
void put_StrTypeAndIcon(uint8 icon, uint8 index)
{

    uint8 sfr_bak, mcsr9_bak;
    uint8 filename[50];
    uint8 longnameflag;
    if ((icon > 0) && (dir_fselType == FSEL_TYPE_DSC))
    {
        icon = icon + 1; //��ʾͼƬӦ���б��Сͼ��
    }
	if(dir_temp_record.name[0]=='\\')
	{
	 	/*if(dir_list_active == 0)
		{
		 	ResShowPic(ICON_ROOT_DIR,0, file_pos[0]);
     	 	ResShowPic(ICON_BACK_DIR_SELECT,16, file_pos[0]);
		}
		else
		{
			ResShowPic(ICON_ROOT_DIR,0, file_pos[0]);
     	 	ResShowPic(ICON_BACK_DIR,16, file_pos[0]);
		}*/
	 return;
	}
	else if(memcmp(dir_temp_record.name,"ROOT",5)==0)//����Ŀ¼
	{
		/*if(dir_list_active == 0)
		{
		 	ResShowPic(ICON_ROOT_DIR,0, file_pos[0]);
     	 	ResShowPic(ICON_HOME_SELECT,16, file_pos[0]);
		}
		else
		{
			ResShowPic(ICON_ROOT_DIR,0, file_pos[0]);
     	 	ResShowPic(ICON_HOME,16, file_pos[0]);
		}*/
     	return;
	}
	else
	{
			ResShowPic(file_icon[dir_temp_record.filetype],BROWER_ICON_X_DEF, file_pos[index]);
			/*if(activeflag == TRUE)
			{
						ResShowPic(file_icon1[dir_temp_record.filetype],BROWER_ICON_X_DEF, file_pos[index]);	//active - dir_top
						ResShowPic(IM_MENU_TEXT_SEL,0, file_pos[index]);
						activeflag = FALSE;
			}*/
	}
//    ResShowPic(file_icon[icon],17, file_pos[index]);//ͼ����ʾ	
	SetAsciiFont(FONT_TYPE_LARGE);
    SetTextPos(19, file_pos[index]);//�ļ�������
    DRAM251B_SetMCU_CLK(sfr_bak, mcsr9_bak)
    memcpy(filename, &dirbuf_filename[index], 50);
    longnameflag = dirbuf_longname_flag[index];
    DRAM251B_Restore_CLK(sfr_bak, mcsr9_bak)
    if (longnameflag == TRUE)
    {
        PutUnS(filename, -1);
    }
    else
    {
        PutS(filename, -1);
    }
    //SetStrEndPosition(0);

}
/********************************************************************************
 * Description : ��ȡ��ǰ���������Ϣ����
 *
 * Arguments  :
 *            selNum: ��ǰ�������
 * Returns     :
 *            ��
 * Notes       :
 *
 ********************************************************************************/
void show_active_Item(uint16 selNum)
{

    uint8 tmp = (uint8) (selNum % ROW_COUNT_DEF);
    uint8 sfr_bak, mcsr9_bak;
    DRAM251B_SetMCU_CLK(sfr_bak, mcsr9_bak)
    memcpy(&dir_temp_record, &OnePageRecord[tmp], sizeof(file_record_m));
    memcpy(scroll_buf, dirbuf_filename[tmp], 50);
    DRAM251B_Restore_CLK(sfr_bak, mcsr9_bak)

}
/********************************************************************************
 * Description : ���ݲ�ͬAP��Ҫ��ʾ���ļ���ʽ��ʾ�б�ͷ��ͼƬ��Ϣ
 *
 * Arguments  :
 *            index: ������ļ���������Ӧ��ͼƬID:
 *#define FSEL_TYPE_MUSIC         0      //�����ļ�
 *#define FSEL_TYPE_VOICE         1      //�����ļ�
 *#define DIRECTORY_ALL           2       //�����ļ���ʽ
 *#define FSEL_TYPE_DSC       3		  //ͼƬ�ļ�
 *#define	FSEL_TYPE_TXT	    4         //�������ļ�
 *#define	FSEL_TYPE_AMV	    5         //��Ƶ�ļ�
 *#define	FSEL_TYPE_ALL	    6        //�����ļ���ʽ
 *#define	FSEL_TYPE_DIR       7         //�ļ���
 * Returns     :
 *            ��
 * Notes       :
 *
 ********************************************************************************/


/********************************************************************************
 * Description : �ļ��������ʾ����
 *
 * Arguments  :
 *            top: �ļ��б�Ŀ�ʼ��
 *            bottom : �ļ��б�Ľ�����
 Old_active���ļ�ѡ��ľɼ�����
 active���ļ�ѡ����¼�����
 total_num����ǰĿ¼���ļ���������ʱ������
 * Returns     :
 *            ��
 * Notes       :
 *
 ********************************************************************************/
void dir_ShowDirectory(uint16 dir_top, uint16 dir_bottom, uint16 Old_active, uint16 active, uint16 total_num)
{	

    uint16 i, tmp;
    uint8 str_lenth;
    int8 str_type;
    int8 language_id;
    uint8 temp_flag;
    uint8 sfr_bak, mcsr9_bak;
//	uint8 type_file;
    SetAsciiFont(FONT_TYPE_LARGE);
    if (!total_num)
    {

    }
    //show_head_info(active,total_num);
  
    if (Old_active != active)
    { 
        //����ɵĻ�˵���
        tmp = (Old_active - dir_top) % ROW_COUNT_DEF;
        region_scroll.x = 0;
        region_scroll.width = Display_Length;
        region_scroll.height = 16;
        region_scroll.y = file_pos[tmp];
        ClearScreen(&region_scroll); 
       // ResShowPic(file_bck_pic[tmp], MENU_PTR_X_DEF, file_pos[tmp] - 2);
        DRAM251B_SetMCU_CLK(sfr_bak, mcsr9_bak)
        memcpy(&dir_temp_record, &OnePageRecord[tmp], sizeof(file_record_m));
        DRAM251B_Restore_CLK(sfr_bak, mcsr9_bak);
        put_StrTypeAndIcon(dir_temp_record.filetype, (uint8) tmp);
		show_active_Item(active - dir_top);  
    }
    else //if(Old_active==active)        //�¾ɻ����ͬ��ʾ���µ�һ������,��Ҫȫ��ˢ��
    {  
        ClearScreen(NULL);
//		ResShowPic(file_icon[dir_temp_record.filetype],BROWER_ICON_X_DEF - 1, file_pos[active - dir_top]);
//		ResShowMultiString(BROWSER,255, 0);//��ʾĿ¼���
        for (i = dir_top; i <= dir_bottom; i++)
        {
							DRAM251B_SetMCU_CLK(sfr_bak, mcsr9_bak)
            	memcpy(&dir_temp_record, &OnePageRecord[i - dir_top], sizeof(file_record_m));
            	DRAM251B_Restore_CLK(sfr_bak, mcsr9_bak)
            	if (i == active)
            	{                   	
					DRAM251B_SetMCU_CLK(sfr_bak, mcsr9_bak)
                	memcpy(scroll_buf, dirbuf_filename[i - dir_top], 50);
                	DRAM251B_Restore_CLK(sfr_bak, mcsr9_bak)
                	continue;
            	}
				
            	put_StrTypeAndIcon(dir_temp_record.filetype, (uint8) (i - dir_top));
        }
    }

    DRAM251B_SetMCU_CLK(sfr_bak, mcsr9_bak)
    temp_flag = dirbuf_longname_flag[active - dir_top];
    DRAM251B_Restore_CLK(sfr_bak, mcsr9_bak)
    if (temp_flag!=0)
    {
        str_lenth = 25;
        str_type = 1;
        language_id = UNICODELANGUAGE;
        longname_flag = 1;
    }
    else
    {
        str_lenth = 50;
        str_type = 0;
        language_id = 0;
        longname_flag = 0;
    }
    
    if ((GetStringLength(scroll_buf, str_lenth, str_type)) < 96)
    {
    	ResShowPic(IM_MENU_TEXT_SEL,0, file_pos[active - dir_top]); //��ʾָʾͼ�� 
//        ResShowPic(file_icon1[dir_temp_record.filetype],BROWER_ICON_X_DEF, file_pos[active - dir_top]);
		ScrollFlag = 0;
        show_active_Item(active - dir_top);
        //ResShowPic(FILEBCK, EXPLORER_STR_X_DEF - 2, file_pos[active - dir_top] - 1);
        put_StrTypeAndIcon(dir_temp_record.filetype, (uint8) (active - dir_top));
    }
    else
    { 
    	ResShowPic(IM_MENU_TEXT_SEL,0, file_pos[active - dir_top]); //��ʾָʾͼ�� 
//        ResShowPic(file_icon1[dir_temp_record.filetype],BROWER_ICON_X_DEF, file_pos[active - dir_top]);
        ScrollFlag = 1;
		region_scroll.x = 19;
        region_scroll.width = 96;//Display_Length - region_scroll.x;
        region_scroll.height = 16;
        region_scroll.y = file_pos[active - dir_top];
	    ClearScreen(&region_scroll);  
//        ScrollString(scroll_buf, TRUE, language_id, &region_scroll);
#if 0
        UpdateScreen(NULL);
        //test
        i = 0;
        while(1)
        {
            if(i>3000)
                 
            {
                ScrollString(scroll_buf, FALSE, UNICODELANGUAGE, &region_scroll);
                UpdateScreen(NULL);
                i =0 ;
            }
            else
            {
                i++;
            }
            
        }
#endif
        
    }
   // InvertRegion(&region_scroll); //ѡ�е�Ŀ¼���� 
    UpdateScreen(NULL);
//    SetAsciiFont(FONT_TYPE_SMALL);
}

/********************************************************************************
 * Description : �ļ���������ں���
 *
 * Arguments  :
 *            browser_vars_t: �ļ������Ĺ��ܽṹ��
 *            type : ������ļ�����:
 *#define FSEL_TYPE_MUSIC         0      //�����ļ�
 *#define FSEL_TYPE_VOICE         1      //�����ļ�
 *#define DIRECTORY_ALL           2       //�����ļ���ʽ
 *#define FSEL_TYPE_DSC       3		  //ͼƬ�ļ�
 *#define	FSEL_TYPE_TXT	    4         //�������ļ�
 *#define	FSEL_TYPE_AMV	    5         //��Ƶ�ļ�
 *#define	FSEL_TYPE_ALL	    6        //�����ļ���ʽ
 *#define	FSEL_TYPE_DIR       7         //�ļ���
 * Returns     :
 *            ��Ӧ��result ֵ
 * Notes       :  only select the valid directory
 *
 ********************************************************************************/
uint16 ui_directory(file_location_t *g_browser_vars, uint8 type)
{

    uint16 result;
    file_location_t temp_location;
    list_total_num = 0;
    dir_total_num  = 0;
    file_total_num = 0;
//    file_mode = type;
	
	//=========
//	dir_list_active =g_musicsequence;//�������к� longjin
	//=========
    memcpy(&temp_location, g_browser_vars, sizeof(file_location_t));
    dir_fselInit(type, FSEL_ALL_REPEATALL, 0); //��ʼ����ǰ���̵��ļ�ϵͳ
    dir_fselSetLocation(g_browser_vars);
    result = dir_ui_directory(g_browser_vars, type);
    if (result == 0)
    {
        return dir_File_Type(g_browser_vars->filename);
    }
    else if (result == RESULT_IGNORE)
    {
        dir_fselSetLocation(&temp_location);
		dir_getSelNumByFileName(&temp_location);//�����ļ��������ļ���� longjin
        memcpy(g_browser_vars, &temp_location, sizeof(file_location_t));	
    }
    else
    {

    }
    return result;

}

