;define the stack size
;∂®“Â∂—’ªµƒ¥Û–°
FIQ_STACK_LEGTH         EQU         100
IRQ_STACK_LEGTH         EQU         200	             ;every layer need 9 bytes stack , permit 8 layer .√ø≤„«∂Ã◊–Ë“™9∏ˆ◊÷∂—’ª£¨‘ –Ì8≤„«∂Ã◊
ABT_STACK_LEGTH         EQU         100
UND_STACK_LEGTH         EQU         100
SYS_STACK_LEGTH         EQU         400

;∂®“Âƒ£ Ω
NoInt       EQU 0xC0
USR32Mode   EQU 0x10
SVC32Mode   EQU 0x13
SYS32Mode   EQU 0x1f
IRQ32Mode   EQU 0x12
FIQ32Mode   EQU 0x11
MODE_MASK	EQU	0x1f

;The imported labels        
;“˝»ÎµƒÕ‚≤ø±Í∫≈‘⁄’‚…˘√˜
    IMPORT  HAL_INTR_ENTRY                   ;Fast interrupt exceptions handler øÏÀŸ÷–∂œ“Ï≥£¥¶¿Ì≥Ã–Ú
    IMPORT  acoral_start                          ;The entry point to the main function C”Ô—‘÷˜≥Ã–Ú»Îø⁄ 
    IMPORT 	EXP_HANDLER                          ;err entry  

;The emported labels        
;∏¯Õ‚≤ø π”√µƒ±Í∫≈‘⁄’‚…˘√˜
	EXPORT  heap_end
    EXPORT  heap_start
    EXPORT  StackIrq
	EXPORT	MMU_base

;»´æ÷±‰¡ø‘⁄’‚¿Ô…˘√˜


	EXPORT  Reset



	;INTCON 		EQU		0x01E00000
INTMSK 			EQU		0x4A000008
LOCKTIME		EQU		0x4C000000
PLLCON 			EQU		0x4C000004
CLKDIVN			EQU		0x4C000014
WTCON  			EQU		0x53000000
;vPLLCON 		EQU		0x48032				;40M hz
MEM_CTL_BASE	EQU		0x48000000
M_DIVN    		EQU		0x3


	CODE32
	
	AREA	vectors, CODE,READONLY
	
	ENTRY
	
Reset

	b ResetHandler
	LDR 	PC,	HandleUndef	;handler for Undefined mode
	LDR 	PC,	HandleSWI       ;handler for SWI interrupt
	LDR 	PC,	HandlePabort	;handler for PAbort
	LDR 	PC,	HandleDabort	;handler for DAbort
	B       .			        ;reserved
	LDR		PC, HandleIRQ 	;handler for IRQ interrupt
	LDR		PC,	HandleFIQ	;handler for FIQ interrupt
	
	B		.
	
	
	

;@****************************************************************
;@ intvector setup
;@****************************************************************

;ResetHandler		DCD		ResetHandler
HandleFIQ			DCD		acoral_start
HandleIRQ			DCD		HAL_INTR_ENTRY
HandleUndef			DCD		acoral_start
HandleSWI			DCD		acoral_start
HandleDabort		DCD		acoral_start
HandlePabort		DCD 	acoral_start



;*************************************************************
;*************************************************************
;*∂—’ª≥ı ºªØ
;*************************************************************

InitStack    
        MOV     R0, LR
;Build the SVC stack
;…Ë÷√÷–∂œƒ£ Ω∂—’ª
 		MSR     CPSR_c, #0xd2
        LDR     SP, StackIrq
;Build the FIQ stack	
;…Ë÷√øÏÀŸ÷–∂œƒ£ Ω∂—’ª
        MSR     CPSR_c, #0xd1
        LDR     SP, StackFiq
;Build the DATAABORT stack
;…Ë÷√÷–÷πƒ£ Ω∂—’ª
        MSR     CPSR_c, #0xd7
        LDR     SP, StackAbt
;Build the UDF stack
;…Ë÷√Œ¥∂®“Âƒ£ Ω∂—’ª
        MSR     CPSR_c, #0xdb
        LDR     SP, StackUnd
        
        MSR     CPSR_c, #0xd3
        LDR     SP, StackSys

;Build the SYS stack
;…Ë÷√œµÕ≥ƒ£ Ω∂—’ª
        MSR     CPSR_c, #0xdf
        LDR     SP, StackSys
 
        MOV     PC, R0
        
        
        
        
        
;@****************************************************************
;@             ResetHandler fuction
;@****************************************************************

ResetHandler
    
    ;set the cpu to svc32 mode
	mrs 	r0,cpsr
	bic 	r0,r0,#0x1f
	orr 	r0,r0,#0x13
	msr 	cpsr_c,r0
       

    
       	;@ disable watch dog timer
	mov	r1, #0x53000000
	mov	r2, #0x0
	str	r2, [r1]

	;disable all interrupts
	
	ldr 	r1,=INTMSK
	ldr 	r0, =0xffffffff
	str 	r0, [r1]
	
	
	;@ initialise system clocks
	;mov	r1, #LOCKTIME
	;mvn	r2, #0xff000000
	;str	r2, [r1]

;@1:2:4
	
	;ldr	r1, =CLKDIVN
	;mov	r2, #M_DIVN
	;str	r2, [r1]

	mrc	p15, 0, r1, c1, c0, 0		;@ read ctrl register
	orr	r1, r1, #0xc0000000			;@ Asynchronous
	mcr	p15, 0, r1, c1, c0, 0		;@ write ctrl register


;@ the default flck=120M
;@	mov	r1, #CLK_CTL_BASE
;@	ldr 	r2, =vMPLLCON	       	;@ clock user set
;@	str	r2, [r1, #oMPLLCON]

	;bl	memsetup

	bl     InitStack

	ldr  r0,=bss_start
	ldr  r1,=bss_end
	bl    mem_clear

    	

	ldr    pc,=acoral_start		;@ call sys_start
	b 	.



;@***************************************************************
;@                       Â†ÜÊ†àÂàùÂßãÂåñ
;@***************************************************************

;InitStacks
;	mov r2,lr
;	mrs	r0,cpsr
;	bic	r0,r0,#MODE_MASK
;	orr	r1,r0,#UND_MODE|NOINT
;	msr	cpsr_cxsf,r1		;@UndefMode
;	ldr	sp,=UDF_stack		;@ UndefStack=0x33FF_5C00
;
;	orr	r1,r0,#ABT_MODE|NOINT
;	msr	cpsr_cxsf,r1		;@AbortMode
;	ldr	sp,=ABT_stack		;@ AbortStack=0x33FF_6000

;	orr	r1,r0,#IRQ_MODE|NOINT
;	msr	cpsr_cxsf,r1		;@IRQMode
;	ldr	sp,=IRQ_stack		;@ IRQStack=0x33FF_7000

;	orr	r1,r0,#FIQ_MODE|NOINT
;	msr	cpsr_cxsf,r1		;@FIQMode
;	ldr	sp,=FIQ_stack		;@ FIQStack=0x33FF_8000
;
;	bic	r0,r0,#MODE_MASK|NOINT
;	orr	r1,r0,#SVC_MODE
;	msr	cpsr_cxsf,r1		;@SVCMode
;	ldr	sp,=SVC_stack		;@ SVCStack=0x33FF_5800
;	mov	pc,r2
;*/
;@***************************************************************
;@ initialise the static memory
;@ set memory control registers
;@***************************************************************

memsetup
	mov	r1, #MEM_CTL_BASE
	adrl	r2, mem_cfg_val
	add	r3, r1, #52
1
	ldr	r4, [r2], #4
	str	r4, [r1], #4
	cmp	r1, r3
	bne	%B1
	mov	pc, lr

;@***************************************************************
;@ clear memory
;@ r0: start address
;@ r1: length
;@***************************************************************

mem_clear
	mov r2,#0
1
	str r2,[r0],#4
	cmp r0,r1
	blt %B1
	mov pc,lr


;@***************************************************************
;@ Data Area
;@ Memory configuration values
;@***************************************************************

ALIGN
mem_cfg_val
vBWSCON		DCD		0x22111110
vBANKCON0	DCD		0x00000700
vBANKCON1	DCD		0x00000700
vBANKCON2	DCD		0x00000700
vBANKCON3	DCD		0x00000700
vBANKCON4	DCD		0x00000700
vBANKCON5	DCD		0x00000700
vBANKCON6	DCD		0x00018005
vBANKCON7	DCD		0x00018005
vREFRESH	DCD		0x008e0459
vBANKSIZE	DCD		0xb2
vMRSRB6		DCD		0x30
vMRSRB7		DCD		0x30
;**************************************************************
;*∂—’ª∑÷≈‰;/* ∑÷≈‰∂—’ªø’º‰ */
;**************************************************************
StackIrq           DCD     IrqStackSpace + (IRQ_STACK_LEGTH - 1)* 4
StackFiq           DCD     FiqStackSpace + (FIQ_STACK_LEGTH - 1)* 4
StackAbt           DCD     AbtStackSpace + (ABT_STACK_LEGTH - 1)* 4
StackUnd           DCD     UndtStackSpace + (UND_STACK_LEGTH - 1)* 4
StackSys           DCD     SysStackSpace + (SYS_STACK_LEGTH - 1)* 4





        AREA    MyStacks, DATA, NOINIT, ALIGN=2
IrqStackSpace      SPACE   IRQ_STACK_LEGTH * 4  ;Stack spaces for Interrupt ReQuest Mode ÷–∂œƒ£ Ω∂—’ªø’º‰
FiqStackSpace      SPACE   FIQ_STACK_LEGTH * 4  ;Stack spaces for Fast Interrupt reQuest Mode øÏÀŸ÷–∂œƒ£ Ω∂—’ªø’º‰
AbtStackSpace      SPACE   ABT_STACK_LEGTH * 4  ;Stack spaces for Suspend Mode ÷–÷π“Âƒ£ Ω∂—’ªø’º‰
UndtStackSpace     SPACE   UND_STACK_LEGTH * 4  ;Stack spaces for Undefined Mode Œ¥∂®“Âƒ£ Ω∂—’ª
SysStackSpace      SPACE   SYS_STACK_LEGTH * 4  ;Stack spaces for Undefin


;***********************************************************
;*∂Œ√˚≥∆∂®“Â
;***********************************************************
        AREA    Bss_Start, DATA, NOINIT
bss_start    SPACE   1

        AREA    Bss_End, DATA, NOINIT
bss_end   SPACE   1

        AREA    HeapStart, DATA, NOINIT
heap_start    SPACE   1

        AREA    HeapEnd, DATA, NOINIT
heap_end   SPACE   1

        AREA   MMU, DATA, NOINIT
MMU_base   SPACE   1






	
	END