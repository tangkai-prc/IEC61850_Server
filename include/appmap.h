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
	short version;		/*	�汾��	*/
	int iAppSize;	/*	���Ļ�������С��ÿһ����¼��Ҫ����16�ֽںͲ��ҹؼ��ֵĳ���	*/
	int iBufTime;	/*	�Զ����̻���ʱ��, ms	*/
}AppMapTable;

#define APP_MAP_FILENAME		"app.log"

#define MAX_MAP_RECORD_SIZE		9216
#define MAX_MAP_KEY_SIZE		256

/*	��¼����������ڶ�ȡ��¼����	*/
typedef void *	HAPPREC;

#ifdef __cplusplus

/*����Ӧ�ü�¼��map Ӧ�ö��壬num Ӧ����*/
APPMAP_API int AppMapCreate(AppMapTable * map, int num);

/*ɾ��Ӧ�ü�¼��*/
APPMAP_API void AppMapClear();

/*�Զ�����Ӧ�ü�¼��interval ����AppMapUpdate��ʱ����������*/
APPMAP_API void AppMapUpdate(int interval);

/*����Ӧ�ü�¼��*/
APPMAP_API BOOL AppMapSave();

/*����Ӧ�ü�¼��*/
APPMAP_API BOOL AppMapLoad();

class APPMAP_API CAppMap{
public:
	CAppMap();
	CAppMap(short appId);
	~CAppMap();

/*Ӧ�ó�ʼ����appId Ӧ��ID*/
	BOOL Init(short appId);
	
/*��ü�¼��*/
	int GetRecCount();

/*�õ���һ����¼�����ص�һ����¼�ľ��*/
	HAPPREC GetFirstRec();

/*�õ����һ����¼���������һ����¼�ľ��*/
	HAPPREC GetLastRec();

/*�õ���һ����¼����hRec ��ǰ��¼��������ص�һ����¼�ľ��*/
	HAPPREC GetNextRec(HAPPREC hRec);

/*�½�һ����¼��iLen ��¼���ȣ�iKeyLen �ؼ��ֳ��ȣ����ؼ�¼���
��Ҫʹ��SetRec��SetRecKey���ü�¼�͹ؼ��֣�ʹ��EnableAutoSave�ر��Զ�����	*/
	HAPPREC NewRec(int iLen, int iKeyLen = 0);

/*����һ����¼��iLen ��¼���ȣ�pKey �ؼ��֣�iKeyLen �ؼ��ֳ��ȣ����ؼ�¼���*/
	HAPPREC AddRec(void * pBuf, int iLen, void * pKey = NULL, int iKeyLen = 0);

/*���ü�¼���ݣ�hRec ��¼�����pBuf ��������iLen ����������
���������ȱ������¼�ĳ���һ��	*/
	BOOL SetRec(HAPPREC hRec, void * pBuf, int iLen);

/*ɾ����¼��hRec ��¼�����
������ǵ�һ�������һ����¼��������ҪReArrang	*/
	BOOL DelRec(HAPPREC hRec);

/*ɾ����һ����¼���ڶ�����¼�Զ���Ϊ��һ��*/
	BOOL DelFirstRec();

/*ɾ�����һ����¼*/
	BOOL DelLastRec();

/*ɾ�����м�¼*/
	void DelAllRec();

/*���Ҽ�¼��pKey �ؼ��֣�iKeyLen �ؼ��ֳ��ȣ����ز��ҵ��ļ�¼���*/
	HAPPREC FindRec(void * pKey, int iKeyLen);

/*�õ���¼�ؼ��֣�hRec ��¼����� ����ָ��ؼ��ֵ�ָ��*/
	void * GetRecKey(HAPPREC hRec);

/*���ü�¼�ؼ��֣�hRec ��¼����� pKey �ؼ��֣�iKeyLen �ؼ��ֳ���
�ؼ��ֳ��ȱ�����ԭ�йؼ���һ��*/
	BOOL SetRecKey(HAPPREC hRec, void * pKey, int iKeyLen);

/*�����¼�������ڴ���Ƭ*/
	BOOL ReArrange();

/*�������м�¼*/
	BOOL Save();

/*ʹ�ܻ��ֹ�Զ����棬bEnable �Ƿ��Զ�����*/
	void EnableAutoSave(BOOL bEnable);

/*ͨ���±���ʼ�¼��table[0] ��һ����¼�����table[1] �ڶ�����¼�����������*/
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

/*�½�һ����¼��iLen ��¼���ȣ�pKey �ؼ��֣����ؼ�¼���
��Ҫʹ��SetRec��SetRecKey���ü�¼�͹ؼ��֣�ʹ��EnableAutoSave�ر��Զ�����	*/
	HAPPREC NewRec(int iLen, LogKey * pKey);

/*����һ����¼��iLen ��¼���ȣ�pKey �ؼ��֣����ؼ�¼���*/
	HAPPREC AddRec(void * pBuf, int iLen, LogKey * pKey);

/*����ʱ����Ҽ�¼�����ز��ҵ��ļ�¼���*/
	HAPPREC FindRec(TimeStamp& t);

/*����entry_id���Ҽ�¼�����ز��ҵ��ļ�¼���*/
	HAPPREC FindRec(EntryId& entry_id);

/*���ü�¼�ؼ��֣�hRec ��¼����� pKey �ؼ���*/
	BOOL SetRecKey(HAPPREC hRec, LogKey * pKey);

/*���ݿ�ʼ�ͽ���ʱ��������м�¼�����ز��ҵ��ļ�¼���
������ʼ�ͽ���ʱ�䣬ʹ��QueryNext���ҷ�����������һ����¼*/
	HAPPREC QueryByTime(TimeStamp *pStart, TimeStamp * pStop);

/*����ָ��entry_id֮�����м�¼�����ز��ҵ��ļ�¼���
����entry_id��ʹ��QueryNext���ҷ�����������һ����¼*/
	HAPPREC QueryAfter(TimeStamp *pStart, EntryId& entry_id);

/*������һ����¼�����ز��ҵ��ļ�¼���*/
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
