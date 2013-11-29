#include "acoral.h"
void hal_board_init(void){
 	hal_cpu_init();
}
void hal_cpu_init(){
	//MMU_EnableDCache();
	//MMU_EnableICache();
	hal_scu_enable();
}
