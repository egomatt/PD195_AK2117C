#ifndef _ID3PICDEC_H
#define _ID3PICDEC_H

#include "typeext.h"
//#include "display.h"

#include "ak211x.h"
#ifndef PC
sbit MemBankCtl_PCMMU2=MemBankCtl^3;
#else
sbit MemBankCtl_PCMMU2;
#endif
 
//for lib compile
#define JPEG_RAM_INDEX     1
#define MURAM1L_RAM_INDEX  2

extern void DMA3Init(uint8 pictype);

extern bool DMA3StartTrans(uint8 dma3_ram);	 //开始DMA3传输，dma3_ram=0, jpeg ram;dma3_ram=1,muram1m. ret=true---success; false---fail

extern void DMA3SetCounter(uint8 row, uint16 col);	 //设置DMA3 counter,注意length不要超过min[DMA3counter， Memory length]
extern void DMA3SetSrcAddr(uint16 src_addr);  //设置memory起始地址 


extern void DMA0Init(uint8 pictype);

extern bool DMA0StartTrans(uint8 dma0_ram);	 //开始DMA3传输，dma3_ram=0, jpeg ram;dma3_ram=1,muram1m. ret=true---success; false---fail

extern void DMA0SetCounter(uint16 data_width, uint8 data_height);	 //设置DMA3 counter,注意length不要超过min[DMA3counter， Memory length]
extern void DMA0SetSrcAddr(uint16 src_addr0,uint16 src_addr1,uint16 src_addr2);  //设置memory起始地址
#define PREVIEW_PLAY 0
#define NORMAL_PLAY	 1


typedef enum
{
    Pic_JPG,
    Pic_BMP,
    Pic_GIF   
}Pic_type_t;

typedef enum
{
    RGB565,
    RGB888  
}Bmp_type_t;

typedef  struct
{
	uint16 x;
    uint16 y;
    uint16 w;
    uint16 h;
}wregion_t;

typedef  struct
{
#ifndef PC
    uint16 fp;
#else
    IntAddr fp;				  
#endif
	wregion_t   *pregion1;	  	  //show region
    wregion_t   *pregion2;       //setwindow region 
    uint16  	offset;	      //file offset
    Bmp_type_t  bmpType;	  //BMP type(RGB565/RGB888)
    uint8 playtype;      	//show type	bit   0: auto rotation; 1: preview 	 ; 0 no; 1 yes
	uint8 reserved;       // reserved for if winh2v
}ColorLcmInfor;

#define PICDEC_OK            0
#define PICDEC_ERROR         -1
#define PICDEC_UNSUPPORTED   -2
#define PICDEC_STRM_ERROR    -3
#define PICDEC_TIMEOUT       -4	

uint8 PicViewInit(ColorLcmInfor *picInfor, Pic_type_t pictype);
void PicViewExit(Pic_type_t pictype);
uint8 PicDec(ColorLcmInfor *picInfor, Pic_type_t pictype);
uint8  JPEGInit(ColorLcmInfor *picInfor);
void JPEGExit(void);
uint8 JPEGDec( ColorLcmInfor *picInfor);

extern uint8 PicPlay(ColorLcmInfor* pPicInfo, Pic_type_t pictype);

//pic_common content
uint8  mygetchar(void);
void get512byte(void);
void fileinit(handle fp,uint16 offset);
void FileReset(void);
void  seektodata(uint16  pos);

uint16 readint16le(void);	//little endian
uint16 readint16be(void);	//big endian
uint32 readint32le(void);
uint32 readint32be(void);
uint16 readint16(void);	
uint32 readint32(void);

void storeclk(void);
void restoreclk(void);
//===========================
//for picdata header
#define source_buf 0xdc00 
#define dest 0x3000



//extern uint8 zigzag[64];
///////////JPEG DEC////////////////
extern int16  near source_count;
extern uint8* near psrc ;
extern handle near picfp;

extern uint8 near DC_TAB0[16] ;
extern uint8 near DC_TAB1[16] ;
extern uint8 near AC_TAB0[16] ;
extern uint8 near AC_TAB1[16] ;
extern uint8   near     WindowModeH2V   ;
extern uint16 near imgHigh ;
extern uint16 near imgWide;
extern uint8  near	inputmode  ;
extern uint8  near qtabs   ;
extern uint8  near dcactab ;
extern uint8  near H_SampFactor[4] ;
extern uint8  near V_SampFactor[4] ;
extern uint8  near downscale;
extern uint16 near decHigh;
extern uint16 near decWide;
extern uint16 near showlength;//for lcd show
extern uint16 near showlines;//
extern uint16 near jpglcd_high;
extern uint16 near jpglcd_wide;
extern uint16 near jpglcd_x;
extern uint16 near jpglcd_y;




extern uint8  near reg_adh_bak;
extern uint8  near reg_aeh_bak;
extern uint8  near reg_afh_bak;
extern uint8  near reg_bfh_bak;//jram123

extern uint8  near reg_b1h_bak;
extern uint8  near reg_b4h_bak;//dram251b
///////////////////////////////////////////




extern uint8   near     readfileflag  ;


extern wregion_t near sregion_in;
extern wregion_t near sregion_out;

//================================
//for jpeg dec 
extern uint8 Readjpghead(void);
extern void writehuftoreg(void);

//===============================

//for some special register

//commom reg

#ifndef  PC

sbit MemMapReg_OutFormatSel =MemMapReg^4;
sbit MRCR2_VideoReset = MRCR2^6;//ve 0 reset, 1 normal
sbit MCSR3_VECLKEN = MCSR3^5;  //ve clk enable
 // CPUCLKCTL, VECLKCTL, 
sbit MCSR5_JRAM23CLK=MCSR5^3;	  //jram23 clk:  0 cpu,1 ve
sbit MCSR5_PCMRAMRCLK =MCSR5^7;//PCMRAMR  clk, 0 cpu, 1ve
sbit MCSR5_PCMRAMLCLK1 =MCSR5^2;//PCMRAML  clk, 00 cpu, 01ae, 11ve
sbit MCSR5_PCMRAMLCLK0 =MCSR5^1;//

sbit MCSR6_MURAM2CLK1=MCSR6^3;	 //muram2 clk ,00 cpu, 01  ae, 11 ve;
sbit MCSR6_MURAM2CLK0=MCSR6^2;
sbit MCSR6_JRAM0CLK=MCSR6^0;  //jram0 clk, 0 cpu ,1 ve

sbit MCSR7_MURAM1HCLK =MCSR7^2;//muram1h clk 0cpu,1 ae
sbit MCSR7_JRAM1CLK=MCSR7^0;	 //jram1 clk  ,0 cpu 1 ve
	  	 
sbit MCSR8_MURAM1MCLK1=MCSR8^6;//muram1m clk 00cpu,01ae,11ve
sbit MCSR8_MURAM1MCLK0=MCSR8^5;	  
sbit MCSR8_MURAM1LCLK1=MCSR8^3;//muram1lclk 00cpu,01ae,11ve
sbit MCSR8_MURAM1LCLK0=MCSR8^2;

sbit MCSR9_DRAM251BCLK=MCSR9^0; //dram251b clk 0cpu,1 ve

sbit MCSR11_MURAM1HCLKEN=MCSR11^6;
sbit MCSR11_MURAM1MCLKEN=MCSR11^5;
sbit MCSR11_MURAM1LCLKEN=MCSR11^4;
sbit MCSR11_D251BCLKEN=MCSR11^3;
sbit MCSR11_MURAM2CLKEN=MCSR11^1;
sbit MCSR11_PCMRAMLCLKEN=MCSR11^0;

sbit MCSR12_PCMRAMRCLKEN=MCSR12^5;
sbit MCSR12_JRAM23CLKEN=MCSR12^4;
sbit MCSR12_JRAM1CLKEN=MCSR12^3;
sbit MCSR12_JRAM0CLKEN=MCSR12^2;

sbit  AMVDecConfig_end=AMVDecConfig^0;
sbit AMVEN_TimeOutInt =AMVEN^2;
sbit AMVEN_IntDis =AMVEN^1;
sbit AMVEN_DecStart =AMVEN^0;
sbit AMVIE_Timeout=AMVIE^5;
sbit AMVIE_SliceOK=AMVIE^4;
sbit AMVIE_StreamError=AMVIE^3;
sbit AMVIE_BufferEmpty=AMVIE^2;
sbit AMVIE_DecReady=AMVIE^1;
sbit AMVIE_IntBit=AMVIE^0;

sbit MemMapReg_JRAM01Map=MemMapReg^0;
sbit Debug_Endian = Debug^3;
sbit MCUPLLCTL_EN  = MCUPLL_CTL^0;

#else
sbit MemMapReg_OutFormatSel;
sbit MRCR2_VideoReset;//ve 0 reset, 1 normal
sbit MCSR3_VECLKEN;  //ve clk enable
 // CPUCLKCTL, VECLKCTL, 
sbit MCSR5_JRAM23CLK;	  //jram23 clk:  0 cpu,1 ve
sbit MCSR5_PCMRAMRCLK;//PCMRAMR  clk, 0 cpu, 1ve
sbit MCSR5_PCMRAMLCLK1;//PCMRAML  clk, 00 cpu, 01ae, 11ve
sbit MCSR5_PCMRAMLCLK0;//

sbit MCSR6_MURAM2CLK1;	 //muram2 clk ,00 cpu, 01  ae, 11 ve;
sbit MCSR6_MURAM2CLK0;
sbit MCSR6_JRAM0CLK;  //jram0 clk, 0 cpu ,1 ve

sbit MCSR7_MURAM1HCLK;//muram1h clk 0cpu,1 ae
sbit MCSR7_JRAM1CLK;	 //jram1 clk  ,0 cpu 1 ve
	  	 
sbit MCSR8_MURAM1MCLK1;//muram1m clk 00cpu,01ae,11ve
sbit MCSR8_MURAM1MCLK0;	  
sbit MCSR8_MURAM1LCLK1;//muram1lclk 00cpu,01ae,11ve
sbit MCSR8_MURAM1LCLK0;

sbit MCSR9_DRAM251BCLK; //dram251b clk 0cpu,1 ve

sbit MCSR11_MURAM1HCLKEN;
sbit MCSR11_MURAM1MCLKEN;
sbit MCSR11_MURAM1LCLKEN;
sbit MCSR11_D251BCLKEN;
sbit MCSR11_MURAM2CLKEN;
sbit MCSR11_PCMRAMLCLKEN;

sbit MCSR12_PCMRAMRCLKEN;
sbit MCSR12_JRAM23CLKEN;
sbit MCSR12_JRAM1CLKEN;
sbit MCSR12_JRAM0CLKEN;

sbit  AMVDecConfig_end;
sbit AMVEN_TimeOutInt;
sbit AMVEN_IntDis;
sbit AMVEN_DecStart;
sbit AMVIE_Timeout;
sbit AMVIE_SliceOK;
sbit AMVIE_StreamError;
sbit AMVIE_BufferEmpty;
sbit AMVIE_DecReady;
sbit AMVIE_IntBit;

sbit MemMapReg_JRAM01Map;
sbit Debug_Endian;
sbit MCUPLLCTL_EN;

#endif



//=========================


#endif
