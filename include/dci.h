#ifndef _DCI_INCLUDE_
#define _DCI_INCLUDE_

/****************************************************************
 *                        DCI 程序 
 *             Copyright (C) 2010, 南瑞科技股份有限公司
 *                        ALL RIGHTS RESERVED
 * 版本 : 1.10
 * 作者 : 窦仁晖
 * 创建日期: 2010.03.29
 * 文件名 : dci.h
 * 描述 : 设备通信接口函数
 *
 *
 * 修改记录1
 * 版本 : 1.10
 * 修改日期 : 2010.07.09
 * 修改人 : 窦仁晖
 * 修改内容及原因 : 改为共享库
 *
 *
 * 修改记录2
 * 版本 : 2.10
 * 修改日期 : 2010.12.14
 * 修改人 : 窦仁晖
 * 修改内容及原因 : 增加数据读取和查询函数，并将与模型相关的函数移到dci_model.h
 * 
***************************************************************/

#ifdef WIN32
#ifdef DCI_EXPORTS
#define DCI_API __declspec(dllexport)
#else
#define DCI_API __declspec(dllimport)
#endif

#if !defined(DCI_EXPORTS)
    #pragma comment(lib,"libdci.lib")
    #pragma comment(lib,"lib61850.lib")
    #pragma message("Automatically linking with lib61850.dll")
#endif
#else
#define DCI_API
#endif

#include "dcidef.h"
#include "dcisys.h"
#include "dci_model.h"


typedef struct{
    DCI_SID test_sid;
    unsigned char numOfSGCB;    /*定值控制块数*/
}DCI_AppInfo;

enum DCI_NamPlt{
    vendor, hwVer, swVer, serNum, model, location
};

typedef struct{
    unsigned char numOfSG;      /*定值组数*/
    unsigned char actSG;        /*当前活动定值组*/
    unsigned short numOfSet;    /*定值个数*/
    DCI_Time lActTm;            /*刷新时间*/
}DCI_SGCB;


/*遥控参数的数据结构定义如下*/
typedef struct 
{
    DCI_SID sid;            /*信号ID*/
    unsigned char cmd;          /*选择、执行，取消等*/
    unsigned char type;         /*类型，DCI_OPER_BOOL,DCI_OPER_INT,DCI_OPER_FLOAT,DCI_OPER_STEP等*/
    unsigned char result;       /*结果*/
    unsigned char test;         /*测试标识*/
    unsigned char sync;         /*同期标识*/
    unsigned char block;        /*闭锁标识*/
    union{
        int i;
        float f;
    }value;                     /*控制值*/
    unsigned int timer;         /*遥控时间参数*/
    unsigned int syncPara;      /*同期参数*/
    unsigned int ip;            /*控制来源的IP地址*/
}DCI_Oper;                      /*控制数据*/


typedef struct
{
    DCI_SID sid;        /*信号ID*/
    DCI_VariantData value;  /*值*/
    DCI_Quality q;      /*品质*/
    DCI_Time t;         /*刷新时间*/
}DCI_Data;              /*一般数据*/

typedef struct
{
    DCI_SID sid;        /*信号ID*/
    DCI_VariantData value;  /*值*/
}DCI_Value;             /*一般数据值*/

typedef struct
{
    DCI_SID sid;        /*信号ID*/
    char value;         /*值，DCI_DPI类型，单点需要转换为双点*/
    char resv[1];
    DCI_Quality q;      /*品质*/
    DCI_Time t;         /*刷新时间*/
}DCI_DPI_Data;          /*数字量数据*/

typedef struct
{
    DCI_SID sid;        /*信号ID*/
    short type;         /*数据类型，DCI_INT32 or DCI_FLOAT32*/
    union{
        int i;          /*码值*/
        float f;        /*实际值*/
    }value;
    DCI_Quality q;      /*品质*/
    DCI_Time t;         /*刷新时间*/
}DCI_AI_Data;           /*模拟量数据*/

typedef struct
{
    DCI_SID sid;        /*信号ID*/
    short type;         /*数据类型，DCI_INT32 or DCI_FLOAT32*/
    union{
        int i;
        float f;
    }mag;               /*幅值*/
    union{
        int i;
        float f;
    }ang;               /*角度*/
    DCI_Quality q;      /*品质*/
    DCI_Time t;         /*刷新时间*/
}DCI_CAI_Data;          /*模拟量数据*/

typedef struct
{
    DCI_SID sid;            /*信号ID*/
    unsigned char phase;    /*故障相*/
    char value;             /*值, DCI_BOOL类型*/
    char resv[1];
    DCI_Quality q;      /*品质*/
    DCI_Time t;         /*刷新时间*/
}DCI_Event_Data;

#ifdef __cplusplus
extern "C"{
#endif

/**************************************************************************
功能：DCI初始化
参数：
返回值:
    错误原因
**************************************************************************/
int DCI_Init();

/**************************************************************************
功能：清理DCI
参数：
返回值:
**************************************************************************/
void DCI_Clear();

/**************************************************************************
功能：DCI启动服务
参数：
返回值:
    错误原因
**************************************************************************/
int DCI_StartService();

/**************************************************************************
功能：DCI停止服务
参数：
返回值:
    错误原因
**************************************************************************/
int DCI_StopService();

/**************************************************************************
功能：得到所有数据
参数：
    reqId --- 请求ID
返回值:
    错误原因
**************************************************************************/
int DCI_OnGetAllDataValues(unsigned int reqId);

/**************************************************************************
功能：遥控执行。
参数：
    reqId --- 请求ID
    oper_data --- 遥控参数
返回值：
    错误原因
**************************************************************************/
int DCI_OnOperate(unsigned int reqId, DCI_Oper * oper_data);

/**************************************************************************
功能：写数值。
参数：
    var-- 数值
    num -- 数量
返回值：
    错误原因
**************************************************************************/
int DCI_OnSetValues(unsigned int reqId, DCI_Value * var, short num);

/**************************************************************************
读取定值组信息(定值组个数,当前活动定值组组号,每个定值控制块的定值个数)
参数：
    sgcbNo --- 定值控制块号
    sgcb --- 返回的信息结果
返回值：
    错误原因
**************************************************************************/
int DCI_OnGetSGCB(unsigned char sgcbNo, DCI_SGCB * sgcb);

/**************************************************************************
读取定值
参数：
    reqId --- 请求ID
    sgcbNo --- 定值控制块号
    sgNo --- 定值区号
返回值：
    错误原因
**************************************************************************/
int DCI_OnGetSGValues(unsigned int reqId, unsigned char sgcbNo, unsigned char sgNo);



/**************************************************************************
功能：切换定值区号。
参数：
    sgcbNo --- 定值控制块号，起始为0。仅当IED装置存在多个定值控制块的场合下（比如1:N集中式保护），此参数才有意义。
    sgNo -- 定值区号
返回值：
    错误原因
**************************************************************************/
int DCI_OnSwitchSG(unsigned int reqId, unsigned char sgcbNo, unsigned char sgNo);


/**************************************************************************
功能：预写修改定值。
参数：
    sgcbNo --- 定值控制块号，起始为0。
                     仅当IED装置存在多个定值控制块的场合下（比如1:N集中式保护），此参数才有意义。
    sgNo -- 定值区号
    var-- 定值
    num -- 定值数
返回值：
    错误原因
**************************************************************************/
int DCI_OnSetSGValues(unsigned int reqId, unsigned char sgcbNo, unsigned char sgNo, DCI_Value * var, short num);


/**************************************************************************
功能：确认修改定值。
参数：
    sgcbNo --- 定值控制块号，起始为0。
                     仅当IED装置存在多个定值控制块的场合下（比如1:N集中式保护），此参数才有意义。
    sgNo -- 定值区号
    var-- 定值
    num -- 定值数
返回值：
    错误原因
**************************************************************************/
int DCI_OnConfirmEditSGValues(unsigned int reqId, unsigned char sgcbNo, unsigned char sgNo, DCI_Value * var, short num);


/**************************************************************************
功能：撤销修改定值
参数：
    sgcbNo --- 定值控制块号，起始为0。
返回值：
    错误原因
**************************************************************************/
int DCI_OnCancelEditSGValues(unsigned int reqId, unsigned char sgcbNo);


/**************************************************************************
功能：取代
参数：
    data -- 取代值
    state -- 取代状态，DCI_OFF, DCI_ON
返回值:
    错误原因
**************************************************************************/
int DCI_OnSubstitute(unsigned int reqId, DCI_Data * data, unsigned char state);


/**************************************************************************
功能：发送结果
参数：
    reqId --- 请求ID
    result -- 结果
返回值:
    错误原因
**************************************************************************/
int DCI_SendResult(unsigned int reqId, short result);

/**************************************************************************
功能：发送数据
参数：
    reqId --- 请求ID
    data -- 数据
    num -- 数量
    reason -- 发送原因
返回值:
    错误原因
**************************************************************************/
int DCI_SendData(unsigned int reqId, DCI_Data * data, short num, unsigned short reason);

/**************************************************************************
功能：发送数据值
参数：
    reqId --- 请求ID
    var -- 数据值
    num -- 数量
    reason -- 发送原因
返回值:
    错误原因
**************************************************************************/
int DCI_SendValues(unsigned int reqId, DCI_Value * var, short num, unsigned short reason);

/**************************************************************************
功能：发送数字量数据
参数：
    reqId --- 请求ID
    data -- 数据
    num -- 数量
    reason -- 发送原因
返回值:
    错误原因
**************************************************************************/
int DCI_SendDPI(unsigned int reqId, DCI_DPI_Data * data, short num, unsigned short reason);

/**************************************************************************
功能：发送模拟量数据
参数：
    reqId --- 请求ID
    data -- 数据
    num -- 数量
    reason -- 发送原因
返回值:
    错误原因
**************************************************************************/
int DCI_SendAI(unsigned int reqId, DCI_AI_Data * data, short num, unsigned short reason);

/**************************************************************************
功能：发送模拟量数据
参数：
    reqId --- 请求ID
    data -- 数据
    num -- 数量
    reason -- 发送原因
返回值:
    错误原因
**************************************************************************/
int DCI_SendCAI(unsigned int reqId, DCI_CAI_Data * data, short num, unsigned short reason);

/**************************************************************************
功能：发送事件
参数：
    reqId --- 请求ID
    event -- 保护事件
    reason -- 发送原因
返回值:
    错误原因
**************************************************************************/
int DCI_SendEvent(unsigned int reqId, DCI_Event_Data * event, unsigned short reason);


/**************************************************************************
功能：发送定值
参数：
    sgcbNo --- 定值控制块号，起始为0。
                     仅当IED装置存在多个定值控制块的场合下（比如1:N集中式保护），此参数才有意义。
    sgNo -- 定值区号
    set -- 定值缓冲区
    num -- 定值个数
    reason -- 发送原因，DCI_REASON_SWITCH_SG表示就地切换定值
返回值:
    错误原因
**************************************************************************/
int DCI_SendSGValues(unsigned int reqId,unsigned char sgcbNo, unsigned char sgNo, DCI_Value * data, short num, unsigned short reason);


/**************************************************************************
功能：设置AppInfo
参数：
    info -- AppInfo结果
返回值:
    错误原因
**************************************************************************/
DCI_API int DCI_SetAppInfo(DCI_AppInfo * info);

/**************************************************************************
功能：得到请求的命令
参数：超时时间，毫秒，-1表示无限
返回值:
    错误原因, DCI_ETIMEOUT or DCI_ENONE
说明：调用此函数会导致任务挂起，直到接收到命令或timeouts时间到。
      收到命令，使得回调函数(例如DCI_OnGetAllDataValues，DCI_OnOperate等)被调用，然后此函数返回
**************************************************************************/
int DCI_GetCommand(int timeouts);

/**************************************************************************
功能：设置q
参数：
    q -- 品质
    bits -- 设置的品质位
    value -- 设置的品质值
返回值:
    错误原因
**************************************************************************/
DCI_API int DCI_Q_Set(DCI_Quality * q, int bits, int value);

/**************************************************************************
功能：得到品质值
参数：
    q -- 品质
    bits -- 获取的品质位
返回值:
    品质
**************************************************************************/
DCI_API int DCI_Q_Get(DCI_Quality * q, int bits);


/**************************************************************************
功能：打印调试参数
参数：
返回值:
    错误原因
**************************************************************************/
DCI_API void DCI_PrintOpts();


/**************************************************************************
功能：设置调试参数
参数：
    opt -- 参数名称
    value -- 参数值
    size -- 参数长度
返回值:
    错误原因
**************************************************************************/
DCI_API int DCI_SetOpt(char * opt, void * value, unsigned int size);


/**************************************************************************
功能：设置装置的铭牌信息
参数：
    opt -- 参数名称
    value -- 参数值
    size -- 参数长度
返回值:
    错误原因
**************************************************************************/
DCI_API void DCI_SetNamePltInfo(enum DCI_NamPlt type, char* info);



/**************************************************************************
功能：IEC61850单线程任务的入口，只用于ARP平台装置
返回值:
    错误原因
**************************************************************************/
int Iec61850TaskRoutine();
void OnMmsSockRecv();


#ifdef __cplusplus
}
#endif

#endif
