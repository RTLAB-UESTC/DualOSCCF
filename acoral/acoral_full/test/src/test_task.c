#include "acoral.h"
ACORAL_COMM_THREAD test(acoral_u32 num){
	acoral_print("in cpu:%dtask%d\n",acoral_current_cpu,num);
}

void test_task_init()
{
	acoral_u32 i;
	for(i=0;i<CFG_TEST_TASK_NUM;i++)
		acoral_create_thread(test,ACORAL_PRINT_STACK_SIZE,i,"task",i+1, -1);
}

TEST_CALL(test_task_init)
