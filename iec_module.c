/******************************************
 * File:    iec_module.c
 * Author:  LuoLinglu
 * Date:    2018/04/25
 * Fun:		获取系统相关信息
 ******************************************/

/************************include************************/
#include <stdio.h>
#include <stdlib.h>

#include "dci.h"
#include "dcidef.h"

#include "iec_interface.h"
#include "iec_module.h"

/***************static or global variables***************/
static data_upd_handle_s *mdata_upd_head = NULL;
static data_timer_handle_s *mdata_timer_head = NULL;

static data_timer_funmgmt_s *mdata_tfun_head = NULL;
static data_timer_funmgmt_s *mdata_tfun_last = NULL;
 
/**************************codes*************************/
struct data_timer_funmgmt* data_timer_funAdd(unsigned int clientreg, void *p_timer_handle);

/***************************************************
 * auth: luolinglu
 * fun: 调用各模块初始化函数，
 		函数内包括短地址注册，数据初始化注册及数据轮询定时器注册
 ***************************************************/ 
void iec_module_init(void)
{
	init_sysinfo();
	init_event_mgmt();
	init_cfginfo();
	init_port_mgmt();
}

/***************************************************
 * auth: luolinglu
 * fun: 各模块初始化数据上送
 ***************************************************/ 
void iec_data_init(void)
{
	data_upd_handle_s *p_upd;

	for(p_upd = mdata_upd_head; p_upd; p_upd = p_upd->next)
	{
		p_upd->p_fun_upd(NULL);
		send_mms_values_cash(DCI_REASON_INIT);
	}
}

/***************************************************
 * auth: luolinglu
 * fun: 启动各模块数据轮询定时器
 ***************************************************/ 
void iec_data_timer_start(void)
{
	data_timer_handle_s *p_timer;

	for(p_timer = mdata_timer_head; p_timer; p_timer = p_timer->next)
	{/*
		p_timer->alarm_handle = 
			switch_timer_register(p_timer->timer, p_timer->flags, 
			p_timer->p_fun_timer, p_timer->para);*/

        p_timer->alarm_handle = 
			switch_timer_register(p_timer->timer, p_timer->flags, 
			data_timer_funAdd, p_timer);
			
	    printf("p_timer->alarm_handle=%d\n", p_timer->alarm_handle);
	}
}


/***************************************************
 * auth: luolinglu
 * fun: 各模块数据配置变更上送
 ***************************************************/ 
void iec_data_change(switch_snmp_env_s* p_env)
{
	data_upd_handle_s *p_upd;

	for(p_upd = mdata_upd_head; p_upd; p_upd = p_upd->next)
	{
		if(p_upd->table_id == p_env->tab_id)
		{
			p_upd->p_fun_upd(NULL);
			send_mms_values_cash(DCI_REASON_DCHG|DCI_REASON_QCHG);

			//break; 	//待定
		}
	}
}


/***************************************************
 * author: luolinglu
 * fun: 接收装置告警信息并上送
***************************************************/ 
void sw_msg_rcv_cfgchange(switch_snmp_env_s* p_env)
{
	printf("fun: %s called. t_id=%d\n", __FUNCTION__, p_env->tab_id);
	iec_data_change(p_env);
}



/***************************************************
 * auth: luolinglu
 * fun: 将各模块数据更新函数注册到链表mdata_upd_head
 ***************************************************/ 
struct data_upd_handle* data_update_register(UINT32 tab_id, pfun_data_upd* p_fupd)
{
	struct data_upd_handle *p_upd = (data_upd_handle_s *)malloc(sizeof(data_upd_handle_s));

	if(!p_upd)
	{
	    printf("%s:err: malloc failed !!!\n", __FUNCTION__);
		return NULL;
	}

	p_upd->table_id = tab_id;
	p_upd->p_fun_upd = p_fupd;

	p_upd->next = mdata_upd_head;
	mdata_upd_head = p_upd;
	
	return p_upd;
}

/***************************************************
 * auth: luolinglu
 * fun: 释放数据更新函数链表mdata_upd_head
 ***************************************************/ 
void data_upd_handle_release(void)
{
	struct data_upd_handle *this, *that;

	printf("fun:%s called.\n", __FUNCTION__);

	for(this = mdata_upd_head; this; this = that)
	{
		that = this->next;
		free(this);
	}

	mdata_upd_head = NULL;
}

/***************************************************
 * auth: luolinglu
 * fun: 将各模块数据轮询函数注册到链表mdata_timer_head*
 ***************************************************/ 
struct data_timer_handle* data_timer_register(UINT32 timer, UINT32 flags, switch_timer_allback* p_ftimer, void* para)
{
	struct data_timer_handle *p_timer = (data_timer_handle_s *)malloc(sizeof(data_timer_handle_s));

	if(!p_timer)
	{
	    printf("%s:err: malloc failed !!!\n", __FUNCTION__);
		return NULL;
	}

	p_timer->timer = timer;
	p_timer->flags = flags;
	p_timer->p_fun_timer = p_ftimer;
	p_timer->para = para;

	p_timer->next = mdata_timer_head;
	mdata_timer_head = p_timer;
	
	return p_timer;
}


/***************************************************
 * auth: luolinglu
 * fun: 插入一个定时器函数并立即开始计时生效
 ***************************************************/ 
struct data_timer_handle* data_timer_effect(UINT32 timer, UINT32 flags, switch_timer_allback* p_ftimer, void* para)
{
	struct data_timer_handle* p_timer = data_timer_register(timer, flags, p_ftimer, para);

	if(p_timer)
	{
	    p_timer->alarm_handle = 
			switch_timer_register(p_timer->timer, p_timer->flags, 
			data_timer_funAdd, p_timer);
	}	
    return p_timer;
}



/***************************************************
 * auth: luolinglu
 * fun: 释放数据轮询函数链表mdata_timer_head*
 ***************************************************/ 
void data_timer_handle_release(void)
{
	struct data_timer_handle *this, *that;
	printf("fun:%s called.\n", __FUNCTION__);

	for(this = mdata_timer_head; this; this = that)
	{
		that = this->next;
		free(this);
	}

	mdata_timer_head = NULL;
}

/***************************************************
 * auth: luolinglu
 * fun: 添加一个需要执行的定时器函数(尾插法)
 ***************************************************/ 
struct data_timer_funmgmt* data_timer_funAdd(unsigned int clientreg, void *p_timer_handle)
{
    struct data_timer_funmgmt *p_mgmt = (data_timer_funmgmt_s *)malloc(sizeof(data_timer_funmgmt_s));
    data_timer_handle_s *p_timer;


    if(!p_mgmt)
    {
        printf("%s:err: malloc failed !!!\n", __FUNCTION__);
        return NULL;
    }

    p_timer = (data_timer_handle_s*)p_timer_handle;
    if(!p_timer)
    {
        printf("%s:err: p_timer==NULL !!!\n", __FUNCTION__);
        free(p_mgmt);
        return NULL;
    }
    
    p_mgmt->p_fun_timer = p_timer->p_fun_timer;
    p_mgmt->para = p_timer->para;
    p_mgmt->alarm_handle = p_timer->alarm_handle;
    p_mgmt->next = NULL;

    if(mdata_tfun_head == NULL)
    {
        mdata_tfun_head = p_mgmt;
        mdata_tfun_last = p_mgmt;
    }
    else
    {
        mdata_tfun_last ->next = p_mgmt;
        mdata_tfun_last = p_mgmt;
    }

    return p_mgmt;
}

/***************************************************
 * auth: luolinglu
 * fun: 执行一个定时器函数(从头取)
 ***************************************************/ 
INT32 data_timer_funRun(void)
{
    struct data_timer_funmgmt *p_mgmt = mdata_tfun_head;

    if(p_mgmt == NULL)
    {
        //printf("fun:%s called. p_mgmt == NULL\n", __FUNCTION__);
        return SW_ERROR;
    }

    //printf("fun:%s called. handle=%d\n", __FUNCTION__, p_mgmt->alarm_handle);


    (*(p_mgmt->p_fun_timer))(0, p_mgmt->para);
    mdata_tfun_head = p_mgmt->next;
    free(p_mgmt);
    
    if(mdata_tfun_head == NULL)
    {
        mdata_tfun_last = mdata_tfun_head;
    }
    
    return SW_OK;

}


/***************************************************
 * auth: luolinglu
 * fun: 添加一个需要执行的定时器函数(尾插法)
 ***************************************************/ 
void data_timer_funRelease(void)
{
    struct data_timer_funmgmt *this, *that;
	printf("fun:%s called.\n", __FUNCTION__);

	for(this = mdata_tfun_head; this; this = that)
	{
		that = this->next;
		free(this);
	}

	mdata_tfun_head = NULL;
	mdata_tfun_last = NULL;
}



