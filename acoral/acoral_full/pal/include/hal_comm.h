#ifndef HAL_COMM_H
#define HAL_COMM_H
#include<type.h>
typedef acoral_u32 acoral_sr;
#include<hal_port.h>
#define HAL_MAX_CPU   CFG_MAX_CPU
#define HAL_REG(base,offset) (*(volatile acoral_u32 *)((base) + (offset)))

/****************************                                                                                                                 
* the comm interrupt interface of hal     
*  hal层中断部分通用接口
*****************************/
#define HAL_START_OS(stack) HAL_SWITCH_TO(stack)
#define HAL_INTR_NESTING_INIT()   hal_intr_nesting_init_comm()
#define HAL_GET_INTR_NESTING()    hal_get_intr_nesting_comm()
#define HAL_INTR_NESTING_DEC()    hal_intr_nesting_dec_comm()
#define HAL_INTR_NESTING_INC()    hal_intr_nesting_inc_comm()
#define HAL_ENTER_CRITICAL()  (cpu_sr = HAL_INTR_DISABLE_SAVE())
#define HAL_EXIT_CRITICAL()  (HAL_INTR_RESTORE(cpu_sr))
#define HAL_INTR_SPECIAL()
#define HAL_INTR_ATTACH(vecotr,isr)
#define HAL_SCHED_BRIDGE() hal_sched_bridge_comm()
#define HAL_INTR_EXIT_BRIDGE() hal_intr_exit_bridge_comm()

#define HAL_ATOMIC_INIT(v) 
#define HAL_ATOMIC_READ(v) ((v)->val)
#define HAL_ATOMIC_SET(i,v) (((v)->val) = (i))
#define HAL_ATOMIC_INC(v)  hal_atomic_add_comm(1,v)
#define HAL_ATOMIC_DEC(v)  hal_atomic_sub_comm(1,v)
#define HAL_ATOMIC_ADD(i,v) hal_atomic_add_comm(i,v)
#define HAL_ATOMIC_SUB(i,v) hal_atomic_sub_comm(i,v)
#endif
