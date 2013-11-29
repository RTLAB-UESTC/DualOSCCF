#ifndef HAL_INTR_H
#define HAL_INTR_H
#include<type.h>
#define HAL_INTR_MIN            15
#define EXP_SYSTICK             HAL_INTR_MIN+0
#define IRQ_WWDG        	HAL_INTR_MIN+1
#define IRQ_PVD 		HAL_INTR_MIN+2
#define IRQ_TAMPER         	HAL_INTR_MIN+3
#define IRQ_RTC  		HAL_INTR_MIN+4
#define IRQ_FLASH		HAL_INTR_MIN+5
#define IRQ_RCC 	 	HAL_INTR_MIN+6
#define IRQ_EXTI0		HAL_INTR_MIN+7			/* Added for 2440.*/
#define IRQ_EXTI1  		HAL_INTR_MIN+8
#define IRQ_EXTI2		HAL_INTR_MIN+9
#define IRQ_EXTI3		HAL_INTR_MIN+10  /*Changed to IRQ_WDT_AC97 for 2440A*/
#define IRQ_EXTI4	 	HAL_INTR_MIN+11
#define IRQ_DMA1_Channel1	HAL_INTR_MIN+12
#define IRQ_DMA1_Channel2	HAL_INTR_MIN+13
#define IRQ_DMA1_Channel3	HAL_INTR_MIN+14
#define IRQ_DMA1_Channel4	HAL_INTR_MIN+15
#define IRQ_DMA1_Channel5	HAL_INTR_MIN+16
#define IRQ_DMA1_Channel6	HAL_INTR_MIN+17
#define IRQ_DMA1_Channel7	HAL_INTR_MIN+18
#define IRQ_ADC1_2		HAL_INTR_MIN+19
#define IRQ_USB_HP_CAN1_TX	HAL_INTR_MIN+20
#define IRQ_USB_LP_CAN1_RX0	HAL_INTR_MIN+21
#define IRQ_CAN1_RX1	        HAL_INTR_MIN+22
#define IRQ_CAN1_SCE		HAL_INTR_MIN+23
#define IRQ_EXTI9_5		HAL_INTR_MIN+24
#define IRQ_TIM1_BRK		HAL_INTR_MIN+25		/* Added for 2440.*/
#define IRQ_TIM1_UP		HAL_INTR_MIN+26
#define IRQ_TIM1_TRG_COM	HAL_INTR_MIN+27
#define IRQ_TIM1_CC		HAL_INTR_MIN+28
#define IRQ_TIM2		HAL_INTR_MIN+29
#define IRQ_TIM3		HAL_INTR_MIN+30
#define IRQ_TIM4		HAL_INTR_MIN+31
#define IRQ_I2C1_EV        	HAL_INTR_MIN+32
#define IRQ_I2C1_ER             HAL_INTR_MIN+33
#define IRQ_I2C2_EV             HAL_INTR_MIN+34
#define IRQ_I2C2_ER             HAL_INTR_MIN+35
#define IRQ_SPI1                HAL_INTR_MIN+36
#define IRQ_SPI2                HAL_INTR_MIN+37
#define IRQ_USART1              HAL_INTR_MIN+38
#define IRQ_USART2              HAL_INTR_MIN+39
#define IRQ_USART3              HAL_INTR_MIN+40
#define IRQ_EXTI15_10           HAL_INTR_MIN+41
#define IRQ_RTCALARM            HAL_INTR_MIN+42
#define IRQ_USBWAKEUP           HAL_INTR_MIN+43


#define HAL_INTR_NUM   44
#define HAL_INTR_MAX    HAL_INTR_MIN+HAL_INTR_NUM-1

#ifndef HAL_TRANSLATE_VECTOR
#define HAL_TRANSLATE_VECTOR(_vector_,_index_) \
	(_index_)=(_vector_)-15;
#endif

#define HAL_INTR_INIT() hal_intr_init()
#define HAL_INTR_SET_ENTRY(isr) hal_intr_set_entry(isr)
struct int_reg     /*at 0xe000e100*/
{
     acoral_u32 setena[32];/*使能寄存器组*/
     acoral_u32 clrena[32];/*除能寄存器组*/
     acoral_u32 setpend[32];/*挂起寄存器组*/
     acoral_u32 clrpend[32];/*解挂寄存器组*/
     acoral_u32 active[32]; /*活动状态寄存器组   */
     acoral_u32 RESERVED5[32];/*保留*/
     acoral_u8  pri[240];/*优先级寄存器组*/
};
#endif

