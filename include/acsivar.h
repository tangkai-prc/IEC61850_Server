#ifndef _ACSI_VAR_INCLUDE_
#define _ACSI_VAR_INCLUDE_

#ifdef WIN32
#ifdef ASN1_EXPORTS
#define ASN1_API __declspec(dllexport)
#else
#define ASN1_API __declspec(dllimport)
#endif

#if !defined(ASN1_EXPORTS)
    #pragma comment(lib,"asn1.lib")
    #pragma message("Automatically linking with asn1.dll")
#endif

#else
#define ASN1_API
#endif

#include "acsidef.h"
#include <string.h>

#define INVALID_MMSTYPE		((short)0xffff)

#ifndef ABS
#define ABS(n)	(n < 0 ? -n : n)
#endif

typedef struct{
	short size;
	short mms_type;
	short mms_size;
	short var_type;		/*BaseDaType*/
	short var_size;		/*数据宽度*/
	short name_len;
	union{
		char b;					/*BOOL8*/
		int i;					/*char, short, int 均转为int*/
		unsigned int ui;		/*BYTE, WORD, DWORD 均转为unsigned int*/
		INT64 iL;
		double f;				/*float, double 均转为double*/
		unsigned char bstr[4];		/*bitstring*/
		unsigned char oct[64];		/*octstring*/
		char str[4];				/*string*/
		unsigned char utf8str[4];	/*UTF8 string*/
		TimeStamp t;
		unsigned char data[300];
	}un;
} AcsiVariant;

typedef struct{
	short size;
	short mms_type;
	short mms_size;
	short var_type;		/*BaseDaType*/
	short var_size;		/*数据宽度*/
	short name_len;
	union{
		char b;					/*BOOL8*/
		int i;					/*char, short, int 均转为int*/
		unsigned int ui;		/*BYTE, WORD, DWORD 均转为unsigned int*/
		INT64 iL;
		double f;				/*float, double 均转为double*/
		unsigned char bstr[4];		/*bitstring*/
		char str[4];				/*string*/
		TimeStamp t;
		unsigned char data[16];
	}un;
} AcsiVariant_S;


#ifdef __cplusplus
extern "C"{
#endif

ASN1_API int GetVariantSize(AcsiVariant *var);
ASN1_API void ClearVariant(AcsiVariant * var);
ASN1_API char SetVariantBool(AcsiVariant * var, char b);
ASN1_API char SetVariantInt8(AcsiVariant * var, char n);
ASN1_API short SetVariantInt16(AcsiVariant * var, short n);
ASN1_API int SetVariantInt32(AcsiVariant * var, int n);
ASN1_API INT64 SetVariantInt64(AcsiVariant * var, INT64 n);
ASN1_API unsigned char SetVariantUInt8(AcsiVariant * var, unsigned char n);
ASN1_API unsigned short SetVariantUInt16(AcsiVariant * var, unsigned short n);
ASN1_API unsigned int SetVariantUInt32(AcsiVariant * var, unsigned int n);
ASN1_API float SetVariantFloat(AcsiVariant * var, float f);
ASN1_API double SetVariantDouble(AcsiVariant * var, double f);
ASN1_API char * SetVariantString(AcsiVariant * var, char * s, int len);
ASN1_API unsigned char * SetVariantUtf8Str(AcsiVariant * var, unsigned char * s, int len);
ASN1_API unsigned char * SetVariantOctStr(AcsiVariant * var, unsigned char * oct, int len);
ASN1_API unsigned char * SetVariantOvStr(AcsiVariant * var, unsigned char * oct, int len);
ASN1_API unsigned char * SetVariantBitStr(AcsiVariant * var, unsigned char * bstr, int len);
ASN1_API unsigned char * SetVariantBvStr(AcsiVariant * var, unsigned char * bstr, int len);
ASN1_API TimeStamp * SetVariantTime(AcsiVariant * var, TimeStamp * time);
ASN1_API TimeStamp * SetVariantBTime6(AcsiVariant * var, TimeStamp * time);
ASN1_API char * SetVariantName(AcsiVariant * var, char * name, int len);
ASN1_API int SetVariantType(AcsiVariant * var, int type, int size);
ASN1_API char * GetVariantName(AcsiVariant * var);
ASN1_API int GetArrayVariantCount(AcsiVariant * var);
ASN1_API int CopyVariant(AcsiVariant *dest_var, AcsiVariant* src_var);
ASN1_API void PrintVariant(AcsiVariant * var);
ASN1_API void PrintVariantMMSType(AcsiVariant * var);
ASN1_API int CheckVariant(AcsiVariant * var, int type, int size);		/* 检查variant符合type和size */

#ifdef __cplusplus
}
#endif

#define SetVariantInt			SetVariantInt32
#define SetVariantUInt			SetVariantUInt32

#ifdef __cplusplus

class CAcsiVar
{
public:
	CAcsiVar(){ ClearVariant(&var); };
	CAcsiVar(AcsiVariant * pVar){ memcpy(&var, pVar, GetVariantSize(pVar)); };
	~CAcsiVar(){};

	CAcsiVar& operator=(AcsiVariant * pVar){ memcpy(&var, pVar, GetVariantSize(pVar)); return *this; };
	CAcsiVar& operator=(AcsiVariant &variant){ memcpy(&var, &variant, GetVariantSize(&variant)); return *this; };
	operator AcsiVariant *(){ return &var; };
	operator AcsiVariant &(){ return var; };

	CAcsiVar& operator=(char b){ SetBool(b); return *this; };
	CAcsiVar& operator=(int n){ SetInt(n); return *this; };
	CAcsiVar& operator=(unsigned int n){ SetUInt(n); return *this; };
	CAcsiVar& operator=(float f){ SetFloat(f); return *this; };
	CAcsiVar& operator=(double f){ SetDouble(f); return *this; };
	CAcsiVar& operator=(char * s){ SetString(s); return *this; };
	CAcsiVar& operator=(TimeStamp * time){ SetTime(time); return *this; };
	
	operator char(){ return GetBool(); };
	operator int(){ return GetInt(); };
	operator unsigned int(){ return GetUInt(); };
	operator double(){ return GetFloat(); };
	operator char *(){ return (char *)GetString(); };
	operator TimeStamp &(){ return GetTime(); };

	int GetSize(){ return GetVariantSize(&var); };
	void Clear(){ ClearVariant(&var); };
	char SetBool(char b){ return SetVariantBool(&var, b); };
	char SetInt8(char n){ return SetVariantInt8(&var, n); };
	short SetInt16(short n){ return SetVariantInt16(&var, n); };
	int SetInt32(int n){ return SetVariantInt32(&var, n); };
	int SetInt(int n){ return SetVariantInt(&var, n); };
	unsigned char SetUInt8(unsigned char n){ return SetVariantUInt8(&var, n); };
	unsigned short SetUInt16(unsigned short n){ return SetVariantUInt16(&var, n); };
	unsigned int SetUInt32(unsigned int n){ return SetVariantUInt32(&var, n); };
	unsigned int SetUInt(unsigned int n){ return SetVariantUInt(&var, n); };
	float SetFloat(float f){ return SetVariantFloat(&var, f); };
	double SetDouble(double f){ return SetVariantDouble(&var, f); };
	char * SetString(char * s, int len = -1){ return SetVariantString(&var, s, len); };
	unsigned char * SetUtf8Str(unsigned char * s, int len){ return SetVariantUtf8Str(&var, s, len); };
	unsigned char * SetOctStr(unsigned char * oct, int len){ return SetVariantOctStr(&var, oct, len); };
	unsigned char * SetOvStr(unsigned char * oct, int len){ return SetVariantOctStr(&var, oct, -len); };
	unsigned char * SetBitStr(unsigned char * bstr, int len){ return SetVariantBitStr(&var, bstr, len); };
	unsigned char * SetBvStr(unsigned char * bstr, int len){ return SetVariantBitStr(&var, bstr, -len); };
	TimeStamp * SetTime(TimeStamp * time){ return SetVariantTime(&var, time); };
	TimeStamp * SetBTime6(TimeStamp * time){ return SetVariantBTime6(&var, time); };
	char * SetName(char * name){ return SetVariantName(&var, name, strlen(name)); };

	char GetBool(){ return var.un.b; };
	char GetInt8(){ return (char)var.un.i; };
	short GetInt16(){ return (short)var.un.i; };
	int GetInt32(){ return var.un.i; };
	int GetInt(){ return var.un.i; };
	unsigned char GetUInt8(){ return (unsigned char)var.un.ui; };
	unsigned short GetUInt16(){ return (unsigned short)var.un.ui; };
	unsigned int GetUInt32(){ return var.un.ui; };
	unsigned int GetUInt(){ return var.un.ui; };
	float GetFloat(){ return (float)var.un.f; };
	double GetDouble(){ return var.un.f; };
	char * GetString(){ return var.un.str; };
	unsigned char * GetUtf8Str(){ return var.un.utf8str; };
	unsigned char * GetOctStr(){ return var.un.oct; };
	int GetOctLen(){ return ABS(var.var_size); };
	unsigned char * GetBitStr(){ return var.un.bstr; };
	int GetBitLen(){ return ABS(var.var_size); };
	TimeStamp& GetTime(){ return var.un.t; };
	char * GetName(){ return GetVariantName(&var); };

	int CmpBitStr(AcsiVariant &acsivar){
		if(var.var_size != acsivar.var_size) return var.var_size-acsivar.var_size;
		int resv=8-(var.var_size&7);
		if(resv != 0){
			resv = 0xff<<resv;
			var.un.bstr[var.size] &= resv;
			acsivar.un.bstr[var.size] &= resv;
			return memcmp(var.un.bstr, acsivar.un.bstr, var.size);
		}
		return memcmp(var.un.bstr, acsivar.un.bstr, var.size);
	};
	
	int CheckVar(int type, int size){ return CheckVariant(&var, type, size); };
	int GetVarType(){ return var.var_type; };
	int GetVarSize(){ return ABS(var.var_size); };

	int GetMmsType(){ return var.mms_type; };
	int GetMmsSize(){ return var.mms_size; };

	int SetType(int type, int size){ return SetVariantType(&var, type, size); };

	AcsiVariant& GetVar(){ return var; };
	unsigned char * GetVarPtr(){ return (unsigned char*)&var; };
	void PrintVar(){ PrintVariant(&var); };
	void PrintVarMMSType(){ PrintVariantMMSType(&var); };

protected:
	AcsiVariant var;
};
#endif


#endif
