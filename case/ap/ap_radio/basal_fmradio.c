/*
 *******************************************************************************
 *                ACTOS AP
 *        FM tunner basal module implementation
 *
 *        (c) Copyright Actions Co,Ld.
 *
 *******************************************************************************
 */
#include "ap_radio.h"

#pragma name(MOD_BS_FMRADIO)

//-----basal module ----

uint16 m_FMStationTab[30]; //��̨Ƶ�ʶ��ձ�(ע:Ƶ�ʷ�Χ87M-108M ��KΪ����).
fm_play_status_t m_FMPlaystatus; //��ǰ��̨��Ϣ
FMStatus_Tab_t d_FMStatus; //��FMӲ����ȡ�ĵ�ǰƵ�ʺ�������Ϣ
extern fm_play_status_t FMStatus; //��ǰ��̨��Ϣ,8 bytes


/*********************************************************
 *input:  buf---ptr point to buffer which is wait to deal
 *          curfreq----current freq which will be inserted to buffer
 *          totalnum---current total freq num in freq list
 *output: no
 *Description:  insert current freq into freq list
 *note:  freq list from small to large
 **********************************************************
 */
void SequenceFreqlist(uint16* buf, uint16 curfreq, uint8 totalnum)
{
    uint8 i, cur_pos;
    uint16* dealbuf;
    int8 j;

    dealbuf = buf;
    cur_pos = 0;

    for (i = 0; i < 30; i++)
    {
        if (curfreq < (*dealbuf))
        {
            cur_pos = i;
            if (cur_pos == 29)
            {
                *dealbuf = curfreq;
                m_FMPlaystatus.station = i + 1;
                break;
            }

            for (j = 28; j >= (int8) cur_pos; j--)
            {
                *(buf + j + 1) = *(buf + j);
            }
            *dealbuf = curfreq;
            m_FMPlaystatus.station = i + 1;

            break;
        }
        else if (curfreq == (*dealbuf))
        {
            break; //���ظ��������±�
        }
        else
        {
            if ((*dealbuf == 0) && (i < totalnum))
            {
                *dealbuf = curfreq;
                m_FMPlaystatus.station = i + 1;
                break;
            }
            dealbuf++;
        }
    }
    return;
}

/*
 ********************************************************************************
 *             BOOL fmSendCommand(uint8 cmd, uint16 param)
 *
 * Description : FM�����
 *
 * Arguments   :
 *
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************
 */
uint8 fmSendCommand(uint8 cmd, uint16 param)
{
    uint8 i;
    uint8 k;
	
    uint16 Tempfreq;
    switch (cmd)
    {
        case MC_OPEN:
        return FM_Init(m_FMPlaystatus.mode); //FM��ʼ��
		//return FM_Init(m_FMPlaystatus.mode, (uint8) param); //FM��ʼ��
        case MC_PLAY:
        Tempfreq = m_FMPlaystatus.freq ;
        FM_SetFreq(Tempfreq); //���õ�̨Ƶ��
        break;

        case MC_STEP:
        if (m_FMPlaystatus.station_count == 0)
        {
            return FALSE;
        }
        k = 0;
        switch (param)
        {
            case UP: //�ҵ���һ��̨Ƶ��
/*			Tempfreq = m_FMPlaystatus.freq;
			while(flag)
			{
				m_FMPlaystatus.freq ++;
				if(m_FMPlaystatus.station != 0)
				{
					flag = 0;
				}
			}*/
            if (m_FMPlaystatus.station >= m_FMPlaystatus.station_count)
            {
                m_FMPlaystatus.station = 0;
            }
            m_FMPlaystatus.station++;
            for (i = 0; i < 30; i++)
            {
                if (m_FMStationTab[i] != 0)
                {
                    k++;
                    if (k == m_FMPlaystatus.station)
                    {
                        break;
                    }
                }

            }
            if (i >= 30) // ��û���ҵ���̨,�������õ�̨��
            {
                m_FMPlaystatus.station_count = k - 1;
                m_FMPlaystatus.station = 1;
            }
            break;
            case DOWN: //�ҵ���һ��̨Ƶ��
            if (m_FMPlaystatus.station == 1)
            {
                m_FMPlaystatus.station = m_FMPlaystatus.station_count + 1;
            }
            m_FMPlaystatus.station--;
            for (i = 0; i < 30; i++)
            {
                if (m_FMStationTab[i] != 0)
                {
                    k++;
                    if (k == m_FMPlaystatus.station)
                    {
                        break;
                    }
                }
            }
            if (i >= 30) // ��û���ҵ���̨,�������õ�̨��
            {
                m_FMPlaystatus.station = m_FMPlaystatus.station_count = k + 1;
            }
            break;
            case CUR: //��ǰ��̨Ƶ��
            if (m_FMPlaystatus.station == 0)
            {
                m_FMPlaystatus.station = 1;
            }
            for (i = 0; i < 30; i++)
            {
                if (m_FMStationTab[i] != 0)
                {
                    k++;
                    if (k == m_FMPlaystatus.station)
                    {
                        break;
                    }
                }

            }
            if (i >= 30) // ��û���ҵ���̨,�������õ�̨��
            {
                m_FMPlaystatus.station_count = k - 1;
                m_FMPlaystatus.station = 1;
            }
            break;
            default:
            break;
        }
        if (i < 30)
        {
            m_FMPlaystatus.freq = m_FMStationTab[i];
        }
        Tempfreq = m_FMPlaystatus.freq  ;
        FM_SetFreq(Tempfreq);
        return TRUE;

        case MC_STEPGRID:
        switch (param)
        {
            case UP:
            if (((m_FMPlaystatus.freq + 1) > FM_BAND_US_TOP) && (m_FMPlaystatus.mode == 0))
            {
                m_FMPlaystatus.freq = FM_BAND_US_BOTTOM;
            }
            else if (((m_FMPlaystatus.freq + 1) > FM_BAND_JP_TOP/*900*/) && (m_FMPlaystatus.mode == 1))  //suber
            {
                m_FMPlaystatus.freq = FM_BAND_JP_BOTTOM;
            }
            else
            {
                m_FMPlaystatus.freq += 1;
            }
            break;
            case DOWN:
            if (((m_FMPlaystatus.freq - 1 ) < FM_BAND_US_BOTTOM) && (m_FMPlaystatus.mode == 0))
            {
                m_FMPlaystatus.freq = FM_BAND_US_TOP;
            }
            else if (((m_FMPlaystatus.freq - 1 ) < FM_BAND_JP_BOTTOM) && (m_FMPlaystatus.mode == 1))
            {
                m_FMPlaystatus.freq = FM_BAND_JP_TOP;//900;   //suber
            }
            else
            {
                m_FMPlaystatus.freq -= 1;
            }
            break;
            default:
            break;
        }
        Tempfreq = m_FMPlaystatus.freq ;
		FM_Mute(SetMUTE);
        FM_SetFreq(Tempfreq);
		FM_Mute(releaseMUTE);
        return TRUE;

        case MC_SEARCH:
        Tempfreq = m_FMPlaystatus.freq ;
        //S_QND.ctrl = param;
        return FM_Search(&FMStatus, Tempfreq, (uint8) param);

        case MC_SAVESTATION:
        if (m_FMPlaystatus.station_count > 29)
            return FALSE;
        if (m_FMPlaystatus.station_count != 0) //��ǰ��̨�ڵ�̨�����б�������
        {
            for (i = 0; i < m_FMPlaystatus.station_count; i++)
            {
                if(m_FMStationTab[i] == m_FMPlaystatus.freq)
                {
                    m_FMPlaystatus.station = i+1;
                    return TRUE;
                }
            }
            m_FMStationTab[m_FMPlaystatus.station_count] = m_FMPlaystatus.freq;
            m_FMPlaystatus.station_count++;
            m_FMPlaystatus.station = m_FMPlaystatus.station_count;
            
        }
        else //�µĵ�̨Ƶ�ʱ��浽��̨��
        {
            m_FMStationTab[m_FMPlaystatus.station_count] = m_FMPlaystatus.freq;
            m_FMPlaystatus.station_count++;
            m_FMPlaystatus.station = m_FMPlaystatus.station_count;
        }
        return TRUE;

        case MC_SETMODE:
        m_FMPlaystatus.mode = (Station_bank_t) param;
        break;

        case MC_SETVOLUME:
        FM_VolumeSet((uint8) param);
        m_FMPlaystatus.volume = (uint8) param;
        break;

        case MC_BACKUP:
#ifndef PC
        memcpy(((FMRADIO_param_t *) param)->param1, m_FMStationTab, 60);
        memcpy(((FMRADIO_param_t *) param)->param2, &m_FMPlaystatus, sizeof(fm_play_status_t));
#else
        memcpy((void *)param,m_FMStationTab, 60);
        memcpy((void *)(param+2),&m_FMPlaystatus, sizeof(fm_play_status_t));
#endif
        break;

        case MC_RESTORE:
#ifndef PC
        memcpy(m_FMStationTab, ((FMRADIO_param_t *) param)->param1, 60);
        memcpy(&m_FMPlaystatus, ((FMRADIO_param_t *) param)->param2, sizeof(fm_play_status_t));
#else
        memcpy(m_FMStationTab, (void *)param, 60);
        memcpy(&m_FMPlaystatus,(void *)(param+2), sizeof(fm_play_status_t));
#endif
        break;

        case MC_GETSTATUS:
        FM_GetStatus(&d_FMStatus);
        m_FMPlaystatus.freq = d_FMStatus.FM_CurrentFreq;
        m_FMPlaystatus.stereo_status = d_FMStatus.FM_Stereo_Status; //0: stereo;   1: mono
        k = 0;
//        //��ǰ��̨Ƶ��ֵ�͵�̨����ĳһƵ�ʲ��С��150KHZ���˵�̨�ʹ˵�̨���Ӧ
        /*
        for (i = 0; i < 30; i++)
        {
            m_FMPlaystatus.station = 0;
            if (m_FMStationTab[i] != 0)
            {
                k++;
                if ((m_FMPlaystatus.freq < (m_FMStationTab[i] + 1)) && (m_FMPlaystatus.freq > (m_FMStationTab[i] - 1)))
                {
                    if (k <= m_FMPlaystatus.station_count)
                    {
                        m_FMPlaystatus.station = k;
                    }
                    break;
                }
            }
        }
        */
        memcpy((fm_play_status_t *) param, &m_FMPlaystatus, sizeof(fm_play_status_t));
        break;

        case MC_GETTABLE:
        memcpy((uint16 *) param, &m_FMStationTab[0], 60);
        break;

        case MC_DELONE:
        if (m_FMPlaystatus.station == 0)
        {
            return FALSE;
        }
        //m_FMStationTab[m_FMPlaystatus.station-1]=0;
        //for (k = m_FMPlaystatus.station; k < (m_FMPlaystatus.station_count + 1); k++)
        for (k = m_FMPlaystatus.station; k < (m_FMPlaystatus.station_count); k++)
        {
            m_FMStationTab[k - 1] = m_FMStationTab[k];
        }
        m_FMStationTab[m_FMPlaystatus.station_count - 1] = 0;
        m_FMPlaystatus.station_count--;
        m_FMPlaystatus.station = 0;
        return TRUE;

        case MC_DELALL:
        for (i = 0; i < 30; i++)
        {
            m_FMStationTab[i] = 0;
        }
        m_FMPlaystatus.station = 0;
        m_FMPlaystatus.station_count = 0;
        break;

        case MC_MUTE: //����
#if 1
		FM_Mute(param);
#else
        i = 0x10;
        while ((!FM_Mute(param)) && (i > 0))
        {
            i--;
            ClearWatchDog();
            FM_GetStatus(&d_FMStatus);
        }
#endif
        break;

        case MC_CLOSE:
        FM_Standby();
        FM_Exit();
        break;

        case MC_GETFREQ:
        *(uint16 *) param = FMStatus.freq;
        break;

        case MC_SETFREQ:
        m_FMPlaystatus.freq = param;
        Tempfreq = m_FMPlaystatus.freq ;
        FM_SetFreq(Tempfreq);
        break;

        default:
        break;
    }

    return TRUE;

}

