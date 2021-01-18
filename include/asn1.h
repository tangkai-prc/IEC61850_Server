#ifndef __ASN1_H_INCLUDE__
#define __ASN1_H_INCLUDE__

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

#include "mtree.h"
#include "acsivar.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef struct{
	char * name;
	int type;			/*BDT Type*/
	int size;			/*字节数*/
	void * data;
}Asn1StructDefine;

/*
	struct asn1_struct st;
	{ "",			BDT_STRUCT_BEGIN,		0,	&st },			//开始行
		{ "Data1",		BDT_VISIBLESTRING,		65,		&st.data1 },
		{ "Data2",		BDT_BOOLEAN,			1,		&st.data2 },
		{ "",			BDT_STRUCT_BEGIN,		0,	NULL },
			{ "Data31",		BDT_INT32U,				4,		&st.data3.data31 },
			{ "Data32",		BDT_INT32U,				4,		&st.data3.data32 },
		{ "",			BDT_STRUCT_END,		0,	NULL }
		{ "Data4",		BDT_BOOLEAN,			1,		&st.data4 },
	{ "",			BDT_STRUCT_END,		0,	NULL }				//结束行
*/

ASN1_API int asn1_decode(unsigned char * asn1_data, int asn1_len, MTree * tree);
ASN1_API int asn1_decode_struct(unsigned char * asn1_data, int asn1_len, Asn1StructDefine * asn1_struct, unsigned char * buf, int len);
ASN1_API int asn1_encode(MTree * tree, unsigned char * asn1_data, int asn1_len);
ASN1_API int asn1_encode_struct(Asn1StructDefine * asn1_struct, unsigned char * buf, int len, unsigned char * asn1_data, int asn1_len);
ASN1_API int asn1_encode_var(AcsiVariant * var, unsigned char * asn1_data, int asn1_len);
ASN1_API int asn1_decode_type(unsigned char * asn1_data, int asn1_len, MTree * tree);
ASN1_API int asn1_encode_type(MTree * tree, unsigned char * asn1_data, int asn1_len);
ASN1_API int asn1_tree_to_struct(MTree * tree, Asn1StructDefine * asn1_struct, unsigned char * buf, int len);
ASN1_API int asn1_struct_to_tree(Asn1StructDefine * asn1_struct, unsigned char * buf, int len, MTree * tree);
ASN1_API int encode_arrayElemName(char* name, char* alternateAccess);

#ifdef __cplusplus
}
#endif

#endif
