/********************************************************************************************
 Airoha Technology FM module driver(I2C DRIVER)  for ATJ2097
 last update 2007/11/02
 Version:V0.5
 ********************************************************************************************/
#include "ap_radio.h"

#pragma name(FM_CTRL_D2)

extern uint8 earphoneinflag;
extern void I2C_Init(void);
//extern void I2C_Exit(void);
//extern void FM_Wait50ms(void);
extern void I2C_Enable(void);
extern void I2C_Disable(void);
extern uint8 QND_ReadReg(uint8 adr);
extern uint8 QND_WriteReg(uint8 adr, uint8 value);
extern void QNF_SetRegBit(uint8 reg, uint8 bitMask, uint8 data_val);
extern void SetMfpToNand(void);
extern void FM_Wait1ms(uint16 time);



uint8 QN_ChipInitialization_2(void)
{

	QND_WriteReg(0x00, 0x81); 
    FM_Wait1ms(10);
    QNF_SetRegBit(0x01,0x80,0x80);
    //Following is where change the input clock frequency.
    QND_WriteReg(0x15, 0xDC);
    QND_WriteReg(0x16, 0x02);
    QND_WriteReg(0x17, 0x54);    
    FM_Wait1ms(10);
    QNF_SetMute(1);
    QND_WriteReg(0x54, 0x47);//mod pll setting
    QND_WriteReg(0x19, 0x40);//AGC setting
    QND_WriteReg(0x2d, 0xD6);//notch filter threshold adjusting
    QND_WriteReg(0x43, 0x10);//notch filter threshold enable    
    QND_WriteReg(0x00, 0x51);
    QND_WriteReg(0x00, 0x21); // chip calibration
    QNF_SetRegBit(0x34, 0x7f, 12);
    QNF_SetRegBit(0x35, 0x7f, 51);
    QNF_SetRegBit(0x36, 0x7f, 18);
    QND_WriteReg(0x38, 0x03);
    QND_WriteReg(0x39, 0x09);
    FM_Wait1ms(50);
    return 1;
}

void QNF_SetMute_2(BYTE On)	
{
	if(On)
	{
        QNF_SetRegBit(0x4C, 0x0B, 0x0B);
	}
	else
	{
        //FM_Wait1ms(200);
        QNF_SetRegBit(0x4C, 0x0B, 0x00);
	}
}

void QNF_ConfigScan(uint16 start, uint16 stop, uint8 step) 
{
    // calculate ch para
    uint8 tStep = 0;
    uint8 tS;
    uint16 fStart;
    uint16 fStop;
    fStart = FREQ2CHREG(start);
    fStop = FREQ2CHREG(stop);
        // set to reg: CH_START
    tS = (uint8) fStart;
    QND_WriteReg(0x08, tS);
    tStep |= ((uint8) (fStart >> 6) & 0x0c);
    // set to reg: CH_STOP
    tS = (uint8) fStop;
    QND_WriteReg(0x09, tS);
    tStep |= ((uint8) (fStop >> 4) & 0x30);
    // set to reg: CH_STEP
    tStep |= step << 6;
    QND_WriteReg(0x0a, tStep);
}


/***********************************************************************
UINT16 QND_RXValidCH(UINT16 freq, UINT8 db);
***********************************************************************
Description: to validate a ch (frequency)(if it's a valid channel)
Freq: specific channel frequency, unit: 10Khz
  eg: 108.00MHz will be set to 10800.
Step:  
  FM:
  QND_FMSTEP_100KHZ: set leap step to 100kHz
  QND_FMSTEP_200KHZ: set leap step to 200kHz
  QND_FMSTEP_50KHZ:  set leap step to 50kHz
Return Value:
  0: not a valid channel
  other: a valid channel at this frequency
***********************************************************************/
uint16 QND_RXValidCH(uint16 freq, uint8 step) 
{
    uint8 regValue;
    uint8 timeOut;
    uint8 loop;
    //QNF_ConfigScan(freq, freq, step);
    freq = freq;
    step = step;
    QNF_SetRegBit(0x00,0x03,0x02);//channel scan mode,channel frequency is decided by internal CCA
    timeOut = 0;
    loop = 0;
    do
    {
        regValue = QND_ReadReg(0x00);
        timeOut++;
    } while((regValue & 0x02) && timeOut<100);//when seeking a channel or time out,be quited the loop
    for(loop; loop < 10 ; loop++)
    {
        regValue |= QND_ReadReg(0x04)& 0x08;//reading the rxcca_fail flag of RXCCA status 
    }
    if(regValue & 0x08)
    {               
        return 0;
    }
    else
    {
        return 1;  
    }
}

uint16 QNF_GetCh_2() 
{
	uint8 tCh;
    uint8  tStep; 
    uint16 ch = 0;
    // set to reg: CH_STEP
    tStep = QND_ReadReg(0x0a);
    tStep &= 0x03;
    ch  =  tStep ;
    tCh= QND_ReadReg(0x07);    
    ch = (ch<<8)+tCh;
    return CHREG2FREQ(ch);
}

//uint8 QNF_GetRSSIn(uint16 chFreq)
//{
//    uint8 idx,i;
//    for(i=0; i<QND_BAND_NUM; i++) 
//    {
//        if(chFreq < qnd_RSSInBB[i+1]) 
//        {
//            // finally come to here, if no wrong input
//            idx = i;
//        }
//        
//    }
//    return qnd_RSSIn[idx];
//}
//
uint8 QND_GetRSSI_2(void) 
{    
	uint8 result;
	I2C_Enable();
	I2C_Init();
    result = QND_ReadReg(0x03);  
	I2C_Disable();
  	SetMfpToNand(); //将GPIO切到NAND FLASH
	return	result; 
}
//
//uint16 QND_RXSeekCH(uint16 start, uint16 stop, uint8 step, uint16 db, uint16 up) 
//{
//		uint8 regValue;
//    uint16 c, chfreq;
////    uint16 ifcnt;
//    uint8 savevalue;
//    uint8 rssi/*,mprssi*/;
//    uint8 scanFlag = 1;
//    int8 stepvalue;
//    uint8 timeout = 0;
//    uint8 minrssi;
//    uint8 pilot_snr;
//    uint16 pStart = start;
//    uint16 pStop = stop;
//    up=(pStart<=pStop) ? 1 : 0;
//
//    if (qnd_AutoScanAll == 0)
//    {
//        QNF_SetMute(1);
//        QNF_SetRegBit(32,0x08,0x08); //WQF0715
//        QNF_SetRegBit(46,0x30,0x30); //WQF0715
//    }
//    {
//        stepvalue = qnd_StepTbl[step];
//        
//        if (qnd_AutoScanAll == 0)
//        {
//            QND_UpdateRSSIn(0);	
//        }
//        
//        QNF_SetRegBit(0x02, 0x80, 0);	
//        
//        do
//        {
//            minrssi = QNF_GetRSSIn(pStart);
//            QNF_ConfigScan(pStart, pStop, step);
//            savevalue = minrssi - 22 + db;
//            if (savevalue > 63)
//            {
//                savevalue = 63;
//            }
//           	QND_WriteReg(0x01, savevalue);
//            QNF_SetRegBit(0x00,0x03,0x02);
//            do
//            {
//                FM_Wait1ms(5);
//                regValue =QND_ReadReg(0x00);
//            } while((regValue&0x03));
//    
//            regValue = QND_ReadReg(0x04)& 0x08;
//            if (regValue )
//            {
//                chfreq = 0;
//                scanFlag = 0;
//            }
//            else
//            {
//                chfreq = QNF_GetCh();
//                rssi = QND_GetRSSI(chfreq);
//                if (rssi > minrssi + 10 +db)
//                {
//                    scanFlag = 0;
//                }
//                else
//                {                    
//                    FM_Wait1ms(70);
//                    pilot_snr = (QND_ReadReg(0x01) & 0x01) ? 0 : 1;   
//                    if (pilot_snr)
//                    {
//                        scanFlag = 0;
//                    }            
//                    else
//                    {
//                        FM_Wait1ms(80); 
//                        pilot_snr = QND_ReadReg(0x22);  
//                        if (pilot_snr > 2 && pilot_snr < 55) 
//                        {
//                            scanFlag = 0;
//                        }
//                    }
//                }
//
//                if (scanFlag)
//                {
//                    pStart = chfreq + (up ? stepvalue : (-stepvalue));
//                    if ((up && (pStop < pStart)) || (!up && (pStart < pStop)))
//                    {
//                        scanFlag = 0;
//                        chfreq = 0;
//                    }
//                }
//                else
//                {
////                    if(qnd_CallBackFunc)
////                      ;// qnd_CallBackFunc(chfreq, BAND_FM);
//                }
//            }
//        } 
//        while (scanFlag != 0);
//        c = chfreq;
//    }
//
//    if (qnd_AutoScanAll == 0)
//    {
//        FM_SetFrequency(c ? c: stop); 
//    }
//    return c;  
//
//}
//
//void QND_UpdateRSSIn(uint16 ch) 
//{
//    uint8 temp;
//    uint8 v_abs;
//    uint8 i,j;
//    uint8 r0;
//    uint16 chfreq ;
//    uint8 minrssi;
//    uint8 tmp1,tmp2;
////     XBYTE temp;
////     XBYTE v_abs;
//    if (qnd_FirstScan == 0 )
//    {
//        temp = QND_GetRSSI(qnd_ClearChannel);
//
//        if(temp > qnd_RSSIns)
//        {
//            v_abs = temp - qnd_RSSIns;
//        }
//        else
//        {
//            v_abs = qnd_RSSIns - temp;
//        }
//        if (v_abs < RSSINTHRESHOLD)
//        {
//            qnd_ClearScanFlag = 0;    
//        }
//        else
//        {
//            qnd_ClearScanFlag = 1;
//        }
//    }
//    if (qnd_ClearScanFlag||qnd_FirstScan||ch)
//    {
//        r0 = QND_ReadReg(0x00);    
//
//    		for( i = 0; i < QND_BAND_NUM; i++ )
//    		{
//        		minrssi = 255;
//        		for (j = 0; j < 4; j++)
//        		{
//            		tmp1 = QND_GetRSSI(qnd_RSSInBB[i] + j*qnd_StepTbl[qnd_CH_STEP]);
//            		if(i == QND_BAND_NUM-1 )
//                		tmp2 = QND_GetRSSI(qnd_RSSInBB[i+1] - qnd_StepTbl[qnd_CH_STEP] - j*qnd_StepTbl[qnd_CH_STEP]);
//            		else
//                		tmp2 = QND_GetRSSI(qnd_RSSInBB[i+1] - j*qnd_StepTbl[qnd_CH_STEP]);
//            		if (tmp1 <  minrssi)
//            		{
//                		minrssi = tmp1;
//                		chfreq = qnd_RSSInBB[i] + j*qnd_StepTbl[qnd_CH_STEP];
//            		}
//            		if (tmp2 <  minrssi)
//            		{
//                		minrssi = tmp2;
//                		chfreq = qnd_RSSInBB[i+1] - j*qnd_StepTbl[qnd_CH_STEP];
//            		}
//        		}
//        		qnd_RSSIn[i] = minrssi;
//        		if (qnd_RSSIns > minrssi)
//        		{
//            		qnd_RSSIns = minrssi;
//            		qnd_ClearChannel = chfreq;
//        		}
//    		}
//    		QND_WriteReg(0x3b,qnd_RSSIns + 12);
//    		QND_WriteReg(0x3d,qnd_RSSIns + 12);
//    		QND_WriteReg(0x3c,qnd_RSSIns + 18);
//    		// restore SYSTEM1 register
//    		QND_WriteReg(0x00, r0);
//        qnd_FirstScan = 0;
//    }
//    return;
//}

void QNF_RXInit_2()
{
    QNF_SetRegBit(0x1B,0x08,0x00); //Let NFILT adjust freely
    QNF_SetRegBit(0x2C,0x3F,0x09); //When SNR<ccth31, ccfilt3 will work
    QNF_SetRegBit(0x1D,0x40,0x00);//Let ccfilter3 adjust freely
    QNF_SetRegBit(0x41,0x0F,0x0A);//Set a hcc index to trig ccfilter3's adjust
    QND_WriteReg(0x45,0x20);//Set aud_thrd will affect ccfilter3's tap number
    QNF_SetRegBit(0x40,0x70,0x70); //snc/hcc/sm snr_rssi_sel; snc_start=40; hcc_start=22; sm_start=20
    QNF_SetRegBit(0x19,0x80,0x80); //Use SNR for ccfilter selection criterion
    QNF_SetRegBit(0x3E,0x80,0x80); //it is decided by programming this register
    QNF_SetRegBit(0x41,0xE0,0xA0);//DC notching High pass filter bandwidth; remove low freqency dc signals        
    QNF_SetRegBit(0x42,0x10,0x10);//disable the vtune monitor
    QNF_SetRegBit(0x34, 0x7F,20); //set SNCSTART
   // QNF_SetRegBit(0x35,0x7F,SNCSTART_VAL); //set SNCSTART
   // QNF_SetRegBit(0x36,0x7F,HCCSTART_VAL); //set HCCSTART
   QNF_SetRegBit(0x35,0x7F,60); //Change SNCSTART=60,the past SNCSTART=40
   QNF_SetRegBit(0x36,0x7F,36); //Change HCCSTART=36,the past HCCSTART=22
    
}

uint8 QNF_SetCh_2(uint16 freq) 
{
    // calculate ch parameter used for register setting
    uint8 tStep;
    uint8 tCh;
    uint16 f; 
        f = FREQ2CHREG(freq);
        //WFreq = f; 
        // set to reg: CH
        tCh = (uint8) f;
        QND_WriteReg(0x07, tCh);
        // set to reg: CH_STEP
        tStep = QND_ReadReg(0x0a);
        tStep &= ~0x03;
        tStep |= ((uint8) (f >> 8) & 0x03);
        QND_WriteReg(0x0a, tStep);
				
    return 1;
}
void QNF_InitRSSInBB_2() 
{
    uint8 i,d2,d,step; 
    uint16 d1;

    // get frequency leap step
    step = qnd_StepTbl[qnd_CH_STEP];
    // total frequency span
    d1 = qnd_CH_STOP - qnd_CH_START;
    // make sure d2 value <= 255, normally QND_BAND_NUM should < 10
    d2 = step * QND_BAND_NUM;
    d = d1/d2;
    // first one should be CH_START
    qnd_RSSInBB[0] = qnd_CH_START;
    for(i=1; i<QND_BAND_NUM; i++) {
        qnd_RSSInBB[i] = qnd_RSSInBB[i-1] + d * step;
    }
    // last one set one more step higher for convenience
    qnd_RSSInBB[i] = qnd_CH_STOP+step;
}

/*
 ********************************************************************************
 *             uint8 FM_SetFrequency(uint16 Freq)
 *
 * Description : 频点设置
 *
 * Arguments   :	需设置的频率值
 *
 * Returns     :	 若频点设置成功，返回1;否则,返回0
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint8 FM_SetFrequency_2(uint16 Freq)
{
    uint8 reg;

    Freq *= 10;
    QNF_RXInit_2();
    QNF_SetMute_2(1);
    if ((Freq==8430)||(Freq==7290)||(Freq==6910))  //Peter has a list of channel to flip IMR. Please ask him for update
    {
        QNF_SetRegBit(0x01, 0x40, 0x40);           //this is a software patch
    }
    else
    {
        QNF_SetRegBit(0x01, 0x40, 0x00);  
    }
    QNF_ConfigScan(Freq, Freq, qnd_CH_STEP); 
    QNF_SetCh_2(Freq);
    QNF_SetRegBit(0x00, 0x33, 0x13);  //Enter CCA mode. This speed up the channel locking. 
    //Auto tuning
    QND_WriteReg(0x4F, 0x80);
    reg = QND_ReadReg(0x4F);
    reg >>= 1;
    QND_WriteReg(0x4F, reg);
    FM_Wait1ms(500);	 
    QNF_SetMute_2(0);  
    return 1;
}


/*	   
 ********************************************************************************
 *             uint8 FM_Search(fm_play_status_t * Buf,uint16 Freq,uint8 SeekDirect)
 *
 * Description : FM搜台
 *
 * Arguments   :
 param1:  保存电台信息的结构体指针
 param2:  软件搜台方式,需设置的频率值
 param3:  搜索方向
 * Returns     :		如果搜到的电台为真台,则返回1;否则,返回0
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint8 FM_Search_2(fm_play_status_t * Buf, uint16 Freq, uint8 SeekDirect)
{
    uint16 wFreq;
	uint32 wFreqStop;
	uint16 f;
    uint8 regValue;
    uint8 timeOut=0;

    Freq = Freq*10;
	if(SeekDirect == 1)
	{
	    if(Buf->mode == 0)
	        wFreqStop = FM_BAND_US_TOP*10; 
	    else
	        wFreqStop = FM_BAND_US_TOP*10; 
	}
	else 
	{
	    if(Buf->mode == 0)
	        wFreqStop = FM_BAND_US_BOTTOM*10; 
	    else
	        wFreqStop = FM_BAND_US_BOTTOM*10; 
	}
	f = (Freq-6000)/5; 
	
	I2C_Enable();
	I2C_Init();
    /////////////////////////////////////////////////
    QNF_SetRegBit(0x1B, 0x08, 0x08);//NFILT program is enabled 
    QNF_SetRegBit(0x27, 0x30, 0x30); //use threshold extension filter  
    QNF_SetRegBit(0x01, 0x3F, 25);//setting the threshold for CCA	
    QNF_SetRegBit(0x39, 0x3F, 11); //set SNR threshold to be 11
    QNF_SetRegBit(0x3A, 0xC0, 0xC0); 
    /////////////////////////////////////////////////
	QNF_ConfigScan(Freq, wFreqStop, 0x01);  //
	//QNF_SetRegBit(0x01, 0x3F, 0x14);
	QNF_SetRegBit(0x00, 0x03, 0x02);
 
    QNF_SetMute_2(1);
    do
    {
        regValue = QND_ReadReg(0x00);
        timeOut++;
    } while((regValue & 0x02) && (timeOut<100));//when seeking a channel or time out,be quited the loop
    if ((QND_ReadReg(0x04)& 0x08) != 0)
  	{
       	Buf->stereo_status = 1;
       	SetMfpToNand(); //将GPIO切到NAND FLASH
       	return 0;
  	}
  	else
  	{
  		Buf->stereo_status = 0;
  	}
  	wFreq = QNF_GetCh_2();
  	FM_Wait1ms(50);
	    
 	    wFreq = wFreq /10;
 		Buf->freq = wFreq ;
 		FM_SetFrequency_2(wFreq);
 		QNF_SetMute_2(0);
	    I2C_Disable();
//			#endif
  		//SetMfpToNand(); //将GPIO切到NAND FLASH
  		return TRUE;
}



