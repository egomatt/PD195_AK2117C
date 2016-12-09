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
* \file     usbdisk.c
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
* \date  2010-3-10
*******************************************************************************/

#include "Udisk.h"

#pragma name(AP_MAIN)

uint8 DisplayIndi,UdiskStatus;
uint8 MenuKeyPress;
uint8 IsCardOut; 
uint8 ChargeCheckCounter;
uint8 gRtcCounter; 
comval_t g_comval; //ȫ�ֱ������ݽṹ 
uint8 HoldDispState;
//res_infor_t res_infor[5];
uint8 Precess_cnt=0;
uint8 ChargeState;
uint8 full_flag = FALSE, charging_flag = TRUE;
uint8 bat_value;
uint8 init_flag=1;
#pragma renamecode(?PR?AP_MAIN)

int main(void)
{
    uint8 para = 0x01;	
	uint8 dispIndibak=0xff;
    uint8 rii=0;
    uint8  cnt=0;
	uint8 sfr_bak;
	sfr_bak = SFR_BANK;
    DisplayIndi=0x00; 	                         
    ChargeCheckCounter = 0;
    gRtcCounter = 0;
	     
	//sKY_ChargeSet(1);    //���          
    //sKY_ChargeGetandSet(); 

	//note:��udisk������������,ע�ⲻҪ�����漰����flashSD���Ĳ���.
	ChargeState = sKY_ChargeSet(0);
//	Lightmodeset(1);

    #if((MACHINE_MODEL==2)||(MACHINE_MODEL==3))
      sfr_bak = SFR_BANK;
	  SFR_BANK = BANK_GPIO;
	  GPIOCOUTEN |= 1;
      GPIOCDAT |= 1;     
      SFR_BANK = sfr_bak;
    #endif
      
	/*AP��ʼ��*/ 	
	if(!UDiskInitAll(para))                     
    {
		//��ǰ�����˳�
		//#if(MACHINE_MODEL==1)
		//UdiskStatus = 0x40;
		//return  UdiskExitAll();
		//#elif((MACHINE_MODEL==2)||(MACHINE_MODEL==3))
        init_flag = 0;
		//#endif
    }       
    while(1)
    {
        ClearWatchDog(); 
		/*deal message */
        MsgManager(DisplayIndi);
                           
        if((IsCardOut == 1) && init_flag)
        {            
            UDiskPullCardOut();
            IsCardOut = 0;
        }

				//����״̬��ִ������ 
        if(init_flag)	
        	UDiskHandle();		
				//��ȡUSB״̬                  
        //0:����  10h:�ϴ�  20h:�´�  30h:����Ҫ���˳�  40h:����Ҫ���˳�  50h:�л����ͼ�����Ӧ�� 
        //60h:��������״̬  70h:����Ҫ���˳�  80h:����Suspend״̬  90h:�˳�Suspend״̬ 
        if(!init_flag)
        	UdiskStatus = 0;
        else
        	UdiskStatus = UDiskGetStatus();   

        if((DisplayIndi==0xff) && ( UdiskStatus !=0x80))
        {   
           //StandbyScreen(1);
           SetContrast(g_comval.DisplayContrast);      
        }

        //#if(MACHINE_MODEL==1)
        //if((UdiskStatus==0x30) || (UdiskStatus==0x40) || (UdiskStatus==0x50) || (UdiskStatus==0x70))
        //{
        //    break;     
        //}
        //#elif((MACHINE_MODEL==2)||(MACHINE_MODEL==3))
        if((UdiskStatus==0x30) || (UdiskStatus==0x40) || /*(UdiskStatus==0x50) ||*/ (UdiskStatus==0x70)|| (UdiskStatus==0x80))
        {
            init_flag = 0;     
        }
        if(UdiskStatus==0x50)
        {
            break;     
        }
        //#endif
        
        sfr_bak=SFR_BANK;
   		SFR_BANK = BANK_PMU;
        if(!((SYSTEM_VOL & 0x80)||(CHG_CTL & 0x80)))
        {
        	return UdiskExitAll();
        }
     	SFR_BANK = sfr_bak;   
        

        switch(UdiskStatus)
        {
            case 0:      //���� 
                if(DisplayIndi != 0x00)
                {   
                    cnt=0;
					Precess_cnt=0;
                    DisplayIndi=0x00;
                    Display(DisplayIndi);    
                }
				/*
				if (ChgMsgFlag)
                {
                       //�����Ϣ����Ҫˢ�µ�ص�����ʾ                            
                    ChgMsgFlag = FALSE;
                    ui_show_battt();
                } 
				*/  
                break;
    
            case 0x10:	 //�ϴ� 
                if(DisplayIndi!=0x01)
                {
                    DisplayIndi=0x01;
                    Display(DisplayIndi);   
                }
                break;
                
            case 0x11://��ʾCommu
            case 0x21://��ʾDownLoad
            case 0x31://��ʾUpLoad
            case 0x41://��ʾSucc
            case 0x61://��ʾSucc���˳� 
                DisplayIndi=UdiskStatus>>4;
                if (UdiskStatus==0x61)
                {
                    DisplayIndi -= 1;       
                }
                DisplayIndi+=0x10;				
                if(dispIndibak != DisplayIndi)    
                {
                    dispIndibak=DisplayIndi;
                    Display(DisplayIndi);
                }
                break;
                
            case 0x51:      //�л���adfu
                //�ؼ��̺�RTC�ж� 
				IE0 = IE0 & 0xf7;  
                DisplayIndi=0x10;
				//��ֻ֤��ʾһ�� 
                if(dispIndibak != DisplayIndi)       
                {
                    dispIndibak=DisplayIndi;
                    Display(DisplayIndi);
                }
                break;
                
            case 0x20:	   //�´� 
               // if(DisplayIndi!=0x02)
                if(cnt++>200)
                {   
                    cnt=0;
					Precess_cnt++;
					if(Precess_cnt==8)
						Precess_cnt=0;
                    DisplayIndi=0x02;
                    Display(DisplayIndi);   
                }
                break;
                
            case 0x80:	  //����suspend
                DisplayIndi=0xff;       
                break;
                
            case 0x90:    //�˳�Suspend״̬
                //StandbyScreen(1);				
                //SetContrast(g_comval.DisplayContrast);      
                break;
                
            default:
                break;
        }    

        if(((MenuKeyPress==1) && (UdiskStatus==0))||(UdiskStatus==0x90))
        {   
            break;       
        }
        else
        {   
            MenuKeyPress = 0;           
        }               
    }/*end of while loop*/  
	
#ifdef  BACKLIGHTCTL 	
	LightOnOff(1);
#endif

	return  UdiskExitAll();              
}
