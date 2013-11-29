#ifndef HAL_2410_C_H
#define HAL_2410_C_H
#include"type.h"

/* INTERRUPT*/
#define rSRCPND     (*(volatile acoral_u32 *)0x4a000000)	/*Interrupt request status*/
#define rINTMOD     (*(volatile acoral_u32 *)0x4a000004)	/*Interrupt mode control*/
#define rINTMSK     (*(volatile acoral_u32 *)0x4a000008)	/*Interrupt mask control*/
#define rPRIORITY   (*(volatile acoral_u32 *)0x4a00000c)	/*IRQ priority control*/
#define rINTPND     (*(volatile acoral_u32 *)0x4a000010)	/*Interrupt request status*/
#define rINTOFFSET  (*(volatile acoral_u32 *)0x4a000014)	/*Interruot request source offset*/
#define rSUBSRCPND  (*(volatile acoral_u32 *)0x4a000018)	/*Sub source pending*/
#define rINTSUBMSK  (*(volatile acoral_u32 *)0x4a00001c)	/*Interrupt sub mask*/

/* UART*/
#define rULCON0     (*(volatile acoral_u32 *)0x50000000)	/*UART 0 Line control*/
#define rUCON0      (*(volatile acoral_u32 *)0x50000004)	/*UART 0 Control*/
#define rUFCON0     (*(volatile acoral_u32 *)0x50000008)	/*UART 0 FIFO control*/
#define rUMCON0     (*(volatile acoral_u32 *)0x5000000c)	/*UART 0 Modem control*/
#define rUTRSTAT0   (*(volatile acoral_u32 *)0x50000010)	/*UART 0 Tx/Rx status*/
#define rUERSTAT0   (*(volatile acoral_u32 *)0x50000014)	/*UART 0 Rx error status*/
#define rUFSTAT0    (*(volatile acoral_u32 *)0x50000018)	/*UART 0 FIFO status*/
#define rUMSTAT0    (*(volatile acoral_u32 *)0x5000001c)	/*UART 0 Modem status*/
#define rUBRDIV0    (*(volatile acoral_u32 *)0x50000028)	/*UART 0 Baud rate divisor*/

#define rULCON1     (*(volatile acoral_u32 *)0x50004000)	/*UART 1 Line control*/
#define rUCON1      (*(volatile acoral_u32 *)0x50004004)	/*UART 1 Control*/
#define rUFCON1     (*(volatile acoral_u32 *)0x50004008)	/*UART 1 FIFO control*/
#define rUMCON1     (*(volatile acoral_u32 *)0x5000400c)	/*UART 1 Modem control*/
#define rUTRSTAT1   (*(volatile acoral_u32 *)0x50004010)	/*UART 1 Tx/Rx status*/
#define rUERSTAT1   (*(volatile acoral_u32 *)0x50004014)	/*UART 1 Rx error status*/
#define rUFSTAT1    (*(volatile acoral_u32 *)0x50004018)	/*UART 1 FIFO status*/
#define rUMSTAT1    (*(volatile acoral_u32 *)0x5000401c)	/*UART 1 Modem status*/
#define rUBRDIV1    (*(volatile acoral_u32 *)0x50004028)	/*UART 1 Baud rate divisor*/
#define rULCON2     (*(volatile acoral_u32 *)0x50008000)	/*UART 2 Line control*/
#define rUCON2      (*(volatile acoral_u32 *)0x50008004)	/*UART 2 Control*/
#define rUFCON2     (*(volatile acoral_u32 *)0x50008008)	/*UART 2 FIFO control*/
#define rUMCON2     (*(volatile acoral_u32 *)0x5000800c)	/*UART 2 Modem control*/
#define rUTRSTAT2   (*(volatile acoral_u32 *)0x50008010)	/*UART 2 Tx/Rx status*/
#define rUERSTAT2   (*(volatile acoral_u32 *)0x50008014)	/*UART 2 Rx error status*/
#define rUFSTAT2    (*(volatile acoral_u32 *)0x50008018)	/*UART 2 FIFO status*/
#define rUMSTAT2    (*(volatile acoral_u32 *)0x5000801c)	/*UART 2 Modem status*/
#define rUBRDIV2    (*(volatile acoral_u32 *)0x50008028)	/*UART 2 Baud rate divisor*/

#ifdef __BIG_ENDIAN
#define WrUTXH0(ch) (*(volatile acoral_u8 *)0x50000023)=(acoral_u8)(ch)
#define RdURXH0()   (*(volatile acoral_u8 *)0x50000027)
#define WrUTXH1(ch) (*(volatile acoral_u8 *)0x50004023)=(acoral_u8)(ch)
#define RdURXH1()   (*(volatile acoral_u8 *)0x50004027)
#define WrUTXH2(ch) (*(volatile acoral_u8 *)0x50008023)=(acoral_u8)(ch)
#define RdURXH2()   (*(volatile acoral_u8 *)0x50008027)

#define UTXH0       (0x50000020+3)  /*Byte_access address by DMA*/
#define URXH0       (0x50000024+3)
#define UTXH1       (0x50004020+3)
#define URXH1       (0x50004024+3)
#define UTXH2       (0x50008020+3)
#define URXH2       (0x50008024+3)

#else /*Little Endian*/
#define rUTXH0 (*(volatile acoral_u8 *)0x50000020)	/*UART 0 Transmission Hold*/
#define rURXH0 (*(volatile acoral_u8 *)0x50000024)	/*UART 0 Receive buffer*/
#define rUTXH1 (*(volatile acoral_u8 *)0x50004020)	/*UART 1 Transmission Hold*/
#define rURXH1 (*(volatile acoral_u8 *)0x50004024)	/*UART 1 Receive buffer*/
#define rUTXH2 (*(volatile acoral_u8 *)0x50008020)	/*UART 2 Transmission Hold*/
#define rURXH2 (*(volatile acoral_u8 *)0x50008024)	/*UART 2 Receive buffer*/

#define WrUTXH0(ch) (*(volatile acoral_u8 *)0x50000020)=(acoral_u8)(ch)
#define RdURXH0()   (*(volatile acoral_u8 *)0x50000024)
#define WrUTXH1(ch) (*(volatile acoral_u8 *)0x50004020)=(acoral_u8)(ch)
#define RdURXH1()   (*(volatile acoral_u8 *)0x50004024)
#define WrUTXH2(ch) (*(volatile acoral_u8 *)0x50008020)=(acoral_u8)(ch)
#define RdURXH2()   (*(volatile acoral_u8 *)0x50008024)
#endif


/* PWM TIMER*/
#define rTCFG0  (*(volatile acoral_u32 *)0x51000000)	/*Timer 0 configuration*/
#define rTCFG1  (*(volatile acoral_u32 *)0x51000004)	/*Timer 1 configuration*/
#define rTCON   (*(volatile acoral_u32 *)0x51000008)	/*Timer control*/
#define rTCNTB0 (*(volatile acoral_u32 *)0x5100000c)	/*Timer count buffer 0*/
#define rTCMPB0 (*(volatile acoral_u32 *)0x51000010)	/*Timer compare buffer 0*/
#define rTCNTO0 (*(volatile acoral_u32 *)0x51000014)	/*Timer count observation 0*/
#define rTCNTB1 (*(volatile acoral_u32 *)0x51000018)	/*Timer count buffer 1*/
#define rTCMPB1 (*(volatile acoral_u32 *)0x5100001c)	/*Timer compare buffer 1*/
#define rTCNTO1 (*(volatile acoral_u32 *)0x51000020)	/*Timer count observation 1*/
#define rTCNTB2 (*(volatile acoral_u32 *)0x51000024)	/*Timer count buffer 2*/
#define rTCMPB2 (*(volatile acoral_u32 *)0x51000028)	/*Timer compare buffer 2*/
#define rTCNTO2 (*(volatile acoral_u32 *)0x5100002c)	/*Timer count observation 2*/
#define rTCNTB3 (*(volatile acoral_u32 *)0x51000030)	/*Timer count buffer 3*/
#define rTCMPB3 (*(volatile acoral_u32 *)0x51000034)	/*Timer compare buffer 3*/
#define rTCNTO3 (*(volatile acoral_u32 *)0x51000038)	/*Timer count observation 3*/
#define rTCNTB4 (*(volatile acoral_u32 *)0x5100003c)	/*Timer count buffer 4*/
#define rTCNTO4 (*(volatile acoral_u32 *)0x51000040)	/*Timer count observation 4*/

/* I/O PORT */
#define rGPACON    (*(volatile acoral_u32 *)0x56000000)	/*Port A control*/
#define rGPADAT    (*(volatile acoral_u32 *)0x56000004)	/*Port A data*/

#define rGPBCON    (*(volatile acoral_u32 *)0x56000010)	/*Port B control*/
#define rGPBDAT    (*(volatile acoral_u32 *)0x56000014)	/*Port B data*/
#define rGPBUP     (*(volatile acoral_u32 *)0x56000018)	/*Pull-up control B*/

#define rGPCCON    (*(volatile acoral_u32 *)0x56000020)	/*Port C control*/
#define rGPCDAT    (*(volatile acoral_u32 *)0x56000024)	/*Port C data*/
#define rGPCUP     (*(volatile acoral_u32 *)0x56000028)	/*Pull-up control C*/

#define rGPDCON    (*(volatile acoral_u32 *)0x56000030)	/*Port D control*/
#define rGPDDAT    (*(volatile acoral_u32 *)0x56000034)	/*Port D data*/
#define rGPDUP     (*(volatile acoral_u32 *)0x56000038)	/*Pull-up control D*/

#define rGPECON    (*(volatile acoral_u32 *)0x56000040)	/*Port E control*/
#define rGPEDAT    (*(volatile acoral_u32 *)0x56000044)	/*Port E data*/
#define rGPEUP     (*(volatile acoral_u32 *)0x56000048)	/*Pull-up control E*/

#define rGPFCON    (*(volatile acoral_u32 *)0x56000050)	/*Port F control*/
#define rGPFDAT    (*(volatile acoral_u32 *)0x56000054)	/*Port F data*/
#define rGPFUP     (*(volatile acoral_u32 *)0x56000058)	/*Pull-up control F*/

#define rGPGCON    (*(volatile acoral_u32 *)0x56000060)	/*Port G control*/
#define rGPGDAT    (*(volatile acoral_u32 *)0x56000064)	/*Port G data*/
#define rGPGUP     (*(volatile acoral_u32 *)0x56000068)	/*Pull-up control G*/

#define rGPHCON    (*(volatile acoral_u32 *)0x56000070)	/*Port H control*/
#define rGPHDAT    (*(volatile acoral_u32 *)0x56000074)	/*Port H data*/
#define rGPHUP     (*(volatile acoral_u32 *)0x56000078)	/*Pull-up control H*/

#define rGPJCON    (*(volatile acoral_u32 *)0x560000d0)	/*Port J control*/
#define rGPJDAT    (*(volatile acoral_u32 *)0x560000d4)	/*Port J data*/
#define rGPJUP     (*(volatile acoral_u32 *)0x560000d8)	/*Pull-up control J*/

#define rMISCCR    (*(volatile acoral_u32 *)0x56000080)	/*Miscellaneous control*/
#define rDCLKCON   (*(volatile acoral_u32 *)0x56000084)	/*DCLK0/1 control*/
#define rEXTINT0   (*(volatile acoral_u32 *)0x56000088)	/*External interrupt control register 0*/
#define rEXTINT1   (*(volatile acoral_u32 *)0x5600008c)	/*External interrupt control register 1*/
#define rEXTINT2   (*(volatile acoral_u32 *)0x56000090)	/*External interrupt control register 2*/
#define rEINTFLT0  (*(volatile acoral_u32 *)0x56000094)	/*Reserved*/
#define rEINTFLT1  (*(volatile acoral_u32 *)0x56000098)	/*Reserved*/
#define rEINTFLT2  (*(volatile acoral_u32 *)0x5600009c)	/*External interrupt filter control register 2*/
#define rEINTFLT3  (*(volatile acoral_u32 *)0x560000a0)	/*External interrupt filter control register 3*/
#define rEINTMSK  (*(volatile acoral_u32 *)0x560000a4)	/*External interrupt mask*/
#define rEINTPND  (*(volatile acoral_u32 *)0x560000a8)	/*External interrupt pending*/
#define rGSTATUS0  (*(volatile acoral_u32 *)0x560000ac)	/*External pin status*/
#define rGSTATUS1  (*(volatile acoral_u32 *)0x560000b0)	/*Chip ID(0x32440000)*/
#define rGSTATUS2  (*(volatile acoral_u32 *)0x560000b4)	/*Reset type*/
#define rGSTATUS3  (*(volatile acoral_u32 *)0x560000b8)	/*Saved data0(32-bit) before entering POWER_OFF mode */
#define rGSTATUS4  (*(volatile acoral_u32 *)0x560000bc)	/*Saved data0(32-bit) before entering POWER_OFF mode */



/*mmu*/
#define DESC_SEC	(0x2|(1<<4))
#define CB		(3<<2)  /*cache_on, write_back*/
#define CNB		(2<<2)  /*cache_on, write_through */
#define NCB             (1<<2)  /*cache_off,WR_BUF on*/
#define NCNB		(0<<2)  /*cache_off,WR_BUF off*/
#define AP_RW		(3<<10) /*supervisor=RW, user=RW*/
#define AP_RO		(2<<10) /*supervisor=RW, user=RO*/

#define DOMAIN_FAULT	(0x0)
#define DOMAIN_CHK	(0x1) 
#define DOMAIN_NOTCHK	(0x3) 
#define DOMAIN0		(0x0<<5)
#define DOMAIN1		(0x1<<5)

#define DOMAIN0_ATTR	(DOMAIN_CHK<<0) 
#define DOMAIN1_ATTR	(DOMAIN_FAULT<<2) 

#define RW_CB		(AP_RW|DOMAIN0|CB|DESC_SEC)
#define RW_CNB		(AP_RW|DOMAIN0|CNB|DESC_SEC)
#define RW_NCNB		(AP_RW|DOMAIN0|NCNB|DESC_SEC)
#define RW_FAULT	(AP_RW|DOMAIN1|NCNB|DESC_SEC)





/*Nand Flash*/
#define __REGb(x)	(*(volatile unsigned char *)(x))
#define __REGw(x)	(*(volatile unsigned short *)(x))
#define __REGi(x)	(*(volatile unsigned int *)(x))

#define NF_BASE		0x4e000000
#define NFCONF		__REGi(NF_BASE + 0x0)
#define NFCMD		__REGb(NF_BASE + 0x4)
#define NFADDR		__REGb(NF_BASE + 0x8)
#define NFDATA		__REGb(NF_BASE + 0xc)
#define NFSTAT		__REGb(NF_BASE + 0x10)
#define NFSTAT_BUSY	1
#define nand_select()	(NFCONF &= ~0x800)
#define nand_deselect()	(NFCONF |= 0x800)
#define nand_clear_RnB()	do {} while (0)



/* Standard NAND flash commands*/                                                                                                            

 #define NAND_CMD_READ0          0
 #define NAND_CMD_READ1          1
 #define NAND_CMD_RNDOUT         5
 #define NAND_CMD_PAGEPROG       0x10                                                                                                          
#define NAND_CMD_READOOB         0x50                                                                                                          
 #define NAND_CMD_ERASE1         0x60                                                                                                          
 #define NAND_CMD_STATUS         0x70
 #define NAND_CMD_STATUS_MULTI   0x71                                                                                                          
 #define NAND_CMD_SEQIN          0x80                                                                                                          
 #define NAND_CMD_RNDIN          0x85                                                                                                          
 #define NAND_CMD_READID         0x90                                                                                                          
 #define NAND_CMD_ERASE2         0xd0                                                                                                          
 #define NAND_CMD_RESET          0xff
                 
 /* Extended commands for large page devices */
 #define NAND_CMD_READSTART      0x30
 #define NAND_CMD_RNDOUTSTART    0xE0 
 #define NAND_CMD_CACHEDPROG     0x15 





#endif
