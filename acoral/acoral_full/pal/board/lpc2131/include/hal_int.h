#ifndef HAL_INTR_H
#define HAL_INTR_H
#define HAL_INTR_MIN            0
#define IRQ_BOD 		HAL_INTR_MIN+0
#define IRQ_EINT0		HAL_INTR_MIN+1
#define IRQ_EINT1		HAL_INTR_MIN+2
#define IRQ_EINT2		HAL_INTR_MIN+3
#define IRQ_EINT3		HAL_INTR_MIN+4
#define IRQ_TIMER0	 	HAL_INTR_MIN+5
#define IRQ_TIMER1	 	HAL_INTR_MIN+6
#define IRQ_WDT 		HAL_INTR_MIN+7
#define IRQ_DBGCOMMRX		HAL_INTR_MIN+8
#define IRQ_DBGCOMMTX 		HAL_INTR_MIN+9
#define IRQ_UART0		HAL_INTR_MIN+10
#define IRQ_UART1		HAL_INTR_MIN+11
#define IRQ_PWM0		HAL_INTR_MIN+12
#define IRQ_IIC0 		HAL_INTR_MIN+13
#define IRQ_IIC1 		HAL_INTR_MIN+14
#define IRQ_SPI0 		HAL_INTR_MIN+15
#define IRQ_SPI1 		HAL_INTR_MIN+16
#define IRQ_PLL 		HAL_INTR_MIN+17
#define IRQ_RTC 		HAL_INTR_MIN+18
#define IRQ_AD 			HAL_INTR_MIN+19
#define IRQ_SOFTINT 		HAL_INTR_MIN+20

#define HAL_INTR_NUM 21
#define HAL_INTR_MAX HAL_INTR_MIN+HAL_INTR_NUM-1

#ifndef HAL_TRANSLATE_VECTOR
#define HAL_TRANSLATE_VECTOR(_vector_,_index_) \
	(_index_)=(_vector_);
#endif

#define HAL_INTR_INIT()	hal_intr_init()
#define HAL_INTR_SET_ENTRY(isr) hal_intr_set_entry(isr)

#endif

