#ifndef DCI_NET_H_
#define DCI_NET_H_

#if defined WIN32
#include "winsock2.h"
#elif defined VXWORKS
#include "sys/times.h"
#elif defined LINUX
#include "bits/time.h"
#endif

#ifdef WIN32
#ifdef DCISYS_EXPORTS
#define DCINET_API __declspec(dllexport)
#else
#define DCINET_API __declspec(dllimport)
#endif

#if !defined(DCISYS_EXPORTS)
    //#pragma comment(lib,"libdcisys.lib")
    //#pragma message("Automatically linking with libdcisys.dll")
#endif
#else
#define DCINET_API
#endif

/*#include "dcidef.h"*/


#define DCI_PROTOCOL_GOOSE		0x1
#define DCI_PROTOCOL_SMV91    0x2
#define DCI_PROTOCOL_SMV92    0x4
#define DCI_PROTOCOL_GMRP     0x8
#define DCI_PROTOCOL_GOMNT    0x10


#define DCI_SV_BUF_NUM     2048      /*可以接收2048包sv, 必须为2的整次幂*/
#define DCI_GOOSE_BUF_NUM  256       /*可以接收256包goose, 必须为2的整次幂*/
#define DCI_MAX_NETHOOK    4         /*每个网口最多管理的hook数目*/


typedef void (*SendHook)(int result);
typedef int (*InputHook)(char *buffer, int length);

typedef struct
{
	struct timeval ts;
	char netNo;
	char flag;
	unsigned short len;
	char  *buf;
	char  resv[4];
}RECV_PKT;

typedef struct
{
	char srcAddr[8];
	int protocol;
	short pos;
	short len;
	char sample[8];
}RECV_FILTER;

typedef struct
{
	unsigned short netPorts;    /* bit0~bit15 : net0~net15 */
	unsigned short len;
	char resv[8];
	char buf[1536];
}SEND_PKT;




#if defined __cplusplus
extern "C" {
#endif

/************************************************************************/
/* 初始化系统网络，应允许多个应用来初始化调用，应最先调用               */
/* 返回值：0表示成功，<0表示失败                                        */
/************************************************************************/
DCINET_API int DCINET_Init(int protocol);


/************************************************************************/
/* 释放系统网络，应允许多个应用来调用                                   */
/************************************************************************/
DCINET_API void DCINET_Release();


/************************************************************************/
/* 获取当前的网络发送方式                                               */
/* 返回值：0 (直接发送) , 1 (中断发送)                                  */
/************************************************************************/
DCINET_API int DCINET_GetSendMode();


/************************************************************************/
/* 获取网口的物理MAC地址                                                */
/* 返回值：0表示成功，<0表示失败                                        */
/************************************************************************/
DCINET_API int DCINET_GetNetMac(char netNo, char mac[6]);


/************************************************************************/
/* 设定网口的组播地址                                                   */
/* 返回值：0表示成功，<0表示失败                                        */
/************************************************************************/
DCINET_API int DCINET_AddMultiAddr(char netNo, char* addr);

/************************************************************************/
/* 删除网口的组播地址                                                   */
/* 返回值：0表示成功，<0表示失败                                        */
/************************************************************************/
DCINET_API int DCINET_DelMultiAddr(char netNo, char* addr);

/************************************************************************/
/* 接收控制接口                                                         */
/* 参数：enable=0表示拒绝符合filter条件的报文，否则接收                 */
/*       DCINET接收控制功能的默认行为为允许                             */
/* 返回值：>=0表示成功，<0表示失败                                        */
/************************************************************************/
DCINET_API int DCINET_RecvControl(char enable, RECV_FILTER* filter);

/************************************************************************/
/* 报文发送(直接发送方式)                                               */
/* 返回值：0,成功;否则为失败                                            */
/************************************************************************/
DCINET_API int DCINET_Send(SEND_PKT* pSendPkt);


/************************************************************************/
/* 报文接收                                                             */
/* 返回值：接收有效报文数目                                             */
/************************************************************************/
DCINET_API int DCINET_Recv(int protocol, RECV_PKT **ppPkt, int timeout);


/************************************************************************/
/* 增加网络接收钩子函数                                                 */
/* 参数：1. netNo, 网络端口序号,从0开始;                                */
/*       2. hookfunc, 网络报文处理函数,应定义成如下形式：               */
/*         int hookFunc(char *buffer, int length)                       */
/*       当hookFunc已对该报文进行处理,并且其他应用也不需要该报文时,     */
/*       hookFunc应当返回1,否则返回0;                                   */
/*       !!! hookFunc应当保证实现代码的简洁可靠,否则会影响系统运行      */
/* 返回值：0,成功; <0为失败                                             */
/************************************************************************/
DCINET_API int DCINET_InputHookAdd(char netNo, InputHook hookfunc);



/*************** 以下为网络报文中断发送方式接口 *************************/

/************************************************************************/
/* 设定每次中断最大发送报文数量                                         */
/************************************************************************/
DCINET_API void DCINET_SetMaxSendNumPerInt(unsigned int num);

/************************************************************************/
/* create package send pipe                                             */
/* mode : 0, no copy;   1, copy                                         */
/* return : if >=0 , pipe id; if < 0 , error code                       */
/************************************************************************/
DCINET_API int DCINET_CreateSendPipe(int maxPkts, int priority, int mode, SendHook fun);

/************************************************************************/
/* 将报文发送给pipe                                                     */
/************************************************************************/
DCINET_API int DCINET_SendPipePkt(int id, SEND_PKT *pSendPkt);

/************************************************************************/
/* 报文发送（中断发送方式）,由中断服务入口调用                          */
/************************************************************************/
DCINET_API void DCINET_IsrSend();


#if defined __cplusplus
}
#endif

#endif
