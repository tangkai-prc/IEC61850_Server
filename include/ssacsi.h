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
		ACSIREQ_DATASET_DIRECTORY,  ACSIREQ_DATA_DIRECTORY�� ACSIREQ_LOG_NAME		*/
typedef struct{
	int iMaxNames;		/*һ����෵�ص���������*/
	char * name;		/*��ʼ����*/
}SSAcsiGetNameReq;

typedef struct{
	int num;			/*�������Ƶ�����*/
	int moreFollow;		/*����������Ҫ������ȡ��*/
	char * name[MAX_REQ_GET_NAME];		/*����*/
}SSAcsiGetNameRsp;

/*		ACSIREQ_DATA_DEFINITION,  ACSIREQ_DATA_VALUES,  ACSIREQ_DATASET_VALUES		*/
#define ACSIREQ_SET_DATA_VALUES			13			/*дһ��DATA��ֵ*/
typedef struct{
	int num;
	char * name[MAX_REQ_GET_DATA];		/*Data��DataSet����*/
}SSAcsiGetDataReq;

typedef struct{
	int num;			/*����Data������*/
	Asn1Data data[MAX_REQ_GET_DATA];		/*����Data��asn1����*/
	int error[MAX_REQ_GET_DATA];			/* AcsiServiceError */
}SSAcsiGetDataRsp;

/*		ACSIREQ_SET_DATA_VALUES		*/
typedef struct{
	int num;			/*��Ҫд��Data������*/
	Asn1Data data[MAX_REQ_SET_DATA];		/*��Ҫд��Data��asn1����*/
}SSAcsiSetDataReq;

typedef struct{
	int num;			/*��Ҫд��Data������*/
	int error[MAX_REQ_SET_DATA];		/* AcsiServiceError */
}SSAcsiSetDataRsp;

/*		ACSIREQ_GET_DIR		*/
typedef struct{
	char nameDir[MAX_61850_FILENAME_LEN];
	char nameFollow[MAX_61850_FILENAME_LEN];
}SSAcsiFileDirReq;

typedef struct{
	int num;			/*�����ļ����Ƶ�����*/
	int moreFollow;		/*�����ļ�������Ҫ������ȡ��*/
	FileAttrib * attr[MAX_REQ_GET_NAME];
}SSAcsiFileDirRsp;

/*		ACSIREQ_OPEN_FILE		*/
typedef struct{
	char filename[MAX_61850_FILENAME_LEN];
	int init_pos;
}SSAcsiFileOpenReq;

typedef struct{
	unsigned int fp;			/*�ļ�������*/
	FileAttrib attr;
}SSAcsiFileOpenRsp;

/*		ACSIREQ_CLOSE_FILE		*/
typedef struct{
	unsigned int fp;			/*�ļ�������*/
}SSAcsiFileCloseReq;

/*		ACSIREQ_READ_FILE		*/
typedef struct{
	unsigned int fp;			/*�ļ�������*/
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
	char * name;			/*log����*/
}SSAcsiInitLogReq;

typedef struct{
	int del_num;			/*ɾ������Ŀ��*/
}SSAcsiInitLogRsp;

/*		ACSIREQ_LOG_STATUS		*/
typedef struct{
	char * name;			/*log����*/
}SSAcsiLogStatusReq;

typedef struct{
	int num;				/*Log����Ŀ��*/
	int deleteble;			/*��ɾ����?*/
}SSAcsiLogStatusRsp;

/*		ACSIREQ_QUERY_LOG		*/
typedef struct{
	char * name;			/*log����*/
	int iMaxEntry;			/*������ص������Ŀ��*/
	int bStartTime;			/*�в�ѯ��ʼʱ��*/
	int bStartEntry;		/*�в�ѯ��ʼ��Ŀ*/
	int bStopTime;			/*�в�ѯ����ʱ��*/
	TimeStamp start_time;		/*��ʼʱ��*/
	TimeStamp stop_time;		/*����ʱ��*/
	EntryId start_entry;		/*��Ŀid*/
}SSAcsiQueryLogReq;

typedef struct{
	int num;
	int moreFollow;		/*���к�����Ŀ��*/
	SSAcsiEntry entry[MAX_REPORT_ENTRY];
}SSAcsiQueryLogRsp_old;

typedef struct{
	int num;
	int moreFollow;		/*���к�����Ŀ��*/
	SSAcsiEntry entry[MAX_LOG_ENTRY];
}SSAcsiQueryLogRsp;


typedef struct{
	long ld;				/*����������*/
	int type;			/*��������*/
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
