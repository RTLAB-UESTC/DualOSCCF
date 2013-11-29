#include"acoral.h"
#include"sig.h"
extern void (*HAL_INTR_ENTRY)(acoral_vector vector);
void (*intr_c_entry)(acoral_vector);
acoral_sr hal_intr_state;
acoral_sr intr_state[HAL_INTR_MAX-HAL_INTR_MIN+1];
sigset_t intmask;
acoral_u32 IRQ_stack[HAL_MAX_CPU];

void intr_special(acoral_u32 sp){
	acoral_u32 cpu;
	cpu=acoral_current_cpu;
	IRQ_stack[cpu]=sp;
}

void hal_intr_mask(acoral_vector vector){
        intr_state[vector-HAL_INTR_MIN]=false;
}

void hal_intr_unmask(acoral_vector vector){
        intr_state[vector-HAL_INTR_MIN]=true;
}

void hal_intr_set_entry(void (*isr)(acoral_vector)){
  /*int i;
	for(i=HAL_INTR_MIN;i<=HAL_INTR_MAX;i++)
	if(signal(i,&HAL_INTR_ENTRY)==SIG_ERR)
  		acoral_print("hal_entry_err");*/

  	if(signal(HAL_TICKS_INTR,&HAL_INTR_ENTRY)==SIG_ERR)
      		acoral_print("hal_entry_err");
#ifdef CFG_CMP
  	if(signal(HAL_CMD_IPI,&HAL_INTR_ENTRY)==SIG_ERR)
      		acoral_print("hal_entry_err");
#endif
      	intr_c_entry=isr;
}

void hal_intr_enable(){
    hal_intr_state=true;
    sigprocmask(SIG_UNBLOCK,&intmask,NULL);
}

void hal_intr_disable(){
     sigprocmask(SIG_BLOCK,&intmask,NULL);
     hal_intr_state=false;
}

acoral_sr hal_intr_disable_save(){
  	acoral_sr tmp;
	tmp=hal_intr_state;
	acoral_intr_disable();
	return tmp;
}

void hal_intr_restore(acoral_sr sr){
     if(sr)
       	acoral_intr_enable();
     else
       	acoral_intr_disable();
}

void hal_intr_init(){
	acoral_u32 i;
	sigemptyset(&intmask);
	sigaddset(&intmask,HAL_TICKS_INTR);
#ifdef CFG_CMP
	sigaddset(&intmask,HAL_CMD_IPI);
#endif
	for(i=HAL_INTR_MIN;i<=HAL_INTR_MAX;i++){
		acoral_set_intr_enter(i,NULL);
		acoral_set_intr_exit(i,NULL);
		acoral_set_intr_mask(i,hal_intr_mask);
		acoral_set_intr_unmask(i,hal_intr_unmask);
	}
}
