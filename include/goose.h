#ifndef _GOOSE_H_
#define _GOOSE_H_


/****************************************************************
 *                        GOOSE ���� 
 *             Copyright (C) 2010, ����Ƽ��ɷ����޹�˾
 *                        ALL RIGHTS RESERVED
 * �汾 : 1.00
 * ���� : ÷�¶�
 * ��������: 2010.03.29
 * �ļ��� : goose.h
 * ���� : GOOSEͨ�Žӿ�
 *
 *
 * �޸ļ�¼1
 * �汾 : 
 * �޸����� :
 * �޸��� : 
 * �޸����ݼ�ԭ�� :
 *
 *
 * �޸ļ�¼2
 * �汾 : 
 * �޸����� : 
 * �޸��� : 
 * �޸����ݼ�ԭ�� :
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
	unsigned short * inf;			/*װ����Ϣ���*/
}GSE_ElemInfo;


typedef struct
{
	GSE_ElemInfo elem;
	DCI_VariantData value;	/*ֵ*/
	DCI_Quality q;		/*Ʒ��*/
	DCI_Time t;			/*ˢ��ʱ��*/
}GSE_DATA;


typedef struct
{
	char gocb_id;    /* goose������ƿ�����*/
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
	char dblNet;             /*�Ƿ�˫��*/
	char netA;               /*A�����ں�*/
	char netB;
	char res[3];
	int elem_num;
	unsigned int count;      /*����/���ձ�����*/
}GSE_GoCB;


typedef struct
{
	char gocb_id;    /* goose������ƿ�����*/
	int state;
	DCI_Time t;			/*ʱ��*/
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
/* ��ʼ��goose������Ӧ�ó�����ã�gooseģ��ʵ��                       */
/************************************************************************/
GCIX_API int GSE_Init(GSE_AppInfo * app_inf);

GCIX_API void GSE_Clear();

/************************************************************************/
/* ��ʼ��goose Inputs������Ϣ����Ӧ�ó������                           */
/************************************************************************/
GCIX_API int GSE_SetInputs(DCI_Inputs * inputs, int inputsNum);


GCIX_API int GSE_SetCallBack(GSE_CallBackFunc func);

/************************************************************************/
/* ����goose����goose���Ϳ��ƿ�Ĭ��Ϊ��ʹ��״̬                       */
/************************************************************************/
GCIX_API void GSE_StartService();

/************************************************************************/
/* ��ֹgoose��������У����ͷ���Ӧ��Դ                                  */
/************************************************************************/
GCIX_API void GSE_StopService();

/************************************************************************/
/* ʹ��goose���Ϳ��ƿ飬ʹ֮��ʼ/ֹͣ���ķ��ͣ�������GSE_StartService() */
/************************************************************************/
GCIX_API int GSE_Enable(char gocbId, int enable);

/************************************************************************/
/* ����gooseģ���װ�ü���״̬                                          */
/************************************************************************/
GCIX_API int GSE_SetTest(int test);

/************************************************************************/
/* ���goose����������ƿ���Ŀ                                          */
/************************************************************************/
GCIX_API int GSE_GetGoCBNum();

/************************************************************************/
/* ���goose����������ƿ���Ϣ                                          */
/************************************************************************/
GCIX_API GSE_GoCB * GSE_GetGoCB(int no);


/************************************************************************/
/* ���ÿ�����ƿ�����Ӧ���ݵ���Ϣ                                       */
/* ����ֵ�����ݵĸ���                                                   */
/************************************************************************/
GCIX_API int GSE_GetElemInfo(int gocb_id, GSE_ElemInfo * elem, int max_num);


/************************************************************************/
/* ����goose����������ģ���еĹ���λ�û�ö�Ӧ��Ӧ�����ݵ���Ϣ          */
/* ����ֵ��=1�ɹ���<0ʧ��                                               */
/************************************************************************/
GCIX_API int GSE_GetGoInElementInfo(char inputNo, char extRefNo, GSE_ElemInfo * info);


/************************************************************************/
/* ��gooseģ��д����                                                    */
/* num: ��Ҫ����512�������512����Ҫ�ֶ��дֵ                          */
/* reason: 0/1; 1��ʾ���ݱ仯��0��ʾ����ˢ��                            */
/* return: =1����ʾдֵ�ɹ�                                             */
/*         <=0����ʾдֵʧ��                                            */
/************************************************************************/
GCIX_API int GSE_SetData(GSE_DATA * data, int num, unsigned short reason);


/************************************************************************/
/* ��goose���ݼ�дȫ����                                                */
/* num: ���ݼ���С                                                      */
/* chgFlag: ���ݼ��������ݵı仯��־                                    */
/* gocbId: goose���ƿ����                                              */
/* return: =1����ʾдֵ�ɹ�                                             */
/*         <=0����ʾдֵʧ��                                            */
/************************************************************************/
GCIX_API int GSE_SetDataset(char gocbId, DCI_VariantData * data, char * chgFlag, int num);



/************************************************************************/
/* GSE_GetData��                                                        */
/* ��Ӧ�ó�����ã�gooseģ��ʵ��                                        */
/* dataΪָ��maxNum��С�Ļ����������ڱ���goose���յ�������              */
/* inf��ָ��goose��Ϣ�ṹ��                                             */
/*    ����pPktΪָ���Ļ�����ָ�룬¼�����ã������ڴ˺����н��б��Ŀ���*/
/* ����ֵ>=0: �������ݸ���                                              */
/* ����ֵ<0����ʾ�д�����                                             */
/************************************************************************/
GCIX_API int GSE_GetData(GSE_DATA * data, int maxNum, GSE_InputInfo * inf, int timeout);


/************************************************************************/
/* GSE_GetAlarm��                                                       */
/* ����alarm�г�Աgocb_id���ʼ��                                       */
/* ����ֵ=0����ʾ���µ�goose�澯��Ϣ                                    */
/* ����ֵ=1: ��ʾ���µ�goose�澯��Ϣ                                    */
/* ����ֵ<0����ʾ�д�����                                             */
/************************************************************************/
GCIX_API int GSE_GetAlarm(GSE_Alarm * alarm);


/************************************************************************/
/* GSE_ResetAlarm��                                                     */
/* �����ڸ���GSE_ERR_CFG_ERROR�ź�                                      */
/* ����ֵ>=0: ��ʾ�ɹ�                                                  */
/* ����ֵ<0����ʾ�д�����                                             */
/************************************************************************/
GCIX_API int GSE_ResetAlarm(unsigned char gocbId);



#if defined __cplusplus
}
#endif

#endif

