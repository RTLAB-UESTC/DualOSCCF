#ifndef __FS_H 
#define __FS_H
#include <type.h>
#include<driver.h>
//#define strupr(str) acoral_str_upr(str)
extern acoral_char  *strupr(acoral_char *Str);
#define GET_DISK_INFO 0x01
/********************************/
/*     本例子的配置             */
/********************************/
#define MAX_DISK_CACHES     10
#include    	"fat.h"
#include        "format.h"
#include 	"fscmd.h"
#endif
