/*
********************************************************************************
*                       ACTOS
*                  ui dirver show picture
*
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* File   : UI_ClrScreen.c
* By     : Mars
* Version: 1> v1.00     first version     2008/07/30  14:40
* Version: 1> v1.01     reagan            2010-11-10 11:00    
* Description: support unicode font
********************************************************************************
*/
#include "Drv_S7565.h"
#include "display.h"
#include "string.h"



#pragma renamecode(?PR?BAC_CLRS)
////*******************************************************************************/
///*
//** FUNCTION:     ClearScreen
//**
//** Description:  ��ָ������Ļ������Ϊ����ɫ
//**
//**  input
//**        region_t *clrregion :Ҫ������������Ϊ NULL ����ȫ��
//**
//**  output
//**     none
//********************************************************************************/

void Sdrv_ClearScreen(region_t *clrregion)
{
	
	uint16  i,j,k;
	uint16 ImgDataLen;
//	uint8 setcolcmdH,setcolcmdL;
//	uint8 begincol=0;
//	uint8 pageloop=0;
	//�ݴ浱ǰ������ɫ(��Ϊ����ʱ�ǰ�ָ��������ɱ���ɫ)
	//PenColor_bak = dc.PenColor;
	//dc.PenColor = dc.BackgdColor;//�ѻ�����ɫ��Ϊ����ɫ
	//Hdrv_SetDrawMode(DrawModeH_def);    //����ʾģʽ���ó�������ʾģʽ������ֿ���ݣ�
	
	if (clrregion == NULL)
	{
		//����ʾ��������Ϊȫ��
		region.x = 0;
		region.y = 0;
		region.width = Display_Length;
		region.height = Display_Height;
		TextColumn=0;
		TextPage=0;
		ImgDataLen=DisplayBufLen;
		memset(DisplayBuffer,0x00,DisplayBufLen);		
//		begincol = 0x20;
//  	setcolcmdH = ((begincol&0xf0)>>4)|0x10;
//  	setcolcmdL = begincol & 0x0f; 
//  	while(pageloop < 5) //8
//  	{
//    	//set page munber command
//			Write_Command(7-pageloop|0xb0);
//			//set column bits
//    	Write_Command(setcolcmdH);
//   		Write_Command(setcolcmdL);
//			//sent data to lcd
//			Write_Data(DisplayBuffer,Display_Length);
//			pageloop++;
// 	}
	}
	else
	{
		ImgDataLen=(clrregion->width>>3)*(clrregion->height);
		j=clrregion->y>>3;
		k=clrregion->height>>3;
		k+=j;
		//����ǰ�������ϣ������,���Ƚ��������ӳ�䴦��,Ȼ�������buffer�����ӳ����� by cailizhen
		RegionMapping(clrregion, -1);
		for(i=j;i<k;i++)
		{
			memset(&DisplayBuffer[clrregion->x+i*Display_Length],0x00,clrregion->width);
		}
		RegionMapping(clrregion, -1);
//		Hdrv_UpdateScreen(clrregion);
	}

	return;
}

