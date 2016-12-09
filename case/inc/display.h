
#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "actapi.h"
#include "port.h"
#include "driver.h"
#include "sddrv.h"
#include "sdfs.h"
#include "irq.h"
#include "chip_cfg.h"
#include "lang_id.h"
#include "ap_cfg.h"
#include "simulatorpc.h"


#define  COLORREF uint32
//FontID
//#define  FONT_TYPE_TINY           0xff
//#define  FONT_TYPE_TINY_INVERT    0xff
#define  FONT_TYPE_SMALL          0
//#define  FONT_TYPE_SMALL_INVERT   1
#define  FONT_TYPE_LARGE          4
//#define  FONT_TYPE_LARGE_INVERT   0xff
//#define      FONT_TYPE_DEFAULT      FONT_TYPE_SMALL
#define  FONT_TYPE_DEFAULT        FONT_TYPE_SMALL

//#define  DISPLAY_BUFFER_ADDR     0x5800

#define  Color_BLACK_def     0x00000000//�� 
#define  Color_WHITE_def     0x0000ffff//�� 
#define  Color_RED_def       0x0000f800//�� 
//#define  Color_YELLOW_def    0x0000fff2//�� 
#define  Color_YELLOW_def    0x0000e748//�� 
#define  Color_GREEN_def     0x000007e0//�� 
#define  Color_BULE_def      0x0000001f//�� 
#define  Color_BULE1_def     0x0000753E//���� 
#define  Color_BULE2_def     0x0000335f//ǳ�� 
#define  Color_MENU_def      0x0000fcc0//�� 
#define  Color_CYAN_def      0x00008cc1//�� 
#define  Color_SAVE_def      0x0000ed80//���汳�� 

#define  Color_MUSIC_def    0x00001d1f //0x000014bf//����ģʽ�µı���
#define  Color_MUSIC_def1    0x0000092d
#define  Color_MENU_BAR      0x00002a6f
#define  Color_MENU_GRD      0x0000efbf
#define  Color_MENU_GRD1     0x00006c35
#define  Color_WAIT_GRD      0x00000410
#define  Color_TOP_BAR       0x0000A6BD

#define  Color_REC_def       0x00009BA9
#define  Color_REC_def1      0x0000C69C

#define  Color_FM_def1       0x0000D759


//������� windows �����г�������ɫ�� 
// Color_tab0106_def �е� 01��ʾ���е�1�У�06��ʾ��6�� 
// �ɲ��� windows ���ʵ�ɫ�� 
#define  Color_tab0101_def   Color_BLACK_def
#define  Color_tab0102_def   0x00008410
#define  Color_tab0103_def   0x00008000
#define  Color_tab0104_def   0x00008400
#define  Color_tab0105_def   0x00000400
#define  Color_tab0106_def   0x00000410
#define  Color_tab0107_def   0x00000010
#define  Color_tab0108_def   0x00008010
#define  Color_tab0109_def   0x00008408
#define  Color_tab0110_def   0x00000208
#define  Color_tab0111_def   0x0000041f
#define  Color_tab0112_def   0x00000210
#define  Color_tab0113_def   0x0000401f
#define  Color_tab0114_def   0x00008200
#define  Color_tab0201_def   Color_WHITE_def
#define  Color_tab0202_def   0x0000c618
#define  Color_tab0203_def   Color_RED_def
#define  Color_tab0204_def   0x0000ffe0
#define  Color_tab0205_def   Color_GREEN_def
#define  Color_tab0206_def   0x000007ff
#define  Color_tab0207_def   Color_BULE_def
#define  Color_tab0208_def   0x0000f81f
#define  Color_tab0209_def   0x0000fff0
#define  Color_tab0210_def   0x000007f0
#define  Color_tab0211_def   0x000087ff
#define  Color_tab0212_def   0x0000841f
#define  Color_tab0213_def   0x0000f810
#define  Color_tab0214_def   0x0000fc08

/*�˵�����������ɫ */
#define Color_BULE_Scroll_def  0x0000001f//���� 

#define SCROLL_REGION_X        0 //45    //30
#define SCROLL_REGION_Y       24
#define SCROLL_REGION_WIDTH    128//140   //87
#define SCROLL_REGION_HEIGHT   16
	  
//#define FirstSectionCode    0xa1
#define DisplayBufLen       1024//468//384//��ʾ�������Ĵ�С 

#define ASCIIDISCODE        0x80
#define FirstSectionCode    0xa1
#define DisplaySuppPage     0x08//0x06//0x08
//lcd use 
#define LCMRST_GIO_EN_REG        GPIOEOUTEN    //GPIO_C0 (Output)
#define LCMRST_GIO_DATA_REG      GPIOEDAT
#define LCMRST_GIO_EN_BIT        0x10
#define LCMRST_SET_BIT           0x10
#define LCMRST_CLR_BIT           0xEF

#define LCMBL_GIO_EN_REG        GPIOCOUTEN        //GPIO_C1 (Output)
#define LCMBL_GIO_DATA_REG      GPIOCDAT
#define LCMBL_GIO_EN_BIT        0x02
#define LCMBL_SET_BIT           0x02
#define LCMBL_CLR_BIT           0xFD  

#define LCMRS_GIO_EN_REG        GPIOBOUTEN        //GPIO_B4 (Output) 
#define LCMRS_GIO_DATA_REG      GPIOBDAT
#define LCMRS_GIO_EN_BIT        0x10
#define LCMRS_GIO_DN_BIT        0xEF
#define LCMRS_SET_BIT           0x10
#define LCMRS_CLR_BIT           0xEF

#define CE_GIO_EN_REG           GPIOAOUTEN        //GPIO_A7 CS pin 
#define CE_GIO_DATA_REG         GPIOADAT
#define CE_EN_BIT               0x80      //ce enable
#define CE_DN_BIT               0x7F
#define CE_SET_BIT              0x80
#define CE_CLR_BIT              0x7F 


//=========================
#ifdef QVGA_LCM
#define Display_Length      242
#define Display_Height      320
#else
#define Display_Length     128//64 //128
#define Display_Height      64//48//64
#endif

#define Lcd_acturalwidth  128//64//128  /*LCD ģ��ʵ�ʵķֱ��ʵ�ʵ�ʳ���*/
#define Lcd_acturalheight 64//48//64 /*LCDģ��ֱ��ʵ�ʵ�ʿ��*/

//for not actural size lcd
//Lcd_widthoffset = (Lcd_acturalwidth - Display_Length)/2
//Lcd_heightoffset (Lcd_acturalheight - Display_Height)/2

#define Lcd_widthoffset 2    /*132X162 ��LCD������ʾʱ�������LCDģ�鳤�ȵ���ʼƫ��*/
#define Lcd_heightoffset 1   /*132X162 ��LCD������ʾʱ�������LCDģ���ȵ���ʼƫ��*/

#define	  AuxiliaryBufferLen	   54
#define   RowWordsCountMax	   26

//��������ˢ�����buf��RAM��� 
#define LCD_RAM_INDEX      0
#define JPEG_RAM_INDEX     1
#define MURAM1L_RAM_INDEX  2
#define MURAM1L_RAM_USB    3

#define JPEG_BUF_ADDR 	 0x3000
#define LCD_BUF_ADDR  	 0x2400
#define MURAM1L_BUF_ADDR 0x5000

#define JPEG_BUF_LEN  	0x2000	//8K
#define LCD_BUF_LEN   	0x400	//1K

#define DMA_RGB    0
#define DMA_YCBCR  1

//lcd cmd
#define LCD_CMD_SETX     0x2a
#define LCD_CMD_SETY     0x2b
#define LCD_CMD_WRITE    0x2c
#define LCD_CMD_READ     0x2e
#define LCD_CMD_SETMODE  0x36

//display mode

#define RGBFLAG     0x08
#define MXENDFLAG   0x01
#define MYENDFLAG   0x02
#define MVENDFLAG   0x03
//add for less mode 
#define MYREVERSE   0x04

#ifdef __C251__
#include <typeext.h>


typedef struct
{
    COLORREF PenColor;      //���ʺ�������ɫ 
    COLORREF BackgdColor;   //������ɫ !!
}DC, *PDC;


#ifdef QVGA_LCM
typedef struct
{
    uint16 x;         //��λ�� 
    uint16 y;         //��λ�� 
    uint16 width;     //���	 
    uint16 height;    //�߶� 
}region_t;
#else
typedef struct
{
    uchar x;         //��λ�� 
    uchar y;         //��λ�� 
    uchar width;     //���	 
    uchar height;    //�߶� 
}region_t;
#endif



/***************************UI����API�ӿ�******************************/
/**********************************************************************/
/**************************1.�������*********************************/
/**************************(a) RCODE�ӿ�*******************************/
void SetAsciiFont(uint8 FontID);           //����Ascii����  FontID���϶��� 
void SetTextPos(uint8 x, uint8 y); //������Ļ����  x:0~127  y:0~96
void SetPenColor(COLORREF color);
void SetBackgdColor(COLORREF color);
void SetTextOutMode(uint8 mod);   //ǰ4λ��ʾ�Ƿ�Ҫ����1��ʾ����,0��ʾ������,��4λ��ʾ͸����0-15,0��ʾ��͸����
void SetTranMode(uint8 Mode);             //���ô������ݵ�ģʽ 
void SetDrawMode(uint8 mode);            //������Ļ�ĺ��������� 
void SetStrEndPosition(uint8 str_endx); 
void SetWindow(region_t *winregion);    //������ʾ��������,winregion,���ڶ��� 

/**************************(b) BANK�ӿ�*******************************/
void SetContrast(uint8 ContrastValue);            //������Ļ�Աȶ� 
bool SetLanguage(uint8 lang_id);                  //���ö������ѡ�� 
//void SetBrightness(char BrightnessValue);      //������Ļ���� 
/********************************************************************/

/***********************2.��ȡ״̬�ӿ�********************************/
/**************************(a) RCODE�ӿ�*******************************/
void  GetFontPoint(uint16 code_rename, uint8 *picbuf);      //ȡ�ַ��ĵ���. code:Ascii�룬GB�룬BIG5�� picbuf:����buffer
char* GetDisplayBufAddr(void);                          //ȡ����ʾBuffer��ַ��	  
void  GetTextPos(uint8 *x, uint8 *y);   //ȡ��Ļ����  
void  GetDCStatus(PDC pdc);                            //ȡ�û�ͼ�豸��״̬������ɫֵ������ɫ�� 
uint8  GetOutPutErr(void);                             //ȡ�����	
//uint8 GetStringLength(char *str, uint16 str_length, char str_type); 
uint8 GetStringLength(uint8 *str,uint16 CharCount,uint8 str_type);
/********************************************************************/

/***********************3.��ͼAPI�ӿ�**********************************/
/**************************(a) RCODE�ӿ�*******************************/
uint16  FontCodeMBToUni(uint16 coderename);  //�������������ת��Ϊunicode�� 

/**************************(b) BANK�ӿ�*******************************/
char* PutUnS(const uint8 *str, int8 strlen);   
char* PutS(const uint8 *str, int8 strlen); 
void  PutSDImage(region_t *imageregion, uint32 sdimageaddr);  //��Flash��ͼ��������ʾ  sdimageaddr:Flash��ַ
//add for trans deal
//void Transdeal(uint16 transcnt,uint16 picwidth,region_t *pImRgn,uint8 *psrcbuf);
void Transdeal(uint8 *psrcbuf,uint16 transcnt);
//==============================================================
void  InvertRegion(region_t *invregion);                   //���� 
void  RegionCopy(region_t *sregion, region_t *tregion);   //������ 
//void RegionMove(region_t *sregion, region_t *tregion); //�����ƶ�  
uint8  CharOutRect(uint8 *str, region_t *CRregion, uint8 colcnt, uint8 language);//matt120802#1
uint8 *PutStringExt(uint8 *str, int8 strlen, int8 IsUnicode, int8 mode);//matt120802#1
void  FillRect(region_t* region);
void  DrawRectFrame(region_t* region );
void  TextOutRect(region_t* region, uint8 *str);
void  Draw3DRect(region_t* D3region, uint8 type); 
//================================================================
uint8 ThaiPoint(const uint8 * str, uint8 IsUnicode, uint16 sorcecode,uint8* charwidth);
uint8 ThaiCharOutRect(uint8*str, region_t *CRregion, uint8 colcnt,uint8 mode);
uint8 ArabicCopy(uint8 *Dst,uint8 *src,int8 Length,uint8 codeType);
bool GetStringLengthExt(uint8 *str, uint16 str_length, uint8 str_type, uint8 * ResCharCount, uint8 * ResColCount);
/********************************************************************/

/***************************4.�����ӿ�********************************/
/**************************(a) RCODE�ӿ�*******************************/
void BuffDataTrans(uint8 *buff, uint16 datacnt);    //��BUFF�������͵�LCM��buff������BUFF���׵�ַ��datacnt�����ݸ��� 
void DispBufSelect(uint8 rampage);      //ѡ��buffer��zram�ĸ�Page��USBר�ú��� 
void DMA3Init(uint8 pictype);

//#ifdef QVGA_LCM
	bool DMA3StartTrans(uint8 dma3_ram);	 //��ʼDMA3���䣬dma3_ram=0, jpeg ram;dma3_ram=1,muram1m. ret=true---success; false---fail
//#else
	//bool DMA3StartTrans(void);				//��ʼDMA3���䣬true---success; false---fail
//#endif

void DMA3SetCounter(uint8 row, uint16 col);	 //����DMA3 counter,ע��length��Ҫ����min[DMA3counter�� Memory length]
void DMA3SetSrcAddr(uint16 src_addr);  //����memory��ʼ��ַ 
void DMA3SetClock();	//ѡ��DMA3 clock      
/**************************(b) BANK�ӿ�*******************************/
void StandbyScreen(bool bStandby);                 //������ʾ��  bStandby=0:����ʾ�� bStandby=1:����ʾ��
void ClearScreen(region_t *clrregion);            //���� clrregion:�������� clrregion=NULL��ʾ��������Ļ
void UpdateScreen(const region_t *region);       //������Ļ  region:��������  region=NULL��ʾ����������Ļ
void LightOnOff(bool OnOff);                    //���ر��⣨true: ��, false:�أ� 
/********************************************************************/
/********************************************************************/
/********************************************************************/
//extern uint8 sysverflag;
//extern  uint32 FontLibAddr;
//extern  uint32 AsciiFontLibAddr;
//extern  DLL_IMP uint8 DisplayBuffer[];
//extern  uint32 MBToUniTabAddr;

//extern uint8 LcdYDataStartAddr[];
//extern uint8 LcdCCDataStartAddr[];

#endif/*__C251__*/

/**************************������Ϣ********************************/
#define  RESULT_PUTCHAR_OK            0//������� 
#define  RESULT_PUTCHAR_X_ERR         1//X�ᳬ�� 
#define  RESULT_PUTCHAR_Y_ERR         2//Y�ᳬ�� 
#define  RESULT_PUTCHAR_OTHER_ERR     3//������ 

/**************************���ݴ���ģʽ����********************************/ 
#define  NORTransMode_def  0    //����ͨ�Ĵ���ģʽ 
#define  DMATransMode_def  1   //��DMA����ģʽ 

/**************************standby mode ����********************************/  
#define  StandbyOFF_def   0     //����ʾ�� 
#define  StandbyON_def    1    //����ʾ�� 
#define  UNICODELANGUAGE  0x7f    //�ַ���ԴΪunicode����ģʽ  

#define UNICODEARABICDONTJOIN 3
#define ISNARABICDONTJOIN  4

//for arabic ligature
#define  ARABICHASJOIN    -2

/**************************��ͼ����ģʽ����********************************/ 
#define  DrawModeH_def      0  //�������� 
#define  DrawModeV_def      1  //��������   
#define  DrawModeHPic_def   2  //��ͼ����ģʽ 
#define  DrawModeVPic_def   3  //��ͼ����ģʽ 
#define  DrawModeAvi_def    4  //AVIģʽ 
#define  DrawModeJpg_def    5  
#define  DrawModeBMP_def    6

/**************************����ģʽ�Ķ���********************************/ 
//#define  SCMFull_def    0    //ȫ����ģʽ 
//#define  SCMUp_def      1    //������ģʽ 
//#define  SCMDown_def    2     //������ģʽ 
//#define  SCMMid_def     3    //�м��ģʽ 

/**************************�ַ��ĵ���ģʽ********************************/ 
//#ifdef   UNICODE_SUPPORT
#define  UnicodeMode      0    //16*16����ģʽ 
#define  PicDisMode       1    //ͼ����ʾģʽ 
/*#else
#define  CharMode0608   0    //06*08����ģʽ 
#define  CharMode0816   1    //08*16����ģʽ 
#define  CharMode1616   2    //16*16����ģʽ 
#define  PicDisMode     3    //ͼ����ʾģʽ  
#endif*/

/**************************����3dЧ��********************************/ 
#define  D3Normalframe_def  0 //ֻ�ӱ߿� 
#define  D3KeyUp_def        1 //ͻ��Ч�� 
#define  D3KeyDown_def      2 //����Ч�� 

/**************************����3dͼ��Ч����ɫ********************************/ 
#define  FaceLightColor_def  Color_tab0202_def   //���ߵ���ɫ 
#define  BackLightColor_def  Color_tab0102_def   //����ߵ���ɫ 
#define  SelectedColor_def   Color_BLACK_def     //��ѡ��ͼƬ�ı߿���ɫ 

//#ifdef  UNICODE_SUPPORT
/**************************����UNICODE�ַ� SIZE*****************************/
#define UNICODE_HEIGHT_DEF     16
#define UNICODE_WIDTH_DEF      16
//#else
/**************************����ascii�ַ������ֵ���� SIZE********************/
/*#define ASCII_SMALL_HEIGHT_DEF  8
#define ASCII_SMALL_WIDTH_DEF   6
#define ASCII_LARGE_HEIGHT_DEF  16
#define ASCII_LARGE_WIDTH_DEF   8
#define CHINESE_HEIGHT_DEF      12
#define CHINESE_WIDTH_DEF       12
#define CHINESE_HEIGHT_DEF1     16
#define CHINESE_WIDTH_DEF1      16
#endif*/


#ifdef __A251__

//RCODE
#define API_UpdateScreen        0x01
//#define API_SetTextOutMode      0x02
#define API_SetAsciiFont        0x02
#define API_SetStrEndPosition   0x03
#define API_BuffDataTrans       0x04
#define API_SetWindow           0x05
#define API_BackLightOnOff      0x06
#define API_SetContrast         0x07
#define API_StandbyScreen       0x08

//BANKA
//BANKA *****************************************************
// ��ע�⣬���ﲻ�������޸ģ����޸�Ҫ�� DRV �е� .xcl �ļ�����Ӧ�ζ�Ӧ
#define API_ClearScreen         0x0000 + API_UI_BaseNoA
#define API_PutSDImage          0x0001 + API_UI_BaseNoA
#define API_FillRect            0x1002 + API_UI_BaseNoA
#define API_GUI_BIDI_IsNSM      0x0003 + API_UI_BaseNoA
//#define API_ThaiOverlap			0x0004 + API_UI_BaseNoA
//#define API_FontCodeMBToUni		0x1004 + API_UI_BaseNoA
#define API_TurnArabicToUn      0x0005 + API_UI_BaseNoA
#define API_MsgBox              0x0006 + API_UI_BaseNoA


//BANKA *****************************************************

//BANKB *****************************************************
#define API_LanguageSet         0x2000 + API_UI_BaseNoB
#define API_Hdrv_HardInit       0x3000 + API_UI_BaseNoB
#define API_ShowLogo            0x0001 + API_UI_BaseNoB
#define API_RegionCopy          0x0002 + API_UI_BaseNoB
#define API_CharOutRect         0x1002 + API_UI_BaseNoB
#define API_PutChar             0x0003 + API_UI_BaseNoB
#define API_PutS                0x1003 + API_UI_BaseNoB
#define API_PutUnS              0x2003 + API_UI_BaseNoB
#define API_GetFontPoint        0x3003 + API_UI_BaseNoB
#define API_PutStringExt       	0x4003 + API_UI_BaseNoB
#define API_SetTextPos          0x5003 + API_UI_BaseNoB
#define API_GetTextPos          0x6003 + API_UI_BaseNoB

#define API_InvertRegion        0x0004 + API_UI_BaseNoB

#define API_GetStringLength     0x0005 + API_UI_BaseNoB
#define API_UnicodeTOArabic     0x0006 + API_UI_BaseNoB
#define API_CopyStr      		0x0007 + API_UI_BaseNoB
#define API_GetStringLengthExt  0x0008 + API_UI_BaseNoB

#define API_ThaiOverlap			0x0009 + API_UI_BaseNoB
#define API_FontCodeMBToUni		0x1009 + API_UI_BaseNoB
//BANKB



 
#endif/*__A251__*/
#endif/*_DISPLAY_H*/