/******************************************
 * File:    iec_module.h
 * Author:  LuoLinglu
 * Date:    2018/04/26
 * Fun:		Headfile of iec_module.c
 ******************************************/

#ifndef __IEC_MODULE_H__
#define __IEC_MODULE_H__

/*************************include*************************/
#include "mul_timer.h"

/*************************define*************************/


/*************************typedef*************************/
typedef void (*pfun_data_upd)(void*);     //输出初始化函数指针原型

typedef struct data_upd_handle
{
	UINT32 table_id;
	pfun_data_upd p_fun_upd;
	struct data_upd_handle* next;
}data_upd_handle_s;

typedef struct data_timer_handle
{
	UINT32 alarm_handle;
	UINT32 timer;
	UINT32 flags;
	void* para;
	switch_timer_allback* p_fun_timer;
	
	struct data_timer_handle* next;
}data_timer_handle_s;

typedef struct data_timer_funmgmt
{
    UINT32 alarm_handle;
	switch_timer_allback* p_fun_timer;
	void* para;
	
	struct data_timer_funmgmt* next;
}data_timer_funmgmt_s;

/***********************declarations**********************/
void iec_module_init(void);
void iec_data_init(void);
void iec_data_timer_start(void);
void sw_msg_rcv_cfgchange(switch_snmp_env_s* p_env);

struct data_upd_handle* data_update_register(UINT32 tab_id, pfun_data_upd* p_fupd);
struct data_timer_handle* data_timer_register(UINT32 timer, UINT32 flags, switch_timer_allback* p_ftimer, void* para);
struct data_timer_handle* data_timer_effect(UINT32 timer, UINT32 flags, switch_timer_allback* p_ftimer, void* para);

INT32 data_timer_funRun(void);


#endif
