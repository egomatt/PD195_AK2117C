/********************************************************************************
 *                              USDK 5102
 *                            Module: POWER MANAGER
 *                 Copyright(c) 2001-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *    wuyueqian     2009-9-07 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * file
 * brief
 * Author   wuyueqian
 * par
 *
 *      Copyright(c) 2001-2007 Actions Semiconductor, All Rights Reserved.
 *
 * version 1.0
 * date  2008/6/12
 *******************************************************************************/

#include <basal.h>
#include <decC.h>

#pragma name(MW_BS_AD_CMDR)

HANDLE m_deFileHandle;
Open_param_t *m_deOpenparam;
uint32 deApointerSave;
uint32 deBpointerSave;
uint32 deBreakPointSave; //��ǰ�ļ��ϵ�ĵ�ַ���棬һ������Ϊ��λ��m_read_date����
decBreakPointInfor_t m_breakPointInfo;//ֻ����һ�ζϵ���Ϣ������A��
uint32 deFilepages;//��ǰ�����ļ���ҳ��
BYTE m_backupmode;
play_status_t play_status;
BYTE * m_deDataBuffer;//[512];//����ʱ�õ�BUFFER
uint16 m_deOldDspIntAddrSav;//����ɵ�INT��ַ
uint16 m_deOldDmaIntAddrSav;
uint8 m_decTimer_Sav;
time_t m_Totaltimelength;
bool m_deABok_sav;
bool m_deAPELastSecond;

#define READ_SECTOR_BYTE_NUMBER 512
#define MOVE_BACKWARD_ONE_PAGE_FLAG  0x22
#define MOVE_FORWARD_ONE_PAGE_FLAG 0x11
#define RANDOM_CHOOSE_ONE_PAGE_FLAG 0x33

#define ONE_PAGE_BITNUM 9

#ifndef PC
#pragma ROM(large)
void GetFSErrorTYPE(void);
#pragma ROM(huge)
#else
void GetFSErrorTYPE(void);
decBreakPointInfor_t g_decBreakPointInfor;
#endif

#pragma renamecode(MW_BS_AD_CMDR)

extern void (*fun_decrypt_hook)(void *, uint32);

#pragma ROM(large)
uint8 m_deRead_Data(uchar readFileDirection, uint32 position)
{
    bool result = TRUE;
    uint32 posPage = 0;
    uint32 posDistance = 0;
    //CLEAR_WATCHDOG_MIDWARE();
    ClearWatchDog();

    //�ϵ��¼���ǽ��������Ϣ

    if ((play_status.status == PLAYING_REACH_END) || (play_status.status == PLAYING_ERROR) || 
            (play_status.status == PLAYING_REACH_HEAD))
    {
        return 0xff;
    }

    deBreakPointSave = (uint32)(FS_FTell(m_deFileHandle)); //- 1;

    if (deBreakPointSave == deFilepages)
    {
        play_status.status = PLAYING_REACH_END;
        return 0xff;
    }

    if (MOVE_BACKWARD_ONE_PAGE_FLAG == readFileDirection)
    {
        result = FS_FSeek(2, FS_SEEK_BFROMCUR, m_deFileHandle);
    }
    else if (RANDOM_CHOOSE_ONE_PAGE_FLAG == readFileDirection)
    {
        posPage = position >> ONE_PAGE_BITNUM;
        if (posPage > deBreakPointSave)
        {
            result = FS_FSeek((int32)(posPage - deBreakPointSave), FS_SEEK_FFROMCUR, m_deFileHandle);
        }
        else if (posPage < deBreakPointSave)
        {
            posDistance = (deBreakPointSave) - posPage;
            if (posDistance < (deBreakPointSave << 1))
            {
                result = FS_FSeek((int32)(posDistance), FS_SEEK_BFROMCUR, m_deFileHandle);
            }
            else
            {
                result = FS_FSeek((int32)(posPage), FS_SEEK_FFROMSTART, m_deFileHandle);
            }

        }
        else
        {		
        }
    }
    else
    {		
    }

    if (FALSE == result)
    {
        GetFSErrorTYPE();
        return FALSE;
    }

    switch (g_decControlInfor.ABSetFlag)
    {
        case AB_FLAG_DEAL_AB_PROCESS:
        {
            if (play_status.status == PLAYING_WAIT_B) //�Ѿ�����B����߳������ȴ�stop����
            {
                break;
            }

            if ((play_status.status == PLAYING_AB) && (deBreakPointSave >= deBpointerSave))//�ŵ�B��
            {
                if (FALSE != m_deABok_sav) //�����ٲ�����
                {//������,��B��
                    m_deABok_sav = FALSE;
                    play_status.status = PLAYING_PLAYING;
                    g_decControlInfor.ABSetFlag = AB_CLEAR;
                    g_decControlInfor.PlayMode = PLAY_MODE_NORMAL; //not need
                    //play_status.signal = SIGNAL_CLEAR_B;//֪ͨABҪ��B��
                    play_status.signal = 0;//modify by wuyueqian 2010-4-3
                }
                else
                {//����δ��,֪ͨAP�ص�A��
                    play_status.status = PLAYING_WAIT_B;
                    play_status.signal = SIGNAL_REACH_B;
                }
            }
            break;
        }

        case AB_FLAG_SET_A: //������A���ab flagΪ0��������AB�������B��Ҳ��ú�ſ�ʼAB���Ŵ���
        {
            deApointerSave = deBreakPointSave;
            g_decControlInfor.ABSetFlag = AB_CLEAR;//��ʵ��AB_FLAG_SET_A�ͷ�0XFF�Ϳ�����
            memcpy(&m_breakPointInfo, &g_decBreakPointInfor, sizeof(decBreakPointInfor_t));
            break;
        }

        case AB_FLAG_SET_B:
        {
            deBpointerSave = deBreakPointSave;
            play_status.signal = SIGNAL_REACH_B;
            play_status.status = PLAYING_WAIT_B; //ֻ�����m_deRead_Data��������Ч��StopBPlay������PLAYING_WAIT_A
            g_decControlInfor.ABSetFlag = AB_FLAG_DEAL_AB_PROCESS; //�Ժ����Ƿ�B��
            break;
        }

        default:
        {
            break;
        }

    }

    result = FS_FRead(m_deDataBuffer, 1, m_deFileHandle);

    deBreakPointSave = (uint32)(FS_FTell(m_deFileHandle));
    g_decReadFilePara.ReadFileCurAddr = (deBreakPointSave - 1) * READ_SECTOR_BYTE_NUMBER;

	if (fun_decrypt_hook != NULL)
    {
        (*fun_decrypt_hook)(m_deDataBuffer, deBreakPointSave-1);
    }

    if (result == 0)
    {
        GetFSErrorTYPE();
        return FALSE;
    }
    return TRUE;
}

/*  FS_GetInfo()
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

void GetFSErrorTYPE(void)
{
    char result;

    result = (char)FS_GetInfo();
    switch (result)
    {
        case FS_INFO_OVEFLOW: //FIX ME
        {
            if (FS_FTell(m_deFileHandle) <= 1)//seek��ͷʱ����ǰָ����1 cailizhen 2013-2-7 10:04
            {
                play_status.status = PLAYING_REACH_HEAD;
            }
            else
            {
                play_status.status = PLAYING_REACH_END;
            }
            break;
        }

        case FS_INFO_RW_ERR:
        {
            play_status.status = PLAYING_ERROR;
            break;
        }

        default:
        {
            break;
        }
    }
}

#ifndef PC
#pragma ROM(huge)
#endif
