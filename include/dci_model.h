#ifndef _DCI_MODEL_INCLUDE_
#define _DCI_MODEL_INCLUDE_

/****************************************************************
 *                        DCI 程序 
 *             Copyright (C) 2010, 南瑞科技股份有限公司
 *                        ALL RIGHTS RESERVED
 * 版本 : 1.00
 * 作者 : 窦仁晖
 * 创建日期: 2010.12.14
 * 文件名 : dci_model.h
 * 描述 : 模型访问接口函数
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
#ifdef DCI_EXPORTS
#define DCI_API __declspec(dllexport)
#else
#define DCI_API __declspec(dllimport)
#endif

#if !defined(DCI_EXPORTS)
    #pragma comment(lib,"lib61850.lib")
    #pragma message("Automatically linking with lib61850.dll")
#endif
#else
#define DCI_API
#endif

#include "dcidef.h"


typedef struct{
	char * name;
	char * type;
	char * confVersion;
	char * version;   /* for cid */
	char  link_status;
}DCI_IedInfo;

typedef struct{
	char ip[4];
	char subnetmask[4];
	char gatewayip[4];
	char sntpsvrip[4];
}DCI_ComInfo;

typedef struct{
	char * id;
	int numExt;
	char ** extRef;			/*外部Ied的Reference*/
	char ** intAddr;		/*内部的Reference*/
}DCI_Inputs;

typedef struct{
	char * ref;
	int	sid;
	int	dataType;
}DCI_SettingInfo;

typedef struct{
	char * name;		/*属性名*/
	short type;			/*数据类型*/
	short size;			/*数据长度*/
	void * ptr;			/*数据值指针*/
}DCI_DataAttr;

/***********************************
pVal = (i * scaleFactor) + offset
pVal = f * (10 ** multiplier)
************************************/
typedef struct{
	float scaleFactor;		/*系数*/
	float offset;			/*偏移*/
	char multiplier;		/*倍率*/
	unsigned char SIUnit;	/*单位*/
}DCI_AI_CONFIG;

typedef struct{
	DCI_AI_CONFIG mag;
	DCI_AI_CONFIG ang;
}DCI_CAI_CONFIG;

typedef struct{
	void* inf;
	int	sid;
	int	dataType;
}DCI_MAP;


#ifdef __cplusplus
extern "C"{
#endif


/**************************************************************************
功能：得到IedInfo
参数：
	info -- IedInfo结果
返回值:
	错误原因
**************************************************************************/
DCI_API int DCI_GetIedInfo(DCI_IedInfo * info);

/**************************************************************************
功能：得到ComInfo
参数：
	info -- ComInfo结果
	netno -- subNet序号
返回值:
	错误原因
**************************************************************************/
DCI_API int DCI_GetComInfo(DCI_ComInfo * info, unsigned int netno);

/**************************************************************************
功能：通过信息的名称得到SID
参数：
	inf -- 信息名称
返回值:
	SID or DCI_INVALID_SID
**************************************************************************/
DCI_API DCI_SID DCI_GetSidByInf(void * inf);

/**************************************************************************
功能：通过SID得到信息的名称
参数：
	sid -- SID
返回值:
	信息名称 or NULL
**************************************************************************/
DCI_API void * DCI_GetInfBySid(DCI_SID sid);

/**************************************************************************
功能：通过SID得到所有信息的名称
参数：
	sid -- SID
	infs -- 返回的结果
	max_num -- 最大结果数
返回值:
	返回的信息数 或 错误原因
**************************************************************************/
DCI_API int DCI_GetInfsBySid(DCI_SID sid, void ** infs, int max_num);

/**************************************************************************
功能：通过SID得到Reference
参数：
	sid -- SID
	ref -- reference缓冲区
	ref_size -- reference缓冲区大小
返回值:
	错误原因
**************************************************************************/
DCI_API int DCI_GetRefBySid(DCI_SID sid, char * ref, int ref_size);

/**************************************************************************
功能：通过Reference得到SID
参数：
	ref -- reference
返回值:
	SID or DCI_INVALID_SID
**************************************************************************/
DCI_API DCI_SID DCI_GetSidByRef(char * ref);


DCI_API int DCI_GetBioMapNum();

DCI_API int DCI_GetBioMapInfo(DCI_MAP* dci_map, int maxNum);

/**************************************************************************
功能：读取模型数据的值
参数：
	sid -- 模型数据的sid
	value -- 结果值
返回值:
	错误原因
**************************************************************************/
DCI_API int DCI_GetDataValue(DCI_SID sid, DCI_VariantData * value);

/**************************************************************************
功能：读取模型数据的CDC类型
参数：
	sid -- 模型数据的sid
	value -- 结果值
返回值:
	错误原因
**************************************************************************/
DCI_API char * DCI_GetCDCType(DCI_SID sid);

DCI_API char * DCI_GetCDCTypeByRef(char * ref);


/**************************************************************************
功能：查询数据属性
参数：
	sid -- FCDA数据的sid
	attr -- 属性名
返回值:
	SID or DCI_INVALID_SID
**************************************************************************/
DCI_API DCI_SID DCI_FindDataAttr(DCI_SID sid, const char * attr);

/**************************************************************************
功能：查询数据属性值
参数：
	sid -- FCDA数据的sid
	attr_num -- 属性数量
	attrs -- 属性条件和结果值
返回值:
	错误原因 或 结果数量
**************************************************************************/
DCI_API int DCI_QueryDataAttrs(DCI_SID sid, int attr_num, DCI_DataAttr * attrs);

/**************************************************************************
功能：得到Inputs数量
参数：
返回值:
	数量
**************************************************************************/
DCI_API int DCI_GetInputsNum();


/**************************************************************************
功能：得到Inputs数据
参数：
	inputNo -- input序号
	inputs -- 返回的inputs结果
返回值:
	错误原因
**************************************************************************/
DCI_API int DCI_GetInputs(int inputNo, DCI_Inputs * inputs);

DCI_API int DCI_GetSgcbNum();
DCI_API int DCI_GetSettingNum(int sgcb);

DCI_API int DCI_GetSettings(int sgcb, DCI_SettingInfo * setInfos, int maxNum);


/**************************************************************************
功能：通过控制Reference得到信息的名称
参数：
	do_ref -- 控制DO的reference
返回值:
	信息名称
**************************************************************************/
void * DCI_GetOperInf(char * do_ref);

/**************************************************************************
功能：通过Reference得到模拟量的参数
参数：
	ref -- 模拟量reference
	config -- 返回的模拟量参数结果
返回值:
	错误原因
**************************************************************************/
int DCI_GetAIConfig(char * ref, DCI_AI_CONFIG * config);

/**************************************************************************
功能：通过Reference得到模拟量的参数
参数：
	ref -- 模拟量reference
	config -- 返回的模拟量参数结果
返回值:
	错误原因
**************************************************************************/
int DCI_GetCAIConfig(char * ref, DCI_CAI_CONFIG * config);

#ifdef __cplusplus
}
#endif

#endif
