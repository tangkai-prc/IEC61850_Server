#ifndef __DCIDEF_H_
#define __DCIDEF_H_

/****************************************************************
 *                        DCI 程序 
 *             Copyright (C) 2010, 南瑞科技股份有限公司
 *                        ALL RIGHTS RESERVED
 * 版本 : 1.00
 * 作者 : 窦仁晖
 * 创建日期: 2010.03.29
 * 文件名 : dcidef.h
 * 描述 : 设备通信接口数据类型和常量定义
 *
 *
 * 修改记录1
 * 版本 : 
 * 修改日期 :
 * 修改人 : 
 * 修改内容及原因 :
 *
 *
 * 修改记录2
 * 版本 : 
 * 修改日期 : 
 * 修改人 : 
 * 修改内容及原因 :
 * 
***************************************************************/

#include <sys/types.h>

/******************     错误原因        ******************/
#define DCI_ENONE           0       /*  无错误  */
#define DCI_EFAIL           -1      /*  操作失败，通常指系统调用或外部函数调用失败  */
#define DCI_EINVAL          -2      /*  错误的参数 */
#define DCI_EPERM           -3      /*  没有操作权限  */
#define DCI_EACCES          -4      /*  禁止访问，不可读或写  */
#define DCI_ENOENT          -5      /*  数据对象不存在  */
#define DCI_ETIMEOUT        -6      /*  超时出错  */
#define DCI_ENOMEM          -7      /*  内存分配失败，malloc或new出错  */
#define DCI_EBUSY           -8      /*  忙  */
#define DCI_EEXIST          -9      /*  对象已存在  */
#define DCI_EAGAIN          -10     /*  暂时不可用  */
#define DCI_ENOCONN         -11     /*  未连接  */
#define DCI_ENOINIT         -12     /*  未初始化  */
#define DCI_EDATA           -13     /*  数据不正确  */
#define DCI_ENOBUF          -14     /*  缓冲区满，无法写入数据  */
#define DCI_WAITRSP              -100



/******************     基本数据类型        ******************/
#define  DCI_NULL           -1
#define  DCI_BOOL           0   /*布尔值*/
#define  DCI_INT8           1   /*-128 ~ 127*/
#define  DCI_INT16          2   /*-32,768 ~ 32,767*/
#define  DCI_INT32          4   /*-2,147,483,648 ~ 2,147,483,647*/
#define  DCI_INT64          50  /*-2**63 ~ (2**63)-1  计数器所需要*/
#define  DCI_INT8U          6   /*无符号整数 0 ~ 255*/
#define  DCI_INT16U         7   /*无符号整数0 ~ 65,535*/
#define  DCI_INT32U         9   /*无符号整数0 ~ 4,294,967,295*/
#define  DCI_INT64U         51  /*无符号整数0 ~ (2**64)-1*/
#define  DCI_FLOAT32        10  /*值域和精度为 IEEE754单精度浮点数  */
#define  DCI_FLOAT64        11  /*值域和精度为 IEEE754双精度浮点数  */
#define  DCI_TIME           52  /*时间*/

#define  DCI_DPI            101 /*双点，0x01－分，0x02－合*/

/******************     变长数据类型        ******************/
#define  DCI_OCTET_STRING   14
#define  DCI_STRING         15
#define  DCI_UNICODE_STRING 16
#define  DCI_BIT_STRING     17  /*位串*/



/*******************    发送原因    ***************************/
#define DCI_REASON_DCHG             1               /*数据值变化上送*/
#define DCI_REASON_QCHG             2               /*数据品质变化上送*/

#define DCI_REASON_REQ              0x100           /*请求*/
#define DCI_REASON_RESP             0x101           /*响应*/

#define DCI_REASON_INIT             0x102           /*初始化*/
#define DCI_REASON_SWITCH_SG        0x103           /*切换定值区*/

/*******************    控制命令    ***************************/
#define DCI_SELECT              1               /*选择*/
#define DCI_OPER                2               /*执行*/
#define DCI_CANCEL              3               /*取消*/
#define DCI_DIRECT_OPER         4               /*直接执行*/
#define DCI_RESELECT            5               /*重新选择*/


/*******************    控制类型    ***************************/
#define DCI_OPER_BOOL               1           /* value.i, DCI_OPER_OFF, DCI_OPER_ON*/
#define DCI_OPER_INT                2           /* value.i */
#define DCI_OPER_FLOAT              3           /* value.f */
#define DCI_OPER_STEP               4           /* value.i, DCI_OPER_STOP, DCI_OPER_LOWER, DCI_OPER_HIGHER*/

#define DCI_OPER_OFF                0           /*分闸*/
#define DCI_OPER_ON                 1           /*合闸*/
#define DCI_OPER_STOP               0           /*急停*/
#define DCI_OPER_LOWER              1           /*下降*/
#define DCI_OPER_HIGHER             2           /*上升*/

#define DCI_PHASE_A         1
#define DCI_PHASE_B         2
#define DCI_PHASE_C         4
#define DCI_PHASE_N         8

#define DCI_DIR_UNKNOWN         0
#define DCI_DIR_FORWORD         1
#define DCI_DIR_BACKWORD        2
#define DCI_DIR_BOTH            3

/********************   控制操作错误原因    *********************/
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

/********************   品质值  *********************/
#define DCI_Q_good          0
#define DCI_Q_invalid       1
#define DCI_Q_questionable  3

#define DCI_Q_process       0
#define DCI_Q_substituted   1

/********************   品质位  *********************/
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


/********************   相别    *********************/
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


typedef int         DCI_SID;    /* 设备信号ID */
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
    short var_type;     /*变量类型,必须符合BaseDaType范围内类型*/
    char resv[2];
    union{
        char b;                 /*BOOL*/
        char dpi;               /*DPI*/
        int i;                  /*char, short, int 均转为int*/
        unsigned int ui;        /*BYTE, WORD, DWORD 均转为unsigned int*/
        float f;                /*float*/
        unsigned char data[4];
    }un;
}DCI_BasicData;


typedef struct{
    short var_type;     /*变量类型,必须符合BaseDaType范围内类型*/
    short var_len;      /*变量长度*/
    union{
        char b;                 /*BOOL*/
        char dpi;               /*DPI*/
        int i;                  /*char, short, int 均转为int*/
        unsigned int ui;        /*BYTE, WORD, DWORD 均转为unsigned int*/
        double f;               /*float, double 均转为double*/
        DCI_Time t;

        unsigned char bstr[4];      /*bit string*/
        unsigned char oct[4];       /*octet string*/
        char str[4];                /*string*/
        unsigned char data[32];
    }un;
}DCI_VariantData;


#endif
