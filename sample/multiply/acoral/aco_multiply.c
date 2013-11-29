//#include "acoral_bitmap.h"
typedef struct App_Mem_Info
{
    unsigned char *addr;
    void*       para;
    void*       ret;
    int         com_index;
    int         tag;
    int         prio;
	int 		task_start;
}App_Mem_Info;

#define MUTRIX_SIZE 150

#define WIDTH_A     MUTRIX_SIZE
#define HEIGHT_A    MUTRIX_SIZE
#define WIDTH_B     MUTRIX_SIZE
#define HEIGHT_B    MUTRIX_SIZE

#define WIDTH_C     MUTRIX_SIZE
#define HEIGHT_C    MUTRIX_SIZE

typedef struct App_para
{
	unsigned int MatrixA[WIDTH_A][HEIGHT_A];
	unsigned int MatrixB[WIDTH_B][HEIGHT_B];
}App_para;

typedef struct App_ret
{
    unsigned int MatrixC[WIDTH_C][HEIGHT_C];
}App_ret;


void finish_task();

//unsigned int i, j;
void adder(void *args)
{
    App_Mem_Info *memInfo;
    App_para *para;
    App_ret *ret;
    int i, j,k;
	//int task_start,task_end;
    //unsigned int * bitmap = (unsigned int *)(0x80000000);
    //unsigned int oldval, mask;// = 1UL << (nr & 31);
    //unsigned int *p;
   
    memInfo = (App_Mem_Info *)(args);
    para = (App_para *)(memInfo->para);

    ret = (App_ret *)(memInfo->ret);
	//task_start = memInfo->task_start;
	//task_end = task_start + 150;
    for(i = 0; i < WIDTH_A; i++)
    {
        for(j = 0; j < HEIGHT_B; j++)
        {
            ret->MatrixC[i][j] = 0;
            for(k = 0; k < HEIGHT_A; k++)
                ret->MatrixC[i][j] += para->MatrixA[i][k]*para->MatrixB[k][j];
        }
    }
 
    /*send ipi to linux, int cpu0 to run softirq*/ 
    *(volatile int *)(0x80000004) = memInfo->com_index;
//	*(volatile unsigned int *)(0x1F001F00) = 0x10003;
    finish_task();

    //clear_bit(memInfo->com_index, bitmap);
/*
     mask = 1UL << (memInfo->com_index & 31);
     p =bitmap+(memInfo->com_index >> 5);
    oldval = *p;
    *p = oldval &(~mask);
    */
    //finish_task(memInfo);
    return;
}

                       

void finish_task()
{

    //unsigned int * bitmap = (unsigned int *)(0x80000000);
    //*(volatile int *)(0x80000004) =  0x10003;
      /*send ipi to linux, int cpu0 to run softirq*/ 
    //*(volatile int *)(0x80000004) = memInfo->com_index;
	*(volatile unsigned int *)(0x1F001F00) = 0x10003;
//    clear_bit(memInfo->com_index, bitmap);
}
