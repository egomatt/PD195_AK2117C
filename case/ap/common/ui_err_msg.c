/*
 *******************************************************************************
 *                ACTOS AP
 *           ap common lib file
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 *  $Id: ui_err_msg.c,v 1.1.1.1 2006/05/22 03:23:59 selina Exp $
 *******************************************************************************
 */

#include "ap_common.h"
//#include "common.h"
#pragma name(COM_ERR_MSG)

/*
 ********************************************************************************
 * Description : ϵͳ������Ϣ
 *
 * Arguments   :
 *    msgID,      ������ϢID
 *    string_id,  ����

 * Returns     :
 *        0
 * Notes       :
 *    ������˳�,û�а����ȴ�5��֮���˳�.
 ********************************************************************************
 */
uint16 ui_err_msg(uint16 msgID)
{	

    ClearScreen(NULL);
	SetAsciiFont(FONT_TYPE_LARGE);
    ResShowMultiString(msgID, 255,16);
    UpdateScreen(NULL);
    ap_sleep(4);   
    return 0;
 
}

/*
 ********************************************************************************
 * Description : ϵͳ������Ϣ
 *
 * Arguments   :
 *    msgID,      ������ϢID
 *    string_id,  ����

 * Returns     :
 *        0
 * Notes       :
 *    �����˻�
 ********************************************************************************
 */
uint16 ui_show_msg(uint16 msgID) //��DELAY
{
  
    uint16 i; //czh
    ClearScreen(NULL);
    ResShowMultiString(msgID, 255, 16);
    UpdateScreen(NULL);
    for (i = 0; i < 6000; i++)
    {   
    }
	
    return 0;

}
