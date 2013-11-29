;define the stack size
;定义堆栈的大小
FIQ_STACK_LEGTH         EQU         100
IRQ_STACK_LEGTH         EQU         100	             ;every layer need 9 bytes stack , permit 8 layer .每层嵌套需要9个字堆栈，允许8层嵌套
ABT_STACK_LEGTH         EQU         100
UND_STACK_LEGTH         EQU         0
SVC_STACK_LEGTH			EQU	        100

NoInt       EQU 0x80

USR32Mode   EQU 0x10
SVC32Mode   EQU 0x13
SYS32Mode   EQU 0x1f
IRQ32Mode   EQU 0x12
FIQ32Mode   EQU 0x11

PINSEL2     EQU 0xE002C014

BCFG0       EQU 0xFFE00000
BCFG1       EQU 0xFFE00004
BCFG2       EQU 0xFFE00008
BCFG3       EQU 0xFFE0000C


;The imported labels        
;引入的外部标号在这声明
    IMPORT  HAL_INTR_ENTRY                   ;Fast interrupt exceptions handler 快速中断异常处理程序
    IMPORT  acoral_start                          ;The entry point to the main function C语言主程序入口 
    IMPORT EXP_HANDLER                          ;err entry
   ; IMPORT TargetResetInit  
;The emported labels        
;给外部使用的标号在这声明
	EXPORT  heap_end
    EXPORT  heap_start
    EXPORT  StackIrq
    

    CODE32

    AREA    vectors,CODE,READONLY
        ENTRY
;interrupt vectors
;中断向量表
Reset
        LDR     PC, ResetAddr
        LDR     PC, Exp_Addr
        LDR     PC, Exp_Addr
        LDR     PC, Exp_Addr
        LDR     PC, Exp_Addr
        DCD     0xb9205f80
        LDR     PC, [PC, #-0xff0]
        LDR     PC, FIQ_Addr

ResetAddr           DCD     ResetInit
Exp_Addr			DCD		EXP_HANDLER
IRQ_Addr            DCD     HAL_INTR_ENTRY
FIQ_Addr            DCD     HAL_INTR_ENTRY


;/*********************************************************************************************************
;** unction name 	函数名称: 	InitStack
;** Descriptions 	功能描述: 	Initialize the stacks  初始化堆栈
;** input parameters 	输　入:   	None 无
;** Returned value   	输　出 :  	None 无
;** Used global variables 全局变量: 	None 无
;** Calling modules 	调用模块: 	None 无
;** 
;** Created by 		作　者: 	Chenmingji 陈明计
;** Created Date 	日　期: 	2004/02/02 2004年2月2日
;**-------------------------------------------------------------------------------------------------------
;** Modified by 	修　改: 
;** Modified date 	日　期: 
;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
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
;Build the SYS stack
;设置系统模式堆栈
        MSR     CPSR_c, #0xd3
        LDR     SP, StackSvc
        
		MSR     CPSR_c, #0xdf
        LDR     SP, StackSvc
        MOV     PC, R0

;/*********************************************************************************************************
;** unction name 	函数名称: 	ResetInit
;** Descriptions 	功能描述: 	RESET  复位入口
;** input parameters 	输　入:   	None 无
;** Returned value   	输　出 :  	None 无
;** Used global variables 全局变量: 	None 无
;** Calling modules 	调用模块: 	None 无
;** 
;** Created by 		作　者: 	Chenmingji 陈明计
;** Created Date 	日　期: 	2004/02/02 2004年2月2日
;**-------------------------------------------------------------------------------------------------------
;** Modified by 	修　改: Chenmingji 陈明计
;** Modified date 	日　期: 2004/02/02 2004年3月3日
;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
	IMPORT  |Image$$CODE$$Limit|  ; End of ROM code (=start of ROM data)
	IMPORT  |Image$$CODE$$Base|  ; Base of ROM code (=start of ROM data)
	IMPORT  |Image$$DATA$$Base|   ; Base of RAM to initialise
	IMPORT  |Image$$BSS$$Base|   ; Base and limit of area
	IMPORT  |Image$$BSS$$Limit|  ; to zero initialise

	IMPORT	acoral_start
	IMPORT  led

ResetInit
;Initial the extenal bus controller
;初始化外部总线控制器，根据目标板决定配置
		BL		led1
        LDR     R0, =PINSEL2
    IF :DEF: EN_CRP
        LDR     R1, =0x0f814910
    ELSE
        LDR     R1, =0x0f814914
    ENDIF
        STR     R1, [R0]

        LDR     R0, =BCFG0
        LDR     R1, =0x1000ffef				
        STR     R1, [R0]

        LDR     R0, =BCFG1
        LDR     R1, =0x10001460	;/* 配置外部RAM访问速度为最快 */ 
        STR     R1, [R0]

        LDR     R0, =BCFG2
        LDR     R1, =0x10001460				;/* 配置Bank2为16位总线模试，前速度为最快 */
        STR     R1, [R0]

;        LDR     R0, =BCFG3
;        LDR     R1, =0x2000ffef
;        STR     R1, [R0]
        
        BL      InitStack               ; Initialize the stack 初始化堆栈
       	
       ; BL       TargetResetInit
        LDR		R0,	=|Image$$CODE$$Limit|
       	LDR		R1,	=|Image$$DATA$$Base|
       	;CMP		R0, R1
       	;BEQ		star
       	LDR		R2, =|Image$$BSS$$Base|
copy_loop
		LDMIA	R0!, {R3-R10}		
		STMIA	R1!, {R3-R10}		
		CMP		R1, R2			
		BLE		copy_loop
		
clear_bss
		LDR		R0, =|Image$$BSS$$Base|		
		LDR		R1, =|Image$$BSS$$Limit|		
		MOV		R2, #0x00000000		

clbss_l
		STR		R2, [R0]		
		ADD		R0, R0, #4
		CMP		R0, R1
		BLE		clbss_l
star       	
       	LDR		PC, =acoral_start
        ;B       acoral_start
led1
 		LDR R0,=0xE0028028
		LDR R1,[R0]
		mov R2,#1
		ORR R1,R1,r2,lsl #28
		STR R1,[R0]
		
		LDR R0,=0xE0028024
		LDR R1,=0X10000000
		STR R1,[R0]		
		
		LDR R0,=0xE002802C
		LDR R1,=0X10000000
		STR R1,[R0]
		mov pc,lr
			      
StackIrq           DCD     IrqStackSpace + (IRQ_STACK_LEGTH - 1)* 4
StackFiq           DCD     FiqStackSpace + (FIQ_STACK_LEGTH - 1)* 4
StackAbt           DCD     AbtStackSpace + (ABT_STACK_LEGTH - 1)* 4
StackUnd           DCD     UndtStackSpace + (UND_STACK_LEGTH - 1)* 4
StackSvc           DCD     SvcStackSpace + (UND_STACK_LEGTH - 1)* 4

;/*********************************************************************************************************
;** unction name 	函数名称: 	CrpData
;** Descriptions 	功能描述: 	encrypt the chip
;** input parameters 	输　入:   	None 无
;** Returned value   	输　出 :  	None 无
;** Used global variables 全局变量: 	None 无
;** Calling modules 	调用模块: 	None 无
;** 
;** Created by 		作　者: 	Chenmingji 陈明计
;** Created Date 	日　期: 	2004/03/27 2004年3月27日
;**-------------------------------------------------------------------------------------------------------
;** Modified by 	修　改: 
;** Modified date 	日　期: 
;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
    IF :DEF: EN_CRP
        IF  . >= 0x1fc
        INFO    1,"\nThe data at 0x000001fc must be 0x87654321.\nPlease delete some source before this line."
        ENDIF
CrpData
    WHILE . < 0x1fc
    NOP
    WEND
CrpData1
    DCD     0x87654321          ;/*When the Data is 为0x87654321,user code be protected. 当此数为0x87654321时，用户程序被保护 */
    ENDIF

;/* 分配堆栈空间 */
        AREA    MyStacks, DATA, NOINIT, ALIGN=2
IrqStackSpace      SPACE   IRQ_STACK_LEGTH * 4  ;Stack spaces for Interrupt ReQuest Mode 中断模式堆栈空间
FiqStackSpace      SPACE   FIQ_STACK_LEGTH * 4  ;Stack spaces for Fast Interrupt reQuest Mode 快速中断模式堆栈空间
AbtStackSpace      SPACE   ABT_STACK_LEGTH * 4  ;Stack spaces for Suspend Mode 中止义模式堆栈空间
UndtStackSpace     SPACE   UND_STACK_LEGTH * 4  ;Stack spaces for Undefined Mode 未定义模式堆栈
SvcStackSpace      SPACE   SVC_STACK_LEGTH * 4  ;Stack spaces for Undefined Mode 未定义模式堆栈
  		 AREA    BssStart, DATA, NOINIT
bss_start    SPACE   1

        AREA    BssEnd, DATA, NOINIT
bss_end   SPACE   1

        AREA    HeapStart, DATA, NOINIT
heap_start    SPACE   1

        AREA    HeapEnd, DATA, NOINIT
heap_end   SPACE   1

    END
;/*********************************************************************************************************
;**                            End Of File
;********************************************************************************************************/