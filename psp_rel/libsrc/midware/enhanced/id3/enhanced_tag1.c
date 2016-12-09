/*
 *******************************************************************************
 *                ACTOS AP
 *        enhanced module impletementation
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 * File   : enhanced_tag2.c
 * By     : ouyuping
 * Version: v1.00     first version     3/23/2009
 *******************************************************************************
 */

#include <actos.h>
#include <enhanced.h>
#include <stringH.h>
//#include "debug.h"

#pragma name(MW_EH_ID31)

#pragma renamecode(MW_EH_ID31)

#define ID3_COUNT 7 //��Ҫ���ҵ�FrameID��(�������⣬���ߣ�ר��, ABLUM_ART)
/*
 *********************************************************
 1) "RTENC"         :������      2) "TRCK"        :����
 3) "TPE1"          :�ݳ���      4) "TIT2"        :����
 5) "TALB"          :ר��        6) "TYER"        :���
 7) "TMED"          :ý������    8) "TCOM"        :������
 9) "COMM"          :ע��       10) "CON"         :����
 11) "TOPE"          :ԭ����     12) "TCOP"        :��Ȩ
 13) "WXXX"          :URL
 *********************************************************
 */

typedef struct
{
    char Header[3];/*����Ϊ"ID3"������Ϊ��ǩ������*/
    BYTE Ver;/*�汾��;ID3V2.3�ͼ�¼03,ID3V2.4�ͼ�¼04*/
    BYTE Revision;/*���汾��;�˰汾��¼Ϊ00*/
    char Flag;/*��ű�־���ֽڣ�����汾ֻ��������λ*/
    char Size[4];/*��ǩ��С��������ǩͷ��10���ֽں����еı�ǩ֡�Ĵ�С*/
} ID3_10_header;//ID3V2.*��ǩͷ(ǰ10byte)

typedef struct
{
    BYTE bytes_of_header;//��ͬID3�汾Frame Header��ռ�ֽ���
    BYTE bytes_of_title;//��ͬID3�汾Frame Header��FrameID��ռ�ֽ���
    BYTE bytes_of_framesize;//��ͬID3�汾Frame Header��֡����ռ�ֽ���
    char *id3table;//��ͬID3�汾��Ӧ����Ҫ���ҵ�FrameID Table
    BYTE id3tablesize;//��ӦFrameID Table�ĳߴ�
} ID3_Version_Info;//

typedef struct
{
    char FrameID[4]; /*���ĸ��ַ���ʶһ��֡��˵�������ݣ��Ժ��г��õı�ʶ���ձ�*/
    char Size[4]; /*֡���ݵĴ�С��������֡ͷ������С��1*/
    char Flags[2];/*��ű�־��ֻ������6λ*/
//char encodetype;//frame  �������
} ID3_2_3_HEADER;

typedef struct
{
    char FrameID[3]; /*���ĸ��ַ���ʶһ��֡��˵��������*/
    char Size[3]; /*֡���ݵĴ�С��������֡ͷ������С��1*/
//    char encodetype; //frame  �������
} ID3_2_2_HEADER;

typedef struct
{
    uchar savelen; // FrameID����������
    char* saveaddr; // FrameID�����ַ
} ID3SaveItem;

extern char *TempBuf;
const char ID3TitleTable_v2_3[] = "TIT2TPE1TALBTENCTCONTRCKAPIC";// ID3V2.3��Ҫ���ҵ�FrameID Table
const char ID3TitleTable_v2_2[] = "TT2TP1TAL"; // ID3V2.2��Ҫ���ҵ�FrameID Table

const ID3_Version_Info ID3ver[2] =
{
    { 
        sizeof(ID3_2_3_HEADER), 
        4,//size of ID3_2_3_HEADER's FrameID
        4,//size of ID3_2_3_HEADER's Size
        (char *) ID3TitleTable_v2_3,
        sizeof(ID3TitleTable_v2_3) 
    },
    { 
        sizeof(ID3_2_2_HEADER), 
        3,//size of ID3_2_2_HEADER's FrameID
        3,//size of ID3_2_2_HEADER's Size
        (char *) ID3TitleTable_v2_2, 
        sizeof(ID3TitleTable_v2_2)
    } 
};
ID3SaveItem *pId3Save;//FrameID�洢λ��
ID3Info_t *pID3InfoStart;//ap���ṩ�Ĵ洢�ṹ
ID3_10_header far id3h;
int far bufindex;//��ǰTempBufδ�����ַ�������
int far findcount;//�ɹ����ҵ���FrameID��
dword far filepos;//��ǰ�ļ��ľ�ȷָ��
static dword far framepos;//��ǰFrame�ĵ�ַ
static dword far framesize;//��ǰFrame��size(��������������λ)
HANDLE far TagHandle;
BYTE far AvailFlag[ID3_COUNT];//��ЧFrameID���ҵǼǱ�
BYTE far curversion;//��ǰID3�汾
char *TempBuf; //read file buffer
uint8 far albumHeadBuf[80];
//ID3Info_ext_t *ID3Info_ext_global;

/********************************************************************************
 * Description : ����Ƿ�����Ч�Ĵ�����frameID(title)
 *
 * Arguments  :
 *             pid3header : Frame Header ��ͷ�ĸ��ֽ���frameId��������Ƿ� ������Ҫ�ҵ�id
 Ҫ��ID��  ID3TitleTable_v2_3[] = "TIT2TPE1TALB"��ÿ���ĸ��ֽ�bytes_of_title
 AvailFlag[0]��Ӧ��"TIT2"
 * Returns     :
 *            TRUE: ��Ч,
 *            FALSE: ��Ч.
 * Notes       :
 *
 ********************************************************************************/

uint8 IsAvailFrameID(char *pid3header)
{
    uint8 i = 0, j = 0, index;

    ClearWatchDog();
    while ((i < ID3ver[curversion].id3tablesize) && (j < ID3ver[curversion].bytes_of_title))
    {
        if (pid3header[j] == ID3ver[curversion].id3table[i])
        {
            j++;
        }
        else
        {
            j = 0;
        }
        i++;
    }

    //û�ҵ�������Ҫ��ID
    if ((j < ID3ver[curversion].bytes_of_title) || ((i % ID3ver[curversion].bytes_of_title) != 0))
    {
        return 0;
    }

    index = (i / ID3ver[curversion].bytes_of_title) - 1;
    if (AvailFlag[index] == 1)//֮ǰ�Ѿ����ˣ�������һ��������¼
    {
        return 0;
    }

    AvailFlag[index] = 1;//��¼�ҵ��ı�ǩ

    if (index != (ID3_COUNT - 1))
    {
        pId3Save = (ID3SaveItem*) pID3InfoStart + index;//���ý�Ҫ����ĵ�ַ
    }    

    return (index + 1);
}

/********************************************************************************
 * Description : ����Frame Header������Frame size
 *
 * Arguments  :
 *             pid3header : Frame Header
 * Returns     :
 *            TRUE: �ɹ�,
 *            FALSE: ʧ��.
 * Notes       :
 *
 ********************************************************************************/

BOOL GetFrameSize(char *pid3header)
{
    int i = 0;
    uint8 *pfrmsaddr = (uint8 *)(pid3header + ID3ver[curversion].bytes_of_title);
    dword size = 0;

    if(id3h.Ver == 4)	//ID3v2.4
    {
        while(i < 4)
		{
		    size <<= 7;
			size |= (dword)(pfrmsaddr[i]&0x7f);
			i++;
		}
    }
    else
    {
        while (i < ID3ver[curversion].bytes_of_framesize)
        {
            size <<= 8;
            size += (dword) pfrmsaddr[i];
            i++;
        }
    }
    framesize = size;
    return TRUE;
}

/********************************************************************************
 * Description : ��ȷ�ļ���ȡ��ʼ��
 *
 * Arguments  :
 *
 * Returns     :
 *            TRUE: �ɹ�,
 *            FALSE: ʧ��.
 * Notes       :
 *
 ********************************************************************************/

bool FI_FInit(void)
{
    bufindex = 0;
    filepos = 0;

    FS_FRead(TempBuf, 1, TagHandle); //��һPAGE����
    return TRUE;
}

/********************************************************************************
 * Description : ��ȷ�ļ���ȡSEEK
 *
 * Arguments  :
 *             fpos:���ļ�ͷseek���ֽ�ƫ����
 * Returns     :
 *            TRUE: �ɹ�,
 *            FALSE: ʧ��.
 * Notes       :
 *
 ********************************************************************************/

BOOL FI_FSeek(dword fpos)
{
    BOOL result;
    int32 fpagepos = (int32)(fpos >> 9);
    int32 curpagepos = (int32)(filepos >> 9);

    filepos = fpos;
    bufindex = (int16)(fpos & 0x1ff);

    if (fpagepos != curpagepos)
    {
        result = FS_FSeek(fpagepos, 0, TagHandle);
        if (result == 0)
        {
            return FALSE;
        }

        result = FS_FRead(TempBuf, 1, TagHandle);
        if (result == 0)
        {
            return FALSE;
        }
    }

    return TRUE;
}

/********************************************************************************
 * Description : ��ȷ�ļ���ȡREAD
 *
 * Arguments  :
 *             databuf:��ȡ�ļ����ݵ�buf
 *             length:��ȡ�ĳ��ȣ��ֽ�Ϊ��λ
 * Returns     :
 *            TRUE: �ɹ�,
 *            FALSE: ʧ��.
 * Notes       :
 *
 ********************************************************************************/

BOOL FI_FRead(char *databuf, int length)
{
    int i = 0;
    BOOL result;

    //һ��һ���ֽڸ��ƣ����Խ����Ż�
    ClearWatchDog();
    while ((bufindex < 512) && (i < length))
    {
        databuf[i] = TempBuf[bufindex];
        i++;
        bufindex++;
        if (bufindex == 512)
        {
//            result = FS_FSeek(1, 1, TagHandle); //��ǰλ���Ⱥ�ƫ��һҳ 512byte
//            if (result == 0)
//            {
//                return FALSE;
//            }

            result = FS_FRead(TempBuf, 1, TagHandle);
            if (result == 0)
            {
                return FALSE;
            }

            bufindex = 0;
        }
    }

    filepos += (uint32)length;
    return TRUE;
}

/********************************************************************************
 * Description : ���浱ǰFrame
 *
 * Arguments  :
 *             len: Frame����Ч����  pId3Save->savelen��30��֮ǰ�Ѿ������˵�
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************/

void SaveTagInfo(dword len)
{
    dword readlen = len;
    if ((readlen == 0) || (pId3Save->savelen == 0)) //may be is 0
    {
        pId3Save->saveaddr[0] = '\0';
        return;
    }
    FI_FRead(pId3Save->saveaddr, 1);
    if (((pId3Save->saveaddr[0]) == 0) || ((pId3Save->saveaddr[0]) == 1))
    {
        if ((readlen - 1) > (pId3Save->savelen - 2))
        {
            readlen = pId3Save->savelen - 2;
        }
        else
        {
            readlen -= 1;//framesize include the encodetype 	
        }
        FI_FRead(pId3Save->saveaddr, (int16)readlen);
        //pId3Save->saveaddr[readlen] = '\0';
        //pId3Save->saveaddr[readlen + 1] = '\0';
    }
    else   //no encode type
    {
        if ((readlen) > (pId3Save->savelen - 2))
        {
            readlen = pId3Save->savelen - 2;
        }
        FI_FRead(pId3Save->saveaddr + 1, ((int16)(readlen-1)));
    }
    pId3Save->saveaddr[readlen] = '\0';
    pId3Save->saveaddr[readlen + 1] = '\0';
}


/********************************************************************************
 * Description : ���浱ǰFrame
 *
 * Arguments  :
 *             len: Frame����Ч����  pId3Save->savelen��30��֮ǰ�Ѿ������˵�
 * Returns     :
 *
 * Notes       :
 *
 ********************************************************************************/

int8 checkAlbumArtInfo(void)
{
    int8 a;
    FI_FRead(albumHeadBuf, 80);      
    for(a=0 ;a<79; a++)
    {
        if(albumHeadBuf[a]==0xFF&&albumHeadBuf[a+1]==0xD8)
        {
            return a;
        }
    }
    return -1;
}

/********************************************************************************
 * Description : Frame����
 *
 * Arguments  :
 *
 * Returns     :
 *            Frame������
 * Notes       :
 *
 ********************************************************************************/

dword ParseFrame(void)
{
    char id3header[12];
    uint8 index = 0;
    int8 albumOffest = -1;


    //��ȡһ��ͷ��id3header
    FI_FRead(id3header, ID3ver[curversion].bytes_of_header);
    if ((id3header[0] == 0) && (id3header[1] == 0) && (id3header[2] == 0))
    {
        return 0;
    }

    //����һ��ͷ����ø���Ϣ֡�ĳ���
    GetFrameSize(id3header);

    index = IsAvailFrameID(id3header);
    if (0 != index)
    {
        if (ID3_COUNT != index)
        {
            SaveTagInfo(framesize);	 //framesize include the encode type
        }
        else
        {
            albumOffest = checkAlbumArtInfo();
            if (albumOffest != -1)
            {
                pID3InfoStart->APIC_offset = framepos + ID3ver[curversion].bytes_of_header + albumOffest;
                pID3InfoStart->APIC_type = 1;
				            pID3InfoStart->APIC_flag = 1;
            }
            else
            {
                pID3InfoStart->APIC_type = 0;
                pID3InfoStart->APIC_flag = 1;
            }
        }
        findcount++;
    }
    return framesize + ID3ver[curversion].bytes_of_header; //֡����֡ͷ��
}

/********************************************************************************
 * Description : ��ȡV2�汾ID3 Tag
 *
 * Arguments  :
 *             TagFileHandle:�������ļ����
 *             pID3Info:ID3 Tag����ṹ��ָ��
 * Returns     :
 *            TRUE: �ɹ�,
 *            FALSE: ʧ��.
 * Notes       :
 *
 //am7212p

 ********************************************************************************/

BOOL FetchID3_V2info(handle TagFileHandle, ID3Info_t *pID3Info)
{
    dword ID3Size;
    dword whole_framesize;
//	uint8 *pStr;
//	uint16 trackNum = 0;

    TagHandle = TagFileHandle;
    pID3InfoStart = pID3Info;//��Ϣ���أ�pID3Info�Ǵ������Ĳ�������pID3InfoStart��ͬ��������TAGģ������ȫ�ַ��ʵ�
    pID3InfoStart->APIC_flag = 0;

    findcount = 0;
    memset(AvailFlag, 0, sizeof(AvailFlag));
    curversion = 0;//Ĭ��ID3v2.3�汾

    ClearWatchDog();

    FI_FInit();//��ȡ512byte�� TempBuf
    FI_FRead((char*) &id3h, sizeof(id3h));

    if ((id3h.Header[0] != 'I') || (id3h.Header[1] != 'D') || (id3h.Header[2] != '3'))
    {
        return FALSE;
    }

    if (id3h.Ver < 3)
    {
        curversion = 1;//ID3v2.2�汾
    }

    //ID3Size������ID3V2�ĳ���
    ID3Size = ((dword) (id3h.Size[0] & 0x7F) << 21) + ((dword) (id3h.Size[1] & 0x7F) << 14) + ((dword) (id3h.Size[2]
            & 0x7F) << 7) + (dword) (id3h.Size[3] & 0x7F) + sizeof(id3h);

    framepos = sizeof(id3h);

    while ((framepos < ID3Size) && (findcount < ID3_COUNT))
    {
        ClearWatchDog();

        //whole_framesizeһ֡�ĳ���
        whole_framesize = ParseFrame();
        if (whole_framesize == 0) //deal with the padding byte
        {
            break;
        }
        framepos += whole_framesize;

        //SaveTagInfo ��ʹ�õ�len(�����30)��һ����frameSize �����Ҫ���¶���
        if (!FI_FSeek(framepos))
        {
            break;
        }
    }
	/*
	if (pID3Info->TrackNum_buffer[0] != 0)
	{
		pStr = pID3Info->TrackNum_buffer;
		while((*pStr != '/') && (*pStr != 0))
		{
			trackNum *= 10;
			trackNum += (*pStr - 0x30);
			pStr++;
		}
		pID3Info->TrackNum = trackNum;	
	}
	*/
    if (findcount > 0)
    {
        return TRUE;
    }
    return FALSE;
}
