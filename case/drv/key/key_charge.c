#include "actos.h"
#include "irq.h"
#include "string.h"
#include "ap_cfg.h"

extern void _nop_(void);
uint8 sKY_ChargeSet(uint8 Setting); //�������/״̬��ȡ
void sKY_ChargeGetandSet(void); //�����
void Delay_500ms(void);
void Delay_10ms(void);


void stop_charge_exit(void);

#define 	BATLOW_VEL		0x44//	//3.0V�����͵�
#define		BATNON_VEL		0x5d//	//3.6V,����޵��
#define 	BATFULL_VEL		0x73//	//4.1V,	�������
#define 	BATFULL_VEL1	0x75//0x77	//4.2V,�������
#define   ChargeCurrent25mA       0x01
#define   ChargeCurrent50mA       0x02
#define   ChargeCurrent100mA      0x03
#define   ChargeCurrent150mA      0x04
#define   ChargeCurrent200mA      0x05
#define   ChargeCurrent250mA      0x06
#define   ChargeCurrent300mA      0x07
#define   ChargeCurrent350mA      0x08
#define   ChargeCurrent400mA      0x09
#define   ChargeCurrent450mA      0x0a
#define   ChargeCurrent500mA      0x0b

#pragma	renamecode(?PR?BAC_CHARGE)

uint8 TheCharge = 0; //����ʶ��0,stop��1��start
uint8 ChargeCurrent = 0; //������ѡ��
uint8 TrickleTimeCounter = 0; //������������30�����
uint8 BatteryFullFlag = 0; //������ʶ
uint8 LowestCurrentCnt = 0;
uint8 CurrentSetFlag = 0;
uint8 UsbUnStick = 0;
/*
 **************************************************************
 *  char    sKY_ChargeSet(char type)          *
 *  input:   type,  0:get the charge status
 *                       1: start charge battery
 *                       2:stop charge battery
 *   output:
 *              char charge status:  0:no charge
 *                                   1:charge state
 2:batteryfull
 **************************************************************
 */
uint8 sKY_ChargeSet(uint8 Setting)
{
    if (Setting == 0)
    {
        //get charge status
        if (TheCharge == 1)
        {
            return 1;
        }
        else
        {
            if (BatteryFullFlag == 1)
            {
                return 2;
            }
            else
            {
                return 0;
            }
        }
    }
    else if (Setting == 1)
    {
        //start charge,set charge flag
        TheCharge = 1;
    }
    else if (Setting == 2)
    {
        //stop charge, clr charge flag
        TheCharge = 0;
    }
    else
    {
        ;//none
    }
    return 0;
}

/*
 **************************************************************************
 *  void    sKY_ChargeGetandSet(char void)          *
 *  input:   NONE
 *  output: NONE
 *  description: This function is used for controlling the charging process
 **************************************************************************
 */
void sKY_ChargeGetandSet(void)
{
    uint8 sfr_bak;
    uint8 tmp1;

    sfr_bak = SFR_BANK;
    Delay_10ms();

    SFR_BANK = BANK_PMU;
	SYSTEM_VOL |= 0x40;	   // ��UVLO��ȥ��ʱ���Ϊ20us��Ϊ�˽�����USB���޷��������⡣
    if((SYSTEM_VOL & 0x80) || (CHG_CTL & 0x80))
    {
        UsbUnStick = 0;
        //usb connect
        if ((TheCharge == 0) || (TheCharge == 2))
        {
            stop_charge_exit();
            goto exit;
        }
        else if (TheCharge == 1)
        {
            //3.����ص�ѹ�Ƿ�С��3.0V,�ж��Ƿ���Ҫ������޵��
            if (BATADC_DATA < BATLOW_VEL)
            {
                //3.1��С��3.0V������������򿪳�����50ma��磬delay 500ms
                CHG_CTL &= ~1;
                CHG_CTL |= 1 << 1;
                CHG_CTL &= ~(1 << 2); //���ó�������50mA
                CHG_CTL |= 1 << 4; //�������,Ӳ��������д1Ϊ����

                SYSTEM_VOL |= 1 << 6; //����41us���ʱ��
                CHG_CTL |= 1 << 5; //ʹ�ܳ��
                Delay_500ms(); //�ӳ�500ms

                //3.1.1����ص�ѹ�Ƿ�С��3.6V
                if (BATADC_DATA < BATNON_VEL)
                {
                    //have battery,wait next check
                    goto exit;
                }
                else
                {
                    //no battery
                    stop_charge_exit();
                    goto exit;
                }
            }
            else
            {
                //no need check battery exist
                //3.2����ص�ѹ����3.0V���ж��Ƿ����趨�û�������
                if (CurrentSetFlag == 1)
                {
                    //3.2.1�����趨�û����������status�Ƿ�Ϊ0
                    if ((CHG_DET & 0x01) == 0)
                    {
                        //3.2.1.1 status=0���жϳ������Ƿ����25ma
                        if ((CHG_CTL & 0x0f) > ChargeCurrent25mA)
                        {
                            //3.2.1.1.1 ����>25mA��˵�������������㣬�𼶼�С50ma������
                            LowestCurrentCnt = 0; //�͵��������0
                            tmp1 = (CHG_CTL & 0x0f) - 1;
                            CHG_CTL = (CHG_CTL & 0xf0) | tmp1; //������50mA

                            //3.2.1.1.1.1 ����ѹ(BATADC)�Ƿ�>4.2V
             BatFullCheck: if (BATADC_DATA > BATFULL_VEL1)
                            {
                                //3.2.1.1.1.1.1 ����4.2V���ж��Ƿ��Ѽӳ�10����
                                if (TrickleTimeCounter < 20) //30s��һ��chargeSetGet
                                {
                                    //3.2.1.1.1.1.1.1 δ�ӳ䣬�ӳ��ʱ���ۼ�1���˳�
                                    TrickleTimeCounter++;		  
                                    goto exit;
                                }
                                else
                                {
                                    //3.2.1.1.1.1.1.2 �Ѽӳ䣬��Ͽ��ڲ�����·������REG[CHG_EN]������ʱ500ms
                                    CHG_CTL &= ~(1 << 5);
                                    Delay_500ms();

                                    //3.2.1.1.1.1.1.2.1 ����ص�ѹ�Ƿ�>4.2V
                                    if (BATADC_DATA > BATFULL_VEL1)
                                    {
                                        //3.2.1.1.1.1.1.2.1.1 �ǣ����ó��������־,ֹͣ��磬��ʼ�������ز������˳�
                                        BatteryFullFlag = 1;
                                        stop_charge_exit();	
                                        goto exit;
                                    }
                                    else
                                    {
                                        //3.2.1.1.1.1.1.2.1.1 ����򿪳���·����ʼ���ӳ䶨ʱ�����˳�
                                        CHG_CTL |= 1 << 5;
                                        TrickleTimeCounter = 0;
                                        goto exit;
                                    }
                                }
                            }
                            else
                            {
                                //��ص�ѹ<4.2V���˳����ȴ��´μ��
                                goto exit;
                            }
                        }
                        else

                        {
                            //3.2.1.1.2	����<25mA���͵��������һ
                            LowestCurrentCnt++;

                            //3.2.1.1.2.1 �жϵ͵�������Ƿ�>3
                            if (LowestCurrentCnt > 3)
                            {
                                //������>3��������ع��䱻�������߸�����������
                                //�ó��������ֹͣ���˳�
                                BatteryFullFlag = 1;
                                stop_charge_exit();
                                goto exit;
                            }
                            else
                            {
                                //������<3
                                //ת��3.2.1.1.1.1������ص�ѹ>4.2V
                                goto BatFullCheck;
                            }
                        }
                    }
                    else
                    {
                        //3.2.1.2 status!=0,��ת3.2.1.1.1.1������ѹ(BATADC)�Ƿ�>4.2V
                        goto BatFullCheck;
                    }
                }
                else
                {
                    //3.2.2��δ�趨�û�����������ص�ѹ�Ƿ�<4.2V
                    if (BATADC_DATA < BATFULL_VEL1)
                    {
                        //�����������100mAʹ�ܳ�磬�ȴ�0.5s
                        CHG_CTL |= 1;
                        CHG_CTL |= 1 << 1;
                        CHG_CTL &= ~(1 << 2);
                        CHG_CTL &= ~(1 << 3);
                        CHG_CTL |= 1 << 4;
                        CHG_CTL |= 1 << 5;

                        Delay_500ms();

                        //�����û����������˳�
                        #if(MACHINE_MODEL==1)
                        ChargeCurrent = ChargeCurrent100mA; //�û�������Ϊ250mA
                        #elif((MACHINE_MODEL == 2)||(MACHINE_MODEL==3))
                        ChargeCurrent = ChargeCurrent50mA; //�û�������Ϊ250mA
                        #endif

                        tmp1 = (CHG_CTL & 0xf0) | 0x0f;
                        CHG_CTL = (ChargeCurrent | 0xf0) & tmp1; //��ǰcurrent�浽CHG_CTL
                        CurrentSetFlag = 1; //�����û��������ñ�ʶ
                        goto exit;
                    }
                    else
                    {
                        //3.2.2.2 ��ѹ>4.2V��˵���������,�ó��������־
                        BatteryFullFlag = 1;
                        stop_charge_exit();
                        goto exit;
                    }
                }
            }
        }
    }
    else
    {
        //no usb cable connect
        //goto exit;
		if(!UsbUnStick)
		{
			stop_charge_exit();
		}
		UsbUnStick = 1;
    }

    exit: SFR_BANK = sfr_bak;
    return;
}

void stop_charge_exit(void)
{
    uint8 sfr_bak;
    sfr_bak = SFR_BANK;

    SFR_BANK = BANK_PMU;
    CHG_CTL &= ~(1 << 5); //ֹͣ���
    TheCharge = 0;
    CurrentSetFlag = 0;
    LowestCurrentCnt = 0;

    SFR_BANK = sfr_bak;
}


void Delay_10ms(void) //24Mhz����ʱ10ms
{
    uint8 i, j;
    for (i = 0; i < 255; i++)
    {
        for (j = 0; j < 255; j++)
        {
            _nop_();
            _nop_();
        }
    }
}

void Delay_500ms(void) //24Mhz����ʱ500ms
{
    uint8 i;
    for (i = 0; i < 50; i++)
    {
        Delay_10ms();
    }
}
