/******************************************
 * File:    saddrmap.cpp
 * Author:  LuoLinglu
 * Date:    2018/04/24
 * Describe:描述61850短地址及点号对应关系
 ******************************************/

/**********************include*********************/
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <map>

using   namespace   std;

#include "switch_common.h"
#include "saddrmap.h"

#include "dci.h"
#include "dci_model.h"

/************************typedef*************************/
typedef map<string, mms_info_s> smap;

/*************static or global variables************/
static smap saddr_map;

/***********************code************************/
/****************************************
 * author: luolinglu
 * fun: 将变量名及类型注册到短地址map
*****************************************/
int smap_register_handle(smap &m, const char* var_name, int type, pfun_iec_set* p_fun)
{
	smap::iterator it;

    if(!var_name)
    {
        return SW_ERROR;
    }
    
    it = m.find(var_name);
    if(it != m.end())
    {
    	printf("fun:%s:err: re-register var '%s', sid=%d\n", __FUNCTION__, var_name, it->second.sid);
    	return SW_ERROR;
    }

    m.insert((make_pair<string, mms_info_s>)(var_name, {-1, type, p_fun}));
    //printf("fun:%s: key='%s', sid=%d\n", __FUNCTION__, var_name, m[var_name]);
    return SW_OK;
        

}

int smap_getsid_handle(smap &m, const char* var_name)
{
	smap::iterator it;

    if(!var_name)
    {
        return SW_ERROR;
    }
    
    it = m.find(var_name);
    if(it == m.end())
    {
        return SW_ERROR;
    }
    
    return it->second.sid;
}

int smap_getinfo_handle(mms_info_s *p_info, smap &m, const char* var_name)
{
	smap::iterator it;

    if(!var_name)
    {
    	printf("fun:%s:err: var_name==NULL\n", __FUNCTION__);
        return SW_ERROR;
    }

    if(!p_info)
    {
    	printf("fun:%s:err: p_info==NULL\n", __FUNCTION__);
        return SW_ERROR;
    }
    
    it = m.find(var_name);
    if(it == m.end())
    {
    	printf("fun:%s:err: can't find var_name '%s'\n", __FUNCTION__, var_name);
        return SW_ERROR;
    }

    p_info->sid = it->second.sid;
    p_info->type = it->second.type;
    
    return SW_OK;
}

int smap_link_handle(smap &m, const char* saddr, int sid)
{
	smap::iterator it;

    if(!saddr)
    {
        return SW_ERROR;
    }
    
    it = m.find(saddr);
    if(it == m.end())
    {
    	printf("fun:%s:err: ungister saddr:%s\n", __FUNCTION__, saddr);
        return SW_ERROR;
    }
    if(it->second.sid != -1)
    {
    	printf("fun:%s:err: repeatable saddr:%s\n", __FUNCTION__, saddr);
        return SW_ERROR;
    }

	it->second.sid = sid;
	printf("fun:%s: saddr:%s, sid=0x%x, type=%d\n", 
		__FUNCTION__, it->first.c_str(), it->second.sid, it->second.type);
	return SW_OK;
}

int smap_register(const char* var_name, int type, pfun_iec_set* p_fun)
{
	return smap_register_handle(saddr_map, var_name, type, p_fun);
}

int smap_establish(void)
{
	int num;
    DCI_MAP *map = NULL;
    int ret;
    int i;

    if((num = DCI_GetBioMapNum())<=0)
    {
        printf("BioErr or No BioInfo! BioNum=%d!\n",num);
    	return SW_ERROR;
    }
	printf("BioMapNum:%d\n",num);

	map = new DCI_MAP[num];
	if(!map)
	{
		printf("new DCI_MAP[%d] err!\n",num);
		return SW_ERROR;
	}
	

    if((ret = DCI_GetBioMapInfo(map, num))<0)
    {
        printf("DCI_GetBioMapInfo Err! ret=%d!\n",ret);
        delete []map;
    	return SW_ERROR;
    }
    

    for(i=0;i<num;i++)
    {
    	if(::strstr((char*)map[i].inf, "@"))
    	{
    		//printf("  saddr:%s, sid=0x%x, ignore\n", (char*)map[i].inf, map[i].sid);
    		continue;
    	}
    	ret = smap_link_handle(saddr_map, (char*)map[i].inf, map[i].sid);
    	if(ret)
    	{
    		delete []map;
    		saddr_map.clear();
    		return SW_ERROR;
    	}
    }

	delete [] map;
	return SW_OK;
    
}

void smap_release(void)
{
	printf("fun:%s called.\n", __FUNCTION__);
	saddr_map.clear();
}

int smap_get_minfo(mms_info_s *p_info, const char* var_name)
{
	return smap_getinfo_handle(p_info, saddr_map, var_name);
}



