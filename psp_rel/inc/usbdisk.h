#ifndef _USBDISK_H
#define _USBDISK_H

#ifdef __C251__

typedef struct
{
    uint8 vendor[8];
    uint8 productid[16];
    uint8 productver[4];
}CDROM_INFO_t;

bool UDiskInit(void *pcallback, char udisk_set);
void UDiskSetCDROMInfo(CDROM_INFO_t* cdrom_info); //����CDROM�ĳ��̼���Ʒ��Ϣ,Ҫ�ڵ���UdiskInitǰ����; 
void UDiskSetDiskNum(char selector); //���ô��̳�����Ŀ,Ҫ�ڵ���UdiskInitǰ����
char UDiskGetDiskInfo(void);         //ȡ�ô�����Ŀ��� bit7:�Ƿ���� bit6:�Ƿ���� bit5:�Ƿ�忨
bool UDiskSetWP(char letter);
bool UDiskCheckWP(char letter);
void DisableSuspend(void);           //��ֹsuspend
bool UDiskRun( int para );           //para=0:����������=1:Switch from MTP������
bool UDiskExit(void);
void UDiskHandle(void);
BYTE UDiskGetStatus(void);
BYTE CardWPCheck(void);
bool UDiskPullCardOut(void); //jpu
void AlwaysShowCardDisk(BYTE para);  //0:����ʾCard���̷� 1������ʾCard���̷�
BYTE UDiskGetCardStatus(void);       //��ȡ���Ƿ���ڵ���Ϣ
void UDiskSetAutoRunMode(int8 mode); //����Autorun����ʱ���Ƿ���ʾ��mode = 0,���أ�mode = 1,��ʾ;Ҫ�ڵ���UdiskInitǰ����

#endif /*__C251__*/
#endif /*_USBDISK_H*/




