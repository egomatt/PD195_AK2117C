/********************************************************************************************
 Airoha Technology FM module driver(I2C DRIVER)  for ATJ2097
 last update 2007/11/02
 Version:V0.5
 ********************************************************************************************/
#include "actos.h" 
#include <ap_cfg.h>
#include <ap_common.h>

#define SAMPLE_TIME 0x000001	//G-sensor
#define HPF_RC      0x00000F
#define LPF_RC      0x000001

#define SENS_WALK   0x000800		// Memsic Referece:  0.1G
#define SENS_PEDO_UP	100     //100
#define SENS_PEDO_DOWN	100     //100    //0.1G

#define SENS_JOGG	0x000A00		// Memsic Referece:  1G
#define SENS_RUNN	0x000C00		// Memsic Referece:  2G
#define SENS_JMAX	0x000000		//
#define SENS_RMAX	0x000000		//
#define SENS_WMAX	0x000000		//

#define STRI_WALK	0x000050	
#define STRI_JOGG	0x000080	
#define STRI_RUNN	0x0000A0

#pragma name(PEDO_FUNC)

//uint8 gfGSStart=0;     //�Ʋ���ʼ����ͣ��־			
int32 stc_iAcce1_LPFcount1=40832;
int32 stc_iAcce0_LPFcount1=40832;
int32 stc_iAcce1_HPFcount1=32866;
int32 stc_iAcce0_HPFcount1=32866;
int32 stc_iAcceFilter1=2048;          //�����˲��漰������ʼֵ
int32 stc_iG_CountUP=0x800;
int32 stc_iG_CountDOWN=0x800;
uint8 stc_bStatUp=0; 
uint8 stc_bStatDown=0;         //�Ʋ�������ʼ��
uint16 step_gap;

int32 AccLPF(int32 iX0, int32 iY1)
{
	iY1 = iY1 * LPF_RC;					//RC = 0x000003;
	iY1+=((iX0*SAMPLE_TIME)+(LPF_RC+SAMPLE_TIME)/2);
	iY1/=(LPF_RC+SAMPLE_TIME);
	return iY1;
}

int32 AccHPF(int32 iX0, int32 iX1, int32 iY1)
{
	iY1+=(iX0-iX1);
	iY1*=HPF_RC;
	iY1+=0x008000+(HPF_RC+SAMPLE_TIME)/2;					//RC = 0x000003;
	iY1/=(HPF_RC+SAMPLE_TIME);							//	0x000004;		T+RC   T=Timer*100,  
	return iY1;
}

uint32 sqrt_16(uint32 M)  
{  
    uint16 N, i;  
    uint32 tmp, ttp;   // �����ѭ������  
    if (M == 0)               // �����������������ҲΪ0  
        return 0;  

    N = 0;  

    tmp = (M >> 30);          // ��ȡ���λ��B[m-1]  
    M <<= 2;  
    if (tmp > 1)              // ���λΪ1  
    {  
        N ++;                 // �����ǰλΪ1������ΪĬ�ϵ�0  
        tmp -= N;  
    }  

    for (i=15; i>0; i--)      // ��ʣ���15λ  
    {  
        N <<= 1;              // ����һλ  

        tmp <<= 2;  
        tmp += (M >> 30);     // ����  

        ttp = N;  
        ttp = (ttp<<1)+1;  

        M <<= 2;  
        if (tmp >= ttp)       // �������  
        {  
            tmp -= ttp;  
            N ++;  
        }  

    }  

    return N;  
}


void   StepLoops(void)
{
    uint16 gap,temp;
    int32 iAcce_Sample;
    #if (PETOMETER_METHOD == 2)
    int32 iG_CountPEAK,iAcce1_LPFcount0,iAcce0_LPFcount0,iAcce1_HPFcount0,iAcce0_HPFcount0,iAcceFilter0;  
    #endif
	//=========calculate=======masda20110111=====================
    int32 XDataTemp,YDataTemp,ZDataTemp;
    int32 GDataTemp;
	//=============================end===========================
    //GS_XYRead();           //��G-sensor  X��Y���ֵ
    sKY_SetGsensor(5,0,0,Pedo_Swim_flag);
    XDataTemp = sKY_BankGsensorGetADC(1,Pedo_Swim_flag);
    YDataTemp = sKY_BankGsensorGetADC(2,Pedo_Swim_flag);
    ZDataTemp = sKY_BankGsensorGetADC(3,Pedo_Swim_flag);
    sKY_SetGsensor(3,0,0,Pedo_Swim_flag);
    //==============calculate======masda20110111=======================================
    XDataTemp-=2048;         //2048Ϊ����1��������ֵ    //�����������ֵ(�޷�����ֵ)
    YDataTemp-=2048;
    ZDataTemp-=2048;
    GDataTemp=(uint32)((XDataTemp*XDataTemp)+(YDataTemp*YDataTemp)+(ZDataTemp*ZDataTemp));
    iAcce_Sample=(uint32)(sqrt_16(GDataTemp));	   //sqrt_16Ϊ��ƽ������
    //iAcce_Sample = YDataTemp;
    //Pedo_total_step = iAcce_Sample;
    //if((XDataTemp+YDataTemp+ZDataTemp) < 0) iAcce_Sample = -iAcce_Sample; 
    //=================================end=============================================
    
  #if (PETOMETER_METHOD == 1)
    //����2048����㣬����ʵ�ʵõ���ֵҪ-2048��
	//��ADC=sqrt((xadc-2048)*(xadc-2048)+(yadc-2048)*(yadc-2048))	/*sqrtΪ��ƽ����*/
	//�����ٿ�ƽ������Ҫ���Ӵ��룬�ֲ�����ƽ������ֱ����(��ADC*��ADC)���Ƚ�

	/* һЩ��������:
	��ֹ	Xadc	Yadc	X*X+Y*Y	����
	
	����1	2511	2027	214810	463.4759972
	����2	2361	1703	216994	465.8261478
	����3	2037	1565	233410	483.1252426
	����4	1715	1690	239053	488.9304654
	����5	1591	2003	210874	459.2101915
	����6	1765	2416	215513	464.2337773
	����7	2088	2512	216896	465.7209465
	����8	2361	2401	222578	471.7817292
	˵��ƽʱ��ֹ״̬��(��ADC)Ϊ460���ң���(��ADC*��ADC)Ϊ211600��
	��ѡ(��ADC)�Ƚ�ֵΪ550����(��ADC*��ADC)Ϊ302500��

	
	ģ�����4������:
	�˶�	Xadc	Yadc	X*X+Y*Y	����
	
	����1 	2292	1170	830420	911.2738337
	����2	1619	527		2497482	1580.342368
	����3	1756	3000	991568	995.775075
	����4	1625	2844	812545	901.41278
	˵��ƽʱ�˶�״̬��(��ADC)Ϊ����900���ң�
	��ѡ(��ADC)�Ƚ�ֵΪ700����(��ADC*��ADC)Ϊ490000��


	1. Length����һ����ֵ������ �CThreshold < Length < Threshold��0.1g
����2. ���������ȵ��ʱ����������Timespan ��200~2000���룩֮��

	*/

    #define g_sensor_max        ((uint32)(1024+100))   // 1.1g
    #define g_sensor_min        ((uint32)(1024-100))   // 0.9g
    #define g_senser_debounce   1

    if(gsensor_posflag==0)			        //֮ǰ״̬Ϊ��ֵ
	{
		if	(iAcce_Sample < g_sensor_min)		//�ж��Ƿ�Сֵ	
		{
			g_debounce++;
			if(g_debounce>=g_senser_debounce)
			{
				gsensor_posflag=1;		    //�ǵĻ��ͽ�֮ǰ״̬��ΪСֵ�����c
				g_debounce=0;
			}	
		}		
		else
		{
			g_debounce=0;
		}
	}
	else							        //֮ǰ״̬ΪСֵ
	{
		if	(iAcce_Sample > g_sensor_max)		//�ж��Ƿ񵽴�ֵ
		{
			g_debounce++;
			if(g_debounce>=g_senser_debounce)
			{
				gsensor_posflag=0;		    //�ǵĻ��ͽ�֮ǰ״̬��Ϊ��ֵ�����c
				g_debounce=0;

				gap = sKY_BankGsensorGetADC(5,Pedo_Swim_flag);
                temp = gap - step_gap;
                if ((temp >= 6) && (temp <= 67))      //200ms~2000ms
                {
                    if(Pedo_Swim_flag)
                        Swim_total_step++; //������1   
                    else
                        Pedo_total_step++; //������1   
                    sKY_SetGsensor(4,0,0,Pedo_Swim_flag);
                    if(Pedo_Swim_flag)
                        sKY_SetGsensor(6,Swim_total_step,0,Pedo_Swim_flag);
                    else
                        sKY_SetGsensor(6,Pedo_total_step,0,Pedo_Swim_flag);
                    //sKY_SetGsensor(7,0,Pedo_total_sec);
                    gap = 0;
                }
                step_gap = gap;
			}	
		}	
		else
		{
			g_debounce=0;
		}
	}

  #else
  
    iAcce_Sample+=2048; 
	iAcce_Sample*=0x10;
    iAcce1_LPFcount0=AccLPF(iAcce_Sample,stc_iAcce1_LPFcount1);
    iAcce0_LPFcount0=AccLPF(iAcce1_LPFcount0,stc_iAcce0_LPFcount1);   
    iAcce1_HPFcount0=AccHPF(iAcce0_LPFcount0,stc_iAcce0_LPFcount1,stc_iAcce1_HPFcount1);
    iAcce0_HPFcount0=AccHPF(iAcce1_HPFcount0,stc_iAcce1_HPFcount1,stc_iAcce0_HPFcount1);   
    iAcceFilter0=iAcce0_HPFcount0/0x10;         //�˲��������iAcceFilter0��iAcce_Sample��С�޹أ�iAcce_Sample��̬��iAcceFilter0Ϊ2048��iAcce_Sample����ʱiAcceFilter0���󣬷�֮��С
		                                        //��iAcceFilter0��Ӧ����iAcce_Sample�ı仯

    if ((iAcceFilter0>0x000800) && (iAcceFilter0>stc_iG_CountUP))
		stc_iG_CountUP=iAcceFilter0;
    if ((iAcceFilter0<0x000800) && (iAcceFilter0<stc_iG_CountDOWN))
		stc_iG_CountDOWN=iAcceFilter0;
    if ((iAcceFilter0>=0x000800) && (stc_iAcceFilter1<0x000800))
    {
        stc_iG_CountDOWN = 0x000800 - stc_iG_CountDOWN;
        iG_CountPEAK = stc_iG_CountDOWN;
        stc_iG_CountDOWN = 0x000800;
        if(iG_CountPEAK > SENS_PEDO_UP)      //SENS_PEDOΪ�Ʋ������ȣ���С�ɵ�
            stc_bStatUp = 1;
	   	else
	   	    stc_bStatUp = 0;
    }
    if ((iAcceFilter0<=0x000800) && (stc_iAcceFilter1>0x000800))
    {
        stc_iG_CountUP=stc_iG_CountUP-0x000800;
        iG_CountPEAK=stc_iG_CountUP;
        stc_iG_CountUP=0x000800;
        if (iG_CountPEAK > SENS_PEDO_DOWN)     
			stc_bStatDown=1;
		else
			stc_bStatDown=0;
    }
    
    //if ( stc_bStatUp )
    //{
    //  	test_up++;
    //}
    //if ( stc_bStatDown )
    //{
    //  	test_down++;
    //}

    if ( stc_bStatUp && stc_bStatDown )      //stc_bStatUp �� stc_bStatDown�ֱ��������һ����������������½�
    {
       stc_bStatUp = 0;
       stc_bStatDown = 0;
       gap = sKY_BankGsensorGetADC(5,Pedo_Swim_flag);
       temp = gap - step_gap;
       if ((temp >= 6) && (temp <= 67))      //200ms~2000ms
       {
            if(Pedo_Swim_flag)
                Swim_total_step++; //������1   
            else
                Pedo_total_step++; //������1   
            sKY_SetGsensor(4,0,0,Pedo_Swim_flag);
            if(Pedo_Swim_flag)
                sKY_SetGsensor(6,Swim_total_step,0,Pedo_Swim_flag);
            else
                sKY_SetGsensor(6,Pedo_total_step,0,Pedo_Swim_flag);
            //sKY_SetGsensor(7,0,Pedo_total_sec);
            gap = 0;
       }
       step_gap = gap;
    }
    
    stc_iAcce0_LPFcount1=iAcce0_LPFcount0;
    stc_iAcce1_LPFcount1=iAcce1_LPFcount0;
    stc_iAcce0_HPFcount1=iAcce0_HPFcount0;
    stc_iAcce1_HPFcount1=iAcce1_HPFcount0;
    stc_iAcceFilter1=iAcceFilter0;			       //�洢�˲��漰����ֵ

  #endif
}


void Count_Step(uint8 mode)
{
    uint16 workflag;
    workflag = sKY_BankGsensorGetADC(4,Pedo_Swim_flag);
	if(mode && workflag)
	{
	    //SetPLL(PLL_80MHZ);
		StepLoops();
		//SetPLL(DEFAULT_FREQ);
    }
}

