/*======================================================================
    A sharemem driver as an example of char device drivers
    There are two same sharemems in this driver  
    This example is to introduce the function of file->private_data
======================================================================*/
#ifndef ACORAL_LINK_H
#define ACORAL_LINK_H
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>

#include "buddy.h"


/*trans acoral.bin and start it*/
#define ACORAL_BASE		0x06000000
#define ACORAL_MAX		0x0FFFFFFF  /* for general memory access */


#define BRD_FLAGS_RES	0x10000030    /*reserve the flag*/
#define IPI_RES			0x1F001F00    /*IPI register*/
#define SHARE_MEM_START    0x80000000    /*sharemem start address*/
#define SHARE_MEM_SIZE	    0xA000000	/*sharemem size = 160M*/
#define SHARE_MEM_END   (SHARE_MEM_START + SHARE_MEM_SIZE) /*sharemem end address*/

#define ALLOC_MEM_SHIFT     0x100000
#define ALLOC_MEM_START     (SHARE_MEM_START + ALLOC_MEM_SHIFT)  /*from 1M*/
#define ALLOC_MEM_END       SHARE_MEM_END

#define ACORAL_IPI_INT1_CPU3 0x80001 /*use ipi 1 to enable cpu3*/
#define ACORAL_IPI_INT3_APP  0x80003 /*use ipi 3 to start an app from cpu3*/
#define ACORAL_IPI_INT3_CPU1_APP    0x20003
#define ACORAL_IPI_INT3_CPU3_APP    0x80003 /*use ipi 3 to start an app from cpu3*/

static unsigned int  file_len = 0;
static void __iomem *acoral_base;
static void __iomem *flags_base;
static void __iomem *ipi_base;
static void __iomem *share_mem_base;
///////////////////////////////



/*some ioctl cmd*/
#define MEM_CLEAR 0x1  /*clear sharemem*/
#define SEND_IPI   0x2 /*send ipi cmd*/
#define ACORAL_LINK_APP 0x4  /*send app to acoral*/

#define SHAREMEM_MAJOR 251    /*sharemem major device number*/

extern struct completion acoral_com[32];  /*in kernal smp.c*/
extern int *acoral_com_index;
//extern int acoral_tasks;

static int sharemem_major = SHAREMEM_MAJOR;
/*sharemem device struct*/
struct sharemem_dev                                     
{                                                        
  struct cdev cdev; /*cdev char device*/                       
  unsigned char *mem; /*sharemem address*/        
  unsigned int *ipi_flag; /*send core ipi interrupt*/
};

struct sharemem_dev *sharemem_devp; /*设备结构体指针*/


typedef struct App_Info
{
	char* 	filename;       //app.bin name
	void* 	para;           //app para addr pointer
    int     parasize;       //app para struct size
    void*   ret;            //app return value addr
    int     retsize;        //app return value sturct size
    int     prio;           //1-31
}App_Info;

typedef struct App_Mem_Info
{
	unsigned char *addr;    //app  addr
	void*   para;           //app  para addr 
    void*   ret;            //app  return value addr
    int     com_index;      //which struct completion com;
    int     tag;            //app is loaded ?Y=1,N=0
    int     prio;           //1-31
   // int     task_start;     //where to start calculate
}App_Mem_Info;

typedef struct App_para
{
        int para1;
        int para2;
}App_para;

#define MAX_APP_MEM     32       //support MAX_APP_MEM app
App_Mem_Info    *appMemInfo;    //appMemInfo pointer
extern unsigned long *appMemInfo_bitmap;
unsigned int first_blk_phy;     //the first blk pianyi


struct FreeList AllList[M_VAL+1];
#endif
