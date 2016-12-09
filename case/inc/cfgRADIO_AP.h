#ifndef RADIO_AP_H
#define RADIO_AP_H

//����ͼ
#define PIC_ID_B_FMUSBCK FMUSBCK
#define PIC_X_B_FMUSBCK	0
#define PIC_Y_B_FMUSBCK	0

//����ͼ��
#define PIC_ID_FMVOL  MVOL
#define PIC_X_FMVOL	74
#define PIC_Y_FMVOL	143

//̨�ű���
#define PIC_ID_FMCHBG FMCHBG
#define PIC_X_FMCHBG	32               //95
#define PIC_Y_FMCHBG	112

//̨�ű���
#define PIC_ID_FMCH FMCH
#define PIC_X_FMCH	49                      //95
#define PIC_Y_FMCH	8

#define PIC_ID_CHNUM0 CHNUM0
#define PIC_X_CHNUM0	36               //114
#define PIC_Y_CHNUM0	32

#define PIC_ID_CHNUM1 CHNUM0
#define PIC_X_CHNUM1	65                //120
#define PIC_Y_CHNUM1	112

//Ƶ�ʱ���
#define PIC_ID_FMNUMBG   FMNUMBG
#define PIC_X_FMNUMBG	0                 //90
#define PIC_Y_FMNUMBG	80               //100

//Ƶ�ʵ�һλ����
#define PIC_ID_F1RATE FMNUM0
#define PIC_X_F1RATE	10               //90
#define PIC_Y_F1RATE	16               //100

//Ƶ�ʵڶ�λ����
#define PIC_ID_F2RATE FMNUM0
#define PIC_X_F2RATE	PIC_X_F1RATE+16               //115
#define PIC_Y_F2RATE	74                //100

//Ƶ�ʵ���λ����
#define PIC_ID_F3RATE FMNUM0
#define PIC_X_F3RATE	PIC_X_F2RATE+23               //140
#define PIC_Y_F3RATE	80                //100

//�й����ձ�Ƶ����
#define PIC_ID_FMBARBG    FMBARBG
#define PIC_X_FMBARBG	0
#define PIC_Y_FMBARBG	47                  //47

//������
#define PIC_ID_FMSTEREO  FMSTEREO
#define PIC_ID_FMSTEREOOF  STEREOOF
#define PIC_X_FMSTEREO	8                  
#define PIC_Y_FMSTEREO	40                

//������1λ
#define PIC_ID_VOL1  TVNUM0
#define PIC_X_VOL1	21
#define PIC_Y_VOL1	146

//������2λ
#define PIC_ID_VOL2  TVNUM0
#define PIC_X_VOL2	30
#define PIC_Y_VOL2	146

//��ص���
#define PIC_ID_BATTERY    BATTERY1
#define PIC_X_BATTERY	104              //193
#define PIC_Y_BATTERY	146              //162

#define PIC_ID_CH_SLIDPTR    FMPTR
#define PIC_X_CH_SLIDPTR	4          //25
#define PIC_Y_CH_SLIDPTR	47        //53

//����ͼ��
#define PIC_ID_EARLOGO  EARLOGO
#define PIC_X_EARLOGO	3
#define PIC_Y_EARLOGO	145             //162

#define PIC_ID_JP_SLIDPTR    FMPTR
#define PIC_X_JP_SLIDPTR	17         //40
#define PIC_Y_JP_SLIDPTR	47        //53

#define PIC_ID_FMTITLE	 T_FM
#define PIC_X_FMTITLE     0
#define PIC_Y_FMTITLE     128


//����ͼ /*����ͼ��ID ��X�� Y����
#define PIC_ID_ERROR_BG CARDLOCK
#define PIC_X_ERROR_BG	7
#define PIC_Y_ERROR_BG	47


#define STR_ID_ERROR_STR CREADERR
#define STR_X_ERROR_STR	255
#define STR_Y_ERROR_STR	54

//����ͼ /*����ͼ��ID ��X�� Y����
#define PIC_ID_DLALL_BG MENUTWO
#define PIC_X_DLALL_BG	0
#define PIC_Y_DLALL_BG	0

//����ͼ /*����ͼ��ID ��X�� Y����
#define PIC_ID_DLALL_YN DELNO
#define PIC_X_DLALL_YN	70
#define PIC_Y_DLALL_YN	36

//����ͼ
#define PIC_ID_UDLOCK_S UDLOCK_S
#define PIC_X_UDLOCK_S	86
#define PIC_Y_UDLOCK_S	80

#define PIC_ID_B_SSBCK SSBCK
#define PIC_X_B_SSBCK	0
#define PIC_Y_B_SSBCK	0

#define PIC_ID_F_SSLIDER SSLIDER1
#define PIC_X_F_SSLIDER	10             //45
#define PIC_Y_F_SSLIDER	55

#define PIC_ID_F_SLIDPTR SLIDPTR
#define PIC_X_F_SLIDPTR	10             //45
#define PIC_Y_F_SLIDPTR	55


#define STR_ID_T_SLIDE SMAIN12
#define STR_X_T_SLIDE	255
#define STR_Y_T_SLIDE	131

#define PIC_ID_F_POSL   FMNUM0
#define PIC_X_F_POSL	42               //83
#define PIC_Y_F_POSL	80

#define PIC_ID_F_POSR   FMNUM0
#define PIC_X_F_POSR	65              //106
#define PIC_Y_F_POSR	80

#define PIC_ID_F2_MENUNEG   MENUNEG
#define PIC_X_F2_MENUNEG	44              //90
#define PIC_Y_F2_MENUNEG	83

#define PIC_ID_F2_POSR   FMNUM0
#define PIC_X_F2_POSR	62                 //108
#define PIC_Y_F2_POSR	80

#define PIC_ID_F1_POSM   FMNUM0
#define PIC_X_F1_POSM	53                 //98
#define PIC_Y_F1_POSM	80

#define PIC_ID_F3_POSL   FMNUM0
#define PIC_X_F3_POSL	29                  //75
#define PIC_Y_F3_POSL	80

#define PIC_ID_F3_POSM   FMNUM0
#define PIC_X_F3_POSM	52                 //98
#define PIC_Y_F3_POSM	80

#define PIC_ID_F3_POSR   FMNUM0
#define PIC_X_F3_POSR	75                   //121
#define PIC_Y_F3_POSR	80


#define PIC_ID_B_MENUTWO  MENUTWO
#define PIC_X_B_MENUTWO	0
#define PIC_Y_B_MENUTWO	0


#define PIC_ID_F_DHT  DHT
#define PIC_X_F_DHT	112                     //194
#define PIC_Y_F_DHT	13                      //8

#define PIC_ID_BASE    MENURARR1
#define PIC_X_BASE0	10
#define PIC_Y_BASE0	99

#define PIC_X_BASE1	10
#define PIC_Y_BASE1	78

#define PIC_X_BASE2	10
#define PIC_Y_BASE2	57

#define PIC_X_BASE3	10
#define PIC_Y_BASE3	36

#define PIC_X_BASE4	10
#define PIC_Y_BASE4	15

#define PIC_X_BASE5	10
#define PIC_Y_BASE5	14

/*�˵���Ŀ���ַ�����ƫ�������� */
#define POPMENU_ITM_X_ITEM	20         //22
#define POPMENU_ITM_Y_ITEM	0

 /*��������*/
#define POPMENU_SCR_X_M_SCROLLZONE	29
#define POPMENU_SCR_Y_M_SCROLLZONE	12

#define POPMENU_SCR_W_M_SCROLLZONE	84
#define POPMENU_SCR_H_M_SCROLLZONE	16

/*���������ʾ,������λ���Լ���λ�þ�����ʾ*/
#define PIC_ID_F_MENUBCK  MENUBCK
#define PIC_X_F_MENUBCK	0
#define PIC_Y_F_MENUBCK	0
#define PIC_ID_F_MENURARR  MENURARR
#define PIC_X_F_MENURARR	0
#define PIC_Y_F_MENURARR	0
#define PIC_ID_F_DHTHK  DHTHK
#define PIC_X_F_DHTHK	102               //184
#define PIC_Y_F_DHTHK	6                 //6
#define  POPMENU_MENU_TITLE_X 255       //22 // �˵���������
#define  POPMENU_MENU_TITLE_Y 131      //148 //�˵���������

#define PIC_ID_V_MENUTWO  MENUTWO
#define PIC_X_V_MENUTWO	0
#define PIC_Y_V_MENUTWO	0


#define PIC_ID_V_DHT  DHT
#define PIC_X_V_DHT	110         //194
#define PIC_Y_V_DHT	13            //8

#define PIC_ID_FINFO    MDIR
#define PIC_X_FINFO0	10
#define PIC_Y_FINFO0	99

#define PIC_X_FINFO1	10
#define PIC_Y_FINFO1	78

#define PIC_X_FINFO2	10
#define PIC_Y_FINFO2	57

#define PIC_X_FINFO3	10
#define PIC_Y_FINFO3	36

#define PIC_X_FINFO4	9
#define PIC_Y_FINFO4	15

#define PIC_X_FINFO5	9
#define PIC_Y_FINFO5	10

/*�˵���Ŀ���ַ�����ƫ�������� */
#define VARMENU_ITM_X_ITEM	22
#define VARMENU_ITM_Y_ITEM	0

 /*��������*/
#define VARMENU_SCR_X_M_SCROLLZONE	30
#define VARMENU_SCR_Y_M_SCROLLZONE	10

#define VARMENU_SCR_W_M_SCROLLZONE	78          //162
#define VARMENU_SCR_H_M_SCROLLZONE	16

/*���������ʾ,������λ���Լ���λ�þ�����ʾ*/
#define PIC_ID_V_ACTIVEBAR2  MENUBCK
#define PIC_X_V_ACTIVEBAR2	0
#define PIC_Y_V_ACTIVEBAR2	0
#define PIC_ID_V_ACTIVEBARSC  DHTHK
#define PIC_X_V_ACTIVEBARSC	100          //184
#define PIC_Y_V_ACTIVEBARSC	8           //8
#define PIC_ID_V_MFILE MFILE
#define PIC_X_V_MFILE	2
#define PIC_Y_V_MFILE	3
#define PIC_ID_F_FILEBCK  FILEBCK
#define PIC_X_F_FILEBCK	18               //20
#define PIC_Y_F_FILEBCK	0                //0
#define  VARMENU_MENU_TITLE_X 255         //20 // �˵���������
#define  VARMENU_MENU_TITLE_Y 131         //149 //�˵���������

#endif //RADIO_AP_H
