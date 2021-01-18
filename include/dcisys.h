#ifndef _DCI_SYS_INCLUDE_
#define _DCI_SYS_INCLUDE_

/****************************************************************
 *                        DCI 程序 
 *             Copyright (C) 2010, 南瑞科技股份有限公司
 *                        ALL RIGHTS RESERVED
 * 版本 : 1.00
 * 作者 : 窦仁晖
 * 创建日期: 2010.03.29
 * 文件名 : dcisys.h
 * 描述 : 设备通信接口平台相关函数
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

#ifdef WIN32
#ifdef DCISYS_EXPORTS
#define DCISYS_API __declspec(dllexport)
#else
#define DCISYS_API __declspec(dllimport)
#endif

#if !defined(DCISYS_EXPORTS)
    #pragma comment(lib,"dcisys.lib")
    #pragma message("Automatically linking with dcisys.dll")
#endif
#else
#define DCISYS_API
#endif

#include "dcidef.h"

#define DCI_GetTimeInterval(t1, t2)		((long)((t1.tv_sec-t2.tv_sec)*1000+(t1.tv_usec-t2.tv_usec)*0.001))

#ifdef __cplusplus
extern "C"{
#endif


/**************************************************************************
功能：比较信息名称
参数：
	inf1 -- 信息1
	inf2 -- 信息2
返回值:
	小于-1，等于0，大于1
**************************************************************************/
DCISYS_API int DCI_InfComp(void * inf1, void * inf2);

/**************************************************************************
功能：信息名称转换为字符串，用于打印信息名称
参数：
	info -- 信息名称
	asc -- 目的字符串
	size -- asc大小
返回值:
	错误原因
**************************************************************************/
DCISYS_API int DCI_Inf2Asc(void * inf, char * asc, int size);

/**************************************************************************
功能：创建WatchDog
参数：
	tid -- 任务Id
	name -- 任务名称
	period -- 超时时间, ms
返回值:
	WatchDog ID
**************************************************************************/
DCISYS_API DCI_WID DCI_WdCreate(int tid, char * name, unsigned int period);

/**************************************************************************
功能：删除WatchDog
参数：
	wid -- WatchDog Id
返回值:
	错误原因
**************************************************************************/
DCISYS_API int DCI_WdDestroy(DCI_WID wid);

/**************************************************************************
功能：喂WatchDog
参数：
	wid -- WatchDog Id
返回值:
	错误原因
**************************************************************************/
DCISYS_API int DCI_WdSet(DCI_WID wid);


/**************************************************************************
功能：设置时间
参数：
	t -- 时间
返回值:
	1 --- 成功, 0 --- 失败			
**************************************************************************/
DCISYS_API int DCI_SetTime(DCI_Time * t);


/**************************************************************************
功能：得到当前时间
参数：
	t -- 时间
返回值:
	1 --- 成功, 0 --- 失败			
**************************************************************************/
DCISYS_API int DCI_GetTime(DCI_Time * t);


#ifdef __cplusplus
}
#endif

#endif
