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
#pragma name(COM_UI_SCROLL)

void ScrollStringInit(uint8 *str,uint8 language,region_t *scroll);
void HoriScrollString(int8 step, uint8 language, region_t *scroll);
uint16 CountString(uint8 *str, uint8 language, region_t *scroll);
#if 0
void SwapIntByte(uint16 *pword)
{
    uint8 high_byte;
    uint8 low_byte;

    high_byte = (uint8) ((*pword >> 8) & 0xff);
    low_byte = (uint8) (*pword & 0xff);

    *((uint8*) pword + 1) = high_byte;
    *((uint8*) pword) = low_byte;
}
#endif
/* (�ַ�����ַ���Ƿ��ʼ��trueΪҪ��ʼ��) */
void ScrollString( uint8 * scrollstr, bool mode ,uint8 language,region_t *scroll)
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
				if(scroll_buf[i] == '\0')
				{
					break;
				}
			}
			/*����ո��*/
			if(i>=50)
				i=50;
			scroll_buf[i] = ' ';
			scroll_buf[i+1] = ' ';
			scroll_buf[i+2] = '\0';
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
		ScrollStringInit(scrollstr,language,scroll);
	}
	else
	{
		//��������ϣ�����Ѿ���֮ǰ���ӹ�����ΪUnicode���� by cailizhen
		if(g_comval.langid == ARABIC||g_comval.langid == HEBREW)
		{
			language = UNICODELANGUAGE;
		}
		HoriScrollString(1,language,scroll);
	}
	 
}
 void ScrollStringInit(uint8 *str,uint8 language,region_t *scroll)
{
//	uint16 count;
	uint8 result;
	region_t kregion;//���²�������
	scroll_cnt = 0;
	strhead_pt = str;//�ļ�ͷ
	SetTextPos(scroll->x, scroll->y);
	//SetStrEndPosition(scroll->x + scroll->width);//���
	//�԰������������������ת����Ȼ����������ؽ����ַ���ָ�� by cailizhen
	str_pt =PutStringExt(str, -1, language==UNICODELANGUAGE, 0x04);
	SetStrEndPosition(0);
	if(*str_pt!=0)
	{
		GetTextPos(&kregion.x, &kregion.y);
		kregion.width = scroll->width+scroll->x - kregion.x;
		kregion.height = scroll->height;
		
		//������һ���֣�����str_pt���Ա��̩�������ַ����ӵ���ǰ�ַ��� by cailizhen
		result = CharOutRect(str_pt, &kregion, scroll_cnt, language);//
		if (result)
		{
			return;
		}
		scroll_cnt = kregion.width;
	}
}

//==========================================================
/*
������Ļ
*/
void HoriScrollString(int8 step,uint8 language,region_t *scroll)
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
		if( *(int *)str_pt ==0x0000 )
		{
			str_pt = strhead_pt;
			
		}
	}
	else  if (*str_pt == '\0')
	{
		str_pt = strhead_pt;
		
	}
	//����str_pt��ȥ���Ա��̩��������⴦�� by cailizhen
	Result = CharOutRect(str_pt, &kregion, scroll_cnt, language);
	if (Result)	//�������һ������
	{
		str_pt +=Result;
		scroll_cnt = 0;
		goto test;
	}
	
/*	if(InvertFlag)
	{
		InvertRegion(&kregion);
	}*/
	scroll_cnt += step;//δ֧�ָ���
	
}
#if 0
uint8 Count_Byte(uint8 *str,uint8 language)   //ͳ������ zxs added 2007.03.26
{
	uint8 n=0;
	uint8 m=0;
	while(*str!=0)
	{
		if((*str >= 0x80)&&(language!=LAN_ID_ENGLISH))
		{
			if(m+16 > DISPLAYWIDTH)
				break;
			*str++;
			*str++;
			n++;
			m=m+16;
		}
		else
		{
			if(m+8>DISPLAYWIDTH)
				break;
			*str++;
			n++;
			m=m+8;
		}
	}
	return n;
}
#endif

