#ifndef _CONFIG_H
#define _CONFIG_H
#include "typeext.h"


/*********************�ܿؿ������� *************************/ 
//#define 	_ACTSIM_                      1
//#define 	_SIMBOOT_ 				      			0
#define 	_ACTDEBUG_                    1
#define 	_HUGEMODE_                    0
//#define     _SUPORT_BANKDATA_INIT_		  1
/*********************�ܹ������� *************************/ 
#define STARTADDR 00fd00h 
#define TRAP_HANDLER STARTADDR+07bh 

 

/*********************���Կ����� *************************/ 
#ifdef __C251__

  #ifdef _ACTDEBUG_ 
    #define MSG(x) putstr(x)
    extern void putstr(int8 *);
  #else
    #define MSG(x)
  #endif 
#endif
/*****************��������******************************/







#endif