/*
 *******************************************************************************
 *                ACTOS AP
 *        wma player basal module implementation
 *
 *        (c) Copyright Actions Co,Ld.
 *
 *  $Id: ui_scroll.c,v 1.1.1.1 2006/04/21 08:22:43 selina Exp $
 *******************************************************************************
 */
/*
 ����������������ڻ��������һ�� C ,���������C����BANK��ʽCALL
 */

#include "actos.h"
#include "ap_common.h"
#pragma name(COM_UI_SCROLL2)


//===============
uint8 *str_pt2;//�ַ�����ǰָ��
uint8 *strhead_pt2;//�ִ���ַ��¼
uint8 scroll_cnt2;
//uint8 ScrollFlag2;
uint8 scroll_buf2[104];
region_t region_scroll2; //����
uint8 InvertFlag2;
//================


void ScrollStringInit2(uint8 *str,uint8 language,region_t *scroll);
void HoriScrollString2(int8 step, uint8 language, region_t *scroll);

/* (�ַ�����ַ���Ƿ��ʼ��trueΪҪ��ʼ��) */
void ScrollString2( uint8 * scrollstr, bool mode ,uint8 language,region_t *scroll)
{
    uint8 i;
    
	/*���÷�͸������*/
	//SetTextOutMode(0);
	if (mode)
	{
		//��ʼ����������һ�ε���ʱ����ʾһ���ַ���
		if(language !=UNICODELANGUAGE)
		{
			for(i=0;i<50;i++)
			{
				if(scroll_buf2[i] == '\0')
				{
					break;
				}
			}
			/*����ո��*/
			if(i>=50)
				i=50;
			scroll_buf2[i] = ' ';
			scroll_buf2[i+1] = ' ';
			scroll_buf2[i+2] = '\0';
		}
		else
		{
            for (i = 0; i < 100; i += 2)
            {
                if ((scroll_buf[i] == 0) && (scroll_buf[i + 1] == 0))
                {
                    break;
                }
                //i++;
            }
            /*����ո�� */
            if (i < 100)
            {
                scroll_buf[i] = 0x20;
                scroll_buf[i + 1] = 0;
                scroll_buf[i + 2] = 0;
                scroll_buf[i + 3] = 0;
            }
            else
            {
                scroll_buf[100] = 0x20;
                scroll_buf[101] = 0;
                scroll_buf[102] = 0;
                scroll_buf[103] = 0;
            }
		}
		ClearScreen(scroll);
		ScrollStringInit2(scrollstr,language,scroll);
	}
	else
	{
		//��������ϣ�����Ѿ���֮ǰ���ӹ�����ΪUnicode���� by cailizhen
		if(g_comval.langid == ARABIC||g_comval.langid == HEBREW)
		{
			language = UNICODELANGUAGE;
		}
		HoriScrollString2(1,language,scroll);
	}
	 
}

void ScrollStringInit2(uint8 *str,uint8 language,region_t *scroll)
{
//	uint16 count;
	uint8 result;
	region_t kregion;//���²�������
	scroll_cnt2 = 0;
	strhead_pt2 = str;
	SetTextPos(scroll->x, scroll->y);
	//SetStrEndPosition(scroll->x + scroll->width);
	//�԰������������������ת����Ȼ����������ؽ����ַ���ָ�� by cailizhen
	str_pt2 =PutStringExt(str, -1, language==UNICODELANGUAGE, 0x04);
	SetStrEndPosition(0);
	if(*str_pt2!=0)
	{
		GetTextPos(&kregion.x, &kregion.y);
		kregion.width = scroll->width+scroll->x - kregion.x;
		kregion.height = scroll->height;
		
		//������һ���֣�����str_pt���Ա��̩�������ַ����ӵ���ǰ�ַ��� by cailizhen
		result = CharOutRect(str_pt2, &kregion, scroll_cnt2, language);
		if (result)
		{
			return;
		}
		scroll_cnt2 = kregion.width;
	}
}

//==========================================================
/*
������Ļ
*/
void HoriScrollString2(int8 step,uint8 language,region_t *scroll)
{
//	uint16 ucode;
	region_t tregion;
	region_t sregion;
	region_t kregion;//���²�������
	uint8 Result;
	
	//���²�������
	kregion.x = scroll->x + scroll->width - step;
	kregion.y = scroll->y;
	kregion.width = step;
	kregion.height = scroll->height;
	
	//�趨�ƶ���Դ��Ŀ������
	sregion.x = scroll->x + step;//��ʱδ����Ϊ���������
	tregion.x = scroll->x;
	tregion.y = sregion.y = scroll->y;
	tregion.height = sregion.height = scroll->height;
	tregion.width = sregion.width =scroll->width - step;
	//������step��
	RegionCopy(&sregion, &tregion);
	
test:
	if( language == UNICODELANGUAGE )
	{
		if( *(int *)str_pt2 ==0x0000 )
		{
			str_pt2 = strhead_pt2;
		}
	}
	else  if (*str_pt2 == '\0')
	{
		str_pt2 = strhead_pt2;
	}
	//����str_pt��ȥ���Ա��̩��������⴦�� by cailizhen
	Result = CharOutRect(str_pt2, &kregion, scroll_cnt2, language);
	if (Result)	//�������һ������
	{
		str_pt2 +=Result;
		scroll_cnt2 = 0;
		goto test;
	}
	
	/*if(InvertFlag2)
	{
		InvertRegion(&kregion);
	}*/
	scroll_cnt2 += step;//δ֧�ָ���
	
}
/*
 ********************************************************************************
 * Description : ��ָ��λ�ù�����ʾ�������ַ���
 *
 * Arguments   : id, ��Դ����
 x, y, ��Ļ����,x=255 ��ʾx�������
 *       scroll_width, ��������Ŀ��
 * Returns     : �ɹ�, 1
 ʧ��, 0
 *
 * Notes       :
 *
 ********************************************************************************
 */
 
uint16 ResShowMultiString_Scroll2(uint16 str_id, uint8 x, uint8 y, uint8 scroll_width)
{
#if 1
    uint8 str_len;
    uint8 strWidth;
    uint16 str_size;
    res_langid_entry_t res_langid_entry;
    /*��ȡ�ַ���ID������Ϣ */
    memset(scroll_buf2, 0, sizeof(scroll_buf2));
	str_len=scroll_width ;
    str_size = sizeof(res_langid_entry_t);
    SD_FSeek(res_fp, SEEK_SET, string_entry_offset + (uint32) (str_id * str_size)); // ָ��ָ���ַ����ļ��е�λ��
    SD_FRead(res_fp, &res_langid_entry, str_size); //res_fp->&res_entry
    SIM_ChgLongPC((BYTE *) &res_langid_entry.dwOffset);
    SIM_ChgWordPC((BYTE *) &res_langid_entry.dwLength);

    /*��ȡ�ַ������ݣ�unicode���� */
    //    str_len = (uint8) (res_langid_entry.dwLength < 100) ? (res_langid_entry.dwLength) : (100);
    if (res_langid_entry.dwLength < 100)
    {
        str_len = (uint8) res_langid_entry.dwLength;
    }
    else
    {
        str_len = 100;
    }
    SD_FSeek(res_fp, SEEK_SET, res_langid_entry.dwOffset);
    SD_FRead(res_fp, scroll_buf2, str_len);

    /*�Ƿ������ʾ�ַ��� */
    strWidth = GetStringLength(scroll_buf2, (uint16) str_len / 2, 1);
    if (strWidth > 96)
    {
        /*init scroll parameters*/
        region_scroll2.width = 96;
        region_scroll2.height = SCROLL_REGION_HEIGHT;
        region_scroll2.x = x;
        region_scroll2.y = y;
        scroll_buf2[str_len - 2] = ' ';
        scroll_buf2[str_len - 1] = 0x00;
        scroll_buf2[str_len] = ' ';
        scroll_buf2[str_len + 1] = 0x00;
        scroll_buf2[str_len + 2] = 0x00;
        scroll_buf2[str_len + 3] = 0x00;
        ScrollFlag2 = TRUE;
        ScrollString2(scroll_buf2, TRUE,UNICODELANGUAGE , &region_scroll2);// ARABICHASJOIN
    }
    else
    {
        ScrollFlag2 = FALSE;
        SetTextPos(x, y);
        PutUnS(scroll_buf2, -1);//ARABICHASJOIN
    }
#endif
    return 1;
    
}
void ScrollString2_re(void)
{
  ScrollString2(scroll_buf2, FALSE,UNICODELANGUAGE,&region_scroll2);
}