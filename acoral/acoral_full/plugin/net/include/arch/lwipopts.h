#ifndef _ACORAL_LWIPOPTS_H__
#define _ACORAL_LWIPOPTS_H__

#include "arch/cc.h"
#include "arch/sys_arch.h"

#define OS_LOWEST_PRIO    20


//数据接收线程
#define  ETH_RECV_TASK_PPIO            (OS_LOWEST_PRIO + 0)
//TCPIP线程
#define  TCPIP_THREAD_PRIO			   (OS_LOWEST_PRIO + 1)
//HTTPD网页服务器任务
#define  APP_TASK_HTTPD_PRIO		   (OS_LOWEST_PRIO + 2)
//远程登录任务
#define  TELNET_TASK_PRIO			   (OS_LOWEST_PRIO + 3)



//Task Stk lengh
#define TCPIP_THREAD_STACKSIZE  1024*4

#define strlen  acoral_str_len
#define strcpy  acoral_str_cpy
#define strcmp  acoral_str_cmp
#define memcpy  __re_memcpy
#define memset  __re_memset

#endif /* _ACORAL_LWIPOPTS_H__ */
