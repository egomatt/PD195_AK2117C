#include "actos.h"
#include "key.h"
#include "string.h"

#define NO_KEY 0xff
#pragma renamecode(?PR?KY_COM)

#define GPIOA_BITS  0x1b
#define GPIOA_MASK  0xe4
#define GPIOE_BITS  0xe1
#define GPIOE_MASK  0x1e

extern void sKY_CheckCardStatus(void);

uint8 newkey = NO_KEY;
uint8 oldkey = NO_KEY;
uint8 tmpkey = NO_KEY;
uint8 tmp_count, hold_count;
uint8 s_timer;
uint8 wait_keyup = 0;
uint8 WPCardStatue = 0;
uint8 cardpowerflag = 0;
uint8 cardoutflag = 0;
uint8 batDetectCnt = 0;
//bool HOLD_FLAG=FALSE;
uint8 earInFlag = 0;
uint8 earOutFlag = 1;
uint8 const key_map[16];

uint8 gsensor_type=0;
uint32 step_counter=0;		//20110524	���˶�����
uint32 step_second=0;
uint32 step_counter_swim=0;		//20110524	���˶�����
uint32 step_second_swim=0;
uint16 gsensor_xadc1=0;
uint16 gsensor_yadc1=0;
uint16 gsensor_zadc1=0;
uint8 gsensor_period=0;		//20110524 ɨ��Gsensor��ʱ����������ÿ20MS����һ��
uint8 gsensor_workflag=0;	//20110524	1:ɨ��	0:��ɨ��	
uint8 gsensor_workflag_swim=0;
uint16 gsensor_gap=0;
uint8 gsensor_got=0;


uint8 usb_plugin;
uint8 usb_unstickcount;
uint8 usb_stickcount;


extern uint8 earphoneinflag;
extern uint8 sysverflag;


void key_scan(void)
{
    uint8 sfr_bak;
    uint8 key_val, adcdat;
    //static uint8 cnt = 0;

	#if (PETOMETER_USE)
	if(gsensor_workflag	|| gsensor_workflag_swim)			//�Ƿ�Ҫɨ��
	{
	    gsensor_gap ++;
		gsensor_period++;	
		if (gsensor_period >= 1)			//30msɨ��һ�Σ�	ɨ������(Unit:*30ms) 
		{
			//sKY_ReadGSensor();
			gsensor_period=0;
			gsensor_got = 1;
		}	
	}
    #endif

    //if((++cnt) % 2)
    //{
    //    return;
    //}
    

    sfr_bak = (uint8) SFR_BANK;

    //demoֱ����CLD���л�
    /*SFR_BANK = BANK_GPIO;
    if (earphoneinflag != 0) //����cld�Զ��л�
    {
        if ((GPIOADAT & 0x01) == 1) //�޶������룬�ߵ�ƽ
        {
            if (!earOutFlag) //�״ζ����γ����
            {
                EnableCLD();
                earOutFlag = 1;
            }
            earInFlag = 0;
        }

        if ((GPIOADAT & 0x01) == 0) //�ж������룬�͵�ƽ
        {
            if (!earInFlag) //�״ζ���������
            {
                DisableCLD();
                earInFlag = 1;
            }
            earOutFlag = 0;
        }
    }
    else
    {
        earInFlag = 0;
        earOutFlag = 0;
    }*/

#if 0 //20110424	��Ϊ�����������볤�������ο��ļ����
    sKY_CheckCardStatus();
#endif

    if (!sysverflag)
    {
        BAPI_sKY_CheckUsbStatus();
    }



    SFR_BANK = BANK_PMU;
    adcdat = LRADC1_DATA & 0x3f;

    if (adcdat >= 56) //white110121#1 for hold adc 56
    {
			//if(!HOLD_FLAG)  //SC903
    	  	//{
    	  	//  HOLD_FLAG=TRUE;
    	  	//  PutSysMsg(Msg_KeyHold);//������Ϣ
    	  	//}
        	key_val = NO_KEY;
    } 
    /*
    else if(adcdat > 44)
    {
    	 if(HOLD_FLAG)
    	 {
    	   PutSysMsg(Msg_KeyUnHold);//����
    	   HOLD_FLAG=FALSE;
    	 }
    	 else
    	    key_val = NO_KEY;	    	 
    }
    */
	#if(MACHINE_MODEL==1)
	else if ((adcdat >=0) && (adcdat <= 5)) //0
	{
        key_val = 2;
	}
    else if ((adcdat>=5) && (adcdat <= 15)) //14
	{
        key_val = 3;
    }
	else if ((adcdat>15) && (adcdat <= 25)) //27
    {
	    	key_val = 4;
    }
	else if ((adcdat>25) && (adcdat <= 35)) //40
    {
        key_val = 5;
    } 
	#else
    //else if ((adcdat >=0) && (adcdat <= 5)) //0
	//{
    //    key_val = 2;
	//}
    else if ((adcdat>=5) && (adcdat <= 15)) //14
	{
		#if(MACHINE_MODEL==3)
		key_val = 5;
		#else
        key_val = 6;
		#endif
    }
	#if(MACHINE_MODEL==3)
	else if ((adcdat>15) && (adcdat <= 30)) //27
    {
	    key_val = 3;
    }
	else if ((adcdat>30) && (adcdat <= 44)) //40
    {
        key_val = 2;
    } 
	#endif
	#endif
//	else if ((adcdat > 44) && (adcdat <= 48)) //47
//    {
//        key_val = 6;
//    }
    else
    {
        key_val = NO_KEY; //δɨ����а�������
    }
   
    //����play������
    if (PRESS_DISPLAY & 0xc0) //bit6��7��Ϊ0������/�̰�play��ͳһ��play��ת��APȥ����
    {
        key_val = 1;
        /*
        if(HOLD_FLAG)
    	  {
    	   		PutSysMsg(Msg_KeyHold);//����
            PRESS_DISPLAY = (uint8) (PRESS_DISPLAY | 0xc0);//���PRESS_DIAPLAY  bit7��bit6
    	  }
    	  else */
    	  {
        		PutSysMsg(key_map[key_val]);
      	}
      	
        SFR_BANK = sfr_bak;
        return;
    }
    
	#if(MACHINE_MODEL==1)
	
	//����Menu������
    sfr_bak = SFR_BANK;
	SFR_BANK = BANK_GPIO;
	if ((GPIOCDAT & 0x01) == 0) //gpioc0    
	    key_val = 6;
	SFR_BANK = sfr_bak; 
	
	#else
	
    ///////////////////////////////////////////////////////////////
    //Next,Menu������
	SFR_BANK = BANK_GPIO;
	//PADDRV1 |= 0x80;
	//GPIOCDAT |= 0x03;
	//GPIOCOUTEN &= ~0x03;
	GPIOCINEN |= 0x03;
	if ((GPIOCDAT & 0x02) == 0) //gpioc0 
		#if(MACHINE_MODEL==3)
	    key_val = 6;
		#else
		key_val = 5;
		#endif
	else if ((GPIOCDAT & 0x01) == 0)
	    key_val = 4;
	GPIOCINEN &= ~0x03;
	GPIOCOUTEN &= ~0x03;
    SFR_BANK = sfr_bak; 
    ////////////////////////////////////////////////////////////////
	
    #endif
	
    if (key_val != oldkey) //�������ɨ��ֵ�������ϴΰ���ֵ
    {
        if (key_val == tmpkey) //������μ�ֵ����ǰ��ɨ��ֵ
        {
            tmp_count++; //�ۼ�ɨ�����

            if (tmp_count > 3)
            {
                tmp_count = 3;
            }

            if (tmp_count == 3) //ɨ�����Σ���ͬһ��ֵ��ȷ���а�������
            {
                if (key_val != NO_KEY) //�������ɨ�貶���ֵ��Ϊ��
                {
                    oldkey = tmpkey;
                    wait_keyup = 1;
        								//if(HOLD_FLAG)
										//{
										//	 PutSysMsg(Msg_KeyHold);
										//}
										//else
										{                   		
                    		PutSysMsg(key_map[oldkey]);
                  	}
                }
                else //���μ�ֵΪNO_KEY
                {
                    hold_count = 0;
                    oldkey = NO_KEY;
                    tmpkey = NO_KEY;

                    if (wait_keyup) //�ȴ�KEYUP
                    {
                        wait_keyup = 0;
                        tmp_count = 0; //��ɨ�����
                        PutSysMsg(Msg_KeyShortUp); //��keyup��Ϣ
                    }
                }
            }
        }
        else //�������ɨ��ֵ������ǰ��ɨ��ֵ�������ü�ֵ�����¼���
        {
            tmpkey = key_val;
            if (!wait_keyup)
            {
                tmp_count = 0;
            }
        }
    }
    else //������μ�ֵ�����ϴΰ���ֵ,�ۼ�16��(320ms)����һ�ΰ�����Ϣ
    {
        if (key_val != NO_KEY) //
        {
            hold_count++;

            if (hold_count == 16)
            {
                hold_count = 0;
        					//if(HOLD_FLAG)       		 
                   //PutSysMsg(Msg_KeyHold);//��������           		   
                //else                                   
                PutSysMsg(key_map[oldkey]); //����ֵ��Ϣ
            }
        }
        else
        {
            tmpkey = key_val;
            hold_count = 0;
            if (!wait_keyup)
            {
                tmp_count = 0;
            }
        }
    }

    SFR_BANK = sfr_bak;
}


//cts isr, send	2hz msg
void ctc_isr(void)
{
    //send 2-hz msg
    uint8 sfr_bak;

    PutSysMsg(Msg_HalfSecond); //send 2-hz msg
    sfr_bak = SFR_BANK;
    SFR_BANK = BANK_RTC;

    CTCCtl |= 1 << 7; //enable ctc
    SFR_BANK = sfr_bak;
}


void cld_overflow_isr(void)
{
    uint8 sfr_bak;

    sfr_bak = SFR_BANK;
    SFR_BANK = BANK_AUIP;

    if ((CLD_CTL1 >> 2) & 0x01)
    {
        //��·
        DisableCLD();
    }

    if ((CLD_CTL1 >> 3) & 0x01)
    {
        //����
        DisableCLD();
    }

    SFR_BANK = sfr_bak;
}

