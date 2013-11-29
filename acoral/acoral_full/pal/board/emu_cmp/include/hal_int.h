#ifndef HAL_INTR_H
#define HAL_INTR_H
#include"type.h"
#include"hal_comm.h"
#define SIGUSER1 10
#define HAL_INTR_MIN SIGUSER1
#define HAL_INTR_NUM 3
#define HAL_INTR_MAX HAL_INTR_MIN+HAL_INTR_NUM-1

#define HAL_INTR_INIT() hal_intr_init()
#ifndef HAL_TRANSLATE_VECTOR
#define HAL_TRANSLATE_VECTOR(_vector_,_index_) \
	(_index_)=((_vector_)-HAL_INTR_MIN)
#endif
#define HAL_INTR_SET_ENTRY(isr) hal_intr_set_entry(isr)
#define HAL_INTR_DISABLE()   hal_intr_disable()
#define HAL_INTR_ENABLE()   hal_intr_enable()
#define HAL_INTR_DISABLE_SAVE()  hal_intr_disable_save()
#define HAL_INTR_RESTORE(sr)   hal_intr_restore(sr)
acoral_sr hal_intr_disable_save();
void hal_intr_restore(acoral_sr sr);
extern acoral_sr hal_intr_state;
#endif

