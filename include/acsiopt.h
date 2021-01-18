#ifndef _ACSIOPT_INCLUDED_
#define _ACSIOPT_INCLUDED_

enum { ACSI_OPT_NONE, ACSI_OPT_DIR, ACSI_OPT_FILE, ACSI_OPT_REF, ACSI_OPT_EOC, ACSI_OPT_NUM };

/************************************************************************/
/* name: ACSI_OPT_DIR							                         */
/* type: char											                 */
/* value: ACSI_DIR_61850, ACSI_DIR_MMS	                                 */
/************************************************************************/
#define ACSI_DIR_61850		1				/* 返回61850格式的目录名称 */
#define ACSI_DIR_MMS		2				/* 返回mms格式的目录名称 */

/************************************************************************/
/* name: ACSI_OPT_FILE							                         */
/* type: char											                 */
/* value: ACSI_FILE_61850, ACSI_FILE_MMS                                 */
/************************************************************************/
#define ACSI_FILE_61850		1				/* mms不处理文件服务，将请求传出来 */
#define ACSI_FILE_MMS		2				/* 在mms中自动处理文件服务 */

/************************************************************************/
/* name: ACSI_OPT_REF							                         */
/* type: char											                 */
/* value: ACSI_REF_61850, ACSI_REF_MMS                                 */
/************************************************************************/
#define ACSI_REF				1				/* 接口采用ACSI 的reference格式 */
#define MMS_REF				2				 /* 接口采用MMS 的reference格式 */

#endif

