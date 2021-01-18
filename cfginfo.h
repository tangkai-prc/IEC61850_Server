/******************************************
 * File:    cfginfo.h
 * Author:  LuoLinglu
 * Date:    2018/05/11
 * Fun:		Headfile of cfginfo.c
 ******************************************/

#ifndef __CFGINFO_H__
#define __CFGINFO_H__

#include "switch_types.h"

/*************************define*************************/
#define CFG_VLAN_NUM_MAX 128
#define CFG_VLAN_PBMP_MASK 0xA0000000

#define CFG_MC_SFDB_NUM_MAX 512

#define CFG_QOS_WEIGHT 1
#define CFG_QOS_STRICT 2

#define CFG_PORTSEC_NONE 1
#define CFG_PORTSEC_STATICMAC 2

#define CFG_MAC_STATIC_NUM_MAX PORT_MAX_NUMBER

#define CFG_MC_STATIC_NUM_MAX 512

#define CFG_SNTP_UNICAST 1
#define CFG_SNTP_BROADCAST 2

#define CFG_MIRROR_MODE_OFF 1
#define CFG_MIRROR_MODE_L2 2
#define CFG_MIRROR_MIRPBMP_MASK 0x60000000
#define CFG_MIRROR_IGRPBMP_MASK 0x50000000
#define CFG_MIRROR_EGRPBMP_MASK 0x60000000

#define CFG_SNMP_VER_V1 1
#define CFG_SNMP_VER_V2C 2
#define CFG_SNMP_VER_V3 3
#define CFG_SNMP_VER_ALL 4

#define CFG_SNMPV3_NUM_MAX 2
#define CFG_SNMPV3_AUTH_NONE 1
#define CFG_SNMPV3_AUTH_MD5 2
#define CFG_SNMPV3_PRIV_NONE 1
#define CFG_SNMPV3_PRIV_CBCDES 2

#define CFG_LLDP_PORT_DISABLE 1
#define CFG_LLDP_PORT_TXONLY 2
#define CFG_LLDP_PORT_RXONLY 3
#define CFG_LLDP_PORT_TXRX 4

#define CFG_NTP_PORT_DEBUG 1
#define CFG_NTP_PORT_MMS 2

#define CFG_USER_NUM_MAX 2
#define CFG_USER_AUTHORITY_ADMIN 1
#define CFG_USER_AUTHORITY_USER 2

/**********************declarations**********************/
void init_cfginfo(void);

 
#endif
