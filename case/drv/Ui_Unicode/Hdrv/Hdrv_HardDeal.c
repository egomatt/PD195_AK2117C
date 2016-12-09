#include "actos.h"
#include "display.h"

#include "Hdrv_S7565.h"

#pragma renamecode(HARD_DEAL)

//MFP�е�EXT bus
void Hdrv_SetMfpToEmif(void)
{
    uint8 sfr_bak;
    sfr_bak = SFR_BANK;
    //set MFP_GPIO Bank
    SFR_BANK = BANK_GPIO;

    MFPSEL0 |= 0xa1;
    //set GPIOD[7:0] to EMIF Data[7:0]
    MFPSEL0 &= 0xbd;
    MFPSEL2 &= 0x7f;    //matt140303#1
    // set GPIOB4 to Lcd RS
    MFPSEL1 &= 0xfc;
    MFPSEL1 |= 0x02;
    SFR_BANK = sfr_bak;
}

void StoreCE(void)
{
    uint8 sfr_bak;
    if (!reent_flag)
    {
        disable_interrupt(); //���ж�

        //ƬѡCE
        //matt140303#1
        /*
        sfr_bak = SFR_BANK;		
        Hdrv_SetMfpToEmif(); //set MFP to External Memory I/F
        SFR_BANK = BANK_GPIO;
		CE_GIO_EN_REG |= CE_EN_BIT;				
        cesel = EXTMEM_CTL & 0x7;
        EXTMEM_CTL = (EXTMEM_CTL & 0xf8) | 0x5;
		CE_GIO_DATA_REG &= CE_CLR_BIT;
        SFR_BANK = sfr_bak;
        */
        Hdrv_SetMfpToEmif(); //set MFP to External Memory I/F
        sfr_bak = SFR_BANK;
        //store ce 
        SFR_BANK = BANK_LCD;
        EXTMEM_CTL = (EXTMEM_CTL & 0xf8) | 0x5;
        SFR_BANK = sfr_bak;
    }
    reent_flag++;
}

void RestoreCE(void)
{
    uint8 sfr_bak;
    reent_flag--;
    if (!reent_flag)
    {
        //�ͷ�CE
        //matt140303#1
        /*
        sfr_bak = SFR_BANK;
        SFR_BANK = BANK_GPIO;
        EXTMEM_CTL = (EXTMEM_CTL & 0xf8) | cesel;
		CE_GIO_DATA_REG |= CE_SET_BIT;
		CE_GIO_EN_REG &= CE_DN_BIT;
        SFR_BANK = sfr_bak;	
        */
        sfr_bak = SFR_BANK;
        SFR_BANK = BANK_LCD;
        EXTMEM_CTL &= 0xf8;
        SFR_BANK = sfr_bak;

        enable_interrupt(); //���ж�
    }
    else if (reent_flag < 0)
    {
        reent_flag = 0;
    }
    else
    {
        ;//fo nothing
    }
}

/*void Write_Lcm(uint8 dat) //������д����
{
    uint8 i;
    uint8 sfr_bak;
    sfr_bak = SFR_BANK;

    SFR_BANK = BANK_GPIO;

    SCLK_GIO_DATA_REG &= SCLK_CLR_BIT;
    SCLK_GIO_EN_REG |= SCLK_GIO_EN_BIT;
    SDIN_GIO_EN_REG |= SDIN_GIO_EN_BIT;

    for (i=0; i<8; i++)
    {
        if ((dat & 0x80) != 0)
        {
            SDIN_GIO_DATA_REG |= SDIN_SET_BIT;
        }
        else
        {
            SDIN_GIO_DATA_REG &= SDIN_CLR_BIT;
        }

        SCLK_GIO_DATA_REG &= SCLK_CLR_BIT;
        _nop_();
        _nop_();
        _nop_();
        SCLK_GIO_DATA_REG |= SCLK_SET_BIT;
        _nop_();
        _nop_();
        _nop_();
        dat = (dat << 1);
    }

    SCLK_GIO_EN_REG &= SCLK_GIO_DN_BIT;
    SDIN_GIO_EN_REG &= SDIN_GIO_DN_BIT;
    SFR_BANK = sfr_bak;
}*/

void Write_Command(uint8 cmd)
{
    //дCMD
    uint8 IE0Bak = IE0;
    uint8 sfr_bak;
    sfr_bak = SFR_BANK;

	IE0 = IE0 & 0x7f; //��ʾʱ�����жϣ��������жϷ����������ήƵ��
    SFR_BANK = BANK_GPIO;

	//EXTMEM_CTL &= 0xf7;
	LCMRS_GIO_EN_REG |= LCMRS_GIO_EN_BIT;
	LCMRS_GIO_DATA_REG &= LCMRS_CLR_BIT;

#if 1 //������
    EXTMEM_DL = cmd;
#else //������
    Write_Lcm(cmd);
#endif
	LCMRS_GIO_EN_REG &= LCMRS_GIO_DN_BIT;
    SFR_BANK = sfr_bak;
	IE0 = IE0Bak;
}

void Write_Data(uint8* mydata, uint16 len)
{
    //дdata
    uint8 IE0Bak = IE0;
    uint8 sfr_bak;
    sfr_bak = SFR_BANK;

	IE0 = IE0 & 0x7f; //��ʾʱ�����жϣ��������жϷ����������ήƵ��
    SFR_BANK = BANK_GPIO;

	//EXTMEM_CTL |= 0x08;
	LCMRS_GIO_EN_REG |= LCMRS_GIO_EN_BIT;
	LCMRS_GIO_DATA_REG |= LCMRS_SET_BIT;
    
    while (len != 0)
    {
        len--;
#if 1 //������
        EXTMEM_DL = *mydata;
#else //������
        Write_Lcm(*mydata);
#endif
        mydata++;
    }
	LCMRS_GIO_EN_REG &= LCMRS_GIO_DN_BIT;
    SFR_BANK = sfr_bak;
	IE0 = IE0Bak;
}

void Hdrv_LightOnOff(bool blctl)
{
    uint8 sfr_bak = SFR_BANK;

    SFR_BANK = BANK_GPIO; //set GPIO Bank
    #if(MACHINE_MODEL==1)
    //ENABLE GPIOC.1
	LCMBL_GIO_EN_REG |= LCMBL_GIO_EN_BIT;
	#endif
    if (blctl == TRUE) //������
    {
        #if(MACHINE_MODEL==1)
        //SETB GPIOC.1
		LCMBL_GIO_DATA_REG |= LCMBL_SET_BIT;
		#endif
		StoreCE();
		#if(MACHINE_MODEL!=1)
		#if(LCD_TYPE==4)
		Write_Command(0x8d);
		Write_Command(0x14);
		#elif(LCD_TYPE==5)
		Write_Command(0xad);
		Write_Command(0x8b);
		#endif
		#endif
		Write_Command(0xaf);
		RestoreCE();     
    }
    else //�ر���
    {   
        StoreCE();
        Write_Command(0xae);
        #if(LCD_TYPE==4)
		Write_Command(0x8d);
		Write_Command(0x10);
		#elif(LCD_TYPE==5)
		Write_Command(0xad);
		Write_Command(0x8a);
		#endif
		RestoreCE();   
		#if(MACHINE_MODEL==1)
        //clr GPIOC.1
		LCMBL_GIO_DATA_REG &= LCMBL_CLR_BIT;
		#endif
    }
    SFR_BANK = sfr_bak;
}