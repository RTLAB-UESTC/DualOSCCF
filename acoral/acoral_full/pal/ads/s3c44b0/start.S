;acoral_zgl

;define the stack size
;定义堆栈的大小
FIQ_STACK_LEGTH         EQU         100
IRQ_STACK_LEGTH         EQU         200	             ;every layer need 9 bytes stack , permit 8 layer .每层嵌套需要9个字堆栈，允许8层嵌套
ABT_STACK_LEGTH         EQU         100
UND_STACK_LEGTH         EQU         0
SYS_STACK_LEGTH         EQU         200


;定义模式
NoInt       EQU 0xC0
USR32Mode   EQU 0x10
SVC32Mode   EQU 0x13
SYS32Mode   EQU 0x1f
IRQ32Mode   EQU 0x12
FIQ32Mode   EQU 0x11
MODE_MASK	EQU	0x1f

;The imported labels        
;引入的外部标号在这声明
    IMPORT  HAL_INTR_ENTRY                   ;Fast interrupt exceptions handler 快速中断异常处理程序
    IMPORT  acoral_start                          ;The entry point to the main function C语言主程序入口 
    IMPORT 	EXP_HANDLER                          ;err entry  
;	IMPORT	Mystacks
;	IMPORT	Bss_Start
;	IMPORT	Bss_End
;	IMPORT	Heap
;	IMPORT	HeapTop

;The emported labels        
;给外部使用的标号在这声明
	EXPORT  heap_end
    EXPORT  heap_start
    EXPORT  StackIrq

;全局变量在这里声明


	EXPORT  Reset
	;EXPORT HandleIRQ

;立即数声明
	
INTCON EQU		0x01E00000
INTMSK EQU		0x01E0000c
LOCKTIME EQU	0x01D8000c
PLLCON EQU		0x01D80000
CLKCON EQU		0x01D80004
WTCON  EQU		0x01D30000
vPLLCON EQU		0x48032				;40M hz
MEM_CTL_BASE	EQU	0x01c80000




	CODE32

	AREA  vectors, CODE, READONLY

	ENTRY
Reset
	;LDR		PC, ResetHandler
	b ResetHandler
	LDR 	PC,	HandleUndef	;handler for Undefined mode
	LDR 	PC,	HandleSWI       ;handler for SWI interrupt
	LDR 	PC,	HandlePabort	;handler for PAbort
	LDR 	PC,	HandleDabort	;handler for DAbort
	B       .			        ;reserved
	LDR		PC, HandleIRQ 	;handler for IRQ interrupt
	LDR		PC,	HandleFIQ	;handler for FIQ interrupt
;0x20: magic number so we can verify that we only put
	;.long   0
;0x24:
	;.long   0
;0x28: where this was linked, so we can put it in memory in the right place
	;.long   __ENTRY
;0x2C: this contains the platform, cpu and machine id
	;.long   2410
;0x30:  capabilities
	;.long   0
;0x34:


	b   	.

;****************************************************************
; intvector setup
;****************************************************************
;ResetHandler		DCD		ResetHandler
HandleFIQ			DCD		acoral_start
HandleIRQ			DCD		HAL_INTR_ENTRY
HandleUndef			DCD		acoral_start
HandleSWI			DCD		acoral_start
HandleDabort		DCD		acoral_start
HandlePabort		DCD 	acoral_start



;*************************************************************
;*************************************************************
;*堆栈初始化
;*************************************************************

InitStack    
        MOV     R0, LR
;Build the SVC stack
;设置中断模式堆栈
        MSR     CPSR_c, #0xd2
        LDR     SP, StackIrq
;Build the FIQ stack	
;设置快速中断模式堆栈
        MSR     CPSR_c, #0xd1
        LDR     SP, StackFiq
;Build the DATAABORT stack
;设置中止模式堆栈
        MSR     CPSR_c, #0xd7
        LDR     SP, StackAbt
;Build the UDF stack
;设置未定义模式堆栈
        MSR     CPSR_c, #0xdb
        LDR     SP, StackUnd
         
        MSR     CPSR_c, #0xd3
        LDR     SP, StackSys

;Build the SYS stack
;设置系统模式堆栈
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
	ldr	r1, =WTCON
	mov	r2, #0x0
	str	r2, [r1]

	;@ disable all interrupts
	ldr 	r1,=INTMSK
	ldr 	r0, =0x03fffeff
	str 	r0, [r1]

	ldr 	r1, =INTCON
	ldr 	r0, =0x05
	str 	r0, [r1]


	;@ initialise system clocks
	ldr 	r1, =LOCKTIME
	ldr 	r0, =800
	str 	r0, [r1]

	ldr 	r1, =PLLCON
	ldr 	r0, =vPLLCON
	;@ldr 	r0, =0x34031
	str 	r0,[r1]
		
	ldr 	r1,=CLKCON
	ldr 	r0,=0x7ff8
	str 	r0,[r1]
	bl	  memsetup
	
	
;@****************************************************************
;@ relocate 
;@****************************************************************
cur
  		adr     r0, cur            ; /* r0 <- current position of code   */
        ldr     r1, =cur           ;/* test if we run from flash or RAM */
        cmp     r0, r1             ;/* don't reloc during debug         */
        beq     end_copy 

;@****************************************************************
;@ Copy and paste RW data/zero initialized data 
;@****************************************************************
	mov     r1,#0
    ldr     r2,=Reset
    ldr     r3,=bss_start
copy_loop
	ldr     r0,[r1],#4
	str     r0,[r2],#4
	cmp     r2,r3
    blt     copy_loop
end_copy

	ldr  r0,=bss_start
	ldr  r1,=bss_end
	bl    mem_clear
	bl    InitStack
	LDR		pc,	=acoral_start	;@ call sys_start
	b 	.
	
	
	
	
	
;@***************************************************************
;@ initialise the static memory
;@ set memory control registers
;@***************************************************************

memsetup
	ldr	r1, =MEM_CTL_BASE
	adrl	r2, mem_cfg_val
	add	r3, r1, #52
1	ldr	r4, [r2], #4
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
1	str r2,[r0],#4
	cmp r0,r1
	blt %B1
	mov pc,lr

;@***************************************************************
;@ Data Area
;@ Memory configuration values
;@***************************************************************

ALIGN
mem_cfg_val
vBWSCON  	DCD	0x9000000
vBANKCON0	DCD	0x400
vBANKCON1	DCD	0x00007ffc
vBANKCON2	DCD	0x00007ffc
vBANKCON3	DCD	0x00007ffc
vBANKCON4	DCD	0x00007ffc
vBANKCON5	DCD	0x00007ffc
vBANKCON6	DCD	0x18000
vBANKCON7	DCD	0x18000
vREFRESH	DCD	0x84041b
vBANKSIZE	DCD	0x36
vMRSRB6		DCD	0x20
vMRSRB7		DCD	0x20


;**************************************************************
;*堆栈分配;/* 分配堆栈空间 */
;**************************************************************
StackIrq           DCD     IrqStackSpace + (IRQ_STACK_LEGTH - 1)* 4
StackFiq           DCD     FiqStackSpace + (FIQ_STACK_LEGTH - 1)* 4
StackAbt           DCD     AbtStackSpace + (ABT_STACK_LEGTH - 1)* 4
StackUnd           DCD     UndtStackSpace + (UND_STACK_LEGTH - 1)* 4
StackSys           DCD     SysStackSpace + (SYS_STACK_LEGTH - 1)* 4





        AREA    MyStacks, DATA, NOINIT, ALIGN=2
IrqStackSpace      SPACE   IRQ_STACK_LEGTH * 4  ;Stack spaces for Interrupt ReQuest Mode 中断模式堆栈空间
FiqStackSpace      SPACE   FIQ_STACK_LEGTH * 4  ;Stack spaces for Fast Interrupt reQuest Mode 快速中断模式堆栈空间
AbtStackSpace      SPACE   ABT_STACK_LEGTH * 4  ;Stack spaces for Suspend Mode 中止义模式堆栈空间
UndtStackSpace     SPACE   UND_STACK_LEGTH * 4  ;Stack spaces for Undefined Mode 未定义模式堆栈
SysStackSpace      SPACE   SYS_STACK_LEGTH * 4  ;Stack spaces for Undefin
;***********************************************************
;*段名称定义
;***********************************************************
        AREA    Bss_Start, DATA, NOINIT
bss_start    SPACE   1

        AREA    Bss_End, DATA, NOINIT
bss_end   SPACE   1

        AREA    HeapStart, DATA, NOINIT
heap_start    SPACE   1

        AREA    HeapEnd, DATA, NOINIT
heap_end   SPACE   1






	
	END
