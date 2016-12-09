/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : sdrv_functions.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00    
* Description: support unicode font
********************************************************************************
*/
#include "Drv_S7565.h"
#include "display.h"
#include "string.h"


#pragma renamecode(UI_RCODE_2)




/*
*******************************************************************************
*
** FUNCTION:     SetStrEndPosition
**
** Description:  ������ʾ������ʾ���ֵĽ���λ��
**
**  input
**     StrEnd: ����ַ�������λ�õ����꣬0������������н���λ�ã������������������
**
**  output
**     True: NONE
********************************************************************************
*/
void Sdrv_SetStrEndPosition(uint8 str_endx)
{
    StrEndX = str_endx;
}

//================================================================================
