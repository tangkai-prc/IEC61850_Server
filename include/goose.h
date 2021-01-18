#ifndef _GOOSE_H_
#define _GOOSE_H_


/****************************************************************
 *                        GOOSE 程序 
 *             Copyright (C) 2010, 南瑞科技股份有限公司
 *                        ALL RIGHTS RESERVED
 * 版本 : 1.00
 * 作者 : 梅德冬
 * 创建日期: 2010.03.29
 * 文件名 : goose.h
 * 描述 : GOOSE通信接口
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

#include "dcidef.h"
#include "dcisys.h"
#include "dci_model.h"


#ifdef WIN32
#if defined GOOSE_EXPORTS
#define GCIX_API __declspec(dllexport)
#else
#define GCIX_API __declspec(dllimport)
#endif

#else
#define GCIX_API
#endif



#define GSE_ERR_CFG_ERROR		    1
#define GSE_ERR_NET1_INTERRUPT	2
#define GSE_ERR_NET2_INTERRUPT	4
#define GSE_ERR_NET1_SQBAD		  8
#define GSE_ERR_NET2_SQBAD		  0x10



typedef struct
{
	char * filename;
}GSE_AppInfo;


typedef struct
{
	unsigned char type;
	unsigned char element;
	char resv[2];
	unsigned short * inf;			/*装置信息序号*/
}GSE_ElemInfo;


typedef struct
{
	GSE_ElemInfo elem;
	DCI_VariantData value;	/*值*/
	DCI_Quality q;		/*品质*/
	DCI_Time t;			/*刷新时间*/
}GSE_DATA;


typedef struct
{
	char gocb_id;    /* goose输入控制块的序号*/
	char chg;
	char test;
	int  ttl;
	char *pPkt;      /* point to goose packet */
	int  pktLen;
}GSE_InputInfo;


typedef struct
{
	char id[65];
	char input;				/*input or output*/
	char enable;
	char dblNet;             /*是否双网*/
	char netA;               /*A网网口号*/
	char netB;
	char res[3];
	int elem_num;
	unsigned int count;      /*发送/接收报文数*/
}GSE_GoCB;


typedef struct
{
	char gocb_id;    /* goose输入控制块的序号*/
	int state;
	DCI_Time t;			/*时间*/
}GSE_Alarm;

enum GSE_FUNC{
	GSE_NULL = 0,
	GSE_SendGooseInData,
	GSE_FUNC_NUM
};

typedef int (*GSE_CallBackFunc)(int type, void * arg1, void * arg2, void * arg3, void * arg4);


#if defined __cplusplus
extern "C" {
#endif

/************************************************************************/
/* 初始化goose程序，由应用程序调用，goose模块实现                       */
/************************************************************************/
GCIX_API int GSE_Init(GSE_AppInfo * app_inf);

GCIX_API void GSE_Clear();

/************************************************************************/
/* 初始化goose Inputs配置信息，由应用程序调用                           */
/************************************************************************/
GCIX_API int GSE_SetInputs(DCI_Inputs * inputs, int inputsNum);


GCIX_API int GSE_SetCallBack(GSE_CallBackFunc func);

/************************************************************************/
/* 启动goose服务，goose发送控制块默认为非使能状态                       */
/************************************************************************/
GCIX_API void GSE_StartService();

/************************************************************************/
/* 终止goose程序的运行，并释放相应资源                                  */
/************************************************************************/
GCIX_API void GSE_StopService();

/************************************************************************/
/* 使能goose发送控制块，使之开始/停止报文发送，必须先GSE_StartService() */
/************************************************************************/
GCIX_API int GSE_Enable(char gocbId, int enable);

/************************************************************************/
/* 设置goose模块的装置检修状态                                          */
/************************************************************************/
GCIX_API int GSE_SetTest(int test);

/************************************************************************/
/* 获得goose输入输出控制块数目                                          */
/************************************************************************/
GCIX_API int GSE_GetGoCBNum();

/************************************************************************/
/* 获得goose输入输出控制块信息                                          */
/************************************************************************/
GCIX_API GSE_GoCB * GSE_GetGoCB(int no);


/************************************************************************/
/* 获得每个控制块所对应数据的信息                                       */
/* 返回值：数据的个数                                                   */
/************************************************************************/
GCIX_API int GSE_GetElemInfo(int gocb_id, GSE_ElemInfo * elem, int max_num);


/************************************************************************/
/* 根据goose输入数据在模型中的关联位置获得对应的应用数据的信息          */
/* 返回值：=1成功，<0失败                                               */
/************************************************************************/
GCIX_API int GSE_GetGoInElementInfo(char inputNo, char extRefNo, GSE_ElemInfo * info);


/************************************************************************/
/* 给goose模块写数据                                                    */
/* num: 不要大于512，如大于512，须要分多次写值                          */
/* reason: 0/1; 1表示数据变化，0表示数据刷新                            */
/* return: =1，表示写值成功                                             */
/*         <=0，表示写值失败                                            */
/************************************************************************/
GCIX_API int GSE_SetData(GSE_DATA * data, int num, unsigned short reason);


/************************************************************************/
/* 给goose数据集写全数据                                                */
/* num: 数据集大小                                                      */
/* chgFlag: 数据集各个数据的变化标志                                    */
/* gocbId: goose控制块序号                                              */
/* return: =1，表示写值成功                                             */
/*         <=0，表示写值失败                                            */
/************************************************************************/
GCIX_API int GSE_SetDataset(char gocbId, DCI_VariantData * data, char * chgFlag, int num);



/************************************************************************/
/* GSE_GetData：                                                        */
/* 由应用程序调用，goose模块实现                                        */
/* data为指向maxNum大小的缓冲区，用于保存goose接收到的数据              */
/* inf：指向goose信息结构，                                             */
/*    其中pPkt为指向报文缓冲区指针，录报文用；不可在此函数中进行报文拷贝*/
/* 返回值>=0: 接收数据个数                                              */
/* 返回值<0，表示有错误发生                                             */
/************************************************************************/
GCIX_API int GSE_GetData(GSE_DATA * data, int maxNum, GSE_InputInfo * inf, int timeout);


/************************************************************************/
/* GSE_GetAlarm：                                                       */
/* 参数alarm中成员gocb_id须初始化                                       */
/* 返回值=0，表示无新的goose告警信息                                    */
/* 返回值=1: 表示有新的goose告警信息                                    */
/* 返回值<0，表示有错误发生                                             */
/************************************************************************/
GCIX_API int GSE_GetAlarm(GSE_Alarm * alarm);


/************************************************************************/
/* GSE_ResetAlarm：                                                     */
/* 可用于复归GSE_ERR_CFG_ERROR信号                                      */
/* 返回值>=0: 表示成功                                                  */
/* 返回值<0，表示有错误发生                                             */
/************************************************************************/
GCIX_API int GSE_ResetAlarm(unsigned char gocbId);



#if defined __cplusplus
}
#endif

#endif

