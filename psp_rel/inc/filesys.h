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
#ifndef _FILESYS_INCLUDE
#define _FILESYS_INCLUDE

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�˴����c���Ժͻ�����Զ�Ҫʹ�õĶ���
#include "actapi.h"
#include "typeext.h"     
#include "uddrv.h" 

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// file system type 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define FS_FAT12        0x00
#define FS_FAT16        0x01
#define FS_FAT32        0x02

#define EXFAT_GET_DIRLAYER          0 //��ȡȫ·��
#define EXFAT_SET_DIRLAYER          1 //����ȫ·��
#define EXFAT_SET_CURDIR            2 //ֱ�����õ�ǰ�غ�
#define EXFAT_SET_CURDIR_FILENAME   3 //ͨ�������ṹ���õ�ǰ�غ�

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

/* prepare file cluster link when open     2008-12-19 13:56  */
#define FS_OPEN_TIDYCLUS        0x2
#define FS_OPEN_LN_TIDYCLUS     0x82
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
    

#define ATTR_READ_ONLY          0x01      /* Indicates that writes to the file should fail. */
#define ATTR_HIDDEN             0x02      /* Indicates that normal directory listings should not show this file. */
#define ATTR_SYSTEM             0x04      /* Indicates that this is an operating system file. */
#define ATTR_ARCHIVE            0x20      /* This attribute supports backup utilities. */
#define FILESIZE_BORDERLINE     0x10      /* borderline of file size, it is multiple of 64K  */

#define FS_SDFFAPIEntry         0x2b00    /* for data exchange, SDCard fast driver entry  2008-9-28 14:29   */

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�˴����c���еĶ���
#ifdef __C251__
//


typedef struct
{
    WORD year;
    uchar month;
    uchar day;       
    uchar hour;
    uchar minute;
    uchar second; 
}fs_crt_time_t;

//-----------------------------------------------------------------------------------

/*  FS_Init()
 *  ��ʼ����ǰ�������ļ�ϵͳ������֮��ſ��Կ�ʼ�ļ�ϵͳ���ܵ���    
 */
 bool   FS_Init(void);


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
 bool FS_DiskInit(uint8 letter);


//-----------------------------------------------------------------------------------

/*  FS_Exit()
 *  �������˳��ļ�ϵͳ���ܵ���  
 */
 bool   FS_Exit(void);

//-----------------------------------------------------------------------------------

/*  FS_CD()
 *����:  
    ���ݽ��û����뽫��ǰĿ¼ָ��ǰĿ¼����Ŀ¼����Ŀ¼��ֱ�ӷ��ظ�Ŀ¼ 
    strDirNameΪ�û�����8.3��Ŀ¼����(11��byte,��д,���㲹20h)��ʾ�ı䵱ǰĿ¼������Ŀ¼
 *����:  
    uint8 *strNameΪ8.3��Ŀ¼����bufferָ��
    strName��һ���ַ�Ϊ"\"��5chʱ��ʾ�ı䵱ǰĿ¼����һ����Ŀ¼ 
    strName��һ���ַ�Ϊ":"��3ahʱ��ʾ�ı䵱ǰĿ¼����Ŀ¼
    strName��һ���ַ�Ϊ"/"��2fhʱ��ʾ�ı䵱ǰĿ¼����ǰĿ¼���Ӧ����Ŀ¼
 *����:
    0,1                          
 */
 bool   FS_CD(const uint8 *strName);

//-----------------------------------------------------------------------------------

/*  FS_Dir()
 *����:
    �ڵ�ǰĿ¼�¼����ļ�����Ŀ¼��ͨ���������ѡ�������Ŀ¼�����ļ�
    ���øú������ú����ӵ�ǰĿ¼��ָ�뿪ʼȡһ���ļ�����Ŀ¼������
    a.����FS_CD()����FS_SetCurDir()��һ��Ŀ¼Ȼ����Dir��Dir��������ʼĿ¼��ָ����Ϊ00000000h
    b.����FS_SetCurDirEntry()�趨һ��Ŀ¼�Dir��������ʼĿ¼��ָ����ΪFS_SetCurDirEntry()������ֵ
 *����:    
    DirType�����������DIR�Ķ���,0��DIR�ļ�,1��ʾDIRĿ¼,��ǰ�汾��FAT16��DirType��0x80��0x81��Ŀ¼�׿�ʼDir 
    uint8 *StrName����������������ļ�����bufferָ��
    uint8 *StrExtName����������չ����Ϣ,����DIR����������'*'��ʾdir���������ļ�
    uint8 ExtNameNum ��ʾStrExtName����չ���ĸ���
 *����: 
    0,1                         
 */
  bool   FS_Dir(uint8 *StrName, const uint8 *StrExtName, uint8 type,uint8 ExtNameNum);



//-----------------------------------------------------------------------------------

/*  FS_GetName()
 *����:
    ȡ���ļ���,ͨ������Ҫ�󷵻ص�Unicode�ַ���������ȡ��������ȡ������ȷ���û�ȡ������Unicode�ַ���Ŀ
    ͨ�����ļ������Buffer��ʼ���ݣ�����������ȡ�ļ����ķ�ʽ
    һ���Ǹ��ݵ�ǰָ���Ŀ¼��ȡ��Ӧ���ļ�����
    һ���Ǹ����ļ��洢�õĶ����ڵ�ǰĿ¼���ȶ�λ��Ȼ����ȡ�ļ�����
 *����:
    uint8 CharNumΪϣ����ȡ�ļ���������Unicode�ַ������û�������Ӧ�趨�㹻buffer�ռ�
    1.) uint8 CharNum��Ϊ0��ʾȡ��������
    2.) uint8 CharNumΪ0���ʾ�ӵ�ǰĿ¼��ֱ�ӻ�ȡ�ļ�8+3���͵Ķ�����
    ���ò���Ҫ��NameBuf�ĳ�ʼ��
    �ĵ��������ļ�����Ŀ¼����ָ��д��8+3�ַ������ʽ,ռ��11��byte,���㲹20h

    uint8*NameBufΪ��ŵ���������ļ���������������bufferָ��
    1.) ���ֱ�Ӵӵ�ǰĿ¼����Ҹ�Ŀ¼�����������NameBuf�ĵ�һ���ֽڳ�ʼ��Ϊ20h����
    2.) �����Ҫ���ݶ�������Unicode����������ʱ��Ҫ��NameBuf��ʼ�������������������DIR���û��߱����û�ȡ
 *����:
    ȡ����ʱ����ʵ�ʺ�������ĳ����ַ��ĸ�����������������ʱ��ӵĽ�����0x0000��   
 *˵��:
    ȡ����ʱ���ص��ַ���С���ļ�ʵ���ַ���ʱ�������ں������Unicode��������
    �����û�������CharNumΪ1ʱ��buffer����Ӧ����4byte��2ʱ����6byte    
 */
 uint8   FS_GetName(uint8 *NameBuf, uint8 CharNum);


//-----------------------------------------------------------------------------------

/*  FS_GetCurDirEntry()
 *����:
    ȡ��ǰ���������ļ���Ӧ��Ŀ¼���ڵ�ǰĿ¼�е�ƫ���ֽ�
    ����Ŀ¼������ڵ�ǰĿ¼�׵�ƫ�ƣ�����ȷ����Ŀ¼���λ�ã�ͬʱָ��һ���ļ�������Ŀ¼
 *����:
    long DirEntry   
 */     
 long   FS_GetCurDirEntry(void);


//-----------------------------------------------------------------------------------

/*  FS_SetCurDirEntry()
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
 bool   FS_SetCurDirEntry(long DirEntry);


//-----------------------------------------------------------------------------------

/*  FS_GetCurDir()
 *����:
    ȡ��ǰĿ¼�ľ����Ҳ����Ŀ¼��ʼ�Ĵغ�
    ͨ���ļ�����Ŀ¼�ؾ�����ļ�Ŀ¼��������Ŀ¼�е�ƫ�ƣ�,����ȷ��һ���ļ���λ��
 *����:
 *����:
    long DirStartClusterҲ����Ŀ¼��ʼ�Ĵغ�    
 */
 long   FS_GetCurDir(void);


 //-----------------------------------------------------------------------------------

/*  FS_SetCurDir()
 *���v?
    �����û����������Ŀ¼����趨��ǰĿ¼  
 *����:
    long DirStartClusterҲ����Ŀ¼��ʼ�Ĵغ�
 *����:
    0,1                                     
 */
 bool   FS_SetCurDir(long DirStartCluster);
 

//-----------------------------------------------------------------------------------

/*  FS_GetOrSetDir_Exfat()
 *����:
    Exfat�ļ�ϵͳ֧������ӿڣ���ͬFat32/16��Ϊһ��  
 *����:
    uint8 *dirlayer 1. Exfat�ļ�ϵͳ��8��ȫ·���ṹ��ָ�룬set/getȫ·��ʱ����
                    2. Exfat�ļ�����������ṹ��ָ�룬ͨ�������ṹset��ǰ�غ�
    long curdir ��ǰ�غţ�ֱ������set��ǰ�غ�ʱ��Ч
    uint8 mode  ����ģʽ������ֵ����һ��
               #define EXFAT_GET_DIRLAYER          0 //��ȡȫ·��
               #define EXFAT_SET_DIRLAYER          1 //����ȫ·��
               #define EXFAT_SET_CURDIR            2 //ֱ�����õ�ǰ�غ�
               #define EXFAT_SET_CURDIR_FILENAME   3 //ͨ�������ṹ���õ�ǰ�غ�
 *����:
    0,1                                     
 */ 
 bool   FS_GetOrSetDir_Exfat(uint8 *dirlayer, long curdir, uint8 mode);

//-----------------------------------------------------------------------------------

/*  FS_MakeDir()
 *����:
    �ڵ�ǰĿ¼������һ���û������ƶ���Ŀ¼��
    �ĵ��������ļ�����Ŀ¼����ָ��д��8+3�ַ������ʽ,ռ��11��byte,���㲹20h
 *����:
    const uint8 *strSubDirName,���ɵ�Ŀ¼��8.3����
 *����:
    0,1                                     
 */
 bool   FS_MakeDir(const uint8 *strSubDirName);


//-----------------------------------------------------------------------------------

/*  FS_RemoveDir()  
 *����:
    �ڵ�ǰĿ¼��ɾ��һ���û�����ָ����Ŀ¼��
 *����:
    const uint8 *strSubDirName,��ɾ��Ŀ¼��8.3����
 *����:
    0,1                                     
 */
 bool   FS_RemoveDir(uint8 *strSubDirName);


//-----------------------------------------------------------------------------------

/*  FS_FOpen()  
 *����:
    �����û�������ļ����ڵ�ǰĿ¼�д�һ���Ѵ��ڵ��ļ���
    1.���ļ��ķ�ʽ�����֣�
    һ���ǵ�Dir��������Ӧ�ļ�Ŀ¼��ʱ������ѡ��ֱ�Ӵ��ļ�����ʱuint8* strFlieName����0��
    һ�־��ǿ��Ի�ȡ���ļ���8��3������Ȼ���ڵ�ǰĿ¼������uint8* strFlieName���ļ�
    Ҳ����˵����Ӧ�ļ�ͨ��·��������
        * �趨��ǰĿ¼�������ļ�������
        * �趨��ǰĿ¼���趨��ǰĿ¼��������ļ���ֱ�ӵ���
    2.�ĵ��������ļ�����Ŀ¼��������˵��һ��ָ��д��8+3�ַ������ʽ����������ʽ,ռ��11��byte,���㲹20h
    3.FS_FOpen()֧�ֶ������߳������룬*strFlieNameָ��Ŀ����Ƕ�������������˵����
 
 *����:  
    uint8 *strFlieNameΪ�����ļ����ļ���������ָ��
    uint8 OpenModeΪ0��ʾ���ļ����ȡʱ֧������SEEK, Ϊ1��ʾ��
    �ļ����ȡʱ��֧������SEEK
 *����:
    handle fp �ļ����                   
 */
 handle FS_FOpen(const uint8 *strFlieName, uint8 Mode);

//-----------------------------------------------------------------------------------

/*  FS_FOpenbyLN()  
 *����:
    �����û������UNICODE�ļ����ڵ�ǰĿ¼�д�һ���Ѵ��ڵ��ļ���
    
 *����:  
    UNstr ���ļ����ļ���������ָ��
    Mode  0    ���ļ����ȡ����ʱ֧������SEEK 
          1    ��ʾ���ļ����ȡʱ��֧������SEEK
    SNstr ����ָ�����ڷ��ظ��ļ��洢��Ŀ¼�еĶ���

 *����:
    handle fp �ļ����                   
 */

 handle FS_FOpenbyLN(const uint8 *UNstr, uint8 Mode, uint8 *SNstr);
//-----------------------------------------------------------------------------------

/*  FS_FCreate()
 *����:  
    �����û�������ļ�������һ���ļ�Ŀ¼��,�û��ɻ�õ�ǰ�����ļ��ľ��
    �ĵ��������ļ�����Ŀ¼����ָ��д��8+3�ַ������ʽ,ռ��11��byte,���㲹20h
 *����:  
    uint8 *strFlieNameΪ�����ļ����ļ���������ָ��
 *����:    
    handle fp �ļ����                   
 */
 handle FS_FCreate(const uint8*strFileName);


//-----------------------------------------------------------------------------------

/*  FS_FClose()
 *����
    �ر��û��������ļ�,�û�������Ҫ�����ļ��ľ��
 *����
    handle fp �ļ����
 *���� 
    0,1                                 
 */
 bool   FS_FClose(handle hFile);


//-----------------------------------------------------------------------------------

/*  FS_FRemove()
 *����  
    �ڵ�ǰĿ¼��ɾ��һ���û�����ָ����Ŀ¼��
    �ĵ��������ļ�����Ŀ¼����ָ��д��8+3�ַ������ʽ,ռ��11��byte,���㲹20h
 *����  
    uint8 *strNameɾ��Ŀ¼��Ŀ¼��ָ������
 *����    
    0,1                                 
 */
 bool   FS_FRemove(uint8 *strName);


//-----------------------------------------------------------------------------------

/*  FS_GetFileLen()
 *����  
    ȡ��ǰ���ļ����ļ��ĳ���,������Ϊ��λ
 *����
    handle fp �ļ����  
 *����
    long    FileLength                  
 */
 long   FS_GetFileLen(handle hFile);


//-----------------------------------------------------------------------------------
 
/*  FS_GetUnalignedLen()
 *����
 	��ȡ��ǰ���ļ����һ���Ƕ����������ֽ������緵��0������ļ���������������
 *����
    handle fp �ļ����
 *����  						 
 	int UnalignedLength��ǰ���ļ����һ���Ƕ����������ֽ������緵��0������ļ���������������
 */
 int    FS_GetUnalignedLen(handle hFile);
                                                                                         
//-----------------------------------------------------------------------------------                                                                                          
                                                                                         
 
/*  FS_GetCrtTime()
 *����
    ��ȡ��ǰ���ļ��Ĵ���ʱ��
    ʱ�������ʽ��
            struct timebuf
                {
                    WORD year;
                    uchar month;
                    uchar day;       
                    uchar hour;
                    uchar minute;
                    uchar second; 
                }Crttimebuffer;    
 *����
    handle fp �ļ������ʱ�����bufָ��
 *����  
    0��1
 */
 bool     FS_GetCrtTime(fs_crt_time_t *Crttimebuffer, handle hFile);

 
//-----------------------------------------------------------------------------------

 bool     FS_GetWrtTime(fs_crt_time_t *Crttimebuffer, handle hFile);
/*  FS_FSeek()  
 *����:
    ����������FAT�е���֯��ʽ�������Ҷ�λ�Ѵ����ļ��е�һ��������ʵ�֣�
    a.��������ļ��׵�ƫ������ָ��һ��������ʵ���û������˳����������
    b.ʵ�ִӵ�ǰλ����ǰ������ƫ������������ʵ���û������˳����������
    c.��������ļ��׵�ƫ������ָ���Ѵ����ļ���һ��������ʵ���û�����ĶԸ��������ݵ��޸�
    ���Fread()֧���û�����˳������ݣ���������Լ������λ�����ݣ�������Է���ʵ�ֻ�д
    �޸��Ѿ����ɵ��ļ�
 *����:  
    long fileoffset��ӦSEEKƫ���� 
    uint8 seektype��ӦSEEK���� 0:���ļ�ͷ��ʼ���ļ�β�����1:�ӵ�ǰλ�����ļ�β���
    						  2:�ӵ�ǰλ�����ļ�ͷ����
    handle hFile��Ӧ�ļ��������
 *����:  
    0,1                                 
 */
 bool   FS_FSeek(long fileoffset, uint8 seektype, handle hFile);


//-----------------------------------------------------------------------------------

/*  FS_FTell()  
 *����:
    ȡ��ǰ��д������ָ�룬ָ����ָ����ļ�ͷ������ƫ������
    ������ʱ�û����øú�����¼AB�㣬���FS_Seek��FS_FReadʵ������
    ��AB��ȡ.д����ʱ��֧���û������޸��������ļ���һ������
 *����:
    handle hFile��Ӧ�ļ��������  
 *����:  
    long fileoffset��Ӧ�������е�bcde�Ĵ���    
 */
 long   FS_FTell(handle hFile);


//-----------------------------------------------------------------------------------

/*  FS_FRead()  
 *����
    ��FS_FSeek()ָ���������ȡ������Ϊ��λ������,Ҳ��������˳���ȡ.  
 *���� 
    uint8 *DataIObuffer �������������buffer��ָ��
    uint8 DataSecNum������������Ŀ
    handle hFile�����ļ��ľ��
 *����  
    0,1                                     
 */
 bool   FS_FRead(uint8 *DataIObuffer, uint8 DataSecNum, handle hFile);


//-----------------------------------------------------------------------------------

/*  FS_Write()  
 *����
    �û�������øú���ÿ��������Ϊ��λ������˳��д���Ѵ����ļ��Ĵ�����ʾ������ռ���  
 *���� 
    uint8 *DataIObuffer д�����������buffer��ָ��
    uint8 DataSecNum������������Ŀ
    handle hFile�����ļ��ľ��
 *����  
    0,1                                     
 */
 bool   FS_FWrite(const uint8 *DataIObuffer, uint8 DataSecNum, handle hFile);

//-----------------------------------------------------------------------------------
 
/*  FS_GetSpace()
 *����  
    ��ȡu��ʣ��ռ䣬�������������ͬѡ��Ҫ�󷵻ش����ܵ�����������ʣ
    ��������
 *����
    uint8 typechar 0��ʾ���ý����ر�ʾ��ǰ�����������ܿռ����������
                   1��ʾ���ص�ǰ����������ʣ��ռ��������
 *����  
    long diskspace                          
 */ 
 long   FS_GetSpace(uint8 type);


//-----------------------------------------------------------------------------------

/*  FS_GetInfo()
 *����  
    ��ȡ�ļ�ϵͳ������Ϣ
 *����  
 *����  
    uint8 status��������Ϣ
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
 uint8   FS_GetInfo(void);

//-----------------------------------------------------------------------------------

/*  FS_Backup()  
 *����  
    ���ݵ�ǰ�ļ�������״̬
 *����  
    uint8* pbufΪ����buffer��ָ�룬buffer��С�ӱ������ݶ���
    uint8 typeΪ0��ʾ���ݵ�ǰ����д״̬��Ϣ��1��ʾ�����ļ�ϵͳ�ڲ�����buffer 
    handle hFile 
 *����  
    0,1                             
 */ 
 bool   FS_Backup(const uint8 *pbuf, uint8 type, handle hFile);

//-----------------------------------------------------------------------------------

/*  FS_Restore
 *����  
    �ָ������ݵ��ļ�����״̬
 *����  
    uint8 * pbufΪ����buffer��ָ�룬buffer��С�ӱ������ݶ���
    uint8 typeΪ0��ʾ�ָ���ǰ����д״̬��Ϣ��1��ʾ�ָ��ļ�ϵͳ�ڲ�����buffer  
    handle hFile  
 *����  
    0,1                             
 */
 bool   FS_Restore(uint8 *pbuf, uint8 type,handle hFile);

//-----------------------------------------------------------------------------------
        
/*  FS_MakeSN
 *����  
    ���������UNICODE�Լ���ǰ���̵��ļ���Ϣ��ת�����ļ�ϵͳ�����ļ�ʱѡ�õĶ���
 *����  
    uint8 *SFNstrΪָ����������ַ�����ָ��
    uint8 *UNstrΪָ�����볤����unicode���ַ�����ָ��  
    uint8 encodeΪת���ɶ���ʱ���õı������Եĸ�ʽ��GB,..��  
 *����  
    0,1                             
 */
 bool   FS_MakeSN(uint8 *UNstr,uint8 *SFNstr,uint8 encode);
//-----------------------------------------------------------------------------------


        
/*  FS_CDbyLN
 *����  
    ���������UNICODE��Ŀ¼����������Ŀ¼
 *����  
    uint8 *UNstrΪָ�����볤Ŀ¼����unicode���ַ�����ָ�� 
    uint8 *SFNstrΪָ����������ַ�����ָ�� 
 *����  
    0,1                             
 */
 bool   FS_CDbyLN(uint8 *UNstr,uint8 *SNstr);
//-----------------------------------------------------------------------------------


/*  FS_MakeDirbyLN
 *����  
    ���������UNICODE��Ŀ¼����������Ŀ¼
 *����  
    uint8 *UNstrΪָ�����볤Ŀ¼����unicode���ַ�����ָ��  
    uint8 *SFNstrΪָ����������ַ�����ָ��    
    uint8 encodeΪת���ɶ���ʱ���õı������Եĸ�ʽ��GB,..��  
 *����  
    0,1                             
 */
 bool   FS_MakeDirbyLN(uint8 *UNstr,uint8 *SNstr,uint8 encode);
//-----------------------------------------------------------------------------------


/*  FS_FCreatebyLN
 *����  
    ���������UNICODE���ļ����������ļ�
 *����  
    uint8 *UNstrΪָ�����볤Ŀ¼����unicode���ַ�����ָ��  
    uint8 *SFNstrΪָ����������ַ�����ָ��
    uint8 encodeΪת���ɶ���ʱ���õı������Եĸ�ʽ��GB,..��  
 *����  
    0,1                             
 */ 
 
 handle FS_FCreatebyLN(uint8 *UNstr,uint8 *SNstr,uint8 encode);
//-----------------------------------------------------------------------------------


/*  FS_JustBeing
 *����  
    
 *����  
  InputName ����ļ���Ŀ¼�����ַ�����ָ��
            ���û������ļ�����Ŀ¼�Ƿ����ʱ����ʼ���������Ƶ���ָ��ָ����ַ�����
            ��ʼ������ʱ������type��Ӧ�Ĳ������ͣ�����������߳���
  OutName    ���ļ���Ŀ¼���ڣ����Ҹ��û������OutName�ǿ�ʱ����������
            ������ļ���Ŀ¼�Ķ���
          
            
  JustType  Type�ĸ���λ��ʾһ�������������λ��ʾС�������ϸ��
            0x10    dir��ǰĿ¼���Ƿ����StrName���������������ļ�
            0x11    dir��ǰĿ¼���Ƿ����StrName����������������Ŀ¼
            0x12    dir��ǰĿ¼���Ƿ����StrName���������������ļ�����Ŀ¼
            0x20    dir��ǰĿ¼���Ƿ����StrName���������������ļ�
            0x21    dir��ǰĿ¼���Ƿ����StrName����������������Ŀ¼
            0x22    dir��ǰĿ¼���Ƿ����StrName���������������ļ�����Ŀ¼
 *����  
  fail 0,succese,1      
  ��������Success��ʾ�������д���������������������ļ���Ŀ¼������fail��ʾ������                      
 */ 
 bool  FS_JustBeing (uint8 *InputName, uint8 *OutName, uint8 JustType);
//-----------------------------------------------------------------------------------

/*
*********************************************************************************************************
*Function	��ɾ���ļ���ĳһ������Ϣ	2007-1-22 18:46
*
*Description���ú�������ɾ��һ���ļ���ĳһ���֣��м���߽�β����������ȫ������Ϣ
*
*Arguments	: StartSec	����ɾ���Ŀ�ʼ����λ�ã�������ļ��׵�����ƫ�ƣ�
*			  EndSec	����ɾ���Ľ�������λ�ã�������ļ��׵�����ƫ�ƣ�
*			  hFile		��ǰ�ļ�������ľ��
*
*Rerurns	��0			failed
*			  1			successful
*
*Other		:
*********************************************************************************************************
*/
 bool FS_FRemovePart (long StartSec, long EndSec, handle hFile);

/*
*********************************************************************************************************
*Function	������ļ���ĳ���ֵ���һ���ļ�	2007-1-25 20:18
*
*Description���ú�����������ļ���ĳ���֣��м���߽�β����������ȫ��������һ���ļ�
*
*Arguments	: InsertSec	�ļ�1�Ĳ���㣨������ļ��׵�����ƫ�ƣ�
*			  StartSec	�ļ�2�Ĳ��벿�ֵ���ʼ����λ�ã�������ļ��׵�����ƫ�ƣ�
*			  EndSec	�ļ�2�Ĳ��벿�ֵĽ�������λ�ã�������ļ��׵�����ƫ�ƣ�
*			  hFile1	����������޸ĵ��ļ��ľ��
*			  hFile2	Ҫ���벢����ɾ����������Ҫ���ܲ�����ɾ������������ɾ�������ļ��ľ��
*
*Rerurns	��0			failed
*			  1			successful
*
*Other		:
*********************************************************************************************************
*/
 bool FS_FAddFile (long InsertSec, long StartSec, long EndSec, handle hFile1, handle hFile2);

/*
*********************************************************************************************************
*Function	������FAT1������FAT2	2007-3-26 17:10
*
*Description���ú������ڿ���FAT1������FAT2
*
*Arguments	: 
*
*Rerurns	��0			failed
*			  1			successful
*
*Other		:
*********************************************************************************************************
*/
 bool FS_CopyFATSec (void);

/*
*********************************************************************************************************
*Function	��alter file attribute        2007-7-5 11:12
*
*Description��used just after FS_FOPen() or FS_FCreate()
*
*Arguments	: mode ---- ATTR_READ_ONLY (0x01), indicates that writes to the file should fail.
*                       ATTR_HIDDEN (0x02), indicates that normal directory listings should not show this file.
*                       ATTR_SYSTEM (0x04), indicates that this is an operating system file. 
*                       ATTR_ARCHIVE (0x20), supports backup utilities.
*                       
*
*Rerurns	��0			failed
*			  1			successful
*
*Other		:
*********************************************************************************************************
*/
 bool FS_FAttrAlter (uint8 mode);

/*
*********************************************************************************************************
*Function	��rename a file        2007-9-17 16:07
*
*Description��Change the name of a file according to the new name from application.
*
*Arguments	: NewName ---- address of new short name (8 filename and 3 ext).
*                       
*
*Rerurns	��0			failed
*			  1			successful
*
*Other		: used just after FS_FOPen() or FS_FCreate()
*********************************************************************************************************
*/
 bool FS_FRename (uint8 *NewName);

//-----------------------------------------------------------------------------------

/*  FS_ModUnalignedLen()
 *����  
    �����ļ�ʱ���޸ı�ʾ�ļ���ȷ���ȵ��ļ��ֽ�����
 *����  
    UnalignedLen    �ļ���β�������ֽ�����1~512��
    hFile   ��ǰд�ļ�������ľ��

 *����  
  fail 0,succese,1                          
 */ 
 bool  FS_ModUnalignedLen (int UnalignedLen , handle hFile );
//-----------------------------------------------------------------------------------
 
 
 bool  FS_FFRead (word fileoffset, uint8* databuf, handle hFile );
 //-----------------------------------------------------------------------------------
 
 /*  FS_FRecSave()
  *����
	 �ָ�����¼��ʱ����û���ü����������
  *����
	 handle fp �ļ�������þ��������Ϊ��¼��ʱ������ļ��������Ҫ��ʱ����
  *���� 
	 0,1								 
  */

 bool  FS_FRecSave(handle hFile );
//-----------------------------------------------------------------------------------

/*  FS_GetFileStartClusNum()
 *����  
    ȡ���ļ����״غţ���Ҫ��FS_FOPen��FS_FOpenbyLN��������ã�������ļ�����������������ص��ļ����
 *����  
    hFile  �ļ���� 
    type   ��ʱδʹ��

 *����  
  fail 0,succese,1                          
 */ 
 long  FS_GetFileStartClusNum(handle hFile, uint8 type);

/*
*********************************************************************************************************
*Function	���޸��ļ�������
*
*Description�����ļ�������չ������ΪСд��ʾ��
*
*Arguments	: NameAttr-0x00 �ļ�������չ��Ϊ��д
*			          -0x08 �ļ���ΪСд,��չ��Ϊ��д 
*					  -0x10 �ļ���Ϊ��д����չ��ΪСд
*				      -0x18 �ļ�������չ������Сд
*                       
*
*Rerurns	��0			failed
*			  1			successful
*
*Other		: ֻ���� FS_FOPen() ���� FS_FCreate()��ʹ�á�
*********************************************************************************************************
*/
 bool FS_FSetNameAttr (uint8 NameAttr);

//-----------------------------------------------------------------------------------
/*  FS_DirLast()
 *����  
    ��λ����ǰĿ¼�����
 *����  
	void

 *����  
  fail 0,succese,1                          
 */ 
 bool  FS_DirLast(void);

//-----------------------------------------------------------------------------------
/*  FS_DirParNext()
 *����  
    ��Ŀ¼ָ��ָ����һ��Ŀ¼��ָ��ǰĿ¼��Ŀ¼�����һ��Ŀ¼�����ǰָ��ָ���
    �Ǹ�Ŀ¼����ô������������ش���
 *����  
	void
 *����  
  fail 0,succese,1                          
 */ 
 bool  FS_DirParNext(void);
 
//-----------------------------------------------------------------------------------
/*  FS_DirPrev()
 *����:
    �ڵ�ǰĿ¼�¼����ļ�����Ŀ¼��ͨ���������ѡ�������Ŀ¼�����ļ�
    ���øú������ú����ӵ�ǰĿ¼��ָ�뿪ʼȡһ���ļ�����Ŀ¼������
    a.����FS_DirLast���ٵ���FS_DirPrev����ȡ���һ����Ч��Ŀ¼�
    b.����FS_SetCurDirEntry()�趨һ��Ŀ¼�DirPrev�����ظ�Ŀ¼���ǰһ����ЧĿ¼�
    c.����ͨ�����DirPrev���ص��ǵ�ǰĿ¼���ǰһ����ЧĿ¼�
 *����:    
    Type�����������DIR�Ķ���,0��DIR�ļ�,1��ʾDIRĿ¼��
    uint8 *StrName����������������ļ�����bufferָ��
    uint8 *StrExtName����������չ����Ϣ,����DIR����������'*'��ʾdir���������ļ�
    uint8	ExtNameNum ��ʾStrExtName����չ���ĸ���
 *����: 
    0,1                           
 */ 

 bool  FS_DirPrev(uint8 *StrName, const uint8 *StrExtName, uint8 Type,uint8	ExtNameNum); 

//-----------------------------------------------------------------------------------
/*  FS_CreateVolume()
 *����  
    ����DriveActive֮����Ϊ��Ҫ�������̵ĸ�Ŀ¼���������ֻ�������չ̼���ĵ�һ�������ŵ�����Ӧ����
 *����  
	void
 *����  
  fail 0,succese,1                          
 */ 
 bool FS_CreateVolume(void); 
 
 
#endif /*__C251__ */

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//�˴����c���еĶ���
#ifdef __A251__

#define API_FS_GetInfo                  0x00
#define API_FS_FTell                    0x01
#define API_FS_FRead                    0x02
#define API_FS_FSeek                    0x03

#define API_FS_Init                     0x0000+API_FS_BaseNoB 
#define API_FS_Exit                     0x1000+API_FS_BaseNoB 
#define	API_FS_DiskInit					0x2000+API_FS_BaseNoB

#define API_FS_CD                       0x0001+API_FS_BaseNoB
#define API_FS_Dir                      0x0002+API_FS_BaseNoB
#define API_FS_GetName                  0x0003+API_FS_BaseNoB

#define API_FS_GetCurDirEntry           0x0004+API_FS_BaseNoB
#define API_FS_SetCurDirEntry           0x1004+API_FS_BaseNoB
#define API_FS_GetCurDir                0x2004+API_FS_BaseNoB
#define API_FS_SetCurDir                0x3004+API_FS_BaseNoB
#define API_FS_GetOrSetDir_Exfat        0x4004+API_FS_BaseNoB
                                                                                
#define API_FS_MakeDir                  0x0005+API_FS_BaseNoB
#define API_FS_MakeDirbyLN              0x1005+API_FS_BaseNoB
#define API_FS_RemoveDir                0x0006+API_FS_BaseNoB   
  
#define API_FS_FOpen                    0x0007+API_FS_BaseNoB    
#define API_FS_GetFileStartClusNum      0x1007+API_FS_BaseNoB   
#define API_FS_FOpenbyLN                0x0008+API_FS_BaseNoB 
#define API_FS_FOpenbyLFN               0x0008+API_FS_BaseNoB
#define API_FS_FCreate                  0x0009+API_FS_BaseNoB
#define API_FS_FCreatebyLN              0x1009+API_FS_BaseNoB
#define API_FS_FClose                   0x000a+API_FS_BaseNoB   
#define API_FS_FRemove                  0x000b+API_FS_BaseNoB
                                        
                                        
#define API_FS_FWrite                   0x000c+API_FS_BaseNoB

#define API_FS_GetSpace                 0x000d+API_FS_BaseNoB
#define API_FS_GetFileLen               0x100d+API_FS_BaseNoB
#define API_FS_GetUnalignedLen          0x200d+API_FS_BaseNoB
#define API_FS_GetCrtTime               0x300d+API_FS_BaseNoB
#define API_FS_GetWrtTime               0x400d+API_FS_BaseNoB
#define API_FS_ModUnalignedLen          0x500d+API_FS_BaseNoB

#define API_FS_Backup                   0x000e+API_FS_BaseNoB
#define API_FS_Restore                  0x100e+API_FS_BaseNoB
#define API_FS_FFSeek                   0x200e+API_FS_BaseNoB

#define API_FS_CDbyLN                   0x000f+API_FS_BaseNoB
#define API_FS_MakeSN                   0x0010+API_FS_BaseNoB  
  
#define API_FS_JustBeing                0x0012+API_FS_BaseNoB  
#define API_FS_FRemovePart				0x0013+API_FS_BaseNoB
#define API_FS_FAddFile                 0x0014+API_FS_BaseNoB
#define API_FS_CopyFATSec               0x0015+API_FS_BaseNoB
//#define API_FS_CreateSegTbl             0x1015+API_FS_BaseNoB
#define API_FS_FAttrAlter               0x0016+API_FS_BaseNoB
#define API_FS_FSetNameAttr             0x1016+API_FS_BaseNoB
#define API_FS_FRename                  0x2016+API_FS_BaseNoB	 
#define API_FS_CreateVolume             0x3016+API_FS_BaseNoB
#define API_FS_DirPrev					0x0017+API_FS_BaseNoB
#define	API_FS_DirLast					0x0018+API_FS_BaseNoB
#define	API_FS_DirParNext				0x0019+API_FS_BaseNoB
#define API_FS_FRecSave                 0x001A+API_FS_BaseNoB 

#define API_FS_FFRead                   0x000f+API_FS_BaseNoA        


/*
* �˴�ʹ�ú���Ϊ����bank��ʹ��ʱ
*/
mFS_Init    macro   
      
        mov     wr8,#API_FS_Init
        lcall	RSTBankAPI
        endm        
        
mFS_Exit    macro
        mov     wr8,#API_FS_Exit
        lcall   RSTBankAPI
        endm
        
mFS_DiskInit	macro
        mov     wr8,#API_FS_DiskInit
        lcall   RSTBankAPI
        endm			        

mFS_CD      macro
        mov     wr8,#API_FS_CD
        lcall   RSTBankAPI   
        endm
        
mFS_CDbyLN     macro
        mov     wr8,#API_FS_CDbyLN
        lcall   RSTBankAPI   
        endm        
                
mFS_Dir     macro         
        mov     wr8,#API_FS_Dir
        lcall   RSTBankAPI
        endm
 
mFS_JustBeing     macro         
        mov     wr8,#API_FS_JustBeing
        lcall   RSTBankAPI
        endm        

mFS_FRemovePart		macro
		mov     wr8,#API_FS_FRemovePart
		lcall	RSTBankAPI
		endm

mFS_FAddFile        macro
		mov     wr8,#API_FS_FAddFile
		lcall	RSTBankAPI
		endm

mFS_CopyFATSec        macro
		mov     wr8,#API_FS_CopyFATSec
		lcall	RSTBankAPI
		endm
		
//mFS_CreateSegTbl      macro
//        mov     wr8,#API_FS_CreateSegTbl
//		lcall	RSTBankAPI
//		endm

mFS_FAttrAlter        macro
        mov     wr8,#API_FS_FAttrAlter
        lcall   RSTBankAPI
        endm
        
mFS_FRename       macro
        mov     wr8,#API_FS_FRename
        lcall   RSTBankAPI
        endm
        
mFS_FSetNameAttr       macro
        mov     wr8,#API_FS_FSetNameAttr
        lcall   RSTBankAPI
        endm
        
mFS_GetName macro
        mov     wr8,#API_FS_GetName
        lcall   RSTBankAPI        
        endm

mFS_GetCurDirEntry  macro
        mov     wr8,#API_FS_GetCurDirEntry
        lcall   RSTBankAPI        
        endm
        
mFS_SetCurDirEntry  macro
        mov     wr8,#API_FS_SetCurDirEntry
        lcall   RSTBankAPI        
        endm
                                     
mFS_GetCurDir   macro
        mov     wr8,#API_FS_GetCurDir
        lcall   RSTBankAPI   
        endm
 
mFS_SetCurDir   macro
        mov     wr8,#API_FS_SetCurDir
        lcall   RSTBankAPI   
        endm
		            
mFS_MakeDir     macro
        mov     wr8,#API_FS_MakeDir
        lcall   RSTBankAPI   
        endm

mFS_MakeDirbyLN     macro
        mov     wr8,#API_FS_MakeDirbyLN
        lcall   RSTBankAPI   
        endm
        
mFS_RemoveDir   macro
        mov     wr8,#API_FS_RemoveDir
        lcall   RSTBankAPI   
        endm   


mFS_FOpen       macro
        local   ByShortName  
        mov     wr8,#API_FS_FOpen  
        bit     7,c
        jr      z,ByShortName
        mov     wr8,#API_FS_FOpenbyLN
ByShortName:       
        lcall   RSTBankAPI
        endm

mFS_FOpenbyLN       macro
        mov     wr8,#API_FS_FOpenbyLN  
        lcall   RSTBankAPI
        endm        
        
mFS_FCreate     macro
        mov     wr8,#API_FS_FCreate
        lcall   RSTBankAPI   
        endm
        
mFS_FCreatebyLN     macro
        mov     wr8,#API_FS_FCreatebyLN
        lcall   RSTBankAPI   
        endm
        
      
mFS_FClose      macro
        mov     wr8,#API_FS_FClose
        lcall   RSTBankAPI   
        endm    
//        
                               
mFS_FRemove     macro
        mov     wr8,#API_FS_FRemove
        lcall   RSTBankAPI   
        endm
        
mFS_GetFileLen          macro
        mov     wr8,#API_FS_GetFileLen
        lcall   RSTBankAPI   
        endm  
        
        
mFS_GetUnalignedLen     macro    
        mov     wr8,#API_FS_GetUnalignedLen
        lcall   RSTBankAPI
        endm        

mFS_GetCrtTime     macro    
        mov     wr8,#API_FS_GetCrtTime
        lcall   RSTBankAPI
        endm        

mFS_GetWrtTime     macro    
        mov     wr8,#API_FS_GetWrtTime
        lcall   RSTBankAPI
        endm        
  
mFS_FS_ModUnalignedLen     macro    
        mov     wr8,#API_FS_ModUnalignedLen
        lcall   RSTBankAPI
        endm      
  
mFS_FSeek               macro 
        local   NormalSeek
		cmp		r11,#03h
        jne		NormalSeek
        mov     wr8,#API_FS_FFSeek  
        lcall   RSTBankAPI
        jr		FFSeekCpl         
       
NormalSeek:  
        mov     wr8,#API_FS_FSeek
        jmp   	RSTFS2API         		 
FFSeekCpl:
        endm


mFS_FTell               macro
        mov     wr8,#API_FS_FTell
        jmp	   	RSTFS2API
        endm        

        
mFS_FRead               macro 
        mov     wr8,#API_FS_FRead
        jmp		RSTFS2API
        endm

mFS_FWrite              macro 
        mov     wr8,#API_FS_FWrite
        lcall   RSTBankAPI
        endm
                

mFS_GetSpace              macro
        mov     wr8,#API_FS_GetSpace
        lcall   RSTBankAPI
        endm  
                       
mFS_Backup      macro
        mov     wr8,#API_FS_Backup
        lcall   RSTBankAPI
        endm 
               
mFS_Restore              macro
        mov     wr8,#API_FS_Restore
        lcall   RSTBankAPI
        endm 
                     
mFS_GetInfo     macro     
        mov     wr8,#API_FS_GetInfo
        jmp		RSTFS2API
        endm

mFS_MakeSN     macro     
        mov     wr8,#API_FS_MakeSN
        lcall   RSTBankAPI
        endm        

mFS_FFRead     macro     
        mov     wr8,#API_FS_FFRead
        lcall   RSTBankAPI
        endm  

mFS_FRecSave     macro     
        mov     wr8,#API_FS_FRecSave
        lcall   RSTBankAPI
        endm 
		        
mFS_GetFileStartClusNum       macro
        mov     wr8,#API_FS_GetFileStartClusNum  
        lcall   RSTBankAPI
        endm 
        
mFS_DirPrev       macro
        mov     wr8,#API_FS_DirPrev  
        lcall   RSTBankAPI
        endm 
        
mFS_DirLast       macro
        mov     wr8,#API_FS_DirLast  
        lcall   RSTBankAPI
        endm 
        
mFS_DirParNext       macro
        mov     wr8,#API_FS_DirParNext  
        lcall   RSTBankAPI
        endm  
		        
mFS_CreateVolume	 macro
        mov     wr8,#API_FS_CreateVolume  
        lcall   RSTBankAPI
        endm                         
           
#endif /*__A251__ */
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif  /* _FILESYS_INCLUDE */

