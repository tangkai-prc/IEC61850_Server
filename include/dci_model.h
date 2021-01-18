#ifndef _DCI_MODEL_INCLUDE_
#define _DCI_MODEL_INCLUDE_

/****************************************************************
 *                        DCI ���� 
 *             Copyright (C) 2010, ����Ƽ��ɷ����޹�˾
 *                        ALL RIGHTS RESERVED
 * �汾 : 1.00
 * ���� : �����
 * ��������: 2010.12.14
 * �ļ��� : dci_model.h
 * ���� : ģ�ͷ��ʽӿں���
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

#ifdef WIN32
#ifdef DCI_EXPORTS
#define DCI_API __declspec(dllexport)
#else
#define DCI_API __declspec(dllimport)
#endif

#if !defined(DCI_EXPORTS)
    #pragma comment(lib,"lib61850.lib")
    #pragma message("Automatically linking with lib61850.dll")
#endif
#else
#define DCI_API
#endif

#include "dcidef.h"


typedef struct{
	char * name;
	char * type;
	char * confVersion;
	char * version;   /* for cid */
	char  link_status;
}DCI_IedInfo;

typedef struct{
	char ip[4];
	char subnetmask[4];
	char gatewayip[4];
	char sntpsvrip[4];
}DCI_ComInfo;

typedef struct{
	char * id;
	int numExt;
	char ** extRef;			/*�ⲿIed��Reference*/
	char ** intAddr;		/*�ڲ���Reference*/
}DCI_Inputs;

typedef struct{
	char * ref;
	int	sid;
	int	dataType;
}DCI_SettingInfo;

typedef struct{
	char * name;		/*������*/
	short type;			/*��������*/
	short size;			/*���ݳ���*/
	void * ptr;			/*����ֵָ��*/
}DCI_DataAttr;

/***********************************
pVal = (i * scaleFactor) + offset
pVal = f * (10 ** multiplier)
************************************/
typedef struct{
	float scaleFactor;		/*ϵ��*/
	float offset;			/*ƫ��*/
	char multiplier;		/*����*/
	unsigned char SIUnit;	/*��λ*/
}DCI_AI_CONFIG;

typedef struct{
	DCI_AI_CONFIG mag;
	DCI_AI_CONFIG ang;
}DCI_CAI_CONFIG;

typedef struct{
	void* inf;
	int	sid;
	int	dataType;
}DCI_MAP;


#ifdef __cplusplus
extern "C"{
#endif


/**************************************************************************
���ܣ��õ�IedInfo
������
	info -- IedInfo���
����ֵ:
	����ԭ��
**************************************************************************/
DCI_API int DCI_GetIedInfo(DCI_IedInfo * info);

/**************************************************************************
���ܣ��õ�ComInfo
������
	info -- ComInfo���
	netno -- subNet���
����ֵ:
	����ԭ��
**************************************************************************/
DCI_API int DCI_GetComInfo(DCI_ComInfo * info, unsigned int netno);

/**************************************************************************
���ܣ�ͨ����Ϣ�����Ƶõ�SID
������
	inf -- ��Ϣ����
����ֵ:
	SID or DCI_INVALID_SID
**************************************************************************/
DCI_API DCI_SID DCI_GetSidByInf(void * inf);

/**************************************************************************
���ܣ�ͨ��SID�õ���Ϣ������
������
	sid -- SID
����ֵ:
	��Ϣ���� or NULL
**************************************************************************/
DCI_API void * DCI_GetInfBySid(DCI_SID sid);

/**************************************************************************
���ܣ�ͨ��SID�õ�������Ϣ������
������
	sid -- SID
	infs -- ���صĽ��
	max_num -- �������
����ֵ:
	���ص���Ϣ�� �� ����ԭ��
**************************************************************************/
DCI_API int DCI_GetInfsBySid(DCI_SID sid, void ** infs, int max_num);

/**************************************************************************
���ܣ�ͨ��SID�õ�Reference
������
	sid -- SID
	ref -- reference������
	ref_size -- reference��������С
����ֵ:
	����ԭ��
**************************************************************************/
DCI_API int DCI_GetRefBySid(DCI_SID sid, char * ref, int ref_size);

/**************************************************************************
���ܣ�ͨ��Reference�õ�SID
������
	ref -- reference
����ֵ:
	SID or DCI_INVALID_SID
**************************************************************************/
DCI_API DCI_SID DCI_GetSidByRef(char * ref);


DCI_API int DCI_GetBioMapNum();

DCI_API int DCI_GetBioMapInfo(DCI_MAP* dci_map, int maxNum);

/**************************************************************************
���ܣ���ȡģ�����ݵ�ֵ
������
	sid -- ģ�����ݵ�sid
	value -- ���ֵ
����ֵ:
	����ԭ��
**************************************************************************/
DCI_API int DCI_GetDataValue(DCI_SID sid, DCI_VariantData * value);

/**************************************************************************
���ܣ���ȡģ�����ݵ�CDC����
������
	sid -- ģ�����ݵ�sid
	value -- ���ֵ
����ֵ:
	����ԭ��
**************************************************************************/
DCI_API char * DCI_GetCDCType(DCI_SID sid);

DCI_API char * DCI_GetCDCTypeByRef(char * ref);


/**************************************************************************
���ܣ���ѯ��������
������
	sid -- FCDA���ݵ�sid
	attr -- ������
����ֵ:
	SID or DCI_INVALID_SID
**************************************************************************/
DCI_API DCI_SID DCI_FindDataAttr(DCI_SID sid, const char * attr);

/**************************************************************************
���ܣ���ѯ��������ֵ
������
	sid -- FCDA���ݵ�sid
	attr_num -- ��������
	attrs -- ���������ͽ��ֵ
����ֵ:
	����ԭ�� �� �������
**************************************************************************/
DCI_API int DCI_QueryDataAttrs(DCI_SID sid, int attr_num, DCI_DataAttr * attrs);

/**************************************************************************
���ܣ��õ�Inputs����
������
����ֵ:
	����
**************************************************************************/
DCI_API int DCI_GetInputsNum();


/**************************************************************************
���ܣ��õ�Inputs����
������
	inputNo -- input���
	inputs -- ���ص�inputs���
����ֵ:
	����ԭ��
**************************************************************************/
DCI_API int DCI_GetInputs(int inputNo, DCI_Inputs * inputs);

DCI_API int DCI_GetSgcbNum();
DCI_API int DCI_GetSettingNum(int sgcb);

DCI_API int DCI_GetSettings(int sgcb, DCI_SettingInfo * setInfos, int maxNum);


/**************************************************************************
���ܣ�ͨ������Reference�õ���Ϣ������
������
	do_ref -- ����DO��reference
����ֵ:
	��Ϣ����
**************************************************************************/
void * DCI_GetOperInf(char * do_ref);

/**************************************************************************
���ܣ�ͨ��Reference�õ�ģ�����Ĳ���
������
	ref -- ģ����reference
	config -- ���ص�ģ�����������
����ֵ:
	����ԭ��
**************************************************************************/
int DCI_GetAIConfig(char * ref, DCI_AI_CONFIG * config);

/**************************************************************************
���ܣ�ͨ��Reference�õ�ģ�����Ĳ���
������
	ref -- ģ����reference
	config -- ���ص�ģ�����������
����ֵ:
	����ԭ��
**************************************************************************/
int DCI_GetCAIConfig(char * ref, DCI_CAI_CONFIG * config);

#ifdef __cplusplus
}
#endif

#endif
