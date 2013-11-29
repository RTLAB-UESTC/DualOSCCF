#ifndef ACORAL_HAL_CORE_H
#define ACORAL_HAL_CORE_H
#include<type.h>
#define HAL_INTR_SWITCH_TO(next) hal_switch_to(next)
#define HAL_SWITCH_TO(next) hal_switch_to(next)
#define HAL_INTR_CTX_SWITCH(prev,next) hal_intr_ctx_switch(prev,next)

#define HAL_STACK_INIT(stack,route,exit,args) hal_stack_init(stack,route,exit,args)
void hal_switch_to(acoral_u32 **stack);
void HAL_CONTEXT_SWITCH(acoral_u32 **prev,acoral_u32 **next);
void hal_context_switch(acoral_u32 **prev,acoral_u32 **next);
typedef struct {
  	acoral_u32 eax;
	acoral_u32 ebx;
	acoral_u32 ecx;
	acoral_u32 edx;
	acoral_u32 esi;
	acoral_u32 edi;
	acoral_u32 eflags;
	acoral_u32 ebp;
	acoral_u32 intr_state;
	acoral_u32 eip;
}hal_ctx_t;
#endif
