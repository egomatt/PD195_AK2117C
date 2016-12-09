/*
 ******************************************************************************
 *                               AS211A
 *                            Module: music
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: timer ISR module of music
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xusong    2010-07-15 15:00     1.0             build this file
 ******************************************************************************
 */
#include "ap_music.h"

#pragma name(AP_SUB_INT)

#pragma ROM(large)
void Music_timeint(void)
{
    if (g_ABMinTimeCNT != 0)
    {
        g_ABMinTimeCNT--;
    }
    if ((g_ScrollTimeCNT++) > SCROLLTIME_DEF)
    {
        g_ScrollTimeCNT = 0;
        g_ScrollFlag = TRUE;
    }
}

void Music_2HZint(void)
{
    g_2HZ_CNT++;
    g_2HZFlag = !g_2HZFlag;
}

#pragma ROM(huge)

// para 是音频文件数据地址，长度为512字节
// sector 是当前数据的扇区序号，1扇区=512字节
void data_decrypt(void *para, uint32 sector)
{
	int i;
	uint8 key = sector + 0x5a;
	char *p = (char *)para;
	
	for (i=0; i<512; i++, p++)
	{
		*p = ((*p) ^ key);
	}
}

