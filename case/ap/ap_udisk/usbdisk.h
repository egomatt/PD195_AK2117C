#ifndef _USBDISK_H
#define _USBDISK_H

#ifdef __C251__

//#define BACKLIGHTCTL
bool UDiskInit(void *pcallback, char udisk_set);
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
#endif /*__C251__*/
#endif /*_USBDISK_H*/




