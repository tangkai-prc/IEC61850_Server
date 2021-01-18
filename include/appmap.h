#ifndef _APP_MAP_H_
#define _APP_MAP_H_

#include "acsidef.h"
#include "typedef.h"

#ifdef WIN32
#ifdef APPMAP_EXPORTS
#define APPMAP_API __declspec(dllexport)
#else
#define APPMAP_API __declspec(dllimport)
#endif

#if !defined(APPMAP_EXPORTS)
    #pragma comment(lib,"fmap.lib")
    #pragma message("Automatically linking with fmap.dll")
#endif

#else
#define APPMAP_API
#endif

typedef struct{
	short appId;		/*	App ID	*/
	short version;		/*	版本号	*/
	int iAppSize;	/*	最大的缓冲区大小，每一条记录需要加上16字节和查找关键字的长度	*/
	int iBufTime;	/*	自动存盘缓冲时间, ms	*/
}AppMapTable;

#define APP_MAP_FILENAME		"app.log"

#define MAX_MAP_RECORD_SIZE		9216
#define MAX_MAP_KEY_SIZE		256

/*	记录句柄，可用于读取记录内容	*/
typedef void *	HAPPREC;

#ifdef __cplusplus

/*创建应用记录表，map 应用定义，num 应用数*/
APPMAP_API int AppMapCreate(AppMapTable * map, int num);

/*删除应用记录表*/
APPMAP_API void AppMapClear();

/*自动保存应用记录表，interval 调用AppMapUpdate的时间间隔，毫秒*/
APPMAP_API void AppMapUpdate(int interval);

/*保存应用记录表*/
APPMAP_API BOOL AppMapSave();

/*载入应用记录表*/
APPMAP_API BOOL AppMapLoad();

class APPMAP_API CAppMap{
public:
	CAppMap();
	CAppMap(short appId);
	~CAppMap();

/*应用初始化，appId 应用ID*/
	BOOL Init(short appId);
	
/*获得记录数*/
	int GetRecCount();

/*得到第一条记录，返回第一条记录的句柄*/
	HAPPREC GetFirstRec();

/*得到最后一条记录，返回最后一条记录的句柄*/
	HAPPREC GetLastRec();

/*得到下一条记录，，hRec 当前记录句柄，返回第一条记录的句柄*/
	HAPPREC GetNextRec(HAPPREC hRec);

/*新建一条记录，iLen 记录长度，iKeyLen 关键字长度，返回记录句柄
需要使用SetRec和SetRecKey设置记录和关键字，使用EnableAutoSave关闭自动保存	*/
	HAPPREC NewRec(int iLen, int iKeyLen = 0);

/*增加一条记录，iLen 记录长度，pKey 关键字，iKeyLen 关键字长度，返回记录句柄*/
	HAPPREC AddRec(void * pBuf, int iLen, void * pKey = NULL, int iKeyLen = 0);

/*设置记录内容，hRec 记录句柄，pBuf 缓冲区，iLen 缓冲区长度
缓冲区长度必须与记录的长度一致	*/
	BOOL SetRec(HAPPREC hRec, void * pBuf, int iLen);

/*删除记录，hRec 记录句柄，
如果不是第一条或最后一条记录，可能需要ReArrang	*/
	BOOL DelRec(HAPPREC hRec);

/*删除第一条记录，第二条记录自动变为第一条*/
	BOOL DelFirstRec();

/*删除最后一条记录*/
	BOOL DelLastRec();

/*删除所有记录*/
	void DelAllRec();

/*查找记录，pKey 关键字，iKeyLen 关键字长度，返回查找到的记录句柄*/
	HAPPREC FindRec(void * pKey, int iKeyLen);

/*得到记录关键字，hRec 记录句柄， 返回指向关键字的指针*/
	void * GetRecKey(HAPPREC hRec);

/*设置记录关键字，hRec 记录句柄， pKey 关键字，iKeyLen 关键字长度
关键字长度必须与原有关键字一致*/
	BOOL SetRecKey(HAPPREC hRec, void * pKey, int iKeyLen);

/*整理记录，回收内存碎片*/
	BOOL ReArrange();

/*保存所有记录*/
	BOOL Save();

/*使能或禁止自动保存，bEnable 是否自动保存*/
	void EnableAutoSave(BOOL bEnable);

/*通过下标访问记录，table[0] 第一条记录句柄，table[1] 第二条记录句柄，。。。*/
	HAPPREC operator[](int n);

protected:
	void * m_pTable;
};

typedef struct{
	TimeStamp t;
	EntryId entry_id;
}LogKey;

class APPMAP_API CLogMap : public CAppMap  
{
public:
	CLogMap();
	virtual ~CLogMap();

/*新建一条记录，iLen 记录长度，pKey 关键字，返回记录句柄
需要使用SetRec和SetRecKey设置记录和关键字，使用EnableAutoSave关闭自动保存	*/
	HAPPREC NewRec(int iLen, LogKey * pKey);

/*增加一条记录，iLen 记录长度，pKey 关键字，返回记录句柄*/
	HAPPREC AddRec(void * pBuf, int iLen, LogKey * pKey);

/*根据时间查找记录，返回查找到的记录句柄*/
	HAPPREC FindRec(TimeStamp& t);

/*根据entry_id查找记录，返回查找到的记录句柄*/
	HAPPREC FindRec(EntryId& entry_id);

/*设置记录关键字，hRec 记录句柄， pKey 关键字*/
	BOOL SetRecKey(HAPPREC hRec, LogKey * pKey);

/*根据开始和结束时间查找所有记录，返回查找到的记录句柄
包括开始和结束时间，使用QueryNext查找符合条件的下一条记录*/
	HAPPREC QueryByTime(TimeStamp *pStart, TimeStamp * pStop);

/*查找指定entry_id之后所有记录，返回查找到的记录句柄
包括entry_id，使用QueryNext查找符合条件的下一条记录*/
	HAPPREC QueryAfter(TimeStamp *pStart, EntryId& entry_id);

/*查找下一条记录，返回查找到的记录句柄*/
	HAPPREC QueryNext();

protected:
	TimeStamp m_start;
	TimeStamp m_stop;
	EntryId m_entry_id;
	BOOL m_bStart;
	BOOL m_bStop;
	BOOL m_bEntryId;
	HAPPREC m_hRec;
};

#endif
#endif
