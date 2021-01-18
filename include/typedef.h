/****************************************************************************
*                                                                           *
* typedef.h -- Basic Type Definitions				            *
*                                                                           *
* Copyright (c) 2003 . All rights reserved.				     *
*                                                                           *
****************************************************************************/


#ifndef _TYPEDEF_H_INCLUDE_
#define _TYPEDEF_H_INCLUDE_

#include <string.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(WIN32)
#include <windows.h>
#elif defined(VXWORKS)
#include <vxworks.h>
#endif

#if !defined(WIN32) && !defined(VXWORKS)
typedef char		BOOL;
#endif

typedef char		BOOL8;

#if !defined(_WINDOWS_)

#if !defined(VXWORKS)
typedef unsigned long ULONG;
typedef ULONG *PULONG;
typedef unsigned short USHORT;
typedef USHORT *PUSHORT;
typedef unsigned char UCHAR;
typedef UCHAR *PUCHAR;
typedef char *PSZ;
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifndef UNULL
#ifdef __cplusplus
#define UNULL    (~0)
#else
#define UNULL    ((void *)(~0))
#endif
#endif

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef OPTIONAL
#define OPTIONAL
#endif

typedef long                LONG;
typedef unsigned long       DWORD;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef FLOAT               *PFLOAT;

#if !defined(VXWORKS)
typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;


#ifdef WIN32
typedef __int64				INT64;
typedef unsigned __int64	UINT64;
#else
typedef long long			INT64;
typedef unsigned long long	UINT64;
#endif
#endif

/*typedef void * HANDLE;*/
/*typedef const void * LPVOID;*/
typedef const char * LPCSTR;
typedef const char * LPCTSTR;

#ifdef MINMAX

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#endif  /* MINMAX */

#define MAKEWORD(a, b)      ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#define LOWORD(l)           ((WORD)(l))
#define HIWORD(l)           ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)           ((BYTE)(w))
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))

#endif
/*
#if defined(WIN32) || defined(VXWORKS)
typedef int	socklen_t;
#endif
*/
#define SIZEOF(n)		(int)sizeof(n)

#ifdef __cplusplus
}
#endif

enum { NS_LOW_ENDIAN, NS_HIGH_ENDIAN };

static int get_byte_endian()
{
	int i = 1;
	if(*(BYTE *)&i == 1)
		return NS_LOW_ENDIAN;
	else
		return NS_HIGH_ENDIAN;
}

#endif /* _TYPEDEF_ */
