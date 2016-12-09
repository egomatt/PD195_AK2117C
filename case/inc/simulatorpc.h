#ifndef __SIMULATORPC_H__
#define __SIMULATORPC_H__

#define  ColorScrnPC

#define  DialogWidth 320
#define  DialogHeight 400


#ifdef PC
	#define NAPDEBUG  
	#define non_banked  
	#define banked  
	#define reentrant  
	#define bool unsigned char  
	#define word unsigned short  
	#define dword unsigned long  
	#define sfr  static unsigned short  
	_declspec(dllimport) char* GetRealAddrPC(unsigned int addr);  
	#define	GET_REAL_ADDR   GetRealAddrPC  
	typedef  unsigned char byte;
	#define DLL_IMP __declspec(dllimport)  
	#define sbit  static unsigned char  
	#define far	    
	#define near   
	#define xdata 
	#define REG4E_RTC_WATCH_DOG		99
	#define __C251__				    
	#define huge     
	#define large     

	// ˫�ֽڸߵ�λ����
	void SIM_ChgWordPC(unsigned char *p);
	// 4�ֽڸߵ�λ����
	void SIM_ChgLongPC(unsigned char *p);
	void SIM_Sleep(int millsec);

	// Unicode �ַ����ߵ��ֽڽ���
	void SIM_ChgStrUni(unsigned char *pStr);
	// ASCII �ַ����ߵ��ֽڽ���
	void SIM_ChgStrAsc(unsigned char *pStr);
#else
	#define GET_REAL_ADDR  
	#define DLL_IMP 
	#define	SIM_ChgWordPC(x)       
	#define	SIM_ChgLongPC(x)    
	#define	SIM_Sleep(x)    
	#define	SIM_ChgStrUni(x)       
	#define	SIM_ChgStrAsc(x)    
#endif
#endif
