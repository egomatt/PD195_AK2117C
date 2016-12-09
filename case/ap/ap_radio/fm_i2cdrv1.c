/********************************************************************************************
 Airoha Technology FM module driver(I2C DRIVER)  for ATJ2097
 last update 2007/11/02
 Version:V0.5
 ********************************************************************************************/
#include "ap_radio.h"

#pragma name(FM_CTRL_D)

uint8 qnd_i2c;
uint8 qnd_i2c_timeout;
uint8 qnd_PrevMode;
uint8 qnd_Country  = COUNTRY_CHINA ;
uint16 qnd_CH_START;
uint16 qnd_CH_STOP;
uint8 qnd_CH_STEP  = 1;
uint8 qnd_StepTbl[3]={5,10,20};
uint8 qnd_RSSInBB[QND_BAND_NUM+1]; 
uint8 qnd_RSSIns = 255;
uint16 qnd_ClearChannel = 0;
uint8 qnd_ClearScanFlag = 0;
uint8 qnd_FirstScan = 1;
uint8 qnd_AutoScanAll=0;
uint8 qnd_RSSIn[QND_BAND_NUM];
uint8 Is_Stere0 = 0;
uint8 EnterFlag = 0;
uint16 WFreq;

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
void OpenPA(void);

void QND_SetSysMode(uint16 mode); 	
uint8 FM_Standby(void);

uint8 QN_ChipInitialization_1(void);
uint8 QN_ChipInitialization_2(void);
void QNF_SetMute_1(BYTE On);
void QNF_SetMute_2(BYTE On);
uint16 QNF_GetCh_1();
uint16 QNF_GetCh_2();
uint8 QND_GetRSSI_1(void);
uint8 QND_GetRSSI_2(void);
void QNF_RXInit_1();
void QNF_RXInit_2();
uint8 QNF_SetCh_1(uint16 freq);
uint8 QNF_SetCh_2(uint16 freq);
void QNF_InitRSSInBB_1();
void QNF_InitRSSInBB_2();
uint8 FM_SetFrequency_1(uint16 Freq);
uint8 FM_SetFrequency_2(uint16 Freq);
uint8 FM_Search_1(fm_play_status_t * Buf, uint16 Freq, uint8 SeekDirect);
uint8 FM_Search_2(fm_play_status_t * Buf, uint16 Freq, uint8 SeekDirect);


uint16 QND_RXValidCH(uint16 freq, uint8 step);
void QNF_ConfigScan(uint16 start,uint16 stop, uint8 step);
//uint16 QND_RXSeekCH(uint16 start, uint16 stop, uint8 step, uint16 db, uint16 up);
//void QND_UpdateRSSIn(uint16 ch) ;
//void QNF_UpdateRSSIn(uint16 chFreq);


void FM_Wait1ms(uint16 time)
{
	uint16 j, i;
    for (j = 0; j < time; j++)
    {
    	for (i = 2000; i > 0; i--)
    	{
        	; //FM短暂延时
    	}
    }

} 

void QN8025_RST(uint8 OnOff)	
{
	uint8 sfr_bak;
	sfr_bak = SFR_BANK;
    SFR_BANK = BANK_GPIO;
    GPIOAOUTEN |= 0x01;
    if (OnOff)
    {
				 
		GPIOADAT |= 0x01;
	}
	else
	{
		GPIOADAT &= ~0x01;
	}
	SFR_BANK = sfr_bak;
}

uint8 FM_Init(uchar mode)
{
//	uint8 result;
    uint8 Analogtempbuf[2] =
    { 0, 0 };

//		uint8 i=30;
    QN8025_RST(0);
    FM_Wait1ms(5);
    QN8025_RST(1);
    FM_Wait1ms(5);
    I2C_Enable();
    I2C_Init(); //I2C初始化

    if(mode == 0)
    {
	    qnd_CH_START = FM_BAND_US_BOTTOM;
        qnd_CH_STOP = FM_BAND_US_TOP;
    }
    else
    {
	    qnd_CH_START = FM_BAND_JP_BOTTOM;
        qnd_CH_STOP = FM_BAND_JP_TOP;
    }
    qnd_CH_STEP = 1;

    if((QND_ReadReg(0x06)&0xfc)==0x80)
    {
        FM_Chip = 1;
    }
    else if((QND_ReadReg(0x06)&0xfc)==0x84)
    {
        FM_Chip = 2;
    }
    else
    {
        FM_Chip = 0;
        SetMfpToNand(); //将GPIO切到NAND FLASH  
    	return 0;
    }
    
    if(FM_Chip == 1) 
    { 
        QN_ChipInitialization_1();
      	QNF_SetMute_1(1);	
      	FM_Wait1ms(50);
      	QNF_InitRSSInBB_1();
      	QND_WriteReg(0x00,0x01);	
      	QND_SetSysMode(QND_MODE_RX|QND_MODE_FM);
      	QNF_SetMute_1(0);
    }
    else //if(FM_Chip == 2) 
    { 
        QN_ChipInitialization_2();
      	QND_SetSysMode(QND_MODE_FM|QND_MODE_RX);
      	QNF_SetMute_2(1);
      	QNF_SetRegBit(0x14, 0x80, 0x00); 
      	QNF_SetRegBit(0x14, 0x07, 6); 
      	QNF_SetRegBit(0x14, 0x38, (uint8)((5-4)<<3));   	
        QNF_SetMute_2(0);
    }
    
    I2C_Disable();
  	SetMfpToNand(); //将GPIO切到NAND FLASH
    FM_Wait1ms(50);
    
	Analogtempbuf[0] = 0x01;
    EnableAIN(Analogtempbuf[0]); //使能FM到ADC的输入
    if (EnterFlag == 0)
    {
        OpenPA();
    }
    		
    return 1;
}
/*
 ********************************************************************************
 *             void FM_Exit(void)
 *
 * Description : 退出FM
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void FM_Exit(void)
{
	uint8 sfr_bak;
	sfr_bak = SFR_BANK;
    SFR_BANK = BANK_GPIO;
    QN8025_RST(0);
    SFR_BANK = sfr_bak;
    SetPAVolume(0);
    DisableAIN();
    if (EnterFlag == 0)
    {
        if (g_comval.EarphoneFlag == 0)
        {
            DisablePA(); //耳机非直驱方式关PA
        }
        else
        {
            DisablePADDV(); //耳机直驱方式关PA
        }
        sfr_bak = SFR_BANK;
        SFR_BANK = 0x01; //disable ADC和DAC
        MRCR2 &= 0xcf;
        SFR_BANK = sfr_bak;
    }
}
/*
 ********************************************************************************
 *             uint8 FM_SetFreq(uint16 Freq)
 *
 * Description : 设置频点,进行tune操作
 *
 * Arguments   :	  需设置的频点值
 *
 * Returns     :		//设置频点是否成功,若为真台,返回1
 *					       否则,返回0
 * Notes       :
 *
 ********************************************************************************
 */
uint8 FM_SetFreq(uint16 Freq)
{
    uint16 result;
    I2C_Enable();
    I2C_Init(); //将GPIO切到I2C

    if(FM_Chip == 1)
    {
        result = FM_SetFrequency_1(Freq);
    }
    else
    {
        result = FM_SetFrequency_2(Freq);
    }
        
    //result = CheckStation();
//    if (result != 0)
//    {
//        Is_Stere0 = 1;
//    }
//    else
//    {
//        Is_Stere0 = 0;
//    }
    I2C_Disable();
    SetMfpToNand(); //将GPIO切到NAND FLASH
    return result;
}
/*
 ********************************************************************************
 *             uint8 FM_GetStatus(FMStatus_Tab_t * Buf)
 *
 * Description : 获取搜索到的真台的频点和立体声信息
 *
 * Arguments   :	  保存电台信息的结构体指针
 *
 * Returns     :		//是否读取状态成功,如果读取成功,则返回值为1
 *					       否则,返回0
 * Notes       :
 *
 ********************************************************************************
 */
uint8 FM_GetStatus(FMStatus_Tab_t * Buf)
{
//    uint8 result;
    uint16 value;
//     uint8 tStep;
//    uint8 tCh;
//    uint16 f;
    I2C_Enable();
    I2C_Init(); //将GPIO切到I2C
	
    if ((QND_ReadReg(0x04)& 0x01) != 0)
    {
          Buf->FM_Stereo_Status = 1;
    }
    else
    {
          Buf->FM_Stereo_Status = 0;
          
    }

    if(FM_Chip == 1)
        value = QNF_GetCh_1();
    else
        value = QNF_GetCh_2();
        
    Buf->FM_CurrentFreq = value / 10; //WFreq ;//value / 10;

    I2C_Disable();
    SetMfpToNand();
    return 1;
} 
/*
 ********************************************************************************
 *             uint8 FM_Mute(FM_MUTE_t mode)
 *
 * Description : FM静音设置
 *
 * Arguments   :	  是否静音,0为取消静音,1为静音
 *
 * Returns     :		 若设置静音控制成功,则返回1,否则,返回0
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint8 FM_Mute(FM_MUTE_t mode)
{
    I2C_Enable();
    I2C_Init(); //将GPIO切到I2C
   
    if(mode)
    {
        if(FM_Chip == 1)
    		QNF_SetRegBit(0x4A,0x03,0x03);
        else
            QNF_SetRegBit(0x4C, 0x0B, 0x0B);
    }
    else
    {
    	if(FM_Chip == 1)
    	{
    		//FM_Wait1ms(200);
    		QNF_SetRegBit(0x4A,0x03,0x00);
        }
        else
        {
            FM_Wait1ms(200);
            QNF_SetRegBit(0x4C, 0x0B, 0x00);
        }
   }
    	
    I2C_Disable();
    SetMfpToNand(); //将GPIO切到NAND FLASH
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
uint8 FM_Search(fm_play_status_t * Buf, uint16 Freq, uint8 SeekDirect)
{
    if(FM_Chip == 1)
        return FM_Search_1(Buf, Freq, SeekDirect);
    else
        return FM_Search_2(Buf, Freq, SeekDirect);
}

/*
 ********************************************************************************
 *             void FM_VolumeSet(uint8 Vol)
 *
 * Description : 设置PA音量
 *
 * Arguments   :	  需设置的音量值
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
void FM_VolumeSet(uint8 Vol)
{
    SetPAVolume(Vol);
    // SetVolume(Vol);
}


uint8 FM_Standby_Drv(uint8 StandbyFlag)
{
//    uint8 result;
   // Set_Standby = StandbyFlag;
    if (StandbyFlag == 0)
    {
        QND_WriteReg(0x00,0x21);
    }
    return 1;
}

uint8 FM_Standby(void)
{
    uint8 result;
    I2C_Enable();
    I2C_Init(); //将GPIO切到I2C
    result = FM_Standby_Drv(0);
    I2C_Disable();
    SetMfpToNand(); //将GPIO切到NAND FLASH
    return result;
}


void OpenPA(void)
{
    pa_t paattr;
    uint8 sfr_bak;
    sfr_bak = SFR_BANK;
    paattr.pa_in.dacin = 0;
    paattr.pa_in.fmin = 1; //选择PA的输入为FMIN
    paattr.pa_in.linein = 0;
    paattr.pa_in.micin = 0;

    SFR_BANK = 0x01; //使能FMIN到PA的通路
    MRCR2 |= 0x30;
    SFR_BANK = BANK_AUIP;
    FMOP_CTL &= (uint8) 0xf8;
    FMOP_CTL |= 0x05;
    FMOP_CTL |= 0x18;
    SFR_BANK = sfr_bak;
    paattr.volume = 0;
    if (g_comval.EarphoneFlag == 0)
    {
        EnablePA(&paattr);
    }
    else
    {
        EnablePADDV(&paattr);
    }
}


uint8 QND_GetRSSI(void)
{
    if(FM_Chip == 1)
        return QND_GetRSSI_1();
    else
        return QND_GetRSSI_2();
}

void QNF_SetMute(BYTE On)
{
    if(FM_Chip == 1)
        QNF_SetMute_1(On);
    else
        QNF_SetMute_2(On);
}

void QND_SetSysMode(uint16 mode) 	
{    
    uint8 val;
    I2C_Enable();
		I2C_Init();
    switch(mode)        
    {        
        case QND_MODE_SLEEP:                       //set sleep mode        
        //QNF_SetRegBit(0x48, 0x80, 0x00);
        qnd_PrevMode = QND_ReadReg(0x00);        
        QNF_SetRegBit(0x00, 0x30, 0x20);         
        break;        
    	case QND_MODE_WAKEUP:                      //set wakeup mode        
        QND_WriteReg(0x00, qnd_PrevMode);  
        QNF_SetMute(0);
        break;        
    	case QND_MODE_DEFAULT:
        QNF_SetRegBit(0x00,0x30,0x10);
        break;
        default:    
        val = (uint8)(mode >> 8);        
        if (val)
        {
            //QNF_SetRegBit(0x48, 0x80, 0x80);
            val = val >> 3;
            if(val&0x10)
                // set to new mode if it's not same as old
                if((QND_ReadReg(0x00) & 0x30) != val)
                {
                    QNF_SetMute(1);
                    QNF_SetRegBit(0x00, 0x30, val); 
                    //   QNF_SetMute(0);
                }
                // make sure it's working on analog output
                //QNF_SetRegBit(0x00, 0x08, 0x00);    
        }    
        break;        
    }    
    I2C_Disable();
}


