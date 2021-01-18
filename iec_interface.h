/******************************************
 * File:    iec_interface.h
 * Author:  LuoLinglu
 * Date:    2018/04/26
 * Fun:		headfile of iec_interface.cpp
 ******************************************/
 #ifndef __IEC_INERFACE_H__
 #define __IEC_INERFACE_H__

/************************define*************************/
#define VTOSTR(x) #x
#define VTOSTRVP(x) VTOSTR(x), &(x)

#define IEC_TYPE_BOOL 0
#define IEC_TYPE_INT8 1
#define IEC_TYPE_INT16 2
#define IEC_TYPE_INT32 3
#define IEC_TYPE_UINT8 4
#define IEC_TYPE_UINT16 5
#define IEC_TYPE_UINT32 6
#define IEC_TYPE_FLOAT32 7
#define IEC_TYPE_STRING 8
#define IEC_TYPE_STRUCT 9

#define MMS_VALUE_CASH_LEN 512
#define MMS_VALUE_CASH_OVER 500

#define IEC_TIME_QUALITY_NOTSYNC 0x2a
#define IEC_TIME_QUALITY_SYNC 0x0a

#define STG_LEN 32

/************************typedef*************************/
typedef int (*pfun_iec_set)(void);     //61850 set的回调函数原型

typedef struct mms_info
{
	int sid;
	int type;
	pfun_iec_set *value_set_fun;
	
}mms_info_s;

/**********************declarations***********************/
int start_iec_service(void);
void stop_iec_service(void);

void write_mms_values_cash(const char* name, void* p_val, int len, struct timeval time);
void send_mms_values_cash(unsigned short reason);

void mms_cash_printf_enable(void);
void mms_cash_printf_disable(void);

void mms_data_time_not_synchronized(void);
void mms_data_time_synchronized(void);

#endif
