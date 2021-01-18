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


#define DCI_SV_BUF_NUM     2048      /*���Խ���2048��sv, ����Ϊ2��������*/
#define DCI_GOOSE_BUF_NUM  256       /*���Խ���256��goose, ����Ϊ2��������*/
#define DCI_MAX_NETHOOK    4         /*ÿ�������������hook��Ŀ*/


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
/* ��ʼ��ϵͳ���磬Ӧ������Ӧ������ʼ�����ã�Ӧ���ȵ���               */
/* ����ֵ��0��ʾ�ɹ���<0��ʾʧ��                                        */
/************************************************************************/
DCINET_API int DCINET_Init(int protocol);


/************************************************************************/
/* �ͷ�ϵͳ���磬Ӧ������Ӧ��������                                   */
/************************************************************************/
DCINET_API void DCINET_Release();


/************************************************************************/
/* ��ȡ��ǰ�����緢�ͷ�ʽ                                               */
/* ����ֵ��0 (ֱ�ӷ���) , 1 (�жϷ���)                                  */
/************************************************************************/
DCINET_API int DCINET_GetSendMode();


/************************************************************************/
/* ��ȡ���ڵ�����MAC��ַ                                                */
/* ����ֵ��0��ʾ�ɹ���<0��ʾʧ��                                        */
/************************************************************************/
DCINET_API int DCINET_GetNetMac(char netNo, char mac[6]);


/************************************************************************/
/* �趨���ڵ��鲥��ַ                                                   */
/* ����ֵ��0��ʾ�ɹ���<0��ʾʧ��                                        */
/************************************************************************/
DCINET_API int DCINET_AddMultiAddr(char netNo, char* addr);

/************************************************************************/
/* ɾ�����ڵ��鲥��ַ                                                   */
/* ����ֵ��0��ʾ�ɹ���<0��ʾʧ��                                        */
/************************************************************************/
DCINET_API int DCINET_DelMultiAddr(char netNo, char* addr);

/************************************************************************/
/* ���տ��ƽӿ�                                                         */
/* ������enable=0��ʾ�ܾ�����filter�����ı��ģ��������                 */
/*       DCINET���տ��ƹ��ܵ�Ĭ����ΪΪ����                             */
/* ����ֵ��>=0��ʾ�ɹ���<0��ʾʧ��                                        */
/************************************************************************/
DCINET_API int DCINET_RecvControl(char enable, RECV_FILTER* filter);

/************************************************************************/
/* ���ķ���(ֱ�ӷ��ͷ�ʽ)                                               */
/* ����ֵ��0,�ɹ�;����Ϊʧ��                                            */
/************************************************************************/
DCINET_API int DCINET_Send(SEND_PKT* pSendPkt);


/************************************************************************/
/* ���Ľ���                                                             */
/* ����ֵ��������Ч������Ŀ                                             */
/************************************************************************/
DCINET_API int DCINET_Recv(int protocol, RECV_PKT **ppPkt, int timeout);


/************************************************************************/
/* ����������չ��Ӻ���                                                 */
/* ������1. netNo, ����˿����,��0��ʼ;                                */
/*       2. hookfunc, ���籨�Ĵ�����,Ӧ�����������ʽ��               */
/*         int hookFunc(char *buffer, int length)                       */
/*       ��hookFunc�ѶԸñ��Ľ��д���,��������Ӧ��Ҳ����Ҫ�ñ���ʱ,     */
/*       hookFuncӦ������1,���򷵻�0;                                   */
/*       !!! hookFuncӦ����֤ʵ�ִ���ļ��ɿ�,�����Ӱ��ϵͳ����      */
/* ����ֵ��0,�ɹ�; <0Ϊʧ��                                             */
/************************************************************************/
DCINET_API int DCINET_InputHookAdd(char netNo, InputHook hookfunc);



/*************** ����Ϊ���籨���жϷ��ͷ�ʽ�ӿ� *************************/

/************************************************************************/
/* �趨ÿ���ж�����ͱ�������                                         */
/************************************************************************/
DCINET_API void DCINET_SetMaxSendNumPerInt(unsigned int num);

/************************************************************************/
/* create package send pipe                                             */
/* mode : 0, no copy;   1, copy                                         */
/* return : if >=0 , pipe id; if < 0 , error code                       */
/************************************************************************/
DCINET_API int DCINET_CreateSendPipe(int maxPkts, int priority, int mode, SendHook fun);

/************************************************************************/
/* �����ķ��͸�pipe                                                     */
/************************************************************************/
DCINET_API int DCINET_SendPipePkt(int id, SEND_PKT *pSendPkt);

/************************************************************************/
/* ���ķ��ͣ��жϷ��ͷ�ʽ��,���жϷ�����ڵ���                          */
/************************************************************************/
DCINET_API void DCINET_IsrSend();


#if defined __cplusplus
}
#endif

#endif
