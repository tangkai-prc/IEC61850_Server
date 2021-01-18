#ifndef __ACSIDEF_H_
#define __ACSIDEF_H_



/****************************************************************
 *                        NS2000 源代码
 *             Copyright (C) 2000, 南瑞系统控制公司
 *                        ALL RIGHTS RESERVED
 * 版本 : 1.00
 * 作者 : 窦仁晖
 * 创建日期: 2005.11.16
 * 文件名 : AcsiDef.h
 * 描述 : Acsi数据类型和常量定义的头文件
 *
 *
 * 修改记录1
 * 版本 : 
 * 修改日期 :
 * 修改人 : 
 * 修改内容及原因 :
 *
 *
 * 修改记录1
 * 版本 : 
 * 修改日期 : 
 * 修改人 : 
 * 修改内容及原因 :
 * 
***************************************************************/

#include <sys/types.h>
#include "typedef.h"

/*BaseDaType 基本数据属性类型*/
#define  BDT_NULL			-1
#define  BDT_BOOLEAN		0	/*布尔值*/
#define  BDT_INT8			1	/*-128 ~ 127*/
#define  BDT_INT16			2	/*-32,768 ~ 32,767*/
#define  BDT_INT24			3	/*- 8388608 ~ 8388607  时标类型*/
#define  BDT_INT32			4	/*-2,147,483,648 ~ 2,147,483,647*/
#define  BDT_INT128			5	/*-2**127 ~ (2**127)-1  计数器所需要*/
#define  BDT_INT8U			6	/*无符号整数 0 ~ 255*/
#define  BDT_INT16U			7	/*无符号整数0 ~ 65,535*/
#define  BDT_INT24U			8	/*无符号整数0 ~ 16,777,215*/
#define  BDT_INT32U			9	/*无符号整数0 ~ 4,294,967,295*/
#define  BDT_FLOAT32		10	/*值域和精度为 IEEE754单精度浮点数	*/
#define  BDT_FLOAT64		11	/*值域和精度为 IEEE754双精度浮点数	*/
#define  BDT_ENUMERATED		12	/*值的有序集，采用类型的定义	允许用户扩展*/
#define  BDT_CODEDENUM		13	/*值的有序集，采用类型的定义,不允许用户扩展。类型映射到	*/
#define  BDT_OCTETSTRING	14	/*An element of type OCTET STRING can contain any string of bits. */
#define  BDT_VISIBLESTRING	15	/*An element of type VISIBLE STRING can contain ASCII characters. This set of characters does not include diacritic characters, special European characters, etc. */
#define  BDT_UNICODESTRING	16
#define  BDT_BITSTRING		17	/*位串*/
#define  BDT_VOID			20	/*无符号类型*/
#define  BDT_TimeStamp		21	/*时标*/
#define  BDT_ENTRYTIME		22	/*条目时间*/
#define  BDT_PACKEDLIST		23	/*压缩列表类型*/
#define  BDT_STRUCT       24  
#define  BDT_ARRAY        25  

#define  BDT_MMSEOC			29   /*MMS ERROR CODE*/
#define  BDT_NAME			30	/*ObjectName or Refrence, Visible String*/
#define  BDT_INT			31
#define  BDT_UINT			32
#define  BDT_FLOAT			33
#define  BDT_INT64			34
#define  BDT_BVSTRING		35	/*变长位串*/
#define  BDT_OVSTRING		36	/*变长octet串*/



#define  BDT_STRUCT_BEGIN	100	/* struct { */
#define  BDT_STRUCT_END		101	/* struct } */

#define  MAX_61850_REF_LEN		256
#define  MAX_61850_NAME_LEN	32
#define  MAX_61850_NAME8_LEN     8
#define  MAX_61850_NAME16_LEN     16
#define  MAX_61850_NAME64_LEN		64
#define  MAX_61850_NAME129_LEN		129

#define  MAX_61850_FILENAME_LEN		256

/*功能约束定义*/

typedef enum{
	FC_MX = 0,	/*测量值(模拟值)	DA 代表测量值信息，它的值可读、取代、报告或记入日志但不能写。	从过程得到 DA的初始值.	*/
	FC_ST = 1,	/*状态信息	DA代表状态信息，它的值可读、取代、报告或记入日志但不能写。 	从过程得到 DA的初始值.	*/
	FC_CO = 2,	/*控制	DA 代表控制信息，它的值可操作(控制模型)和读.	n.a.*/
	FC_CF = 3,	/*配置	DA代表配置信息，它的值可写、可读. 值写入后立即生效，或者延缓，延缓的原因超出本标准的范围。	DA的初始值为配置的；值为非易失的.*/
	FC_DC = 4,	/*描述	DA代表描述信息，它的值可写、可读。	 DataAttrib的初始值是配置的，值为非易失的。*/
	FC_SP = 5,	/*设点	DA代表设点信息，它的值可控制(控制模型)和读。其值立即生效。	DA的初始值为配置的；值为非易失的.*/
	FC_SG = 6,	/*定值组	SGCB类的逻辑设备具有几组DA的全部实例值,其功能约束为SG. 在每一组内每个DA有一个带功能约束SG的值. 其中一组值为当前激活值。功能约束SG 的DA值不可写	DA的初始值为配置的；值为非易失的*/
	FC_RP = 7,	/*非缓存报告c)	Attribute代表URCB的报告控制信息，它的值可写、可读.	Attribute的初始值为配置的；值为非易失的.*/
	FC_LG = 8,	/*日志c)	Attribute代表LCB的日志控制信息，它的值可写、可读.	Attribute的初始值为配置的；值为非易失的.*/
	FC_BR = 9,	/*缓存报告c)	Attribute代表BRCB的报告控制信息，它的值可写、可读. 	Attribute的初始值为配置的；值为非易失的.*/
	FC_GO = 10,	/*Goose控制 c)	Attribute代表GoCB的goose控制信息，它的值可写、可读.	Attribute的初始值为配置的；值为非易失的.*/
	FC_GS = 11,	/*gsse 控制c)	Attribute代表GsCB的gsse控制信息，它的值可写、可读.	Attribute的初始值为配置的；值为非易失的.*/
	FC_SV = 12,	/*取代	DA代表取代信息，它的值可写以取代值属性并可读.	DataAttributr的值为易失的, 初始值为FALSE, 否则值为设置或配置.*/
	FC_SE = 13,	/*定值组可编辑的 	DA可由 SGCB 服务进行编辑.	SelectEditSG服务处理后， DA值可用.*/
	FC_MS = 14,	/*多路广播采样值控制c)	Attribute代表MSVCB 的采样值控制信息，它的值可写、可读.	Attribute的初始值为配置的；值为非易失的.*/
	FC_SC = 15,
	FC_US = 16,	/*单路传播采样值控制c)	Attribute代表UNICAST-SVC实例的采样值控制信息，它的值可写、可读.	Attribute的初始值为配置的；值为非易失的.*/
	FC_EX = 17,	/*扩充定义	DA代表扩充信息, 提供引用命名空间(name space)，扩充用于DL/T860.73、 DL/T860.74的LN、 DATA、 DA的扩充定义。功能约束EX的DA值不可写   	DA的初始值为配置的；值为非易失的*/
	FC_XX = 18,	/*所有信息*/
	FC_NUM
}FcEnum;

#define INVALID_LD		((long)-1)

#define REASON_NONE						0x0000
#define REASON_DATA_CHANGE				0x0001
#define REASON_QUALITY_CHANGE			0x0002
#define REASON_DATA_UPDATE				0x0003
#define REASON_INTEGRITY				0x0004
#define REASON_GENERAL_INTERROGATION	0x0005

#define REASON_READ						0x10000
#define REASON_UNSOLICITED				0x20000

#define RANGE_NORMAL			0
#define RANGE_HIGH				1
#define RANGE_LOW				2
#define RANGE_HIGH_HIGH			3
#define RANGE_LOW_LOW			4

#define ACSI_SUCCESS		1
#define ACSI_SUCC			1
#define ACSI_ENONE			0		/*  无错误  */
#define ACSI_EFAIL			-1		/*  操作失败，通常指系统调用或外部函数调用失败  */
#define ACSI_EINVAL			-2		/*  错误的参数，指传入的参数错误，指针为NULL，长度为0等  */
#define ACSI_EPERM			-3		/*  没有操作权限  */
#define ACSI_EACCES			-4		/*  禁止访问，不可读或写  */
#define ACSI_ENOENT			-5		/*  数据对象不存在  */
#define ACSI_ETIMEOUT		-6		/*  超时出错  */
#define ACSI_ENOMEM			-7		/*  内存分配失败，malloc或new出错  */
#define ACSI_EBUSY			-8		/*  忙  */
#define ACSI_EEXIST			-9		/*  对象已存在  */
#define ACSI_EAGAIN			-10		/*  暂时不可用  */
#define ACSI_ENOCONN		-11		/*  未连接  */
#define ACSI_ENOINIT		-12		/*  未初始化  */
#define ACSI_EDATA			-13		/*  数据不正确  */
#define ACSI_ENOBUF			-14		/*  缓冲区满，无法写入数据  */
#define ACSI_ETYPE			-15		/*  类型不一致  */
#define ACSI_ENOOP			-16		/*  无操作  */
#define ACSI_WAITRSP            -100        /* 等待响应*/


#define ACSIREQ_NONE					0
#define ACSIREQ_ASSOCIATE				1			/*建立连接*/
#define ACSIREQ_ABORT					2			/*强行关闭连接，所有未处理的请求被丢弃*/
#define ACSIREQ_RELEASE					3			/*正常关闭连接，所有未处理的请求继续执行并等待其结束*/

#define ACSIREQ_LOGIC_DEVICE_NAME		4			/*读所有LogicDevice名称*/
#define ACSIREQ_LOGIC_NODE_NAME			5			/*读LogicNode的名称*/
#define ACSIREQ_DATASET_NAME			6			/*读所有DATASET的名称*/
#define ACSIREQ_DATASET_DIRECTORY		7			/*读一个DATASET的目录*/
#define ACSIREQ_DATA_DIRECTORY			8			/*读DATA的目录*/
#define ACSIREQ_LOG_NAME				9			/*读所有LOG的名称*/

#define ACSIREQ_DATA_DEFINITION			10			/*读DATA的定义*/
#define ACSIREQ_DATA_VALUES				11			/*读DATA的值*/
#define ACSIREQ_DATASET_VALUES			12			/*读DATASET中所有DATA的值*/
#define ACSIREQ_SET_DATA_VALUES			13			/*写一组DATA的值*/

#define ACSIREQ_FILE_DIR				20			/*读取目录*/
#define ACSIREQ_FILE_OPEN				21			/*打开文件*/
#define ACSIREQ_FILE_READ				22			/*读文件*/
#define ACSIREQ_FILE_CLOSE				23			/*关闭文件*/
#define ACSIREQ_FILE_WRITE				24			/*写文件*/
#define ACSIREQ_FILE_RENAME				25			/*重命名文件*/
#define ACSIREQ_FILE_DELETE				26			/*删除文件*/

#define ACSIREQ_INIT_LOG				30			/*初始化LOG*/
#define ACSIREQ_LOG_STATUS				31			/*读LOG的状态*/
#define ACSIREQ_QUERY_LOG				32			/*查询Log*/

#define ACSIRSP_GET_DATA				1
#define ACSIRSP_OPER_RESULT				2
#define ACSIRSP_REPORT					3
#define ACSIRSP_LOG						4
#define ACSIRSP_ABORT					5


#define MAX_REQ_GET_NAME			500
#define MAX_REQ_GET_DATA			100
#define MAX_REQ_SET_DATA			50
#define MAX_REPORT_DATA				400 /*256*/
#define MAX_REPORT_ENTRY			1
#define MAX_LOG_ENTRY				12
#define MAX_REPORT_VAR				MAX_REPORT_DATA * 3 + 16

#define ACSIWR_SUCCESS					1
#define	ACSIWR_OBJ_INVALIDATED			0
#define	ACSIWR_HW_FAULT					-1
#define	ACSIWR_TEMP_UNAVAIL				-2
#define	ACSIWR_OBJ_ACCESS_DENIED		-3
#define	ACSIWR_OBJ_UNDEFINED			-4
#define	ACSIWR_INVAL_ADDR				-5
#define	ACSIWR_TYPE_UNSUPPORTED			-6
#define	ACSIWR_TYPE_INCONSISTENT		-7
#define ACSIWR_OBJ_ATTR_INCONSISTENT	-8
#define ACSIWR_OBJ_ACC_UNSUPPORTED		-9
#define ACSIWR_OBJ_NONEXISTENT			-10


typedef enum{
	service_error_none = 0,					/* 无错误 */
	instance_not_available,					/* 实例不可用 */
	instance_in_use,						/* 实例正被使用 */
	access_violation,						/* 访问违规 */
	access_not_allowed_in_current_state,
	parameter_value_inappropriate,			/* 参数值不合适 */
	parameter_value_inconsistent,			/* 参数值不一致 */
	class_not_supported,
	instance_locked_by_other_client, 		/* 实例被另一个客户锁定 */
	control_must_be_selected,
	type_conflict,							/* 类型冲突 */
	failed_due_to_communications_constraint,/* 由于通讯限制导致失败 */
	failed_due_to_server_constraint,		/* 由于服务器限制导致失败 */

/* 扩展错误类型，用于同为读写value，某些服务映射为non_existen，某些服务映射为undefined */
	instance_undefined						/* 实例未定义 */
}AcsiServiceError;



#include <sys/types.h>

typedef struct{
	time_t secs;
	unsigned int fraction;
	unsigned char qflags;
	unsigned char res[3];
}TimeStamp;

typedef struct {
	int len;
	unsigned char data[8];
}EntryId;

typedef struct{
	unsigned int size;			/* file size (# bytes)		*/
	TimeStamp mtime; 			/* last modified time		*/
	char * filename;
}FileAttrib;

#include "acsivar.h"

#ifndef bstr_copy
#define bstr_copy(dest, src, len)	(memcpy(dest, src, (len + 7) / 8))
#endif

#ifndef BSTR_BIT_SET_ON
#define BSTR_BIT_SET_ON(ptr,bitnum) \
	( ((unsigned char *)(ptr))[(bitnum)/8] |= (0x80>>((bitnum)&7)) )
#endif

#ifndef BSTR_BIT_SET_OFF
#define BSTR_BIT_SET_OFF(ptr,bitnum) \
	( ((unsigned char *)(ptr))[(bitnum)/8] &= ~(0x80>>((bitnum)&7)) )
#endif

/* BSTR_BIT_GET returns zero if bit is clear, non-zero if bit is set.	*/
/* If MUST return ONLY 0 or 1, simply add "? 1:0" to end of macro.	*/ 
#ifndef BSTR_BIT_GET
#define BSTR_BIT_GET(ptr,bitnum) \
	( ((unsigned char *)(ptr))[(bitnum)/8] &  (0x80>>((bitnum)&7)) )
#endif

#ifndef SetTimeStampSecs
#define SetTimeStampSecs(t, sec) \
	( ((TimeStamp *)t)->secs = sec )
#endif

#ifndef SetTimeStampMsec
#define SetTimeStampMsec(t, msec) \
	( ((TimeStamp *)t)->fraction = (int)((double)msec / 1000.0 * 0x00ffffff + 0.0001) )
#endif

#ifndef GetTimeStampSecs
#define GetTimeStampSecs(t) \
	( ((TimeStamp *)t)->secs )
#endif

#ifndef GetTimeStampMsec
#define GetTimeStampMsec(t) \
	( (int)((double)1000.0 * ((TimeStamp *)t)->fraction / 0x00ffffff + 0.0001) )
#endif

#ifndef GetTimeStampUsec
#define GetTimeStampUsec(t) \
	( (long)((double)1000.0 * ((TimeStamp *)t)->fraction / 0x00ffffff + 0.0001) * 1000 )
#endif

#ifndef SetTimeStampUsec
#define SetTimeStampUsec(t, usec) \
	( ((TimeStamp *)t)->fraction = (int)((double)usec / 1000000.0 * 0x00ffffff + 0.0001) )
#endif

#ifndef SetTimeStampQFlag
#define SetTimeStampQFlag(t, q) \
	( ((TimeStamp *)t)->qflags = (unsigned char)q )
#endif

#ifndef SetEntryId
#define SetEntryId(entry, size, buf) \
	{	((EntryId *)entry)->len = size;\
		if(((EntryId *)entry)->len > sizeof(((EntryId *)entry)->data)) ((EntryId *)entry)->len = sizeof(((EntryId *)entry)->data);\
		memcpy(((EntryId *)entry)->data, buf, ((EntryId *)entry)->len); }
#endif

#define TRGOP_NONE			0
#define TRGOP_DCHG			1
#define TRGOP_QCHG			2
#define TRGOP_DUPD			3
#define TRGOP_INTG			4
#define TRGOP_GI			5


#define OPTFLD_BITNUM_RESERVED		0
#define OPTFLD_BITNUM_SQNUM			1
#define OPTFLD_BITNUM_TIMESTAMP		2
#define OPTFLD_BITNUM_REASON		3
#define OPTFLD_BITNUM_DATSETNAME	4
#define OPTFLD_BITNUM_DATAREF		5
#define OPTFLD_BITNUM_BUFOVFL		6
#define OPTFLD_BITNUM_ENTRYID		7
#define OPTFLD_BITNUM_CONFREV		8
#define OPTFLD_BITNUM_SUBSEQNUM		9

#define CHECK_BITNUM_SYNCHROCH		0
#define CHECK_BITNUM_INTERLOCK		1

#define ORIGIN_NOT_SUPPORTED		0		/*OrCat不支持。*/
#define ORIGIN_BAY_CONTROL			1		/*操作员通过间隔层客户发出的控制操作。*/
#define ORIGIN_STATION_CONTROL		2		/*操作员通过变电站层客户发出的控制操作。*/
#define ORIGIN_REMOTE_CONTROL		3		/*远方操作员在变电站外（如网络控制中心）发出的控制操作。*/
#define ORIGIN_AUTOMATIC_BAY		4		/*间隔层自动功能发出的控制操作。*/
#define ORIGIN_AUTOMATIC_STATION	5		/*变电站层自动功能发出的控制操作。*/
#define ORIGIN_AUTOMATIC_REMOTE		6		/*变电站外的自动功能发出的控制操作。*/
#define ORIGIN_MAINTENANCE			7		/*维护/服务工具发出的控制操作。*/
#define ORIGIN_PROCESS				8		/*无控制行为而出现的状态变位（如断路器外部跳闸或内部故障）。*/

#define OPERATE_UNKNOWN					0
#define OPERATE_NOT_SUPPORTED			1
#define OPERATE_BLOCKED_BY_SWITCHING_HIERARCHY		2
#define OPERATE_SELECT_FAILED			3
#define OPERATE_INVALID_POSITION		4
#define OPERATE_POSITION_REACHED		5
#define OPERATE_PARAMETER_CHANGE_IN_EXECUTION	6
#define OPERATE_STEP_LIMIT				7
#define OPERATE_BLOCKED_BY_MODE			8
#define OPERATE_BLOCKED_BY_PROCESS		9
#define OPERATE_BLOCKED_BY_INTERLOCKING		10
#define OPERATE_BLOCKED_BY_SYNCHROCHECK		11
#define OPERATE_COMMAND_ALREADY_IN_EXECUTION	12
#define OPERATE_BLOCKED_BY_HEALTH		13
#define OPERATE_1_OF_N_CONTROL			14
#define OPERATE_ABORTION_BY_CANCEL		15
#define OPERATE_TIME_LIMIT_OVER			16
#define OPERATE_ABORTION_BY_TRIP		17
#define OPERATE_OBJECT_NOT_SELECTED		18

#define OPERATE_NO_ERROR			0
#define OPERATE_ERROR_UNKNOW		1
#define OPERATE_ERROR_TIMEOUT		2
#define OPERATE_ERROR_OPERATE		3

#endif
