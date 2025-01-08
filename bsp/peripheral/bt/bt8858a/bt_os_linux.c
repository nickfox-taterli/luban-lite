
#include "bt_config.h"
#include "bt_api.h"
#include "bt_os.h"
#include "bt_core.h"

#ifdef __cplusplus
extern "C" {
#endif


#if LINUX

static int32_t   	bt_receivecmd_timer;
static int32_t   	bt_decodecmd_timer;

static pthread_t   	uart_recive_task_id;
static pthread_t   	uart_decode_task_id;


#if (runmode == 1)//�߳�����

void sys_bt_decode_task(void*parg)
{
    while(1)
    {
        sys_bt_decode_cmd(parg);

    }

}
void sys_bt_receive_task(void*parg)
{
    while(1)
    {
        sys_bt_receive_cmd(parg);

    }

}

__s32 bt_task_start(void)//task
{
        int     err;
        
		printf("bt_task_start\n");
		err = pthread_create(&uart_decode_task_id, NULL, sys_bt_decode_task, (void*)NULL);
        if (err) 
        {
            printf("decode_task pthread_create error!");
            return EPDK_FAIL;
        }
        printf("create uart_decode_task_id:0x%x", uart_decode_task_id);
       
		err = pthread_create(&uart_recive_task_id, NULL, sys_bt_receive_task, (void*)NULL);
        if (err) 
        {
            printf("recive_task pthread_create error!");
            return EPDK_FAIL;
        }
        printf("create uart_recive_task_id:0x%x", uart_recive_task_id);
        
	    return EPDK_OK;	
}

__s32 bt_task_stop(void)
{
	if(uart_recive_task_id != 0)
    {
        pthread_cancel(uart_recive_task_id);
        uart_recive_task_id = 0;
    }
    if(uart_decode_task_id != 0)
    {
        pthread_cancel(uart_decode_task_id);
        uart_decode_task_id = 0;
    }
	return EPDK_OK;
}

#elif(runmode == 0)//��ʱ������
__s32 bt_task_start(void) //timer
{
        struct itimerspec its1;    
        timer_t timerid1;     // ��ʼ�� itimerspec �ṹ��    
        struct sigevent se1;
        struct itimerspec its2;    
        timer_t timerid2;     // ��ʼ�� itimerspec �ṹ��    
        struct sigevent se2;
         // ���ö�ʱ���¼�    
        se1.sigev_notify = SIGEV_THREAD;  // ʹ���̴߳���ʽ    
        se1.sigev_notify_function = sys_bt_decode_cmd;  // �����̴߳�����    
        se1.sigev_notify_attributes = NULL;

		printf("timer_create bt_decodecmd_timer\n");
		timer_create(CLOCK_REALTIME, &se1, &timerid1)
		if(timerid1 == NULL)
		{
			printf("create bt_decodecmd_timer failed\n");
			return EPDK_FAIL;
		}
		        
        its1.it_value.tv_sec = 0;   //���ö�ʱ���ĳ�ʼ��ʱʱ��Ϊ10ms   
        its1.it_value.tv_nsec = 10*1000;    
        its1.it_interval.tv_sec = 0;  // ���ö�ʱ���ļ��ʱ��Ϊ20ms  
        its1.it_interval.tv_nsec = 20*1000;  
        timer_settime(timerid1, 0, &its1, NULL);
        
        // ���ö�ʱ���¼�    
        se2.sigev_notify = SIGEV_THREAD;  // ʹ���̴߳���ʽ    
        se2.sigev_notify_function = sys_bt_receive_cmd;  // �����̴߳�����    
        se2.sigev_notify_attributes = NULL;
         
		printf("timer_create bt_receivecmd_timer\n");
		timer_create(CLOCK_REALTIME, &se2, &timerid2);	
		if(bt_receivecmd_timer == NULL)
		{
			printf("create bt_receivecmd_timer failed\n");
			return EPDK_FAIL;
		}
		        
        its2.it_value.tv_sec = 0;   //���ö�ʱ���ĳ�ʼ��ʱʱ��Ϊ70ms   
        its2.it_value.tv_nsec = 70*1000;    
        its2.it_interval.tv_sec = 0;  // ���ö�ʱ���ļ��ʱ��Ϊ80ms  
        its2.it_interval.tv_nsec = 80*1000;  
		timer_settime(timerid2, 0, &its2, NULL);
	
	    printf("timer_settime end\n");
	    
	    return EPDK_OK;
start_exit:
    {
        struct itimerspec its = {{0, 0}, {0, 0}};     
    	if(bt_receivecmd_timer!=0)
    	{
            timer_settime(bt_receivecmd_timer, 0, &its, NULL);
            timer_delete(bt_receivecmd_timer);//���ڴ�Ŷ�ʱ��������ݵĽṹ��������
            bt_receivecmd_timer = 0; 
    	}
    	if(bt_decodecmd_timer!=0)
    	{
    	    timer_settime(bt_decodecmd_timer, 0, &its, NULL);
            timer_delete(bt_decodecmd_timer);//���ڴ�Ŷ�ʱ��������ݵĽṹ��������
            bt_decodecmd_timer = 0; 
    	}
	}
	return EPDK_FAIL;
}

__s32 bt_task_stop(void)
{
    struct itimerspec its = {{0, 0}, {0, 0}};     
	if(bt_receivecmd_timer!=0)
	{
        timer_settime(bt_receivecmd_timer, 0, &its, NULL);
        timer_delete(bt_receivecmd_timer);//���ڴ�Ŷ�ʱ��������ݵĽṹ��������
        bt_receivecmd_timer = 0; 
	}
	if(bt_decodecmd_timer!=0)
	{
	    timer_settime(bt_decodecmd_timer, 0, &its, NULL);
        timer_delete(bt_decodecmd_timer);//���ڴ�Ŷ�ʱ��������ݵĽṹ��������
        bt_decodecmd_timer = 0; 
	}
	return EPDK_OK;
}
#endif
#endif
#ifdef __cplusplus
}
#endif

