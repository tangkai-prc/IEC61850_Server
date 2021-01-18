/******************************************
 * File:    data_access.h
 * Author:  LuoLinglu
 * Date:    2018/04/26
 * Fun:		Headfile of data_access.c
 ******************************************/
 #ifndef __DATA_ACCESS_H__
 #define __DATA_ACCESS_H__

 #include <sqlite3.h>
 #include "switch_web_datebase.h"
 #include "mul_timer.h"

/*************************defines*************************/ 

#ifdef IECCSG	//默认国网
#define STD_GDNR 0 //国网
#define STD_CSG 1	//南网
#else
#define STD_GDNR 1 //国网
#define STD_CSG 0	//南网
#endif


#define DATA_UPDATE_INTERVAL 2
#define DATA_INIT_TIME 5
#define ALM_TIME_OUT_MAX 300

#define BOOL_T 1
#define BOOL_F 0



/*************************typedef*************************/
typedef int (*pfun_msg_write)(UCHAR*, void*, void*);     //向消息中填入内容的函数指针原型

typedef enum
{
	MSG_BOARD_TEMP,
	MSG_BOARD_VOLT,
	
	MSG_TYPE_MAX
}msg_type_e;

typedef struct msg_flag
{
    UINT8 msg_flag[(msg_type_e)MSG_TYPE_MAX];
    UINT8 msg_squence[(msg_type_e)MSG_TYPE_MAX];
}msg_flg_s;

typedef enum port_id_transform
{
    ADC_TO_P61850,       // 0-28 to 1-28
    P61850_TO_ADC,        // 1-28 to 0-28
    SQL_TO_P61850,		// 1-29 to 1-28 
    P61850_TO_SQL		// 1-28 to 1-29
    
}pid_trans_e;

typedef enum
{
    IEC_ALM_DATA_UPDATE = 0,
    IEC_ALM_PORT_STATISITCS,
    IEC_ALM_UPDATE_DEVINFO,

    IEC_ALM_MAX
}iec_alm_e;


/**********************declarations************************/
extern time_t alm_time[IEC_ALM_MAX];

void init_data_access(void);
void end_data_access(void);
void msg_rcv_progress(void);

//int get_sys_mac(UINT8* macAdd, char *eth_name);
//int get_sys_ip(char* ip, char *eth_name);
//int get_sys_netmask(char* netmask, char *eth_name);
//int get_gateway_by_ip(char* gateway, char *ip);

void sql_exec_DB_B(web_table_e tab, char* tabsql, sqlite3_callback p_call, void* p_res);
UINT32 set_uint32_1bit(UINT32 bmp, UINT32 bnum, UINT32 val);
UINT32 get_uint32_1bit(UINT32 bmp, UINT32 bnum);
UINT8 cal_port_id(UINT8 port, pid_trans_e type);
UINT32 cal_port_bmp(UINT32 bmp, pid_trans_e type);

int sw_msg_send(UINT16 msg_key, pfun_msg_write msg_w, void* para1, void* para2);

INT32 sql_callback_get_1UINT32(void *p_res, int argc, char **argv, char **azColName);



#endif
