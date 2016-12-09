/********************************************************************************************
 Airoha Technology FM module driver(I2C DRIVER)  for ATJ2097
 last update 2007/11/02
 Version:V0.5
 ********************************************************************************************/
#include "ap_radio.h"

#pragma name(FM_CTRL_D1)


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


uint8 QN_ChipInitialization_1(void)
{
	QND_WriteReg(0x00,0x81);        //sw rst
    FM_Wait1ms(20);
    //change crystal setting here
    QND_WriteReg(0x15,0x00);        //select xtal
    QND_WriteReg(0x16,0x02);        //select xtal
    QND_WriteReg(0x18,0x10);        //select xtal
    QND_WriteReg(0x2b,0xe5);        //select xtal
    QND_WriteReg(0x2f,0x98);        //set agc_g2th (??)
    QND_WriteReg(0x40,0x01);
    QND_WriteReg(0x45,0x39);
    QND_WriteReg(0x46,0x05);
    QND_WriteReg(0x47,0xd0);
    QND_WriteReg(0x48,0xa8);
    QND_WriteReg(0x4A,0x08);
    QND_WriteReg(0x00,0x51);        //re-cal	
    QND_WriteReg(0x00,0x01);
    QND_WriteReg(0x24,0xA9);    //HPF3DB2 to improve SNR(weak signal)
    FM_Wait1ms(20);                 
    // for speeding up SNR reading    
    QNF_SetRegBit(43,0x30,0x00);    //minimize rxagc_timeout
    QNF_SetRegBit(51,0xc0,0x00);    //minimize ccnclk2
    QNF_SetRegBit(52,0x60,0x00);        //minimize ccnclk1
    QNF_SetRegBit(32,0x08,0x08);        //WQF0715
    QNF_SetRegBit(46,0x30,0x20);        //WQF0715
    //PX&WQF: Following setting are moved here from other places
    QND_WriteReg(0x3a, 0xff);
    QND_WriteReg(0x3e, 0x12);
    QNF_SetRegBit(0x28,(1<<7),(1<<7)); //set ccth11
    QNF_SetRegBit(0x35,0xf,0xf);       //set ccth11
    // HW CCA time from 5ms/CH to 10ms/CH 
    QNF_SetRegBit(0x2f,0x38,0x18);     //R47[5:3]=3'b011
    QNF_SetRegBit(0x2b,0xc0,0xc0);     // R43[7:6]=2'b11  
    QNF_SetRegBit(43,0xc0,0xc0); //CCA: 5ms -> 10ms  ifcnt central value: 2048->4096
    QNF_SetRegBit(47,0x38,0x18); //CCA: 5ms -> 10ms
    return 1;
}

void QNF_SetMute_1(BYTE On)	
{
	if(On)
	{
			QNF_SetRegBit(0x4a, 0x03, 0x03); // mute/unmute to avoid noise       
	}
	else
	{
			FM_Wait1ms(200);
			QNF_SetRegBit(0x4a, 0x03, 0x00); // mute/unmute to avoid noise     
	}

}

//void QNF_ConfigScan(uint16 start,uint16 stop, uint8 step) 
//{
//    // calculate ch para
//    uint8 tStep = 0;
//    uint8 tS;
//    uint16 fStart;
//    uint16 fStop;
//    fStart = (start-6175)/5;
//    fStop = (stop-6175)/5;
//        // set to reg: CH_START
//    tS = (uint8) fStart;
//    QND_WriteReg(0x08, tS);
//    tStep |= ((uint8) (fStart >> 6) & CH_CH_START);
//    // set to reg: CH_STOP
//    tS = (uint8) fStop;
//    QND_WriteReg(0x09, tS);
//    tStep |= ((uint8) (fStop >> 4) & CH_CH_STOP);
//    // set to reg: CH_STEP
//    tStep |= step << 6;
//    QND_WriteReg(0x0a, tStep);
//}

uint16 QNF_GetCh_1() 
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
    return (ch*5+6175);
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

uint8 QND_GetRSSI_1(void) 
{    
	uint8 result;
	I2C_Enable();
	I2C_Init();
    result=QND_ReadReg(0x03);  
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

uint8 QNF_SetCh_1(uint16 freq) 
{
    // calculate ch parameter used for register setting
    uint8 tStep;
    uint8 tCh;
    uint16 f; 
        f = (freq-6175)/5;
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
void QNF_InitRSSInBB_1() 
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
uint8 FM_SetFrequency_1(uint16 Freq)
{
		//WFreq = Freq;
    Freq = Freq*10;
//		uint8 rssi;
//    uint8 minrssi;

    //ch=ch*10;
		
    QNF_SetRegBit(72,0x80,0x80);
    QNF_SetRegBit(36,0x40,0x40);
    QNF_SetRegBit(0x3b,0x3f,8);
    QNF_SetRegBit(0x3d,0x3f,22);
    QNF_SetRegBit(0x3c,0x7f,40);
   	QNF_SetRegBit(0x4A,0x03,0x03);
//		QNF_SetMute_1(1);
    QNF_SetCh_1(Freq);
    FM_Wait1ms(200);
    QNF_SetRegBit(0x00,0x01,0x01);
    QNF_SetRegBit(0x20,0x08,0x08);
    QNF_SetRegBit(0x2e,0x30,0x20);
    FM_Wait1ms(200);
    QNF_SetRegBit(0x4A,0x03,0x00);
    QNF_SetMute_1(0);
    
//    QNF_RXInit_1();
//    QNF_SetMute_1(1);  
//    if ((Freq==10710)||(Freq==9000)||(Freq==9570))
//    {
//        QNF_SetRegBit(0x02, 0x80, 0x80);           //blur patch      
//    }
//    else
//    {
//        QNF_SetRegBit(0x02, 0x80, 0x00);      
//    }  
//    QNF_SetCh_1(Freq);		
//		FM_Wait1ms(50);
//		QNF_SetRegBit(0x00, 0x01, 0x01); 
//		if (QND_ReadReg(0x01) & 0x08) //check RDS status for filter setting
//    {
//        QNF_SetRegBit(0x20,0x08,0x00); //No forced setting for channel filter
//    }
//    else
//    {
//        QNF_SetRegBit(0x20,0x08,0x08); //forced setting for channel filter
//        QNF_SetRegBit(0x2e,0x30,0x20); //Set filter 2 for Non-RDS
//    }
//    QNF_SetMute_1(0);
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
uint8 FM_Search_1(fm_play_status_t * Buf, uint16 Freq, uint8 SeekDirect)
{
			
			uint16 wFreq;
			uint32 /*wFreqStart,wFreqEnd,*/wFreqStop;
			uint16 f;
			uint8 wFreqStep,i,j;
			Freq = Freq*10;
    		I2C_Enable();
			I2C_Init();
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
            wFreqStop = (wFreqStop-6175)/5; 
            
			QNF_SetRegBit(0x4A,0x03,0x03);
			QNF_SetRegBit(32,0x08,0x08);
			QNF_SetRegBit(46,0x30,0x30);
			QNF_SetRegBit(0x02,0x80,0x00);
			f = (Freq-6175)/5; 
        // set to reg: CH
			QND_WriteReg(0x08,(uint8)f);
			QND_WriteReg(0x09,(uint8)wFreqStop);
			i = (uint8)(f / 256);
			j = (uint8)(wFreqStop / 256); 
			wFreqStep = (i << 2) | (j << 4);
			 
			QND_WriteReg(0x0A,wFreqStep|0x40);
			#ifdef Greenhouse
			QND_WriteReg(0x01,30);//26
			#else
			QND_WriteReg(0x01,26);//30
			#endif
			QNF_SetRegBit(0x00,0x03,0x02);
			
//			while((QND_ReadReg(0x00) & 0x03) == 0)
//			{
//					FM_Wait1ms(50);
//			}
			FM_Wait1ms(200); 
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
  		wFreq = QNF_GetCh_1();
  		FM_Wait1ms(50);
		#ifdef Greenhouse
		if(QND_ReadReg(0x03) >= 76)	//66
		#else
  		if(QND_ReadReg(0x03) >= 66)
		#endif
  		{
  					goto SEEK_SUCCES;

  	    }
  		FM_Wait1ms(50);
		
		#ifndef Greenhouse
  		if((QND_ReadReg(0x04) & 0x01) == 1)
  		{
  				
//  				SetMfpToNand(); //将GPIO切到NAND FLASH
//  				return 0;
						goto  SEEK_SUCCES;
  	    }
		#endif
		
  	    FM_Wait1ms(50);
  	  		if((QND_ReadReg(0x22) < 10)||(QND_ReadReg(0x22) > 55))
  				{
  						SetMfpToNand(); //将GPIO切到NAND FLASH
  						return 0;
  				}
//  				if(QND_ReadReg(0x22) != 55)
//  				{
//  						SetMfpToNand(); //将GPIO切到NAND FLASH
//  						return 0;
//  				}
  		//WFreq = wFreq/10;
 SEEK_SUCCES:
 			wFreq = wFreq /10;
 			Buf->freq = wFreq ;
 	 		FM_SetFrequency_1(wFreq);//Freq
    		I2C_Disable();
  		SetMfpToNand(); //将GPIO切到NAND FLASH
  		return TRUE;
}


