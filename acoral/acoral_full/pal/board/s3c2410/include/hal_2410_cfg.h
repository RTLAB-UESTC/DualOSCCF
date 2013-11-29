#ifndef HAL_2410_CFG_H
#define HAL_2410_CFG_H

#define FCLK 120000000
#define HCLK 60000000
#define PCLK 30000000
#define M_MDIV    0x5c 
#define M_PDIV    0x4
#define M_SDIV    0x0
#define M_DIVN    0x3
#define vMPLLCON		((M_MDIV << 12) | (M_PDIV << 4) | (M_SDIV)) 
#define UART_BAUD_RATE 115200

/* initial values for DRAM */
#define vBWSCON			0x22111110
#define vBANKCON0		0x00000700
#define vBANKCON1		0x00000700
#define vBANKCON2		0x00000700
#define vBANKCON3		0x00000700
#define vBANKCON4		0x00000700
#define vBANKCON5		0x00000700
#define vBANKCON6		0x00018005
#define vBANKCON7		0x00018005
#define vREFRESH		0x008e0459
#define vBANKSIZE		0xb2
#define vMRSRB6			0x30
#define vMRSRB7			0x30

#endif
