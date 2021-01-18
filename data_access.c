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
 * fun: ���������ݻ�ȡ��س�ʼ��
 ***************************************************/ 
void init_data_access(void)
{
	mms_data_time_not_synchronized(); //Ĭ��ϵͳδ��ʱ���˲����������Ϣ���г�ʼ��֮ǰ
	switch_msg_init(SWITCH_61850_MSG_KEY);
    sw_sql_init();
    init_timer_post_config();  

    //start_data_access_thread();   //�����߳�

    //ע�����ݸ��¶�ʱ��
    data_timer_register(DATA_UPDATE_INTERVAL, SA_REPEAT, data_update_handle,NULL);
	//data_updt_alarmhandle = 
	//	switch_timer_register(DATA_UPDATE_INTERVAL, SA_REPEAT, data_update_handle,NULL);

	start_data_access_thread();
}

/***************************************************
 * author: luolinglu
 * fun: ���������ݻ�ȡ�����Դ�ͷ�
 ***************************************************/ 
void end_data_access(void)
{
	switch_msg_que_del();
    sw_sql_end();

    switch_timer_unregister_all();   //�ر����ж�ʱ��
}


/***************************************************
 * author: luolinglu
 * fun: ���������ݻ�ȡ�̳߳�ʼ��(�����߳�)
 ***************************************************/ 
void start_data_access_thread(void)
{
    pthread_attr_t attr;    //�߳����Խṹ��
    pthread_t rtid;             //�̱߳��

    //����radius���߳�
    pthread_attr_init(&data_attr);   //��ʼ�� attr
    pthread_attr_setscope(&data_attr, PTHREAD_SCOPE_SYSTEM); // �����߳�������(ϵͳ�����������߳�����CPU)
    pthread_attr_setdetachstate(&data_attr, PTHREAD_CREATE_DETACHED); // �����߳�Ϊ���룬�˳�ʱ�Զ��ͷ��߳���Դ
    //pthread_create(&rtid, &data_attr, (void*)data_access_main, NULL);   //������Ϊrtid���̣߳����ܺ���Ϊdata_access_main
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
 * Purpose:     ϵͳget_local_mac
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
 * fun: ������������ȡϵͳip��ַ
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
 * fun: ������������ȡϵͳ��������
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
 * fun: ����ip��ַ��ȡ����
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
 * fun: ִ�����ݿ�b��ز���
 ***************************************************/
void sql_exec_DB_B(web_table_e tab, char* tabsql, sqlite3_callback p_call, void* p_res)
{
    switch_sem_take(sql_semid[tab], 0);  //�ȴ��ź���
    //printf("tabsql: %s\n",tabsql);
    sql_ExecDatabase(sw_db[SWITCH_WEB_B], tabsql, p_call, p_res);    //ִ�����ݿ��������
    switch_sem_give(sql_semid[tab], 0);//�ͷ��ź���  
    
    return;    
}

/***************************************************
 * auth: luolinglu
 * fun: ��uint32ĳһλ��ֵ
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
 * fun: ��ȡuint32ĳһλ��ֵ
 ***************************************************/
UINT32 get_uint32_1bit(UINT32 bmp, UINT32 bnum)
{
	return (bmp >> bnum) & 0x1;	
}

/***************************************************
 * auth: luolinglu
 * fun: ����˿�id
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
 * fun: ����˿�bmp
 ***************************************************/
UINT32 cal_port_bmp(UINT32 bmp, pid_trans_e type)
{
    UINT32 tmp, ret=0;

    if(type == ADC_TO_P61850)
    {
        ret = bmp;      
        tmp = (ret & GE_PORT_MASK) >>1;
        ret = (ret & FE_PORT_MASK) | tmp;   //ȥ��CUP�˿�
    }
    else if(type == P61850_TO_ADC)
    {
        ret = bmp;
        tmp = (ret <<1) & GE_PORT_MASK;
        ret = (ret & FE_PORT_MASK) | tmp; //���CPU�˿�
    }
    else ret=0;

   return ret;
    
}


/***************************************************
 * author: luolinglu
 * fun: ������Ϣ
 * para: msg_key ��Ϣ��Ŀ�Ľ���key
         msg_w ��дmsgʵ�����������
         para ��Ҫʱ������ز��������ݸ�msg_w
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
 * func:    �ȴ�����Ϣ���أ���ʱ��Ҳ���Զ�����
 * para:    swmsg_type �ȴ�����Ϣ����
            time �ȴ���ʱ�䣬�ʱ��Ϊ time*50ms
***************************************************/
void sw_msg_wait_rcv(msg_type_e swmsg_type, UINT8 time)
{
	#if 0
    UINT8 tmp=0;

    //�ȴ���ȡ��Ϣ
    while(swmsg_flg.msg_flag[swmsg_type]==0)
    {
        tmp++;
        usleep(50000);
        if(tmp>time)     //������Ϣ��ʱ��������Ϣ���кţ�������ȡ��Ϣ
        {
            swmsg_flg.msg_squence[swmsg_type]=
                sw_update_squence( swmsg_flg.msg_squence[swmsg_type], SW_UINT8_MAX);
            DEBUGMSGTL(("switch", "warning: fun [%s] timeout, mode=%d. \n", __FUNCTION__ ,swmsg_type));
            break;
        }
    }
    
    //��ձ�־λ
    swmsg_flg.msg_flag[swmsg_type]=0;    //��ձ��λ
    #endif
}


/***************************************************
 * auth: luolinglu
 * fun: ��ȡ���ݿ���һ������
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



