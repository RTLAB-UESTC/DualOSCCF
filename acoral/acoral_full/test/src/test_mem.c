#include "acoral.h"
ACORAL_COMM_THREAD test(){
	acoral_u32 * p1;
	acoral_u32 * p2;
	acoral_u32 * p3,*p4;
	p1=acoral_malloc(1024);
	p2=acoral_malloc(1024);
	p3=acoral_malloc(1024);
	p4=acoral_malloc(1024);
	acoral_free(p1);
//	acoral_free(p1);
	acoral_free(p1);
	acoral_free(p2);
	acoral_free(p3);
	acoral_free(p4);
//	acoral_free(0x601a8b9);
}

void test_mem_init()
{
	acoral_create_thread(test, ACORAL_PRINT_STACK_SIZE,NULL,"Memtest",42, 0);
//	acoral_create_thread(test, ACORAL_PRINT_STACK_SIZE,NULL,"Memtest",42, 1);
}

TEST_CALL(test_mem_init)
