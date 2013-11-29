#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


#define ACORAL_LINK_APP 0x4

typedef struct App_Info
{
	char *filename;
	void *para;
    int  parasize;
    void *ret;
    int  retsize;
    int  prio;   //1-31
}App_Info;

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

int   timeval_subtract(struct   timeval*   result,   struct   timeval*   x,struct   timeval*   y);


int open_acoral_link(void)
{
    int fdev;
    if((fdev = open("/dev/sharemem", O_RDWR)) < 0)
    {
        return -1;
    }
    return fdev;
}

int app_acoral_link(int fdev, App_Info *info)
{
    
	if(ioctl(fdev, ACORAL_LINK_APP, info) < 0)
    {
        return -1;
    }
    return 0;
}

int close_acoral_link(int fdev)
{
    close(fdev);
    return 0;
}

int main(int argc,const char** argv)
{
    int fdev, res;
	App_Info info;
//	App_para para;
	App_ret  ret;
	App_para para;
    int i, j;
    struct   timeval   start,stop,diff;  
    gettimeofday(&start,0);
    if(argc < 2)
    {
        printf("invalid param\n");
        return -1;
    }

    fdev = open_acoral_link();
    if(fdev < 0)
    {
        printf("cannot open acoral_link\n");
        return -1;
    }
/*
    for(i = 0; i < WIDTH_A; i++)
    {
        for(j = 0; j < HEIGHT_A; j++)
        {
            para.MatrixA[i][j] = i+1;
            printf("%d ", i+1);
        }
        printf("\n");
    }

    for(i = 0; i < WIDTH_B; i++)
    {
        for(j = 0; j < HEIGHT_B; j++)
        {
            para.MatrixB[i][j] = i+1;
            printf("%d ", i+1);
        }
        printf("\n");
    }
*/

	
	info.filename = (char *)argv[1];
	info.para = (void *)(&para);
    info.parasize = sizeof(App_para);
    info.ret =  (void *)(&ret);
    info.retsize = sizeof(App_ret);
    info.prio = 16;
    //printf("para:%x\n", info.para);
   // printf("ret:%x\n", info.ret);
    printf("parasize:%d\n", info.parasize);
    //printf("para1:%d\n", para.para1);
    //printf("para2:%d\n", para.para2);
    printf("retsize:%d\n", info.retsize);
    res = app_acoral_link(fdev, &info);
    if(res < 0)
    {
        printf("app_acoral_link error\n");
        close_acoral_link(fdev);
        return -1;
    }
    gettimeofday(&stop,0);  
    timeval_subtract(&diff,&start,&stop);  
    printf("%ld.%lds\n", diff.tv_sec,diff.tv_usec);   
/*
    for(i = 0; i < WIDTH_C; i++)
    {
        for(j = 0; j < HEIGHT_C; j++)
            printf("%d ", ret.MatrixC[i][j]);
        printf("\n");
    }
*/
    close_acoral_link(fdev);

    return 0;

}



int   timeval_subtract(struct   timeval*   result,   struct   timeval*   x,struct   timeval*   y)  
{  
      	int   nsec;  
        if(x->tv_sec>y->tv_sec)  
        	return   -1;  
   
        if((x->tv_sec==y->tv_sec) && (x->tv_usec>y->tv_usec))  
        	return   -1;  
   
        result->tv_sec = (y->tv_sec-x->tv_sec);  
        result->tv_usec = (y->tv_usec-x->tv_usec);  
   
        if(result->tv_usec<0)  
        {  
        	result->tv_sec--;  
            result->tv_usec+=1000000;  
        }  
        return   0;  
  }     
