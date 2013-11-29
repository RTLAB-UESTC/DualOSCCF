#ifndef HAL_TIMER_H
#define HAL_TIMER_H
#include"hal_int.h"
#define HAL_TICKS_INTR HAL_INTR_MIN
#define HAL_TICKS_INIT() hal_ticks_init()
#define HAL_TICKS_DELAY() 0
void hal_ticks_init(void);
#endif
