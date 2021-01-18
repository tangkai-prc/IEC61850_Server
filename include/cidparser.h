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
用于组态根据CID文件导出装置用二进制文件
file_name		:	CID 文件名
ExportDirPath	:	输出存放目录
ap_name			:	CID文件中的访问点名字
file_type		:	文件类型,0: sclServerCfg.dat for littleEndian, 1:  sscfg.dat for bigEndian
*********************************************/
CIDPARSER_API int OutputDatFileFromCid(char * file_name, char * ap_name, int file_type);

CIDPARSER_API struct sSclTree * GetSclTreeFromCid(char * file_name, char * ap_name);

CIDPARSER_API void FreeSclTree();

CIDPARSER_API void GetIpAfterCidParse(char **ip, char **ipMask);

#ifdef __cplusplus
}
#endif


#endif
