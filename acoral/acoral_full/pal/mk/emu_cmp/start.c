#define _GNU_SOURCE
#include<type.h>
#include<hal_extent.h>
#include<hal_ipi.h>
#include<hal_timer.h>
#include<hal_comm.h>
#include<timer.h>
#include<int.h>
extern void acoral_start();
extern volatile acoral_u32 acoral_start_sched;
int main(){
  pthread_t id;
  pthread_attr_t attr;
  cpu_set_t cpuset;
  pthread_attr_init( &attr );
  pthread_create(&id,&attr,(void *)acoral_start,NULL);
  CPU_ZERO(&cpuset );
  CPU_SET(0, &cpuset);
  pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset);
  acoral_intr_disable();
  while(!acoral_start_sched);
  while(1){
    	usleep(1000000/ACORAL_TICKS_PER_SEC);

#ifdef CFG_CMP
	pthread_kill(cpus_tid[0],HAL_TICKS_INTR);
#else
	pthread_kill(id,HAL_TICKS_INTR);
#endif
  }
  pthread_join(id,NULL);
  return 0;
}
