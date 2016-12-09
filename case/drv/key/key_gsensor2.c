#include "actos.h"
#include "chip_cfg.h"
#include "ap_cfg.h"
#include "math.h"

extern void SetMfpToNand(void);
extern void I2C_Init(void);
extern void I2C_Start(void);
extern void I2C_Stop(void);
extern uint8 I2C_TransByte(uint8 param1);
extern uint8 I2C_ReceByte(uint8 param);

extern uint8 gsensor_posflag;
extern uint32 step_counter;		//20110524	���˶�����
extern uint32 step_counter_swim;		//20110524	���˶�����
extern uint32 step_second_swim;
extern uint16 gsensor_xadc1;
extern uint16 gsensor_yadc1;
extern uint16 gsensor_zadc1;
extern uint16 gsensor_gap;
extern uint16 gsensor_gap_last;
extern uint16 gsensor_got;
extern uint8 g_debounce;
extern uint8 earOutFlag;
extern uint32 gf_tmp32;


#pragma renamecode(?PR?KY_GSENSOR2)

/*
 *******************************************************************************
 *                MMA8452
 *
 * Descriptor:
 *
 * Arguments: VOID
 *
 * Returns:   VOID
 *******************************************************************************
 */
bool sKY_GsensorOn2()
{
    uint8 ret=1;
    
    disable_interrupt();    //��ֹ�ж�
    
    I2C_Init();	
    I2C_Start();
    ret&=I2C_TransByte(0x3a);    //--addr--
	ret&=I2C_TransByte(0x2a);
	ret&=I2C_TransByte(0x19);    //����
	I2C_Stop();

    SetMfpToNand();
 	enable_interrupt();     //���ж�

 	return ret;
}

void sKY_GsensorOff2()
{
    disable_interrupt(); //��ֹ�ж�
    
    I2C_Init();
	I2C_Start();
	I2C_TransByte(0x3a); //--addr--
	I2C_TransByte(0x2a);
	I2C_TransByte(0x18);	//˯��
	I2C_Stop();

    SetMfpToNand();
	enable_interrupt(); //���ж�
}

/*
 *******************************************************************************
 *                sKY_ReadGSensor{RPrefix}:
 *
 * Descriptor:
 *
 * Arguments: VOID
 *
 * Returns:   VOID
 *******************************************************************************
 */
void sKY_ReadGSensor2(void)
{
	uint8 xtemp,ytemp,ztemp;
	
	disable_interrupt(); //��ֹ�ж�
	
    I2C_Init();
	I2C_Start();
	I2C_TransByte(0x3a); //--addr--
	I2C_TransByte(0x01); //0x01
	//I2C_Stop();
    I2C_Start();
	I2C_TransByte(0x3b); //--addr--
    gsensor_xadc1 = I2C_ReceByte(0x00); 
	xtemp = I2C_ReceByte(0x00); 
    gsensor_yadc1 = I2C_ReceByte(0x00);
	ytemp = I2C_ReceByte(0x00);
	gsensor_zadc1 = I2C_ReceByte(0x00);
	ztemp = I2C_ReceByte(0x01);
	I2C_Stop();
	
    SetMfpToNand();
	enable_interrupt(); //���ж�
	
	gsensor_xadc1 = (gsensor_xadc1 << 8) | xtemp; 
	gsensor_yadc1 = (gsensor_yadc1 << 8) | ytemp; 
	gsensor_zadc1 = (gsensor_zadc1 << 8) | ztemp; 
	//12bit
    gsensor_xadc1 = (int16)gsensor_xadc1 >> 4;
    gsensor_yadc1 = (int16)gsensor_yadc1 >> 4;
    gsensor_zadc1 = (int16)gsensor_zadc1 >> 4;
    
    gsensor_xadc1 = (int16)gsensor_xadc1 + 2048; 
	gsensor_yadc1 = (int16)gsensor_yadc1 + 2048; 
	gsensor_zadc1 = (int16)gsensor_zadc1 + 2048; 
  
}

