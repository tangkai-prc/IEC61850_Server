#ifndef _SSACSI_INCLUDE_
#define _SSACSI_INCLUDE_

#ifdef WIN32
#ifdef SSACSI_EXPORTS
#define SSACSI_API __declspec(dllexport)
#else
#define SSACSI_API __declspec(dllimport)
#endif

#ifndef SSACSI_ONLY_INCLUDE
#if !defined(SSACSI_EXPORTS)
    #pragma comment(lib,"SSAcsi.lib")
    #pragma message("Automatically linking with SSAcsi.dll")
#endif
#endif
#else
#define SSACSI_API 
#endif

#include "acsidef.h"
#include "mtree.h"

typedef struct{
	char * szAccessPoint;
	unsigned long addr;
	void * pAutoInfo;
	int iAutoLen;
}SSAcsiLinkData;

typedef struct{
	char * name;
	int len;
	unsigned char *data;
}Asn1Data;

typedef struct{
	Asn1Data var;
	unsigned char reason[3];
	unsigned char bLogEvt;  /*indicates if it is a event data for log*/
}SSAcsiEntryData;

typedef struct{
	TimeStamp time;
	EntryId id;
	int num;
	SSAcsiEntryData data[MAX_REPORT_DATA];
}SSAcsiEntry;

typedef struct{
	char * rptID;
	unsigned char optFlds[4];
	unsigned short iSqNum;
	char * szDataSet;
	char bBufOvfl;
	short iSubSeqNum;
	char bMoreSegmentsFollow;
	unsigned int iConfRev;
	int inclusionSize;
	unsigned char inclusion[32];
	SSAcsiEntry entry;
} SSAcsiReport;

#define GetReportBufSize(data_num)		(sizeof(SSAcsiReport)-sizeof(SSAcsiEntryData)*(MAX_REPORT_DATA-data_num))

/*		ACSIREQ_ASSOCIATE, 	ACSIREQ_ABORT,  ACSIREQ_RELEASE		*/
typedef SSAcsiLinkData		SSAcsiAssocReq;

/*		ACSIREQ_LOGIC_DEVICE_NAME, 	ACSIREQ_DATASET_NAME, ACSIREQ_DATASET_NAME
		ACSIREQ_DATASET_DIRECTORY,  ACSIREQ_DATA_DIRECTORY， ACSIREQ_LOG_NAME		*/
typedef struct{
	int iMaxNames;		/*一次最多返回的名称数量*/
	char * name;		/*开始名称*/
}SSAcsiGetNameReq;

typedef struct{
	int num;			/*返回名称的数量*/
	int moreFollow;		/*还有名称需要继续读取吗？*/
	char * name[MAX_REQ_GET_NAME];		/*名称*/
}SSAcsiGetNameRsp;

/*		ACSIREQ_DATA_DEFINITION,  ACSIREQ_DATA_VALUES,  ACSIREQ_DATASET_VALUES		*/
#define ACSIREQ_SET_DATA_VALUES			13			/*写一组DATA的值*/
typedef struct{
	int num;
	char * name[MAX_REQ_GET_DATA];		/*Data或DataSet名称*/
}SSAcsiGetDataReq;

typedef struct{
	int num;			/*返回Data的数量*/
	Asn1Data data[MAX_REQ_GET_DATA];		/*返回Data的asn1编码*/
	int error[MAX_REQ_GET_DATA];			/* AcsiServiceError */
}SSAcsiGetDataRsp;

/*		ACSIREQ_SET_DATA_VALUES		*/
typedef struct{
	int num;			/*需要写入Data的数量*/
	Asn1Data data[MAX_REQ_SET_DATA];		/*需要写入Data的asn1编码*/
}SSAcsiSetDataReq;

typedef struct{
	int num;			/*需要写入Data的数量*/
	int error[MAX_REQ_SET_DATA];		/* AcsiServiceError */
}SSAcsiSetDataRsp;

/*		ACSIREQ_GET_DIR		*/
typedef struct{
	char nameDir[MAX_61850_FILENAME_LEN];
	char nameFollow[MAX_61850_FILENAME_LEN];
}SSAcsiFileDirReq;

typedef struct{
	int num;			/*返回文件名称的数量*/
	int moreFollow;		/*还有文件名称需要继续读取吗？*/
	FileAttrib * attr[MAX_REQ_GET_NAME];
}SSAcsiFileDirRsp;

/*		ACSIREQ_OPEN_FILE		*/
typedef struct{
	char filename[MAX_61850_FILENAME_LEN];
	int init_pos;
}SSAcsiFileOpenReq;

typedef struct{
	unsigned int fp;			/*文件描述符*/
	FileAttrib attr;
}SSAcsiFileOpenRsp;

/*		ACSIREQ_CLOSE_FILE		*/
typedef struct{
	unsigned int fp;			/*文件描述符*/
}SSAcsiFileCloseReq;

/*		ACSIREQ_READ_FILE		*/
typedef struct{
	unsigned int fp;			/*文件描述符*/
	int iMaxSize;
}SSAcsiFileReadReq;

typedef struct{
	int len;
	unsigned char * data;
	int moreFollow;
}SSAcsiFileReadRsp;

/*		ACSIREQ_RENAME_FILE		*/
typedef struct{
	char old_name[MAX_61850_FILENAME_LEN];
	char new_file[MAX_61850_FILENAME_LEN];
}SSAcsiFileRenameReq;

/*		ACSIREQ_DELETE_FILE		*/
typedef struct{
	char filename[MAX_61850_FILENAME_LEN];
}SSAcsiFileDeleteReq;

/*		ACSIREQ_WRITE_FILE		*/
typedef struct{
	char filename[MAX_61850_FILENAME_LEN];
}SSAcsiFileWriteReq;

/*		ACSIREQ_INIT_LOG		*/
typedef struct{
	char * name;			/*log名称*/
}SSAcsiInitLogReq;

typedef struct{
	int del_num;			/*删除的条目数*/
}SSAcsiInitLogRsp;

/*		ACSIREQ_LOG_STATUS		*/
typedef struct{
	char * name;			/*log名称*/
}SSAcsiLogStatusReq;

typedef struct{
	int num;				/*Log的条目数*/
	int deleteble;			/*可删除吗?*/
}SSAcsiLogStatusRsp;

/*		ACSIREQ_QUERY_LOG		*/
typedef struct{
	char * name;			/*log名称*/
	int iMaxEntry;			/*结果返回的最大条目数*/
	int bStartTime;			/*有查询开始时间*/
	int bStartEntry;		/*有查询开始条目*/
	int bStopTime;			/*有查询截至时间*/
	TimeStamp start_time;		/*开始时间*/
	TimeStamp stop_time;		/*截至时间*/
	EntryId start_entry;		/*条目id*/
}SSAcsiQueryLogReq;

typedef struct{
	int num;
	int moreFollow;		/*还有后续条目吗？*/
	SSAcsiEntry entry[MAX_REPORT_ENTRY];
}SSAcsiQueryLogRsp_old;

typedef struct{
	int num;
	int moreFollow;		/*还有后续条目吗？*/
	SSAcsiEntry entry[MAX_LOG_ENTRY];
}SSAcsiQueryLogRsp;


typedef struct{
	long ld;				/*连接描述符*/
	int type;			/*请求类型*/
	union{
		SSAcsiAssocReq assoc;
		SSAcsiGetNameReq get_name;
		SSAcsiGetDataReq get_data;
		SSAcsiSetDataReq set_data;
		SSAcsiFileDirReq file_dir;
		SSAcsiFileOpenReq file_open;
		SSAcsiFileCloseReq file_close;
		SSAcsiFileReadReq file_read;
		SSAcsiFileDeleteReq file_delete;
		SSAcsiFileRenameReq file_rename;
		SSAcsiFileWriteReq file_write;
		SSAcsiInitLogReq init_log;
		SSAcsiLogStatusReq log_status;
		SSAcsiQueryLogReq query_log;
		unsigned char buf[8];
	}un;
}SSAcsiReq;

typedef struct{
	long ld;
	int type;
	int result;
	int error;			/* AcsiServiceError */
	union{
		SSAcsiGetNameRsp get_name;
		SSAcsiGetDataRsp get_data;
		SSAcsiSetDataRsp set_data;
		SSAcsiFileDirRsp file_dir;
		SSAcsiFileOpenRsp file_open;
		SSAcsiFileReadRsp file_read;
		SSAcsiInitLogRsp init_log;
		SSAcsiLogStatusRsp log_status;
		SSAcsiQueryLogRsp_old resv;
		unsigned char buf[8];
	}un;
}SSAcsiRsp;

typedef struct{
	long ld;
	int type;
	int result;
	int error;			/* AcsiServiceError */
	SSAcsiQueryLogRsp query_log;
}SSLogRsp;     /*the query_log is very large, so separates it from SSAcsiRsp*/


#ifdef __cplusplus
extern "C" {
#endif

SSACSI_API int SSAcsi_Init(int count, SSAcsiLinkData * pLinkData);
SSACSI_API void SSAcsi_Clear();
SSACSI_API int SSAcsi_Request(SSAcsiReq * pReq);
SSACSI_API int SSAcsi_Request0(SSAcsiReq ** ppReq);
SSACSI_API int SSAcsi_Response(SSAcsiRsp * pRsp);
SSACSI_API int SSAcsi_Report(long * ld, int ld_num, SSAcsiReport * rpt);
SSACSI_API int SSAcsi_VarList(long * ld, int ld_num, Asn1Data * var, int num);
SSACSI_API unsigned long SSAcsi_GetLinkAddr(long ld);
SSACSI_API int SSAcsi_SetRootPath(char * path);
SSACSI_API int SSAcsi_SetOpt(int opt_name, void * opt_data, int opt_size);
SSACSI_API int SSAcsi_GetOpt(int opt_name, void * opt_data, int opt_size);
SSACSI_API int SSAcsi_SetLocalAddr(char * ip_addr);
SSACSI_API void SSAcsi_EnaTLS();
SSACSI_API void SSAcsi_EnaAcseAuth();

#ifdef __cplusplus
}
#endif

#endif
