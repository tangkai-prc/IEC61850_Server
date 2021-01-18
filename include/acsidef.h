#ifndef __ACSIDEF_H_
#define __ACSIDEF_H_



/****************************************************************
 *                        NS2000 Դ����
 *             Copyright (C) 2000, ����ϵͳ���ƹ�˾
 *                        ALL RIGHTS RESERVED
 * �汾 : 1.00
 * ���� : �����
 * ��������: 2005.11.16
 * �ļ��� : AcsiDef.h
 * ���� : Acsi�������ͺͳ��������ͷ�ļ�
 *
 *
 * �޸ļ�¼1
 * �汾 : 
 * �޸����� :
 * �޸��� : 
 * �޸����ݼ�ԭ�� :
 *
 *
 * �޸ļ�¼1
 * �汾 : 
 * �޸����� : 
 * �޸��� : 
 * �޸����ݼ�ԭ�� :
 * 
***************************************************************/

#include <sys/types.h>
#include "typedef.h"

/*BaseDaType ����������������*/
#define  BDT_NULL			-1
#define  BDT_BOOLEAN		0	/*����ֵ*/
#define  BDT_INT8			1	/*-128 ~ 127*/
#define  BDT_INT16			2	/*-32,768 ~ 32,767*/
#define  BDT_INT24			3	/*- 8388608 ~ 8388607  ʱ������*/
#define  BDT_INT32			4	/*-2,147,483,648 ~ 2,147,483,647*/
#define  BDT_INT128			5	/*-2**127 ~ (2**127)-1  ����������Ҫ*/
#define  BDT_INT8U			6	/*�޷������� 0 ~ 255*/
#define  BDT_INT16U			7	/*�޷�������0 ~ 65,535*/
#define  BDT_INT24U			8	/*�޷�������0 ~ 16,777,215*/
#define  BDT_INT32U			9	/*�޷�������0 ~ 4,294,967,295*/
#define  BDT_FLOAT32		10	/*ֵ��;���Ϊ IEEE754�����ȸ�����	*/
#define  BDT_FLOAT64		11	/*ֵ��;���Ϊ IEEE754˫���ȸ�����	*/
#define  BDT_ENUMERATED		12	/*ֵ�����򼯣��������͵Ķ���	�����û���չ*/
#define  BDT_CODEDENUM		13	/*ֵ�����򼯣��������͵Ķ���,�������û���չ������ӳ�䵽	*/
#define  BDT_OCTETSTRING	14	/*An element of type OCTET STRING can contain any string of bits. */
#define  BDT_VISIBLESTRING	15	/*An element of type VISIBLE STRING can contain ASCII characters. This set of characters does not include diacritic characters, special European characters, etc. */
#define  BDT_UNICODESTRING	16
#define  BDT_BITSTRING		17	/*λ��*/
#define  BDT_VOID			20	/*�޷�������*/
#define  BDT_TimeStamp		21	/*ʱ��*/
#define  BDT_ENTRYTIME		22	/*��Ŀʱ��*/
#define  BDT_PACKEDLIST		23	/*ѹ���б�����*/
#define  BDT_STRUCT       24  
#define  BDT_ARRAY        25  

#define  BDT_MMSEOC			29   /*MMS ERROR CODE*/
#define  BDT_NAME			30	/*ObjectName or Refrence, Visible String*/
#define  BDT_INT			31
#define  BDT_UINT			32
#define  BDT_FLOAT			33
#define  BDT_INT64			34
#define  BDT_BVSTRING		35	/*�䳤λ��*/
#define  BDT_OVSTRING		36	/*�䳤octet��*/



#define  BDT_STRUCT_BEGIN	100	/* struct { */
#define  BDT_STRUCT_END		101	/* struct } */

#define  MAX_61850_REF_LEN		256
#define  MAX_61850_NAME_LEN	32
#define  MAX_61850_NAME8_LEN     8
#define  MAX_61850_NAME16_LEN     16
#define  MAX_61850_NAME64_LEN		64
#define  MAX_61850_NAME129_LEN		129

#define  MAX_61850_FILENAME_LEN		256

/*����Լ������*/

typedef enum{
	FC_MX = 0,	/*����ֵ(ģ��ֵ)	DA �������ֵ��Ϣ������ֵ�ɶ���ȡ��������������־������д��	�ӹ��̵õ� DA�ĳ�ʼֵ.	*/
	FC_ST = 1,	/*״̬��Ϣ	DA����״̬��Ϣ������ֵ�ɶ���ȡ��������������־������д�� 	�ӹ��̵õ� DA�ĳ�ʼֵ.	*/
	FC_CO = 2,	/*����	DA ���������Ϣ������ֵ�ɲ���(����ģ��)�Ͷ�.	n.a.*/
	FC_CF = 3,	/*����	DA����������Ϣ������ֵ��д���ɶ�. ֵд���������Ч�������ӻ����ӻ���ԭ�򳬳�����׼�ķ�Χ��	DA�ĳ�ʼֵΪ���õģ�ֵΪ����ʧ��.*/
	FC_DC = 4,	/*����	DA����������Ϣ������ֵ��д���ɶ���	 DataAttrib�ĳ�ʼֵ�����õģ�ֵΪ����ʧ�ġ�*/
	FC_SP = 5,	/*���	DA���������Ϣ������ֵ�ɿ���(����ģ��)�Ͷ�����ֵ������Ч��	DA�ĳ�ʼֵΪ���õģ�ֵΪ����ʧ��.*/
	FC_SG = 6,	/*��ֵ��	SGCB����߼��豸���м���DA��ȫ��ʵ��ֵ,�书��Լ��ΪSG. ��ÿһ����ÿ��DA��һ��������Լ��SG��ֵ. ����һ��ֵΪ��ǰ����ֵ������Լ��SG ��DAֵ����д	DA�ĳ�ʼֵΪ���õģ�ֵΪ����ʧ��*/
	FC_RP = 7,	/*�ǻ��汨��c)	Attribute����URCB�ı��������Ϣ������ֵ��д���ɶ�.	Attribute�ĳ�ʼֵΪ���õģ�ֵΪ����ʧ��.*/
	FC_LG = 8,	/*��־c)	Attribute����LCB����־������Ϣ������ֵ��д���ɶ�.	Attribute�ĳ�ʼֵΪ���õģ�ֵΪ����ʧ��.*/
	FC_BR = 9,	/*���汨��c)	Attribute����BRCB�ı��������Ϣ������ֵ��д���ɶ�. 	Attribute�ĳ�ʼֵΪ���õģ�ֵΪ����ʧ��.*/
	FC_GO = 10,	/*Goose���� c)	Attribute����GoCB��goose������Ϣ������ֵ��д���ɶ�.	Attribute�ĳ�ʼֵΪ���õģ�ֵΪ����ʧ��.*/
	FC_GS = 11,	/*gsse ����c)	Attribute����GsCB��gsse������Ϣ������ֵ��д���ɶ�.	Attribute�ĳ�ʼֵΪ���õģ�ֵΪ����ʧ��.*/
	FC_SV = 12,	/*ȡ��	DA����ȡ����Ϣ������ֵ��д��ȡ��ֵ���Բ��ɶ�.	DataAttributr��ֵΪ��ʧ��, ��ʼֵΪFALSE, ����ֵΪ���û�����.*/
	FC_SE = 13,	/*��ֵ��ɱ༭�� 	DA���� SGCB ������б༭.	SelectEditSG������� DAֵ����.*/
	FC_MS = 14,	/*��·�㲥����ֵ����c)	Attribute����MSVCB �Ĳ���ֵ������Ϣ������ֵ��д���ɶ�.	Attribute�ĳ�ʼֵΪ���õģ�ֵΪ����ʧ��.*/
	FC_SC = 15,
	FC_US = 16,	/*��·��������ֵ����c)	Attribute����UNICAST-SVCʵ���Ĳ���ֵ������Ϣ������ֵ��д���ɶ�.	Attribute�ĳ�ʼֵΪ���õģ�ֵΪ����ʧ��.*/
	FC_EX = 17,	/*���䶨��	DA����������Ϣ, �ṩ���������ռ�(name space)����������DL/T860.73�� DL/T860.74��LN�� DATA�� DA�����䶨�塣����Լ��EX��DAֵ����д   	DA�ĳ�ʼֵΪ���õģ�ֵΪ����ʧ��*/
	FC_XX = 18,	/*������Ϣ*/
	FC_NUM
}FcEnum;

#define INVALID_LD		((long)-1)

#define REASON_NONE						0x0000
#define REASON_DATA_CHANGE				0x0001
#define REASON_QUALITY_CHANGE			0x0002
#define REASON_DATA_UPDATE				0x0003
#define REASON_INTEGRITY				0x0004
#define REASON_GENERAL_INTERROGATION	0x0005

#define REASON_READ						0x10000
#define REASON_UNSOLICITED				0x20000

#define RANGE_NORMAL			0
#define RANGE_HIGH				1
#define RANGE_LOW				2
#define RANGE_HIGH_HIGH			3
#define RANGE_LOW_LOW			4

#define ACSI_SUCCESS		1
#define ACSI_SUCC			1
#define ACSI_ENONE			0		/*  �޴���  */
#define ACSI_EFAIL			-1		/*  ����ʧ�ܣ�ͨ��ָϵͳ���û��ⲿ��������ʧ��  */
#define ACSI_EINVAL			-2		/*  ����Ĳ�����ָ����Ĳ�������ָ��ΪNULL������Ϊ0��  */
#define ACSI_EPERM			-3		/*  û�в���Ȩ��  */
#define ACSI_EACCES			-4		/*  ��ֹ���ʣ����ɶ���д  */
#define ACSI_ENOENT			-5		/*  ���ݶ��󲻴���  */
#define ACSI_ETIMEOUT		-6		/*  ��ʱ����  */
#define ACSI_ENOMEM			-7		/*  �ڴ����ʧ�ܣ�malloc��new����  */
#define ACSI_EBUSY			-8		/*  æ  */
#define ACSI_EEXIST			-9		/*  �����Ѵ���  */
#define ACSI_EAGAIN			-10		/*  ��ʱ������  */
#define ACSI_ENOCONN		-11		/*  δ����  */
#define ACSI_ENOINIT		-12		/*  δ��ʼ��  */
#define ACSI_EDATA			-13		/*  ���ݲ���ȷ  */
#define ACSI_ENOBUF			-14		/*  �����������޷�д������  */
#define ACSI_ETYPE			-15		/*  ���Ͳ�һ��  */
#define ACSI_ENOOP			-16		/*  �޲���  */
#define ACSI_WAITRSP            -100        /* �ȴ���Ӧ*/


#define ACSIREQ_NONE					0
#define ACSIREQ_ASSOCIATE				1			/*��������*/
#define ACSIREQ_ABORT					2			/*ǿ�йر����ӣ�����δ��������󱻶���*/
#define ACSIREQ_RELEASE					3			/*�����ر����ӣ�����δ������������ִ�в��ȴ������*/

#define ACSIREQ_LOGIC_DEVICE_NAME		4			/*������LogicDevice����*/
#define ACSIREQ_LOGIC_NODE_NAME			5			/*��LogicNode������*/
#define ACSIREQ_DATASET_NAME			6			/*������DATASET������*/
#define ACSIREQ_DATASET_DIRECTORY		7			/*��һ��DATASET��Ŀ¼*/
#define ACSIREQ_DATA_DIRECTORY			8			/*��DATA��Ŀ¼*/
#define ACSIREQ_LOG_NAME				9			/*������LOG������*/

#define ACSIREQ_DATA_DEFINITION			10			/*��DATA�Ķ���*/
#define ACSIREQ_DATA_VALUES				11			/*��DATA��ֵ*/
#define ACSIREQ_DATASET_VALUES			12			/*��DATASET������DATA��ֵ*/
#define ACSIREQ_SET_DATA_VALUES			13			/*дһ��DATA��ֵ*/

#define ACSIREQ_FILE_DIR				20			/*��ȡĿ¼*/
#define ACSIREQ_FILE_OPEN				21			/*���ļ�*/
#define ACSIREQ_FILE_READ				22			/*���ļ�*/
#define ACSIREQ_FILE_CLOSE				23			/*�ر��ļ�*/
#define ACSIREQ_FILE_WRITE				24			/*д�ļ�*/
#define ACSIREQ_FILE_RENAME				25			/*�������ļ�*/
#define ACSIREQ_FILE_DELETE				26			/*ɾ���ļ�*/

#define ACSIREQ_INIT_LOG				30			/*��ʼ��LOG*/
#define ACSIREQ_LOG_STATUS				31			/*��LOG��״̬*/
#define ACSIREQ_QUERY_LOG				32			/*��ѯLog*/

#define ACSIRSP_GET_DATA				1
#define ACSIRSP_OPER_RESULT				2
#define ACSIRSP_REPORT					3
#define ACSIRSP_LOG						4
#define ACSIRSP_ABORT					5


#define MAX_REQ_GET_NAME			500
#define MAX_REQ_GET_DATA			100
#define MAX_REQ_SET_DATA			50
#define MAX_REPORT_DATA				400 /*256*/
#define MAX_REPORT_ENTRY			1
#define MAX_LOG_ENTRY				12
#define MAX_REPORT_VAR				MAX_REPORT_DATA * 3 + 16

#define ACSIWR_SUCCESS					1
#define	ACSIWR_OBJ_INVALIDATED			0
#define	ACSIWR_HW_FAULT					-1
#define	ACSIWR_TEMP_UNAVAIL				-2
#define	ACSIWR_OBJ_ACCESS_DENIED		-3
#define	ACSIWR_OBJ_UNDEFINED			-4
#define	ACSIWR_INVAL_ADDR				-5
#define	ACSIWR_TYPE_UNSUPPORTED			-6
#define	ACSIWR_TYPE_INCONSISTENT		-7
#define ACSIWR_OBJ_ATTR_INCONSISTENT	-8
#define ACSIWR_OBJ_ACC_UNSUPPORTED		-9
#define ACSIWR_OBJ_NONEXISTENT			-10


typedef enum{
	service_error_none = 0,					/* �޴��� */
	instance_not_available,					/* ʵ�������� */
	instance_in_use,						/* ʵ������ʹ�� */
	access_violation,						/* ����Υ�� */
	access_not_allowed_in_current_state,
	parameter_value_inappropriate,			/* ����ֵ������ */
	parameter_value_inconsistent,			/* ����ֵ��һ�� */
	class_not_supported,
	instance_locked_by_other_client, 		/* ʵ������һ���ͻ����� */
	control_must_be_selected,
	type_conflict,							/* ���ͳ�ͻ */
	failed_due_to_communications_constraint,/* ����ͨѶ���Ƶ���ʧ�� */
	failed_due_to_server_constraint,		/* ���ڷ��������Ƶ���ʧ�� */

/* ��չ�������ͣ�����ͬΪ��дvalue��ĳЩ����ӳ��Ϊnon_existen��ĳЩ����ӳ��Ϊundefined */
	instance_undefined						/* ʵ��δ���� */
}AcsiServiceError;



#include <sys/types.h>

typedef struct{
	time_t secs;
	unsigned int fraction;
	unsigned char qflags;
	unsigned char res[3];
}TimeStamp;

typedef struct {
	int len;
	unsigned char data[8];
}EntryId;

typedef struct{
	unsigned int size;			/* file size (# bytes)		*/
	TimeStamp mtime; 			/* last modified time		*/
	char * filename;
}FileAttrib;

#include "acsivar.h"

#ifndef bstr_copy
#define bstr_copy(dest, src, len)	(memcpy(dest, src, (len + 7) / 8))
#endif

#ifndef BSTR_BIT_SET_ON
#define BSTR_BIT_SET_ON(ptr,bitnum) \
	( ((unsigned char *)(ptr))[(bitnum)/8] |= (0x80>>((bitnum)&7)) )
#endif

#ifndef BSTR_BIT_SET_OFF
#define BSTR_BIT_SET_OFF(ptr,bitnum) \
	( ((unsigned char *)(ptr))[(bitnum)/8] &= ~(0x80>>((bitnum)&7)) )
#endif

/* BSTR_BIT_GET returns zero if bit is clear, non-zero if bit is set.	*/
/* If MUST return ONLY 0 or 1, simply add "? 1:0" to end of macro.	*/ 
#ifndef BSTR_BIT_GET
#define BSTR_BIT_GET(ptr,bitnum) \
	( ((unsigned char *)(ptr))[(bitnum)/8] &  (0x80>>((bitnum)&7)) )
#endif

#ifndef SetTimeStampSecs
#define SetTimeStampSecs(t, sec) \
	( ((TimeStamp *)t)->secs = sec )
#endif

#ifndef SetTimeStampMsec
#define SetTimeStampMsec(t, msec) \
	( ((TimeStamp *)t)->fraction = (int)((double)msec / 1000.0 * 0x00ffffff + 0.0001) )
#endif

#ifndef GetTimeStampSecs
#define GetTimeStampSecs(t) \
	( ((TimeStamp *)t)->secs )
#endif

#ifndef GetTimeStampMsec
#define GetTimeStampMsec(t) \
	( (int)((double)1000.0 * ((TimeStamp *)t)->fraction / 0x00ffffff + 0.0001) )
#endif

#ifndef GetTimeStampUsec
#define GetTimeStampUsec(t) \
	( (long)((double)1000.0 * ((TimeStamp *)t)->fraction / 0x00ffffff + 0.0001) * 1000 )
#endif

#ifndef SetTimeStampUsec
#define SetTimeStampUsec(t, usec) \
	( ((TimeStamp *)t)->fraction = (int)((double)usec / 1000000.0 * 0x00ffffff + 0.0001) )
#endif

#ifndef SetTimeStampQFlag
#define SetTimeStampQFlag(t, q) \
	( ((TimeStamp *)t)->qflags = (unsigned char)q )
#endif

#ifndef SetEntryId
#define SetEntryId(entry, size, buf) \
	{	((EntryId *)entry)->len = size;\
		if(((EntryId *)entry)->len > sizeof(((EntryId *)entry)->data)) ((EntryId *)entry)->len = sizeof(((EntryId *)entry)->data);\
		memcpy(((EntryId *)entry)->data, buf, ((EntryId *)entry)->len); }
#endif

#define TRGOP_NONE			0
#define TRGOP_DCHG			1
#define TRGOP_QCHG			2
#define TRGOP_DUPD			3
#define TRGOP_INTG			4
#define TRGOP_GI			5


#define OPTFLD_BITNUM_RESERVED		0
#define OPTFLD_BITNUM_SQNUM			1
#define OPTFLD_BITNUM_TIMESTAMP		2
#define OPTFLD_BITNUM_REASON		3
#define OPTFLD_BITNUM_DATSETNAME	4
#define OPTFLD_BITNUM_DATAREF		5
#define OPTFLD_BITNUM_BUFOVFL		6
#define OPTFLD_BITNUM_ENTRYID		7
#define OPTFLD_BITNUM_CONFREV		8
#define OPTFLD_BITNUM_SUBSEQNUM		9

#define CHECK_BITNUM_SYNCHROCH		0
#define CHECK_BITNUM_INTERLOCK		1

#define ORIGIN_NOT_SUPPORTED		0		/*OrCat��֧�֡�*/
#define ORIGIN_BAY_CONTROL			1		/*����Աͨ�������ͻ������Ŀ��Ʋ�����*/
#define ORIGIN_STATION_CONTROL		2		/*����Աͨ�����վ��ͻ������Ŀ��Ʋ�����*/
#define ORIGIN_REMOTE_CONTROL		3		/*Զ������Ա�ڱ��վ�⣨������������ģ������Ŀ��Ʋ�����*/
#define ORIGIN_AUTOMATIC_BAY		4		/*������Զ����ܷ����Ŀ��Ʋ�����*/
#define ORIGIN_AUTOMATIC_STATION	5		/*���վ���Զ����ܷ����Ŀ��Ʋ�����*/
#define ORIGIN_AUTOMATIC_REMOTE		6		/*���վ����Զ����ܷ����Ŀ��Ʋ�����*/
#define ORIGIN_MAINTENANCE			7		/*ά��/���񹤾߷����Ŀ��Ʋ�����*/
#define ORIGIN_PROCESS				8		/*�޿�����Ϊ�����ֵ�״̬��λ�����·���ⲿ��բ���ڲ����ϣ���*/

#define OPERATE_UNKNOWN					0
#define OPERATE_NOT_SUPPORTED			1
#define OPERATE_BLOCKED_BY_SWITCHING_HIERARCHY		2
#define OPERATE_SELECT_FAILED			3
#define OPERATE_INVALID_POSITION		4
#define OPERATE_POSITION_REACHED		5
#define OPERATE_PARAMETER_CHANGE_IN_EXECUTION	6
#define OPERATE_STEP_LIMIT				7
#define OPERATE_BLOCKED_BY_MODE			8
#define OPERATE_BLOCKED_BY_PROCESS		9
#define OPERATE_BLOCKED_BY_INTERLOCKING		10
#define OPERATE_BLOCKED_BY_SYNCHROCHECK		11
#define OPERATE_COMMAND_ALREADY_IN_EXECUTION	12
#define OPERATE_BLOCKED_BY_HEALTH		13
#define OPERATE_1_OF_N_CONTROL			14
#define OPERATE_ABORTION_BY_CANCEL		15
#define OPERATE_TIME_LIMIT_OVER			16
#define OPERATE_ABORTION_BY_TRIP		17
#define OPERATE_OBJECT_NOT_SELECTED		18

#define OPERATE_NO_ERROR			0
#define OPERATE_ERROR_UNKNOW		1
#define OPERATE_ERROR_TIMEOUT		2
#define OPERATE_ERROR_OPERATE		3

#endif
