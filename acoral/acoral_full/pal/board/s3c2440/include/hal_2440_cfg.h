#ifndef HAL_2440_CFG_H
#define HAL_2440_CFG_H

#define FCLK 400000000
#define HCLK 100000000
#define PCLK 50000000
#define M_MDIV    0x7f 
#define M_PDIV    0x2
#define M_SDIV    0x1
#define M_DIVN    0x5
#define vMPLLCON	((M_MDIV << 12) | (M_PDIV << 4) | (M_SDIV)) 
/* initial values for DRAM */
#define vBWSCON			0x22111110
#define vBANKCON0		0x00000700
#define vBANKCON1		0x00000700
#define vBANKCON2		0x00000700
#define vBANKCON3		0x00000700
#define vBANKCON4		0x00000700
#define vBANKCON5		0x00000700
#define vBANKCON6		0x00018009
#define vBANKCON7		0x00018009
#define vREFRESH                0x008e04eb
#define vBANKSIZE		0xB2
#define vMRSRB6			0x30
#define vMRSRB7			0x30

#endif
