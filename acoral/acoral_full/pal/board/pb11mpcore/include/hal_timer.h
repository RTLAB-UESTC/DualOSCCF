#ifndef HAL_TIMER_H
#define HAL_TIMER_H
#include"hal_int.h"
//#define HAL_TICKS_INTR IRQ_TC11MP_TIMER0_1
#define HAL_TICKS_INTR IRQ_TC11MP_TIMER2_3
//#define HAL_TICKS_INTR IRQ_PB11MP_TIMER4_5
//#define HAL_TICKS_INTR IRQ_PB11MP_RTC_GIC1
//#define HAL_TICKS_INTR IRQ_PRIVATE_TIMER


#define HAL_TICKS_INIT() hal_ticks_init()
#define HAL_TICKS_DELAY() hal_ticks_delay()
void hal_ticks_init();
#endif
