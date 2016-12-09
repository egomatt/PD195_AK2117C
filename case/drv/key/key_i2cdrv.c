#include "actos.h"
#include "chip_cfg.h"


/* yuhu 20110527
//˵��:����Key_code�Ŀռ䲻�㣬���β��־���Ϊ���ڳ��ռ�



	PIOBDAT |= (0x01 << SCL_NUM);
	GPIOBDAT |= (0x01 << SDA_NUM); 

	GPIOBDAT &= ~(0x01 << SDA_NUM);
	GPIOBDAT &= ~(0x01 << SCL_NUM);

	

	GPIOBINEN |= (0x01 << SCL_NUM);
	GPIOBINEN |= (0x01 << SDA_NUM); 

	GPIOBINEN &= ~(0x01 << SDA_NUM);
	GPIOBINEN &= ~(0x01 << SCL_NUM);


	GPIOBOUTEN |= (0x01 << SCL_NUM);
	GPIOBOUTEN |= (0x01 << SDA_NUM); 

	GPIOBOUTEN &= ~(0x01 << SDA_NUM);
	GPIOBOUTEN &= ~(0x01 << SCL_NUM);


*/
#pragma renamecode(?PR?KY_IIC_DRV)
 

#define		SDA_NUM		0x04
#define   	SCL_NUM		0x05



void I2C_Wait5us(void)
{
    uint8 i;
    for (i = 10; i > 0; i--)    //10
    {
        ; //������ʱ
    }
}
 

void I2C_Start(void)
{
	uint8 sfr_bak;
    sfr_bak = SFR_BANK;
    SFR_BANK = BANK_GPIO;
	GPIOBDAT |= (0x01 << SDA_NUM); 
	I2C_Wait5us();
	GPIOBDAT |= (0x01 << SCL_NUM);
	I2C_Wait5us();
	GPIOBDAT &= ~(0x01 << SDA_NUM);
	I2C_Wait5us();
	GPIOBDAT &= ~(0x01 << SCL_NUM);
	I2C_Wait5us();
	SFR_BANK = sfr_bak;
}

void I2C_Stop(void)
{
	uint8 sfr_bak;
    sfr_bak = SFR_BANK;
    SFR_BANK = BANK_GPIO;
	GPIOBDAT &= ~(0x01 << SDA_NUM);
	I2C_Wait5us();
    GPIOBDAT |= (0x01 << SCL_NUM);
    I2C_Wait5us();
    GPIOBDAT |= (0x01 << SDA_NUM);
	I2C_Wait5us();
	GPIOBDAT &= ~(0x01 << SCL_NUM);
	I2C_Wait5us();
	SFR_BANK = sfr_bak;
}

void I2C_Init(void)
{
    uint8 sfr_bak;
    sfr_bak = SFR_BANK;
    SFR_BANK = BANK_GPIO;
	
	GPIOBDAT &= ~(0x01 << SCL_NUM);
	GPIOBDAT &= ~(0x01 << SDA_NUM); 
	
	GPIOBINEN &= ~(0x01 << SCL_NUM);
	GPIOBINEN &= ~(0x01 << SDA_NUM);

	GPIOBOUTEN |= (0x01 << SDA_NUM);
	GPIOBOUTEN |= (0x01 << SCL_NUM);
	  
	SFR_BANK = sfr_bak;

	I2C_Stop();
}

uint8 I2C_TransByte(uint8 param1) //ret: 1-ok, 0-notok
{
//    uint8 IOStatus;
    uint8 sfr_bak;
	uint8 IOStatus;
	uint8 i;
    sfr_bak = SFR_BANK;
    SFR_BANK = BANK_GPIO;


	//sendbyte
	for (i = 0; i < 8; i++)
    {
        if (((param1 << i) & 0x80) != 0)
        {
            GPIOBDAT |= (0x01 << SDA_NUM);
        }
        else
        {
            GPIOBDAT &= ~(0x01 << SDA_NUM);
        }
        I2C_Wait5us();
        GPIOBDAT |= (0x01 << SCL_NUM);
        I2C_Wait5us();
        GPIOBDAT &= ~(0x01 << SCL_NUM);
    }

    GPIOBOUTEN &= ~(0x01 << SDA_NUM);
    GPIOBINEN |= (0x01 << SDA_NUM);
    I2C_Wait5us();
    GPIOBDAT |= (0x01 << SCL_NUM);
    I2C_Wait5us();

#if 1	//���ڿռ䲻�㣬��δ����д��

    IOStatus = GPIOBDAT & (0x01 << SDA_NUM);
    if (IOStatus != 0)
    {
        GPIOBDAT &= ~(0x01 << SDA_NUM);
		GPIOBINEN &= ~(0x01 << SDA_NUM);
        GPIOBOUTEN |= (0x01 << SDA_NUM); 
        I2C_Stop();
        /*
        GPIOBDAT |= (0x01 << SDA_NUM);
		GPIOBINEN &= ~(0x01 << SDA_NUM);
        GPIOBOUTEN |= (0x01 << SDA_NUM); 
        I2C_Wait5us();
        GPIOBDAT &= ~(0x01 << SCL_NUM);
        GPIOBDAT &= ~(0x01 << SDA_NUM);
		I2C_Wait5us();
		*/
        SFR_BANK = sfr_bak;
        return FALSE;
    }
    else
    {
        GPIOBDAT &= ~(0x01 << SCL_NUM);
        I2C_Wait5us();
		GPIOBDAT &= ~(0x01 << SDA_NUM);
		GPIOBINEN &= ~(0x01 << SDA_NUM);
        GPIOBOUTEN |= (0x01 << SDA_NUM); 
        I2C_Wait5us();
    }

    SFR_BANK = sfr_bak;
    return TRUE;
    
#else
 			//����para��Ϊ0�����Բ��ö�para2���ж�
 			//���IOStatus�����ǲ���0��Ҫִ��������룬��IOStatusҲ�����ж�
 			//ͬʱ�ⲿ���øú���ʱҲ����Ҫ�жϷ���ֵ�����Բ���IOStatus�ǲ���0�� return false;
 			//20110526

			GPIOBDAT &= ~(0x01 << SDA_NUM);
            GPIOBOUTEN |= (0x01 << SDA_NUM); 
            I2C_Wait5us();
            GPIOBDAT &= ~(0x01 << SCL_NUM);
 
        SFR_BANK = sfr_bak;
        return FALSE;
    

#endif
}



uint8 I2C_ReceByte(uint8 param)
{
    uint8 IOStatus2; //IOStatus1, 
    uint8 sfr_bak;
    uint8 result = 0;
    uint8 temp = 0;
    uint8 i;
    sfr_bak = SFR_BANK;
    SFR_BANK = BANK_GPIO;


    GPIOBINEN |= (0x01 << SDA_NUM);
    GPIOBOUTEN &= ~(0x01 << SDA_NUM);
    I2C_Wait5us();
    for (i = 0; i < 8; i++)
    {
        GPIOBDAT |= (0x01 << SCL_NUM);
        I2C_Wait5us();
        result <<= 1;
        IOStatus2 = GPIOBDAT & (0x01 << SDA_NUM);
        if (IOStatus2 != 0)
        {
            result += 0x01;
        }
        GPIOBDAT &= ~(0x01 << SCL_NUM);        
		I2C_Wait5us();
    }


		if (param == 0)
    	{
	        GPIOBDAT &= ~(0x01 << SDA_NUM);
	        GPIOBOUTEN |= (0x01 << SDA_NUM); 
	        GPIOBINEN &= ~(0x01 << SDA_NUM);
	        I2C_Wait5us();
		}
		else
		{
			GPIOBDAT |= (0x01 << SDA_NUM);
	        GPIOBOUTEN |= (0x01 << SDA_NUM); 
	        GPIOBINEN &= ~(0x01 << SDA_NUM);
	        I2C_Wait5us();
		}

		GPIOBDAT |= (0x01 << SCL_NUM);
        I2C_Wait5us(); 
		GPIOBDAT &= ~(0x01 << SCL_NUM);
		I2C_Wait5us(); 

	
    SFR_BANK = sfr_bak;
    return result;
}


void SetMfpToNand(void)
{ 
    uint8 sfr_bak;
    sfr_bak = SFR_BANK;
    SFR_BANK = BANK_GPIO;
    GPIOBOUTEN &= ~(0x01 << SDA_NUM);
    GPIOBOUTEN &= ~(0x01 << SCL_NUM);
    GPIOBINEN &= ~(0x01 << SDA_NUM);
    GPIOBINEN &= ~(0x01 << SCL_NUM);
    SFR_BANK = sfr_bak;
}

