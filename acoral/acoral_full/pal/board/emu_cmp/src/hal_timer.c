#include"acoral.h"
#include<unistd.h>
#include<signal.h>
void hal_ticks_handle_set(void (*handler)(acoral_vector)){
  	acoral_intr_attach(HAL_TICKS_INTR,handler);
}
/*****************************************/
/*****这个函数的作用是初始化滴答时钟数据**/
/**********相关数据*********************/
/*****************************************/
void hal_ticks_init(){
}
