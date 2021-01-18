#ifndef _DCI_FILE_INCLUDE_
#define _DCI_FILE_INCLUDE_

#include "dci.h"
#include "ssacsi.h"


#ifdef __cplusplus
extern "C"{
#endif

/**************************************************************************
���ܣ�Acsi���Ӻ��������ڴ���acsi�й�����Ϣ
������
    pReq --- Acsi������Ϣ
    pRsp --- Acsi��Ӧ���
����ֵ:
DCI_EAGAIN ��ʾ��Ϣδ���� DCI_ENONE��ʾ��Ϣ�ѱ�����
**************************************************************************/
typedef int (*DCI_AcsiHookFunc)(const SSAcsiReq * pReq, SSAcsiRsp * pRsp);


/**************************************************************************
���ܣ�����Acsi���Ӻ���
������
func -- Acsi���Ӻ���
����ֵ:
����ԭ�� or DCI_ENONE
**************************************************************************/
DCI_API int DCI_SetAcsiHook(DCI_AcsiHookFunc func);


#ifdef __cplusplus
}
#endif

#endif
