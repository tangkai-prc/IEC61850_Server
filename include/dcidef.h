#ifndef __DCIDEF_H_
#define __DCIDEF_H_

/****************************************************************
 *                        DCI ���� 
 *             Copyright (C) 2010, ����Ƽ��ɷ����޹�˾
 *                        ALL RIGHTS RESERVED
 * �汾 : 1.00
 * ���� : �����
 * ��������: 2010.03.29
 * �ļ��� : dcidef.h
 * ���� : �豸ͨ�Žӿ��������ͺͳ�������
 *
 *
 * �޸ļ�¼1
 * �汾 : 
 * �޸����� :
 * �޸��� : 
 * �޸����ݼ�ԭ�� :
 *
 *
 * �޸ļ�¼2
 * �汾 : 
 * �޸����� : 
 * �޸��� : 
 * �޸����ݼ�ԭ�� :
 * 
***************************************************************/

#include <sys/types.h>

/******************     ����ԭ��        ******************/
#define DCI_ENONE           0       /*  �޴���  */
#define DCI_EFAIL           -1      /*  ����ʧ�ܣ�ͨ��ָϵͳ���û��ⲿ��������ʧ��  */
#define DCI_EINVAL          -2      /*  ����Ĳ��� */
#define DCI_EPERM           -3      /*  û�в���Ȩ��  */
#define DCI_EACCES          -4      /*  ��ֹ���ʣ����ɶ���д  */
#define DCI_ENOENT          -5      /*  ���ݶ��󲻴���  */
#define DCI_ETIMEOUT        -6      /*  ��ʱ����  */
#define DCI_ENOMEM          -7      /*  �ڴ����ʧ�ܣ�malloc��new����  */
#define DCI_EBUSY           -8      /*  æ  */
#define DCI_EEXIST          -9      /*  �����Ѵ���  */
#define DCI_EAGAIN          -10     /*  ��ʱ������  */
#define DCI_ENOCONN         -11     /*  δ����  */
#define DCI_ENOINIT         -12     /*  δ��ʼ��  */
#define DCI_EDATA           -13     /*  ���ݲ���ȷ  */
#define DCI_ENOBUF          -14     /*  �����������޷�д������  */
#define DCI_WAITRSP              -100



/******************     ������������        ******************/
#define  DCI_NULL           -1
#define  DCI_BOOL           0   /*����ֵ*/
#define  DCI_INT8           1   /*-128 ~ 127*/
#define  DCI_INT16          2   /*-32,768 ~ 32,767*/
#define  DCI_INT32          4   /*-2,147,483,648 ~ 2,147,483,647*/
#define  DCI_INT64          50  /*-2**63 ~ (2**63)-1  ����������Ҫ*/
#define  DCI_INT8U          6   /*�޷������� 0 ~ 255*/
#define  DCI_INT16U         7   /*�޷�������0 ~ 65,535*/
#define  DCI_INT32U         9   /*�޷�������0 ~ 4,294,967,295*/
#define  DCI_INT64U         51  /*�޷�������0 ~ (2**64)-1*/
#define  DCI_FLOAT32        10  /*ֵ��;���Ϊ IEEE754�����ȸ�����  */
#define  DCI_FLOAT64        11  /*ֵ��;���Ϊ IEEE754˫���ȸ�����  */
#define  DCI_TIME           52  /*ʱ��*/

#define  DCI_DPI            101 /*˫�㣬0x01���֣�0x02����*/

/******************     �䳤��������        ******************/
#define  DCI_OCTET_STRING   14
#define  DCI_STRING         15
#define  DCI_UNICODE_STRING 16
#define  DCI_BIT_STRING     17  /*λ��*/



/*******************    ����ԭ��    ***************************/
#define DCI_REASON_DCHG             1               /*����ֵ�仯����*/
#define DCI_REASON_QCHG             2               /*����Ʒ�ʱ仯����*/

#define DCI_REASON_REQ              0x100           /*����*/
#define DCI_REASON_RESP             0x101           /*��Ӧ*/

#define DCI_REASON_INIT             0x102           /*��ʼ��*/
#define DCI_REASON_SWITCH_SG        0x103           /*�л���ֵ��*/

/*******************    ��������    ***************************/
#define DCI_SELECT              1               /*ѡ��*/
#define DCI_OPER                2               /*ִ��*/
#define DCI_CANCEL              3               /*ȡ��*/
#define DCI_DIRECT_OPER         4               /*ֱ��ִ��*/
#define DCI_RESELECT            5               /*����ѡ��*/


/*******************    ��������    ***************************/
#define DCI_OPER_BOOL               1           /* value.i, DCI_OPER_OFF, DCI_OPER_ON*/
#define DCI_OPER_INT                2           /* value.i */
#define DCI_OPER_FLOAT              3           /* value.f */
#define DCI_OPER_STEP               4           /* value.i, DCI_OPER_STOP, DCI_OPER_LOWER, DCI_OPER_HIGHER*/

#define DCI_OPER_OFF                0           /*��բ*/
#define DCI_OPER_ON                 1           /*��բ*/
#define DCI_OPER_STOP               0           /*��ͣ*/
#define DCI_OPER_LOWER              1           /*�½�*/
#define DCI_OPER_HIGHER             2           /*����*/

#define DCI_PHASE_A         1
#define DCI_PHASE_B         2
#define DCI_PHASE_C         4
#define DCI_PHASE_N         8

#define DCI_DIR_UNKNOWN         0
#define DCI_DIR_FORWORD         1
#define DCI_DIR_BACKWORD        2
#define DCI_DIR_BOTH            3

/********************   ���Ʋ�������ԭ��    *********************/
#define OPERATE_SUCC                                0x30
#define OPERATE_UNKNOWN                             0
#define OPERATE_NOT_SUPPORTED                       1
#define OPERATE_BLOCKED_BY_SWITCHING_HIERARCHY      2
#define OPERATE_SELECT_FAILED                       3
#define OPERATE_INVALID_POSITION                    4
#define OPERATE_POSITION_REACHED                    5
#define OPERATE_PARAMETER_CHANGE_IN_EXECUTION       6
#define OPERATE_STEP_LIMIT                          7
#define OPERATE_BLOCKED_BY_MODE                     8
#define OPERATE_BLOCKED_BY_PROCESS                  9
#define OPERATE_BLOCKED_BY_INTERLOCKING             10
#define OPERATE_BLOCKED_BY_SYNCHROCHECK             11
#define OPERATE_COMMAND_ALREADY_IN_EXECUTION        12
#define OPERATE_BLOCKED_BY_HEALTH                   13
#define OPERATE_1_OF_N_CONTROL                      14
#define OPERATE_ABORTION_BY_CANCEL                  15
#define OPERATE_TIME_LIMIT_OVER                     16
#define OPERATE_ABORTION_BY_TRIP                    17
#define OPERATE_OBJECT_NOT_SELECTED                 18

/********************   Ʒ��ֵ  *********************/
#define DCI_Q_good          0
#define DCI_Q_invalid       1
#define DCI_Q_questionable  3

#define DCI_Q_process       0
#define DCI_Q_substituted   1

/********************   Ʒ��λ  *********************/
#define DCI_Q_VALIDITY          0
#define DCI_Q_OVERFLOW          2
#define DCI_Q_OUTOFRANGE        3
#define DCI_Q_BADREFERENCE      4
#define DCI_Q_OSCILLATORY       5
#define DCI_Q_FAILURE           6
#define DCI_Q_OLDDATA           7
#define DCI_Q_INCONSISTENT      8
#define DCI_Q_INACCURATE        9
#define DCI_Q_SOURCE            10
#define DCI_Q_TEST              11
#define DCI_Q_OPERATORBLOCKED   12


/********************   ���    *********************/
#define DCI_PHASE_NULL          0
#define DCI_PHASE_A             1
#define DCI_PHASE_B             2
#define DCI_PHASE_C             4
#define DCI_PHASE_NEUT          8

#define DCI_OFF                 0
#define DCI_ON                  1
#define DCI_FALSE               0
#define DCI_TRUE                1
#define DCI_DPI_OFF             1
#define DCI_DPI_ON              2


#define DCI_INVALID_SID         -1
#define DCI_INVALID_WID         -1

#ifndef NULL
#define NULL    0
#endif


typedef int         DCI_SID;    /* �豸�ź�ID */
typedef int         DCI_WID;    /* WatchDog ID */


#if 0
struct timeval {
    long tv_sec;        /* seconds */
    long tv_usec;       /* and microseconds */
};
#endif

typedef struct{
    unsigned long tv_sec;
    unsigned long tv_usec;
    unsigned short qflags;
}DCI_Time;


typedef struct{
    unsigned char data[2];
}DCI_Quality;


typedef struct{
    short var_type;     /*��������,�������BaseDaType��Χ������*/
    char resv[2];
    union{
        char b;                 /*BOOL*/
        char dpi;               /*DPI*/
        int i;                  /*char, short, int ��תΪint*/
        unsigned int ui;        /*BYTE, WORD, DWORD ��תΪunsigned int*/
        float f;                /*float*/
        unsigned char data[4];
    }un;
}DCI_BasicData;


typedef struct{
    short var_type;     /*��������,�������BaseDaType��Χ������*/
    short var_len;      /*��������*/
    union{
        char b;                 /*BOOL*/
        char dpi;               /*DPI*/
        int i;                  /*char, short, int ��תΪint*/
        unsigned int ui;        /*BYTE, WORD, DWORD ��תΪunsigned int*/
        double f;               /*float, double ��תΪdouble*/
        DCI_Time t;

        unsigned char bstr[4];      /*bit string*/
        unsigned char oct[4];       /*octet string*/
        char str[4];                /*string*/
        unsigned char data[32];
    }un;
}DCI_VariantData;


#endif
