// jc3320.h
// revision history
// 120113 initial revision
// 120114 add CP_CTL1, NTRIRQ, NORMALLENH, NORMALLENL and HCIN2CNTL and NANDFLASH registers
// 120117 modify some SFRs' address of NANDFLASH controller
// 120117 add some SFRs of BANK_GPIO
// 120117 modify the address of EXTMEM_D
// 120117 add DMA0LCDCFG, DMA1LCDCFG, DMA2LCDCFG register
// 120117 add DBGCIE debug register
// 120119 add some ACC registers
// 120202 add CLASSD registers
// 120202 add MJPEG registers
// 120202 rename ADC_FITOCTL1 to ADC_FIFOCTL1
// 120203 modify the address of APETTIMEH, APETTIMEM, APETTIMES, APECTIMEH, APECTIMEM, APECTIMES and APEErrInfo
// 120203 modify DMA0DBG to DMA012DBG register, delete DMA1DBG & DMA2DBG register
// 120216 delete PWMCLKCTL register
// 120221 add OutBase register and modify the address of Debug register
// 120228 fix some syntax error
// 120306 modify the address of NAND_BCL register and delete the ACCCLKCTL register
// 120311 add 3D speaker and BassBack registers
// 120313 add ADC_DAC_CLK_CTL register
// 120321 change DAC_TUNE0 to address 0xf8
// 120322 add BANK_LCD
// 120322 change register name DMA_CTL to DMA3_CTL
// 120322 add PA_CTL @ 0x4 page sfr address 0xfc
// 120322 add FMOP_CTL @ 0x4 page sfr address 0xfd
// 120330 modify Index to AmvAuIndex
// 120330 modify FrameLenH to AmvFrameLenH
// 120330 modify FrameLenL to AmvFrameLenL
// 120401 add DiscardLen register
// 120401 modify LOSC1_CTL register to CMU_RESERVED1 register
// 120401 modify CK48M_CTL to CMU_RESERVED0
// 120401 modify Reserved1 to CMU_RESERVED2
// 120412 add LCD_CTL0,LCD_CTL1,WIDTH_INVALID_CNT register
// 120418 delete the page 0x0a for ACC register access
// 120510 add EFUSE3_ID register
//for simulator
#include "simulatorpc.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// BANK NUMBER OF EACH MODULE
#define BANK_BASIC           0
#define BANK_CMU_RMU         1
#define BANK_DMA0            0xe
#define BANK_DMA1            0xf
#define BANK_DMA2            0x10
#define BANK_AUIP            4
#define BANK_PMU             5
#define BANK_GPIO            6
#define BANK_LCD             6
#define BANK_USB             7
#define BANK_NAND            8
#define	BANK_ACC             8
#define BANK_CARD            9
#define BANK_I2C             10
#define BANK_SPI             10
#define BANK_UART            10
#define BANK_IR              10
#define BANK_MJPEG           11
#define BANK_RTC             12
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __JC3320_H__
#define __JC3320_H__
//  BYTE Registers
// Mnemonic               address       bank number           Description
////////////////////////////////////////////////////////////////////////////////////////////////
// mcu core register
sfr SPH                   =  0xbe;      // all bank     SPH Stack Pointer high byte
sfr SPL                   =  0x81;      // all bank     SPL Stack Pointer low byte
sfr SP                    =  0x81;      // all bank     SPL Stack Pointer low byte  另外一种写法
sfr DPXL                  =  0x84;      // all bank     DPXL    Data Pointer extended byte
sfr DPH                   =  0x83;      // all bank     DPH Data Pointer high byte
sfr DPL                   =  0x82;      // all bank     DPL Data Pointer low byte
sfr PSW                   =  0xd0;      // all bank     PSW Program Status Word
sfr PSW1                  =  0xd1;      // all bank     PSW1    Program Status Word1
sfr ACC                   =  0xe0;      // all bank     ACC Accumulator
sfr B                     =  0xf0;      // all bank     B   B Register
// psw & psw1 bit register
#ifndef PC
sbit CY                   =  0xD7;
#endif
sbit AC                   =  0xD6;
sbit F0                   =  0xD5;
sbit RS1                  =  0xD4;
sbit RS0                  =  0xD3;
sbit OV                   =  0xD2;
sbit UD                   =  0xD1;
sbit P                    =  0xD0;
////////////////////////////////////////////////////////////////////////////////////////////////
// external bus access wait state cycle count
sfr ExWait                =  0x96;      // all bank     ExWait  External bus access wait cycle register
////////////////////////////////////////////////////////////////////////////////////////////////
// debug register
sfr CCMCON                =  0x8e;      // all bank     CCMCON  Communication Control Register
sfr CCMVAL                =  0x8f;      // all bank     CCMVAL  Communication Value Register
sfr BIRDINFO              =  0x85;      // all bank     BIRDINFO    Bird information
////////////////////////////////////////////////////////////////////////////////////////////////
// SFR Bank control
sfr SFR_BANK              =  0x80;      // all bank     SFR_BANK    SFR Banking control Register
////////////////////////////////////////////////////////////////////////////////////////////////
// Indirect Addressing
sfr P2                    =  0xa0;      // all bank     P2  Port2 Register which is not used of IO access
sfr MPAGE                 =  0xa1;      // all bank     MPAGE   same as P2 register
////////////////////////////////////////////////////////////////////////////////////////////////
// Interrupt control
sfr IE0                   =  0xa8;      // all bank     IE0 Interrupt Enable register 0
// sbit EA     			  = IE0^7;
#ifndef PC
sbit EA     			  = IE0^7;
#else
sbit EA     			  = 0xAF;
#endif
sfr AIE                   =  0xe8;      // all bank     AIE Additional Interrupt Enable register
sfr IPH0                  =  0xb7;      // 0x00         Interrupt Priority High register 0
sfr IPL0                  =  0xb8;      // 0x00         Interrupt Priority Low register 0
sfr AIPH                  =  0xf7;      // 0x00         Additional Interrupt Priority High register
sfr AIPL                  =  0xf8;      // 0x00         Additional Interrupt Priority Low register
sfr IF0                   =  0x88;      // all bank     Interrupt Flag register 0
sfr AIF                   =  0xc0;      // all bank     Additional interrupt flag register
sfr EXTINT                =  0xd8;      // 0x06         External Interrupt Control
sfr IFDebug0              =  0xf5;      // 0x01         Interrupt Flag Debug register 0
sfr IFDebug1              =  0xf6;      // 0x01         Interrupt Flag Debug register 0
////////////////////////////////////////////////////////////////////////////////////////////////
// bank switch register
sfr MemBankCtl            =  0xf9;      // all bank     Memory banking control register
sfr PageAddr0             =  0xeb;      // 0x00         Page Address 0
sfr PageAddr1             =  0xec;      // 0x00         Page Address 1
sfr PageAddr2             =  0xed;      // 0x00         Page Address 2
sfr PageAddr3             =  0xee;      // 0x00         Page Address 3
sfr PageAddr4             =  0xef;      // 0x00         Page Address 4
sfr PageAddr5             =  0xf2;      // 0x00         Page Address 5
sfr PageAddr6             =  0xf3;      // 0x00         Page Address 6
sfr PageAddr7             =  0xf4;      // 0x00         Page Address 7
sfr PageMissEntryH        =  0xf5;      // 0x00         Page Miss Entry Address High byte
sfr PageMissEntryL        =  0xf6;      // 0x00         Page Miss Entry Address Low byte
#ifdef __A251__
sfr PageMissEntry         =  0xf5;      // 0x00         Bank Miss Entry Address 16 bit mode access
#endif
////////////////////////////////////////////////////////////////////////////////////////////////
// test counter registers
sfr TestCounterByte0      =  0xfc;      // 0x00         Test Counter byte register 0
sfr TestCounterByte1      =  0xfd;      // 0x00         Test Counter byte register 1
sfr TestCounterByte2      =  0xfe;      // 0x00         Test Counter byte register 2
sfr TestCounterByte3      =  0xff;      // 0x00         Test Counter byte register 3
sfr TestCounterByte4      =  0xf1;      // 0x00         Test Counter byte register 4
////////////////////////////////////////////////////////////////////////////////////////////////
// BIST controller
sfr bist_en1              =  0xe1;      // 0x01         Bist enable register1
sfr bist_en2              =  0xe2;      // 0x01         Bist enable register2
sfr bist_en3              =  0xe3;      // 0x01         Bist enable register3
sfr bist_fin1             =  0xe4;      // 0x01         Bist finish flag register1
sfr bist_fin2             =  0xe5;      // 0x01         Bist finish flag register2
sfr bist_fin3             =  0xe6;      // 0x01         Bist finish flag register3
sfr bist_info1            =  0xe7;      // 0x01         Bist information register1
sfr bist_info2            =  0xe9;      // 0x01         Bist information register2
sfr bist_info3            =  0xea;      // 0x01         Bist information register3
////////////////////////////////////////////////////////////////////////////////////////////////
// RMU
sfr MRCR1                 =  0x86;      // 0x01         Module Reset Control Register 1
sfr PCON                  =  0x87;      // all bank     Power Control
sfr MRCR2                 =  0x89;      // 0x01         Module Reset Control Register 2
sfr MRCR3                 =  0x9f;      // 0x01         Module Reset Control Register 3
////////////////////////////////////////////////////////////////////////////////////////////////
// CMU
sfr MCSR1                 =  0xa9;      // 0x01         Module Clock Select Register 1    (gl5102 style)
sfr MCSR2                 =  0xaa;      // 0x01         Module Clock Select Register 2    (gl5102 style)
sfr MCSR3                 =  0xab;      // 0x01         Module Clock Select Register 3    (gl5102 style)
sfr MCSR4                 =  0xac;      // 0x01         Module Clock Select Register 4    (gl5102 style)
sfr MCSR5                 =  0xad;      // 0x01         Module Clock Select Register 5    (gl5102 style)
sfr MCSR6                 =  0xae;      // 0x01         Module Clock Select Register 6    (gl5102 style)
sfr MCSR7                 =  0xaf;      // 0x01         Module Clock Select Register 7    (gl5102 style)
sfr MCSR8                 =  0xb0;      // 0x01         Module Clock Select Register 8    (gl5102 style)
sfr MCSR9                 =  0xb1;      // 0x01         Module Clock Select Register 9    (gl5102 style)
sfr MCSR10                =  0xb2;      // 0x01         Module Clock Select Register 10   (gl5102 style)
sfr MCSR11                =  0xb4;      // 0x01         Module Clock Select Register 11   (gl5102 style)
sfr MCSR12                =  0xbf;      // 0x01         Module Clock Select Register 12   (gl5102 style)
sfr CLKENCTL0             =  0xa9;      // 0x01         Clock Enable Control Register 0
sfr CLKENCTL1             =  0xaa;      // 0x01         Clock Enable Control Register 1
sfr CLKENCTL2             =  0xab;      // 0x01         Clock Enable Control Register 2
sfr CLKENCTL3             =  0xac;      // 0x01         Clock Enable Control Register 3
sfr CPUCLKCTL             =  0xc2;      // 0x01         CPU Clock Control Register
sfr VECLKCTL              =  0xc3;      // 0x01         Video Codec Clock Control Register
sfr AECLKCTL              =  0xc4;      // 0x01         Audio Codec Clock Control Register
sfr LCDCLKCTL             =  0xc7;      // 0x01         LCD controller Clock Control Register
sfr NANDCLKCTL            =  0xc8;      // 0x01         NAND controller Clock Control Register
sfr SPICLKCTL             =  0xca;      // 0x01         SPI controller Clock Control Register
sfr MEMCLKSELCTL0         =  0xad;      // 0x01         Memory Clock Selection Control Register 0
sfr MEMCLKSELCTL1         =  0xae;      // 0x01         Memory Clock Selection Control Register 1
sfr MEMCLKSELCTL2         =  0xaf;      // 0x01         Memory Clock Selection Control Register 2
sfr MEMCLKSELCTL3         =  0xb0;      // 0x01         Memory Clock Selection Control Register 3
sfr MEMCLKSELCTL4         =  0xb1;      // 0x01         Memory Clock Selection Control Register 4
sfr MEMCLKENCTL0          =  0xb4;      // 0x01         Memory Clock Enable Control Register 0
sfr MEMCLKENCTL1          =  0xbf;      // 0x01         Memory Clock Enable Control Register 1
sfr CMU_RESERVED1         =  0xb6;      // 0x01
sfr LOSC1_CTL             =  0xb6;      // 0x01         for standby ap compile
sfr LOSC2_CTL             =  0xb7;      // 0x01         Dual pin LOSC Control Register
sfr LFPLL_CTL             =  0xb8;      // 0x01         LFPLL Control Register
sfr LFPLL_STATUS          =  0xb9;      // 0x01         LFPLL Status Register
sfr HOSC_CTL              =  0xba;      // 0x01         HOSC Control Register
sfr AUDIOPLL_CTL          =  0xbb;      // 0x01         AudioPLL Control Register
sfr CMU_RESERVED0         =  0xbc;      // 0x01
sfr ADC_DAC_CLK_CTL       =  0xbd;      // 0x01         ADC DAC clock control Register
sfr CMU_RESERVED2         =  0xc1;      // 0x01
sfr MCUPLL_CTL            =  0xb3;      // 0x01         MCU PLL Register
//--------------Bits Location------------------------------------------//
#define     VECLKCTL_VECLKDIV_e                                               2
#define     VECLKCTL_VECLKDIV_SHIFT                                           1
#define     VECLKCTL_VECLKDIV_MASK                                            (0x3<<1)
#define     VECLKCTL_VECLKSEL                                                 0

#define     AECLKCTL_AECLKDIV0                                                3
#define     AECLKCTL_AECLKDIV1_e                                              2
#define     AECLKCTL_AECLKDIV1_SHIFT                                          1
#define     AECLKCTL_AECLKDIV1_MASK                                           (0x3<<1)
#define     AECLKCTL_AECLKSEL                                                 0
///////////////////////////////////////////////////////////////////////////////////////////////
// DMA0/1/2 register
sfr DMAPriority           =  0x90;      // 0xe/0xf/0x10 DMA prority register
sfr DMA0IP                =  0x91;      // 0x0e         DMA0 interrupt pending register
sfr DMA0IE                =  0x92;      // 0x0e         DMA0 interrupt enable register
sfr DMA012DBG             =  0x93;      // 0xe/0xf,0x10 DMA012 debug register
sfr DMA0CTL0              =  0x94;      // 0x0e         DMA0 control register 0
sfr DMA0CTL1              =  0x95;      // 0x0e         DMA0 control register 1
sfr DMA0SADDR0H           =  0x97;      // 0x0e         DMA0 source address register 0 high byte
sfr DMA0SADDR0L           =  0x98;      // 0x0e         DMA0 source address register 0 low byte
sfr DMA0SADDR1H           =  0x99;      // 0x0e         DMA0 source address register 1 high byte
sfr DMA0SADDR1L           =  0x9a;      // 0x0e         DMA0 source address register 1 low byte
sfr DMA0SADDR2H           =  0x9b;      // 0x0e         DMA0 source address register 2 high byte
sfr DMA0SADDR2L           =  0x9c;      // 0x0e         DMA0 source address register 2 low byte
sfr DMA0DADDR0H           =  0x9d;      // 0x0e         DMA0 destination address register 0 high byte
sfr DMA0DADDR0L           =  0x9e;      // 0x0e         DMA0 destination address register 0 low byte
sfr DMA0DADDR1H           =  0xa2;      // 0x0e         DMA0 destination address register 1 high byte
sfr DMA0DADDR1L           =  0xa3;      // 0x0e         DMA0 destination address register 1 low byte
sfr DMA0FrameLenH         =  0xa4;      // 0x0e         DMA0 frame length register high byte
sfr DMA0FrameLenL         =  0xa5;      // 0x0e         DMA0 frame length register low byte
sfr DMA0LCDCFG            =  0xa6;      // 0x0e         DMA0 LCD configuration register
///////////////////////////////////////////////////////////////////////////////////////////////
sfr DMA1IP                =  0x91;      // 0x0f         DMA1 interrupt pending register
sfr DMA1IE                =  0x92;      // 0x0f         DMA1 interrupt enable register
sfr DMA1CTL0              =  0x94;      // 0x0f         DMA1 control register 0
sfr DMA1CTL1              =  0x95;      // 0x0f         DMA1 control register 1
sfr DMA1SADDR0H           =  0x97;      // 0x0f         DMA1 source address register 0 high byte
sfr DMA1SADDR0L           =  0x98;      // 0x0f         DMA1 source address register 0 low byte
sfr DMA1SADDR1H           =  0x99;      // 0x0f         DMA1 source address register 1 high byte
sfr DMA1SADDR1L           =  0x9a;      // 0x0f         DMA1 source address register 1 low byte
sfr DMA1SADDR2H           =  0x9b;      // 0x0f         DMA1 source address register 2 high byte
sfr DMA1SADDR2L           =  0x9c;      // 0x0f         DMA1 source address register 2 low byte
sfr DMA1DADDR0H           =  0x9d;      // 0x0f         DMA1 destination address register 0 high byte
sfr DMA1DADDR0L           =  0x9e;      // 0x0f         DMA1 destination address register 0 low byte
sfr DMA1DADDR1H           =  0xa2;      // 0x0f         DMA1 destination address register 1 high byte
sfr DMA1DADDR1L           =  0xa3;      // 0x0f         DMA1 destination address register 1 low byte
sfr DMA1FrameLenH         =  0xa4;      // 0x0f         DMA1 frame length register high byte
sfr DMA1FrameLenL         =  0xa5;      // 0x0f         DMA1 frame length register low byte
sfr DMA1LCDCFG            =  0xa6;      // 0x0f         DMA1 LCD configuration register
///////////////////////////////////////////////////////////////////////////////////////////////
sfr DMA2IP                =  0x91;      // 0x10         DMA2 interrupt pending register
sfr DMA2IE                =  0x92;      // 0x10         DMA2 interrupt enable register
sfr DMA2CTL0              =  0x94;      // 0x10         DMA2 control register 0
sfr DMA2CTL1              =  0x95;      // 0x10         DMA2 control register 1
sfr DMA2SADDR0H           =  0x97;      // 0x10         DMA2 source address register 0 high byte
sfr DMA2SADDR0L           =  0x98;      // 0x10         DMA2 source address register 0 low byte
sfr DMA2SADDR1H           =  0x99;      // 0x10         DMA2 source address register 1 high byte
sfr DMA2SADDR1L           =  0x9a;      // 0x10         DMA2 source address register 1 low byte
sfr DMA2SADDR2H           =  0x9b;      // 0x10         DMA2 source address register 2 high byte
sfr DMA2SADDR2L           =  0x9c;      // 0x10         DMA2 source address register 2 low byte
sfr DMA2DADDR0H           =  0x9d;      // 0x10         DMA2 destination address register 0 high byte
sfr DMA2DADDR0L           =  0x9e;      // 0x10         DMA2 destination address register 0 low byte
sfr DMA2DADDR1H           =  0xa2;      // 0x10         DMA2 destination address register 1 high byte
sfr DMA2DADDR1L           =  0xa3;      // 0x10         DMA2 destination address register 1 low byte
sfr DMA2FrameLenH         =  0xa4;      // 0x10         DMA2 frame length register high byte
sfr DMA2FrameLenL         =  0xa5;      // 0x10         DMA2 frame length register low byte
sfr DMA2LCDCFG            =  0xa6;      // 0x10         DMA2 LCD configuration register
///////////////////////////////////////////////////////////////////////////////////////////////
//#ifdef __A251__
// common register of DMA0/1/2
// 16 bit mode access
sfr DMAnSADDR0_16bit      =  0x97;      // 0xe/0xf/0x10 DMAn source address register 0 (16 bit mode access)
sfr DMAnSADDR1_16bit      =  0x99;      // 0xe/0xf/0x10 DMAn source address register 1 (16 bit mode access)
sfr DMAnSADDR2_16bit      =  0x9b;      // 0xe/0xf/0x10 DMAn source address register 2 (16 bit mode access)
sfr DMAnDADDR0_16bit      =  0x9d;      // 0xe/0xf/0x10 DMAn destination address register 0 (16 bit mode access)
sfr DMAnDADDR1_16bit      =  0xa2;      // 0xe/0xf/0x10 DMAn destination address register 1 (16 bit mode access)
sfr DMAnFrameLen_16bit    =  0xa4;      // 0xe/0xf/0x10 DMA0 frame length register (16 bit mode access)
//#endif
// 8 bit mode access
sfr DMAnIP                =  0x91;      // 0xe/0xf/0x10 DMAn interrupt pending register
sfr DMAnIE                =  0x92;      // 0xe/0xf/0x10 DMAn interrupt enable register
sfr DMAnDBG               =  0x93;      // 0xe/0xf/0x10 DMAn debug register
sfr DMAnCTL0              =  0x94;      // 0xe/0xf/0x10 DMAn control register 0
sfr DMAnCTL1              =  0x95;      // 0xe/0xf/0x10 DMAn control register 1
sfr DMAnSADDR0H           =  0x97;      // 0xe/0xf/0x10 DMAn source address register 0 high byte
sfr DMAnSADDR0L           =  0x98;      // 0xe/0xf/0x10 DMAn source address register 0 low byte
sfr DMAnSADDR1H           =  0x99;      // 0xe/0xf/0x10 DMAn source address register 1 high byte
sfr DMAnSADDR1L           =  0x9a;      // 0xe/0xf/0x10 DMAn source address register 1 low byte
sfr DMAnSADDR2H           =  0x9b;      // 0xe/0xf/0x10 DMAn source address register 2 high byte
sfr DMAnSADDR2L           =  0x9c;      // 0xe/0xf/0x10 DMAn source address register 2 low byte
sfr DMAnDADDR0H           =  0x9d;      // 0xe/0xf/0x10 DMAn destination address register 0 high byte
sfr DMAnDADDR0L           =  0x9e;      // 0xe/0xf/0x10 DMAn destination address register 0 low byte
sfr DMAnDADDR1H           =  0xa2;      // 0xe/0xf/0x10 DMAn destination address register 1 high byte
sfr DMAnDADDR1L           =  0xa3;      // 0xe/0xf/0x10 DMAn destination address register 1 low byte
sfr DMAnFrameLenH         =  0xa4;      // 0xe/0xf/0x10 DMAn frame length register high byte
sfr DMAnFrameLenL         =  0xa5;      // 0xe/0xf/0x10 DMAn frame length register low byte
sfr DMAnLCDCFG            =  0xa6;      // 0xe/0xf/0x10 DMAn LCD configuration register
////////////////////////////////////////////////////////////////////////////////////////////////
// Audio Codec TOP
sfr AuCodecCtl            =  0x90;      // 0x04         Audio Codec Control Register
sfr AuDebugLength         =  0x91;      // 0x04         Audio Codec Debug Length Register
sfr AuCodecDebug          =  0x92;      // 0x04         Audio Codec Debug Register
sfr AuCodecFIFOCtl        =  0x93;      // 0x04         Audio Codec FIFO control Register
sfr AuCodecFIFOData       =  0x94;      // 0x04         Audio Codec FIFO Data Register
sfr AuCodecDebug2         =  0x95;      // 0x04         Audio Codec Debug Register 2
sfr AuCodecDecStateH      =  0x97;      // 0x04         Decoding State Register High byte
sfr AuCodecDecStateL      =  0x98;      // 0x04         Decoding State Register Low byte
// MP3 Decoder
sfr MP3IE                 =  0xa9;      // 0x04         MP3 Decoder Interrupt Enable Register
sfr MP3IP                 =  0xaa;      // 0x04         MP3 Decoder Interrupt Pending Register
sfr MP3Ctl                =  0xab;      // 0x04         MP3 Decoder Control Register
sfr MP3HeaderInfo         =  0xac;      // 0x04         Header information register
sfr MP3BitRateH           =  0xad;      // 0x04         High byte of bit rate index register
sfr MP3BitRateL           =  0xae;      // 0x04         Low byte of bit rate index register
sfr MP3TTimeH             =  0xaf;      // 0x04         Total time hours register
sfr MP3TTimeM             =  0xb0;      // 0x04         Total time minutes register
sfr MP3TTimeS             =  0xb1;      // 0x04         Total time seconds register
sfr MP3CTimeH             =  0xb2;      // 0x04         Current time hours register
sfr MP3CTimeM             =  0xb3;      // 0x04         Current time minutes register
sfr MP3CTimeS             =  0xb4;      // 0x04         Current time seconds register
sfr MP3FrameNumH          =  0xb5;      // 0x04         High byte of Block Number register
sfr MP3FrameNumM          =  0xb6;      // 0x04         Middle byte of Block Number register
sfr MP3FrameNumL          =  0xb7;      // 0x04         Low byte of Block Number register
sfr MP3FileLen3           =  0xb8;      // 0x04         Byte 3 of File Length register
sfr MP3FileLen2           =  0xb9;      // 0x04         Byte 2 of File Length register
sfr MP3FileLen1           =  0xba;      // 0x04         Byte 1 of File Length register
sfr MP3HeaderSynCtl       =  0xbb;      // 0x04         MP3 header synchronization control register
sfr SynHeaderData1        =  0xbc;      // 0x04         MP3 header synchronization Data register 1
sfr SynHeaderData2        =  0xbd;      // 0x04         MP3 header synchronization Data register 2
// WMA Decoder
sfr WMAIE                 =  0xa9;      // 0x04         WMA Decoder Interrupt Enable Register
sfr WMAIP                 =  0xaa;      // 0x04         WMA Decoder Interrupt Pending Register
sfr WMACtl                =  0xab;      // 0x04         WMA Decoder Control Register
sfr WMAHeaderInfo         =  0xac;      // 0x04         Header information register
sfr WMABitRateH           =  0xad;      // 0x04         High byte of bit rate index register
sfr WMABitRateL           =  0xae;      // 0x04         Low byte of bit rate index register
sfr WMATTimeH             =  0xaf;      // 0x04         Total time hours register
sfr WMATTimeM             =  0xb0;      // 0x04         Total time minutes register
sfr WMATTimeS             =  0xb1;      // 0x04         Total time seconds register
sfr WMACTimeH             =  0xb2;      // 0x04         Current time hours register
sfr WMACTimeM             =  0xb3;      // 0x04         Current time minutes register
sfr WMACTimeS             =  0xb4;      // 0x04         Current time seconds register
sfr WMAPackNumH           =  0xb5;      // 0x04         High byte of Packet Number register
sfr WMAPackNumM           =  0xb6;      // 0x04         Middle byte of Packet Number register
sfr WMAPackNumL           =  0xb7;      // 0x04         Low byte of Packet Number register
sfr WMAPackSizeH          =  0xb8;      // 0x04         High byte of Packet Size register
sfr WMAPackSizeM          =  0xb9;      // 0x04         Middle byte of Packet Size register
sfr WMAPackSizeL          =  0xba;      // 0x04         Low byte of Packet Size register
sfr VitrualAddrH          =  0xbb;      // 0x04         High byte of virtual address of current frame
sfr VitrualAddrL          =  0xbc;      // 0x04         Low byte of virtual address of current frame
sfr PhysicalAddrH         =  0xbd;      // 0x04         High byte of physical address of current frame
sfr PhysicalAddrL         =  0xbf;      // 0x04         Low byte of physical address of current frame
sfr DiscardLen            =  0xc1;      // 0x04         The discard length of current subframe
// WAV Decoder
sfr WAVIE                 =  0xa9;      // 0x04         WAV Decoder Interrupt Enable Register
sfr WAVIP                 =  0xaa;      // 0x04         WAV Decoder Interrupt Pending Register
sfr WAVCtl                =  0xab;      // 0x04         WAV Decoder Control Register
sfr WAVHeaderInfo         =  0xac;      // 0x04         Header information register
sfr WAVBitRateH           =  0xad;      // 0x04         High byte of bit rate index register
sfr WAVBitRateL           =  0xae;      // 0x04         Low byte of bit rate index register
sfr WAVTTimeH             =  0xaf;      // 0x04         Total time hours register
sfr WAVTTimeM             =  0xb0;      // 0x04         Total time minutes register
sfr WAVTTimeS             =  0xb1;      // 0x04         Total time seconds register
sfr WAVCTimeH             =  0xb2;      // 0x04         Current time hours register
sfr WAVCTimeM             =  0xb3;      // 0x04         Current time minutes register
sfr WAVCTimeS             =  0xb4;      // 0x04         Current time seconds register
sfr WAVBlockNumH          =  0xb5;      // 0x04         High byte of Block Number register
sfr WAVBlockNumM          =  0xb6;      // 0x04         Middle byte of Block Number register
sfr WAVBlockNumL          =  0xb7;      // 0x04         Low byte of Block Number register
// WAV Encoder
sfr WAVEncIE              =  0xa9;      // 0x04         WAV Encoder Interrupt Enable Register
sfr WAVEncIP              =  0xaa;      // 0x04         WAV Encoder Interrupt Pending Register
sfr WAVEncCtl             =  0xab;      // 0x04         WAV Encoder Control Register
// FLAC Decoder
sfr FLACIE                =  0xa9;      // 0x04         FLAC Decoder Interrupt Enable Register
sfr FLACIP                =  0xaa;      // 0x04         FLAC Decoder Interrupt Pending Register
sfr FLACCtl               =  0xab;      // 0x04         FLAC Decoder Control Register
sfr FLACHeaderInfo1       =  0xac;      // 0x04         Header information register 1
sfr FLACHeaderInfo2       =  0xad;      // 0x04         Header information register 2
sfr METADATAInfo          =  0xae;      // 0x04         METADATA information register
sfr FLACTTimeH            =  0xaf;      // 0x04         Total time hours register
sfr FLACTTimeM            =  0xb0;      // 0x04         Total time minutes register
sfr FLACTTimeS            =  0xb1;      // 0x04         Total time seconds register
sfr FLACCTimeH            =  0xb2;      // 0x04         Current time hours register
sfr FLACCTimeM            =  0xb3;      // 0x04         Current time minutes register
sfr FLACCTimeS            =  0xb4;      // 0x04         Current time seconds register
sfr FLACErrInfo           =  0xb5;      // 0x04         FLAC Decoding Error Information register
sfr CurrentPos3           =  0xb6;      // 0x04         Current Position byte 3 register
sfr CurrentPos2           =  0xb7;      // 0x04         Current Position byte 2 register
sfr CurrentPos1           =  0xb8;      // 0x04         Current Position byte 1 register
sfr CurrentPos0           =  0xb9;      // 0x04         Current Position byte 0 register
sfr FLACCFrameNum3        =  0xba;      // 0x04         FLAC Current Frame Number byte3 register
sfr FLACCFrameNum2        =  0xbb;      // 0x04         FLAC Current Frame Number byte2 register
sfr FLACCFrameNum1        =  0xbc;      // 0x04         FLAC Current Frame Number byte1 register
sfr FLACCFrameNum0        =  0xbd;      // 0x04         FLAC Current Frame Number byte0 register
sfr FLACHeaderSynCtl      =  0xbf;      // 0x04         FLAC synchronization control register
// APE Decoder
sfr APEIE                 =  0xa9;      // 0x04         APE Decoder Interrupt Enable Register
sfr APEIP                 =  0xaa;      // 0x04         APE Decoder Interrupt Pending Register
sfr APECtl                =  0xab;      // 0x04         APE Decoder Control Register
sfr APEHeaderInfo1        =  0xac;      // 0x04         Header information register 1
sfr APEHeaderInfo2        =  0xad;      // 0x04         Header information register 2
sfr APETTimeH             =  0xaf;      // 0x04         Total time hours register
sfr APETTimeM             =  0xb0;      // 0x04         Total time minutes register
sfr APETTimeS             =  0xb1;      // 0x04         Total time seconds register
sfr APECTimeH             =  0xb2;      // 0x04         Current time hours register
sfr APECTimeM             =  0xb3;      // 0x04         Current time minutes register
sfr APECTimeS             =  0xb4;      // 0x04         Current time seconds register
sfr APEErrInfo            =  0xb5;      // 0x04         APE Decoding Error Information register
sfr APECFrameNum3         =  0xba;      // 0x04         APE Current Frame Number byte3 register
sfr APECFrameNum2         =  0xbb;      // 0x04         APE Current Frame Number byte2 register
sfr APECFrameNum1         =  0xbc;      // 0x04         APE Current Frame Number byte1 register
sfr APECFrameNum0         =  0xbd;      // 0x04         APE Current Frame Number byte0 register
// Soft Decoder
sfr SoftIE                =  0xa9;      // 0x04         Software Decoder Interrupt Enable Register
sfr SoftIP                =  0xaa;      // 0x04         Software Decoder Interrupt Pending Register
sfr SoftCtl               =  0xab;      // 0x04         Software Decoder Control Register
sfr FrameLen1             =  0xb8;      // 0x04         Frame Length Register 1
sfr FrameLen0             =  0xb9;      // 0x04         Frame Length Register 0
// AMV audio decoder
sfr AmvAudioIP            =  0xaa;      // 0x04         Amv Audio Decoder Pending Register
sfr AmvAudioCtl           =  0xab;      // 0x04         Amv Audio Decoder Control Register
sfr PrevDataH             =  0xb5;      // 0x04         High byte of previous data register
sfr PrevDataL             =  0xb6;      // 0x04         Low byte of previous data register
sfr AmvAuIndex            =  0xb7;      // 0x04         Index register
sfr AmvFrameLenH          =  0xb8;      // 0x04         High byte of frame length register
sfr AmvFrameLenL          =  0xb9;      // 0x04         Low byte of frame length register
// Post Processor
sfr BEPCtl1               =  0xc2;      // 0x04         Back End Processor Control Register 1
sfr BEPCtl2               =  0xc3;      // 0x04         Back End Processor Control Register 2
sfr BEPCtl3               =  0xc4;      // 0x04         Back End Processor Control Register 3
sfr BEPCtl4               =  0xc5;      // 0x04         Back End Processor Control Register 4
sfr BEPCtl5               =  0xc6;      // 0x04         Back End Processor Control Register 5
sfr GlobalGainH           =  0xc7;      // 0x04         Global Gain High Byte Register
sfr GlobalGainM           =  0xc8;      // 0x04         Global Gain High Byte Register
sfr GlobalGainL           =  0xc9;      // 0x04         Global Gain High Byte Register
sfr CurrentEnergy         =  0xca;      // 0x04         Current Energy Register
// Karaoke Processor
sfr KaraokeCtl            =  0xcb;      // 0x04         Karaoke Control Register
sfr LCHGain               =  0xcc;      // 0x04         Left channel gain
sfr RCHGain               =  0xcd;      // 0x04         Right channel gain
sfr LPFGain               =  0xce;      // 0x04         Low pass filter gain
sfr HPFGain               =  0xcf;      // 0x04         High pass filter gain
// EQ Processor
sfr EQCoe0                =  0xcc;      // 0x04         NO.0 Equalizer Coefficients Register
sfr EQCoe1                =  0xcd;      // 0x04         NO.1 Equalizer Coefficients Register
sfr EQCoe2                =  0xce;      // 0x04         NO.2 Equalizer Coefficients Register
sfr EQCoe3                =  0xcf;      // 0x04         NO.3 Equalizer Coefficients Register
sfr EQCoe4                =  0xd2;      // 0x04         NO.4 Equalizer Coefficients Register
sfr EQCoe5                =  0xd3;      // 0x04         NO.5 Equalizer Coefficients Register
sfr EQCoe6                =  0xd4;      // 0x04         NO.6 Equalizer Coefficients Register
// SRS Processor
sfr SRSCtl                =  0xcc;      // 0x04         SRS Control Register
sfr SRSCfg1               =  0xcd;      // 0x04         SRS config register 1
sfr SRSCfg2               =  0xce;      // 0x04         SRS config register 2
sfr SRSCfg3               =  0xcf;      // 0x04         SRS config register 3
sfr SRSCfg4               =  0xd2;      // 0x04         SRS config register 4
// VPS Processor
sfr VPSCtl                =  0xcc;      // 0x04         Variable Pitch Shifting Control Register
sfr SequenceSize          =  0xcd;      // 0x04         Sequence size
sfr OverlapSize           =  0xce;      // 0x04         Overlapped sample size
sfr SeekWindowSize        =  0xcf;      // 0x04         Seek window size
// audio ip common registers
sfr AuCodecIE             =  0xa9;      // 0x04         Audio Codec Interrupt Enable Register
sfr AuCodecIP             =  0xaa;      // 0x04         Audio Codec Interrupt Pending Register
sfr AuDecEncCtl           =  0xab;      // 0x04         Audio decoding or encoding Control Register
sfr AuCodecHeaderInfo     =  0xac;      // 0x04         Header information register
sfr AuCodecBirRateH       =  0xad;      // 0x04         High byte of bit rate index register
sfr AuCodecBirRateL       =  0xae;      // 0x04         Low byte of bit rate index register
sfr AuCodecTTimeH         =  0xaf;      // 0x04         Total time hours register
sfr AuCodecTTimeM         =  0xb0;      // 0x04         Total time minutes register
sfr AuCodecTTimeS         =  0xb1;      // 0x04         Total time seconds register
sfr AuCodecCTimeH         =  0xb2;      // 0x04         Current time hours register
sfr AuCodecCTimeM         =  0xb3;      // 0x04         Current time minutes register
sfr AuCodecCTimeS         =  0xb4;      // 0x04         Current time seconds register
sfr CurrentFrameNum3      =  0xba;      // 0x04         FLAC/APE Current Frame Number byte3 register
sfr CurrentFrameNum2      =  0xbb;      // 0x04         FLAC/APE Current Frame Number byte2 register
sfr CurrentFrameNum1      =  0xbc;      // 0x04         FLAC/APE Current Frame Number byte1 register
sfr CurrentFrameNum0      =  0xbd;      // 0x04         FLAC/APE Current Frame Number byte0 register
// 3D Speaker
sfr SPK3DCTL0             =  0x8d;      // 0x04         3D-Speaker Control Register 0
sfr SPK3DCTL1             =  0x99;      // 0x04         3D-Speaker Control Register 1
sfr SPK3DHPF_A1_H         =  0x9a;      // 0x04         Upper Byte of Coefficient A1 of HPF Register
sfr SPK3DHPF_A1_L         =  0x9b;      // 0x04         Lower Byte of Coefficient A1 of HPF Register
sfr SPK3DHPF_A2_H         =  0x9c;      // 0x04         Upper Byte of Coefficient A2 of HPF Register
sfr SPK3DHPF_A2_L         =  0x9d;      // 0x04         Lower Byte of Coefficient A2 of HPF Register
sfr SPK3DHPF_H0_H         =  0x9e;      // 0x04         Upper Byte of Coefficient H0 of HPF Register
sfr SPK3DHPF_H0_L         =  0x9f;      // 0x04         Lower Byte of Coefficient H0 of HPF Register
sfr SPK3DLPF_A1_H         =  0xa2;      // 0x04         Upper Byte of Coefficient A1 of LPF Register
sfr SPK3DLPF_A1_L         =  0xa3;      // 0x04         Lower Byte of Coefficient A1 of LPF Register
sfr SPK3DLPF_A2_H         =  0xa4;      // 0x04         Upper Byte of Coefficient A2 of LPF Register
sfr SPK3DLPF_A2_L         =  0xa5;      // 0x04         Lower Byte of Coefficient A2 of LPF Register
sfr SPK3DLPF_H0_H         =  0xa6;      // 0x04         Upper Byte of Coefficient H0 of LPF Register
sfr SPK3DLPF_H0_L         =  0xa7;      // 0x04         Lower Byte of Coefficient H0 of LPF Register
// BASSBACK
sfr BASSBACKCTL0          =  0x8b;      // 0x04         BassBack Control Register 0
sfr BASSBACKCTL1          =  0x8c;      // 0x04         BassBack Control Register 1
//--------------Bits Location------------------------------------------//
#define     AuCodecCtl_Reset                                                  7
#define     AuCodecCtl_Mode_e                                                 6
#define     AuCodecCtl_Mode_SHIFT                                             4
#define     AuCodecCtl_Mode_MASK                                              (0x7<<4)
#define     AuCodecCtl_EnableAllClk                                           3
#define     AuCodecCtl_AHB_DMA_SEL                                            0

#define     AuDebugLength_PcmLen_e                                            12
#define     AuDebugLength_PcmLen_SHIFT                                        0
#define     AuDebugLength_PcmLen_MASK                                         (0x1FFF<<0)

#define     AuCodecFIFOCtl_DRQLevel_e                                         4
#define     AuCodecFIFOCtl_DRQLevel_SHIFT                                     1
#define     AuCodecFIFOCtl_DRQLevel_MASK                                      (0xF<<1)

#define     AuCodecFIFOData_FIFOData_e                                        31
#define     AuCodecFIFOData_FIFOData_SHIFT                                    0
#define     AuCodecFIFOData_FIFOData_MASK                                     (0xFFFFFFFF<<0)

#define     AuCodecDecState_State_e                                           15
#define     AuCodecDecState_State_SHIFT                                       0
#define     AuCodecDecState_State_MASK                                        (0xFFFF<<0)

#define     MP3IE_LackPcmEn                                                   7
#define     MP3IE_EnergyEn                                                    6
#define     MP3IE_WordIntEn                                                   5
#define     MP3IE_InfoChangeEn2                                               4
#define     MP3IE_InfoChangeEn1                                               3
#define     MP3IE_CRCErrIntEn                                                 2
#define     MP3IE_InfoRdyIntEn                                                1
#define     MP3IE_PCMRdyIntEn                                                 0

#define     MP3IP_LackIP                                                      7
#define     MP3IP_EnergyIP                                                    6
#define     MP3IP_WordIP                                                      5
#define     MP3IP_InfoChangeIP2                                               4
#define     MP3IP_InfoChangeIP1                                               3
#define     MP3IP_CRCErrIP                                                    2
#define     MP3IP_InfoRdyIP                                                   1
#define     MP3IP_PCMRdyIP                                                    0

#define     MP3Ctl_NewFile                                                    7
#define     MP3Ctl_En                                                         6
#define     MP3Ctl_FFEn                                                       5
#define     MP3Ctl_REVEn                                                      4
#define     MP3Ctl_CRCSkipEn                                                  3
#define     MP3Ctl_BKPlayEn                                                   1

#define     MP3HeaderInfo_SampFrq_e                                           7
#define     MP3HeaderInfo_SampFrq_SHIFT                                       4
#define     MP3HeaderInfo_SampFrq_MASK                                        (0xF<<4)
#define     MP3HeaderInfo_Layer_e                                             3
#define     MP3HeaderInfo_Layer_SHIFT                                         2
#define     MP3HeaderInfo_Layer_MASK                                          (0x3<<2)
#define     MP3HeaderInfo_Errprotect                                          1
#define     MP3HeaderInfo_AudioMode                                           0

#define     MP3BitRate_BitRateIdx3_e                                          15
#define     MP3BitRate_BitRateIdx3_SHIFT                                      12
#define     MP3BitRate_BitRateIdx3_MASK                                       (0xF<<12)
#define     MP3BitRate_BitRateIdx2_e                                          11
#define     MP3BitRate_BitRateIdx2_SHIFT                                      8
#define     MP3BitRate_BitRateIdx2_MASK                                       (0xF<<8)
#define     MP3BitRate_BitRateIdx1_e                                          7
#define     MP3BitRate_BitRateIdx1_SHIFT                                      4
#define     MP3BitRate_BitRateIdx1_MASK                                       (0xF<<4)
#define     MP3BitRate_BitRateIdx0_e                                          3
#define     MP3BitRate_BitRateIdx0_SHIFT                                      0
#define     MP3BitRate_BitRateIdx0_MASK                                       (0xF<<0)

#define     MP3TTimeH_XingTag                                                 7
#define     MP3TTimeH_TTimeH_e                                                5
#define     MP3TTimeH_TTimeH_SHIFT                                            0
#define     MP3TTimeH_TTimeH_MASK                                             (0x3F<<0)

#define     MP3TTimeM_TTimeM_e                                                5
#define     MP3TTimeM_TTimeM_SHIFT                                            0
#define     MP3TTimeM_TTimeM_MASK                                             (0x3F<<0)

#define     MP3TTimeS_TTimeS_e                                                5
#define     MP3TTimeS_TTimeS_SHIFT                                            0
#define     MP3TTimeS_TTimeS_MASK                                             (0x3F<<0)

#define     MP3CTimeH_CTimeH_e                                                5
#define     MP3CTimeH_CTimeH_SHIFT                                            0
#define     MP3CTimeH_CTimeH_MASK                                             (0x3F<<0)

#define     MP3CTimeM_CTimeM_e                                                5
#define     MP3CTimeM_CTimeM_SHIFT                                            0
#define     MP3CTimeM_CTimeM_MASK                                             (0x3F<<0)

#define     MP3CTimeS_CTimeS_e                                                5
#define     MP3CTimeS_CTimeS_SHIFT                                            0
#define     MP3CTimeS_CTimeS_MASK                                             (0x3F<<0)

#define     MP3FrameNum_FrameNum_e                                            23
#define     MP3FrameNum_FrameNum_SHIFT                                        0
#define     MP3FrameNum_FrameNum_MASK                                         (0xFFFFFF<<0)

#define     MP3FileLen_FileLength_e                                           31
#define     MP3FileLen_FileLength_SHIFT                                       8
#define     MP3FileLen_FileLength_MASK                                        (0xFFFFFF<<8)

#define     MP3HeaderSynCtl_IDEn                                              7
#define     MP3HeaderSynCtl_LayerEn                                           6
#define     MP3HeaderSynCtl_ProtectionBitEn                                   5
#define     MP3HeaderSynCtl_SampFreqEn                                        4
#define     MP3HeaderSynCtl_ModeEn                                            3
#define     MP3HeaderSynCtl_CopyrightEn                                       2
#define     MP3HeaderSynCtl_OriginalEn                                        1
#define     MP3HeaderSynCtl_EmphasisEn                                        0

#define     SynHeaderData_copyright                                           15
#define     SynHeaderData_original_copy                                       14
#define     SynHeaderData_emphasis_e                                          13
#define     SynHeaderData_emphasis_SHIFT                                      12
#define     SynHeaderData_emphasis_MASK                                       (0x3<<12)
#define     SynHeaderData_Copyright_r                                         11
#define     SynHeaderData_Original_copy_r                                     10
#define     SynHeaderData_Emphasis_r_e                                        9
#define     SynHeaderData_Emphasis_r_SHIFT                                    8
#define     SynHeaderData_Emphasis_r_MASK                                     (0x3<<8)
#define     SynHeaderData_SampFrq_e                                           7
#define     SynHeaderData_SampFrq_SHIFT                                       4
#define     SynHeaderData_SampFrq_MASK                                        (0xF<<4)
#define     SynHeaderData_Layer_e                                             3
#define     SynHeaderData_Layer_SHIFT                                         2
#define     SynHeaderData_Layer_MASK                                          (0x3<<2)
#define     SynHeaderData_Errprotect                                          1
#define     SynHeaderData_AudioMode                                           0

#define     WMAIE_LackIntEn                                                   7
#define     WMAIE_EnergyIntEn                                                 6
#define     WMAIE_WordIntEn                                                   5
#define     WMAIE_DRMInfoIntEn                                                3
#define     WMAIE_ErrIntEn                                                    2
#define     WMAIE_InfoRdyIntEn                                                1
#define     WMAIE_PCMRdyIntEn                                                 0

#define     WMAIP_LackIP                                                      7
#define     WMAIP_EnergyIP                                                    6
#define     WMAIP_WordIP                                                      5
#define     WMAIP_DRMInfoIP                                                   3
#define     WMAIP_ErrIP                                                       2
#define     WMAIP_InfoRdyIP                                                   1
#define     WMAIP_PCMRdyIP                                                    0

#define     WMACtl_NewFile                                                    7
#define     WMACtl_En                                                         6
#define     WMACtl_FF_REV_En                                                  5
#define     WMACtl_REVEn                                                      4
#define     WMACtl_REV_SEL                                                    1

#define     WMAHeaderInfo_SampFrq_e                                           7
#define     WMAHeaderInfo_SampFrq_SHIFT                                       4
#define     WMAHeaderInfo_SampFrq_MASK                                        (0xF<<4)
#define     WMAHeaderInfo_AllowSuper                                          3
#define     WMAHeaderInfo_WeightMode                                          2
#define     WMAHeaderInfo_NoiseSub                                            1
#define     WMAHeaderInfo_AudioMode                                           0

#define     WMABitRate_BitRateIdx3_e                                          15
#define     WMABitRate_BitRateIdx3_SHIFT                                      12
#define     WMABitRate_BitRateIdx3_MASK                                       (0xF<<12)
#define     WMABitRate_BitRateIdx2_e                                          11
#define     WMABitRate_BitRateIdx2_SHIFT                                      8
#define     WMABitRate_BitRateIdx2_MASK                                       (0xF<<8)
#define     WMABitRate_BitRateIdx1_e                                          7
#define     WMABitRate_BitRateIdx1_SHIFT                                      4
#define     WMABitRate_BitRateIdx1_MASK                                       (0xF<<4)
#define     WMABitRate_BitRateIdx0_e                                          3
#define     WMABitRate_BitRateIdx0_SHIFT                                      0
#define     WMABitRate_BitRateIdx0_MASK                                       (0xF<<0)

#define     WMATTimeH_TTimeH_e                                                5
#define     WMATTimeH_TTimeH_SHIFT                                            0
#define     WMATTimeH_TTimeH_MASK                                             (0x3F<<0)

#define     WMATTimeM_TTimeM_e                                                5
#define     WMATTimeM_TTimeM_SHIFT                                            0
#define     WMATTimeM_TTimeM_MASK                                             (0x3F<<0)

#define     WMATTimeS_TTimeS_e                                                5
#define     WMATTimeS_TTimeS_SHIFT                                            0
#define     WMATTimeS_TTimeS_MASK                                             (0x3F<<0)

#define     WMACTimeH_CTimeH_e                                                5
#define     WMACTimeH_CTimeH_SHIFT                                            0
#define     WMACTimeH_CTimeH_MASK                                             (0x3F<<0)

#define     WMACTimeM_CTimeM_e                                                5
#define     WMACTimeM_CTimeM_SHIFT                                            0
#define     WMACTimeM_CTimeM_MASK                                             (0x3F<<0)

#define     WMACTimeS_CTimeS_e                                                5
#define     WMACTimeS_CTimeS_SHIFT                                            0
#define     WMACTimeS_CTimeS_MASK                                             (0x3F<<0)

#define     WMAPackNum_PacketNum_e                                            23
#define     WMAPackNum_PacketNum_SHIFT                                        0
#define     WMAPackNum_PacketNum_MASK                                         (0xFFFFFF<<0)

#define     WMAPackSize_PacketSize_e                                          23
#define     WMAPackSize_PacketSize_SHIFT                                      0
#define     WMAPackSize_PacketSize_MASK                                       (0xFFFFFF<<0)

#define     WAVIE_LackIntEn                                                   7
#define     WAVIE_EnergyIntEn                                                 6
#define     WAVIE_WordIntEn                                                   5
#define     WAVIE_ErrIntEn                                                    2
#define     WAVIE_InfoRdyIntEn                                                1
#define     WAVIE_PCMRdyIntEn                                                 0

#define     WAVIP_LackIP                                                      7
#define     WAVIP_EnergyIP                                                    6
#define     WAVIP_WordIP                                                      5
#define     WAVIP_ErrIP                                                       2
#define     WAVIP_InfoRdyIP                                                   1
#define     WAVIP_PCMRdyIP                                                    0

#define     WAVCtl_NewFile                                                    7
#define     WAVCtl_En                                                         6
#define     WAVCtl_FF_REV_En                                                  5
#define     WAVCtl_REVEn                                                      4

#define     WAVHeaderInfo_SampFrq_e                                           7
#define     WAVHeaderInfo_SampFrq_SHIFT                                       4
#define     WAVHeaderInfo_SampFrq_MASK                                        (0xF<<4)
#define     WAVHeaderInfo_Format                                              1
#define     WAVHeaderInfo_AudioMode                                           0

#define     WAVBitRate_BitRateIdx3_e                                          15
#define     WAVBitRate_BitRateIdx3_SHIFT                                      12
#define     WAVBitRate_BitRateIdx3_MASK                                       (0xF<<12)
#define     WAVBitRate_BitRateIdx2_e                                          11
#define     WAVBitRate_BitRateIdx2_SHIFT                                      8
#define     WAVBitRate_BitRateIdx2_MASK                                       (0xF<<8)
#define     WAVBitRate_BitRateIdx1_e                                          7
#define     WAVBitRate_BitRateIdx1_SHIFT                                      4
#define     WAVBitRate_BitRateIdx1_MASK                                       (0xF<<4)
#define     WAVBitRate_BitRateIdx0_e                                          3
#define     WAVBitRate_BitRateIdx0_SHIFT                                      0
#define     WAVBitRate_BitRateIdx0_MASK                                       (0xF<<0)

#define     WAVTTimeH_TTimeH_e                                                5
#define     WAVTTimeH_TTimeH_SHIFT                                            0
#define     WAVTTimeH_TTimeH_MASK                                             (0x3F<<0)

#define     WAVTTimeM_TTimeM_e                                                5
#define     WAVTTimeM_TTimeM_SHIFT                                            0
#define     WAVTTimeM_TTimeM_MASK                                             (0x3F<<0)

#define     WAVTTimeS_TTimeS_e                                                5
#define     WAVTTimeS_TTimeS_SHIFT                                            0
#define     WAVTTimeS_TTimeS_MASK                                             (0x3F<<0)

#define     WAVCTimeH_CTimeH_e                                                5
#define     WAVCTimeH_CTimeH_SHIFT                                            0
#define     WAVCTimeH_CTimeH_MASK                                             (0x3F<<0)

#define     WAVCTimeM_CTimeM_e                                                5
#define     WAVCTimeM_CTimeM_SHIFT                                            0
#define     WAVCTimeM_CTimeM_MASK                                             (0x3F<<0)

#define     WAVCTimeS_CTimeS_e                                                5
#define     WAVCTimeS_CTimeS_SHIFT                                            0
#define     WAVCTimeS_CTimeS_MASK                                             (0x3F<<0)

#define     WAVBlockNum_BlockNum_e                                            23
#define     WAVBlockNum_BlockNum_SHIFT                                        0
#define     WAVBlockNum_BlockNum_MASK                                         (0xFFFFFF<<0)

#define     FLACIE_LackIntEn                                                  7
#define     FLACIE_EnergyIntEn                                                6
#define     FLACIE_WordIntEn                                                  5
#define     FLACIE_MetadataInfoEn                                             3
#define     FLACIE_ErrIntEn                                                   2
#define     FLACIE_InfoRdyIntEn                                               1
#define     FLACIE_PCMRdyIntEn                                                0

#define     FLACIP_LackIP                                                     7
#define     FLACIP_EnergyIP                                                   6
#define     FLACIP_WordIP                                                     5
#define     FLACIP_MetadataInfoIP                                             3
#define     FLACIP_ErrIP                                                      2
#define     FLACIP_InfoRdyIP                                                  1
#define     FLACIP_PCMRdyIP                                                   0

#define     FLACCtl_NewFile                                                   7
#define     FLACCtl_En                                                        6
#define     FLACCtl_FF_REV_En                                                 5
#define     FLACCtl_CRC16En                                                   0

#define     FLACHeaderInfo2_BlockSize_e                                       7
#define     FLACHeaderInfo2_BlockSize_SHIFT                                   4
#define     FLACHeaderInfo2_BlockSize_MASK                                    (0xF<<4)
#define     FLACHeaderInfo2_BlockingStrategy                                  3
#define     FLACHeaderInfo2_SampleSize_e                                      2
#define     FLACHeaderInfo2_SampleSize_SHIFT                                  0
#define     FLACHeaderInfo2_SampleSize_MASK                                   (0x7<<0)
#define     FLACHeaderInfo1_SampFrq_e                                         7
#define     FLACHeaderInfo1_SampFrq_SHIFT                                     4
#define     FLACHeaderInfo1_SampFrq_MASK                                      (0xF<<4)
#define     FLACHeaderInfo1_Channel_assignment_e                              3
#define     FLACHeaderInfo1_Channel_assignment_SHIFT                          0
#define     FLACHeaderInfo1_Channel_assignment_MASK                           (0xF<<0)

#define     METADATAInfo_ID3V2                                                7
#define     METADATAInfo_PICTURE                                              6
#define     METADATAInfo_CUESHEET                                             5
#define     METADATAInfo_VORBIS                                               4
#define     METADATAInfo_SEEKTABLE                                            3
#define     METADATAInfo_APPLICATION                                          2
#define     METADATAInfo_PADDING                                              1
#define     METADATAInfo_STREAMINFO                                           0

#define     FLACTTimeH_TTimeH_e                                               5
#define     FLACTTimeH_TTimeH_SHIFT                                           0
#define     FLACTTimeH_TTimeH_MASK                                            (0x3F<<0)

#define     FLACTTimeM_TTimeM_e                                               5
#define     FLACTTimeM_TTimeM_SHIFT                                           0
#define     FLACTTimeM_TTimeM_MASK                                            (0x3F<<0)

#define     FLACTTimeS_TTimeS_e                                               5
#define     FLACTTimeS_TTimeS_SHIFT                                           0
#define     FLACTTimeS_TTimeS_MASK                                            (0x3F<<0)

#define     FLACCTimeH_CTimeH_e                                               5
#define     FLACCTimeH_CTimeH_SHIFT                                           0
#define     FLACCTimeH_CTimeH_MASK                                            (0x3F<<0)

#define     FLACCTimeM_CTimeM_e                                               5
#define     FLACCTimeM_CTimeM_SHIFT                                           0
#define     FLACCTimeM_CTimeM_MASK                                            (0x3F<<0)

#define     FLACCTimeS_CTimeS_e                                               5
#define     FLACCTimeS_CTimeS_SHIFT                                           0
#define     FLACCTimeS_CTimeS_MASK                                            (0x3F<<0)

#define     FLACErrInfo_CurrentSampleNum_bit35to32_e                          7
#define     FLACErrInfo_CurrentSampleNum_bit35to32_SHIFT                      4
#define     FLACErrInfo_CurrentSampleNum_bit35to32_MASK                       (0xF<<4)
#define     FLACErrInfo_ErrInfo_e                                             3
#define     FLACErrInfo_ErrInfo_SHIFT                                         0
#define     FLACErrInfo_ErrInfo_MASK                                          (0xF<<0)

#define     CurrentPos_CurrentPos_e                                           31
#define     CurrentPos_CurrentPos_SHIFT                                       0
#define     CurrentPos_CurrentPos_MASK                                        (0xFFFFFFFF<<0)

#define     FLACCFrameNum_CurrentSampleNum_bit31                              31
#define     FLACCFrameNum_CurrentFrame_bit30to0_e                             30
#define     FLACCFrameNum_CurrentFrame_bit30to0_SHIFT                         0
#define     FLACCFrameNum_CurrentFrame_bit30to0_MASK                          (0x7FFFFFFF<<0)

#define     FLACHeaderSynCtl_BlockingstrategyEn                               7
#define     FLACHeaderSynCtl_BlockSizeEn                                      6
#define     FLACHeaderSynCtl_SampleRateEn                                     5
#define     FLACHeaderSynCtl_ChannelNumEn                                     4
#define     FLACHeaderSynCtl_SampleSizeEn                                     3
#define     FLACHeaderSynCtl_SampleNumEn                                      2

#define     APEIE_LackIntEn                                                   7
#define     APEIE_EnergyIntEn                                                 6
#define     APEIE_WordIntEn                                                   5
#define     APEIE_ErrIntEn                                                    2
#define     APEIE_InfoRdyIntEn                                                1
#define     APEIE_PCMRdyIntEn                                                 0

#define     APEIP_LackIP                                                      7
#define     APEIP_EnergyIP                                                    6
#define     APEIP_WordIP                                                      5
#define     APEIP_ErrIP                                                       2
#define     APEIP_InfoRdyIP                                                   1
#define     APEIP_PCMRdyIP                                                    0

#define     APECtl_NewFile                                                    7
#define     APECtl_En                                                         6
#define     APECtl_SaturationPecision                                         5
#define     APECtl_PeakLevelEn                                                4
#define     APECtl_NewFrame                                                   3
#define     APECtl_SkipNum_e                                                  2
#define     APECtl_SkipNum_SHIFT                                              1
#define     APECtl_SkipNum_MASK                                               (0x3<<1)
#define     APECtl_CRC32En                                                    0

#define     APEHeaderInfo1_SampleRate_e                                       7
#define     APEHeaderInfo1_SampleRate_SHIFT                                   4
#define     APEHeaderInfo1_SampleRate_MASK                                    (0xF<<4)
#define     APEHeaderInfo1_nCompressionLevel_e                                3
#define     APEHeaderInfo1_nCompressionLevel_SHIFT                            1
#define     APEHeaderInfo1_nCompressionLevel_MASK                             (0x7<<1)

#define     APEHeaderInfo2_seek_element                                       4
#define     APEHeaderInfo2_nPeakLevel                                         3
#define     APEHeaderInfo2_SampleSize_e                                       2
#define     APEHeaderInfo2_SampleSize_SHIFT                                   1
#define     APEHeaderInfo2_SampleSize_MASK                                    (0x3<<1)
#define     APEHeaderInfo2_Channel_assignment                                 0

#define     APETTimeH_TTimeH_e                                                5
#define     APETTimeH_TTimeH_SHIFT                                            0
#define     APETTimeH_TTimeH_MASK                                             (0x3F<<0)

#define     APETTimeM_TTimeM_e                                                5
#define     APETTimeM_TTimeM_SHIFT                                            0
#define     APETTimeM_TTimeM_MASK                                             (0x3F<<0)

#define     APETTimeS_TTimeS_e                                                5
#define     APETTimeS_TTimeS_SHIFT                                            0
#define     APETTimeS_TTimeS_MASK                                             (0x3F<<0)

#define     APECTimeH_CTimeH_e                                                5
#define     APECTimeH_CTimeH_SHIFT                                            0
#define     APECTimeH_CTimeH_MASK                                             (0x3F<<0)

#define     APECTimeM_CTimeM_e                                                5
#define     APECTimeM_CTimeM_SHIFT                                            0
#define     APECTimeM_CTimeM_MASK                                             (0x3F<<0)

#define     APECTimeS_CTimeS_e                                                5
#define     APECTimeS_CTimeS_SHIFT                                            0
#define     APECTimeS_CTimeS_MASK                                             (0x3F<<0)

#define     APEErrInfo_ErrInfo_e                                              3
#define     APEErrInfo_ErrInfo_SHIFT                                          0
#define     APEErrInfo_ErrInfo_MASK                                           (0xF<<0)

#define     APECFrameNum_CurrentFrame_e                                       31
#define     APECFrameNum_CurrentFrame_SHIFT                                   0
#define     APECFrameNum_CurrentFrame_MASK                                    (0xFFFFFFFF<<0)

#define     SoftIE_LackIntEn                                                  7
#define     SoftIE_EnergyIntEn                                                6
#define     SoftIE_WordIntEn                                                  5
#define     SoftIE_FrameRdyIP                                                 3

#define     SoftIP_LackIP                                                     7
#define     SoftIP_EnergyIP                                                   6
#define     SoftIP_WordIP                                                     5
#define     SoftIP_FrameRdyIP                                                 3

#define     SoftCtl_NewFile                                                   7
#define     SoftCtl_En                                                        6
#define     SoftCtl_SampFrq_e                                                 5
#define     SoftCtl_SampFrq_SHIFT                                             2
#define     SoftCtl_SampFrq_MASK                                              (0xF<<2)

#define     FrameLen_FrameLen_e                                               11
#define     FrameLen_FrameLen_SHIFT                                           0
#define     FrameLen_FrameLen_MASK                                            (0xFFF<<0)


#define     BEPCtl2_EqBandSel_e                                               7
#define     BEPCtl2_EqBandSel_SHIFT                                           5
#define     BEPCtl2_EqBandSel_MASK                                            (0x7<<5)
#define     BEPCtl2_FadeDuration_e                                            4
#define     BEPCtl2_FadeDuration_SHIFT                                        2
#define     BEPCtl2_FadeDuration_MASK                                         (0x7<<2)

#define     BEPCtl3_SilenceT_e                                                4
#define     BEPCtl3_SilenceT_SHIFT                                            0
#define     BEPCtl3_SilenceT_MASK                                             (0x1F<<0)

#define     BEPCtl4_WhishtSample_e                                            7
#define     BEPCtl4_WhishtSample_SHIFT                                        6
#define     BEPCtl4_WhishtSample_MASK                                         (0x3<<6)
#define     BEPCtl4_EffectMode_e                                              3
#define     BEPCtl4_EffectMode_SHIFT                                          2
#define     BEPCtl4_EffectMode_MASK                                           (0x3<<2)

#define     BEPCtl5_EnergyIn_e                                                6
#define     BEPCtl5_EnergyIn_SHIFT                                            0
#define     BEPCtl5_EnergyIn_MASK                                             (0x7F<<0)

#define     GlobalGain_GlobalGain_e                                           23
#define     GlobalGain_GlobalGain_SHIFT                                       0
#define     GlobalGain_GlobalGain_MASK                                        (0xFFFFFF<<0)

#define     CurrentEnergy_Energy_e                                            6
#define     CurrentEnergy_Energy_SHIFT                                        0
#define     CurrentEnergy_Energy_MASK                                         (0x7F<<0)

#define     KaraokeCtl_MonoEn                                                 6
#define     KaraokeCtl_LRselect                                               5
#define     KaraokeCtl_LPF_e                                                  4
#define     KaraokeCtl_LPF_SHIFT                                              3
#define     KaraokeCtl_LPF_MASK                                               (0x3<<3)
#define     KaraokeCtl_HPF_e                                                  2
#define     KaraokeCtl_HPF_SHIFT                                              0
#define     KaraokeCtl_HPF_MASK                                               (0x7<<0)

#define     LCHGain_LCHgain_e                                                 7
#define     LCHGain_LCHgain_SHIFT                                             0
#define     LCHGain_LCHgain_MASK                                              (0xFF<<0)

#define     RCHGain_RCHgain_e                                                 7
#define     RCHGain_RCHgain_SHIFT                                             0
#define     RCHGain_RCHgain_MASK                                              (0xFF<<0)

#define     LPFGain_LPFgain_e                                                 7
#define     LPFGain_LPFgain_SHIFT                                             0
#define     LPFGain_LPFgain_MASK                                              (0xFF<<0)

#define     HPFGain_HPFgain_e                                                 7
#define     HPFGain_HPFgain_SHIFT                                             0
#define     HPFGain_HPFgain_MASK                                              (0xFF<<0)

#define     EQCoe0_GainCoefficient_e                                          5
#define     EQCoe0_GainCoefficient_SHIFT                                      0
#define     EQCoe0_GainCoefficient_MASK                                       (0x3F<<0)

#define     EQCoe1_GainCoefficient_e                                          5
#define     EQCoe1_GainCoefficient_SHIFT                                      0
#define     EQCoe1_GainCoefficient_MASK                                       (0x3F<<0)

#define     EQCoe2_GainCoefficient_e                                          5
#define     EQCoe2_GainCoefficient_SHIFT                                      0
#define     EQCoe2_GainCoefficient_MASK                                       (0x3F<<0)

#define     EQCoe3_GainCoefficient_e                                          5
#define     EQCoe3_GainCoefficient_SHIFT                                      0
#define     EQCoe3_GainCoefficient_MASK                                       (0x3F<<0)

#define     EQCoe4_GainCoefficient_e                                          5
#define     EQCoe4_GainCoefficient_SHIFT                                      0
#define     EQCoe4_GainCoefficient_MASK                                       (0x3F<<0)

#define     EQCoe5_GainCoefficient_e                                          5
#define     EQCoe5_GainCoefficient_SHIFT                                      0
#define     EQCoe5_GainCoefficient_MASK                                       (0x3F<<0)

#define     EQCoe6_GainCoefficient_e                                          5
#define     EQCoe6_GainCoefficient_SHIFT                                      0
#define     EQCoe6_GainCoefficient_MASK                                       (0x3F<<0)

#define     SRSCfg1_LfPeak_e                                                  2
#define     SRSCfg1_LfPeak_SHIFT                                              0
#define     SRSCfg1_LfPeak_MASK                                               (0x7<<0)

#define     SRSCfg2_CenterCtl_e                                               7
#define     SRSCfg2_CenterCtl_SHIFT                                           4
#define     SRSCfg2_CenterCtl_MASK                                            (0xF<<4)
#define     SRSCfg2_SpaceCtl_e                                                3
#define     SRSCfg2_SpaceCtl_SHIFT                                            0
#define     SRSCfg2_SpaceCtl_MASK                                             (0xF<<0)

#define     SRSCfg3_TrubassCtl_e                                              7
#define     SRSCfg3_TrubassCtl_SHIFT                                          4
#define     SRSCfg3_TrubassCtl_MASK                                           (0xF<<4)
#define     SRSCfg3_DefCtl_e                                                  3
#define     SRSCfg3_DefCtl_SHIFT                                              0
#define     SRSCfg3_DefCtl_MASK                                               (0xF<<0)

#define     SRSCfg4_FocusCtl_e                                                3
#define     SRSCfg4_FocusCtl_SHIFT                                            0
#define     SRSCfg4_FocusCtl_MASK                                             (0xF<<0)

#define     VPSCtl_CombineEn                                                  4
#define     VPSCtl_TimeScaleStage_e                                           3
#define     VPSCtl_TimeScaleStage_SHIFT                                       0
#define     VPSCtl_TimeScaleStage_MASK                                        (0xF<<0)

#define     SequenceSize_SequenceSize_e                                       11
#define     SequenceSize_SequenceSize_SHIFT                                   0
#define     SequenceSize_SequenceSize_MASK                                    (0xFFF<<0)

#define     OverlapSize_OverlapSize_e                                         8
#define     OverlapSize_OverlapSize_SHIFT                                     0
#define     OverlapSize_OverlapSize_MASK                                      (0x1FF<<0)

#define     SeekWindowSize_SeekWindowSize_e                                   8
#define     SeekWindowSize_SeekWindowSize_SHIFT                               0
#define     SeekWindowSize_SeekWindowSize_MASK                                (0x1FF<<0)
////////////////////////////////////////////////////////////////////////////////////////////////
// DAC_PA
sfr DAC_CTL0              =  0xd5;      // 0x04         DAC Control Register 0
sfr DAC_VOLUME            =  0xd6;      // 0x04         DAC volume control register
sfr DACFIFO_CTL0          =  0xd7;      // 0x04         DAC FIFO Control Register 0
sfr DACFIFO_CTL1          =  0xd8;      // 0x04         DAC FIFO Register 1
sfr I2S_CTL0              =  0xd9;      // 0x04         I2S Control Register 0
sfr I2S_CTL1              =  0xda;      // 0x04         I2S Control Register 1
sfr DAC_PCML              =  0xdb;      // 0x04         PCM DATA Low byte
sfr DAC_PCMM              =  0xdc;      // 0x04         PCM DATA Middle byte
sfr DAC_PCMH              =  0xdd;      // 0x04         PCM DATA High byte
sfr DAC_ANALOG            =  0xde;      // 0x04         DAC Analog Control Register
sfr PA_VOLUME             =  0xdf;      // 0x04         PA VOLUME Control Register
sfr DAC_TUNE0             =  0xf8;      // 0x04         DAC tuning Control Register 0
sfr DAC_TUNE1             =  0xe1;      // 0x04         DAC tuning Control Register 1
sfr PA_CTL_NEW            =  0xe2;      // 0x04         PA Control Register
sfr PA_APCTL_NEW          =  0xe3;      // 0x04         PA anti-pop Control Register
sfr DDV_CTL0              =  0xe4;      // 0x04         Direct drive Control Register 0
sfr DAC_CTL1_NEW          =  0xe5;      // 0x04         DAC Control Register 1
sfr PA_CTL                =  0xfc;      // 0x04         PA Control Register
sfr FMOP_CTL              =  0xec;      // 0x04         FM OP Control Register // equal ADC_GAIN1 
////////////////////////////////////////////////////////////////////////////////////////////////
// Key-tone-Generator Controller
sfr KT_CTL                =  0xe6;      // 0x04         Key-tone Control Register
sfr KT_VAL                =  0xe7;      // 0x04         Key-tone Value Register
////////////////////////////////////////////////////////////////////////////////////////////////
// ADC
sfr ADC_CTL0              =  0xe9;      // 0x04         ADC Control Register 0
sfr AINOP_CTL             =  0xea;      // 0x04         Mic/FM OP Control Register
sfr ADC_GAIN0             =  0xeb;      // 0x04         ADC gain Control Register0
sfr ADC_GAIN1             =  0xec;      // 0x04         ADC gain Control Register1
sfr ADC_TUNE0             =  0xed;      // 0x04         ADC tuning control Register 0
sfr ADC_TUNE1             =  0xee;      // 0x04         ADC tuning control Register 1
sfr ADC_DAT               =  0xef;      // 0x04         ADC data register
sfr ADC_FIFOCTL0          =  0xf1;      // 0x04         ADC FIFO control register 0
sfr ADC_FIFOCTL1          =  0xf2;      // 0x04         ADC FIFO control register 1
sfr ADC_CTL1              =  0xf3;      // 0x04         ADC Control Register 1
////////////////////////////////////////////////////////////////////////////////////////////////
// CLASSD
sfr CLD_CTL0              =  0xf4;      // 0x04         ClassD Control Register 0
sfr CLD_CTL1              =  0xf5;      // 0x04         ClassD Control Register 1
sfr CLD_CTL2              =  0xf6;      // 0x04         ClassD Control Register 2
sfr CLD_CTL3              =  0xf7;      // 0x04         ClassD Control Register 3
////////////////////////////////////////////////////////////////////////////////////////////////
// PMU
sfr VOUT_CTL              =  0x90;      // 0x05         VCC/VDD voltage set Register
sfr VDD_DCDC_CTL          =  0x91;      // 0x05         VDD DCDC Modulation/frequency/MAX current set
sfr VCC_CURRENT_CTL       =  0x92;      // 0x05         VCC DCDC and LDO current limit set Register
sfr FS_CTL                =  0x93;      // 0x05         FSOURCE control Register
sfr CHG_CTL               =  0x94;      // 0x05         Charge current and temperature set Register
sfr CHG_DET               =  0x95;      // 0x05         Charge current and status detect Register
sfr CHG_ASSISTANT         =  0x97;      // 0x05         Charge terminate voltage and temperature set Register
sfr DIODE_CTL             =  0x98;      // 0x05         Idea diode control Register
sfr BDG_CTL               =  0x99;      // 0x05         Bandgap enable and voltage set Register
sfr EFUSE_CTL             =  0x9a;      // 0x05         Efuse enable and efuse bounding option Register
sfr PMUADC_CTL            =  0x9b;      // 0x05         ADC frequency and enable Register
sfr BATADC_DATA           =  0x9c;      // 0x05         BATADC data Register
sfr LRADC2_DATA           =  0x9d;      // 0x05         LRADC2 data Register
sfr LRADC1_DATA           =  0x9e;      // 0x05         LRADC1 data Register
sfr LRADC3_DATA           =  0x9f;      // 0x05         LRADC3 data Register
sfr LRADC4_DATA           =  0xa2;      // 0x05         LRADC4 data Register
sfr LRADC5_DATA           =  0xa3;      // 0x05         LRADC5 data Register
sfr MULTI_USED            =  0xa4;      // 0x05         GPIO multi-used set Register
sfr SYSTEM_VOL            =  0xa5;      // 0x05         System Voltage detect or set Register
sfr CP_CTL0               =  0xa6;      // 0x05         Charge pump control register 0
sfr SYSTEM_CTL_RTCVDD     =  0xa7;      // 0x05         System on/off and play/pause time set & LB voltage set Register
sfr PRESS_DISPLAY         =  0xa9;      // 0x05         SYSON key-press status display Register
sfr CP_CTL1               =  0xaa;      // 0x05         Charge pump control register 1
sfr ON_OFF_MAPPING        =  0xab;      // 0x05         on button mapping and low power dectect register
sfr SYSTEM_CTL1           =  0xac;      // 0x05         VCC软启动时间设置和长按RESET时间设置寄存器
sfr SDVCCOUT_CTL          =  0xad;      // 0x05         SDVCCOUT_CTL register
sfr ID_STATUS             =  0xae;      // 0x05         EFUSE ID status Register
sfr EFUSE3_ID             =  0xaf;      // 0x05         EFUSE3 ID STATUS Register
////////////////////////////////////////////////////////////////////////////////////////////////
// External memory access
sfr EXTMEM_CTL            =  0xfc;      // 0x06/0x08    External memory control Register
sfr EXTMEM_WT             =  0xfd;      // 0x06/0x08    External memory wait state
sfr EXTMEM_DH             =  0xfe;      // 0x06         External memory high byte
sfr EXTMEM_DL             =  0xff;      // 0x06/0x08    External memory low byte
#ifdef __A251__
sfr EXTMEM_D              =  0xfe;      // 0x06         External memory data 16 bit mode access
#endif
////////////////////////////////////////////////////////////////////////////////////////////////
// LCD controller
sfr LCD_if_pcs            =  0x97;      // 0x06         LCD Interface Parity Sum Register
sfr LCD_CTL               =  0x98;      // 0x06         LCD Control Register
sfr LCD_CTL0              =  0x98;      // 0x06         LCD Control Register 0
sfr LCD_IF_CLK            =  0x99;      // 0x06         LCD Interface Clock Register
sfr DMA_CTL               =  0x9a;      // 0x06         DMA Control Register
sfr DMA3_CTL              =  0x9a;      // 0x06         DMA Control Register
sfr DMA3_SRCADDRH         =  0x9b;      // 0x06         DMA3 SRC Address High Register
sfr DMA3_SRCADDRL         =  0x9c;      // 0x06         DMA3 SRC Address Low Register
sfr DMA3_CCNTH            =  0x9d;      // 0x06         DMA3 Column Higher Counter Register
sfr DMA3_CCNTL            =  0x9e;      // 0x06         DMA3 Column Lower Counter Register
sfr DMA3_RCNT             =  0x9f;      // 0x06         DMA3 Row Counter Register
sfr WIDTH_INVALID_CNT     =  0xf4;      // 0x06         invalid pixels per row
sfr LCD_CTL1              =  0xf5;      // 0x06         LCD Control Register 1
#ifdef __A251__
sfr DMA3_CCNT             =  0x9d;      // 0x06         DMA3_CCNT 16 bit mode access
#endif
////////////////////////////////////////////////////////////////////////////////////////////////
// GPIO
sfr GPIOAOUTEN            =  0xa2;      // 0x06         General Purpose Input Output Group A Output Enable
sfr GPIOAINEN             =  0xa3;      // 0x06         General Purpose Input Output Group A Input Enable
sfr GPIOADAT              =  0xa4;      // 0x06         General Purpose Input Output Group A Data
sfr GPIOBOUTEN            =  0xa5;      // 0x06         General Purpose Input Output Group B Output Enable
sfr GPIOBINEN             =  0xa6;      // 0x06         General Purpose Input Output Group B Input Enable
sfr GPIOBDAT              =  0xa7;      // 0x06         General Purpose Input Output Group B Data
sfr GPIOCOUTEN            =  0xa9;      // 0x06         General Purpose Input Output Group C Output Enable
sfr GPIOCINEN             =  0xaa;      // 0x06         General Purpose Input Output Group C Input Enable
sfr GPIOCDAT              =  0xab;      // 0x06         General Purpose Input Output Group C Data
sfr GPIODOUTEN            =  0xac;      // 0x06         General Purpose Input Output Group D Output Enable
sfr GPIODINEN             =  0xad;      // 0x06         General Purpose Input Output Group D Input Enable
sfr GPIODDAT              =  0xae;      // 0x06         General Purpose Input Output Group D Data
sfr GPIOEOUTEN            =  0xaf;      // 0x06         General Purpose Input Output Group E Output Enable
sfr GPIOEINEN             =  0xb0;      // 0x06         General Purpose Input Output Group E Input Enable
sfr GPIOEDAT              =  0xb1;      // 0x06         General Purpose Input Output Group E Data
sfr GPIOFOUTEN            =  0xb2;      // 0x06         General Purpose Input Output Group F Output Enable
sfr GPIOFINEN             =  0xb3;      // 0x06         General Purpose Input Output Group F Input Enable
sfr GPIOFDAT              =  0xb4;      // 0x06         General Purpose Input Output Group F Data
sfr GPIOGOUTEN            =  0xb5;      // 0x06         General Purpose Input Output Group G Output Enable
sfr GPIOGINEN             =  0xb6;      // 0x06         General Purpose Input Output Group G Input Enable
sfr GPIOGDAT              =  0xb7;      // 0x06         General Purpose Input Output Group G Data
// sfr Reserved              =  0xb8;      // 0x06         Reserved Register
sfr BOOTSEL               =  0xb9;      // 0x06         Boot Mode Select Register
sfr PADDRV0               =  0xba;      // 0x06         Pad Drive Control 0 Register
sfr PADDRV1               =  0xbb;      // 0x06         Pad Drive Control 1 Register
sfr PADDRV2               =  0xbc;      // 0x06         Pad Drive Control 2 Register
// sfr Reserved              =  0xbd;      // 0x06         Pad Drive Reserved Register
// sfr Reserved              =  0xc2;      // 0x06         Pad Drive Reserved Register
sfr PADPUPD0              =  0xc3;      // 0x06         Pad PU and PD Control 0 Register
sfr PADPUPD1              =  0xc4;      // 0x06         Pad PU and PD Control 1Register
// sfr Reserved              =  0xc5;      // 0x06         Pad PU and PD Control Reserved Register
sfr PWMDUTY               =  0xc6;      // 0x06         PWM Control Register
sfr PWMDIV                =  0xc7;      // 0x06         PWM Clock Divide Register
// sfr Reserved              =  0xc8;      // 0x06         PWM Reserved Register
// sfr Reserved              =  0xc9;      // 0x06         PWM Reserved Register
sfr MFPSEL0               =  0xca;      // 0x06,0x08    Multifunction select 0 Register
sfr MFPSEL1               =  0xcb;      // 0x06,0x08    Multifunction select 1 Register
sfr MFPSEL2               =  0xcc;      // 0x06         Multifunction select 2 Register
sfr MFPSEL3               =  0xcd;      // 0x06         Multifunction select 3 Register
sfr MFPSEL4               =  0xce;      // 0x06         Multifunction select 4 Register
sfr MFPSEL5               =  0xcf;      // 6,8          Multifunction select 5 Register
sfr MFPSEL6               =  0xd2;      // 0x06         Multifunction select 6 Register
//  Reserved              =  0xd3;      // 0x06         Multifunction Reserved Register
//  Reserved              =  0xd4;      // 0x06         Debug Reserved Register
sfr DBGSEL                =  0xd5;      // 0x06         Debug Select Register
sfr DBGBOE                =  0xd6;      // 0x06         DebugB[7:0] Output Enable
sfr DBGACOE               =  0xd7;      // 0x06         DebugAC Output Enable
sfr DBGDOE                =  0xd9;      // 0x06         DebugD[7:0] Output Enable
sfr DBGDIE                =  0xda;      // 0x06         DebugD[7:0] Input Enable
sfr DBGFOE                =  0xdb;      // 0x06         DebugF[7:0] Output Enable
//  Reserved                =  0xdc;     // 0x06          Debug Reserved Register
sfr ChipVersion           =  0xde;      // 0x06         Chip Version Register
sfr BDOP                  =  0xdf;      // 0x06         Bonding Option Register
sfr PADDRV3               =  0xe1;      // 0x06         Pad Drive Control 3 Register
sfr PADDRV4               =  0xe2;      // 0x06         Pad Drive Control 4 Register
sfr GPIOAPU               =  0xe3;      // 0x06         GPIOA 50K PU Enable
sfr GPIOAPD               =  0xe4;      // 0x06         GPIOA 50K PD Enable
sfr GPIOBPU               =  0xe5;      // 0x06         GPIOB 50K PU Enable
sfr GPIOBPD               =  0xe6;      // 0x06         GPIOB 50K PD Enable
sfr GPIOCPU               =  0xe7;      // 0x06         GPIOC 50K PU Enable
sfr GPIOCPD               =  0xe9;      // 0x06         GPIOC 50K PD Enable
sfr GPIODPU               =  0xea;      // 0x06         GPIOD 50K PU Enable
sfr GPIODPD               =  0xeb;      // 0x06         GPIOD 50K PD Enable
sfr GPIOEPU               =  0xec;      // 0x06         GPIOE 50K PU Enable
sfr GPIOEPD               =  0xed;      // 0x06         GPIOE 50K PD Enable
sfr GPIOFPU               =  0xee;      // 0x06         GPIOF 50K PU Enable
sfr GPIOFPD               =  0xef;      // 0x06         GPIOF 50K PD Enable
sfr GPIOGPU               =  0xf1;      // 0x06         GPIOG 50K PU Enable
sfr GPIOGPD               =  0xf2;      // 0x06         GPIOG 50K PD Enable
sfr DBGCIE                =  0xf3;      // 0x06         GPIOC Debug Input enable register
////////////////////////////////////////////////////////////////////////////////////////////////
// USB
sfr UDMAM                 =  0x86;      // 0x07         USB DMA MODE
sfr NTRIRQ                =  0x8a;      // 0x07         No transaction IRQ
sfr NORMALLENH            =  0x8c;      // 0x07         UDMA transfer length high in normal mode
sfr NORMALLENL            =  0x8d;      // 0x07         UDMA transfer length low in normal mode
sfr OUTPCKCNTH            =  0x90;      // 0x07         Out transaction packet counter high
sfr OUTPCKCNTL            =  0x91;      // 0x07         Out transaction packet counter low
sfr IDVBUSCTRL            =  0x92;      // 0x07         ID&VBUS control
sfr USBSTATUS             =  0x93;      // 0x07         USB status
sfr DPDMCTRL              =  0x94;      // 0x07         DP DM control register
sfr USB_PHYCTRL           =  0x95;      // 0x07         PHY control register
sfr Out0bc_hcin0bc        =  0x97;      // 0x07         Endpoint 0 OUT Byte Count
sfr In0bc_hcout0bc        =  0x98;      // 0x07         Endpoint 0 IN Byte Count
sfr Ep0cs_hcep0cs         =  0x99;      // 0x07         Endpoint 0 Control and Status
sfr In1bch_hcout1bch      =  0x9a;      // 0x07         Endpoint 1 IN Byte Count High
sfr In1bcl_hcout1bcl      =  0x9b;      // 0x07         Endpoint 1 IN Byte Count Low
sfr In1ctrl_hcout1ctrl    =  0x9c;      // 0x07         Endpoint 1 IN Control
sfr In1cs_hcout1cs        =  0x9d;      // 0x07         Endpoint 1 IN Control And Status
sfr Out2bch_hcin2bch      =  0x9e;      // 0x07         Endpoint 2 OUT Byte Count High
sfr Out2bcl_hcin2bcl      =  0x9f;      // 0x07         Endpoint 2 OUT Byte Count Low
sfr Out2ctrl_hcin2ctrl    =  0xa2;      // 0x07         Endpoint 2 OUT Control
sfr Out2cs_hcin2cs        =  0xa3;      // 0x07         Endpoint 2 OUT Control And Status
sfr In3bc_hcout3bc        =  0xa4;      // 0x07         Endpoint 3 IN Byte Count
sfr In3ctrl_hcout3ctrl    =  0xa5;      // 0x07         Endpoint 3 IN Control
sfr In3cs_hcout3cs        =  0xa6;      // 0x07         Endpoint 3 IN Control And Status
sfr Fifo1dat              =  0xa7;      // 0x07         FIFO 1 Data
sfr Fifo2dat              =  0xa9;      // 0x07         FIFO 2 Data
sfr Fifo3dat              =  0xaa;      // 0x07         FIFO 3 Data
sfr Ep0indata             =  0xab;      // 0x07         EP0 IN DATA
sfr Ep0outdata            =  0xac;      // 0x07         EP0 OUT DATA
sfr Usbirq_hcusbirq       =  0xad;      // 0x07         USB Interrupt
sfr Usbien_hcusbien       =  0xae;      // 0x07         USB interrupt enable
sfr SHORTPCKIRQ           =  0xaf;      // 0x07         Short packets Interrupt request and enable
sfr Hcep0ctrl             =  0xb0;      // 0x07         Endpoint 0 Control
sfr Hcout0err             =  0xb1;      // 0x07         Endpoint 0 HC OUT Error
sfr Hcin0err              =  0xb2;      // 0x07         Endpoint 0 HC IN Error
sfr Hcout1ctrl            =  0xb3;      // 0x07         Endpoint 1 HC OUT Control
sfr Hcout1err             =  0xb4;      // 0x07         Endpoint 1 HC OUT Error
sfr Hcin2ctrl             =  0xb5;      // 0x07         Endpoint 2 HC IN Control
sfr Hcin2err              =  0xb6;      // 0x07         Endpoint 2 HC OUT Error
sfr Hcout3ctrl            =  0xb9;      // 0x07         Endpoint 3 HC OUT Control
sfr Hcout3err             =  0xba;      // 0x07         Endpoint 3 HC OUT Error
sfr Setupdat0             =  0xbb;      // 0x07         SETUP DATA0
sfr Setupdat1             =  0xbc;      // 0x07         SETUP DATA1
sfr Setupdat2             =  0xbd;      // 0x07         SETUP DATA2
sfr Setupdat3             =  0xbf;      // 0x07         SETUP DATA3
sfr Setupdat4             =  0xc1;      // 0x07         SETUP DATA4
sfr Setupdat5             =  0xc2;      // 0x07         SETUP DATA5
sfr Setupdat6             =  0xc3;      // 0x07         SETUP DATA6
sfr Setupdat7             =  0xc4;      // 0x07         SETUP DATA7
sfr Ep03irq               =  0xc5;      // 0x07         Endpoint 0 to 3 Interrupt Request
sfr Ep03ien               =  0xc6;      // 0x07         Endpoint 0 to 3 Interrupt Enables
sfr Ep03tokirq            =  0xc7;      // 0x07         Endpoint 0 to 3 Token Interrupt Request
sfr Ep03tokien            =  0xc8;      // 0x07         Endpoint 0 to 3 Token Interrupt Request Enable
sfr IVECT                 =  0xc9;      // 0x07         Interrupt Vector
sfr EPRST                 =  0xca;      // 0x07         Endpoint Reset
sfr UsbCTRL_STUS          =  0xcb;      // 0x07         USB Control And Status
sfr FrmCNTH               =  0xcc;      // 0x07         USB Frame Counter HIGH
sfr FrmCNTL               =  0xcd;      // 0x07         USB Frame Counter Low
sfr Fnaddr                =  0xce;      // 0x07         Function Address
sfr Clkgate               =  0xcf;      // 0x07         Clock Gate
sfr Fifoctrl              =  0xd2;      // 0x07         FIFO Control
sfr Hcportctrl            =  0xd3;      // 0x07         HC Port Control
sfr Hcfrmnh               =  0xd4;      // 0x07         HC Frame Number high
sfr Hcfrmnl               =  0xd5;      // 0x07         HC Frame Number low
sfr Hcfrmremainh          =  0xd6;      // 0x07         HC Frame Remain high
sfr Hcfrmremainl          =  0xd7;      // 0x07         HC Frame Remain Low
sfr Hcep03errirq          =  0xd8;      // 0x07         HC 0 to 3 Error Interrupt Request
sfr Hcep03errien          =  0xd9;      // 0x07         HC 0 to 3 Error Interrupt Enable
sfr Otgirq                =  0xda;      // 0x07         OTG Interrupt Request
sfr Otgstate              =  0xdb;      // 0x07         The OTG FSM State
sfr Otgctrl               =  0xdc;      // 0x07         OTG Control
sfr Otgstatus             =  0xdd;      // 0x07         OTG Status
sfr Otgien                =  0xde;      // 0x07         OTG Interrupt Enable
sfr TaidlbdisTimer        =  0xdf;      // 0x07         OTG ta_aidl_bdis Timer
sfr TawaitbconTimer       =  0xe1;      // 0x07         OTG ta_wait_bcon Timer
sfr TbvbusplsTimer        =  0xe2;      // 0x07         OTG tb_vbus_pls Timer
sfr TbvbusdisplsTimer     =  0xe3;      // 0x07         OTG tb_vbus_dischrg_pls Timer
sfr Hcin0maxpck           =  0xe4;      // 0x07         HC IN 0 Max Packet Size
sfr Hcin2maxpckh          =  0xe5;      // 0x07         HC IN 2 max packet high
sfr Hcin2maxpckl          =  0xe6;      // 0x07         HC IN 2 max packet low
sfr Hcout1maxpckh         =  0xe9;      // 0x07         HC OUT 1 max packet low
sfr Hcout1maxpckl         =  0xea;      // 0x07         HC OUT 1 max packet high
sfr Hcout3maxpck          =  0xe7;      // 0x07         HC OUT 3 max packet
sfr USBEIRQ               =  0xeb;      // 0x07         USB external Interrupt request
sfr AUTONAKCTRL           =  0xec;      // 0x07         auto nak control
sfr HCINCTRL              =  0xed;      // 0x07         host in control
sfr DBGMODE               =  0xee;      // 0x07         debug mode
sfr VDCTRL                =  0xef;      // 0x07         USB PHY vendor control
sfr VDSTAT                =  0xf1;      // 0x07         USB PHY vendor status
sfr USBFIFODIR            =  0xf2;      // 0x07         Usb fifo dir register
sfr BKDOOR                =  0xf3;      // 0x07         Test back door
sfr SRPCTRL               =  0xf4;      // 0x07         SRP control
sfr OTGTRIEN              =  0xf5;      // 0x07         OTR status machine interrupt enable
sfr OTGTRIRQ              =  0xf6;      // 0x07         OTR status machine interrupt request
sfr USB_Efuse_Ref         =  0xf7;      // 0x07         Usb Access Efuse_Ref register
sfr FSMPRESTATE           =  0xfc;      // 0x07         FSM pre-state register
sfr HCIN2CNTH             =  0xfd;      // 0x07         hcin2 packet counter high
sfr HCIN2CNTL             =  0xfe;      // 0x07         hcin2 packet counter low
////////////////////////////////////////////////////////////////////////////////////////////////
// nand flash register
sfr NAND_EN               =  0x90;      // 0x08         Nand flash controller & CE enable register
sfr NAND_CTL              =  0x91;      // 0x08         Nand flash IF & IE register
sfr NAND_STUTUS           =  0x92;      // 0x08         Nand flash FSM status register
sfr NAND_ECCERR_MAX_STATUS=  0x93;      // 0x08         BCH ECC maximal status register
sfr NAND_ECC_FF_MARK      =  0x94;      // 0x08         Nand flash DAFF status register
sfr NAND_CONFIG           =  0x95;      // 0x08         Nand flash controller config register
sfr NAND_SCOLADDR1        =  0x97;      // 0x08         Nand flash spare col address register1
sfr NAND_SCOLADDR0        =  0x98;      // 0x08         Nand flash spare col address register0
sfr NAND_MCOLADDR1        =  0x99;      // 0x08         Nand flash main col address register1
sfr NAND_MCOLADDR0        =  0x9a;      // 0x08         Nand flash main col address register0
sfr NAND_ROWADDR4         =  0x9b;      // 0x08         Nand flash row address register4
sfr NAND_ROWADDR3         =  0x9c;      // 0x08         Nand flash row address register3
sfr NAND_ROWADDR2         =  0x9d;      // 0x08         Nand flash row address register2
sfr NAND_ROWADDR1         =  0x9e;      // 0x08         Nand flash row address register1
sfr NAND_ROWADDR0         =  0x9f;      // 0x08         Nand flash row address register0
sfr NAND_FSM_START_CTL    =  0xa2;      // 0x08         Nand flash FSM START control register
sfr NAND_FSM_START        =  0xa3;      // 0x08         Nand flash FSM START register
sfr NAND_FSM0_CMD3        =  0xa4;      // 0x08         Nand flash FSM0 command3
sfr NAND_FSM0_CMD2        =  0xa5;      // 0x08         Nand flash FSM0 command2
sfr NAND_FSM0_CMD1        =  0xa6;      // 0x08         Nand flash FSM0 command1
sfr NAND_FSM0_CMD0        =  0xa7;      // 0x08         Nand flash FSM0 command0
sfr NAND_FSM0_CMD_CTL3    =  0xa9;      // 0x08         Nand flash FSM0 command control3
sfr NAND_FSM0_CMD_CTL2    =  0xaa;      // 0x08         Nand flash FSM0 command control2
sfr NAND_FSM0_CMD_CTL1    =  0xab;      // 0x08         Nand flash FSM0 command control1
sfr NAND_FSM0_CMD_CTL0    =  0xac;      // 0x08         Nand flash FSM0 command control0
sfr NAND_FSM1_CMD3        =  0xad;      // 0x08         Nand flash FSM1 command3
sfr NAND_FSM1_CMD2        =  0xae;      // 0x08         Nand flash FSM1 command2
sfr NAND_FSM1_CMD1        =  0xaf;      // 0x08         Nand flash FSM1 command1
sfr NAND_FSM1_CMD0        =  0xb0;      // 0x08         Nand flash FSM1 command0
sfr NAND_FSM1_CMD_CTL3    =  0xb1;      // 0x08         Nand flash FSM1 command control3
sfr NAND_FSM1_CMD_CTL2    =  0xb2;      // 0x08         Nand flash FSM1 command control2
sfr NAND_FSM1_CMD_CTL1    =  0xb3;      // 0x08         Nand flash FSM1 command control1
sfr NAND_FSM1_CMD_CTL0    =  0xb4;      // 0x08         Nand flash FSM1 command control0
sfr NAND_FSM2_CMD3        =  0xb5;      // 0x08         Nand flash FSM2 command3
sfr NAND_FSM2_CMD2        =  0xb6;      // 0x08         Nand flash FSM2 command2
sfr NAND_FSM2_CMD1        =  0xb7;      // 0x08         Nand flash FSM2 command1
sfr NAND_FSM2_CMD0        =  0xb8;      // 0x08         Nand flash FSM2 command0
sfr NAND_FSM2_CMD_CTL3    =  0xb9;      // 0x08         Nand flash FSM2 command control3
sfr NAND_FSM2_CMD_CTL2    =  0xba;      // 0x08         Nand flash FSM2 command control2
sfr NAND_FSM2_CMD_CTL1    =  0xbb;      // 0x08         Nand flash FSM2 command control1
sfr NAND_FSM2_CMD_CTL0    =  0xbc;      // 0x08         Nand flash FSM2 command control0
sfr NAND_ECCSTf           =  0xbd;      // 0x08         Nand BCH ECC status of sectorf register
sfr NAND_ECCSTe           =  0xc1;      // 0x08         Nand BCH ECC status of sectore register
sfr NAND_ECCSTd           =  0xc2;      // 0x08         Nand BCH ECC status of sectord register
sfr NAND_ECCSTc           =  0xc3;      // 0x08         Nand BCH ECC status of sectorc register
sfr NAND_ECCSTb           =  0xc4;      // 0x08         Nand BCH ECC status of sectorb register
sfr NAND_ECCSTa           =  0xc5;      // 0x08         Nand BCH ECC status of sectora register
sfr NAND_ECCST9           =  0xc6;      // 0x08         Nand BCH ECC status of sector9 register
sfr NAND_ECCST8           =  0xc7;      // 0x08         Nand BCH ECC status of sector8 register
sfr NAND_ECCST7           =  0xc8;      // 0x08         Nand BCH ECC status of sector7 register
sfr NAND_ECCST6           =  0xc9;      // 0x08         Nand BCH ECC status of sector6 register
sfr NAND_ECCST5           =  0xcc;      // 0x08         Nand BCH ECC status of sector5 register
sfr NAND_ECCST4           =  0xcd;      // 0x08         Nand BCH ECC status of sector4 register
sfr NAND_ECCST3           =  0xce;      // 0x08         Nand BCH ECC status of sector3 register
sfr NAND_ECCST2           =  0xd2;      // 0x08         Nand BCH ECC status of sector2 register
sfr NAND_ECCST1           =  0xd3;      // 0x08         Nand BCH ECC status of sector1 register
sfr NAND_ECCST0           =  0xd4;      // 0x08         Nand BCH ECC status of sector0 register
sfr NAND_UDATAf           =  0xd5;      // 0x08         Nand flash user data registerf
sfr NAND_UDATAe           =  0xd6;      // 0x08         Nand flash user data registere
sfr NAND_UDATAd           =  0xd7;      // 0x08         Nand flash user data registerd
sfr NAND_UDATAc           =  0xd8;      // 0x08         Nand flash user data registerc
sfr NAND_UDATAb           =  0xd9;      // 0x08         Nand flash user data registerb
sfr NAND_UDATAa           =  0xda;      // 0x08         Nand flash user data registera
sfr NAND_UDATA9           =  0xdb;      // 0x08         Nand flash user data register9
sfr NAND_UDATA8           =  0xdc;      // 0x08         Nand flash user data register8
sfr NAND_UDATA7           =  0xdd;      // 0x08         Nand flash user data register7
sfr NAND_UDATA6           =  0xde;      // 0x08         Nand flash user data register6
sfr NAND_UDATA5           =  0xdf;      // 0x08         Nand flash user data register5
sfr NAND_UDATA4           =  0xe1;      // 0x08         Nand flash user data register4
sfr NAND_UDATA3           =  0xe2;      // 0x08         Nand flash user data register3
sfr NAND_UDATA2           =  0xe3;      // 0x08         Nand flash user data register2
sfr NAND_UDATA1           =  0xe4;      // 0x08         Nand flash user data register1
sfr NAND_UDATA0           =  0xe5;      // 0x08         Nand flash user data register0
sfr NAND_BCH              =  0xe6;      // 0x08         Nand flash byte counter High register
sfr NAND_BCL              =  0xe7;      // 0x08         Nand flash byte counter Low register
sfr NAND_CHIPID           =  0xea;      // 0x08         Nand flash controller chipid input register
sfr NAND_DATA             =  0xeb;      // 0x08         Nand data register
sfr NAND_TIMING2          =  0xec;      // 0x08         Nand flash timing input register2
sfr NAND_TIMING1          =  0xed;      // 0x08         Nand flash timing input register1
sfr NAND_TIMING0          =  0xee;      // 0x08         Nand flash timing input register0
////////////////////////////////////////////////////////////////////////////////////////////////
// ACC
sfr ACC_CTRL              =  0x89;      // 0x08, 0x09   Control Register
sfr ACC_BLKLEN_LENHI      =  0x8a;      // 0x08, 0x09   Search block length or the low byte of data length of unsigned accumulation unit number
sfr ACC_BLKNUM_LENLO      =  0x8b;      // 0x08, 0x09   Search block number or the high byte of data length of unsigned accumulation unit number
sfr ACC_CMPDATA0_SUMHIE   =  0xf1;      // 0x08, 0x09   The first byte of data searched or the higher byte of unsigned accelerator’s result
sfr ACC_CMPDATA1_SUMHI    =  0xf2;      // 0x08, 0x09   The second byte of data searched or the high byte of unsigned accelerator’s result
sfr ACC_CMPDATA2_SUMLO    =  0xf3;      // 0x08, 0x09   The third byte of data searched or the low byte of unsigned accelerator’s result
sfr ACC_CMPDATA3_SUMLOE   =  0xf4;      // 0x08, 0x09   The fourth byte of data searched or the lower byte of unsigned accelerator’s result
sfr ACC_MATCNTHI          =  0xf5;      // 0x08, 0x09   The high byte of bitmap
sfr ACC_MATCNTLO          =  0xf6;      // 0x08, 0x09   The low byte of match number
sfr ACC_BITMAPHI          =  0xf7;      // 0x08, 0x09   The high byte of bitmap
sfr ACC_BITMAPLO          =  0xf8;      // 0x08, 0x09   The low byte of bitmap
sfr ACC_FIFO              =  0x8c;      // 0x08, 0x09   ACC FIFO address
////////////////////////////////////////////////////////////////////////////////////////////////
// CARD controller
sfr SDCMD                 =  0x90;      // 0x09         SD/MMC CMD Register
sfr SDARGRSP              =  0x91;      // 0x09         SD/MMC Argument or RSP Register
sfr SDRSPPOIN             =  0x92;      // 0x09         SD/MMC RSP Point Register
sfr SDTFCTL               =  0x93;      // 0x09         SD/MMC control register
sfr SDSTAT                =  0x94;      // 0x09         MMC/SD status Register
sfr SDBLKSZH              =  0x97;      // 0x09         SD/MMC High Block size Register
sfr SDBLKSZL              =  0x98;      // 0x09         SD/MMC Low Block size Register
sfr SDBLKNUM              =  0x99;      // 0x09         SD/MMC BLOCK number Register
sfr SDCLKCTL              =  0x9a;      // 0x09         SD/MMC Clock Control Register
sfr SDPADCTL              =  0x9b;      // 0x09         SD/MMC PAD CONTROL Register
sfr SDINTCTL              =  0x9c;      // 0x09         SD/MMC INTERRUPT Control Register
sfr SDDATFIFO             =  0x9d;      // 0x09         SD/MMC Data FIFO Register
sfr SD_TIMEOUT_CNT        =  0x9e;      // 0x09         Data Timeout Counter Register
sfr TimingCTL             =  0xad;      // 0x09         CARD interface timing control
////////////////////////////////////////////////////////////////////////////////////////////////
//SPI
sfr SPI_CTL               =  0x99;      // 0x0a         SPI Control Register
sfr SPI_DRQ               =  0x9a;      // 0x0a         SPI DMA/IRQ control Register.
sfr SPI_STA               =  0x9b;      // 0x0a         SPI Status Register
sfr SPI_CLKDIV            =  0x9c;      // 0x0a         SPI Clock Divide Control Register
sfr SPI_TXDAT             =  0x9d;      // 0x0a         SPI TX FIFO register
sfr SPI_RXDAT             =  0x9e;      // 0x0a         SPI RX FIFO register
sfr SPI_BCL               =  0x9f;      // 0x0a         SPI Bytes Count Low Register
sfr SPI_BCH               =  0xa2;      // 0x0a         SPI Bytes Count high Register
sfr SPI_DEBUG             =  0xa3;      // 0x0a         SPI debug register
sfr SPIBT_CTL             =  0xa4;      // 0x0a         SPI BOOT Control
sfr SPIBT_DLY             =  0xac;      // 0x0a         SPI BOOT delay control register
////////////////////////////////////////////////////////////////////////////////////////////////
// I2C
sfr I2C_CTL               =  0x89;      // 0x0a         I2C control register
sfr I2C_STA               =  0x8a;      // 0x0a         I2C status register
sfr I2C_DAT               =  0x8b;      // 0x0a         I2C data register
sfr I2C_ADDR              =  0x8c;      // 0x0a         I2C address register
sfr I2C_DEBUG             =  0x8d;      // 0x0a         I2C debug register
////////////////////////////////////////////////////////////////////////////////////////////////
// UART
sfr UART_BR               =  0x90;      // 0x0a         UART BAUDRATE Register.
sfr UART_MODE             =  0x91;      // 0x0a         UART mode setup Register.
sfr UART_CTL              =  0x92;      // 0x0a         UART Control Register.
sfr UART_DRQ              =  0x93;      // 0x0a         UART DRQ/IRQ register
sfr UART_STA              =  0x94;      // 0x0a         UART Status Register
sfr UART_TXDAT            =  0x95;      // 0x0a         UART TX FIFO register
sfr UART_RXDAT            =  0x97;      // 0x0a         UART RX FIFO register
sfr UART_DEBUG            =  0x98;      // 0x0a         UART debug register.
////////////////////////////////////////////////////////////////////////////////////////////////
// IR
sfr IR_CTL                =  0xa5;      // 0x0a         IR Control Register
sfr IR_STA                =  0xa6;      // 0x0a         IR Status Register
sfr IR_LUC                =  0xa7;      // 0x0a         IR low user code register.
sfr IR_HUC                =  0xa9;      // 0x0a         IR high user code register.
sfr IR_KDC                =  0xaa;      // 0x0a         IR key data code register.
sfr IR_TEST               =  0xab;      // 0x0a         IR test register.
sfr IR_CONFIG0            =  0xad;      // 0x0a         IR inner demodulator config0
sfr IR_CONFIG1            =  0xae;      // 0x0a         IR inner demodulator config1
sfr IR_CONFIG2            =  0xaf;      // 0x0a         IR inner demodulator config2
sfr IR_CONFIG3            =  0xb0;      // 0x0a         IR inner demodulator config3
sfr IR_CONFIG4            =  0xb1;      // 0x0a         IR inner demodulator config4
sfr IR_CONFIG5            =  0xb2;      // 0x0a         IR inner demodulator config5
sfr IR_CONFIG6            =  0xb3;      // 0x0a         IR inner demodulator config6
sfr IR_CONFIG7            =  0xb4;      // 0x0a         IR inner demodulator config7
sfr IR_CONFIG8            =  0xb5;      // 0x0a         IR inner demodulator config8
////////////////////////////////////////////////////////////////////////////////////////////////
// MJPEG
sfr AMVEN                 =  0x90;      // 0x0b         Decoder enable register
sfr AMVIE                 =  0x91;      // 0x0b         Decoder interrupt register
sfr AMVDecWidthH          =  0x92;      // 0x0b         Decoder width register 0
sfr AMVDecWidthL          =  0x93;      // 0x0b         Decoder width register 1
sfr AMVDecHeightH         =  0x94;      // 0x0b         Decoder height register 0
sfr AMVDecHeightL         =  0x95;      // 0x0b         Decoder height register 1
sfr AMVDecConfig          =  0x97;      // 0x0b         Decoder configure register
sfr AMV_SteamOffset       =  0x98;      // 0x0b         Bitstream Offset register
sfr AMV_SteamLenH         =  0x99;      // 0x0b         Stream length register 0
sfr AMV_SteamLenL         =  0x9a;      // 0x0b         Stream length register 1
sfr AMV_START             =  0x9b;      // 0x0b         Stream start bit register
sfr JPEG_DownSample       =  0x9c;      // 0x0b         Downsample register
sfr JPEG_VDownScaleH      =  0x9d;      // 0x0b         Vertical downscale register 0
sfr JPEG_VDownScaleL      =  0x9e;      // 0x0b         Vertical downscale register 1
sfr JPEG_HDownScaleH      =  0x9f;      // 0x0b         Horizontal downscale register 0
sfr JPEG_HDownScaleL      =  0xa2;      // 0x0b         Horizontal downscale register 1
sfr JPEG_SliceHeight      =  0xa3;      // 0x0b         Slice height register
sfr JPEG_OutSliceHeight   =  0xa4;      // 0x0b         Output slice height register
sfr JPEG_OutHeight        =  0xa5;      // 0x0b         DECODER output height register
sfr JPEG_OutWidthH        =  0xa6;      // 0x0b         DECODER output width register 0
sfr JPEG_OutWidthL        =  0xa7;      // 0x0b         DECODER output width register 1
sfr JPEG_PicHeightH       =  0xa9;      // 0x0b         DECODER picture height register 0
sfr JPEG_PicHeightL       =  0xaa;      // 0x0b         DECODER picture height register 1
sfr JPEG_PicWidthH        =  0xab;      // 0x0b         DECODER picture width register 0
sfr JPEG_PicWidthL        =  0xac;      // 0x0b         DECODER picture width register 1
sfr JPEG_HuffTab          =  0xad;      // 0x0b         Huffman table select register
sfr JPEG_AcVlcCode1       =  0xae;      // 0x0b         JPEG AC VLC code part1
sfr JPEG_AcVlcCode2       =  0xaf;      // 0x0b         JPEG AC VLC code part2
sfr JPEG_AcVlcCode3       =  0xb0;      // 0x0b         JPEG AC VLC code part3
sfr JPEG_AcVlcCode4       =  0xb1;      // 0x0b         JPEG AC VLC code part4
sfr JPEG_AcVlcCode5       =  0xb2;      // 0x0b         JPEG AC VLC code part5
sfr JPEG_AcVlcCode6       =  0xb3;      // 0x0b         JPEG AC VLC code part6
sfr JPEG_AcVlcCode7       =  0xb4;      // 0x0b         JPEG AC VLC code part7
sfr JPEG_AcVlcCode8       =  0xb5;      // 0x0b         JPEG AC VLC code part8
sfr JPEG_AcVlcCode9       =  0xb6;      // 0x0b         JPEG AC VLC code part9
sfr JPEG_AcVlcCode10      =  0xb7;      // 0x0b         JPEG AC VLC code part10
sfr JPEG_AcVlcCode11      =  0xb8;      // 0x0b         JPEG AC VLC code part11
sfr JPEG_AcVlcCode12      =  0xb9;      // 0x0b         JPEG AC VLC code part12
sfr JPEG_AcVlcCode13      =  0xba;      // 0x0b         JPEG AC VLC code part13
sfr JPEG_AcVlcCode14      =  0xbb;      // 0x0b         JPEG AC VLC code part14
sfr JPEG_AcVlcCode15      =  0xbc;      // 0x0b         JPEG AC VLC code part15
sfr JPEG_AcVlcCode16      =  0xbd;      // 0x0b         JPEG AC VLC code part16
sfr JPEG_AcVlcCode17      =  0xbf;      // 0x0b         JPEG AC VLC code part17
sfr JPEG_AcVlcCode18      =  0xc1;      // 0x0b         JPEG AC VLC code part18
sfr JPEG_AcVlcCode19      =  0xc2;      // 0x0b         JPEG AC VLC code part19
sfr JPEG_AcVlcCode20      =  0xc3;      // 0x0b         JPEG AC VLC code part20
sfr JPEG_AcVlcCode21      =  0xc4;      // 0x0b         JPEG AC VLC code part21
sfr JPEG_AcVlcCode22      =  0xc5;      // 0x0b         JPEG AC VLC code part22
sfr JPEG_AcVlcCode23      =  0xc6;      // 0x0b         JPEG AC VLC code part23
sfr JPEG_AcVlcCode24      =  0xc7;      // 0x0b         JPEG AC VLC code part24
sfr JPEG_AcVlcCode25      =  0xc8;      // 0x0b         JPEG AC VLC code part25
sfr JPEG_AcVlcCode26      =  0xc9;      // 0x0b         JPEG AC VLC code part26
sfr JPEG_AcVlcCode27      =  0xca;      // 0x0b         JPEG AC VLC code part27
sfr JPEG_AcVlcCode28      =  0xcb;      // 0x0b         JPEG AC VLC code part28
sfr JPEG_AcVlcCode29      =  0xcc;      // 0x0b         JPEG AC VLC code part29
sfr JPEG_AcVlcCode30      =  0xcd;      // 0x0b         JPEG AC VLC code part30
sfr JPEG_AcVlcCode31      =  0xce;      // 0x0b         JPEG AC VLC code part31
sfr JPEG_AcVlcCode32      =  0xcf;      // 0x0b         JPEG AC VLC code part32
sfr JPEG_DcVlcCode1       =  0xd2;      // 0x0b         JPEG DC VLC code part1
sfr JPEG_DcVlcCode2       =  0xd3;      // 0x0b         JPEG DC VLC code part2
sfr JPEG_DcVlcCode3       =  0xd4;      // 0x0b         JPEG DC VLC code part3
sfr JPEG_DcVlcCode4       =  0xd5;      // 0x0b         JPEG DC VLC code part4
sfr JPEG_DcVlcCode5       =  0xd6;      // 0x0b         JPEG DC VLC code part5
sfr JPEG_DcVlcCode6       =  0xd7;      // 0x0b         JPEG DC VLC code part6
sfr JPEG_DcVlcCode7       =  0xd8;      // 0x0b         JPEG DC VLC code part7
sfr JPEG_DcVlcCode8       =  0xd9;      // 0x0b         JPEG DC VLC code part8
sfr JPEG_DcVlcCode9       =  0xda;      // 0x0b         JPEG DC VLC code part9
sfr JPEG_DcVlcCode10      =  0xdb;      // 0x0b         JPEG DC VLC code part10
sfr JPEG_DcVlcCode11      =  0xdc;      // 0x0b         JPEG DC VLC code part11
sfr JPEG_DcVlcCode12      =  0xdd;      // 0x0b         JPEG DC VLC code part12
sfr JPEG_DcVlcCode13      =  0xde;      // 0x0b         JPEG DC VLC code part13
sfr JPEG_DcVlcCode14      =  0xdf;      // 0x0b         JPEG DC VLC code part14
sfr JPEG_DcVlcCode15      =  0xe1;      // 0x0b         JPEG DC VLC code part15
sfr JPEG_DcVlcCode16      =  0xe2;      // 0x0b         JPEG DC VLC code part16
sfr MemMapReg             =  0xe3;      // 0x0b         DECODER memory mapping register
sfr OutBase               =  0xe4;      // 0x0b         Output Buffer base address register
sfr Debug                 =  0xe5;      // 0x0b         Decoder debug register
////////////////////////////////////////////////////////////////////////////////////////////////
// RTC/CTC
sfr RTC_CTL0              =  0xa9;      // 0x0c         RTC Control 0 register
sfr RTCTimeS              =  0xaa;      // 0x0c         RTC Time Second Register
sfr RTCTimeMin            =  0xab;      // 0x0c         RTC Time Minute Register
sfr RTCTimeH              =  0xac;      // 0x0c         RTC Time Hour Register
sfr RTCTimeD              =  0xad;      // 0x0c         RTC Time Day Register
sfr RTCTimeMon            =  0xae;      // 0x0c         RTC Time Month Register
sfr RTCTimeYear           =  0xaf;      // 0x0c         RTC Time Year Register
sfr RTCAlmS               =  0xb0;      // 0x0c         RTC Alarm Second Register
sfr RTCALMM               =  0xb1;      // 0x0c         RTC Alarm Minute Register
sfr RTCALMH               =  0xb2;      // 0x0c         RTC Alarm Hour Register
sfr ADJCtrl               =  0xb3;      // 0x0c         adjust circuit control Register
sfr RTCrdm                =  0xb4;      // 0x0c         RTC Random access Register
sfr RTCREGUPDATE          =  0xb5;      // 0x0c         RTC Register update Register
sfr RTC_CTL1              =  0xb6;      // 0x0c         RTC Control 1 register
sfr TimerLB               =  0xb7;      // 0x0c         Timer low Byte
sfr TimerMB               =  0xb8;      // 0x0c         Timer middle Byte
sfr TimerHB               =  0xb9;      // 0x0c         Timer high Byte
sfr WDCtl                 =  0xba;      // 0x0c         watch dog control register
sfr CTCCtl                =  0xbb;      // 0x0c         CTC control register
sfr CTCCNTL               =  0xbc;      // 0x0c         CTC counter low register
sfr CTCCNTH               =  0xbd;      // 0x0c         CTC counter high register
sfr RTCrdm_2              =  0xc8;      // 0x0c         RTC Random access Register2
sfr RTCrdm_3              =  0xc9;      // 0x0c         RTC Random access Register3
sfr RTCrdm_4              =  0xca;      // 0x0c         RTC Random access Register4
////////////////////////////////////////////////////////////////////////////////////////////////
// ReakTek sound effect
sfr SPK3DCTL              =  0x90;

#endif

















































































































































































































































































































































































































































































































































































































































