#ifndef HAL_2200_CFG_H
#define HAL_2200_CFG_H

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/********************************/
/*      ARM specital code      */
/*      ARM的特殊代码           */
/********************************/
//This segment should not be modify
//这一段无需改动

#include    "hal_2200_c.h"

/* System configuration .Fosc、Fcclk、Fcco、Fpclk must be defined */
/* 系统设置, Fosc、Fcclk、Fcco、Fpclk必须定义*/
#define Fosc            11059200                    //Crystal frequence,10MHz~25MHz，should be the same as actual status. 
						    //应当与实际一至晶振频率,10MHz~25MHz，应当与实际一至
#define Fcclk           (Fosc * 4)                  //System frequence,should be (1~32)multiples of Fosc,and should be equal or less  than 60MHz. 
						    //系统频率，必须为Fosc的整数倍(1~32)，且<=60MHZ
#define Fcco            (Fcclk * 4)                 //CCO frequence,should be 2、4、8、16 multiples of Fcclk, ranged from 156MHz to 320MHz. 
						    //CCO频率，必须为Fcclk的2、4、8、16倍，范围为156MHz~320MHz
#define Fpclk           (Fcclk / 4) * 4             //VPB clock frequence , must be 1、2、4 multiples of (Fcclk / 4).
						    //VPB时钟频率，只能为(Fcclk / 4)的1、2、4倍

//#include    "target.h"              //This line may not be deleted 这一句不能删除

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/


#endif
