/*
 *******************************************************************************
 *                ACTOS AP
 *        enhanced module impletementation
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 *******************************************************************************
 */

#include <actos.h>
#include <enhanced.h>
//#include "debug.h"
#include <lang_id.h>
#define  NEWFORMATRES

#pragma name(MW_EH_UICODE)

#ifdef   NEWFORMATRES

const uint8 UniToMBTab[][12] =
{   "FTBL_GB.$$$", /*��������codepage table*/
    "FTBL_B5.$$$", /*��������codepage table*/
    "1252L.TBL", /*Ӣ��,��ŷ��ϵ codepage table*/
    "FTBL_JP.$$$", /*���� codepage table*/
    "FTBL_KR.$$$", /*���� codepage table*/
    "1257L.TBL", /*���޵غ���ϵ*/
    "1251L.TBL", /*������ϵ*/
    "1254L.TBL", /*��������ϵ*/
    "1255L.TBL", /*ϣ������*/
    "874L.TBL", /*̩��*/
    "1250L.TBL", /*��ŷ����ŷ��ϵ*/
	"1256L.TBL", /*��������ϵ*/
	"1253L.TBL", /*ϣ����*/
	"863L.TBL" /*Ų����*/
};

#pragma renamecode(MW_EH_UICODE)

bool switchtochar(uint8 *str, int len, uint32 sd_address, uint8 latin_flag);
void SwapIntByte2(uint16 *pword);
/*
 ********************************************************************************
 *
 *
 * Description :
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
bool unitochar(char *str, int len, unsigned char encode)
{
    SD_FILE *fp;
    unsigned char table_index = 0;
    unsigned char LatinFlag = 1; /*������ϵ��־��*/
    unsigned long UniToMBTabAddr = 0;
    switch (encode)
    {
        case CHINESE_SIMPLIFIED:
        table_index = 0;
        LatinFlag = 0;
        break;

        case CHINESE_TRADITIONAL:
        table_index = 1;
        LatinFlag = 0;
        break;

        case ENGLISH:	
        case FRENCH:
        case GERMAN:
        case ITALIAN:
        case DUTCH:
        case PORTUGUESE:
        case SPANISH:
        case SWEDISH:
        table_index = 2;
        break;

        case JAPANESE:
        table_index = 3;
        LatinFlag = 0;
        break;

        case KOREAN:
        table_index = 4;
        LatinFlag = 0;
        break;

        case LITHUANIAN:  //white110118#1 for canyon
	    case ESTONIAN:    //white110118#1 for canyon
		case LATVIAN:     //white110118#1 for canyon
        case CZECH:
        case POLISH:
        table_index = 5;
        break;

        case RUSSIAN:
        case BULGARIAN:
        case UKRAINIAN:
        table_index = 6;
        break;

        case TURKISH:
        table_index = 7;
        break;

        case HEBREW:
        table_index = 8;
        break;

        case THAI:
        table_index = 9;
        LatinFlag = 0;
        break;

        case DANISH:
        case HUNGARIAN:
        case ROMANIAN:
        case  CROATIA:
        case  SLOVAK:
        case SERBIAN:
        table_index = 10;
        break;

        case ARABIC:
        table_index = 11;
        break;
        
        case GREEK:
        table_index = 12;
        break;

        case NORWEGIAN:
        table_index = 13;
        break;

        default:
        table_index = 0;
        break;
    }
    /*��unicode to char ת����*/
    fp = SD_FOpen(UniToMBTab[table_index], MODE_READ);
    if (fp == NULL)
    {
        //ERR(printf("Cannot open UniFontTab\n"));
        return FALSE;
    }
    UniToMBTabAddr = fp->startaddr;
    SD_FClose(fp);
    return switchtochar(str, len, UniToMBTabAddr, (char) LatinFlag);
}

/*
 ********************************************************************************
 *
 *
 * Description :
 *
 * Arguments   :�ؼ�����unicode��Ŀ���ַ�����ת����������������Ӧ��Ŀ���ַ�����ASCII�붼��һ���ֽڵ�
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
bool switchtochar(uint8 *str, int len, uint32 sd_address, uint8 latin_flag)
{
    int s_cnt;
    int d_cnt;
    uint16 FontCode;
    SD_RW sdrw;

    sdrw.srampage = 0x08;
    sdrw.sramaddr = (unsigned int) &FontCode;
    for (d_cnt = 0, s_cnt = 0; (s_cnt < len) && ((str[s_cnt] != 0) || (str[s_cnt + 1] != 0));)
    {
        FontCode = *((uint16 *) &str[s_cnt]);//�ַ���ֵ
        //input code is small endian need change to big endian
        SwapIntByte2(&FontCode);
        //��ASCII���ת��
        if (FontCode > 127)
        {
            sdrw.faddr = sd_address + (((DWORD) FontCode) << 1);
            SD_Read(&sdrw, 2); //SD_Read((unsigned int)&sdrw,2);
            //Z80��С�ˣ�KEIL�Ǵ�ˡ�������Ҫע�⣬������Բ�������ʾ�������޸�����
            //��ת������Ŀ���ַ����������ʾ

            str[d_cnt++] = *((uint8*) &FontCode); //low byte
            //��������ϵ�������������ַ�����ʾ
            if ((latin_flag == 0) && (FontCode > 0x7f)) //���ֽ������ת��
            {
                str[d_cnt++] = *((uint8*) &FontCode + 1); //high byte
            }
        }
        else
        {
            str[d_cnt++] = str[s_cnt]; //ֻȡ��λ,��õ��Ƕ�Ӧ�ַ���������
        }
        s_cnt += 2;
    }
    str[d_cnt] = 0;

    return TRUE;
}

#else
typedef struct
{
    word uni_code;
    byte latin_code;
}uni_latin;

#pragma userclass(near = MOC_EH_UNI)

const uni_latin ConvertTab[27]= //"27" meaning:0x20-5

{   0x20ac,0x80, 0x201a,0x82,0x0192,0x83,0x201e,0x84,0x2026,0x85,0x2020,0x86,0x2021,0x87,
    0x02c6,0x88,0x2030,0x89,0x0160,0x8a,0x2039,0x8b,0x0152,0x8c, 0x017d,0x8e,
    0x2018,0x91,0x2019,0x92,0x201c,0x93,0x201d,0x94,0x2022,0x95,0x2013,0x96,0x2014,0x97,
    0x02dc,0x98,0x2122,0x99,0x0161,0x9a,0x203a,0x9b,0x0153,0x9c, 0x017e,0x9e,0x0178,0x9f
};
#pragma userclass(near = default)

#define FindTimes (sizeof(ConvertTab)/sizeof(ConvertTab[0]))   //Ҳ����ֱ�Ӹ�"27"
#pragma renamecode(MO_EH_UNI)

/*
 ********************************************************************************
 *
 *
 * Description :
 *
 * Arguments   : inchar ������ַ�����ַ   codeoffsetaddr ���ձ��е���ַ
 *
 * Returns     : outchar ������ַ�����ַ
 *                       ��������е���Ч�ֽ�����Ϊ0��ʾû�ַ����
 *
 * Notes       :
 *
 ********************************************************************************
 */
WORD OneUniToChar(WORD unicode, const SD_FILE *fp)
{
    WORD code;
    ulong unicode1;

    unicode1=((ulong)unicode)*2;
    if(unicode>127)
    {
        if(!SD_FSeek(fp,SEEK_SET,unicode1))
        return 0x20;
        if(SD_FRead(fp,&code,2))
        return code;
    }
    else //��ASCII

    {
        return unicode;
    }
    return 0;
}

/*
 ********************************************************************************
 *
 * Description : ������:OneUniToLatin1  ����:��һ��Latin1�ַ���unicodeת���ɶ�Ӧ��Latin1����
 *
 * Arguments   : ����:unicode ת����Latin1�ַ�unicodeֵ
 *
 * Returns     : word: ת����õ���Latin����.����:�������unicode��Latin�ַ�ʱ,�򷵻�0x0020!
 *
 *
 * Notes       : for Latin1 unicode convert! by ccm
 *
 ********************************************************************************
 */

word OneUniToLatin1(unsigned int unicode)
{
    byte i;

    if ((unicode<=0x007f) || ((unicode>=0x00a0)&&(unicode<=0x00ff))) //����д��:(0x00a0<=unicode<=0x00ff)! by ccm!
    return unicode;
    else
    {
        for(i=0;i<FindTimes;i++)
        {
            if (unicode==ConvertTab[i].uni_code)
            {
                return (word)ConvertTab[i].latin_code; //�ҵ�,���ض�Ӧ��Latin1����!
            }
        }
        if (i==FindTimes)
        return unicode; //�������unicode��ת�����в��Ҳ���(��Latin�ַ�),�򷵻�0x0020!
    }
}

/*
 ********************************************************************************
 *
 *
 * Description :
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
bool unitochar(char *str,int len,unsigned char encode)
{
    int s_cnt=0;
    int d_cnt=0;
    WORD code;
    SD_FILE *fp;

    const char ftbl_B5[]="FTBL_B5.$$$";
    const char ftbl_GB[]="FTBL_GB.$$$";
    const char ftbl_JP[]="FTBL_JP.$$$";
    const char ftbl_KR[]="FTBL_KR.$$$";

    switch(encode)
    {
        case CP_CHS: //���ļ���
        fp=SD_FOpen(ftbl_GB,MODE_READ);
        break;

        case CP_CHT: //���ķ���
        fp=SD_FOpen(ftbl_B5,MODE_READ);
        break;
        case 3: //����
        fp=SD_FOpen(ftbl_JP,MODE_READ);
        break;
        case 4: //����
        fp=SD_FOpen(ftbl_KR,MODE_READ);
        break;

        default: //ASCII��������
        // add by ccm for Latin1 unicode convert.
        //Latin1��ASCIIʹ��ͬһ���ֿ�ASCII.BIN������UI_L0724.DRV!
        for(d_cnt=0,s_cnt=0;(s_cnt<len)&&((str[s_cnt]!=0)||(str[s_cnt+1]!=0)); )
        {
            code=OneUniToLatin1(str[s_cnt]+str[s_cnt+1]*0x100);
            str[d_cnt++]=code;
            s_cnt+=2;
        }
        str[d_cnt]=0; //��β��0
        return 1; //converted complete !   by ccm
    }

    if(fp==NULL)

    {
        SD_FClose(fp);
        return 0;
    }

    for(d_cnt=0,s_cnt=0; (s_cnt<len)&&( (str[s_cnt]!=0) || (str[s_cnt+1]!=0) ); ) // quinn 070621 modify

    {
        code=OneUniToChar(str[s_cnt]+str[s_cnt+1]*0x100, fp);
        str[d_cnt++]=code;//���˵�λ
        if(code>127)
        {
            str[d_cnt++]=code>>8;//�Ѹ�λ�Ƶ���λȥ
        }
        s_cnt+=2;

    }
    str[d_cnt]=0; //Rcmai 04-1-31 20:37
    SD_FClose(fp);

    return 1;
}
#endif
void SwapIntByte2(uint16 *pword)
{
    uint8 high_byte;
    uint8 low_byte;

    high_byte = (uint8) ((*pword >> 8) & 0xff);
    low_byte = (uint8) (*pword & 0xff);
    *((uint8*) pword + 1) = high_byte;
    *((uint8*) pword) = low_byte;
}
