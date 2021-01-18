#ifndef _INTERFACE_INCLUDE_
#define _INTERFACE_INCLUDE_


#ifdef WIN32
#if defined(LIBCIDPARSER_EXPORTS)
#define CIDPARSER_API   __declspec(dllexport)
#else
#define CIDPARSER_API   __declspec(dllimport)
#pragma comment(lib, "libcidparser.lib")
#endif

#else
#define CIDPARSER_API

#endif


struct sSclTree;

#ifdef __cplusplus
extern "C"{
#endif

//DLLPORT bool OutputDatFile(char * file_name,char * ied_name,char * ap_name);

/********************************************
������̬����CID�ļ�����װ���ö������ļ�
file_name		:	CID �ļ���
ExportDirPath	:	������Ŀ¼
ap_name			:	CID�ļ��еķ��ʵ�����
file_type		:	�ļ�����,0: sclServerCfg.dat for littleEndian, 1:  sscfg.dat for bigEndian
*********************************************/
CIDPARSER_API int OutputDatFileFromCid(char * file_name, char * ap_name, int file_type);

CIDPARSER_API struct sSclTree * GetSclTreeFromCid(char * file_name, char * ap_name);

CIDPARSER_API void FreeSclTree();

CIDPARSER_API void GetIpAfterCidParse(char **ip, char **ipMask);

#ifdef __cplusplus
}
#endif


#endif
