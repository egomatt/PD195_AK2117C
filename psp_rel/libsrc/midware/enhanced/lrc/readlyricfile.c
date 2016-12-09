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
#pragma name(MW_EH_LRC_SUB4)
//edata
extern HANDLE lyricFileHandle;
extern uint8 LanguageFlag;//LanguageFlag = CharSet���ַ�����
//��Ȼ�ܶ�����ϵ����ֻ�������������ֽڱ�ʾ����һ���ֽڱ�ʾһ���ַ�
//ͳһ��Ascii��ʾһ���ֽ�, chinese��ʾ�����ֽ�
extern lyricSaveFormat_T lyricSave;
LrcTime_t * pTimeLabel;
uint8 TimeRecordCounter;//һ����TimeRecordCounter+1����ǩ ��ʾ��������һ������
languageLrc_t LyricLanguageFlag;//���Ա�־       { 01h: Ӣ�� || 02h: ���� }
BOOL AddLinkCharFlag = FALSE;//����ʱҪ�����ӷ���־ 00:����,  0ffh: �� �������Ӧָ��ǰһ���ַ�
uint8 * pLyricStrBufferOffest;
uint8 * pDataBufferOffest;
uint16 PointColCounter;//�����˵��������������
uint8 HalfChineseTempSavRam;//������ִ����ݴ�
uint8 LyricLineCounter;//�������������,��¼��ǰ��ʶ������������ڶ�LRC�ļ�����ʱ�õ���
uint8 No1DataNotStaConFlag;//��һ���ַ��ǡ�[����־����������  //00h: "][" �����  0ffh: "]x"�����
uint8 DivScreenPageCNT;//����������  03-9-12 16:21
uint8 SameStringCNT;//���ʱ���ǩ����һ���ַ���������(Ϊ1ʱ��ʾ��2��ʱ���ǩ����һ���ַ���������)

#pragma userclass(near = DATA_BUFFER)
uchar DataBuffer[PAGE_BYTE];////��ȡ��ʣ�����ʱ��͸�����ݵķ���
#pragma userclass(near = default)

//fardata
bool far No1BitNotZero;//�ڶ�����ļ�ʱ������ʱ��һλ�Ƿ�Ϊ0  ��Ϊ0��һλΪ0 /0ffh:  ǰ�����в���0������
word far SaveTheStartDivAddr;//��¼��ʼ�����Ŀ�ʼʱ��Ƭָ���ַ��
//BOOL far LyricFileFlag;//�Ƿ���ڸ���ļ���־  { 01h:�и���ļ� || 00h:�޸���ļ� }
//uint8 far TmpSavTimeRecCNT;//��ʱ��¼ ʱ���¼ ������������ֵ

uint8 far lyricFilePageNum;
uint16 far lyricFileLastPageByte;
uint8 far lyricFilePagePos;

uchar *far pTimeLabelChar;//��ʱ���ǩ��BCD�仯����


BOOL ExplainLyricFile(const char* id3InfoBuffer);
extern returnDealString_t DealSaveString(uchar charData);
extern uchar ReadID3FromBuf(void);

returnReadData_t TimeStartDataRead(void);
returnReadData_t MinuteDataRead(void);
returnReadData_t SecondDataRead(void);
returnReadString_t ReadStringData(void);

void DealTimeData(void);
returnReadString_t SaveStringData(uchar tempData);

#pragma renamecode(MW_EH_LRC_SUB4)

BOOL ExplainLyricFile(const char* id3InfoBuffer)
{
    returnReadData_t result_data;
    returnReadString_t result_string;
    No1BitNotZero = 0;

    //ClearWatchDog();
    //LyricLanguageFlag = English_LRC;
    //�ǵ���databankȷ�����Ż��ⲿ�֣���Ϊ��ȡoffest��ʱ���Ѿ�����ǰ��
    //һ���������ˣ��������´���
    //֮ǰ�Ǹ��Ƹ�����ݵ�offestBuffer�������Ǹ��Ƶ�dataBuffer����dababank��ԭ��
    lyricFilePageNum = (uint8) FS_GetFileLen(lyricFileHandle);
    lyricFileLastPageByte = (uint16) FS_GetUnalignedLen(lyricFileHandle);

    repeat_parse_file_again: lyricFilePagePos = 0;
    if (!FS_FSeek(0, 0, lyricFileHandle))
    {
        return 1;
    }
    memset((char *) &lyricSave.LyricTimeAddrBuffer, 0, sizeof(lyricSaveFormat_T) - LYRIC_FILENAME_SIZE
            - LYRIC_PAGEMODE_SIZE);
    memset(DataBuffer, 0, PAGE_BYTE);//��һ��Ӧ�������ǰ��������
    memcpy(DataBuffer, id3InfoBuffer, 90); //�����趨��ԭ����90��
    //DataBuffer[90] = 0x0;

    repeat_parse_id3_again: TimeRecordCounter = 0;//�����¼������
    HalfChineseTempSavRam = 0;//�������ּ�¼��
    LyricLineCounter = 0;//��������������
    PointColCounter = 0; //��������������
    SameStringCNT = 0;//��һ�и�ʶ�Ӧ��ʱ���ǩ����Ĭ��Ϊ0����ʾ1��


    //��ȡID3��Ϣ
    if (0 != ReadID3FromBuf())
    {
        goto repeat_parse_id3_again;
    }

    //��ȡ�����Ϣ�ļ�ϵͳ���ļ�ʱӦ�ÿ���������������Ͷ������������
    pDataBufferOffest = DataBuffer + PAGE_BYTE;//for timeStartRead File

    while (1)
    {
        ClearWatchDog();
        repeat_searchTimeStart: result_data = TimeStartDataRead();
        if ((READ_DATA_FILE_END == result_data) || (READ_DATA_ZERO == result_data))
        {
            goto rightExplain;
        }
        else if (READ_DATA_FILE_ERROR == result_data)
        {
            goto errorExplain;
        }
        else
        {
            //right
        }

        repeat_minuteDataRead: //sldfkjsdlf[]
        result_data = MinuteDataRead();
        if ((READ_DATA_FILE_END == result_data) || (READ_DATA_ZERO == result_data))
        {
            goto rightExplain;
        }
        else if (READ_DATA_FILE_ERROR == result_data)
        {
            goto errorExplain;
        }
        else if (READ_DATA_NOT_USE_DATA == result_data)
        {
            goto repeat_searchTimeStart;
        }
        else
        {
            //right
        }

        result_data = SecondDataRead();
        if ((READ_DATA_FILE_END == result_data) || (READ_DATA_ZERO == result_data))
        {
            goto rightExplain;
        }
        else if (READ_DATA_FILE_ERROR == result_data)
        {
            goto errorExplain;
        }
        else if (READ_DATA_NOT_USE_DATA == result_data)
        {
            goto repeat_searchTimeStart;
        }
        else
        {
            //right
        }

        //record string
        result_string = ReadStringData();
        if ((READ_STRING_FILE_END == result_string) || (READ_STRING_ZERO == result_string))
        {
            goto rightExplain;
        }
        else if (READ_STRING_FILE_ERROR == result_string)
        {
            goto errorExplain;
        }
        else if (READ_STRING_DONE == result_string) //���һ�н���
        {
            goto repeat_searchTimeStart;
        }
        else if (READ_STRING_TIME_START == result_string)//sdkfjsdlkf[]
        {
            goto repeat_minuteDataRead;
        }
        else if (READ_STRING_LRC_OVERFLOW == result_string)//sdkfjsdlkf[]
        {
            goto rightExplain;
        }
        else //Ӣ��ģʽ��Ϊ����ģʽ��ȫ���ض� //READ_STRING_REREAD_FILE
        {
            goto repeat_parse_file_again;
        }

    }

    //ReadLyricFileRet
    //�������һ����ǩ
    errorExplain: return ERROR_EXPLAIN;
    rightExplain:
    //add the final timeLabel,
    pTimeLabel->min = 0xff;
    pTimeLabel->sec = 0x55;
    pTimeLabel->p1s = 0xaa;
    //pTimeLabel->buffaddr_High = (uchar)(((WORD)pLyricStrBufferOffest & 0xff00)>>8);//�����ʽ������ŵ���һ��λ��
    //pTimeLabel->buffaddr_Low= (uchar)((WORD)pLyricStrBufferOffest & 0x00ff);
    *(WORD *) (&(pTimeLabel->buffaddr_High)) = (WORD) (pLyricStrBufferOffest);
    //pTimeLabel++;//�����ˣ���������
    return RIGHT_EXPLAIN;

}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//���³�����LRC�ļ��ж��� 1PAGE ���ݵ� DATABUFFER
//input :
//output:(LRCFileEndFlag)=04h,�ļ�����
//output:a=0 : ���ļ�û�� ;  a=0ffh : ���ļ��� ; a=04h : ���ļ�����
//�ļ���ȡʱ���������һҳ���ص���Ȼ��512byte����������в��㣬ֻ��ͨ��FS_GetUnalignedLen����ȡ���һ��������ʵ���ֽ�
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
returnReadFile_t ReadLRCFileDataToBuffer(void)
{
    bool result;

    result = FS_FRead(DataBuffer, 1, lyricFileHandle);

    if (FALSE == result)
    {
        if (FS_INFO_OVEFLOW == FS_GetInfo())
        {
            return READ_FILE_END;
        }
        else
        {
            return READ_FILE_ERROR;
        }
    }
    else
    {
        lyricFilePagePos++;
        if (lyricFilePagePos == lyricFilePageNum)
        {
			if(lyricFileLastPageByte != 0x00)
			{
				*(DataBuffer + lyricFileLastPageByte) = 0x00;
			}             
        }

        return READ_FILE_NORMAL;
    }
}

bool IfDataBufferEnd(void)
{
    ClearWatchDog();
    if (((uint32) pDataBufferOffest) < ((uint32) (DataBuffer + PAGE_BYTE)))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

returnReadData_t SecondDataRead(void)
{
    uchar tempData;
    uchar secondNum = 0;
    returnReadFile_t result;
    bool lyricSecReadOkFlag = FALSE;
    bool secondReadDone = FALSE;
    bool SecondPointFlag = FALSE;
    No1BitNotZero = FALSE;

    while (TRUE)
    {
        //ClearWatchDog();
        while (!IfDataBufferEnd())
        {
            //ClearWatchDog();
            tempData = (*pDataBufferOffest++);
            if (tempData == 0)
            {
                return READ_DATA_ZERO;
            }
            else if (tempData == ']')
            {
                DealTimeData();
                TimeRecordCounter++;
                PointColCounter = 0;
                return READ_DATA_DONE;
            }
            else if (tempData == '.')
            {
                if (!SecondPointFlag)
                {
                    SecondPointFlag = TRUE;
                }
                else
                {
                    return READ_DATA_NOT_USE_DATA;
                }
            }
            else if ((tempData > '9') || (tempData < '0'))
            {
                return READ_DATA_NOT_USE_DATA;
            }
            else
            {
                if ((tempData == '0') && (No1BitNotZero == FALSE))//[00001:]----->[1:]
                {
                    continue;
                }
                else
                {
                    No1BitNotZero = TRUE;
                    if (!lyricSecReadOkFlag)
                    {
                        if (FALSE != SecondPointFlag)
                        {
                            //record point section
                            *(pTimeLabelChar + 9) = tempData - '0';
                            lyricSecReadOkFlag = TRUE;
                        }
                        else
                        {
                            // recond second
                            if (!secondReadDone)
                            {
                                if (secondNum < SECOND_VALID) //������λ����λ
                                {
                                    *(pTimeLabelChar + 5) = *(pTimeLabelChar + 6);
                                    *(pTimeLabelChar + 6) = *(pTimeLabelChar + 7);
                                    *(pTimeLabelChar + 7) = *(pTimeLabelChar + 8);
                                    *(pTimeLabelChar + 8) = tempData - '0';
                                }
                                else
                                {
                                    *(pTimeLabelChar + 5) = 6;
                                    //Ϊ�˽�ʡ����ռ�, cailizhen 2010-11-15 11:13
                                    *(WORD*)(pTimeLabelChar + 6) = 0;
                                    //*(pTimeLabelChar + 6) = 0;
                                    //*(pTimeLabelChar + 7) = 0;
                                    *(WORD*)(pTimeLabelChar + 8) = 0;
                                    //*(pTimeLabelChar + 8) = 0;
                                    //*(pTimeLabelChar + 9) = 0;
                                    secondReadDone = TRUE;
                                }
                                secondNum++;
                            }
                        }
                    }

                }
            }

        }

        result = ReadLRCFileDataToBuffer();
        if (READ_FILE_NORMAL == result)
        {
            pDataBufferOffest = DataBuffer;
        }
        else //Ӧ�����ж��Ƿ��������
        {
            if (READ_FILE_ERROR == result)
            {
                return READ_DATA_FILE_ERROR;
            }
            else//end
            {
                return READ_DATA_FILE_END;
            }
        }

    }

    return 0;//cancel the warmning

}

// 0 means read '\0'; 1 means 'dataBuffer over';2 means found need;

returnReadData_t MinuteDataRead(void)
{
    uchar tempData;
    uchar minuteNum = 0;
    returnReadFile_t result;
    bool minuteReadDone = FALSE;
    pTimeLabelChar = (uchar *) pTimeLabel;
    memset(pTimeLabelChar, 0, sizeof(LrcTime_t) * 2);

    No1BitNotZero = FALSE;

    while (1)
    {
        //ClearWatchDog();
        while (!IfDataBufferEnd())
        {
            //ClearWatchDog();
            tempData = (*pDataBufferOffest++);
            if (tempData == 0)
            {
                return READ_DATA_ZERO;
            }
            else if (tempData == ':')
            {
                return READ_DATA_DONE;
            }
            else if ((tempData > '9') || (tempData < '0'))
            {
                return READ_DATA_NOT_USE_DATA;
            }
            else
            {
                if ((tempData == '0') && (No1BitNotZero == FALSE))//[00001:]----->[1:]
                {
                    continue;
                }
                else
                {
                    No1BitNotZero = TRUE;
                    if (!minuteReadDone)
                    {
                        if (minuteNum < MINUTE_VALID) //������λ����λ
                        {
                            *pTimeLabelChar = *(pTimeLabelChar + 1);
                            *(pTimeLabelChar + 1) = tempData - '0';
                        }
                        else
                        {
                            *pTimeLabelChar = 9;
                            *(pTimeLabelChar + 1) = 9;
                            minuteReadDone = TRUE;
                        }
                        minuteNum++;
                    }

                }
            }

        }

        result = ReadLRCFileDataToBuffer();
        if (READ_FILE_NORMAL == result)
        {
            pDataBufferOffest = DataBuffer;
        }
        else //Ӧ�����ж��Ƿ��������
        {
            if (READ_FILE_ERROR == result)
            {
                return READ_DATA_FILE_ERROR;
            }
            else//end
            {
                return READ_DATA_FILE_END;
            }
        }

    }
    return 0;//cancel the warmning

}

returnReadData_t TimeStartDataRead(void)
{
    uchar tempData;
    returnReadFile_t result;

    while (1)
    {
        //ClearWatchDog();
        while (!IfDataBufferEnd())
        {
            //ClearWatchDog();
            tempData = (*pDataBufferOffest++);
            if (tempData == 0)
            {
                return READ_DATA_ZERO;
            }
            else
            {
                if (tempData != '[')
                {
                    continue;
                }
                else
                {
                    return READ_DATA_DONE;
                }
            }

        }

        result = ReadLRCFileDataToBuffer();
        if (READ_FILE_NORMAL == result)
        {
            pDataBufferOffest = DataBuffer;
        }
        else //Ӧ�����ж��Ƿ��������
        {
            if (READ_FILE_ERROR == result)
            {
                return READ_DATA_FILE_ERROR;
            }
            else//end
            {
                return READ_DATA_FILE_END;
            }
        }

    }
    return 0;//cancel the warmning


}

void DealTimeData(void)
{
    WORD second = 0;
    uchar minute = 0;
    uchar temp;
    uchar i;

    //ClearWatchDog();
    for (i = 0; i < MINUTE_VALID; i++)
    {
        minute *= 10;
        minute += *(pTimeLabelChar + i);
    }
    for (i = 0; i < SECOND_VALID; i++)
    {
        second *= 10;
        second += *(pTimeLabelChar + 5 + i);
    }
    temp = (uchar)(second / 60);

    if ((temp + minute) > MAX_MINUTE)
    {
        pTimeLabel->min = MAX_MINUTE;
        pTimeLabel->sec = 59;
        pTimeLabel->p1s = 9;
    }
    else
    {
        minute += temp;
        second %= 60;
        pTimeLabel->min = minute;
        pTimeLabel->sec = (uchar) second;
        pTimeLabel->p1s = *(pTimeLabelChar + 9);
    }
    //pTimeLabel->buffaddr_High = (uchar)((pLyricStrBufferOffest & 0xff00)>>8);
    //pTimeLabel->buffaddr_Low= (uchar)(pLyricStrBufferOffest & 0x00ff);
    *(WORD *) (&(pTimeLabel->buffaddr_High)) = (WORD) pLyricStrBufferOffest;
    pTimeLabel++;

}

returnReadString_t ReadStringData(void)
{
    uchar tempData;
    returnReadString_t result_dealString;
    returnReadFile_t result_readFile;
    No1DataNotStaConFlag = 0;

    while (1)
    {
        //ClearWatchDog();
        while (!IfDataBufferEnd())//data buffer over
        {
            //ClearWatchDog();
            tempData = (*pDataBufferOffest++);
            if (FALSE != HalfChineseTempSavRam)
            {
                result_dealString = SaveStringData(tempData);
                if (result_dealString != READ_STRING_DONE)
                {
                    return result_dealString;
                }
            }
            else
            {
                if (tempData == 0)
                {
                    *pLyricStrBufferOffest = 0x0d;
                    return READ_STRING_ZERO;
                }
                else if ((tempData == 0x0d) || (tempData == 0x0a))
                {
                    //��ʾ���һ�н����ˣ�Ӧ������[
                    *(pLyricStrBufferOffest++) = NEXT_SCREEN;
                    LyricLineCounter = 0;
                    PointColCounter = 0;
                    DivScreenPageCNT = 0;
                    SameStringCNT = 0;
                    return READ_STRING_DONE;
                }
                else if (tempData != '[')
                {
                    result_dealString = SaveStringData(tempData);
                    if (result_dealString != READ_STRING_DONE)
                    {
                        return result_dealString;
                    }
                }
                else//[
                {
                    if (FALSE != No1DataNotStaConFlag)
                    {
                        // sdfjsdlf []
                        *(pLyricStrBufferOffest++) = NEXT_SCREEN;
                        DivScreenPageCNT = 0;
                        SameStringCNT = 0;
                    }
                    else //[][]uujkkkkl
                    {
                        SameStringCNT++;
                    }
                    LyricLineCounter = 0;
                    PointColCounter = 0;
                    return READ_STRING_TIME_START;
                }

            }

        }

        result_readFile = ReadLRCFileDataToBuffer();
        if (READ_FILE_NORMAL == result_readFile)
        {
            pDataBufferOffest = DataBuffer;
        }
        else //Ӧ�����ж��Ƿ��������
        {
            if (READ_FILE_ERROR == result_readFile)
            {
                return READ_STRING_FILE_ERROR;
            }
            else//end
            {
                *pLyricStrBufferOffest = 0x0d;
                return READ_STRING_FILE_END;
            }
        }

    }
    return 0;//cancel the warmning
}

returnReadString_t SaveStringData(uchar tempData)
{
    returnDealString_t result_dealString = DealSaveString(tempData);
    if (result_dealString == READ_BUFFER_AGAIN)
    {
        return READ_STRING_DONE;
    }
    else if (result_dealString == READ_FILE_AGAIN)
    {
        return READ_STRING_REREAD_FILE;
    }
    else//lrcbuffer over flow
    {
        //empty
        *pLyricStrBufferOffest = 0x0d;
        return READ_STRING_LRC_OVERFLOW;
    }
}

