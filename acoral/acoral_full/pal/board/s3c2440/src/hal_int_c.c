#include<type.h>
#include<hal_int.h>
#include<hal_2440_c.h>

void (*intr_c_entry)(acoral_vector);

void hal_all_entry(acoral_vector vector){
    unsigned long eint;
    unsigned long irq=4;
    if(vector==4||vector==5){
    	eint=rEINTPND;
    	for(;irq<24;irq++){
      		 if(eint & (1<<irq)){
           		intr_c_entry(irq);
           		return;
           	 }
    	}
    }
    if(vector>5)
    	vector+=18;
    if(vector==4)
    	acoral_prints("DErr\n");
    intr_c_entry(vector);
}

static void hal_intr_unmask(acoral_vector vector){
          if((vector>3) && (vector<8)){
               rEINTMSK &=~(1<<vector);
               vector = 4;
           }
         else if((vector>7) && (vector<24)){
               rEINTMSK &=~(1<<vector);
               vector = 5;
           }
         else if(vector > 23)
               vector -= 18; 

         rINTMSK &=~(1<<vector);			/*开启中断*/
}

static void hal_intr_mask(acoral_vector vector){

         if((vector>3) && (vector<8)){
               rEINTMSK |=(1<<vector);
               vector = 4;
           }
         else if((vector>7) && (vector<24)){
               rEINTMSK |=(1<<vector);
               vector = 5;
           }
         else if(vector > 23)
               vector -= 18; 

         rINTMSK |= (1<<vector);
}

static void hal_intr_ack(acoral_u32 vector){

        if((vector>3) && (vector<8)){
               rEINTPND &= ~(1<<vector);
               vector = 4;
           }
        else if((vector>7) && (vector<24)){
               rEINTPND &= ~(1<<vector);
               vector = 5;
           }
        else if(vector > 23)
               vector -= 18;
   
        
        rSRCPND = 1<<vector;
	rINTPND = 1<<vector;		
        
	
}

void hal_intr_set_entry(void (*isr)(acoral_vector)){
	intr_c_entry=isr;
}

void hal_intr_init(){
	acoral_u32 i;
  	rPRIORITY = 0x00000000;		/* 使用默认的固定的优先级*/
	rINTMOD = 0x00000000;		/* 所有中断均为IRQ中断*/
        rEINTMSK = 0xffffffff;              /*屏蔽所有外部中断*/
        rINTMSK = 0xffffffff;               /*屏蔽所有中断*/

	for(i=HAL_INTR_MIN;i<=HAL_INTR_MAX;i++){
		acoral_set_intr_enter(i,hal_intr_ack);
		acoral_set_intr_exit(i,NULL);
		acoral_set_intr_mask(i,hal_intr_mask);
		acoral_set_intr_unmask(i,hal_intr_unmask);
	}
}


