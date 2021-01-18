/******************************************
 * File:    61850.c
 * Author:  LuoLinglu
 * Date:    2018/04/27
 * Fun:		Main process of 61850
 ******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>.
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/wait.h>


#include "dci.h"
#include "dci_model.h"

#include "iec_module.h"


DCI_SettingInfo setInfos[512];
int settingNum=0; 
time_t ai_tmask;
static int run;





/********************************
 * auth: luolinglu
 * fun: ����ctrl+c�˳�����
*********************************/
void stop(int signo)
{
    printf("\nstop..., signo=%d\n", signo);
    run = 0;

    //cid_end();
    end_data_access();
	smap_release();
	data_upd_handle_release();
	data_timer_handle_release();
	data_timer_funRelease();
    
    
    //if(signo != SIGINT)
        write_to_daemon_log(stacktrace_print, &signo);

    if((signo != SIGINT) && (signo != SIGTERM)) 
    {
        exit(1);
    }
    else
    {
        stop_iec_service();    
        exit(0);
    }

   
}

void main()
{    
	time(&ai_tmask);    //��ȡ��ǰʱ��	
	
	//����iec61850����
    if(start_iec_service())
    {
    	return;
    }

    //ģ��ע��
	iec_module_init();

    //�����̵�ַmap  
    if(smap_establish())
    {
    	stop_iec_service(); 
    	return;
    }   

    //�������ݽ����߳�
    init_data_access();

    //���ͳ�ʼ״̬
    iec_data_init();

    //����������ѯ��ʱ��
    
    run = 1;
    while(run)
    {
        
        usleep(500000);
    }   
}

