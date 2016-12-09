/*
 *******************************************************************************
 *                lyric sequencer get functions
 *                    enhanced library
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 ע�⽫Z80��С�˸�Ϊ51�Ĵ��
 ȷ��ָ����16λ
 *
 *******************************************************************************
 */
#include    "ReadLyricFile.h"
#include    <filesys.h>
#include    <stringH.h>
#include	<enhanced.h>

#pragma name(MW_EH_LRC_SUB5)

//edata
extern LrcTime_t * pTimeLabel;
extern uint8 TimeRecordCounter;//һ����TimeRecordCounter+1����ǩ ��ʾ��������һ������
extern lyricSaveFormat_T lyricSave;

extern uint8 offset_p1s;//0.1sֵ   //Ӧ�ø��һ�����ݽṹ
extern uint8 offset_sec;//��ֵ
extern bool positiveflag;//���ű�־


void HandleLastLyric(void);
void HandleID3Overlap(void);
void SortLyricData(void);
extern void AddOffsetData(LrcTime_t *timetab, BYTE recordCNT);
extern bool SaveLyricInVRAM(void);
extern void LyricDivScreen(void);
void AddOffsetData(LrcTime_t *timetab, BYTE recordCNT);

#pragma renamecode(MW_EH_LRC_SUB5)

void SortLyricData(void)
{
    uchar i, j;
    LrcTime_t timeData;
    pTimeLabel = (LrcTime_t *) lyricSave.LyricTimeAddrBuffer;
    for (i = 1; i <= TimeRecordCounter; i++)
    {
        ClearWatchDog();
        for (j = i; j > 0; j--)
        {
            //ClearWatchDog();
            if (pTimeLabel[j].min > pTimeLabel[j - 1].min)
            {
                break;//continue;
            }
            else if ((pTimeLabel[j].min == pTimeLabel[j - 1].min) && (pTimeLabel[j].sec > pTimeLabel[j - 1].sec))
            {
                break;//continue;
            }
            else if ((pTimeLabel[j].min == pTimeLabel[j - 1].min) && (pTimeLabel[j].sec == pTimeLabel[j - 1].sec)
                    && ((pTimeLabel[j].p1s & 0x0f) >= (pTimeLabel[j - 1].p1s & 0x0f)))
            {
                break;//continue;
            }
            else//һ��ҪС�ڲŽ�������֤���˳��
            {
                memcpy(&timeData, pTimeLabel + j, sizeof(LrcTime_t));
                memcpy(pTimeLabel + j, pTimeLabel + j - 1, sizeof(LrcTime_t));
                memcpy(pTimeLabel + j - 1, &timeData, sizeof(LrcTime_t));
            }
        }
    }
    return;
}

//��������и�ʷ������̫С������   ���һ�з���û�вο��㣬ÿ����2�룬
void HandleLastLyric(void)
{
    pTimeLabel = (LrcTime_t *) lyricSave.LyricTimeAddrBuffer;
    pTimeLabel += (TimeRecordCounter - 1); //�޸ĵ���ff aa 55 �����ӵ���һ��
    if ((pTimeLabel->p1s & 0xf0) != 0) //TimeRecordCounter Ϊ��Чʱ����������+1 ��ʾ����FF AA 55��һ��
    {
        if (pTimeLabel->sec < 58)
        {
            pTimeLabel->sec += 2;//��2�룬���һ��û�вο���
            return;
        }
        else if (pTimeLabel->min < MAX_MINUTE)
        {
            pTimeLabel->sec -= 58;
            pTimeLabel->min += 1;
            return;
        }
        else
        {
        }
    }
    else
    {
        return;//means ֻ��һ����Ӧ
    }
}

void HandleID3Overlap(void)
{
    //id3 ���90   /16  =  6 ����¼
    uchar i = 0;
    pTimeLabel = (LrcTime_t *) lyricSave.LyricTimeAddrBuffer + 1;
//    pTimeLabel++;////��һ��ʱ���ǩһ����ID3���ӵڶ�����ʼ����
    ////ID3�������кܶ���ʱ���ǩ���ӵڶ���������λ������ʾΪ��һ��

    while ((((pTimeLabel->p1s) & 0xf0) != 0) &&(pTimeLabel->min != 0xff))	// 
    {
        pTimeLabel++;
    }

    //����Ϊ0ʱ��ʾ���˵�һ����ʵĿ�ʼ��
    //��0�Ķ�����Ϊ1�룬�Ժ�����codec��ʱ����ȡֵ������ʱ�䲻��Ҳû�й�ϵ


    while (i < 10) //�����10��
    {
        if (((pTimeLabel->sec) > 0) || ((pTimeLabel->min) > 0))
        {
            return;
        }
        pTimeLabel->sec = 1; //����������Ϊ1 ��֤ID3��ʾ����Ϊ1�� ���벻��Ҫ����
        pTimeLabel++;
        i++;
    }
    return;
}

void AddOffsetData(LrcTime_t *timetab, BYTE recordCNT)
{
    BYTE i;//��λ��¼
    BYTE j;

    if ((offset_p1s == 0) && (offset_sec == 0))
    {
        //��Ҫ�����ʱ��Ϊ0����Ҫ��
        return;
    }

    if (FALSE == positiveflag)
    {
        //Ϊ����
        for (; recordCNT > 0; recordCNT--)
        {
            //ClearWatchDog();
            if (((timetab->sec != 0) || (timetab->min != 0)) || (timetab->p1s != 0))
            {
                //100ms(0.1s)��λ��
                j = offset_p1s + timetab->p1s;
                i = 0;
                if (j >= 10)
                {
                    j = j - 10;
                    i = 1;
                }
                timetab->p1s = j;

                //����λ��
                j = offset_sec + i + timetab->sec;
                i = 0;
                if (j >= 60)
                {
                    j = j - 60;
                    i = 1;
                }
                timetab->sec = j;

                //��λ����
                timetab->min += i;
            }
            timetab++; //ָ���һ
        }
    }
    else
    { //Ϊ����

        BYTE tmpbuf1[4];
        BYTE tmpbuf2[4];

        //offset_sec�п��ܴ���60�����ܳ���1���ӣ�ֻ��С��100
        //ʱ���ǩ�������Ѿ������ˣ��䳬��60������ӽ�λ������
        //��ƫ����80��ʱ��ԭ��ʱ��Ϊ1���� 10�� 100����ʱ��
        //�Ƚ���Ȼ����������ȴ�Ǵ����
        //tmpbuf1[0] = offset_p1s;
        //tmpbuf1[1] = offset_sec;
        //tmpbuf1[2] = 0;
        //tmpbuf1[3] = 0;
        //������Ҫ�ĳɴ�˵�   wuyueqian-2009-10-22
        if (offset_sec >= 60)
        {
            tmpbuf1[0] = 1;
            tmpbuf1[1] = offset_sec - 60;
        }
        else
        {
            tmpbuf1[0] = 0;
            tmpbuf1[1] = offset_sec;
        }
        tmpbuf1[2] = offset_p1s;
        tmpbuf1[3] = 0;
        #ifdef PC
		SIM_ChgLongPC(tmpbuf1);
        #endif

        for (; recordCNT > 0; recordCNT--)
        {
            //ClearWatchDog();
            if ((timetab->min != 0) || (timetab->sec != 0) || (timetab->p1s != 0))
            {
                tmpbuf2[0] = timetab->min;
                tmpbuf2[1] = timetab->sec;
                tmpbuf2[2] = timetab->p1s;
                tmpbuf2[3] = 0;
                #ifdef PC
				SIM_ChgLongPC(tmpbuf2);
                #endif
                if (*(long *) tmpbuf1 > *(long *) tmpbuf2)
                {
                    //����ʱ������OFFSETֵС�����
                    //memcpy(tmpbuf1, tmpbuf2, sizeof(tmpbuf1));
                    //offset_p1s = tmpbuf1[0];
                    //offset_sec = tmpbuf1[1];
                    timetab->min = 0;
                    timetab->sec = 0;
                    timetab->p1s = 0;
                    timetab++; //ָ���һ
                    continue;
                }

                //100ms(0.1s)��λ��
                if (timetab->p1s >= offset_p1s)
                {
                    j = timetab->p1s;
                    i = 0;
                }
                else
                {
                    j = timetab->p1s + 10;
                    i = 1;//����1
                }
                timetab->p1s = j - offset_p1s;

                //����λ��
                if (timetab->sec >= (tmpbuf1[1] + i))
                {
                    j = timetab->sec - i;
                    i = 0;
                }
                else
                {
                    j = timetab->sec + 60 - i;
                    i = 1;
                }
                timetab->sec = j - tmpbuf1[1];

                //��λ����
                timetab->min -= (i + tmpbuf1[0]);
            }
            timetab++; //ָ���һ
        }
    }
    return;
}

bool Sort_Div_Save(void)
{
    //��֤ID3���㹻1�����ʾ����ʱ�ǩӦ��1�뿪ʼ
    HandleID3Overlap();
    HandleLastLyric();
    ClearWatchDog();
    SortLyricData();
    ClearWatchDog();
    LyricDivScreen();
    AddOffsetData((LrcTime_t *) lyricSave.LyricTimeAddrBuffer, TimeRecordCounter);
    ClearWatchDog();
    return SaveLyricInVRAM();
}
