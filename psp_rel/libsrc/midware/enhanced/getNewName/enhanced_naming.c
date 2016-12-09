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
#pragma name(MW_EH_NAME)
#pragma renamecode(MW_EH_NAME)

#include <actos.h>
#include <enhanced.h>
#include <stringH.h>
//#include <debug.h>


/*
 ********************************************************************************
 * Description : ����ת�����ַ���
 *
 * Arguments   : i, ����
 *               buf, ����ַ���ָ��
 *
 * Returns     : �ɹ�, ����ַ���ָ��
 *
 * Notes       : ����ֻ���� 01~99, û��0��β
 *
 ********************************************************************************
 */
char *my_itoa(int i, uint8 *buf)
{
    buf[0] = ((uint8)((i / 100) % 10)) + '0';
    buf[1] = ((uint8)((i / 10) % 10)) + '0';
    buf[2] = ((uint8)(i % 10)) + '0';
    return buf;
}
/*
 ********************************************************************************
 * Description : ������������µĿ����ļ���
 *
 * Arguments   : location->filename, ԭ�����ļ���
 *               location->filename, ����µ��ļ���
 *
 * Returns     :  ���ļ������, 01~99
 *        ���01~99 �ļ�����, ����0
 *
 * Notes       :�ļ����ָ�ʽ : xxx000 WAV, xxx000 ACT, ***000 JPG...
 *        ���������ļ�������,��ֱ�ӷ���
 ���Ŀ¼���ʱ��15s
 ˳�������һ��Ŀ¼���ʱ��1s.
 *
 * TODO:        ��Ҫʹ�� atoi(), ��ʡ128byte  (ok!)
 ********************************************************************************
 */
#ifndef PC
int sGenNewName(file_location_t *location)
#else
int GenNewName(file_location_t *location)
#endif
//int sGenNewName(file_location_t *location)
{
    int num;
    int loops = 0;
    uint8 new_name[12];
    handle f;

    //ASSERT(location->filename[3] >= '0' && location->filename[3] <= '9');
    //ASSERT(location->filename[4] >= '0' && location->filename[4] <= '9');
    //ASSERT(location->filename[5] >= '0' && location->filename[5] <= '9');

    //���Ƶ�new_name
    memcpy(new_name, location->filename, 12);

    // ����ļ����Ƿ���001����ļ�,��û��RETURN 1
    new_name[3] = '0';
    new_name[4] = '0';
    new_name[5] = '1';
    // ����ļ�������
    f = FS_FOpen(new_name, 0);
    if (f == NULL)
    {
        memcpy(location->filename, new_name, 11);
        location->filename[11] = 0;
        // ɾ���ļ�����Ϊ0���ļ�.
        //�ļ����Ѿ����ڣ����ļ�����Ϊ0ʱopenҲʧ�ܡ�
        FS_FRemove(location->filename);
        return 1;
    }
    else
    {
        FS_FClose(f);
    }

    // ��ָ����ֵ,��������.
    memcpy(new_name, location->filename, 12);
    num = (new_name[3] - '0') * 100 + (new_name[4] - '0') * 10 + (new_name[5] - '0');
    //    if(num <= 0) num = 1;
    //    if(num > 200) num = 1;
    //	my_itoa(num, &new_name[3]);
    if ((num <= 0) || (num > 200))
    {
        num = 1;
        my_itoa(num, &new_name[3]);
    }

    //����ļ�����,���ּ�1
    do
    {
        // ����ļ�������
        ClearWatchDog();
        f = FS_FOpen(new_name, 0);
        if (f == NULL)
        {
            memcpy(location->filename, new_name, 11);
            location->filename[11] = 0;
            FS_FRemove(location->filename);
            return num;
        }
        else
        {
            FS_FClose(f);
            num++;
            if (num > 200)
            {
                num = 1;
            }
            my_itoa(num, &new_name[3]);
        }
        loops++;
    } while (loops < 200);

    return 0;
}
