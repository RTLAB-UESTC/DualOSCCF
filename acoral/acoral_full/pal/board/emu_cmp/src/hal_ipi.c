#include<type.h>
#include<hal_cmp.h>
hal_ipi_send(acoral_u32 cpu,acoral_vector vector){
  	pthread_kill(cpus_tid[cpu],vector);
}
