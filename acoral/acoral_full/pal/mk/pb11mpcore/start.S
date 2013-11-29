#include "hal_brd_cfg.h"

.global  __ENTRY
.global  ResetHandler
__ENTRY:
ResetHandler:
	MRS     r0, CPSR
        ORR     r0, r0, #(PSR_I_BIT|PSR_F_BIT)  @ Disable IRQ & FIQ
        MSR     CPSR_c, r0

        @
        @ Ensure that the MMU and caches are off
        @
        MOV     r0, #0
        MCR     p15, 0, r0, c7, c5, 0           @ Invalidate I Cache
        MCR     p15, 0, r0, c7, c6, 0           @ Invalidate D Cache

        MRC     p15, 0, r0, c1, c0, 0           @ Get control register
        BIC     r0, r0, #(CTRL_M_BIT|CTRL_C_BIT)@ Disable MMU and D Cache
        BIC     r0, r0, #CTRL_I_BIT             @ Disable I Cache
        MCR     p15, 0, r0, c1, c0, 0           @ Write control register

        @
        @ Handle secondary mpcores
        @
        MRC     p15, 0, r0, c0, c0, 5
        AND    r0, r0, #0x0f

	
        cmp	  r0, #1					//main CPU0
      
        BNE	  __secondary_mpcore 

        @@@@@@@@@@@@@@@
       @ BL	uart_set
        @@@@@@@@@@@@@@@
        
        @
        @ Clear the LED s
        @
clear_leds:
        MOV     r0, #BRD_BASE
        MOV     r1, #0
        STR     r1, [r0, #BRD_LED]

        @
        @ Initialise memory
        @
        
        @BL      __platform_memory_init

        @
        @ Are we running from our load address
        @
        LDR     r4, =dummy
        ADR     r1,dummy
        CMP     r1, r4
        BXEQ    r4

        @
        @ Get ready to copy ourselves to our execution address
        @
        ADR     r1, ResetHandler
        LDR     r2, =text_start
        LDR     r3, =bss_start
        CMP     r3, #0

copyloop:     
        LDR     r0, [r1], #4
        STR     r0, [r2], #4
        CMP     r2, r3
        BCC     copyloop

        @
        @ Ensure we are running the code at the address it is linked at.
        @
        BX      r4          @ Ensure we are at the linked address
dummy:

        @
        @ Initiallise static memory controller
        @
        @BL      __platform_static_memory_init

        @
        @ Create a small temporary stack, use the first 4K of memory
        @ as we sould always have some work memory here.  We can now
        @ call some simple C routines with care.
        @
        MOV     sp, #(4 << 10)          @ 4K stack

        @
        @ Basic initialization of the UART, this is so we have something
        @ to output any error message during boot.
        @
uart_set:        
        LDR      r1, =UART0_BASE
        MOV      r0, #0                     @ Disable UART
        STR      r0, [r1,#UART_PL011_CR]
        MOV      r0, #0x27                  @ Set speed to 38400bps
        STR      r0, [r1,#UART_PL011_IBRD]
        MOV      r0, #4
        STR      r0, [r1,#UART_PL011_FBRD]
        MOV      r0, #0x70                  @ 8 bits, 1 stop bit, no parity, fifo enabled
        STR      r0, [r1,#UART_PL011_LCRH]
        LDR      r0, =0x301                 @ Enable UART
        STR      r0, [r1,#UART_PL011_CR]

  @@@@@@@@@@@@@@@@@@@@@   


	@@@########@@@@@
	LDR 	 r8, =UART0_BASE	
	MOV r9, #65
	STR 	r9, [r8,#UART_PL01x_DR]
	@@@########@@@@@

/*
 zero_set:
	LDR      r1, =UART0_BASE	
	@LDR      r1, =0
        MOV     r2, #0
        MOV	    r0, #13
        STR	    r0, [r1,#UART_PL01x_DR]
        MOV     r0, #10
        STR	    r0, [r1,#UART_PL01x_DR]
loop_print:
        MOV	    r0, #68
        STR	    r0, [r1,#UART_PL01x_DR]
        ADD     r2, #1
        CMP     r2, #80
        BEQ	    zero_set
        bl 	    loop_print
*/
 @@@@@@@@@@@@@@@@@@@@@@@@

copy_vectors:
        LDR     r0, =HAL_VECTR_START
	LDR     r2, =0x0
	@LDR     r2, =0x01000000
        CMP     r0, r2
        BEQ     real_code
        LDR     r1, =HAL_VECTR_END
        @MOV     r2, #0
1:    
        LDR     r3, [r0], #4
        STR     r3, [r2], #4
        CMP     r0, r1
        BLO     1b

	@@@########@@@@@
	LDR 	 r8, =UART0_BASE	
	MOV r9, #66
	STR 	r9, [r8,#UART_PL01x_DR]
	@@@########@@@@@
	
real_code:

	bl InitStacks

	@@@###C####@@@@@
	LDR 	 r1, =UART0_BASE	
	MOV r0, #67
	STR 	r0, [r1,#UART_PL01x_DR]
	@@@########@@@@@
	
	ldr  r0,=bss_start
	ldr  r1,=bss_end
	bl    mem_clear

	mrs     r0,cpsr
       	bic     r0,r0,#MODE_MASK
	orr     r1,r0,#MODE_SYSTEM|NOINT
	msr     cpsr_cxsf,r1    	@ userMode

	ldr     sp,=SYS_stack

	@@@####D###@@@@@
	LDR 	 r1, =UART0_BASE	
	MOV r0, #68
	STR 	r0, [r1,#UART_PL01x_DR]
	@@@########@@@@@

        bl acoral_start
@
@ ------------------------------------------------------------
@






@***************************************************************
@                       堆栈初始化
@***************************************************************

InitStacks:
	mov r2,lr
	mrs	r0,cpsr
	bic	r0,r0,#MODE_MASK
	orr	r1,r0,#MODE_UNDEF|NOINT
	msr	cpsr_cxsf,r1		@UndefMode
	ldr	sp,=UDF_stack		@ UndefStack=0x33FF_5C00

	
	@@@####C####@@@@@
	LDR 	 r8, =UART0_BASE	
	MOV r9, #67
	STR 	r9, [r8,#UART_PL01x_DR]
	@@@########@@@@@
	
	orr	r1,r0,#MODE_ABORT|NOINT
	msr	cpsr_cxsf,r1		@AbortMode
	ldr	sp,=ABT_stack		@ AbortStack=0x33FF_6000

	@@@####D####@@@@@
	LDR 	 r8, =UART0_BASE	
	MOV r9, #68
	STR 	r9, [r8,#UART_PL01x_DR]
	@@@########@@@@@
	
	orr	r1,r0,#MODE_IRQ|NOINT
	msr	cpsr_cxsf,r1		@IRQMode
	ldr	sp,=IRQ_stack		@ IRQStack=0x33FF_7000

	@@@####E####@@@@@
	LDR 	 r8, =UART0_BASE	
	MOV r9, #69
	STR 	r9, [r8,#UART_PL01x_DR]
	@@@########@@@@@
	
	orr	r1,r0,#MODE_FIQ|NOINT
	msr	cpsr_cxsf,r1		@FIQMode
	ldr	sp,=FIQ_stack		@ FIQStack=0x33FF_8000

	@@@####F####@@@@@
	LDR 	 r8, =UART0_BASE	
	MOV r9, #70
	STR 	r9, [r8,#UART_PL01x_DR]
	@@@########@@@@@

	@bic	r0,r0,#MODE_MASK|NOINT
	@orr	r1,r0,#MODE_SYSTEM
	orr	r1,r0,#MODE_SYSTEM|NOINT
	msr	cpsr_cxsf,r1		@SVCMode
	ldr	sp,=SVC_stack		@ SVCStack=0x33FF_5800

	@@@####G####@@@@@
	LDR 	 r8, =UART0_BASE	
	MOV r9, #71
	STR 	r9, [r8,#UART_PL01x_DR]
	@@@########@@@@@
	
	mov	pc,r2

@***************************************************************
@ clear memory
@ r0: start address
@ r1: length
@***************************************************************

mem_clear:
	MOV R2,#0
1:	STR R2,[R0],#4
	CMP R0,R1
	BNE 1b
	MOV PC,LR

__secondary_mpcore:

	//b .             //stop here
	
	LDR     r8, =SC_BASE
        LDR     r6, [r8, #SC_CTRL]
        LDR     r0, =SC_REMAP_STAT
        TST     r6, r0
        BEQ     3f

        @
        @ If we are running from the first 64M of memory this
        @ is going to disable when we remap, therfore we need
        @ to jump to the real copy of this code in flash.
        @
        ADR     r1, 1f             @ Get relative address
        CMP     r1, #(64 << 20)     @ Is this in the first 64M
        BCS     3f                 @ No, skip

        @
        @ Check boot select switches
        @
        LDR     r5, =BRD_BASE
        LDR     r6, [r5, #BRD_BOOTCS]
        AND     r6, r6, #0x1

        @
        @ Move execution to the remapped area.
        @ This will either be NOR flash or Expansion Flash.
        @
        MOV     r0, #0
        CMP     r6, #0x0
        MOVEQ   r0, #NOR_FLASH     @ NOR Flash
        CMP     r6, #0x1
        MOVEQ   r0, #EXP_FLASH     @ Expansion Flash
        ADD     pc, r0, r1

1:      ADD     lr, r0, lr          @ Must update our return address

        @ Enable software interrupt
3:
        LDR     r5, =MPCORE_CPU_INTERFACE
        MOV     r6, #0x1
        STR     r6, [r5, #CONTROL_REGISTER]
        MOV     r6, #0xF0
        STR     r6, [r5, #PRIORITY_MASK_REGISTER]

	//clear    if 
	LDR     r5, =0x08000004
	LDR     r6, =0x0
        STR     r6, [r5, #0]
        
        @ Read core number into r0, required by application program
        @ on exit from wait for interrupt loop
        MRC     p15, 0, r0, c0, c0, 5
        AND     r0, r0, #0x0f

4:
        @Set WFI
        MCR     p15, 0, r2, c7, c0, 4 
        
        @ Read flag register to see if address to jump too

	MRC     p15, 0, r0, c0, c0, 5
        AND    r0, r0, #0x0f

	cmp     r0, #2
	beq      JMP_NEXT
	//MCR     p15, 0, r2, c7, c0, 4
JMP_TOP:
	LDR     r5, =0x08000004
        LDR     r6, [r5, #0]
        CMP     r6, #0
	BEQ     JMP_TOP
	
	
JMP_NEXT:	

        LDR     r5, =0x08000000
        LDR     r6, [r5, #0]
        CMP     r6, #0
        BXNE    r6
        B       4b
@这个不会回去
