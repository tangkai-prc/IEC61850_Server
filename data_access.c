/******************************************
 * File:    data_access.c
 * Author:  LuoLinglu
 * Date:    2018/04/26
 * Fun:		intervace functions of iec61850
 ******************************************/

/************************include*************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h> 
#include <netinet/in.h> 


#include "switch_common.h"
#include "switch_ip_app.h"
#include "switch_msg.h"
#include "sql_c.h"
#include "mul_timer.h"
#include "switch_lldp.h"

#include "dci.h"
#include "dci_model.h"
#include "data_access.h"

#define FE_PORT_MASK 0xFFFFFF
#define GE_PORT_MASK 0x1E000000
#define CPU_PORT_MASK 0x1000000

/************************typedef*************************/


/***************static or global variables***************/
static pthread_attr_t data_attr;
static UINT32 data_updt_alarmhandle;
time_t alm_time[IEC_ALM_MAX] = {0};

/*******************Function Declaration******************/
void start_data_access_thread(void);
void data_access_main(void);
void data_update_handle(void);
void data_access_alrm_check(void);
void data_access_alrm_run(void);

/**************************code**************************/
/***************************************************
 * author: luolinglu
 * fun: 交换机数据获取相关初始化
 ***************************************************/ 
void init_data_access(void)
{
	mms_data_time_not_synchronized(); //默认系统未对时，此步骤序放在消息队列初始化之前
	switch_msg_init(SWITCH_61850_MSG_KEY);
    sw_sql_init();
    init_timer_post_config();  

    //start_data_access_thread();   //启动线程

    //注册数据更新定时器
    data_timer_register(DATA_UPDATE_INTERVAL, SA_REPEAT, data_update_handle,NULL);
	//data_updt_alarmhandle = 
	//	switch_timer_register(DATA_UPDATE_INTERVAL, SA_REPEAT, data_update_handle,NULL);

	start_data_access_thread();
}

/***************************************************
 * author: luolinglu
 * fun: 交换机数据获取相关资源释放
 ***************************************************/ 
void end_data_access(void)
{
	switch_msg_que_del();
    sw_sql_end();

    switch_timer_unregister_all();   //关闭所有定时器
}


/***************************************************
 * author: luolinglu
 * fun: 交换机数据获取线程初始化(创立线程)
 ***************************************************/ 
void start_data_access_thread(void)
{
    pthread_attr_t attr;    //线程属性结构体
    pthread_t rtid;             //线程标记

    //创建radius子线程
    pthread_attr_init(&data_attr);   //初始化 attr
    pthread_attr_setscope(&data_attr, PTHREAD_SCOPE_SYSTEM); // 设置线程作用域(系统级，与所有线程争夺CPU)
    pthread_attr_setdetachstate(&data_attr, PTHREAD_CREATE_DETACHED); // 设置线程为分离，退出时自动释放线程资源
    //pthread_create(&rtid, &data_attr, (void*)data_access_main, NULL);   //创建名为rtid的线程，功能函数为data_access_main
    pthread_create(&rtid, &data_attr, (void*)data_access_alrm_run, NULL);
    
    printf("data_access_alrm_run start.\n");
       
}

/***************************************************
 * author: luolinglu
 * fun: rev event msg
***************************************************/ 
void sw_msg_rcv_event(switch_snmp_env_s* p_env)
{
    switch(p_env->env_type)
    {
        case POWER_UP:
        case POWER_DOWN:
        case BOARD_TEMPERATURE_HIGH:
        case BOARD_TEMPERATURE_LOW:
        case BOARD_VOLTAGE_HIGH:
        case BOARD_VOLTAGE_LOW:
        case L2MAC_CHANGE:
        case SNTP_PORTALARM:
	    case SNTP_SRVALARM:
	    case SNTP_CONTALARM:
	    case SNTP_QUALITY:
			sw_msg_rcv_devalrm(p_env);
            break;

        case CFG_CHANGE:
        	sw_msg_rcv_devalrm(p_env);
        	sw_msg_rcv_cfgchange(p_env);
            break; 

        default:
            break;
    }
    return;
}


/***************************************************
 * author: luolinglu
 * fun: rev module msg from adapt
***************************************************/ 
void sw_msg_rcv_module(switch_cmd_s* p_cmd)
{
    switch(p_cmd->cmd_mod)
    {   
        case SWITCH_MOD_SYS_CFG:
            sw_msg_rcv_devinfo(p_cmd);
            break; 
            
		case SWITCH_MOD_IMS:
            sw_msg_rcv_linkalrm(p_cmd);            
            sw_msg_rcv_portsta(p_cmd);
#if STD_CSG
            sw_msg_rcv_sfpalrm(p_cmd);
#endif            
            break;
            
        default:
            return;                
	}
	return;
}



/*********************************
 * auth: luolinglu
 * fun: msg_rcv_progress
**********************************/ 
void msg_rcv_progress(void)
{
	switch_msgbuf_s msg_rcv = {0};
	
	INT32 rc = 1;    
    switch_snmp_env_s *p_env;
    switch_cmd_s *p_cmd;

    while(rc > 0)
    {
        //printf("fun: rcv msg, srckey=%d\n", msg_rcv.srckey, msg_rcv.mtext_len);
        memset(&msg_rcv, 0, sizeof(msg_rcv));
        rc = switch_msg_rcv(&msg_rcv, 200);
        if(rc >0)
        {
        	if(msg_rcv.mtext_len == sizeof(switch_cmd_s))
        	{
        		p_cmd = (switch_cmd_s*)(msg_rcv.mtext);
        		sw_msg_rcv_module(p_cmd);
        	}
        	else if(msg_rcv.mtext_len == sizeof(switch_snmp_env_s))
        	{
        		p_env = (switch_snmp_env_s*)(msg_rcv.mtext);
        		sw_msg_rcv_event(p_env); 
        	}
        	else if(msg_rcv.mtext_len == sizeof(lldp_msg_s))
        	{
        		sw_msg_rcv_portRemote((lldp_msg_s*)(msg_rcv.mtext)); 
        	}
        	else 
        	{
        		printf("fun:err: rcv msg, srckey=%d, len=%d (%d, %d, %d)\n", 
        		    msg_rcv.srckey,
        			msg_rcv.mtext_len, 
        			sizeof(switch_cmd_s), 
        			sizeof(switch_snmp_env_s),
        			sizeof(lldp_msg_s));
        	}
        }
    }
}


/*********************************************************************
 * auth: luolinglu
 * fun: data_access_main
**********************************************************************/ 
void data_access_alrm_check(void)
{    
    int i;
    time_t t;

    sleep(ALM_TIME_OUT_MAX);
    
    while(1)
    {
        t = time(NULL);
        for(i = 0; i < IEC_ALM_MAX; i++)
        {
            if((t - alm_time[i]) > ALM_TIME_OUT_MAX)
            {
                printf("fun:%s:err: alm_time[%d] time-out! t=%d, t[%d]=%d\n", 
                    __FUNCTION__, i, t, i, alm_time[i]);
                switch_timer_unregister_all();
                iec_data_timer_start();
            }
        }

        sleep(5);
    }

}



/*********************************************************************
 * auth: luolinglu
 * fun: data_access_alrm_run
**********************************************************************/ 
void data_access_alrm_run(void)
{    
    INT32 i;

    while(1)
    {
        i = SW_OK;
        while(i == SW_OK)
        {
            i = data_timer_funRun();
            usleep(50);
        } 

        usleep(100000);
    }
}

/*********************************************************************
 * auth: luolinglu
 * fun: data_access_main
**********************************************************************/ 
void data_access_main(void)
{    
    while(1)
    {
        msg_rcv_progress();
        
        usleep(500000);
    }
}

/*********************************************************************
 * auth: luolinglu
 * fun: data_update_handle
**********************************************************************/ 
void data_update_handle(void)
{    
    send_mms_values_cash(DCI_REASON_DCHG|DCI_REASON_QCHG);
    alm_time[IEC_ALM_DATA_UPDATE] = time(0);
}


 /*********************************************************************
 * func:        get_local_mac
 * Purpose:     系统get_local_mac
 * parameter:               
 * return:       success: SW_OK   failed: ERROR CODE
 **********************************************************************
int get_sys_mac(UINT8* macAdd, char *eth_name)
{
    INT32 sock_mac;

    struct ifreq ifr_mac;

    sock_mac = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_mac == -1)
    {
        perror("create socket falise...mac/n");
        return SW_ERROR;
    }

    memset(&ifr_mac,0,sizeof(ifr_mac));
    strncpy(ifr_mac.ifr_name, eth_name, sizeof(ifr_mac.ifr_name)-1);

    if((ioctl(sock_mac, SIOCGIFHWADDR, &ifr_mac)) < 0)
    {
        close(sock_mac);
        printf("mac ioctl error\n");
        return SW_ERROR;
    }
    memcpy(macAdd, ifr_mac.ifr_hwaddr.sa_data, MAC_LEN_IN_BYTE);

    close(sock_mac);
    return SW_OK;
}



/***************************************************
 * auth: luolinglu
 * fun: 根据网卡名获取系统ip地址
 ***************************************************
int get_sys_ip(char* ip, char *eth_name)
{
    INT32 sd;

    struct ifreq ifr;
    struct sockaddr_in *p_sock;

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd == -1)
    {
        perror("create socket falise...ip/n");
        return SW_ERROR;
    }

    memset(&ifr,0,sizeof(ifr));
    strncpy(ifr.ifr_name, eth_name, sizeof(ifr.ifr_name)-1);

    if((ioctl(sd, SIOCGIFADDR, &ifr)) < 0)
    {
        close(sd);
        printf("ip ioctl error\n");
        return SW_ERROR;
    }

    p_sock = (struct sockaddr_in *)(&(ifr.ifr_addr));
    printf("  eth: %s,  ip: %s\n", eth_name, inet_ntoa(p_sock->sin_addr));

    strncpy(ip, inet_ntoa(p_sock->sin_addr), IP_STR_LEN);

    close(sd);
    return SW_OK;
}

/***************************************************
 * auth: luolinglu
 * fun: 根据网卡名获取系统子网掩码
 ***************************************************
int get_sys_netmask(char* netmask, char *eth_name)
{
    INT32 sd;

    struct ifreq ifr;
    struct sockaddr_in *p_sock;

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd == -1)
    {
        perror("create socket falise...ip/n");
        return SW_ERROR;
    }

    memset(&ifr,0,sizeof(ifr));
    strncpy(ifr.ifr_name, eth_name, sizeof(ifr.ifr_name)-1);

    if((ioctl(sd, SIOCGIFNETMASK, &ifr)) < 0)
    {
        close(sd);
        printf("ip ioctl error\n");
        return SW_ERROR;
    }

    p_sock = (struct sockaddr_in *)(&(ifr.ifr_netmask));
    printf("  eth: %s,  netmask: %s\n", eth_name, inet_ntoa(p_sock->sin_addr));

    strncpy(netmask, inet_ntoa(p_sock->sin_addr), IP_STR_LEN);

    close(sd);
    return SW_OK;
}


/***************************************************
 * auth: luolinglu
 * fun: 根据ip地址获取网关
 ***************************************************
int get_gateway_by_ip(char* gateway, char *ip)
{
    INT32 intip = inet_addr(ip);
    INT32 maskip1 = inet_addr("255.255.255.0");
    INT32 maskip2 = inet_addr("0.0.0.1");
    INT32 gatew = intip & maskip1 | maskip2;

    printf("  ip: %s,   gete: %s\n", 
    	 ip, inet_ntoa(*(struct in_addr*)(&gatew)));

    strncpy(gateway, inet_ntoa(*(struct in_addr*)(&gatew)), IP_STR_LEN);

    return SW_OK;
    
}


/***************************************************
 * auth: luolinglu
 * fun: 执行数据库b相关操作
 ***************************************************/
void sql_exec_DB_B(web_table_e tab, char* tabsql, sqlite3_callback p_call, void* p_res)
{
    switch_sem_take(sql_semid[tab], 0);  //等待信号量
    //printf("tabsql: %s\n",tabsql);
    sql_ExecDatabase(sw_db[SWITCH_WEB_B], tabsql, p_call, p_res);    //执行数据库操作过程
    switch_sem_give(sql_semid[tab], 0);//释放信号量  
    
    return;    
}

/***************************************************
 * auth: luolinglu
 * fun: 给uint32某一位赋值
 ***************************************************/
UINT32 set_uint32_1bit(UINT32 bmp, UINT32 bnum, UINT32 val)
{
	UINT32 tmp = bmp;
	if(val == 0)
	{
		tmp = tmp & (~(0x1 << bnum));
	}
	else if(val == 1)
	{
		tmp = tmp | (0x1 << bnum);
	}

	return tmp;
	
}

/***************************************************
 * auth: luolinglu
 * fun: 获取uint32某一位的值
 ***************************************************/
UINT32 get_uint32_1bit(UINT32 bmp, UINT32 bnum)
{
	return (bmp >> bnum) & 0x1;	
}

/***************************************************
 * auth: luolinglu
 * fun: 换算端口id
 ***************************************************/
UINT8 cal_port_id(UINT8 port, pid_trans_e type)
{
    UINT8 rc = port;
    
    if(type == ADC_TO_P61850) 
    {
        if(port < PORT_IS_CPU)  rc = port+1;        
    }
    else if(type == P61850_TO_ADC)
    {
        if(port <= PORT_IS_CPU) rc = port-1;
    }
    else if(type == SQL_TO_P61850)
    {
    	if(port > PORT_IS_CPU) rc = port - 1;
    }
    else if(type == P61850_TO_SQL)
	{
    	if(port > PORT_IS_CPU) rc = port + 1;
    }
    
    return rc;
}

/***************************************************
 * auth: luolinglu
 * fun: 换算端口bmp
 ***************************************************/
UINT32 cal_port_bmp(UINT32 bmp, pid_trans_e type)
{
    UINT32 tmp, ret=0;

    if(type == ADC_TO_P61850)
    {
        ret = bmp;      
        tmp = (ret & GE_PORT_MASK) >>1;
        ret = (ret & FE_PORT_MASK) | tmp;   //去掉CUP端口
    }
    else if(type == P61850_TO_ADC)
    {
        ret = bmp;
        tmp = (ret <<1) & GE_PORT_MASK;
        ret = (ret & FE_PORT_MASK) | tmp; //添加CPU端口
    }
    else ret=0;

   return ret;
    
}


/***************************************************
 * author: luolinglu
 * fun: 发送消息
 * para: msg_key 消息的目的进程key
         msg_w 填写msg实体的真正函数
         para 必要时填入相关参数，传递给msg_w
 ***************************************************/ 
int sw_msg_send(UINT16 msg_key, pfun_msg_write msg_w, void* para1, void* para2)
{
    INT8 rc=SW_OK;
    UCHAR* p_val;
    UINT32 len;

    switch_msgbuf_s sw_msg_snd = {0};

    p_val=sw_msg_snd.mtext;
    
    len = msg_w(p_val, para1, para2);
    if(len<0)  return SW_ERROR;
    
    sw_msg_snd.mtext_len = len;
    
    rc=switch_msg_send_to_key(msg_key, &sw_msg_snd, SWITCH_MSG_NORMAL);
    if(rc<0)
    {
        printf("switch_msg_send_to_key failed...rc=%d, srckey=%d \n", rc, msg_key);
    }

    /*printf("fun:%s: dst_key=%d, len=%d, rc=%d\n", 
            __FUNCTION__, msg_key, len, rc);*/

    return rc;
}



/**************************************************
 * author:  luolinglu
 * func:    等待接消息返回，超时后也会自动返回
 * para:    swmsg_type 等待的消息类型
            time 等待的时间，最长时间为 time*50ms
***************************************************/
void sw_msg_wait_rcv(msg_type_e swmsg_type, UINT8 time)
{
	#if 0
    UINT8 tmp=0;

    //等待获取消息
    while(swmsg_flg.msg_flag[swmsg_type]==0)
    {
        tmp++;
        usleep(50000);
        if(tmp>time)     //接收消息超时，更新消息序列号，放弃获取消息
        {
            swmsg_flg.msg_squence[swmsg_type]=
                sw_update_squence( swmsg_flg.msg_squence[swmsg_type], SW_UINT8_MAX);
            DEBUGMSGTL(("switch", "warning: fun [%s] timeout, mode=%d. \n", __FUNCTION__ ,swmsg_type));
            break;
        }
    }
    
    //清空标志位
    swmsg_flg.msg_flag[swmsg_type]=0;    //清空标记位
    #endif
}


/***************************************************
 * auth: luolinglu
 * fun: 获取数据库中一个整数
 ***************************************************/ 
INT32 sql_callback_get_1UINT32(void *p_res, int argc, char **argv, char **azColName)
{    
	UINT32 val;

	if(argc != 1)
	{
		printf("fun:%s:err: argc=%d(r:%d)\n", __FUNCTION__, argc, 1);
		return SW_ERROR;
	}

	val = atoi(argv[0]);
	*(UINT32*)p_res = val;
    
    return SW_OK;
}



