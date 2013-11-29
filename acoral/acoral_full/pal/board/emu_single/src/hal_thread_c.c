#include"acoral.h"
#include<sig.h>
acoral_sr hal_intr_state;
extern acoral_u32 *IRQ_stack;
void hal_sig_ret(){
  	__asm__ __volatile__("popl %%eax\n\t"\
	"mov $0x77,%%eax\n\t"               \
	"int $0x80\n\t"               \
	:          \
	);

}

void hal_stack_init(acoral_u32 **stk,void (*route)(),void (*exit)(),void *args){
  	hal_ctx_t *stack;
	acoral_u32 *tmp=*stk;
	*tmp--=args;
	*tmp=exit;
	stack=(hal_ctx_t *)tmp;
	stack--;
	stack->eip=(acoral_u32 *)route;
	stack->intr_state=true;
	*stk=stack;
}

void hal_intr_ctx_switch(acoral_u32 **prev,acoral_u32 **next){
  acoral_u32 i;
  hal_ctx_t *tmp=IRQ_stack;
  acoral_u32 *ctx=IRQ_stack;
  *(--ctx)=hal_intr_state;
  tmp--;
  ctx=tmp;
  ctx++;
  *prev=ctx;
   hal_switch_to(next);
}

void hal_switch_to(acoral_u32 **next){
  acoral_u32 i;
  struct sigframe *frame;
  hal_ctx_t *tmp=*next;
  acoral_u32 *ctx;
  frame=*next;
  if(tmp->intr_state){
      frame--;
      frame->sc.eip=tmp->eip;
      frame->sc.esp=tmp+1;
      frame->sc.esp_at_signal=tmp+1;
      frame->sc.eax=tmp->eax;
      frame->sc.ebx=tmp->ebx;
      frame->sc.ecx=tmp->ecx;
      frame->sc.edx=tmp->edx;
      frame->sc.esi=tmp->esi;
      frame->sc.edi=tmp->edi;
      frame->sc.ebp=tmp->ebp;
      frame->sc.cs=0x73;
      frame->sc.ds=0x7b;
      frame->sc.ss=0x7b;;
      frame->sc.fs=0x0;
      frame->sc.gs=0x33;
      frame->sc.eflags=0x246;
      frame->sc.cr2=0x0;
      frame->sc.oldmask=tmp->intr_state<<HAL_TICKS_INTR;/*(1<<HAL_CMP_INTR);*/
      frame->pretcode=(char *)hal_sig_ret;
      ctx=frame;
      *(--ctx)=tmp->intr_state;
      tmp=frame;
      tmp--;
      ctx=tmp;
      ctx++;
      *next=ctx;
   }
   HAL_REAL_SWITCH_TO(next);
}
