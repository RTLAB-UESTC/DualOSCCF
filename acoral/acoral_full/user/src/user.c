#include"acoral.h"
#include"../include/shell.h"
void dummy(void *args){
	while(1){
		acoral_delay_self(1000);
	}
}


void monitor_linux(void *args)
{
	//acoral_u32 app_addr = (*(volatile acoral_u32 *)(0x80000004));	
	while(1){
		 acoral_delay_self(1000);
		acoral_print("%u\n", *(volatile acoral_u32 *)(0x80000000));
		
		if((*(volatile acoral_u32 *)(0x80000000)) & 0x1)
		{
			acoral_print("app start\n");
			acoral_u32 app_addr = (*(volatile acoral_u32 *)(0x80000004));
			acoral_print("app_addr is %d", app_addr);
			acoral_create_thread(app_addr, 512,NULL,"app",21, 3);
			break;
		}
		acoral_print("%u", *(volatile acoral_u32 *)(0x80000008));
	}
		
}

void test1(void *args)
{
	while(1){
		 acoral_delay_self(1000);
		 acoral_print("cpu1\n");
		}
}

void test2(void *args)
{
	while(1){
		 acoral_delay_self(2000);
		 acoral_print("cpu2\n");
		}
}

void test3(void *args)
{
	while(1){
		 acoral_delay_self(3000);
		 acoral_print("cpu3\n");
		}
}


void user_main()
{
#ifdef CFG_TELNET_SHELL
	user_telnetd();
#endif
#ifdef CFG_WEB_SERVER
	user_httpd();
#endif
	
	//acoral_id id = acoral_create_thread(monitor_linux, 512,NULL,"monitor_linux",21, 3);
	//acoral_create_thread(test1, 512,NULL,"test1",21, 0);
	//acoral_create_thread(test2, 512,NULL,"test2",21, 1);
	//acoral_create_thread(test3, 512,NULL,"test3",21, 2);

	/*if(id == -1)
	{
		while(1){
         		acoral_delay_self(1000);
         		acoral_print("Delay test\n");
   		 }
	}
	*/

}

