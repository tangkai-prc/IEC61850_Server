#ifndef _ACSIOPT_INCLUDED_
#define _ACSIOPT_INCLUDED_

enum { ACSI_OPT_NONE, ACSI_OPT_DIR, ACSI_OPT_FILE, ACSI_OPT_REF, ACSI_OPT_EOC, ACSI_OPT_NUM };

/************************************************************************/
/* name: ACSI_OPT_DIR							                         */
/* type: char											                 */
/* value: ACSI_DIR_61850, ACSI_DIR_MMS	                                 */
/************************************************************************/
#define ACSI_DIR_61850		1				/* ����61850��ʽ��Ŀ¼���� */
#define ACSI_DIR_MMS		2				/* ����mms��ʽ��Ŀ¼���� */

/************************************************************************/
/* name: ACSI_OPT_FILE							                         */
/* type: char											                 */
/* value: ACSI_FILE_61850, ACSI_FILE_MMS                                 */
/************************************************************************/
#define ACSI_FILE_61850		1				/* mms�������ļ����񣬽����󴫳��� */
#define ACSI_FILE_MMS		2				/* ��mms���Զ������ļ����� */

/************************************************************************/
/* name: ACSI_OPT_REF							                         */
/* type: char											                 */
/* value: ACSI_REF_61850, ACSI_REF_MMS                                 */
/************************************************************************/
#define ACSI_REF				1				/* �ӿڲ���ACSI ��reference��ʽ */
#define MMS_REF				2				 /* �ӿڲ���MMS ��reference��ʽ */

#endif

