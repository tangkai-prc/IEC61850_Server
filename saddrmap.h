/******************************************
 * File:    saddrmap.h
 * Author:  LuoLinglu
 * Date:    2018/04/24
 * Fun:		headfile of saddrmap.cpp
 ******************************************/

#ifndef __SADDRMAP_H__
#define __SADDRMAP_H__

#include "iec_interface.h"

/************************define*************************/
#define VSMAP_REGISTER_READ(x, y) smap_register(VTOSTR(x), (y), NULL)

/************************typedef*************************/



/**********************declarations**********************/

//extern "C" 用于在C代码中调用C++函数
#ifdef __cplusplus
extern "C"
{
#endif
int smap_register(const char* var_name, int type, pfun_iec_set* p_fun);
int smap_establish(void);
void smap_release(void);
int smap_get_minfo(mms_info_s *p_info, const char* var_name);
	
#ifdef __cplusplus
}
#endif

#endif
 