NOINT        EQU      0xc0
NOIRQ        EQU      0x80
FIQMODE      EQU      0x11
IRQMODE     EQU       0x12
MODEMASK     EQU      0x1f
SYSMODE      EQU      0x1f
SVCMODE      EQU      0x13
INTOFFSET    EQU      0x4a000014

	EXPORT     	HAL_INTR_ENTRY
	EXPORT     	EXP_HANDLER
	EXPORT      HAL_INTR_DISABLE
	EXPORT     	HAL_INTR_ENABLE
	EXPORT     	HAL_INTR_DISABLE_SAVE
	EXPORT     	HAL_INTR_RESTORE
	EXTERN		StackIrq
	
	IMPORT		hal_all_entry
	IMPORT		acoral_intr_exit
	IMPORT		acoral_fault_entry

	
	CODE32
	
	AREA		|.text|, CODE, READONLY
	
HAL_INTR_ENTRY
    stmfd   sp!,{r0-r12,lr}         ;@保护寄存器,以及返回地址
    ldr r0,=INTOFFSET
    ldr r0,[r0]
    mrs     r1,spsr
    stmfd   sp!,{r1}            ;@保护SPSR_irq，以支持中断嵌套
    msr     cpsr_c,#SVCMODE|NOIRQ ;@进入SVCMODE,以便允许中断嵌套
    stmfd   sp!,{lr}            ;@保存lr_svc,

  
    mov     lr,pc           ;@这两条指令模拟函数调用(4G空间)，调用用户中断处理函数,
    ldr     pc,=hal_all_entry


    ldmfd   sp!,{lr}        ;@恢复lr_svc,
 
    msr     cpsr_c,#IRQMODE|NOINT    ;@更新cpsr,进入IRQ模式并禁止中断
    ldmfd   sp!,{r0}        ;@spsr->r0
    msr     spsr_cxsf,r0    ;@恢复spsr
    ldmfd   sp!,{r0-r12,lr}
    subs    pc,lr,#4        ;@此后，中断被重新打开

EXP_HANDLER
	stmfd   sp!,{lr}      ;@保护寄存器,以及返回地址
	mov     r0,sp
	stmfd   r0!,{sp}^     ;@出错线程的SP_sys压入exp中断栈中
	ldmfd   r0!,{r1}      ;@从exp中断栈中读取 SP_sys->R1
	mov     r0,lr
	mov		lr,pc
	ldr		pc,=acoral_fault_entry
	ldmfd   sp!,{lr}      ;@从exp中断栈中读取 SP_sys->R1
	subs pc,lr,#0

HAL_INTR_ENABLE
    mrs r0,cpsr
    bic r0,r0,#NOINT
    msr cpsr_cxsf,r0
    mov pc,lr

HAL_INTR_DISABLE
    mrs r0,cpsr
    mov r1,r0
    orr r1,r1,#NOINT
    msr cpsr_cxsf,r1
    mov pc ,lr

HAL_INTR_RESTORE
	MSR     CPSR_c, R0
	MOV     PC, LR

HAL_INTR_DISABLE_SAVE
	MRS     R0, CPSR				;@ Set IRQ and FIable all interrupts
	ORR     R1, R0, #0xC0
	MSR     CPSR_c, R1
	MRS     R1, CPSR				;@ Confirm that Cpt disable flags
	AND     R1, R1, #0xC0
	CMP     R1, #0xC0
	BNE     HAL_INTR_DISABLE_SAVE			;@ Not properly dsabled (try again)
	MOV     PC, LR					;@ Disabled, return thcontents in R0
