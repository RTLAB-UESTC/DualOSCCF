#include<thread.h>
#include<policy.h>
#include<pal.h>
#include"../include/shell.h"
void display_thread(acoral_32 argc,acoral_char **argv){
	acoral_sr cpu_sr;	
	acoral_list_t *head,*tmp;
	acoral_thread_t * thread;
	head=&acoral_threads_queue.head;
	acoral_prints("系统线程信息如下\r\n");
	acoral_prints("名称\t类型\t终端\t状态\r\n");
	HAL_ENTER_CRITICAL();
	acoral_spin_lock(&acoral_threads_queue.head.lock);
	for(tmp=head->next;tmp!=head;tmp=tmp->next){
		thread=list_entry(tmp,acoral_thread_t,global_list);
		acoral_print("%s\t",thread->name);
		switch(thread->policy){
			case ACORAL_SCHED_POLICY_COMM:
				acoral_prints("普通\t");
				break;
			case ACORAL_SCHED_POLICY_SLICE:
				acoral_prints("分时\t");
				break;
			case ACORAL_SCHED_POLICY_PERIOD:
				acoral_prints("周期\t");
				break;
			case ACORAL_SCHED_POLICY_RM:
				acoral_prints("RM\t");
				break;
			case ACORAL_SCHED_POLICY_POSIX:
				acoral_prints("Posix\t");
			default:
				break;
		}
		acoral_print("%d\t",thread->console_id);
		if(thread->state&ACORAL_THREAD_STATE_RUNNING)
				acoral_prints("运行\t");
		else if(thread->state&ACORAL_THREAD_STATE_READY)
				acoral_prints("就绪\t");
		else if(thread->state&ACORAL_THREAD_STATE_DELAY)
				acoral_prints("延时\t");
		else if(thread->state&ACORAL_THREAD_STATE_SUSPEND)
				acoral_prints("睡眠\t");
		else if(thread->state==ACORAL_THREAD_STATE_EXIT)
				acoral_prints("僵死\t");
		else
				acoral_prints("错误\t");
		acoral_prints("\r\n");
	}
	acoral_spin_unlock(&acoral_threads_queue.head.lock);
	HAL_EXIT_CRITICAL();
}

acoral_shell_cmd_t dt_cmd={
	"dt",
	display_thread,
	"View all thread info",
	NULL
};
