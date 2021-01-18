#ifndef _NETLOG_H_INCLUDE
#define _NETLOG_H_INCLUDE

#ifdef WIN32
#ifdef NETLOG_EXPORTS
#define NETLOG_API __declspec(dllexport)
#else
#define NETLOG_API __declspec(dllimport)
#endif

#if !defined(NETLOG_EXPORTS)
    #pragma comment(lib,"netlog.lib")
    #pragma message("Automatically linking with netlog.dll")
#endif

#else
#define NETLOG_API
#endif

#include <stdio.h>
#include <stdarg.h>
#ifdef __cplusplus
extern "C"{
#endif

NETLOG_API int NetlogInit();
NETLOG_API void NetlogClear();
NETLOG_API int NetlogStart(int port, int buf_size);
NETLOG_API void NetlogStop();
NETLOG_API int NetlogRun(int timeout);
NETLOG_API int NetlogTrace(char * fmt, ...);
NETLOG_API int NetlogVTrace(char * fmt, va_list marker);
NETLOG_API void NetlogStoreFileLine(char * file, int line, int level);
NETLOG_API int NetlogOutput();
int ComTrace(char * fmt, ...);
NETLOG_API void NetlogEnable(int bEnable);

NETLOG_API int Netlog_SetLocalAddr(char *ip_addr);

#ifdef __cplusplus
}
#endif

#define NETLOG0		NETLOG
#define NETLOG		NetlogStoreFileLine(__FILE__, __LINE__, 0), NetlogTrace	/*正常*/
#define NETLOG1		NetlogStoreFileLine(__FILE__, __LINE__, 1), NetlogTrace	/*严重错误, 程序退出,红色*/
#define NETLOG2		NetlogStoreFileLine(__FILE__, __LINE__, 2), NetlogTrace	/*一般错误, 程序继续执行,粉红色*/
#define NETLOG3		NetlogStoreFileLine(__FILE__, __LINE__, 3), NetlogTrace	/*提示信息, 蓝色*/
#define NETLOG4		NetlogStoreFileLine(__FILE__, __LINE__, 4), NetlogTrace	/*保留*/
#define NETPRINT	NetlogStoreFileLine(NULL, 0, -1), NetlogTrace
/*
#define printf		ComTrace
#define NETLOG		ComTrace
#define NETLOG1		ComTrace
#define NETPRINT	ComTrace
*/

#endif
