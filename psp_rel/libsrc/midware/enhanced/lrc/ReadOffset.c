/*
 *******************************************************************************
 *                ACTOS AP
 *        enhanced module impletementation
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 *******************************************************************************
 */

#include <actos.h>
#include <enhanced.h>
#include "ReadLyricFile.h"
#pragma name(MW_EH_LRC_SUB2)

const char offsetkeyword[] =
{ 8, '[', 'O', 'F', 'F', 'S', 'E', 'T', ':' };//8��buffer�ĳ���


//Fdata
extern uchar DataBuffer[PAGE_BYTE];////��ȡ��ʣ�����ʱ��͸�����ݵķ���
int16 far g_keyWordPos;//�ڵ�ǰBUF�е�λ��
static bool far result;

//Edata
extern HANDLE lyricFileHandle;
uint8 offset_p1s;//0.1sֵ
uint8 offset_sec;//��ֵ
bool positiveflag;//���ű�־


bool SearchMem(BYTE *sBuff, const char *memBuff);
CharType_e checkTypeChar(uchar cdata);
bool GetOffsetData(void);
bool ReadPageData(void);

#pragma renamecode(MW_EH_LRC_SUB2)
/********************************************************************************
 * Function:  bool ReadOffset(HANDLE filehandle)
 * Description : ��������ļ��е� offset ��ǩ��ֵ
 * input:    HANDLE filehandle: ����ļ��ľ��
 * output:   bool return: �����Ƿ���OFFSET ��ǩ
 *             ���У������ �������� offset_sec,offset_ms��
 ********************************************************************************/
bool ReadOffset(void)
{
    offset_sec = 0;
    offset_p1s = 0;
    g_keyWordPos = 512;//Ҫ�������(������512��Ҫ������)

    while (1)
    {
        ClearWatchDog();
        //ȫ����һ���ַ�һ���ַ��ؼ�⣬����Ҫ����
        result = ReadPageData();//��һ��PAGE����
        if (!result)
        {
            FS_FSeek(0, 0, lyricFileHandle);//�ص��ļ��Ŀ�ʼλ��
            return FALSE;
        }

        //�ҹؼ��� [OFFSET:
        result = SearchMem(DataBuffer, offsetkeyword);//���ִ�
        if (!result)
        {
            FS_FSeek(0, 0, lyricFileHandle);//�ص��ļ��Ŀ�ʼλ��
            return FALSE;
        }

        //�ҵ��ؼ��֣���ȡƫ��ʱ��
        result = GetOffsetData();
        if (FALSE != result)
        {
            //�ɹ��ҵ�Offset��ֵ���˳�
            FS_FSeek(0, 0, lyricFileHandle);//�ص��ļ��Ŀ�ʼλ��
            return TRUE;
        }
        //ѭ���������ǣ���ƫ��ʱ����������[OFFEST:XXX]�������������˳�
        //�����ʱ���ļ���δ���������Լ�������
    }
    return 0;//cancel the warmning
}

/********************************************************************************
 ����ԭ�ͣ� bool GetOffsetData(void)
 input:
 void :
 g_keyWordPos: ��ǰ����ָ��
 offset_p1s��char�ͣ����С��100���ƫ�����������Ҫ���ô���100��Ӧ�ø����ͣ����ڳ�������Ӵ������

 output:
 bool return:  T���ɹ��� F��ʧ��
 �ɹ�ʱ������ֵ����������������:
 char offset_ms;
 char offset_sec;


 ���ܣ�����OFFSET����
 ********************************************************************************/
bool GetOffsetData(void)
{
    uchar tmpbuff[5] =
    { 0, 0, 0, 0, 0 };
    uchar i = 0;

    positiveflag = TRUE;//Ĭ��Ϊ����

    if (DataBuffer[g_keyWordPos] == '-')
    {
        positiveflag = FALSE;
        g_keyWordPos++;
        result = ReadPageData();
        if (!result)
        {
            return FALSE;
        }
    }
    else if (DataBuffer[g_keyWordPos] == '+')
    {
        g_keyWordPos++;
        result = ReadPageData();
        if (!result)
        {
            return FALSE;
        }
    }
    else
    {
    }

    while (1)
    {
        ClearWatchDog();
        if (DataBuffer[g_keyWordPos] == ']')
        {
            //�Ѷ������ݣ���ת������������ �����ǵ�λ
            //����[OFFEST:234],ƫ��234���룬��Ϊ[2][3][4][][] ��Ӧ�Ƴ�[0][0][2][3][4]
            //������LRC�ļ�����Ҳ��ASCII�ַ�
            for (; i < 5; i++)
            {
                //����һλ,��߲�0
                tmpbuff[4] = tmpbuff[3];
                tmpbuff[3] = tmpbuff[2];
                tmpbuff[2] = tmpbuff[1];
                tmpbuff[1] = tmpbuff[0];
                tmpbuff[0] = 0;
            }

            offset_sec = tmpbuff[1] + (uint8)(tmpbuff[0] * 10);//�������
            offset_p1s = tmpbuff[2];
            return TRUE;
        }

        //��ֹ  [OFFEST:2XXX]�������
        if (checkTypeChar(DataBuffer[g_keyWordPos]) != CharType_NUM)
        {
            //�����������ֵľͳ�
            return FALSE;
        }

        if (i < 5)
        {
            //BUFF ֻ�����byte,�����˾Ͳ��ٲ���������ܻ�������
            tmpbuff[i] = DataBuffer[g_keyWordPos] - '0';
            i++;
        }

        g_keyWordPos++;
        result = ReadPageData();
        if (!result)
        {
            return FALSE; //
        }
    }
    return 0;//cancel the warmning
}

/********************************************************************************
 ����ԭ�ͣ� bool ReadPageData()
 input:
 g_keyWordPos: ��ǰ��BUFF�е�λ��

 output:
 bool return:  T���ɹ��� F�����ļ�ʧ��
 g_keyWordPos: BYTE ָ��

 ���ܣ��ж�BUFFER�����Ƿ����꣬������Ͷ���һ���µ�PAGE����
 ********************************************************************************/
bool ReadPageData(void)
{
    if (g_keyWordPos >= 512)
    {
        //�����ݲ����Ͷ�һ��PAGE����
        result = FS_FRead(DataBuffer, 1, lyricFileHandle); //��һPAGE����
        if (!result)
        {
            return FALSE;
        }
        g_keyWordPos = 0;
    }
    return TRUE;
}

/********************************************************************************
 ����ԭ�ͣ� bool SearchMem(BYTE *TempBuf , const char *strbuf)
 input:
 BYTE *TempBuf: �����ݵ�BUFF��Ϊһ��PAGE��
 const char *strbuf: Ҫ���BUFF����ַ����һ���ֽ�ΪBUFF�ĳ��ȣ�
 g_keyWordPos: ��ǰ��BUFF�е�λ��

 output:
 bool return: �ҵ��ַ���ʱΪ��
 g_keyWordPos: ��ȫ��ͬʱ������ǱȽϴ������һ�����ݵ���һλ��

 ********************************************************************************/
bool SearchMem(BYTE *sBuff, const char *memBuff)
{
    int j;
    uchar k;
    bool result_SearchMem;
    j = 0;

    while (1)
    {
        ClearWatchDog();
        result_SearchMem = ReadPageData();//�����ݲ����Ͷ�һ��PAGE����
        if (!result_SearchMem)
        {
            return result_SearchMem;//��ʧ�ܾ��˳�
        }

        k = sBuff[g_keyWordPos];

        if (checkTypeChar(k) == CharType_DNER)
        {
            k = k - 0x20;//��Ϊ��д������
        }

        if (k != memBuff[j + 1])//��Ϊ��һ��BUFF������ĳ���
        {
            j = 0;
        }
        else
        {
            j++;
        }
        g_keyWordPos++;

        if (j >= memBuff[0])
        {
            //�Ƿ�ȫ��������
            return TRUE;
        }
    }
    return 0;//cancel the warmning

}

CharType_e checkTypeChar(uchar cdata)
{
    if ((cdata >= '0') && (cdata <= '9'))
    {
        return CharType_NUM;
    }

    if ((cdata >= 'A') && (cdata <= 'Z'))
    {
        return CharType_UPER;
    }

    if ((cdata >= 'a') && (cdata <= 'z'))
    {
        return CharType_DNER;
    }

    if (cdata == ' ')
    {
        return CharType_SPACE;
    }
    return CharType_OTHER;
}

