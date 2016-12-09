/****************************************************************
NAME:     TypeExt.h 
ABSTRACT: Define extended type 
Copyright (c) 2003 by Actions, All Rights Reserved.


*****************************************************************/
#ifndef _TYPEEXT_H
#define _TYPEEXT_H


 #ifdef PC
 #define __C251__
 #define xdata
#include "simulatorpc.h"
 #endif

 #ifdef __C251__
 #define OLD_TYPEEXT

 // typedef unsigned char   bool;              // bool ����
 #define  bool unsigned char

 typedef unsigned char   uint8;            // �޷���8λ���ͱ��� //
 typedef signed char     int8;             // �з���8λ���ͱ��� //

   #ifndef PC
    typedef unsigned int    uint16;           // �޷���16λ���ͱ��� //
    typedef signed int      int16;            // �з���16λ���ͱ��� //
   #else
    typedef unsigned short    uint16;           // �޷���16λ���ͱ��� //
    typedef signed short      int16;            // �з���16λ���ͱ��� //
   #endif //endof ifndef PC

 typedef unsigned int    IntAddr;		 // һ���ñ��������ַ�������ڲ�ͬϵͳ�£��õ�ַ����в��죬
											 // �����ض��ڸñ������ͣ�������봦����ĺ괦��

 typedef unsigned long   uint32;           // �޷���32λ���ͱ��� //
 typedef signed long     int32;            // �з���32λ���ͱ��� // 

 
#ifdef OLD_TYPEEXT
#define  uchar unsigned char
#ifndef __DEC
// #define  uchar unsigned char
#define  BYTE unsigned char
#ifndef PC //10-4-11 ��ֹ��windows.h��ͻ
#define  byte unsigned char
typedef unsigned char   BOOL;              // BOOL ����
#define  UCHAR unsigned char
#else
typedef  unsigned char byte;
#define BOOL unsigned char              // BOOL ����
typedef  unsigned char UCHAR;
#endif
    #endif	/* #ifndef __DEC */
    #define  WORD unsigned short
    #define  word unsigned short
    #define  DWORD unsigned long
    #define  dword unsigned long
    #define  HANDLE unsigned int
    #define  handle unsigned int
    #define  HANDLE32 unsigned long
    #define  handle32 unsigned long
    #define  ulong unsigned long
    #define  HANDLE32 unsigned long
  #endif //end of ifdef OLD_TYPEEXT
#endif //end of __C251__


 
#define  TRUE   1
#define  FALSE  0

#define  FOREVER 1
#define  NEVER 0

#define  uint unsigned short

#ifndef NULL
#define  NULL 0
#endif

#endif  /* _TYPEEXT_H*/
 