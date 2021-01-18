/******************************************
 * File:    cfgInfo.c
 * Author:  LuoLinglu
 * Date:    2018/05/11
 * Fun:     获取配置相关信息
 ******************************************/

/**********************include*********************/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>.
#include <sys/time.h>

#include "switch_common.h" 
#include "switch_sys.h"
#include "sql_c.h"
#include "switch_msg.h"
#include "mul_timer.h"
#include "switch_qos.h"
#include "switch_log.h"
#include "switch_interface.h"

#include "dcidef.h"
#include "data_access.h"
#include "iec_module.h"
#include "saddrmap.h" 
#include "cfginfo.h"


/************************defines*************************/


/************************typedef*************************/
typedef enum
{
    N_VLAN = 0,
    N_STATIC_MAC,
    N_MC_SFDB,
    N_MC_STATIC,
    N_SNMPV3,
    
    LAST_NUM_TYPE_MAX
}last_num_e;
typedef struct 
{
    INT16 last_num[LAST_NUM_TYPE_MAX];
    
}cfg_local_s;

typedef struct 
{
    INT32 VlanId;
    INT32 PortBits;
    INT32 UntagBits;
}cfg_vlan_s;

typedef struct 
{
    INT32 GoLimSpd;
    INT32 SvLimSpd;
    UINT8 GoBpEna;
    UINT8 SvBpEna;
    UINT8 MmsEna; 
    UINT8 GooseEna;
}cfg_ieccommu_s;

typedef struct 
{
    FLOAT32 StormRst;
    UINT8 BdCtRstEna;
    UINT8 MtCtRstEna;
    UINT8 UiCtRstEna;
}cfg_grate_s;

typedef struct 
{
    UINT8 SntpClEna;
    INT32 TelgramTyp;
    char TmSrvIP[IP_STR_LEN];
    INT32 ClQuryPd;
}cfg_sntp_s;

typedef struct 
{
    INT32 MirMod;
    INT32 MirPort;
    INT32 IgrBits;
    INT32 EgrBits;
}cfg_mirror_s;

typedef struct 
{
    char ip[IP_STR_LEN];
    char netmask[IP_STR_LEN];
    char gateway[IP_STR_LEN];
    char mac[20];

}cfg_netinfo_s;

typedef struct 
{
    UINT8 SnmpEna;
    INT32 SnmpVer;
    char NmsIPAddr[IP_STR_LEN];
    char V1V2cRNam[SWITCH_NAME_MAX_LEN];
    char V1V2cRWNam[SWITCH_NAME_MAX_LEN];
}cfg_snmpgbl_s;

typedef struct 
{
    UINT8 LldpEna;
    INT32 LldpTxPd;
    INT32 LldpTxHold;
    INT32 ReInitPd;
    INT32 ReSendPd;
}cfg_lldpgbl_s;

typedef struct 
{
    UINT8 NtpSrvEna;
    INT32 EthInterf;
}cfg_ntp_s;

typedef struct 
{
    UINT8 SyslogEna;
    char SysLog[SWITCH_NAME_MAX_LEN];
    char AlmLog[SWITCH_NAME_MAX_LEN];
}cfg_syslog_s;

typedef struct 
{
    UINT8 GmrpEna;
    INT32 LeavPd;
    INT32 LeavAllPd;
    INT32 EnaGmrpPt;
}cfg_gmrp_s;

typedef struct 
{
    UINT8 RstpEna;
    INT32 RstpCftPt;
    INT32 BrgPrio;
    INT32 HelloPd;
    INT32 FordDl;
    INT32 MaxOldTm;
}cfg_stp_s;

typedef struct 
{
    char UserName[SWITCH_NAME_MAX_LEN];
    INT32 UserType;
}cfg_user_s;

/**********************declarations**********************/
void init_cfg_vlanInfo(void);
void update_vlanInfo(void);

void init_port_cfg(void);
void update_portCfgInfo(void);

void init_mcSfdb_cfg(void);
void update_mcSfdbCfgInfo(void);

void init_iecCommu_cfg(void);
void update_iecCommuCfgInfo(void);

void init_portDelay_cfg(void);
void update_portDelayCfgInfo(void);

void init_grateLmt_cfg(void);
void update_grateLmtCfgInfo(void);

void init_qos_cfg(void);
void update_qosCfgInfo(void);

void init_portSec_cfg(void);
void update_portSecCfgInfo(void);

void init_macStatic_cfg(void);
void update_macStaticCfgInfo(void);

void init_mcStatic_cfg(void);
void update_mcStaticCfgInfo(void);

void init_sntp_cfg(void);
void update_sntpCfgInfo(void);

void init_mirror_cfg(void);
void update_mirrorCfgInfo(void);

void init_pvlan_cfg(void);
void update_pvlanCfgInfo(void);

void init_ip_cfg(void);
void update_ipCfgInfo(void);

void init_snmpGbl_cfg(void);
void update_snmpGblCfgInfo(void);

void init_snmpV3_cfg(void);
void update_snmpV3CfgInfo(void);

void init_lldp_cfg(void);
void update_lldpCfgInfo(void);
void update_lldpNeCfgInfo(void);

void init_ntp_cfg(void);
void update_ntpCfgInfo(void);

void init_syslog_cfg(void);
void update_syslogCfgInfo(void);

void init_gmrp_cfg(void);
void update_gmrpCfgInfo(void);

void init_stp_cfg(void);
void update_stpCfgInfo(void);

void init_user_cfg(void);
void update_userCfgInfo(void);

/****************static or global variables**************/
static struct timeval cfg_time;
static cfg_local_s cfg_local_info = {0};

/**************************code**************************/
void init_cfginfo(void)
{
#if STD_GDNR
    init_syslog_cfg();
    init_gmrp_cfg();
    init_stp_cfg();
    init_user_cfg();
#endif

#if STD_CSG    
    init_portSec_cfg();
    init_macStatic_cfg();               
    init_ip_cfg();
    init_snmpGbl_cfg();
    init_snmpV3_cfg();
    init_lldp_cfg();
    init_ntp_cfg();
#endif

    init_cfg_vlanInfo();
    init_pvlan_cfg();
    init_qos_cfg();
    init_grateLmt_cfg();
    init_mirror_cfg(); 
    init_sntp_cfg();
    init_mcSfdb_cfg();
    init_iecCommu_cfg();
    init_portDelay_cfg();
    init_mcStatic_cfg();  
    init_port_cfg();        
}


void init_cfg_vlanInfo(void)
{
    UINT32 i;
    char varname[100] = {0};
    
    for(i = 0; i < CFG_VLAN_NUM_MAX; i++)
    {
        memset(varname, 0 , sizeof(varname));
        sprintf(varname, "cfgVlan[%03d].VlanId", i);
        smap_register(varname, IEC_TYPE_INT32, NULL);

        memset(varname, 0 , sizeof(varname));
        sprintf(varname, "cfgVlan[%03d].PortBits", i);
        smap_register(varname, IEC_TYPE_INT32, NULL);

        memset(varname, 0 , sizeof(varname));
        sprintf(varname, "cfgVlan[%03d].UntagBits", i);
        smap_register(varname, IEC_TYPE_INT32, NULL);
    }

    data_update_register(VLAN_A, update_vlanInfo);
}

/***************************************************
 * auth: luolinglu
 * fun: 获取数据库中vlan相关信息
 ***************************************************/ 
static INT32 sql_callback_get_vlancfginfo(void *p_res, int argc, char **argv, char **azColName)
{
    cfg_vlan_s vlan = {0};
    char varname[100] = {0};
    UINT32 i = *(UINT32*)p_res;

    vlan.VlanId = atoi(argv[0]);
    vlan.PortBits = cal_port_bmp(atoi(argv[1]) | atoi(argv[2]), ADC_TO_P61850) | CFG_VLAN_PBMP_MASK;
    vlan.UntagBits = cal_port_bmp(atoi(argv[2]), ADC_TO_P61850) | CFG_VLAN_PBMP_MASK;

    memset(varname, 0 , sizeof(varname));
    sprintf(varname, "cfgVlan[%03d].VlanId", i);
    write_mms_values_cash(varname, &(vlan.VlanId), sizeof(vlan.VlanId), cfg_time);

    memset(varname, 0 , sizeof(varname));
    sprintf(varname, "cfgVlan[%03d].PortBits", i);
    write_mms_values_cash(varname, &(vlan.PortBits), sizeof(vlan.PortBits), cfg_time);

    memset(varname, 0 , sizeof(varname));
    sprintf(varname, "cfgVlan[%03d].UntagBits", i);
    write_mms_values_cash(varname, &(vlan.UntagBits), sizeof(vlan.UntagBits), cfg_time);

    i++;
    *(UINT32*)p_res = i;

    return SW_OK;
}

void update_vlanInfo(void)
{
    UINT32 i = 0;
    char tabsql[100] = {0};
    INT32 num = 0;
    char varname[100] = {0};
    
    printf("fun: %s called.\n", __FUNCTION__);

    gettimeofday(&cfg_time, NULL);

    sprintf(tabsql, "select vlan_id,TagPort,UntagPort from %s order by vlan_id limit %d", 
        table_name[VLAN_A], CFG_VLAN_NUM_MAX);
    sql_exec_DB_B(VLAN_A, tabsql, sql_callback_get_vlancfginfo, &i);

    memset(tabsql, 0 , sizeof(tabsql));
    sprintf(tabsql, "select count(*) from %s", table_name[VLAN_A]);
    sql_exec_DB_B(VLAN_A, tabsql, sql_callback_get_1UINT32, &num);

    //清空已删除的静态vlan
    if(num < cfg_local_info.last_num[N_VLAN])
    {
        for(i = num; i< cfg_local_info.last_num[N_VLAN]; i++)
        { 
            memset(varname, 0, sizeof(varname));
            sprintf(varname, "cfgVlan[%03d].VlanId", i);
            write_mms_values_cash(varname, NULL, 0, cfg_time);

            memset(varname, 0, sizeof(varname));
            sprintf(varname, "cfgVlan[%03d].PortBits", i);
            write_mms_values_cash(varname, NULL, 0, cfg_time);

            memset(varname, 0, sizeof(varname));
            sprintf(varname, "cfgVlan[%03d].UntagBits", i);
            write_mms_values_cash(varname, NULL, 0, cfg_time);
        }        
    }
    cfg_local_info.last_num[N_VLAN] = 
            (num > CFG_VLAN_NUM_MAX) ? CFG_VLAN_NUM_MAX : num;

}

/***************************************************
 * auth: luolinglu
 * fun: 端口配置信息相关初始化
 ***************************************************/ 
void init_port_cfg(void)
{
    UINT32 i;
    char varname[100];  

    //短地址注册
    for(i = 0; i<PORT_MAX_NUMBER; i++)
    {

        //apst 端口统计
        memset(varname, 0, sizeof(varname));
        sprintf(varname, "portStatistics[%02d].IfDescr", i);
        smap_register(varname, IEC_TYPE_STRING, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "portStatistics[%02d].IfSpeed", i);
        smap_register(varname, IEC_TYPE_FLOAT32, NULL);

#if STD_CSG
        //ZPFR 端口流量限制
        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgPortFlowlm[%02d].InPortRte", i);
        smap_register(varname, IEC_TYPE_FLOAT32, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgPortFlowlm[%02d].InPortBuf", i);
        smap_register(varname, IEC_TYPE_FLOAT32, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgPortFlowlm[%02d].OutPortRte", i);
        smap_register(varname, IEC_TYPE_FLOAT32, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgPortFlowlm[%02d].OutPortBuf", i);
        smap_register(varname, IEC_TYPE_FLOAT32, NULL);

        //ZPFR 端口设置参数
        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgPortPara[%02d].PDescr", i);
        smap_register(varname, IEC_TYPE_STRING, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgPortPara[%02d].PEnable", i);
        smap_register(varname, IEC_TYPE_BOOL, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgPortPara[%02d].PType", i);
        smap_register(varname, IEC_TYPE_BOOL, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgPortPara[%02d].PAutoN", i);
        smap_register(varname, IEC_TYPE_BOOL, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgPortPara[%02d].PSpeed", i);
        smap_register(varname, IEC_TYPE_INT32, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgPortPara[%02d].PFDuplex", i);
        smap_register(varname, IEC_TYPE_BOOL, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgPortPara[%02d].PFLink", i);
        smap_register(varname, IEC_TYPE_BOOL, NULL);
#endif
    }

    //配置更新函数注册
    data_update_register(IF_A, update_portCfgInfo);
}

/***************************************************
 * auth: luolinglu
 * fun: 获取数据库中端口配置相关信息
 ***************************************************/ 
static INT32 sql_callback_get_portcfginfo(void *p_res, int argc, char **argv, char **azColName)
{
    char varname[100] = {0};
    struct timeval t = *(struct timeval*)p_res;
    
    UINT32 p = atoi(argv[0]); //port
    char *desc = argv[1];
    INT32 speed = atoi(argv[2]);
    FLOAT32 val_floate;
    UINT8 val_bool;
    

    if((p-1) == PORT_IS_CPU)
    {
        return SW_OK;
    }

    if(strcmp(desc, "") == 0)
    {
        desc = (speed == 1000) ? "GE" : "FE";
    }

    //apst
    memset(varname, 0 , sizeof(varname));
    sprintf(varname, "portStatistics[%02d].IfDescr", (p-1));
    write_mms_values_cash(varname, desc, strlen(desc), t);

#if STD_CSG
    //ZPFR
    val_floate = atoi(argv[3]);
    memset(varname, 0 , sizeof(varname));
    sprintf(varname, "cfgPortFlowlm[%02d].InPortRte", (p-1));
    write_mms_values_cash(varname, &val_floate, sizeof(val_floate), t);

    val_floate = atoi(argv[4]);
    memset(varname, 0 , sizeof(varname));
    sprintf(varname, "cfgPortFlowlm[%02d].InPortBuf", (p-1));
    write_mms_values_cash(varname, &val_floate, sizeof(val_floate), t);

    val_floate = atoi(argv[5]);
    memset(varname, 0 , sizeof(varname));
    sprintf(varname, "cfgPortFlowlm[%02d].OutPortRte", (p-1));
    write_mms_values_cash(varname, &val_floate, sizeof(val_floate), t);

    val_floate = atoi(argv[6]);
    memset(varname, 0 , sizeof(varname));
    sprintf(varname, "cfgPortFlowlm[%02d].OutPortBuf", (p-1));
    write_mms_values_cash(varname, &val_floate, sizeof(val_floate), t);

    //ZPFR 端口设置参数
    memset(varname, 0 , sizeof(varname));
    sprintf(varname, "cfgPortPara[%02d].PDescr", (p-1));
    write_mms_values_cash(varname, desc, strlen(desc), t);

    val_bool = (strcmp(argv[7], "enable") == 0) ? BOOL_T : BOOL_F;
    memset(varname, 0 , sizeof(varname));
    sprintf(varname, "cfgPortPara[%02d].PEnable", (p-1));
    write_mms_values_cash(varname, &val_bool, sizeof(val_bool), t);

    val_bool = (strcmp(argv[8], "copper") == 0) ? BOOL_T : BOOL_F;
    memset(varname, 0 , sizeof(varname));
    sprintf(varname, "cfgPortPara[%02d].PType", (p-1));
    write_mms_values_cash(varname, &val_bool, sizeof(val_bool), t);

    val_bool = (strcmp(argv[9], "on") == 0) ? BOOL_T : BOOL_F;
    memset(varname, 0 , sizeof(varname));
    sprintf(varname, "cfgPortPara[%02d].PAutoN", (p-1));
    write_mms_values_cash(varname, &val_bool, sizeof(val_bool), t);

    memset(varname, 0 , sizeof(varname));
    sprintf(varname, "cfgPortPara[%02d].PSpeed", (p-1));
    write_mms_values_cash(varname, &speed, sizeof(speed), t);

    val_bool = (strcmp(argv[10], "full") == 0) ? BOOL_T : BOOL_F;
    memset(varname, 0 , sizeof(varname));
    sprintf(varname, "cfgPortPara[%02d].PFDuplex", (p-1));
    write_mms_values_cash(varname, &val_bool, sizeof(val_bool), t);

    val_bool = (strcmp(argv[7], "force") == 0) ? BOOL_T : BOOL_F;
    memset(varname, 0 , sizeof(varname));
    sprintf(varname, "cfgPortPara[%02d].PFLink", (p-1));
    write_mms_values_cash(varname, &val_bool, sizeof(val_bool), t); 
#endif

    return SW_OK;
}
void update_portCfgInfo(void)
{
    char tabsql[256] = {0};
    struct timeval t;
    
    printf("fun: %s called.\n", __FUNCTION__);

    gettimeofday(&t, NULL);

    sprintf(tabsql, "select port_id,describ,speed,\
        ingress_rate,ingress_burst,egress_rate,egress_burst,\
        port_state,mode,auto_N,fullDuplex from %s order by port_id", 
        table_name[IF_A]);
    sql_exec_DB_B(IF_A, tabsql, sql_callback_get_portcfginfo, &t);

}

/***************************************************
 * auth: luolinglu
 * fun: 转发表配置信息相关初始化
 ***************************************************/ 
void init_mcSfdb_cfg(void)
{
    UINT32 i;
    char varname[100];  

    //短地址注册
    for(i = 0; i<CFG_MC_SFDB_NUM_MAX; i++)
    {
        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgSfdb[%03d].McastAddr", i);
        smap_register(varname, IEC_TYPE_STRING, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgSfdb[%03d].VlanId", i);
        smap_register(varname, IEC_TYPE_INT32, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgSfdb[%03d].APPID", i);
        smap_register(varname, IEC_TYPE_INT32, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgSfdb[%03d].PortBits", i);
        smap_register(varname, IEC_TYPE_INT32, NULL);
    }

    //配置更新函数注册
    data_update_register(L2MC_A, update_mcSfdbCfgInfo);
}

/***************************************************
 * auth: luolinglu
 * fun: 获取数据库中转发表配置相关信息
 ***************************************************/ 
static INT32 sql_callback_get_mcSfdbcfginfo(void *p_res, int argc, char **argv, char **azColName)
{
    char varname[100] = {0};
    INT32 i = atoi(argv[0]) - 1; //rowid
    INT32 val;
    struct timeval t = *(struct timeval*)p_res;

    if(argc != 5)
    {
        printf("fun:%s:err: argc=%d(r:%d)\n", __FUNCTION__, argc, 5);
        return SW_ERROR;
    }

    memset(varname, 0 , sizeof(varname));
    sprintf(varname, "cfgSfdb[%03d].McastAddr", i);
    write_mms_values_cash(varname, argv[1], strlen(argv[1]), t);

    val = atoi(argv[2]);
    memset(varname, 0 , sizeof(varname));
    sprintf(varname, "cfgSfdb[%03d].VlanId", i);
    write_mms_values_cash(varname, &val, sizeof(val), t);

    val = atoi(argv[3]);
    memset(varname, 0 , sizeof(varname));
    sprintf(varname, "cfgSfdb[%03d].APPID", i);
    write_mms_values_cash(varname, &val, sizeof(val), t);

    val = cal_port_bmp(atoi(argv[4]), ADC_TO_P61850);
    memset(varname, 0 , sizeof(varname));
    sprintf(varname, "cfgSfdb[%03d].PortBits", i);
    write_mms_values_cash(varname, &val, sizeof(val), t);


    return SW_OK;
}

/***************************************************
 * auth: luolinglu
 * fun: 更新转发表配置相关信息
 ***************************************************/ 
void update_mcSfdbCfgInfo(void)
{
    char tabsql[200] = {0};
    struct timeval t;
    INT32 num = 0, i;
    char varname[100] = {0};
    
    printf("fun: %s called.\n", __FUNCTION__);

    gettimeofday(&t, NULL);


    sprintf(tabsql, "select rowid as RowNumber,mac,vlan,appid,port_bitmap\
        from %s where appid != '' order by %s limit %d;", 
        table_name[L2MC_A], "mac", CFG_MC_SFDB_NUM_MAX);
    sql_exec_DB_B(L2MC_A, tabsql, sql_callback_get_mcSfdbcfginfo, &t);

    memset(tabsql, 0 , sizeof(tabsql));
    sprintf(tabsql, "select count(*) from %s", table_name[L2MC_A]);
    sql_exec_DB_B(L2MC_A, tabsql, sql_callback_get_1UINT32, &num);

    //清空已删除的转发表
    if(num < cfg_local_info.last_num[N_MC_SFDB])
    {
        for(i = num; i< cfg_local_info.last_num[N_MC_SFDB]; i++)
        { 
            memset(varname, 0 , sizeof(varname));
            sprintf(varname, "cfgSfdb[%03d].McastAddr", i);
            write_mms_values_cash(varname, NULL, 0, cfg_time);

            memset(varname, 0 , sizeof(varname));
            sprintf(varname, "cfgSfdb[%03d].VlanId", i);
            write_mms_values_cash(varname, NULL, 0, cfg_time);

            memset(varname, 0 , sizeof(varname));
            sprintf(varname, "cfgSfdb[%03d].APPID", i);
            write_mms_values_cash(varname, NULL, 0, cfg_time);

            memset(varname, 0 , sizeof(varname));
            sprintf(varname, "cfgSfdb[%03d].PortBits", i);
            write_mms_values_cash(varname, NULL, 0, cfg_time);
        }        
    }
    cfg_local_info.last_num[N_MC_SFDB] = 
            (num > CFG_MC_SFDB_NUM_MAX) ? CFG_MC_SFDB_NUM_MAX : num;

}


/***************************************************
 * auth: luolinglu
 * fun: iec专用通信参数配置初始化
 ***************************************************/ 
void init_iecCommu_cfg(void)
{
    cfg_ieccommu_s iecCommu = {0};

    //短地址注册
    smap_register(VTOSTR(iecCommu.GoLimSpd), IEC_TYPE_INT32, NULL);
    smap_register(VTOSTR(iecCommu.SvLimSpd), IEC_TYPE_INT32, NULL);
    smap_register(VTOSTR(iecCommu.GoBpEna), IEC_TYPE_BOOL, NULL);
    smap_register(VTOSTR(iecCommu.SvBpEna), IEC_TYPE_BOOL, NULL);
    smap_register(VTOSTR(iecCommu.MmsEna), IEC_TYPE_BOOL, NULL);
    smap_register(VTOSTR(iecCommu.GooseEna), IEC_TYPE_BOOL, NULL);

    //配置更新函数注册
    data_update_register(IEC_COMMU, update_iecCommuCfgInfo);
}

/***************************************************
 * auth: luolinglu
 * fun: 获取数据库中iec专用通信参数配置相关信息
 ***************************************************/ 
static INT32 sql_callback_get_iecCommucfginfo(void *p_res, int argc, char **argv, char **azColName)
{
    cfg_ieccommu_s iecCommu = {0};
    struct timeval t = *(struct timeval*)p_res;
    

    if(argc != WEB_IEC_COMMU_TABLE_COL)
    {
        printf("fun:%s:err: argc=%d(r:%d)\n", __FUNCTION__, argc, WEB_IEC_COMMU_TABLE_COL);
        return SW_ERROR;
    }

    iecCommu.GoLimSpd = atoi(argv[0]);
    iecCommu.SvLimSpd = atoi(argv[1]);
    iecCommu.GoBpEna = atoi(argv[2]);
    iecCommu.SvBpEna = atoi(argv[3]);
    iecCommu.MmsEna = atoi(argv[4]);
    iecCommu.GooseEna = atoi(argv[5]);

    write_mms_values_cash(VTOSTRVP(iecCommu.GoLimSpd), sizeof(iecCommu.GoLimSpd), t);
    write_mms_values_cash(VTOSTRVP(iecCommu.SvLimSpd), sizeof(iecCommu.SvLimSpd), t);
    write_mms_values_cash(VTOSTRVP(iecCommu.GoBpEna), sizeof(iecCommu.GoBpEna), t);
    write_mms_values_cash(VTOSTRVP(iecCommu.SvBpEna), sizeof(iecCommu.SvBpEna), t);
    write_mms_values_cash(VTOSTRVP(iecCommu.MmsEna), sizeof(iecCommu.MmsEna), t);
    write_mms_values_cash(VTOSTRVP(iecCommu.GooseEna), sizeof(iecCommu.GooseEna), t);

    return SW_OK;
}


/***************************************************
 * auth: luolinglu
 * fun: 更新iec专用通信参数配置相关信息
 ***************************************************/ 
void update_iecCommuCfgInfo(void)
{
    char tabsql[200] = {0};
    struct timeval t;
    
    printf("fun: %s called.\n", __FUNCTION__);

    gettimeofday(&t, NULL);

    sprintf(tabsql, "select * from  %s;", table_name[IEC_COMMU]);
    sql_exec_DB_B(IEC_COMMU, tabsql, sql_callback_get_iecCommucfginfo, &t);
}


/***************************************************
 * auth: luolinglu
 * fun: 端口延时补偿参数配置初始化
 ***************************************************/ 
void init_portDelay_cfg(void)
{
    UINT32 i;
    char varname[100];  

    //短地址注册
    for(i = 0; i<PORT_MAX_NUMBER; i++)
    {
        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgPortDelay[%02d].InDlTmms", i);
        smap_register(varname, IEC_TYPE_INT32, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgPortDelay[%02d].OutDlTmms", i);
        smap_register(varname, IEC_TYPE_INT32, NULL);
    }

    //配置更新函数注册
    data_update_register(DELAY_COMPEN, update_portDelayCfgInfo);
}

/***************************************************
 * auth: luolinglu
 * fun: 获取数据库中端口延时补偿配置相关信息
 ***************************************************/ 
static INT32 sql_callback_get_portDelaycfginfo(void *p_res, int argc, char **argv, char **azColName)
{
    char varname[100] = {0};
    INT32 port = cal_port_id(atoi(argv[0]), SQL_TO_P61850); //rowid
    INT32 val;
    struct timeval t = *(struct timeval*)p_res;

    if(argc != 3)
    {
        printf("fun:%s:err: argc=%d(r:%d)\n", __FUNCTION__, argc, 3);
        return SW_ERROR;
    }

    if((atoi(argv[0]) - 1) == PORT_IS_CPU)
    {
        return SW_OK;
    }

    val = atoi(argv[1]);
    memset(varname, 0 , sizeof(varname));
    sprintf(varname, "cfgPortDelay[%02d].InDlTmms", port-1);
    write_mms_values_cash(varname, &val, sizeof(val), t);

    val = atoi(argv[2]);
    memset(varname, 0 , sizeof(varname));
    sprintf(varname, "cfgPortDelay[%02d].OutDlTmms", port-1);
    write_mms_values_cash(varname, &val, sizeof(val), t);

    return SW_OK;
}


/***************************************************
 * auth: luolinglu
 * fun: 更新端口延时补偿配置相关信息
 ***************************************************/ 
void update_portDelayCfgInfo(void)
{
    char tabsql[200] = {0};
    struct timeval t;
    
    printf("fun: %s called.\n", __FUNCTION__);

    gettimeofday(&t, NULL);

    sprintf(tabsql, "select port_id,rx_eompen,tx_compen from %s order by %s;", 
        table_name[DELAY_COMPEN], "port_id");
    sql_exec_DB_B(DELAY_COMPEN, tabsql, sql_callback_get_portDelaycfginfo, &t);

}


/***************************************************
 * auth: luolinglu
 * fun: 网络风暴抑制参数配置初始化
 ***************************************************/ 
void init_grateLmt_cfg(void)
{
    cfg_grate_s cfgGrate = {0};

    //短地址注册
    smap_register(VTOSTR(cfgGrate.StormRst), IEC_TYPE_FLOAT32, NULL);
    smap_register(VTOSTR(cfgGrate.BdCtRstEna), IEC_TYPE_BOOL, NULL);
    smap_register(VTOSTR(cfgGrate.MtCtRstEna), IEC_TYPE_BOOL, NULL);
    smap_register(VTOSTR(cfgGrate.UiCtRstEna), IEC_TYPE_BOOL, NULL);

    //配置更新函数注册
    data_update_register(G_RATE_A, update_grateLmtCfgInfo);
}

/***************************************************
 * auth: luolinglu
 * fun: 获取数据库中网络风暴抑制参数配置相关信息
 ***************************************************/ 
static INT32 sql_callback_get_grateLimtcfginfo(void *p_res, int argc, char **argv, char **azColName)
{
    cfg_grate_s cfgGrate = {0};
    struct timeval t = *(struct timeval*)p_res;
    
    if(argc != 4)
    {
        printf("fun:%s:err: argc=%d(r:%d)\n", __FUNCTION__, argc, 4);
        return SW_ERROR;
    }
    
    cfgGrate.StormRst = ((FLOAT32)atoi(argv[0]))/62.5/1000; //南网要求单位为 62.5kbps
    cfgGrate.BdCtRstEna = (strcmp(argv[1], "on") == 0) ? BOOL_T : BOOL_F;
    cfgGrate.MtCtRstEna = (strcmp(argv[2], "on") == 0) ? BOOL_T : BOOL_F;
    cfgGrate.UiCtRstEna = (strcmp(argv[3], "on") == 0) ? BOOL_T : BOOL_F;

    write_mms_values_cash(VTOSTRVP(cfgGrate.StormRst), sizeof(cfgGrate.StormRst), t);
    write_mms_values_cash(VTOSTRVP(cfgGrate.BdCtRstEna), sizeof(cfgGrate.BdCtRstEna), t);
    write_mms_values_cash(VTOSTRVP(cfgGrate.MtCtRstEna), sizeof(cfgGrate.MtCtRstEna), t);
    write_mms_values_cash(VTOSTRVP(cfgGrate.UiCtRstEna), sizeof(cfgGrate.UiCtRstEna), t);

    return SW_OK;
}

/***************************************************
 * auth: luolinglu
 * fun: 更新网络风暴抑制参数配置相关信息
 ***************************************************/ 
void update_grateLmtCfgInfo(void)
{
    char tabsql[200] = {0};
    struct timeval t;
    
    printf("fun: %s called.\n", __FUNCTION__);

    gettimeofday(&t, NULL);

    sprintf(tabsql, "select bps_limit,bc_ena,mc_ena,dlf_ena from  %s;", table_name[G_RATE_A]);
    sql_exec_DB_B(G_RATE_A, tabsql, sql_callback_get_grateLimtcfginfo, &t);
}


/***************************************************
 * auth: luolinglu
 * fun: 优先级配置信息相关初始化
 ***************************************************/ 
void init_qos_cfg(void)
{
    UINT32 i;
    char varname[100];  

    //短地址注册
    for(i = 0; i<COS_PRI_NUM_MAX; i++)
    {
        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgQos[%02d].QuePrio", i);
        smap_register(varname, IEC_TYPE_INT32, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgQos[%02d].MsgCos", i);
        smap_register(varname, IEC_TYPE_INT32, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgQos[%02d].PrioQueSet", i);
        smap_register(varname, IEC_TYPE_INT32, NULL);
    }

    //配置更新函数注册
    data_update_register(QOS_A, update_qosCfgInfo);
}

/***************************************************
 * auth: luolinglu
 * fun: 获取数据库中优先级参数配置相关信息
 ***************************************************/ 
static INT32 sql_callback_get_qoscfginfo(void *p_res, int argc, char **argv, char **azColName)
{
    UINT32 i;
    char varname[100];  
    struct timeval t = *(struct timeval*)p_res;

    INT32 type = (strcmp("Strict", argv[0]) == 0) ? CFG_QOS_STRICT : CFG_QOS_WEIGHT;
    INT32 que_pri;

    for(i = 0; i<COS_PRI_NUM_MAX; i++)
    {
        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgQos[%02d].QuePrio", i);
        write_mms_values_cash(varname, &type, sizeof(type), t);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgQos[%02d].MsgCos", i);
        write_mms_values_cash(varname, &i, sizeof(i), t);

        que_pri = atoi(argv[i+1]);
        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgQos[%02d].PrioQueSet", i);
        write_mms_values_cash(varname, &que_pri, sizeof(que_pri), t);
    }

    return SW_OK;
}

/***************************************************
 * auth: luolinglu
 * fun: 更新优先级配置相关信息
 ***************************************************/ 
void update_qosCfgInfo(void)
{
    char tabsql[200] = {0};
    struct timeval t;
    
    printf("fun: %s called.\n", __FUNCTION__);

    gettimeofday(&t, NULL);

    sprintf(tabsql, "select * from %s;", table_name[QOS_A]);
    sql_exec_DB_B(QOS_A, tabsql, sql_callback_get_qoscfginfo, &t);
}


/***************************************************
 * auth: luolinglu
 * fun: 端口安全参数配置初始化
 ***************************************************/ 
void init_portSec_cfg(void)
{
    UINT32 i;
    char varname[100];  

    //短地址注册
    for(i = 0; i<PORT_MAX_NUMBER; i++)
    {
        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgPortSec[%02d].SecurityTp", i);
        smap_register(varname, IEC_TYPE_INT32, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgPortSec[%02d].SwMacLim", i);
        smap_register(varname, IEC_TYPE_INT32, NULL);
    }

    //配置更新函数注册
    data_update_register(PORT_SEC_A, update_portSecCfgInfo);
}

/***************************************************
 * auth: luolinglu
 * fun: 获取数据库中端口安全配置相关信息
 ***************************************************/ 
static INT32 sql_callback_get_portSeccfginfo(void *p_res, int argc, char **argv, char **azColName)
{
    char varname[100] = {0};
    INT32 port = cal_port_id(atoi(argv[0]), SQL_TO_P61850); //rowid
    INT32 val;
    struct timeval t = *(struct timeval*)p_res;

    if((atoi(argv[0]) - 1) == PORT_IS_CPU)
    {
        return SW_OK;
    }

    val = (strcmp(argv[1], "static_mac") == 0) ? CFG_PORTSEC_STATICMAC : CFG_PORTSEC_NONE;
    memset(varname, 0 , sizeof(varname));
    sprintf(varname, "cfgPortSec[%02d].SecurityTp", port-1);
    write_mms_values_cash(varname, &val, sizeof(val), t);

    val = atoi(argv[2]);
    memset(varname, 0 , sizeof(varname));
    sprintf(varname, "cfgPortSec[%02d].SwMacLim", port-1);
    write_mms_values_cash(varname, &val, sizeof(val), t);

    return SW_OK;
}


/***************************************************
 * auth: luolinglu
 * fun: 更新端口安全配置相关信息
 ***************************************************/ 
void update_portSecCfgInfo(void)
{
    char tabsql[200] = {0};
    struct timeval t;
    
    printf("fun: %s called.\n", __FUNCTION__);

    gettimeofday(&t, NULL);

    sprintf(tabsql, "select port_id,sec_mode,mac_limit from %s order by %s;", 
        table_name[PORT_SEC_A], "port_id");
    sql_exec_DB_B(PORT_SEC_A, tabsql, sql_callback_get_portSeccfginfo, &t);
}

/***************************************************
 * auth: luolinglu
 * fun: 静态MAC表配置信息相关初始化
 ***************************************************/ 
void init_macStatic_cfg(void)
{
    UINT32 i;
    char varname[100];  

    //短地址注册
    for(i = 0; i<CFG_MAC_STATIC_NUM_MAX; i++)
    {
        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgSmac[%02d].StcVLanID", i);
        smap_register(varname, IEC_TYPE_INT32, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgSmac[%02d].StaticPort", i);
        smap_register(varname, IEC_TYPE_INT32, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgSmac[%02d].ValidMac", i);
        smap_register(varname, IEC_TYPE_STRING, NULL);
        
    }

    //配置更新函数注册
    data_update_register(L2_MAC_STATIC, update_macStaticCfgInfo);
}

/***************************************************
 * auth: luolinglu
 * fun: 获取数据库中静态MAC表配置相关信息
 ***************************************************/ 
static INT32 sql_callback_get_macStaticcfginfo(void *p_res, int argc, char **argv, char **azColName)
{
    char varname[100] = {0};
    INT32 i = atoi(argv[0]) - 1; //rowid
    INT32 val;
    struct timeval t = *(struct timeval*)p_res;

    if(argc != 4)
    {
        printf("fun:%s:err: argc=%d(r:%d)\n", __FUNCTION__, argc, 4);
        return SW_ERROR;
    }

    val = atoi(argv[1]);
    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgSmac[%02d].StcVLanID", i);
    write_mms_values_cash(varname, &val, sizeof(val), t);

    val = atoi(argv[2]);
    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgSmac[%02d].StaticPort", i);
    write_mms_values_cash(varname, &val, sizeof(val), t);

    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgSmac[%02d].ValidMac", i);
    write_mms_values_cash(varname, argv[3], strlen(argv[3]), t);

    return SW_OK;
}

/***************************************************
 * auth: luolinglu
 * fun: 更新静态MAC表配置相关信息
 ***************************************************/ 
void update_macStaticCfgInfo(void)
{
    char tabsql[200] = {0};
    struct timeval t;
    INT32 num = 0, i;
    char varname[100] = {0};
    
    printf("fun: %s called.\n", __FUNCTION__);

    gettimeofday(&t, NULL);

    sprintf(tabsql, "select rowid as RowNumber,VID,port,mac_address from %s order by %s limit %d;", 
        table_name[L2_MAC_STATIC], "port", CFG_MAC_STATIC_NUM_MAX);
    sql_exec_DB_B(L2_MAC_STATIC, tabsql, sql_callback_get_macStaticcfginfo, &t);

    memset(tabsql, 0 , sizeof(tabsql));
    sprintf(tabsql, "select count(*) from %s", table_name[L2_MAC_STATIC]);
    sql_exec_DB_B(L2_MAC_STATIC, tabsql, sql_callback_get_1UINT32, &num);

    //清空已删除的静态MAC
    if(num < cfg_local_info.last_num[N_STATIC_MAC])
    {
        for(i = num; i< cfg_local_info.last_num[N_STATIC_MAC]; i++)
        {
            memset(varname, 0, sizeof(varname));
            sprintf(varname, "cfgSmac[%02d].StcVLanID", i);
            write_mms_values_cash(varname, NULL, 0, t);

            memset(varname, 0, sizeof(varname));
            sprintf(varname, "cfgSmac[%02d].StaticPort", i);
            write_mms_values_cash(varname, NULL, 0, t);

            memset(varname, 0, sizeof(varname));
            sprintf(varname, "cfgSmac[%02d].ValidMac", i);
            write_mms_values_cash(varname, NULL, 0, t);
        }        
    }
    cfg_local_info.last_num[N_STATIC_MAC] = 
            (num > CFG_MAC_STATIC_NUM_MAX) ? CFG_MAC_STATIC_NUM_MAX : num;

}


/***************************************************
 * auth: luolinglu
 * fun: 静态组播配置信息相关初始化
 ***************************************************/ 
void init_mcStatic_cfg(void)
{
    UINT32 i;
    char varname[100];  

    //短地址注册
    for(i = 0; i < CFG_MC_STATIC_NUM_MAX; i++)
    {
        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgMcs[%03d].McastAddr", i);
        smap_register(varname, IEC_TYPE_STRING, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgMcs[%03d].VlanId", i);
        smap_register(varname, IEC_TYPE_INT32, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgMcs[%03d].PortBits", i);
        smap_register(varname, IEC_TYPE_INT32, NULL);
    }

    //配置更新函数注册
    data_update_register(L2MC_A, update_mcStaticCfgInfo);
}

/***************************************************
 * auth: luolinglu
 * fun: 获取数据库中静态组播配置相关信息
 ***************************************************/ 
static INT32 sql_callback_get_mcStaticfginfo(void *p_res, int argc, char **argv, char **azColName)
{
    char varname[100] = {0};
    INT32 i = atoi(argv[0]) - 1; //rowid
    INT32 val;
    struct timeval t = *(struct timeval*)p_res;

    if(argc != 4)
    {
        printf("fun:%s:err: argc=%d(r:%d)\n", __FUNCTION__, argc, 5);
        return SW_ERROR;
    }

    memset(varname, 0 , sizeof(varname));
    sprintf(varname, "cfgMcs[%03d].McastAddr", i);
    write_mms_values_cash(varname, argv[1], strlen(argv[1]), t);

    val = atoi(argv[2]);
    memset(varname, 0 , sizeof(varname));
    sprintf(varname, "cfgMcs[%03d].VlanId", i);
    write_mms_values_cash(varname, &val, sizeof(val), t);

    val = cal_port_bmp(atoi(argv[3]), ADC_TO_P61850);
    memset(varname, 0 , sizeof(varname));
    sprintf(varname, "cfgMcs[%03d].PortBits", i);
    write_mms_values_cash(varname, &val, sizeof(val), t);


    return SW_OK;
}

/***************************************************
 * auth: luolinglu
 * fun: 更新静态组播配置相关信息
 ***************************************************/ 
void update_mcStaticCfgInfo(void)
{
    char tabsql[200] = {0};
    struct timeval t;
    char varname[100] = {0};
    int num = 0, i;
    
    printf("fun: %s called.\n", __FUNCTION__);

    gettimeofday(&t, NULL);


    sprintf(tabsql, "select rowid as RowNumber,mac,vlan,port_bitmap from %s order by %s limit %d;", 
        table_name[L2MC_A], "mac", CFG_MC_SFDB_NUM_MAX);
    sql_exec_DB_B(L2MC_A, tabsql, sql_callback_get_mcStaticfginfo, &t);

    memset(tabsql, 0 , sizeof(tabsql));
    sprintf(tabsql, "select count(*) from %s", table_name[L2MC_A]);
    sql_exec_DB_B(L2MC_A, tabsql, sql_callback_get_1UINT32, &num);

    //清空已删除的静态组播
    if(num < cfg_local_info.last_num[N_MC_STATIC])
    {
        for(i = num; i< cfg_local_info.last_num[N_MC_STATIC]; i++)
        { 
            memset(varname, 0 , sizeof(varname));
            sprintf(varname, "cfgMcs[%03d].McastAddr", i);
            write_mms_values_cash(varname, NULL, 0, cfg_time);

            memset(varname, 0 , sizeof(varname));
            sprintf(varname, "cfgMcs[%03d].VlanId", i);
            write_mms_values_cash(varname, NULL, 0, cfg_time);

            memset(varname, 0 , sizeof(varname));
            sprintf(varname, "cfgMcs[%03d].PortBits", i);
            write_mms_values_cash(varname, NULL, 0, cfg_time);
        }        
    }
    cfg_local_info.last_num[N_MC_STATIC] = 
            (num > CFG_MC_SFDB_NUM_MAX) ? CFG_MC_SFDB_NUM_MAX : num;

}


/***************************************************
 * auth: luolinglu
 * fun: sntp参数配置初始化
 ***************************************************/ 
void init_sntp_cfg(void)
{
    cfg_sntp_s cfgSntp = {0};

    //短地址注册
    smap_register(VTOSTR(cfgSntp.SntpClEna), IEC_TYPE_BOOL, NULL);
    smap_register(VTOSTR(cfgSntp.TelgramTyp), IEC_TYPE_INT32, NULL);
    smap_register(VTOSTR(cfgSntp.TmSrvIP), IEC_TYPE_STRING, NULL);
    smap_register(VTOSTR(cfgSntp.ClQuryPd), IEC_TYPE_INT32, NULL);

    //配置更新函数注册
    data_update_register(SNTP_A, update_sntpCfgInfo);
}

/***************************************************
 * auth: luolinglu
 * fun: 获取数据库中sntp参数配置相关信息
 ***************************************************/ 
static INT32 sql_callback_get_sntpcfginfo(void *p_res, int argc, char **argv, char **azColName)
{
    cfg_sntp_s cfgSntp = {0};
    struct timeval t = *(struct timeval*)p_res;
    
    if(argc != 4)
    {
        printf("fun:%s:err: argc=%d(r:%d)\n", __FUNCTION__, argc, 4);
        return SW_ERROR;
    }

    cfgSntp.SntpClEna = (strcmp(argv[0], "on") == 0) ? BOOL_T : BOOL_F;
    cfgSntp.TelgramTyp = (strcmp(argv[1], "unicast") == 0) ? CFG_SNTP_UNICAST : CFG_SNTP_BROADCAST;
    strncpy(cfgSntp.TmSrvIP, argv[2], sizeof(cfgSntp.TmSrvIP));
    cfgSntp.ClQuryPd = atoi(argv[3]);

    write_mms_values_cash(VTOSTRVP(cfgSntp.SntpClEna), sizeof(cfgSntp.SntpClEna), t);
    write_mms_values_cash(VTOSTRVP(cfgSntp.TelgramTyp), sizeof(cfgSntp.TelgramTyp), t);
    write_mms_values_cash(VTOSTRVP(cfgSntp.TmSrvIP), sizeof(cfgSntp.TmSrvIP), t);
    write_mms_values_cash(VTOSTRVP(cfgSntp.ClQuryPd), sizeof(cfgSntp.ClQuryPd), t);
    
    return SW_OK;
}


/***************************************************
 * auth: luolinglu
 * fun: 更新sntp参数配置相关信息
 ***************************************************/ 
void update_sntpCfgInfo(void)
{
    char tabsql[200] = {0};
    struct timeval t;
    
    printf("fun: %s called.\n", __FUNCTION__);

    gettimeofday(&t, NULL);

    sprintf(tabsql, "select state,telgram,server_ip,query_period from %s;", table_name[SNTP_A]);
    sql_exec_DB_B(SNTP_A, tabsql, sql_callback_get_sntpcfginfo, &t);
}


/***************************************************
 * auth: luolinglu
 * fun: mirror参数配置初始化
 ***************************************************/ 
void init_mirror_cfg(void)
{
    cfg_mirror_s cfgMirror = {0};

    //短地址注册
    smap_register(VTOSTR(cfgMirror.MirMod), IEC_TYPE_INT32, NULL);
    smap_register(VTOSTR(cfgMirror.MirPort), IEC_TYPE_INT32, NULL);
    smap_register(VTOSTR(cfgMirror.IgrBits), IEC_TYPE_INT32, NULL);
    smap_register(VTOSTR(cfgMirror.EgrBits), IEC_TYPE_INT32, NULL);

    //配置更新函数注册
    data_update_register(MIRROR_A, update_mirrorCfgInfo);
}

/***************************************************
 * auth: luolinglu
 * fun: 获取数据库中mirror参数配置相关信息
 ***************************************************/ 
static INT32 sql_callback_get_mirrorcfginfo(void *p_res, int argc, char **argv, char **azColName)
{
    cfg_mirror_s cfgMirror = {0};
    struct timeval t = *(struct timeval*)p_res;
    int i;
    
    if(argc != 4)
    {
        printf("fun:%s:err: argc=%d(r:%d)\n", __FUNCTION__, argc, 4);
        return SW_ERROR;
    }

    cfgMirror.MirMod = (strcmp(argv[0], "on") == 0) ? CFG_MIRROR_MODE_L2 : CFG_MIRROR_MODE_OFF;
    if(cfgMirror.MirMod == CFG_MIRROR_MODE_OFF)
    {
#if STD_GDNR
        cfgMirror.MirPort = CFG_MIRROR_MIRPBMP_MASK;
#endif 

#if STD_CSG
        cfgMirror.MirPort = 0;
#endif
        cfgMirror.IgrBits = CFG_MIRROR_IGRPBMP_MASK;
        cfgMirror.EgrBits = CFG_MIRROR_EGRPBMP_MASK;
    }
    else
    {
#if STD_GDNR
        cfgMirror.MirPort = CFG_MIRROR_MIRPBMP_MASK | cal_port_bmp(atoi(argv[1]), ADC_TO_P61850);
#endif

#if STD_CSG
        for(i = 0; i < PORT_MAX_NUMBER; i++)
        {
            if((atoi(argv[1]) >> i) & 0x1)  {   break;  }
        }
        cfgMirror.MirPort = i + 1;
#endif
        
        cfgMirror.IgrBits = CFG_MIRROR_IGRPBMP_MASK | cal_port_bmp(atoi(argv[2]), ADC_TO_P61850);
        cfgMirror.EgrBits = CFG_MIRROR_EGRPBMP_MASK | cal_port_bmp(atoi(argv[3]), ADC_TO_P61850);
    }

    write_mms_values_cash(VTOSTRVP(cfgMirror.MirMod), sizeof(cfgMirror.MirMod), t);
    write_mms_values_cash(VTOSTRVP(cfgMirror.MirPort), sizeof(cfgMirror.MirPort), t);
    write_mms_values_cash(VTOSTRVP(cfgMirror.IgrBits), sizeof(cfgMirror.IgrBits), t);
    write_mms_values_cash(VTOSTRVP(cfgMirror.EgrBits), sizeof(cfgMirror.EgrBits), t);
    
    return SW_OK;
}


/***************************************************
 * auth: luolinglu
 * fun: 更新mirror参数配置相关信息
 ***************************************************/ 
void update_mirrorCfgInfo(void)
{
    char tabsql[200] = {0};
    struct timeval t;
    
    printf("fun: %s called.\n", __FUNCTION__);

    gettimeofday(&t, NULL);

    sprintf(tabsql, "select * from %s;", table_name[MIRROR_A]);
    sql_exec_DB_B(MIRROR_A, tabsql, sql_callback_get_mirrorcfginfo, &t);
}


/***************************************************
 * auth: luolinglu
 * fun: pvlan配置信息相关初始化
 ***************************************************/ 
void init_pvlan_cfg(void)
{
    UINT32 i;
    char varname[100];  

    //短地址注册
    for(i = 0; i<PORT_MAX_NUMBER; i++)
    {
        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgPvlan[%02d].PortVId", i);
        smap_register(varname, IEC_TYPE_INT32, NULL);
    }

    //配置更新函数注册
    data_update_register(PVLAN_A, update_pvlanCfgInfo);
}

/***************************************************
 * auth: luolinglu
 * fun: 获取数据库中pvlan参数配置相关信息
 ***************************************************/ 
static INT32 sql_callback_get_pvlancfginfo(void *p_res, int argc, char **argv, char **azColName)
{
    UINT32 i;
    char varname[100];  
    struct timeval t = *(struct timeval*)p_res;

    INT32 port, pvlan;

    for(i = 0; i<PORT_MAX_NUMBER; i++)
    {
        if(i == PORT_IS_CPU)
        {
            continue;
        }

        port = cal_port_id(i, ADC_TO_P61850);
        pvlan = atoi(argv[i]);
        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgPvlan[%02d].PortVId", (port - 1));
        write_mms_values_cash(varname, &pvlan, sizeof(pvlan), t);
    }

    return SW_OK;
}

/***************************************************
 * auth: luolinglu
 * fun: 更新pvlan配置相关信息
 ***************************************************/ 
void update_pvlanCfgInfo(void)
{
    char tabsql[200] = {0};
    struct timeval t;
    
    printf("fun: %s called.\n", __FUNCTION__);

    gettimeofday(&t, NULL);

    sprintf(tabsql, "select * from %s;", table_name[PVLAN_A]);
    sql_exec_DB_B(PVLAN_A, tabsql, sql_callback_get_pvlancfginfo, &t);
}


/***************************************************
 * auth: luolinglu
 * fun: ip地址配置信息相关初始化
 ***************************************************/ 
void init_ip_cfg(void)
{
    char varname[100];  

    //短地址注册
    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgIP.SwIPAddr");
    smap_register(varname, IEC_TYPE_STRING, NULL);

    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgIP.SwMaskAddr");
    smap_register(varname, IEC_TYPE_STRING, NULL);

    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgIP.SwGateway");
    smap_register(varname, IEC_TYPE_STRING, NULL);

    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgIP.MAIPAddr");
    smap_register(varname, IEC_TYPE_STRING, NULL);

    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgIP.MAMaskAddr");
    smap_register(varname, IEC_TYPE_STRING, NULL);

    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgIP.MAGateway");
    smap_register(varname, IEC_TYPE_STRING, NULL);

    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgIP.MBIPAddr");
    smap_register(varname, IEC_TYPE_STRING, NULL);

    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgIP.MBMaskAddr");
    smap_register(varname, IEC_TYPE_STRING, NULL);

    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgIP.MBGateway");
    smap_register(varname, IEC_TYPE_STRING, NULL);

    //配置更新函数注册
    data_update_register(MGMT_PORT, update_ipCfgInfo);
}

/***************************************************
 * auth: luolinglu
 * fun: 获取数据库中ip地址参数配置相关信息
 ***************************************************/ 
static INT32 sql_callback_get_ipcfginfo(void *p_res, int argc, char **argv, char **azColName)
{
    char varname[100];  
    struct timeval t = *(struct timeval*)p_res;

    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgIP.MAIPAddr");
    write_mms_values_cash(varname, argv[0], strlen(argv[0]), t);

    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgIP.MAMaskAddr");
    write_mms_values_cash(varname, argv[1], strlen(argv[1]), t);

    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgIP.MAGateway");
    write_mms_values_cash(varname, argv[2], strlen(argv[2]), t);

    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgIP.MBIPAddr");
    write_mms_values_cash(varname, argv[3], strlen(argv[3]), t);

    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgIP.MBMaskAddr");
    write_mms_values_cash(varname, argv[4], strlen(argv[4]), t);

    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgIP.MBGateway");
    write_mms_values_cash(varname, argv[5], strlen(argv[5]), t);

    return SW_OK;
}

/***************************************************
 * auth: luolinglu
 * fun: 更新ip地址配置相关信息
 ***************************************************/ 
void update_ipCfgInfo(void)
{
    char tabsql[200] = {0};
    struct timeval t;
    cfg_netinfo_s dbg = {0};
    char varname[100];  
    
    printf("fun: %s called.\n", __FUNCTION__);

    gettimeofday(&t, NULL);

    get_sys_ip(dbg.ip, "eth0");
    get_sys_netmask(dbg.netmask, "eth0");
    get_gateway_by_ip(dbg.gateway, dbg.ip);

    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgIP.SwIPAddr");
    write_mms_values_cash(varname, dbg.ip, strlen(dbg.ip), t);

    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgIP.SwMaskAddr");
    write_mms_values_cash(varname, dbg.netmask, strlen(dbg.netmask), t);

    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgIP.SwGateway");
    write_mms_values_cash(varname, dbg.gateway, strlen(dbg.gateway), t);


    sprintf(tabsql, "select ip_mms_a,netmask_mms_a,gateway_mms_a,\
        ip_mms_b,netmask_mms_b,gateway_mms_b from %s;", table_name[MGMT_PORT]);
    sql_exec_DB_B(MGMT_PORT, tabsql, sql_callback_get_ipcfginfo, &t);
}


/***************************************************
 * auth: luolinglu
 * fun: snmp参数配置初始化
 ***************************************************/ 
void init_snmpGbl_cfg(void)
{
    cfg_snmpgbl_s cfgSnmpGbl = {0};

    //短地址注册
    smap_register(VTOSTR(cfgSnmpGbl.SnmpEna), IEC_TYPE_BOOL, NULL);
    smap_register(VTOSTR(cfgSnmpGbl.SnmpVer), IEC_TYPE_INT32, NULL);
    smap_register(VTOSTR(cfgSnmpGbl.NmsIPAddr), IEC_TYPE_STRING, NULL);
    smap_register(VTOSTR(cfgSnmpGbl.V1V2cRNam), IEC_TYPE_STRING, NULL);
    smap_register(VTOSTR(cfgSnmpGbl.V1V2cRWNam), IEC_TYPE_STRING, NULL);

    //配置更新函数注册
    data_update_register(SNMP_A, update_snmpGblCfgInfo);
}

/***************************************************
 * auth: luolinglu
 * fun: 获取数据库中snmp参数配置相关信息
 ***************************************************/ 
static INT32 sql_callback_get_snmpGblcfginfo(void *p_res, int argc, char **argv, char **azColName)
{
    cfg_snmpgbl_s cfgSnmpGbl = {0};
    struct timeval t = *(struct timeval*)p_res;

    cfgSnmpGbl.SnmpEna = (strcmp(argv[0], "on") == 0) ? BOOL_T : BOOL_F;
    strncpy(cfgSnmpGbl.NmsIPAddr, argv[1], IP_STR_LEN); 
    cfgSnmpGbl.SnmpVer = (atoi(argv[2]) == 0) ? CFG_SNMP_VER_ALL : atoi(argv[2]);
    strncpy(cfgSnmpGbl.V1V2cRNam, argv[3], SWITCH_NAME_MAX_LEN); 
    strncpy(cfgSnmpGbl.V1V2cRWNam, argv[4], SWITCH_NAME_MAX_LEN);

    write_mms_values_cash(VTOSTRVP(cfgSnmpGbl.SnmpEna), sizeof(cfgSnmpGbl.SnmpEna), t);
    write_mms_values_cash(VTOSTRVP(cfgSnmpGbl.SnmpVer), sizeof(cfgSnmpGbl.SnmpVer), t);

    write_mms_values_cash(VTOSTRVP(cfgSnmpGbl.NmsIPAddr), strlen(cfgSnmpGbl.NmsIPAddr), t);
    write_mms_values_cash(VTOSTRVP(cfgSnmpGbl.V1V2cRNam), strlen(cfgSnmpGbl.V1V2cRNam), t);
    write_mms_values_cash(VTOSTRVP(cfgSnmpGbl.V1V2cRWNam), strlen(cfgSnmpGbl.V1V2cRWNam), t);
    
    return SW_OK;
}


/***************************************************
 * auth: luolinglu
 * fun: 更新snmp参数配置相关信息
 ***************************************************/ 
void update_snmpGblCfgInfo(void)
{
    char tabsql[200] = {0};
    struct timeval t;
    
    printf("fun: %s called.\n", __FUNCTION__);

    gettimeofday(&t, NULL);

    sprintf(tabsql, "select * from %s;", table_name[SNMP_A]);
    sql_exec_DB_B(SNMP_A, tabsql, sql_callback_get_snmpGblcfginfo, &t);
}


/***************************************************
 * auth: luolinglu
 * fun: snmpv3配置信息相关初始化
 ***************************************************/ 
void init_snmpV3_cfg(void)
{
    UINT32 i;
    char varname[100];  

    //短地址注册
    for(i = 0; i<CFG_SNMPV3_NUM_MAX; i++)
    {
        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgSnmpV3[%02d].V3UsrNam", i);
        smap_register(varname, IEC_TYPE_STRING, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgSnmpV3[%02d].AuthPrtl", i);
        smap_register(varname, IEC_TYPE_INT32, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgSnmpV3[%02d].AuthPW", i);
        smap_register(varname, IEC_TYPE_STRING, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgSnmpV3[%02d].CfmAuthPW", i);
        smap_register(varname, IEC_TYPE_STRING, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgSnmpV3[%02d].PrvcPrtl", i);
        smap_register(varname, IEC_TYPE_INT32, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgSnmpV3[%02d].PrvcPW", i);
        smap_register(varname, IEC_TYPE_STRING, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgSnmpV3[%02d].CfmPrvcPW", i);
        smap_register(varname, IEC_TYPE_STRING, NULL);
    }

    //配置更新函数注册
    data_update_register(SNMPV3, update_snmpV3CfgInfo);
}

/***************************************************
 * auth: luolinglu
 * fun: 获取数据库中snmpv3配置相关信息
 ***************************************************/ 
static INT32 sql_callback_get_snmpV3cfginfo(void *p_res, int argc, char **argv, char **azColName)
{
    char varname[100] = {0};
    INT32 i = atoi(argv[0]) - 1; //rowid
    INT32 val;
    struct timeval t = *(struct timeval*)p_res;

    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgSnmpV3[%02d].V3UsrNam", i);
    write_mms_values_cash(varname, argv[1], strlen(argv[1]), t);

    val = (strcmp(argv[2], "no") == 0) ? CFG_SNMPV3_AUTH_NONE : CFG_SNMPV3_AUTH_MD5;
    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgSnmpV3[%02d].AuthPrtl", i);
    write_mms_values_cash(varname, &val, sizeof(val), t);

    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgSnmpV3[%02d].AuthPW", i);
    write_mms_values_cash(varname, argv[3], strlen(argv[3]), t);

    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgSnmpV3[%02d].CfmAuthPW", i);
    write_mms_values_cash(varname, argv[3], strlen(argv[3]), t);

    val = (strcmp(argv[4], "no") == 0) ? CFG_SNMPV3_PRIV_NONE : CFG_SNMPV3_PRIV_CBCDES;
    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgSnmpV3[%02d].PrvcPrtl", i);
    write_mms_values_cash(varname, &val, sizeof(val), t);

    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgSnmpV3[%02d].PrvcPW", i);
    write_mms_values_cash(varname, argv[5], strlen(argv[5]), t);

    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgSnmpV3[%02d].CfmPrvcPW", i);
    write_mms_values_cash(varname, argv[5], strlen(argv[5]), t);

    return SW_OK;
}

/***************************************************
 * auth: luolinglu
 * fun: 更新snmpv3配置相关信息
 ***************************************************/ 
void update_snmpV3CfgInfo(void)
{
    char tabsql[200] = {0};
    struct timeval t;
    int num = 0, i;
    char varname[100] = {0};
    
    printf("fun: %s called.\n", __FUNCTION__);

    gettimeofday(&t, NULL);

    sprintf(tabsql, "select rowid as RowNumber,* from %s order by %s limit %d;", 
        table_name[SNMPV3], "v3_user", CFG_SNMPV3_NUM_MAX);
    sql_exec_DB_B(SNMPV3, tabsql, sql_callback_get_snmpV3cfginfo, &t);

    memset(tabsql, 0 , sizeof(tabsql));
    sprintf(tabsql, "select count(*) from %s", table_name[SNMPV3]);
    sql_exec_DB_B(SNMPV3, tabsql, sql_callback_get_1UINT32, &num);

    //清空已删除的snmpv3
    if(num < cfg_local_info.last_num[N_SNMPV3])
    {
        for(i = num; i< cfg_local_info.last_num[N_SNMPV3]; i++)
        { 
            memset(varname, 0, sizeof(varname));
            sprintf(varname, "cfgSnmpV3[%02d].V3UsrNam", i);
            write_mms_values_cash(varname, NULL, 0, t);

            memset(varname, 0, sizeof(varname));
            sprintf(varname, "cfgSnmpV3[%02d].AuthPrtl", i);
            write_mms_values_cash(varname, NULL, 0, t);

            memset(varname, 0, sizeof(varname));
            sprintf(varname, "cfgSnmpV3[%02d].AuthPW", i);
            write_mms_values_cash(varname, NULL, 0, t);

            memset(varname, 0, sizeof(varname));
            sprintf(varname, "cfgSnmpV3[%02d].CfmAuthPW", i);
            write_mms_values_cash(varname, NULL, 0, t);

            memset(varname, 0, sizeof(varname));
            sprintf(varname, "cfgSnmpV3[%02d].PrvcPrtl", i);
            write_mms_values_cash(varname, NULL, 0, t);

            memset(varname, 0, sizeof(varname));
            sprintf(varname, "cfgSnmpV3[%02d].PrvcPW", i);
            write_mms_values_cash(varname, NULL, 0, t);

            memset(varname, 0, sizeof(varname));
            sprintf(varname, "cfgSnmpV3[%02d].CfmPrvcPW", i);
            write_mms_values_cash(varname, NULL, 0, t);
        }        
    }
    cfg_local_info.last_num[N_SNMPV3] = 
            (num > CFG_SNMPV3_NUM_MAX) ? CFG_SNMPV3_NUM_MAX : num;

}


/***************************************************
 * auth: luolinglu
 * fun: lldp参数配置初始化
 ***************************************************/ 
void init_lldp_cfg(void)
{
    cfg_lldpgbl_s cfgLldpGbl = {0};
    char varname[100] = {0};
    int i;

    //短地址注册
    smap_register(VTOSTR(cfgLldpGbl.LldpEna), IEC_TYPE_BOOL, NULL);
    smap_register(VTOSTR(cfgLldpGbl.LldpTxPd), IEC_TYPE_INT32, NULL);
    smap_register(VTOSTR(cfgLldpGbl.LldpTxHold), IEC_TYPE_INT32, NULL);
    smap_register(VTOSTR(cfgLldpGbl.ReInitPd), IEC_TYPE_INT32, NULL);
    smap_register(VTOSTR(cfgLldpGbl.ReSendPd), IEC_TYPE_INT32, NULL);

    for(i = 0; i < PORT_MAX_NUMBER; i++)
    {
        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgLldpPort[%02d].CfgPortSta", i);
        smap_register(varname, IEC_TYPE_INT32, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgLldpNebor[%02d].LlpdNeID", i);
        smap_register(varname, IEC_TYPE_STRING, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgLldpNebor[%02d].SystemDesc", i);
        smap_register(varname, IEC_TYPE_STRING, NULL);

    }

    //配置更新函数注册
    data_update_register(LLDP, update_lldpCfgInfo); 
    data_update_register(SYS_INFO_A, update_lldpNeCfgInfo);
}

/***************************************************
 * auth: luolinglu
 * fun: 获取数据库中lldp参数配置相关信息
 ***************************************************/ 
static INT32 sql_callback_get_lldpcfginfo(void *p_res, int argc, char **argv, char **azColName)
{
    cfg_lldpgbl_s cfgLldpGbl = {0};
    struct timeval t = *(struct timeval*)p_res;

    char varname[100] = {0};
    int i, bmpena, bmprx, bmptx, val = CFG_LLDP_PORT_DISABLE;

    cfgLldpGbl.LldpEna = (strcmp(argv[0], "on") == 0) ? BOOL_T : BOOL_F; //state
    cfgLldpGbl.LldpTxPd = atoi(argv[1]);    //tx_interval
    cfgLldpGbl.LldpTxHold= atoi(argv[2]);   //tx_hold
    cfgLldpGbl.ReInitPd= atoi(argv[3]);     //tx_delay  ???????
    cfgLldpGbl.ReSendPd= atoi(argv[3]);     //tx_delay  ???????

    write_mms_values_cash(VTOSTRVP(cfgLldpGbl.LldpEna), sizeof(cfgLldpGbl.LldpEna), t);
    write_mms_values_cash(VTOSTRVP(cfgLldpGbl.LldpTxPd), sizeof(cfgLldpGbl.LldpTxPd), t);
    write_mms_values_cash(VTOSTRVP(cfgLldpGbl.LldpTxHold), sizeof(cfgLldpGbl.LldpTxHold), t);
    write_mms_values_cash(VTOSTRVP(cfgLldpGbl.ReInitPd), sizeof(cfgLldpGbl.ReInitPd), t);
    write_mms_values_cash(VTOSTRVP(cfgLldpGbl.ReSendPd), sizeof(cfgLldpGbl.ReSendPd), t);

    bmpena = atoi(argv[4]);
    bmprx = atoi(argv[5]);
    bmptx = atoi(argv[6]);

    for(i = 0; i < PORT_MAX_NUMBER; i++)
    {
        if(i == PORT_IS_CPU)    {   continue;   }       
        
        if(((bmpena >> i) & 0x1) == 0)
        {
            val = CFG_LLDP_PORT_DISABLE;
        }
        else if((((bmprx >> i) & 0x1) == 1) && (((bmptx >> i) & 0x1) == 0))
        {
            val = CFG_LLDP_PORT_RXONLY;
        }
        else if((((bmprx >> i) & 0x1) == 0) && (((bmptx >> i) & 0x1) == 1))
        {
            val = CFG_LLDP_PORT_TXONLY;
        }
        else if((((bmprx >> i) & 0x1) == 1) && (((bmptx >> i) & 0x1) == 1))
        {
            val = CFG_LLDP_PORT_TXRX;
        }
        
        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgLldpPort[%02d].CfgPortSta", i);
        write_mms_values_cash(varname, &val, sizeof(val), t);
    }
    
    return SW_OK;
}


/***************************************************
 * auth: luolinglu
 * fun: 更新lldp参数配置相关信息
 ***************************************************/ 
void update_lldpCfgInfo(void)
{
    char tabsql[200] = {0};
    struct timeval t;
    int i;
    char varname[100] = {0};
    
    UINT8 mac[6] = {0};
    char macstr[20] = {0};
    
    printf("fun: %s called.\n", __FUNCTION__);

    gettimeofday(&t, NULL);

    sprintf(tabsql, "select state,tx_interval,tx_hold,tx_delay,ena_bmp,rx_bmp,tx_bmp from %s;", 
        table_name[LLDP]);
    sql_exec_DB_B(LLDP, tabsql, sql_callback_get_lldpcfginfo, &t);

}


/***************************************************
 * auth: luolinglu
 * fun: 获取数据库中lldp邻居参数配置相关信息
 ***************************************************/ 
static INT32 sql_callback_get_lldpNecfginfo(void *p_res, int argc, char **argv, char **azColName)
{
    char *desc = (char*)p_res;
    strncpy(desc, argv[0], SWITCH_NAME_MAX_LEN);
    
    return SW_OK;
}


/***************************************************
 * auth: luolinglu
 * fun: 更新lldp邻居参数配置相关信息
 ***************************************************/ 
void update_lldpNeCfgInfo(void)
{
    char tabsql[200] = {0};
    struct timeval t;
    int i;
    char varname[100] = {0};
    
    UINT8 mac[6] = {0};
    char macstr[20] = {0}, sysdesc[SWITCH_NAME_MAX_LEN] = {0};
    
    printf("fun: %s called.\n", __FUNCTION__);

    gettimeofday(&t, NULL);

    //neighbor config
    get_sys_mac(mac, "eth1");
    sprintf(macstr, "%02x:%02x:%02x:%02x:%02x:%02x",
         mac[0],mac[1],mac[2],
         mac[3],mac[4],mac[5]);
    printf("macstr: %s\n", macstr);

    memset(tabsql, 0, sizeof(tabsql));
    sprintf(tabsql, "select switch_desc from %s;", table_name[SYS_INFO_A]);
    sql_exec_DB_B(SYS_INFO_A, tabsql, sql_callback_get_lldpNecfginfo, sysdesc);
    
    for(i = 0; i < PORT_MAX_NUMBER; i++)
    {
        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgLldpNebor[%02d].LlpdNeID", i);
        write_mms_values_cash(varname, macstr, strlen(macstr), t);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgLldpNebor[%02d].SystemDesc", i);
        write_mms_values_cash(varname, sysdesc, strlen(sysdesc), t);
    }
}



/***************************************************
 * auth: luolinglu
 * fun: ntp参数配置初始化
 ***************************************************/ 
void init_ntp_cfg(void)
{
    cfg_ntp_s cfgNtp = {0};

    //短地址注册
    smap_register(VTOSTR(cfgNtp.NtpSrvEna), IEC_TYPE_BOOL, NULL);
    smap_register(VTOSTR(cfgNtp.EthInterf), IEC_TYPE_INT32, NULL);

    //配置更新函数注册
    //暂无配置，用SWITCH_WEB_TABLE_MAX代替
    data_update_register(SWITCH_WEB_TABLE_MAX, update_ntpCfgInfo); 
}

/***************************************************
 * auth: luolinglu
 * fun: 获取数据库中ntp参数配置相关信息
 ***************************************************/ 
static INT32 sql_callback_get_ntpcfginfo(void *p_res, int argc, char **argv, char **azColName)
{
    cfg_ntp_s cfgNtp = {0};
    struct timeval t = *(struct timeval*)p_res;

    
    return SW_OK;
}


/***************************************************
 * auth: luolinglu
 * fun: 更新ntp参数配置相关信息
 ***************************************************/ 
void update_ntpCfgInfo(void)
{
    char tabsql[200] = {0};
    struct timeval t;
    cfg_ntp_s cfgNtp = {0};
    
    printf("fun: %s called.\n", __FUNCTION__);

    gettimeofday(&t, NULL);

    
    //暂不支持ntp，默认禁用
    cfgNtp.NtpSrvEna = BOOL_F;
    cfgNtp.EthInterf = CFG_NTP_PORT_DEBUG;

    write_mms_values_cash(VTOSTRVP(cfgNtp.NtpSrvEna), sizeof(cfgNtp.NtpSrvEna), t);
    write_mms_values_cash(VTOSTRVP(cfgNtp.EthInterf), sizeof(cfgNtp.EthInterf), t);

    //sprintf(tabsql, "select * from %s;", table_name[SNMP_A]);
    //sql_exec_DB_B(SNMP_A, tabsql, sql_callback_get_snmpGblcfginfo, &t);
}


/***************************************************
 * auth: luolinglu
 * fun: syslog参数配置初始化
 ***************************************************/ 
void init_syslog_cfg(void)
{
    cfg_syslog_s cfgSyslog = {0};

    //短地址注册
    smap_register(VTOSTR(cfgSyslog.SyslogEna), IEC_TYPE_BOOL, NULL);
    smap_register(VTOSTR(cfgSyslog.SysLog), IEC_TYPE_STRING, NULL);
    smap_register(VTOSTR(cfgSyslog.AlmLog), IEC_TYPE_STRING, NULL);

    //配置更新函数注册
    //暂无配置，用SWITCH_WEB_TABLE_MAX代替
    data_update_register(SWITCH_WEB_TABLE_MAX, update_syslogCfgInfo); 
}

/***************************************************
 * auth: luolinglu
 * fun: 获取数据库中syslog参数配置相关信息
 ***************************************************/ 
static INT32 sql_callback_get_syslogcfginfo(void *p_res, int argc, char **argv, char **azColName)
{
    cfg_syslog_s cfgSyslog = {0};
    struct timeval t = *(struct timeval*)p_res;

    
    return SW_OK;
}


/***************************************************
 * auth: luolinglu
 * fun: 更新syslog参数配置相关信息
 ***************************************************/ 
void update_syslogCfgInfo(void)
{
    char tabsql[200] = {0};
    struct timeval t;
    cfg_syslog_s cfgSyslog = {0};;
    
    printf("fun: %s called.\n", __FUNCTION__);

    gettimeofday(&t, NULL);

    
    //暂不支持syslog，默认禁用
    cfgSyslog.SyslogEna = BOOL_F;
    strcpy(cfgSyslog.SysLog, SYS_LOG_FILE_NAME);
    strcpy(cfgSyslog.AlmLog, ALARM_LOG_FILE_NAME);

    write_mms_values_cash(VTOSTRVP(cfgSyslog.SyslogEna), sizeof(cfgSyslog.SyslogEna), t);
    write_mms_values_cash(VTOSTRVP(cfgSyslog.SysLog), strlen(cfgSyslog.SysLog), t);
    write_mms_values_cash(VTOSTRVP(cfgSyslog.AlmLog), strlen(cfgSyslog.AlmLog), t);

    //sprintf(tabsql, "select * from %s;", table_name[SNMP_A]);
    //sql_exec_DB_B(SNMP_A, tabsql, sql_callback_get_snmpGblcfginfo, &t);
}


/***************************************************
 * auth: luolinglu
 * fun: gmrp参数配置初始化
 ***************************************************/ 
void init_gmrp_cfg(void)
{
    cfg_gmrp_s cfgGmrp = {0};

    //短地址注册
    smap_register(VTOSTR(cfgGmrp.GmrpEna), IEC_TYPE_BOOL, NULL);
    smap_register(VTOSTR(cfgGmrp.LeavPd), IEC_TYPE_INT32, NULL);
    smap_register(VTOSTR(cfgGmrp.LeavAllPd), IEC_TYPE_INT32, NULL);
    smap_register(VTOSTR(cfgGmrp.EnaGmrpPt), IEC_TYPE_INT32, NULL);

    //配置更新函数注册
    data_update_register(GARP_A, update_gmrpCfgInfo); 
    data_update_register(GMRP_A, update_gmrpCfgInfo); 
}

/***************************************************
 * auth: luolinglu
 * fun: 获取数据库中gmrp参数配置相关信息
 ***************************************************/ 
static INT32 sql_callback_get_gmrpcfginfo(void *p_res, int argc, char **argv, char **azColName)
{
    cfg_gmrp_s cfgGmrp = {0};
    struct timeval t = *(struct timeval*)p_res;

    cfgGmrp.GmrpEna = (strcmp(argv[0], "on") == 0 ) ? BOOL_T : BOOL_F;
    cfgGmrp.EnaGmrpPt = cal_port_bmp(atoi(argv[1]), ADC_TO_P61850);

    write_mms_values_cash(VTOSTRVP(cfgGmrp.GmrpEna), sizeof(cfgGmrp.GmrpEna), t);
    write_mms_values_cash(VTOSTRVP(cfgGmrp.EnaGmrpPt), sizeof(cfgGmrp.EnaGmrpPt), t);
    
    return SW_OK;
}

/***************************************************
 * auth: luolinglu
 * fun: 获取数据库中garp参数配置相关信息
 ***************************************************/ 
static INT32 sql_callback_get_garpcfginfo(void *p_res, int argc, char **argv, char **azColName)
{
    cfg_gmrp_s cfgGmrp = {0};
    struct timeval t = *(struct timeval*)p_res;

    cfgGmrp.LeavPd= atoi(argv[1]) * 10; //厘秒 -> ms
    cfgGmrp.LeavAllPd= atoi(argv[2]) * 10; //厘秒 -> ms

    write_mms_values_cash(VTOSTRVP(cfgGmrp.LeavPd), sizeof(cfgGmrp.LeavPd), t);
    write_mms_values_cash(VTOSTRVP(cfgGmrp.LeavAllPd), sizeof(cfgGmrp.LeavAllPd), t);
    
    return SW_OK;
}


/***************************************************
 * auth: luolinglu
 * fun: 更新gmrp参数配置相关信息
 ***************************************************/ 
void update_gmrpCfgInfo(void)
{
    char tabsql[200] = {0};
    struct timeval t;
    
    printf("fun: %s called.\n", __FUNCTION__);

    gettimeofday(&t, NULL);
	
    sleep(1);

    sprintf(tabsql, "select * from %s;", table_name[GMRP_A]);
    sql_exec_DB_B(GMRP_A, tabsql, sql_callback_get_gmrpcfginfo, &t);

    memset(tabsql, 0 , sizeof(tabsql));
    sprintf(tabsql, "select * from %s;", table_name[GARP_A]);
    sql_exec_DB_B(GARP_A, tabsql, sql_callback_get_garpcfginfo, &t);
}


/***************************************************
 * auth: luolinglu
 * fun: stp参数配置初始化
 ***************************************************/ 
void init_stp_cfg(void)
{
    cfg_stp_s cfgStp = {0};

    //短地址注册
    smap_register(VTOSTR(cfgStp.RstpEna), IEC_TYPE_BOOL, NULL);
    smap_register(VTOSTR(cfgStp.RstpCftPt), IEC_TYPE_INT32, NULL);
    smap_register(VTOSTR(cfgStp.BrgPrio), IEC_TYPE_INT32, NULL);
    smap_register(VTOSTR(cfgStp.HelloPd), IEC_TYPE_INT32, NULL);
    smap_register(VTOSTR(cfgStp.FordDl), IEC_TYPE_INT32, NULL);
    smap_register(VTOSTR(cfgStp.MaxOldTm), IEC_TYPE_INT32, NULL);

    //配置更新函数注册
    data_update_register(RSTP_A, update_stpCfgInfo); 
}

/***************************************************
 * auth: luolinglu
 * fun: 获取数据库中stp参数配置相关信息
 ***************************************************/ 
static INT32 sql_callback_get_stpcfginfo(void *p_res, int argc, char **argv, char **azColName)
{
    cfg_stp_s cfgStp = {0};
    struct timeval t = *(struct timeval*)p_res;

    cfgStp.RstpEna = (strcmp(argv[0], "on") == 0 ) ? BOOL_T : BOOL_F;
    cfgStp.RstpCftPt = cal_port_bmp(atoi(argv[1]), ADC_TO_P61850);
    cfgStp.BrgPrio = atoi(argv[2]);
    cfgStp.HelloPd = atoi(argv[3]);
    cfgStp.FordDl = atoi(argv[4]);
    cfgStp.MaxOldTm = atoi(argv[5]);
    
    write_mms_values_cash(VTOSTRVP(cfgStp.RstpEna), sizeof(cfgStp.RstpEna), t);
    write_mms_values_cash(VTOSTRVP(cfgStp.RstpCftPt), sizeof(cfgStp.RstpCftPt), t);
    write_mms_values_cash(VTOSTRVP(cfgStp.BrgPrio), sizeof(cfgStp.BrgPrio), t);
    write_mms_values_cash(VTOSTRVP(cfgStp.HelloPd), sizeof(cfgStp.HelloPd), t);
    write_mms_values_cash(VTOSTRVP(cfgStp.FordDl), sizeof(cfgStp.FordDl), t);
    write_mms_values_cash(VTOSTRVP(cfgStp.MaxOldTm), sizeof(cfgStp.MaxOldTm), t);
    
    return SW_OK;
}

/***************************************************
 * auth: luolinglu
 * fun: 更新stp参数配置相关信息
 ***************************************************/ 
void update_stpCfgInfo(void)
{
    char tabsql[200] = {0};
    struct timeval t;
    
    printf("fun: %s called.\n", __FUNCTION__);

    gettimeofday(&t, NULL);

    sleep(1);

    sprintf(tabsql, 
        "select rstp_state,port,bridge_priotity,hello_time,forward_delay,max_age_time from %s;", 
        table_name[RSTP_A]);
    sql_exec_DB_B(RSTP_A, tabsql, sql_callback_get_stpcfginfo, &t);

}


/***************************************************
 * auth: luolinglu
 * fun: 用户管理参数配置初始化
 ***************************************************/ 
void init_user_cfg(void)
{
    INT32 i;
    char varname[100];  

    //短地址注册
    for(i = 0; i<CFG_USER_NUM_MAX; i++)
    {
        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgUser[%02d].UserName", i);
        smap_register(varname, IEC_TYPE_STRING, NULL);

        memset(varname, 0, sizeof(varname));
        sprintf(varname, "cfgUser[%02d].UserType", i);
        smap_register(varname, IEC_TYPE_INT32, NULL);
    }

    //配置更新函数注册
    data_update_register(USER_A, update_userCfgInfo); 
}

/***************************************************
 * auth: luolinglu
 * fun: 获取数据库中用户管理参数配置相关信息
 ***************************************************/ 
static INT32 sql_callback_get_usercfginfo(void *p_res, int argc, char **argv, char **azColName)
{
    char varname[100] = {0};
    INT32 i = atoi(argv[0]) - 1; //rowid
    INT32 val;
    struct timeval t = *(struct timeval*)p_res;

    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgUser[%02d].UserName", i);
    write_mms_values_cash(varname, argv[1], strlen(argv[1]), t);

    val = (strcmp(argv[2], "admin") == 0) ? CFG_USER_AUTHORITY_ADMIN : CFG_USER_AUTHORITY_USER;
    memset(varname, 0, sizeof(varname));
    sprintf(varname, "cfgUser[%02d].UserType", i);
    write_mms_values_cash(varname, &val, sizeof(val), t);

    return SW_OK;
}

/***************************************************
 * auth: luolinglu
 * fun: 更新用户管理参数配置相关信息
 ***************************************************/ 
void update_userCfgInfo(void)
{
    char tabsql[200] = {0};
    struct timeval t;
    
    printf("fun: %s called.\n", __FUNCTION__);

    gettimeofday(&t, NULL);

    sprintf(tabsql, "select (select count(*) from %s as t2 where t2.name <= t1.name) as rowNum, \
        name,authority from %s as t1 order by %s limit %d;", 
        table_name[USER_A], table_name[USER_A], "name", CFG_USER_NUM_MAX);
    sql_exec_DB_B(USER_A, tabsql, sql_callback_get_usercfginfo, &t);

}












