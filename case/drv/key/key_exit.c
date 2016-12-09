#include "actos.h"
#include "key.h"
#include "time.h"

extern void ctc_isr(void);
extern void cld_overflow_isr(void);

extern uint8 sysverflag;

#pragma renamecode(?PR?BAC_IE1)

void key_exit(void)
{
		/*���ٶȴ�������RCODE�ռ䲻������ֱ����IICд���˴�*/
		sKY_GsensorOff();   //pedometer
		
	TM_KillTimer(0x02); //�ر�RTC
    IE0 &= ~(1 << 1); //disable IE.1
    IRQ_Release((uint16) ctc_isr, IRQ_CTC);

    if (sysverflag != 0)
    {
        IRQ_Release((uint16) cld_overflow_isr, IRQ_ADC);
    }
}
