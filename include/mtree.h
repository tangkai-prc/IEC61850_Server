#ifndef _MTREE_H_
#define _MTREE_H_

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

#include "typedef.h"

typedef struct{
	int len;
	int used;
	unsigned char * buf;
}MBufPool;

typedef struct mem_tree_node	MTreeNode;

typedef struct{
	MTreeNode * root;
	MTreeNode * current;
	MBufPool * mbuf;
}MTree;

struct mem_tree_node{
	MTreeNode * pParent;
	MTreeNode * pSibling;
	MTreeNode * pChild;
	int len;
	unsigned char data[8];
};

#ifdef __cplusplus
extern "C" {
#endif

ASN1_API int mbuf_init(MBufPool * mbuf, void * buf, int len);
ASN1_API int mbuf_reinit(MBufPool * mbuf);
ASN1_API void * mbuf_alloc(MBufPool * mbuf, int size, int align);
ASN1_API int mbuf_free(MBufPool * mbuf, void * buf);
ASN1_API int mbuf_get_size(MBufPool * mbuf);
ASN1_API int mbuf_get_used(MBufPool * mbuf);


ASN1_API MTree * mtree_new(MBufPool * mbuf);
ASN1_API int mtree_reinit(MTree * tree);
ASN1_API int mtree_is_leaf(MTree * tree);
ASN1_API MTree * mtree_copy(MTree * src_tree, MTree * dest_tree);
ASN1_API MTree * mtree_split(MTree * tree, MTreeNode * pNode, MTree * sub_tree);
ASN1_API MTreeNode * mtree_goto_root(MTree * tree);
ASN1_API MTreeNode * mtree_goto_next(MTree * tree);
ASN1_API MTreeNode * mtree_goto_child(MTree * tree);
ASN1_API MTreeNode * mtree_goto_parent(MTree * tree);
ASN1_API MTreeNode * mtree_goto(MTree * tree, MTreeNode * node);
ASN1_API MTreeNode * mtree_insert(MTree * tree, unsigned char * buf, int len);  /*insert behind current*/
ASN1_API MTreeNode * mtree_append(MTree * tree, unsigned char * buf, int len);
ASN1_API MTreeNode * mtree_insert_child(MTree * tree, unsigned char * buf, int len);
ASN1_API int mtree_get_level(MTree * tree);
ASN1_API int mtree_get_leaf_num(MTree * tree);
ASN1_API int mtree_is_empty(MTree * tree);
ASN1_API void mtree_print(MTree * tree);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
class CMTree{
public:
	CMTree(unsigned char * buf, int len){ pNode = NULL; tree = NULL;\
		if(mbuf_init(&mbuf, buf, len) >= 0) tree = mtree_new(&mbuf); };
	CMTree(MTree * pTree){ pNode = NULL; tree = pTree; };
	CMTree(const CMTree &mtree){ memcpy(this, &mtree, sizeof(CMTree)); tree->mbuf = &mbuf;};
	CMTree(){ pNode = NULL; memset(&mbuf, 0, sizeof(MBufPool)); };
	~CMTree(){};
	MTreeNode * pNode;

	CMTree& operator=(const CMTree &mtree){ memcpy(this, &mtree, sizeof(CMTree)); tree->mbuf = &mbuf; return *this; };
	CMTree& operator=(CMTree * pTree){ memcpy(this, pTree, sizeof(CMTree)); tree->mbuf = &mbuf; return *this; };
	CMTree& operator=(MTree * pTree){ pNode = NULL; tree = pTree; return *this; };
	CMTree& operator=(MTree & mtree){ pNode = NULL; tree = &mtree; return *this; };

	MTree * GetTree(){ return tree; };
	void * GetData(){ if(pNode != NULL && pNode->len > 0) return pNode->data; else return NULL; };
	void Clear(){ pNode = NULL; mtree_reinit(tree); };
	BOOL8 GotoRoot(){ MTreeNode * node = mtree_goto_root(tree); \
		if(node != NULL) { pNode = node; return TRUE; }\
		else return FALSE; };
	BOOL8 GotoNext(){ MTreeNode * node = mtree_goto_next(tree);\
		if(node != NULL) { pNode = node; return TRUE; }\
		else return FALSE; };
	BOOL8 GotoChild(){ MTreeNode * node = mtree_goto_child(tree);\
		if(node != NULL) { pNode = node; return TRUE; }\
		else return FALSE; };
	BOOL8 GotoParent(){ MTreeNode * node = mtree_goto_parent(tree);\
		if(node != NULL) { pNode = node; return TRUE; }\
		else return FALSE; };
	BOOL8 Goto(MTreeNode * node){ MTreeNode * tmp = mtree_goto(tree, node);\
		if(tmp != NULL) { pNode = tmp; return TRUE; }\
		else return FALSE; };
	BOOL8 Insert(unsigned  char * buf, int len){ MTreeNode * node = mtree_insert(tree, buf, len);\
		if(node != NULL) { pNode = node; return TRUE; }\
		else return FALSE; };
	BOOL8 Append(unsigned  char * buf, int len){ MTreeNode * node = mtree_append(tree, buf, len);\
		if(node != NULL) { pNode = node; return TRUE; }\
		else return FALSE; };
	BOOL8 InsertChild(unsigned  char * buf, int len){ MTreeNode * node = mtree_insert_child(tree, buf, len);\
		if(node != NULL) { pNode = node; return TRUE; }\
		else return FALSE; };
	int GetLevel(){ return mtree_get_level(tree); };
	BOOL8 IsLeaf(){ if(mtree_is_leaf(tree) >= 0) return TRUE;\
		else return FALSE; };
	CMTree * Split(MTreeNode * node, CMTree * sub_tree){ mtree_split(tree, node, sub_tree->tree);\
		return sub_tree;};
	CMTree * Copy(CMTree * dest_tree){ mtree_copy(tree, dest_tree->tree);\
		return dest_tree;};
	int GetLeafNum(){ return mtree_get_leaf_num(tree); };
	int IsEmpty(){ return mtree_is_empty(tree); };
	void PrintTree(){ mtree_print(tree); };

protected:
	MBufPool mbuf;
	MTree * tree;
};

#endif

#endif
