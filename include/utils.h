#ifndef _UTILS_H_INCLUDE_
#define _UTILS_H_INCLUDE_

#ifdef WIN32
#ifdef UTILS_EXPORTS
#define UTILS_API __declspec(dllexport)
#else
#define UTILS_API __declspec(dllimport)
#endif

#if !defined(UTILS_EXPORTS)
    #pragma comment(lib,"libutils.lib")
    #pragma message("Automatically linking with libutils.dll")
#endif
#else
#define UTILS_API
#endif


#include "utils/algorithms.h"
#include "utils/memQ.h"
#include "utils/ringBuf.h"
#include "utils/cfg.h"
#include "utils/sys_ex.h"

#endif
