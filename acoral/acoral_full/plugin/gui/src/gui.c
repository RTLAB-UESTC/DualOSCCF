#include"../include/common.h"
extern int server();
void acoral_gui_init(){
	thread_t id;
	pthread_create(&id,NULL,
			(void*)server,NULL);
}
