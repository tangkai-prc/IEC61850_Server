/******************************************
 * File:    iec_interface.c
 * Author:  LuoLinglu
 * Date:    2018/04/26
 * Fun:		intervace functions of iec61850
 ******************************************/

/************************include*************************/
#include "dci.h"
#include "dci_model.h"
#include "dci_model.inl"
#include "dci_file.h"
#include "acsidef.h"

#include "switch_common.h"
#include "switch_semaphore.h"
#include "switch_log.h"

#include "saddrmap.h"
#include "iec_interface.h"

/************************typedef*************************/
typedef struct mms_vcash
{
	int num;		//缓存中的总数
	int mms_no; 	//缓存中最后一次写入mms_no的位置
	DCI_Data value[MMS_VALUE_CASH_LEN];
}mms_vcash_s;

/***************static or global variables***************/
static mms_vcash_s mms_v = {0};	//公共缓存区信号量
static INT32 cash_semid;	//公共缓存区信号量
static INT32 cash_print_ena = 1;    //公共缓存区打印标记，默认使能
static UINT8 time_sync_flag = IEC_TIME_QUALITY_NOTSYNC; //时间质量标记

char file_name[256][200];// = new char[512][256];

static int flag ;
static FileAttrib attrs[100];

//extern "C" int AcsiHook(const SSAcsiReq * pReq, SSAcsiRsp * pRsp);
/**************************code**************************/
int DCI_OnGetAllDataValues(unsigned int reqId)
{

    //DCI_SendValues();
    //DCI_SendResult(reqId, DCI_ENONE);

    return DCI_ENONE;
}

int DCI_OnOperate(unsigned int reqId, DCI_Oper * oper_data)
{
    DCI_SendResult(reqId, OPERATE_SUCC);
    return DCI_ENONE;
}

int DCI_OnSetValues(unsigned int reqId, DCI_Value * var, short num)
{
    /*
    DCI_SendResult(reqId, DCI_ENONE);
    */
    return DCI_ENONE; //DCI_EPERM;
}

int DCI_OnGetSGCB(unsigned char sgcbNo, DCI_SGCB * sgcb)
{
    return DCI_ENONE;
}

int DCI_OnGetSGValues(unsigned int reqId, unsigned char sgcbNo, unsigned char sgNo)
{
    /*
    DCI_SendSGValues(reqId, sgcbNo, sgNo, data, settingNum, DCI_REASON_RESP);
    DCI_SendResult(reqId, DCI_ENONE);
    */
    return DCI_ENONE;
}

int DCI_OnSwitchSG(unsigned int reqId, unsigned char sgcbNo, unsigned char sgNo)
{
    /*
    DCI_SendResult(reqId, DCI_ENONE);
    */
    return DCI_ENONE;
}

int DCI_OnSetSGValues(unsigned int reqId, unsigned char sgcbNo, unsigned char sgNo, DCI_Value * var, short num)
{
    /*
    DCI_SendResult(reqId, DCI_ENONE);
    */
    return DCI_ENONE;
}

int DCI_OnConfirmEditSGValues(unsigned int reqId, unsigned char sgcbNo, unsigned char sgNo, DCI_Value * var, short num)
{
    /*
    DCI_SendResult(reqId, DCI_ENONE);
    */
    return DCI_ENONE;
}

int DCI_OnCancelEditSGValues(unsigned int reqId, unsigned char sgcbNo)
{
    /*
    DCI_SendResult(reqId, DCI_ENONE);
    */
    return DCI_ENONE;
}

int DCI_OnSubstitute(unsigned int reqId, DCI_Data * data, unsigned char state)
{
    /*
    DCI_SendResult(reqId, DCI_ENONE);
    */
    return DCI_ENONE;
}

int AcsiHook(const SSAcsiReq * pReq, SSAcsiRsp * pRsp)
{
//	 return ACSI_EFAIL;

	int file_num = 0;
	char path[256];
	char full_path[256];
	char cmd_buf[256];
	memset (path,'\0',sizeof(path));
	memset (full_path,'\0',sizeof(full_path));
	char* fname[] = {SYS_LOG_FILE_NAME, 
	                ALARM_LOG_FILE_NAME, 
	                MEAS_APP_FILE, 
	                MEAS_CID_FILE, 
	                MEAS_ICD_FILE, 
	                MEAS_VER_FILE
	                };
	int i, fnum;
	//printf("get request 111!\n");

	if(pReq->type != ACSIREQ_FILE_DIR)
		return ACSI_EAGAIN;
		
	//struct stat file_pro[512];
	sprintf(full_path,"%s/", IEC_PATH);
	printf("full_path %s \n",full_path);
	printf("nameDir %s \n",pReq->un.file_dir.nameDir);

    if(flag == 0)
    {

	   for(i=0;i<100;i++)
	   {
	      attrs[i].filename = (char*)malloc(256); //new char[256] ;
	      memset( attrs[i].filename,0,256);
	   }

	   flag=1;

    }

   	if(strcmp(pReq->un.file_dir.nameDir, full_path) != 0)
   	{
   	    return ACSI_ENONE;  //ACSI_EFAIL   	    
   	}

    sprintf(path, "");

    //更新日志文件
    for(i = 0; i < 2; i++)
    {
        memset(cmd_buf, 0, sizeof(cmd_buf));
	    sprintf(cmd_buf, "cp -f %s .%s/", fname[i], IEC_PATH);
	    system(cmd_buf);
	    printf("cmd_buf:%s\n",cmd_buf);
    }

    fnum = sizeof(fname)/sizeof(char*);
    printf("file_num: %d\n", fnum);

    //挂载文件列表
	for (i =0; i <  fnum ; i++)
	{
	   	memset(file_name[i],0,200);
	   	sprintf(file_name[i], "%s",fname[i]); //APP_PATH
	    sprintf(attrs[i].filename ,"%s", file_name[i]);
		printf("file_name_attrs[%d] = %s\n",i,attrs[i].filename);
		
		SetTimeStampSecs(&attrs[i].mtime,20000);
		SetTimeStampMsec(&attrs[i].mtime, 0);
		attrs[i].size=1000+i;
		pRsp->un.file_dir.attr[i]=(FileAttrib*)&attrs[i];
		
	    /*
        memset(file_name[i],0,200);
		memcpy(file_name[i],"D:\\config\\arp\\config.txt",strlen("D:\\config\\arp\\config.txt"));
		sprintf(attrs[i].filename ,"%s", file_name[i]);
		printf("file_name_attrs[%d] = %s\n",i,attrs[i].filename);
		SetTimeStampSecs(&attrs[i].mtime,20000);
		SetTimeStampMsec(&attrs[i].mtime, 0);
		attrs[i].size=1000+i;
		pRsp->un.file_dir.attr[i]=(FileAttrib*)&attrs[i];
		//pRsp->un.file_dir.attr[i]=new FileAttrib[1];
		//pRsp->un.file_dir.attr[i]->filename =new char[200];
		//memset(pRsp->un.file_dir.attr[i]->filename ,0,200);
		//memcpy(pRsp->un.file_dir.attr[i]->filename,(char*)attrs[i].filename,strlen(attrs[i].filename));//无法传递
		*/
	}

    //test(pRsp);
	pRsp->un.file_dir.num = fnum;
	pRsp->un.file_dir.moreFollow = 0;

	pRsp->result = ACSI_ENONE;
	pRsp->error = service_error_none;


    return ACSI_ENONE;//ACSI_EFAIL
}

extern int OutputDatFileFromCid(char * file_name,char * ap_name,int file_type);

/****************************************
 * author: luolinglu
 * fun: start_iec_service
*****************************************/
int start_iec_service(void)
{
	DCI_AppInfo appInfo;    
    int res;
    char cid_name[64]={0};

    appInfo.numOfSGCB = 0;

    //解析cid文件
    sprintf(cid_name, "%s/%s", APP_PATH, CID_FILE);
    if((res=OutputDatFileFromCid(cid_name, "S1", 0))<0)
    {
        printf("Read cid file failed!,res=%d\n",res);
        return SW_ERROR;
    }

    //DCI初始化
    printf("1111111111\n");
    res = DCI_Init();
    if( res != DCI_ENONE )
    {
        printf("DCI_Init() failed! res=%d\n", res);
        DCI_Clear();
        return SW_ERROR;
    }

    //设置文件服务
    DCI_SetAcsiHook(AcsiHook);

    //DCI启动服务
    printf("33333\n");
    if( DCI_StartService() != DCI_ENONE )
    {
        printf("DCI_StartService() failed! \n");
        return SW_ERROR;
    }
    
    //设置appinfo
    printf("4444\n"); 
    appInfo.numOfSGCB = 0;
    res = DCI_SetAppInfo(&appInfo);
    if( res != DCI_ENONE )
    {
        printf("DCI_SetAppInfo() failed! res=%d\n", res);
        return SW_ERROR;
    }

    return SW_OK;
}

/****************************************
 * author: luolinglu
 * fun: stop_iec_service
*****************************************/
void stop_iec_service(void)
{
    DCI_StopService();
    DCI_Clear();
}


/****************************************
 * author: luolinglu
 * fun: init_mms_values_cash
*****************************************/
void init_mms_values_cash(void)
{
	cash_semid = switch_sem_init(SW_61850_SEMKEY, 2);
    cash_semid = switch_sem_get(SW_61850_SEMKEY);

    switch_sem_take(cash_semid, 0);  //获取信号量
    memset(&mms_v, 0, sizeof(mms_v));
    switch_sem_give(cash_semid, 0);  //释放信号量
}
    
/****************************************
 * author: luolinglu
 * fun: printf_mms_values_cash
*****************************************/
void printf_mms_values_cash(mms_vcash_s *p_cash, char* name)
{
    struct tm *p_t;
    time_t t;
    
	if(cash_print_ena == 0)
	{
		return;
	}

	t = p_cash->value[p_cash->mms_no].t.tv_sec;
	p_t = gmtime(&t);
	
	printf("mms_cash: sid=0x%x, type=%d, len=%d, tq=0x%02x, time:%04d%02d%02d %02d:%02d:%02d, name:%s, ", 
		p_cash->value[p_cash->mms_no].sid,
		p_cash->value[p_cash->mms_no].value.var_type,
    	p_cash->value[p_cash->mms_no].value.var_len, 
    	p_cash->value[p_cash->mms_no].t.qflags,
    	p_t->tm_year+1900, p_t->tm_mon+1, p_t->tm_mday, 
    	p_t->tm_hour, p_t->tm_min, p_t->tm_sec,
    	name);
	switch(p_cash->value[p_cash->mms_no].value.var_type)
	{
		case DCI_BOOL:
			printf("val=%d\n", p_cash->value[p_cash->mms_no].value.un.b);
			break;
		case DCI_INT32U:
			printf("val=%d\n", p_cash->value[p_cash->mms_no].value.un.ui);
			break;
		case DCI_INT32:
			printf("val=%d\n", p_cash->value[p_cash->mms_no].value.un.i);
			break;
		case DCI_FLOAT32:
			printf("val=%f\n", p_cash->value[p_cash->mms_no].value.un.f);
			break;
		case DCI_UNICODE_STRING:
			printf("val=%s\n", p_cash->value[p_cash->mms_no].value.un.data);
			break;
		default:
			break;
	}

}

/****************************************
 * author: luolinglu
 * fun: 向mms缓存中写入数据
 * para: name 	注册的变量名(与短地址相同)
 		 p_val 	写入值的指针
 		 len	写入值的长度
 		 time	数据变化的时间
*****************************************/
void write_mms_values_cash(const char* name, void* p_val, int len, struct timeval time)
{
	mms_info_s info = {0};
	mms_vcash_s *p_cash = &mms_v;
	char buf[40] = {0};
	int length = (len<32) ? len : 32;
	
    if(smap_get_minfo(&info, name))
    {
    	printf("fun:%s:err: unregister var '%s'\n", __FUNCTION__, name);
    	return;
    }

    if(info.sid == (-1))
    {
    	//printf("fun:%s:warn: un-instantiated saddr '%s', sid=0x%x\n", __FUNCTION__, name, info.sid);
    	return;
    }

    switch_sem_take(cash_semid, 0);  //获取信号量

	//防止溢出，提前清空缓存中的数据
    if(p_cash->num > MMS_VALUE_CASH_OVER)
    {
    	switch_sem_give(cash_semid, 0);  //释放信号量
    	send_mms_values_cash(DCI_REASON_DCHG|DCI_REASON_QCHG);
    	
    }
    else
    {
    	switch_sem_give(cash_semid, 0);  //释放信号量
    }

    if(p_val)
    {
        memcpy(buf, (char*)p_val, length);
    }
	

    switch_sem_take(cash_semid, 0);  //获取信号量
        
	p_cash->num++;
	p_cash->mms_no++;
	if(p_cash->mms_no >= MMS_VALUE_CASH_LEN)
	{
		p_cash->mms_no = p_cash->mms_no - MMS_VALUE_CASH_LEN;
	}

	memset(&(p_cash->value[p_cash->mms_no]), 0, sizeof(DCI_Data));
    p_cash->value[p_cash->mms_no].sid = info.sid;
	p_cash->value[p_cash->mms_no].t.tv_sec = time.tv_sec;
	p_cash->value[p_cash->mms_no].t.tv_usec= time.tv_usec;
	p_cash->value[p_cash->mms_no].t.qflags = time_sync_flag;

	//printf("fun:%s: name:%s, sid=0x%x, ", __FUNCTION__,name, p_cash->value[p_cash->mms_no].sid);

    switch(info.type)
    {
    	case IEC_TYPE_BOOL:
    		p_cash->value[p_cash->mms_no].value.var_type = DCI_BOOL;
    		p_cash->value[p_cash->mms_no].value.var_len = 1;
    		p_cash->value[p_cash->mms_no].value.un.b= *(UINT8*)buf;
    		break;
    	case IEC_TYPE_UINT8:
    	case IEC_TYPE_UINT16:
    	case IEC_TYPE_UINT32:
    		p_cash->value[p_cash->mms_no].value.var_type = DCI_INT32U;
    		p_cash->value[p_cash->mms_no].value.var_len = 4;
    		p_cash->value[p_cash->mms_no].value.un.ui= *(unsigned int*)buf;
    	break;
    	case IEC_TYPE_INT8:
    	case IEC_TYPE_INT16:
    	case IEC_TYPE_INT32:
    		p_cash->value[p_cash->mms_no].value.var_type = DCI_INT32;
    		p_cash->value[p_cash->mms_no].value.var_len = 4;
    		p_cash->value[p_cash->mms_no].value.un.i= *(int*)buf;
    	break;
    	case IEC_TYPE_FLOAT32:
    		p_cash->value[p_cash->mms_no].value.var_type = DCI_FLOAT32;
    		p_cash->value[p_cash->mms_no].value.var_len = 4;
    		p_cash->value[p_cash->mms_no].value.un.f= *(FLOAT32*)buf;    	
    	break;
    	case IEC_TYPE_STRING:
    		p_cash->value[p_cash->mms_no].value.var_type = DCI_UNICODE_STRING;
    		p_cash->value[p_cash->mms_no].value.var_len = length;
    		memcpy(p_cash->value[p_cash->mms_no].value.un.data, buf, length);  	
    	break;

    	default:
    		printf("fun%s:err: unsupported type %d\n", __FUNCTION__,info.type);
    	break;    	
    }

    if(len == 0)
    {
        p_cash->value[p_cash->mms_no].value.var_len = 0;
    }

	printf_mms_values_cash(p_cash, name); //酌情打印
    switch_sem_give(cash_semid, 0);  //释放信号量
    return;
}

/****************************************
 * author: luolinglu
 * fun:    将mme缓存中的数据上送
 * para: reason 请求上送数据的原因
*****************************************/
void send_mms_values_cash(unsigned short reason)
{
	int begin;
	mms_vcash_s *p_cash = &mms_v;

	switch_sem_take(cash_semid, 0);  //获取信号量
	if(p_cash->num == 0)
	{
		switch_sem_give(cash_semid, 0);  //释放信号量
		return;
	}

	begin = p_cash->mms_no - p_cash->num + 1;

	if(begin >= 0)
	{
		DCI_SendData(0, &(p_cash->value[begin]), p_cash->num, reason);
	}
	else
	{
		begin = begin + MMS_VALUE_CASH_LEN;
		DCI_SendData(0, &(p_cash->value[begin]), MMS_VALUE_CASH_LEN - begin, reason);
		DCI_SendData(0, &(p_cash->value[0]), p_cash->mms_no, reason);
	}
	//printf("fun:%s: %d~%d, num=%d\n", __FUNCTION__, begin, p_cash->mms_no, p_cash->num);

	p_cash->num = 0;
	switch_sem_give(cash_semid, 0);  //释放信号量
}

/****************************************
 * author: luolinglu
 * fun: mms_cash_printf_enable
*****************************************/
void mms_cash_printf_enable(void)
{
	cash_print_ena = 1;
}

/****************************************
 * author: luolinglu
 * fun: mms_cash_printf_disable
*****************************************/
void mms_cash_printf_disable(void)
{
	cash_print_ena = 0;
}


/****************************************
 * author: luolinglu
 * fun: mms_data_time_not_synchronized
*****************************************/
void mms_data_time_not_synchronized(void)
{
	time_sync_flag = IEC_TIME_QUALITY_NOTSYNC;
}

/****************************************
 * author: luolinglu
 * fun: mms_data_time_synchronized
*****************************************/
void mms_data_time_synchronized(void)
{
	time_sync_flag = IEC_TIME_QUALITY_SYNC;
}

