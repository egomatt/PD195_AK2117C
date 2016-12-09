/*
 *******************************************************************************
 *                ACTOS AP
 *           ap common lib file
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 *  $Id: ui_progress.c,v 1.1.1.1 2006/05/22 03:23:59 selina Exp $
 *******************************************************************************
 */

#include "ap_common.h"
#pragma name(COM_UI_PROGRESS)

/*
 ********************************************************************************
 * Description : �������ؼ�
 *
 * Arguments   : progress, �������ṹ
 *
 * Returns     : 0
 *
 * Notes       : ����ؼ�û���Լ�����Ϣѭ��
 *
 ********************************************************************************
 */
uint16 ui_show_progress(const progress_t *progress)
{
	uint16 start, end;
	//	char buf[5];	//ע����ܺ�����
	//	char key;
	const region_t region =
	{ 0, 32, SCR_WIDTH, 16 }; //�ؼ���������
	BOOL need_draw = TRUE;

	//ASSERT(progress->value <= progress->max);
	//ASSERT(progress->value >= progress->min);

	while (1)
	{
		if (need_draw)
		{
			//clear screen
			ClearScreen(&region);

			//show pic
//			ResShowPic(PBAR, POSITION_PROGRESSBAR, 32);
			start = POSITION_PROGRESSBAR + 1;
			end = (POSITION_PROGRESSBAR - 1) + PROGRESS_BAR_WIDTH * (progress->value - progress->min) / (progress->max
					- progress->min);
			//ASSERT(end < SCR_WIDTH);
			//�ֶ���ʾ����
			while (start + PROGRESS_BG_WIDTH <= end)
			{
//				ResShowPic(PBKGRD, start, 32);
				start += PROGRESS_BG_WIDTH;
			}



			//check if auto_update
			if (!ui_auto_update)
				UpdateScreen(NULL);

			need_draw = FALSE;
		}//need_draw


		//direct return
		return 0;

	}//while(1)
	return 0;
}
