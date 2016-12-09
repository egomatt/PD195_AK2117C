/********************************************************************************
*                              AS211A
*                            Module: USB MAIN
*                 Copyright(c) 2001-2010 Actions Semiconductor,
*                            All Rights Reserved. 
*
* History:         
*      <author>    <time>           <version >             <desc>
*      wanghaijing 2010-3-10         1.0                build this file 
********************************************************************************/
/*!
* \file     usbdisk_.c
* \brief    
* \author   wanghaijing
* \par      GENERAL DESCRIPTION:
*               ������ļ���������
* \par      EXTERNALIZED FUNCTIONS:
*               �����������õ������ģ��
*					/********************************************************************************
*                              AS211A
*                            Module: USB MAIN
*                 Copyright(c) 2001-2010 Actions Semiconductor,
*                            All Rights Reserved. 
*
* History:         
*      <author>    <time>           <version >             <desc>
*      wanghaijing 2010-3-10         1.0                build this file 
********************************************************************************/
/*!
* \file     usbdisk_enter2exit.c
* \brief    
* \author   wanghaijing
* \par      GENERAL DESCRIPTION:
*               ������ļ���������
* \par      EXTERNALIZED FUNCTIONS:
*               �����������õ������ģ��
*
*      Copyright(c) 2001-2010 Actions Semiconductor, All Rights Reserved.
*
* \version 1.0
* \date  2010-3-27
*******************************************************************************/
#include "Udisk.h"
extern uint8 UdiskStatus;
extern comval_t g_comval; //ȫ�ֱ������ݽṹ 
extern uint8 HoldDispState;

extern void ReadBattPicInRam(void);
//extern void ui_show_batt(uint8 value);
void FlashFrequenceSet(uint8 Frequence);//EMI
void  FlashPadDrvSet(uint8 PadDrv);
#pragma name(AP_ENTEREXIT)
bool UDiskInitAll(int para)
{      
	bool ret;     
//    /*�͵�ģʽ�½��г��*/
//    if(GetBattery() < 0x13)
//    {
//       sKY_ChargeSet(1);    
//       sKY_ChargeGetandSet();    
//    } 

	//��ram��clk���л���mcu clk,����Ӧ���˳���u��,clk���Ե���ʹ������.
	RamToMcu();	

	//��װud����
	res_fp = ResOpen("ui30.res");

	ResShowPic(IM_USB_BG, 0, 0); //��ʾ���л���	

	//���ͼƬ�������ڴ���,������ддflashʱ��ȥƵ����flash��Ӱ��д�ٶ�. add by wanghaijing,2010-5-27
	ReadBattPicInRam();
	
	if (DRV_DetectUD(0x00) == 0xff)
    {
		return 0;
    }
	else
    {
		DRV_ActiveUD(0x00, MODE_FAST); //����ģʽud����	
    }
    
    #ifdef USB_EMI
    //SetPLL(PLL_6MHZ);
    FlashFrequenceSet(2);//EMI //0~3
    FlashPadDrvSet(0);   //EMI
    #endif
    
	//read comval
	VMRead(&g_comval, VM_SYSTEM, sizeof(g_comval)); //��VRAM
	//check if valid
	if (g_comval.magic != MAGIC_COMVAL) //ȡ����ģʽ�趨
	{
		GetDefaultComval(&g_comval);
	}

	//==================================================================================
	//**********************���º������ô����ܸĶ�***************************************
	//==================================================================================
	UDiskSetDiskNum(g_comval.OnlineMode);       //��������ģʽ

	AlwaysShowCardDisk(g_comval.SuppCard);      //�����Ƿ���ʾ���̷�  //reagan modify 2009-1-6 13:39	
								
	//U�̳�ʼ�ջ�,�������:callback����������...  
	UDiskInit(0,g_comval.udisk_setting);
	
	if (sKY_HoldCheck()) //HOLD��ʾ��Ҫд����
	{		
        int i,j;
        for(i=0;i<20;i++)
        {
            if(sKY_HoldCheck() == 0)
            {
               break;
            }
            /*delay*/
            for(j=0;j++;j<1000)
            {
                ClearWatchDog();
            }
        }
        if(i==20) 
        {
		UDiskSetWP('C');
		HoldDispState = 1;
		//ResShowPic(UDLOCK_S, 115, 16);
//		ResShowPic(UDLOCK_S, 65, 49);
		}
	}
			  
    ret = UDiskRun(para); //����USB����
    return ret;   
}
extern void          _nop_     (void);
void power_control(void)
{
    uint8 sfr_bak;
    sfr_bak = SFR_BANK;

 	SFR_BANK = BANK_CMU_RMU;
	MRCR3|=0x08;
	_nop_();
	_nop_();
 	_nop_();
 	_nop_();
	MRCR2|=0x08;
	_nop_();
	_nop_();

	SFR_BANK = BANK_PMU;
	VDD_DCDC_CTL = (VDD_DCDC_CTL & 0xC1)|0x16;
	BDG_CTL|=0x40;
	EFUSE_CTL|=0x40;
	SFR_BANK = BANK_USB;
	USB_PHYCTRL|=0xC0;
	USB_Efuse_Ref|= 0x40;
 	SFR_BANK = BANK_CMU_RMU;
	MRCR1 &= 0xf7;
	MCSR2 &= 0xe0;
	/*spi norbase���˳�u�̺�,���u�̲���ʱDPDM��״̬ʱ,��Ҫ��usbclk*/
	//MCSR3 &= 0xbf;
	MCSR5 &= 0xe7;

    SFR_BANK = sfr_bak;
}

int UdiskExitAll(void)
{
	int ret;
	UDiskExit();
    sKY_ChargeGetandSet(); /*�ٴμ���磬����ʱ����������*/	                            
    ResClose(res_fp); 	 

    DRV_ActiveUD(0x00, MODE_SLOW);

    #ifdef USB_EMI
    //SetPLL(DEFAULT_FREQ);
    #endif
    
	/*
	//��Mode�˳�  
    if(UdiskStatus == 0x00)                       
    {  
        ret = RESULT_MAIN; 
    }
	//�����˳� 
    else 
    */
    if(UdiskStatus == 0x50) 
    {                
        ret = RESULT_UPGRADE;
    }
    else
    {    
	    //����,����  
        //ret = 0;    
        ret = RESULT_STANDBY;  
    }
	power_control();
	return ret;
}


void RamToMcu(void)
{
    uint8 sfr_bak;
    sfr_bak = SFR_BANK;

    SFR_BANK = BANK_CMU_RMU;
    MCSR11 = (uint8) (MCSR11 & 0x04);
    MCSR5 = (uint8) ((MCSR5 & 0xe1) | 0x34); //JRAM3Clk:MCU clk; PCMRAM:MCU clk;
    MCSR6 = (uint8) ((MCSR6 & 0xf0) | 0x0a); //MURAM2:MCU clk; JRAM1Clk:MCU clk;
	MCSR7 = 0x2a; //B1 Clk:MCU clk; MURAM1HClk:MCU clk; JRAM2Clk:MCU clk;
    MCSR8 = (uint8) ((MCSR8 & 0x03) | 0x48); //MURAM1MClk:MCU clk; MURAM1LClk:MCU clk;
	MCSR9 = 0x4a; //URAMClk:MCU clk; B2 Clk:MCU clk; DRAM251BClk:MCU clk; 
	MCSR11 = MCSR11 | 0xfb;

    SFR_BANK = sfr_bak;

}

#ifdef USB_EMI       
//Frequence:flashƵ��
//"0":2612/7652 KB
//"1":2296/5632 KB
//"2":1879/3593 KB
//"3":1361/2152 KB
void FlashFrequenceSet(uint8 Frequence)					//EMI ADD
{
    uint8 pll,sfr_bak;
    if(Frequence <= 16)
    {
        sfr_bak =  SFR_BANK;
        SFR_BANK = BANK_CMU_RMU;
        pll = MCSR1;
        pll &= 0xfd;
        pll |= 0x02;
        MCSR1 = pll;
        SFR_BANK = sfr_bak;
        uprintf("Frequence: %x\n",Frequence);
        sfr_bak =  SFR_BANK;
        SFR_BANK = BANK_CMU_RMU;
        pll = NANDCLKCTL;                   //Ĭ�� bit3=bit0=1��bit3������0��
        pll &= 0xf9;
        if(Frequence > 3) Frequence = 3;    //��Ƶֵ
        pll |= (Frequence<<1);
        //pll &= 0xfe;                      //0:xtal 1:pll, ʹ��xtal��pll��Ž�һ��
        NANDCLKCTL = pll;
        SFR_BANK = sfr_bak;
    }
    else
    {
        SetPLL(Frequence/2);
        sfr_bak =  SFR_BANK;
        SFR_BANK = BANK_CMU_RMU;
        pll = MCUPLL_CTL;
        SFR_BANK = sfr_bak;
        pll >>= 1;
        pll = pll & 0x1f;
        pll <<= 2;
        uprintf("pll %x\n",pll);

        sfr_bak =  SFR_BANK;
        SFR_BANK = BANK_CMU_RMU;
        pll = NANDCLKCTL;
        pll &= 0xf3;
        NANDCLKCTL = pll;
        SFR_BANK = sfr_bak;
    }

}

//PadDrv:(0,1,2) ��������������0��ͣ�2���
void  FlashPadDrvSet(uint8 PadDrv)					//EMI ADD
{
    uint8 padrv0,padrv1;
    uint8 sfr_bak = SFR_BANK;

    SFR_BANK = 6;

    padrv0 = PADDRV0;
    padrv1 = PADDRV1;
    padrv0 &= 0xc8;
    padrv1 &= 0xc3;

    if(PadDrv == 0)
    {
        padrv0 |= 0;
        padrv1 |= 0;
    }
    else if (PadDrv == 1)
    {
        padrv0 |= 0x13;
        padrv1 |= 0x14;
    }
    else
    {
        padrv0 |= 0x25;
        padrv1 |= 0x28;
    }
    PADDRV0 = padrv0;
    PADDRV1 = padrv1;
    SFR_BANK = sfr_bak;
}
#endif
