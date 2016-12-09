/*
********************************************************************************
*                       ACTOS
*                  file system include file
*                                                                         
*                (c) Copyright 2002-2003, Actions Co,Ld.                       
*                        All Right Reserved                               
*
* File   : filesys.h
* By     : DONGZH
* Version: 1> v1.00     first version     03-11-3 13:54
********************************************************************************
*/
#ifndef _MFS_INCLUDE
#define _MFS_INCLUDE

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�˴����c���Ժͻ�����Զ�Ҫʹ�õĶ���
#include "actapi.h"
#include "typeext.h"     
#include "uddrv.h" 

/*
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// file system type 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define FS_FAT12        0x00
#define FS_FAT16        0x01
#define FS_FAT32        0x02

//FIXME: complete these defines

//input parameter
#define FS_DIR_FILE             0x80    //  ��ʾ������������չ�����ӵ�ǰĿ¼��DIR��һ���ļ�
#define FS_DIR_DIR              0x81    //  ��ʾ������������չ�����ӵ�ǰĿ¼��DIR��һ��Ŀ¼
#define FS_DIR_FILE_CONT        0x00    //  ��ʾ������������չ����DIR��һ���ļ�
#define FS_DIR_DIR_CONT         0x01    //  ��ʾ������������չ����DIR��һ���ļ�

//#define FS_MODE_RONLY         0   //  ��ʱû��ʹ��
//#define FS_MODE_WONLY         1   //  ��ʱû��ʹ��
//#define FS_MODE_RW            2   //  ��ʱû��ʹ��

#define FS_OPEN_NORMAL          0x0   //  ��ʾ�������ļ�
#define FS_OPEN_NOSEEKBACK      0x1   //  ��ʾ��ʾ���ļ�ʱ���趨�ļ��������Ĳ�����Ϊ1��ʾ������¼����Ϣ����֧��Seekback
#define FS_OPEN_LN_NORMAL       0x80  //  ��ʾ���볤�ļ����������ļ�
#define FS_OPEN_LN_NOSEEKBACK   0x81  //  ��ʾ���볤�ļ������ļ�ʱ���趨�ļ��������Ĳ�����Ϊ1��ʾ������¼����Ϣ����֧��Seekback

#define FS_CD_ROOT              ":"   //  ��ʾ�ı䵱ǰĿ¼����Ŀ¼����ǰFAT16֧�֣�
#define FS_CD_PARENT            "\\"  //  ��ʾ�ı䵱ǰĿ¼����һ����Ŀ¼����ǰFAT16��֧�֣�
#define FS_CD_SON               "/"   //  ��ʾ�ı䵱ǰĿ¼����ǰĿ¼��ָ�����Ŀ¼����ǰFAT16��֧�֣�
 
#define FS_GET_SPACE_TOTAL      0x00  //  ��ʾ��ȡ������ǰ�������ܿռ��������   
#define FS_GET_SPACE_FREE       0x01  //  ��ʾ��ȡ������ǰ������ʣ��ռ��������

#define FS_BACKUP_RWINFO        0     //  ���ݵ�ǰ�Ķ�����д������״̬    
#define FS_BACKUP_BUFFER        1     //  �����ļ�ϵͳ��TempBuf
#define FS_BACKUP_DIRINFO       2     //  ���ݵ�ǰDir������״̬  

 
#define FS_RESTORE_RWINFO       0     //  ���ݱ��ݵĶ�����д������״̬
#define FS_RESTORE_BUFFER       1     //  �ָ����ݵ��ļ�ϵͳ��TempBuf
#define FS_RESTORE_DIRINFO      2     //    ���ݱ��ݵ�Dir������״̬

#define FS_SEEK_FFROMSTART      0     //  ���ļ���ʼ��ʼλ����ǰSeek����������ƫ����ָ��������
#define FS_SEEK_FFROMCUR        1     //  ���ļ���ǰ��ȡλ����ǰSeek����������ƫ����ָ��������
#define FS_SEEK_BFROMCUR        2     //  ���ļ���ǰ��ȡλ�����Seek����������ƫ����ָ��������


//ret
#define FS_INFO_RW_ERR          1   //   ���̶�д����
#define FS_INFO_WRITE_PROTECT   2   //   ����д����
#define FS_INFO_UNFORMATTED     3   //   ����δ��ʽ��
#define FS_INFO_OVEFLOW         4   //   �ļ����������ļ��߽磬��Ŀ¼��������Ŀ¼�߽�
#define FS_INFO_NOT_EXIST       5   //   �ļ�������Ŀ���ļ�����Ŀ¼������Ŀ¼��
#define FS_INFO_SPACE_OUT       6   //   ��ʾ�ļ�������Ŀ¼����û�д��̿ռ䣬����д���ݻ�����չ��Ŀ¼
#define FS_INFO_DIR_OUT         7   //   �ļ�������Ŀ¼Ŀ¼����
#define FS_INFO_NOT_EMPTY       8   //   ɾ��Ŀ¼ʱ���أ���ʾɾ����Ŀ¼Ϊ�ǿ�Ŀ¼
    
*/
#define FS_SDFFAPIEntry         0x2b00    /* for data exchange, SDCard fast driver entry  2008-9-28 14:29   */

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�˴����c���еĶ���
#ifdef __C251__
//

/*
//�ļ�����ʱ��
typedef struct
{
    WORD year;
    uchar month;
    uchar day;       
    uchar hour;
    uchar minute;
    uchar second; 
}fs_crt_time_t;

*/

//-----------------------------------------------------------------------------------

/*  MFS_Init()
 *  ��ʼ����ǰ�������ļ�ϵͳ������֮��ſ��Կ�ʼ�ļ�ϵͳ���ܵ���    
 */
bool   MFS_Init(void);

/*
*********************************************************************************************************
*Function    ��initialize FS by drive letter         2008-9-19 14:45
*
*Description��used specially for data copying, after DRV_SActiveDrive() and before FS_Init().
*
*Arguments   : letter   ---- drive letter, such as C--Flash, H--Card, L--HideDisk.
*
*Rerurns    ��0            failed
*             1            successful
*
*Other        :
*********************************************************************************************************
*/
bool MFS_DiskInit(char letter);

//-----------------------------------------------------------------------------------

/*  MFS_Exit()
 *  �������˳��ļ�ϵͳ���ܵ���  
 */
bool MFS_Exit(void);

//-----------------------------------------------------------------------------------

/*  MFS_CD()
 *����:  
    ���ݽ��û����뽫��ǰĿ¼ָ��ǰĿ¼����Ŀ¼����Ŀ¼��ֱ�ӷ��ظ�Ŀ¼ 
    strDirNameΪ�û�����8.3��Ŀ¼����(11��byte,��д,���㲹20h)��ʾ�ı䵱ǰĿ¼������Ŀ¼
 *����:  
    char *strNameΪ8.3��Ŀ¼����bufferָ��
    strName��һ���ַ�Ϊ"\"��5chʱ��ʾ�ı䵱ǰĿ¼����һ����Ŀ¼ 
    strName��һ���ַ�Ϊ":"��3ahʱ��ʾ�ı䵱ǰĿ¼����Ŀ¼
    strName��һ���ַ�Ϊ"/"��2fhʱ��ʾ�ı䵱ǰĿ¼����ǰĿ¼���Ӧ����Ŀ¼
 *����:
    0,1                          
 */
bool   MFS_CD(const char *strName);

//-----------------------------------------------------------------------------------

/*  MFS_Dir()
 *����:
    �ڵ�ǰĿ¼�¼����ļ�����Ŀ¼��ͨ���������ѡ�������Ŀ¼�����ļ�
    ���øú������ú����ӵ�ǰĿ¼��ָ�뿪ʼȡһ���ļ�����Ŀ¼������
    a.����FS_CD()����FS_SetCurDir()��һ��Ŀ¼Ȼ����Dir��Dir��������ʼĿ¼��ָ����Ϊ00000000h
    b.����FS_SetCurDirEntry()�趨һ��Ŀ¼�Dir��������ʼĿ¼��ָ����ΪFS_SetCurDirEntry()������ֵ
 *����:    
    DirType�����������DIR�Ķ���,0��DIR�ļ�,1��ʾDIRĿ¼,��ǰ�汾��FAT16��DirType��0x80��0x81��Ŀ¼�׿�ʼDir 
    char *StrName����������������ļ�����bufferָ��
    char *StrExtName����������չ����Ϣ,����DIR����������'*'��ʾdir���������ļ�
    char ExtNameNum ��ʾStrExtName����չ���ĸ���
 *����: 
    0,1                         
 */
 bool   MFS_Dir(char *StrName, const char *StrExtName, char type,char ExtNameNum);




//-----------------------------------------------------------------------------------

/*  MFS_GetCurDirEntry()
 *����:
    ȡ��ǰ���������ļ���Ӧ��Ŀ¼���ڵ�ǰĿ¼�е�ƫ���ֽ�
    ����Ŀ¼������ڵ�ǰĿ¼�׵�ƫ�ƣ�����ȷ����Ŀ¼���λ�ã�ͬʱָ��һ���ļ�������Ŀ¼
 *����:
    long DirEntry   
 */     
long   MFS_GetCurDirEntry(void);


//-----------------------------------------------------------------------------------

/*  MFS_SetCurDirEntry()
 *����:
    ��Ŀ¼��ƫ����Ϊ�������룬�����趨Dir������Ŀ¼��,
    ͨ���趨ƫ���趨Ŀ¼�����ָ��һ���ļ�������Ŀ¼,
    ��ָ���ļ��Ϳ��Զ��ļ�ֱ�Ӳ���,
    ��ָ����Ŀ¼����ɻ�ȡĿ¼��Ȼ��CD����Ŀ¼
 *����:
    long DirEntryΪ���趨��Ŀ¼�����Ŀ¼��ƫ��
 *����:
    0,1 
 */
bool   MFS_SetCurDirEntry(long DirEntry);


//-----------------------------------------------------------------------------------

/*  MFS_GetCurDir()
 *����:
    ȡ��ǰĿ¼�ľ����Ҳ����Ŀ¼��ʼ�Ĵغ�
    ͨ���ļ�����Ŀ¼�ؾ�����ļ�Ŀ¼��������Ŀ¼�е�ƫ�ƣ�,����ȷ��һ���ļ���λ��
 *����:
 *����:
    long DirStartClusterҲ����Ŀ¼��ʼ�Ĵغ�    
 */
long   MFS_GetCurDir(void);


//-----------------------------------------------------------------------------------

/*  MFS_SetCurDir()
 *����:
    �����û����������Ŀ¼����趨��ǰĿ¼  
 *����:
    long DirStartClusterҲ����Ŀ¼��ʼ�Ĵغ�
 *����:
    0,1                                     
 */
bool   MFS_SetCurDir(long DirStartCluster);



//-----------------------------------------------------------------------------------

/*  MFS_FOpen()  
 *����:
    �����û�������ļ����ڵ�ǰĿ¼�д�һ���Ѵ��ڵ��ļ���
    1.���ļ��ķ�ʽ�����֣�
    һ���ǵ�Dir��������Ӧ�ļ�Ŀ¼��ʱ������ѡ��ֱ�Ӵ��ļ�����ʱchar* strFlieName����0��
    һ�־��ǿ��Ի�ȡ���ļ���8��3������Ȼ���ڵ�ǰĿ¼������char* strFlieName���ļ�
    Ҳ����˵����Ӧ�ļ�ͨ��·��������
        * �趨��ǰĿ¼�������ļ�������
        * �趨��ǰĿ¼���趨��ǰĿ¼��������ļ���ֱ�ӵ���
    2.�ĵ��������ļ�����Ŀ¼��������˵��һ��ָ��д��8+3�ַ������ʽ����������ʽ,ռ��11��byte,���㲹20h
    3.FS_FOpen()֧�ֶ������߳������룬*strFlieNameָ��Ŀ����Ƕ�������������˵����
 
 *����:  
    char *strFlieNameΪ�����ļ����ļ���������ָ��
    char OpenModeΪ0��ʾ���ļ����ȡʱ֧������SEEK, Ϊ1��ʾ��
    �ļ����ȡʱ��֧������SEEK
 *����:
    handle fp �ļ����                   
 */
handle MFS_FOpen(const char *strFlieName, char Mode);

//-------------------------------------------------------------------------------------
/*  FS_FRead()  
 *����
    ��FS_FSeek()ָ���������ȡ������Ϊ��λ������,Ҳ��������˳���ȡ.  
 *���� 
    char *DataIObuffer �������������buffer��ָ��
    char DataSecNum������������Ŀ
    handle hFile�����ļ��ľ��
 *����  
    0,1                                     
 */
 bool   MFS_FRead(char *DataIObuffer, char DataSecNum, handle hFile);

//-------------------------------------------------------------------------------------

/*  MFS_FClose()
 *����
    �ر��û��������ļ�,�û�������Ҫ�����ļ��ľ��
 *����
    handle fp �ļ����
 *���� 
    0,1                                 
 */
bool   MFS_FClose(handle hFile);


//-----------------------------------------------------------------------------------

/*  MFS_GetFileLen()
 *����  
    ȡ��ǰ���ļ����ļ��ĳ���,������Ϊ��λ
 *����
    handle fp �ļ����  
 *����
    long    FileLength                  
 */
long   MFS_GetFileLen(handle hFile);


//-----------------------------------------------------------------------------------
 
/*  MFS_GetUnalignedLen()
 *����
    ��ȡ��ǰ���ļ����һ���������ݵ��ֽ���
 *����
    handle fp �ļ����
 *����  
    int UnalignedLength��ǰ���ļ����һ���������ݵ��ֽ��� 
 */
int    MFS_GetUnalignedLen(handle hFile);


//-----------------------------------------------------------------------------------

/*  MFS_GetInfo()
 *����  
    ��ȡ�ļ�ϵͳ������Ϣ
 *����  
 *����  
    char status��������Ϣ
        1   ���̶�д����     
        2   ����д����
        3   ����δ��ʽ��        
        4   �ļ����������ļ��߽�,Ŀ¼��������Ŀ¼�߽�   
        5   �ļ�������Ŀ���ļ�,Ŀ¼������Ŀ¼�����
        6   ��ʾ�ļ�����ʱû�д��̿ռ�,����д���ݻ�����չĿ¼    
            ��ʾĿ¼����ʱû�д��̿ռ�,������չĿ¼,�½���Ŀ¼
        7   �ļ�����ʱ��Ŀ¼Ŀ¼����
        8   ɾ��Ŀ¼ʱ����,��ʾɾ����Ŀ¼�ǿ�
 */
char   MFS_GetInfo(void);

//-----------------------------------------------------------------------------------


/*  MFS_JustBeing
 *����  
    
 *����  
  InputName ����ļ���Ŀ¼�����ַ�����ָ��
            ���û������ļ�����Ŀ¼�Ƿ����ʱ����ʼ���������Ƶ���ָ��ָ����ַ�����
            ��ʼ������ʱ������type��Ӧ�Ĳ������ͣ�����������߳���
  OutName    ���ļ���Ŀ¼���ڣ����Ҹ��û������OutName�ǿ�ʱ����������
            ������ļ���Ŀ¼�Ķ���
          
            
  JustType  Type�ĸ���λ��ʾһ�������������λ��ʾС�������ϸ��
            0x10    dir��ǰĿ¼���Ƿ����StrName���������������ļ�
            0x11    dir��ǰĿ¼���Ƿ����StrName���������������ļ�����Ŀ¼
            0x12    dir��ǰĿ¼���Ƿ����StrName���������������ļ�����Ŀ¼
            0x20    dir��ǰĿ¼���Ƿ����StrName���������������ļ�
            0x21    dir��ǰĿ¼���Ƿ����StrName����������������Ŀ¼
            0x22    dir��ǰĿ¼���Ƿ����StrName���������������ļ�����Ŀ¼
 *����  
  fail 0,succese,1      
  ��������Success��ʾ�������д���������������������ļ���Ŀ¼������fail��ʾ������                      
 */ 
bool  MFS_JustBeing (char *InputName, char *OutName, char JustType);


/*
*********************************************************************************************************
*Function    ��2008-9-24 18:02
*
*Description ��.
*
*Arguments   : 
*����:
    ȡ���ļ���,ͨ������Ҫ�󷵻ص�UnIcdeo�ַ���������ȡ��������ȡ������ȷ���û�ȡ������UnIcdeo�ַ���Ŀ
    ͨ�����ļ������Buffer��ʼ���ݣ�����������ȡ�ļ����ķ�ʽ
    һ���Ǹ��ݵ�ǰָ���Ŀ¼��ȡ��Ӧ���ļ�����
    һ���Ǹ����ļ��洢�õĶ����ڵ�ǰĿ¼���ȶ�λ��Ȼ����ȡ�ļ�����
 *����:
    char CharNumΪϣ����ȡ�ļ���������Unicoe�ַ������û�������Ӧ�趨�㹻buffer�ռ�
    1.) char CharNum��Ϊ0��ʾȡ��������
    2.) char CharNumΪ0���ʾ�ӵ�ǰĿ¼��ֱ�ӻ�ȡ�ļ�8+3���͵Ķ�����
    ���ò���Ҫ��NameBuf�ĳ�ʼ��
    �ĵ��������ļ�����Ŀ¼����ָ��д��8+3�ַ������ʽ,ռ��11��byte,���㲹20h

    char*NameBufΪ��ŵ���������ļ���������������bufferָ��
    1.) ���ֱ�Ӵӵ�ǰĿ¼����Ҹ�Ŀ¼�����������NameBuf�ĵ�һ���ֽڳ�ʼ��Ϊ20h����
    2.) �����Ҫ���ݶ�������Unicode����������ʱ��Ҫ��NameBuf��ʼ�������������������DIR���û��߱����û�ȡ
    
    NameAttr ---- address of attribute of filename, capital or small letter.
    
 *����:
    ȡ����ʱ����ʵ�ʺ�������ĳ����ַ��ĸ�����������������ʱ��ӵĽ�����0x0000��   
 *˵��:
    ȡ����ʱ���ص��ַ���С���ļ�ʵ���ַ���ʱ�������ں������Unicode��������
    �����û�������CharNumΪ1ʱ��buffer����Ӧ����4byte��2ʱ����6byte 
*
*Rerurns    ��0            failed
*             1            successful
*
*Other        :
*********************************************************************************************************
*/
bool MFS_GetName(char *NameBuf, char CharNum);


#endif /*__IAR_SYSTEMS_ICC */

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�˴����c���еĶ���
#ifdef __A251__

//��פ�ڴ�ĺ���
#define API_MFS_GetInfo                     0x00ff 
#define API_MFS_GetFileLen                  0x01ff
#define	API_MFS_GetUnalignedLen				0x02ff
#define API_MFS_FOpen                       0x03ff 
#define API_MFS_FRead                      	0x04ff
#define API_MFS_FClose                      0x05ff

#define BI_RC_GetInfo                       0x00   
#define BI_RC_GetFileLen                    0x01
#define	BI_RC_GetUnalignedLen				0x02 
#define BI_RC_FOpen                       	0x03 
#define BI_RC_FRead                       	0x04 
#define BI_RC_FClose                       	0x05 


//��0��bank 
#define API_MFS_Init                        0x0000    
#define API_MFS_Exit                        0x0100    
#define API_MFS_DiskInit                    0x0200  
#define API_MFS_CD                    		0x0300   
#define BI_B0_Init                          0x00    
#define BI_B0_Exit                          0x01    
#define BI_B0_DiskInit                      0x02 
#define BI_B0_CD                      		0x03  

//��1��bank                                                     
#define API_MFS_Dir                         0x0001                             
#define API_MFS_GetCurDir                   0x0101     
#define API_MFS_GetCurDirEntry             	0x0201  
#define API_MFS_SetCurDir                   0x0301     
#define API_MFS_SetCurDirEntry             	0x0401   
#define BI_B1_Dir                          	0x00    
#define BI_B1_GetCurDir                     0x01    
#define BI_B1_GetCurDirEntry                0x02 
#define BI_B1_SetCurDir                		0x03 
#define BI_B1_SetCurDirEntry                0x04 

//��2��bank
#define API_MFS_JustBeing                   0x0002                      
#define API_MFS_GetName                     0x0102
#define BI_B2_JustBeing                   	0x00
#define BI_B2_GetName                     	0x01                                                
                                                                                                                    
/*                      
* �˴�ʹ�ú���Ϊ����bank��ʹ��ʱ
*/                      
mMFS_Init    macro   
      
        mov		wr8,#API_MFS_Init
        jmp     RSTFSAPI
        endm
        
mMFS_DiskInit    macro   
      
        mov		wr8,#API_MFS_DiskInit
        jmp     RSTFSAPI
        endm
        
mMFS_Exit    macro
        mov		wr8,#API_MFS_Exit
        jmp     RSTFSAPI
        endm

mMFS_CD      macro
        mov		wr8,#API_MFS_CD
        jmp     RSTFSAPI   
        endm
        
                
mMFS_Dir     macro         
        mov		wr8,#API_MFS_Dir
        jmp     RSTFSAPI
        endm
 
mMFS_JustBeing     macro         
        mov		wr8,#API_MFS_JustBeing
        jmp     RSTFSAPI
        endm        
                
        
mMFS_SetCurDirEntry  macro
        mov		wr8,#API_MFS_SetCurDirEntry
        jmp     RSTFSAPI        
        endm      
                      
                                     
mMFS_GetCurDir   macro
        mov		wr8,API_MFS_GetCurDir
        jmp     RSTFSAPI   
        endm
 
mMFS_SetCurDir   macro
        mov		wr8,API_MFS_SetCurDir
        jmp     RSTFSAPI   
        endm


mMFS_FOpen       macro
        mov		wr8,API_MFS_FOpen    
        jmp     RSTFSAPI
        endm           
              
mMFS_FClose      macro
        mov		wr8,#API_MFS_FClose
        jmp		RSTFSAPI   
        endm           
        
mMFS_GetName         macro 
        mov		wr8,#API_MFS_GetName
        jmp     RSTFSAPI
        endm
                
mMFS_GetFileLen         macro 
        mov		wr8,#API_MFS_GetFileLen
        jmp     RSTFSAPI
        endm
                
mMFS_GetUnalignedLen         macro 
        mov		wr8,#API_MFS_GetUnalignedLen
        jmp     RSTFSAPI
        endm
                
mMFS_GetCurDirEntry         macro 
        mov		wr8,#API_MFS_GetCurDirEntry
        jmp     RSTFSAPI
        endm                

mMFS_GetInfo     macro     
        mov     wr8,#API_MFS_GetInfo
        jmp     RSTFSAPI
        endm           


#endif /*__IAR_SYSTEMS_ASM */
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif  /* _FILESYS_INCLUDE */

