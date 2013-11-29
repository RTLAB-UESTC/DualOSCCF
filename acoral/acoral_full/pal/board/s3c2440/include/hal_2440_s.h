#ifndef HAL_2440_S_H
#define HAL_2440_S_H
#include "hal_2440_cfg.h"
#define USR26_MODE		0x00
#define FIQ26_MODE		0x01
#define IRQ26_MODE		0x02
#define SVC26_MODE		0x03
#define USR_MODE		0x10
#define FIQ_MODE		0x11
#define IRQ_MODE		0x12
#define SVC_MODE		0x13
#define ABT_MODE		0x17
#define UND_MODE		0x1b
#define SYS_MODE		0x1f
#define MODE_MASK		0x1f
#define NOINT        	0xc0

/* Memory Controller */
#define MEM_CTL_BASE		0x48000000

/* Clock and Power Management */
#define CLK_CTL_BASE		0x4C000000

/* Offset */
#define oLOCKTIME		0x00	/* R/W, PLL lock time count register */
#define oMPLLCON		0x04	/* R/W, MPLL configuration register */
#define oCLKDIVN		0x14	/* R/W, Clock divider control */

/* Interrupts */
#define INT_CTL_BASE		0x4A000000
#define oINTMSK			0x08
#define oINTSUBMSK		0x1C

#endif
