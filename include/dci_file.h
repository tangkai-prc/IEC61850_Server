#ifndef _DCI_FILE_INCLUDE_
#define _DCI_FILE_INCLUDE_

#include "dci.h"
#include "ssacsi.h"


#ifdef __cplusplus
extern "C"{
#endif

/**************************************************************************
功能：Acsi钩子函数，用于处理acsi中钩出消息
参数：
    pReq --- Acsi请求消息
    pRsp --- Acsi响应结果
返回值:
DCI_EAGAIN 表示消息未处理， DCI_ENONE表示消息已被拦截
**************************************************************************/
typedef int (*DCI_AcsiHookFunc)(const SSAcsiReq * pReq, SSAcsiRsp * pRsp);


/**************************************************************************
功能：设置Acsi钩子函数
参数：
func -- Acsi钩子函数
返回值:
错误原因 or DCI_ENONE
**************************************************************************/
DCI_API int DCI_SetAcsiHook(DCI_AcsiHookFunc func);


#ifdef __cplusplus
}
#endif

#endif
