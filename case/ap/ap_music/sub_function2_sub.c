/*
 ******************************************************************************
 *                               AS211A
 *                            Module: music
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: sub functions 2 of music
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xusong    2010-07-15 15:00     1.0             build this file
 ******************************************************************************
 */
#include "ap_music.h"

#pragma name(AP_FUN2_SUB)
uint8 far tmpbuf[30];

uint8 get_endposition(uint8 *string_pt, uint8 string_len, uint8 type)
{
    uint8 i = 0;
    type = type;
    while (i < (string_len - 1))
    {
        if ((string_pt[i] == 0) && (string_pt[i + 1] == 0))
        {
            string_pt[i] = 0x20;
            string_pt[i + 1] = 0x00;
            break;
        }
        else
        {
            i += 2;
        }
    }
    if (i > (string_len - 1))
    {
        string_pt[i - 2] = 0x20;
        string_pt[i - 1] = 0x00;
        return (i - 2);
    }
    return i;
}

void endchar_add(uint8 end_position)
{

    if (end_position >= 88)
    {
        g_TagInfoBuf[end_position - 2] = 0x20;
        g_TagInfoBuf[end_position - 1] = 0x00;
    }
    else
    {
        end_position += 2;
    }
    g_TagInfoBuf[end_position] = 0x00;
    g_TagInfoBuf[end_position + 1] = 0x00;
}

/*
 ********************************************************************************
 *             void Deal_UniID3Infor(void)
 *
 * Description : 处理unicode的歌曲信息,歌词
 *
 * Arguments   : void
 *
 * Returns     : void
 *
 * Notes       :
 *
 ********************************************************************************
 */
void Deal_UniID3Infor(bool mp3flag, bool longnameflag, bool uni_flag)
{

    uint8 end_pos, end_pos_last;
    uint8 temp_buf[30];
	uint16 temp_code;

	for (end_pos=0; end_pos<90; end_pos++)	   	//ID3中有回车符，转换为空格符 
    {
    	temp_code = g_TagInfoBuf[end_pos];
		if (uni_flag == TRUE)
		{
			temp_code = temp_code + ((uint16)(g_TagInfoBuf[end_pos]) << 8);
		}
        if ((temp_code == 0x0d) || (temp_code == 0x0a))
		{
            g_TagInfoBuf[end_pos] = ' ';
		}
		if (uni_flag == TRUE)
		{
			end_pos++;
		}
    }

    if (mp3flag != 0)
    {
        if ((g_TagInfoBuf[0] != 0xff) || (g_TagInfoBuf[1] != 0xfe))
        {
            end_pos = get_endposition(g_TagInfoBuf, 30, uni_flag);
            memcpy(temp_buf, g_TagInfoBuf, 30);
            memcpy(&g_TagInfoBuf[2], temp_buf, 28);
            g_TagInfoBuf[0] = 0xff;
            g_TagInfoBuf[1] = 0xfe;
            end_pos += 2;
        }
        else
        {
            end_pos = get_endposition(&g_TagInfoBuf[2], 28, uni_flag);
            end_pos += 4;
        }
        memcpy(g_TagInfoBuf + end_pos, &g_TagInfoBuf[32], 28);
        end_pos_last = end_pos;
        end_pos = get_endposition(&g_TagInfoBuf[end_pos_last], 28, uni_flag);
        end_pos += end_pos_last;
        end_pos_last = end_pos + 2;
        memcpy(g_TagInfoBuf + end_pos_last, &g_TagInfoBuf[62], 28);
        end_pos = get_endposition(&g_TagInfoBuf[end_pos_last], 28, uni_flag);
        end_pos += end_pos_last;
        endchar_add(end_pos);
    }
    else
    {

        if (longnameflag != 0)
        {
            end_pos = get_endposition(g_TagInfoBuf, 64, uni_flag);
            end_pos += 4;
            g_TagInfoBuf[end_pos] = 0x00;
            g_TagInfoBuf[end_pos + 1] = 0x00;
            end_pos -= 2;
            while (end_pos >= 2)
            {
                g_TagInfoBuf[end_pos] = g_TagInfoBuf[end_pos - 2];
                end_pos--;
            }
        }
        else
        {
            memcpy(temp_buf, g_TagInfoBuf, 30);
            memcpy(&g_TagInfoBuf[2], temp_buf, 28);
            end_pos = get_endposition(&g_TagInfoBuf[2], 28, uni_flag);
        }

        g_TagInfoBuf[0] = 0xff;
        g_TagInfoBuf[1] = 0xfe;
        if (!longnameflag)
        {
            end_pos += 4;
            memcpy(g_TagInfoBuf + end_pos, &g_TagInfoBuf[30], 30);
            end_pos_last = end_pos;
            end_pos = get_endposition(&g_TagInfoBuf[end_pos_last], 30, uni_flag);
            end_pos += end_pos_last;
            end_pos_last = end_pos + 2;
            memcpy(g_TagInfoBuf + end_pos_last, &g_TagInfoBuf[60], 30);
            end_pos = get_endposition(&g_TagInfoBuf[end_pos_last], 30, uni_flag);
            end_pos += end_pos_last;
            endchar_add(end_pos);
        }

    }
}

void utf8_to_unicode(uint8 *dst, uint8 dst_len, uint8 *src, uint8 src_len)
{
    uint8 i, j, tmp;
    uint16 c;
    *dst++ = 0xff;
    *dst++ = 0xfe;
    j = 0;
    for (i=2; i<dst_len; i+=2)
    {
        tmp = *src;
        if (tmp < 0x80)         // 单字节   0xxxxxxx(00-7F) ==> 0xxxxxxx
        {
            c = *src++;
            j += 1;
        }
        else if (tmp < 0xe0)    // 双字节`  110xxxxx(C0-DF) 10yyyyyy(80-BF) ==> 00000xxx xxyyyyyy
        {
            c = ((uint16)(*src++) & 0x1f) << 6;
            c += ((uint16)(*src++) & 0x3f);
            j += 2;
        }
        else if (tmp < 0xf0)    // 三字节   1110xxxx(E0-EF) 10yyyyyy 10zzzzzz ==> xxxxyyyy yyzzzzzz
        {
            c = ((uint16)(*src++) & 0x0f) << 12;
            c += ((uint16)(*src++) & 0x3f) << 6;
            c += ((uint16)(*src++) & 0x3f);
            j += 3;
        }
        else                    // 四字节或更多 不支持
        {
            c = 0;
        }
        
        if(j > src_len)//源长度超出
        {
            c = 0;
        }
        *dst++ = c & 0xff;
        *dst++ = c >> 8;
        if (c == 0)
        {
            return;
        }
    }
}
void DealTagUtf8(void)
{
    if (g_TagInfoBuf[0] == 3)
    {
        memcpy(tmpbuf, &g_TagInfoBuf[1], 29);
        utf8_to_unicode(&g_TagInfoBuf[0], 30, tmpbuf, 29);
    }
    if (g_TagInfoBuf[30] == 3)
    {
        memcpy(tmpbuf, &g_TagInfoBuf[31], 29);
        utf8_to_unicode(&g_TagInfoBuf[30], 30, tmpbuf, 29);
    }
    if (g_TagInfoBuf[60] == 3)
    {
        memcpy(tmpbuf, &g_TagInfoBuf[61], 29);
        utf8_to_unicode(&g_TagInfoBuf[60], 30, tmpbuf, 29);
    }
}

/*将ID3/Tag信息字符串资源大端转为小段*/
void DealBETagBuffer(void)
{
    uint8 i, tmp;

	if ( (g_TagInfoBuf[1] == 0xff) && (g_TagInfoBuf[0] == 0xfe))
	{
		for (i=0; i<30; i+=2)
		{
			tmp = g_TagInfoBuf[i];
			g_TagInfoBuf[i] = g_TagInfoBuf[i+1];
			g_TagInfoBuf[i+1] = tmp;	
		}	
	}

	if ( (g_TagInfoBuf[31] == 0xff) && (g_TagInfoBuf[30] == 0xfe))
	{
		for (i=30; i<60; i+=2)
		{
			tmp = g_TagInfoBuf[i];
			g_TagInfoBuf[i] = g_TagInfoBuf[i+1];
			g_TagInfoBuf[i+1] = tmp;	
		}	
	}

	if ( (g_TagInfoBuf[61] == 0xff) && (g_TagInfoBuf[60] == 0xfe))
	{
		for (i=60; i<90; i+=2)
		{
			tmp = g_TagInfoBuf[i];
			g_TagInfoBuf[i] = g_TagInfoBuf[i+1];
			g_TagInfoBuf[i+1] = tmp;	
		}	
	}
}

/*MP3 ID3非全部Unicode编码，转换为全部ANSI编码*/
void DealMP3ANSI(void)
{
	if((g_TagInfoBuf[0] == 0xff) && (g_TagInfoBuf[1] == 0xfe))
    {
        memcpy(g_TagInfoBuf,&g_TagInfoBuf[2],28);
        g_TagInfoBuf[28]=0;
        g_TagInfoBuf[29]=0;
        unitochar(g_TagInfoBuf, 30, (uint8)g_comval.langid);
    } 
    if((g_TagInfoBuf[30] == 0xff) && (g_TagInfoBuf[31] == 0xfe))
    {
        memcpy(g_TagInfoBuf+30,&g_TagInfoBuf[32],28);
        g_TagInfoBuf[58]=0;
        g_TagInfoBuf[59]=0;
        unitochar(g_TagInfoBuf+30, 30, (uint8)g_comval.langid);                 
    }
    if((g_TagInfoBuf[60] == 0xff) && (g_TagInfoBuf[61] == 0xfe))     
    {
        memcpy(g_TagInfoBuf+60,&g_TagInfoBuf[62],28);
        g_TagInfoBuf[88]=0;
        g_TagInfoBuf[89]=0;
        unitochar(g_TagInfoBuf+60, 30, (uint8)g_comval.langid);                         
    }
    
    /*在ID3/Tag信息字符串资源中插入空格字符*/
    //strcpy(g_TagInfoBuf, &g_TagInfoBuf[0]);
    //strcat(g_TagInfoBuf, " ");
    //strcat(g_TagInfoBuf, &g_TagInfoBuf[30]);
    //strcat(g_TagInfoBuf, " ");
    //strcat(g_TagInfoBuf, &g_TagInfoBuf[60]);
    //strcat(g_TagInfoBuf, " ");
}
