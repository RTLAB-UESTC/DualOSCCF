#include <type.h>
#include<spinlock.h>
#include<pal.h>
#include <print.h>
#define LEVEL 14 
#define BLOCK_INDEX(index) ((index)>>1)
#define BLOCK_SHIFT 7 
#define BLOCK_SIZE (1<<BLOCK_SHIFT)
#define MEM_NO_ALLOC 0
#define MEM_OK 1
typedef struct{
	acoral_8 level;
}acoral_block_t;
extern volatile acoral_u32 acoral_start_sched;
typedef struct{
	acoral_32 *free_list[LEVEL];
	acoral_u32 *bitmap[LEVEL];
	acoral_32 free_cur[LEVEL];
	acoral_u32 num[LEVEL];
	acoral_8 level;
	acoral_u8 state;
	acoral_u32 start_adr;
	acoral_u32 end_adr;
	acoral_u32 block_num;
	acoral_u32 free_num;
	acoral_u32 block_size;
	acoral_spinlock_t lock;
}acoral_block_ctr_t;

acoral_block_ctr_t *acoral_mem_ctrl;
acoral_block_t *acoral_mem_blocks;

void buddy_scan(){
	acoral_u32 i,k,num,n;	
	acoral_u32 max_level=acoral_mem_ctrl->level;
	acoral_print("Free Mem Block Number:%d\r\n",acoral_mem_ctrl->free_num); 	
	for(i=0;i<max_level;i++){
		acoral_print("Level%d\r\n",i); 	
		acoral_prints("bitmap:"); 	
		num=acoral_mem_ctrl->num[i];
		for(k=0;k<num;){
			for(n=0;n<8&&k<num;n++,k++){
				acoral_print("%x ",acoral_mem_ctrl->bitmap[i][k]); 
			}	
			acoral_prints("\r\n"); 	
		}
		acoral_print("Free Block head:%d\r\n",acoral_mem_ctrl->free_cur[i]); 	
		acoral_prints("\r\n"); 	
	}
	acoral_print("\r\n"); 	
}

acoral_err buddy_init(acoral_u32 start_adr,acoral_u32 end_adr){
	acoral_32 i,k;
	acoral_u32 resize_size;
	acoral_u32 save_adr;
	acoral_u32 index;
	acoral_u32 num=1;
	acoral_u32 adjust_level=1;
	acoral_32 level=0;
	acoral_u32 max_num,o_num;
	acoral_u32 cur;
	start_adr+=3;
	start_adr&=~(4-1);		
	end_adr&=~(4-1);		
	resize_size=BLOCK_SIZE;
	end_adr=end_adr - sizeof(acoral_block_ctr_t);
	end_adr&=~(4-1);
	acoral_mem_ctrl=(acoral_block_ctr_t *)end_adr;
	/*如果内存这么少，不适合分配*/
	if(start_adr>end_adr||end_adr-start_adr<BLOCK_SIZE){
		acoral_mem_ctrl->state=MEM_NO_ALLOC;
		return -1;
	}
	acoral_mem_ctrl->state=MEM_OK;
	while(1){
		if(end_adr<=start_adr+resize_size)
			break;
		resize_size=resize_size<<1;
		num=num<<1;
		adjust_level++;
	}
	acoral_mem_blocks=(acoral_block_t *)end_adr-num;
	save_adr=(acoral_u32)acoral_mem_blocks;
	level=adjust_level;
	if(adjust_level>LEVEL)
		level=LEVEL;
	num=num/32; 
	for(i=0;i<level-1;i++){
		num=num>>1;
		if(num==0)
			num=1;
		save_adr-=num*4;
		save_adr&=~(4-1);
		acoral_mem_ctrl->bitmap[i]=(acoral_u32 *)save_adr;
		acoral_mem_ctrl->num[i]=num;
		save_adr-=num*4;
		save_adr&=~(4-1);
		acoral_mem_ctrl->free_list[i]=(acoral_32 *)save_adr;
		for(k=0;k<num;k++){
			acoral_mem_ctrl->bitmap[i][k]=0;;
			acoral_mem_ctrl->free_list[i][k]=-1;
		}
		acoral_mem_ctrl->free_cur[i]=-1;
	}
	if(num==0)
		num=1;
	save_adr-=num*4;
	save_adr&=~(4-1);
	acoral_mem_ctrl->bitmap[i]=(acoral_u32 *)save_adr;
	acoral_mem_ctrl->num[i]=num;
	save_adr-=num*4;
	save_adr&=~(4-1);
	acoral_mem_ctrl->free_list[i]=(acoral_32 *)save_adr;
	for(k=0;k<num;k++){
		acoral_mem_ctrl->bitmap[i][k]=0;;
		acoral_mem_ctrl->free_list[i][k]=-1;
	}
	acoral_mem_ctrl->free_cur[i]=-1;

	/*如果减去刚才bitmap用的内存刚好是下一level*/
	if(save_adr<=(start_adr+(resize_size>>1)))
		adjust_level--;
	if(adjust_level>LEVEL)
		level=LEVEL;
	acoral_mem_ctrl->level=level;
	acoral_mem_ctrl->start_adr=start_adr;
	num=(save_adr-start_adr)>>BLOCK_SHIFT;
	acoral_mem_ctrl->end_adr=start_adr+(num<<BLOCK_SHIFT);
	acoral_mem_ctrl->block_num=num;
	acoral_mem_ctrl->free_num=num;
	acoral_mem_ctrl->block_size=BLOCK_SIZE;

	i=0;
	max_num=1<<level-1;
	o_num=0;
	if(num>0)
		acoral_mem_ctrl->free_cur[level-1]=0;
	else
		acoral_mem_ctrl->free_cur[level-1]=-1;

	while(num>=max_num*32){
		acoral_mem_ctrl->bitmap[level-1][i]=-1;;
		acoral_mem_ctrl->free_list[level-1][i]=i+1;
		num-=max_num*32;
		o_num+=max_num*32;
		i++;
	}
	if(num==0)
		acoral_mem_ctrl->free_list[level-1][i-1]=-1;
	while(num>=max_num){
		index=o_num>>level-1;
		acoral_set_bit(index,acoral_mem_ctrl->bitmap[level-1]);
		num-=max_num;
		o_num+=max_num;
	}
	acoral_mem_ctrl->free_list[level-1][i]=-1;

	
	while(--level>0){
		index=o_num>>level;
		if(num==0)
			break;
		cur=index/32;
		max_num=1<<level-1;
		if(num>=max_num){
			acoral_mem_blocks[BLOCK_INDEX(o_num)].level=-1;	
			acoral_set_bit(index,acoral_mem_ctrl->bitmap[level-1]);
			acoral_mem_ctrl->free_list[level-1][cur]=-1;	
			acoral_mem_ctrl->free_cur[level-1]=cur;	
			o_num+=max_num;
			num-=max_num;
		}
	}
	acoral_spin_init(&acoral_mem_ctrl->lock);
	return 0;
}

static acoral_32 recus_malloc(level){
	acoral_u32 index;
	acoral_32 cur;
	acoral_32 num;
	if(level>=acoral_mem_ctrl->level)
		return -1;
	cur=acoral_mem_ctrl->free_cur[level];
	if(cur<0){
		num=recus_malloc(level+1);
		if(num<0)
			return -1;
		index=num>>level+1;
		cur=index/32;
		acoral_set_bit(index,acoral_mem_ctrl->bitmap[level]);
		acoral_mem_ctrl->free_list[level][cur]=-1;
		acoral_mem_ctrl->free_cur[level]=cur;
		return num;
	}
	index=acoral_ffs(acoral_mem_ctrl->bitmap[level][cur]);
	index=cur*32+index;
	acoral_clear_bit(index,acoral_mem_ctrl->bitmap[level]);
	if(acoral_mem_ctrl->bitmap[level][cur]==0)
		acoral_mem_ctrl->free_cur[level]=acoral_mem_ctrl->free_list[level][cur];
	num=index<<level+1;
	/*最高level情况*/
	if(level==acoral_mem_ctrl->level-1){
		if((num>>1)+(1<<level)>acoral_mem_ctrl->block_num)
			return -1;
		return num>>1;
	}
	if(acoral_mem_blocks[BLOCK_INDEX(num)].level>=0)
		return num+(1<<level);
	else
		return num;
}

static void *r_malloc(acoral_u8 level){
	acoral_sr cpu_sr;
	acoral_u32 index;
	acoral_32 num,cur;
	HAL_ENTER_CRITICAL();
	acoral_spin_lock(&acoral_mem_ctrl->lock);
	acoral_mem_ctrl->free_num-=1<<level;
	cur=acoral_mem_ctrl->free_cur[level];
	if(cur<0){
		num=recus_malloc(level+1);
		if(num<0){
			acoral_spin_unlock(&acoral_mem_ctrl->lock);
			HAL_EXIT_CRITICAL();
			return NULL;
		}
		index=num>>level+1;
		cur=index/32;
		acoral_set_bit(index,acoral_mem_ctrl->bitmap[level]);
		acoral_mem_ctrl->free_list[level][cur]=-1;
		acoral_mem_ctrl->free_cur[level]=cur;
		if((num&0x1)==0)
			acoral_mem_blocks[BLOCK_INDEX(num)].level=level;
#ifdef CFG_TEST
		acoral_print("Malloc-level:%d,num:%d\n",level,num);
#endif
#ifdef CFG_TEST_MEM
		buddy_scan();
#endif
		acoral_spin_unlock(&acoral_mem_ctrl->lock);
		HAL_EXIT_CRITICAL();
		return (void *)(acoral_mem_ctrl->start_adr+(num<<BLOCK_SHIFT));
	}
	index=acoral_ffs(acoral_mem_ctrl->bitmap[level][cur]);
	index=index+cur*32;
	acoral_clear_bit(index,acoral_mem_ctrl->bitmap[level]);
	if(acoral_mem_ctrl->bitmap[level][cur]==0){
		acoral_mem_ctrl->free_cur[level]=acoral_mem_ctrl->free_list[level][cur];
	}
	if(level==acoral_mem_ctrl->level-1){
		num=index<<level;
		if(num+(1<<level)>acoral_mem_ctrl->block_num){
			acoral_spin_unlock(&acoral_mem_ctrl->lock);
			HAL_EXIT_CRITICAL();
			return NULL; 
		}
	}
	else{
		num=index<<level+1;
		if(acoral_mem_blocks[BLOCK_INDEX(num)].level>=0)
			num+=(1<<level);
	}
	if((num&0x1)==0)
		acoral_mem_blocks[BLOCK_INDEX(num)].level=level;
#ifdef CFG_TEST
	acoral_print("malloc-level:%d,num:%d\n",level,num);
#endif
#ifdef CFG_TEST_MEM
	buddy_scan();
#endif
	acoral_spin_unlock(&acoral_mem_ctrl->lock);
	HAL_EXIT_CRITICAL();
	return (void *)(acoral_mem_ctrl->start_adr+(num<<BLOCK_SHIFT));
}

acoral_u32 buddy_malloc_size(acoral_u32 size){
	acoral_u32 resize_size;	
	acoral_u8 level=0;	
	acoral_u32 num=1;	
	resize_size=BLOCK_SIZE;
	if(acoral_mem_ctrl->state==MEM_NO_ALLOC)
		return 0;
	while(resize_size<size){
		num=num<<1;
		level++;
		resize_size=resize_size<<1;
	}
	return resize_size;
}

void *buddy_malloc(acoral_u32 size){
	acoral_u32 resize_size;	
	acoral_u8 level=0;	
	acoral_u32 num=1;	
	resize_size=BLOCK_SIZE;
	if(acoral_mem_ctrl->state==MEM_NO_ALLOC)
		return NULL;
	while(resize_size<size){
		num=num<<1;
		level++;
		resize_size=resize_size<<1;
	}
	if(num>acoral_mem_ctrl->free_num)
		return NULL;
	if(level>=acoral_mem_ctrl->level)
		return NULL;	
	return r_malloc(level);
}

void buddy_free(void *ptr){
	acoral_sr cpu_sr;
	acoral_8 level;
	acoral_8 buddy_level;
	acoral_32 cur;
	acoral_u32 index;
	acoral_u32 num;
	acoral_u32 max_level;
	acoral_u32 adr;
	adr=(acoral_u32)ptr;
	if(acoral_mem_ctrl->state==MEM_NO_ALLOC)
		return;
	if(ptr==NULL||adr<acoral_mem_ctrl->start_adr||adr+BLOCK_SIZE>acoral_mem_ctrl->end_adr){
		acoral_printerr("Invalid Free Address:0x%x\n",ptr);
		return;
	}
	max_level=acoral_mem_ctrl->level;
	num=(adr-acoral_mem_ctrl->start_adr)>>BLOCK_SHIFT;
	/*如果不是block整数倍，肯定是非法地址*/
	if(adr!=acoral_mem_ctrl->start_adr+(num<<BLOCK_SHIFT)){
		acoral_printerr("Invalid Free Address:0x%x\n",ptr);
		return;
	}
	HAL_ENTER_CRITICAL();
	acoral_spin_lock(&acoral_mem_ctrl->lock);
	if(num&0x1){
		level=0;
		/*下面是地址检查*/
		index=num>>1;
		buddy_level=acoral_mem_blocks[BLOCK_INDEX(num)].level;
		if(buddy_level>0){
			acoral_printerr("Invalid Free Address:0x%x\n",ptr);
			acoral_spin_unlock(&acoral_mem_ctrl->lock);
			HAL_EXIT_CRITICAL();
			return;
		}
		/*伙伴分配出去，如果对应的位为1,肯定是回收过一次了*/
		if(buddy_level==0&&acoral_get_bit(index,acoral_mem_ctrl->bitmap[level])){
			acoral_printerr("Address:0x%x have been freed\n",ptr);
			acoral_spin_unlock(&acoral_mem_ctrl->lock);
			HAL_EXIT_CRITICAL();
			return;
		}
		/*伙伴没有分配出去了，如果对应的位为0,肯定是回收过一次了*/
		if(buddy_level<0&&!acoral_get_bit(index,acoral_mem_ctrl->bitmap[level])){
			acoral_printerr("Address:0x%x have been freed\n",ptr);
			acoral_spin_unlock(&acoral_mem_ctrl->lock);
			HAL_EXIT_CRITICAL();
			return;
		}
	}else{
		level=acoral_mem_blocks[BLOCK_INDEX(num)].level;
		/*已经释放*/
		if(level<0){
			acoral_printerr("Address:0x%x have been freed\n",ptr);
			acoral_spin_unlock(&acoral_mem_ctrl->lock);
			HAL_EXIT_CRITICAL();
			return;
		}	
		acoral_mem_ctrl->free_num+=1<<level;
		acoral_mem_blocks[BLOCK_INDEX(num)].level=-1;
	}
#ifdef CFG_TEST
	acoral_print("Free-level:%d,num:%d\n",level,num);
#endif
	if(level==max_level-1){
		index=num>>level;
		acoral_set_bit(index,acoral_mem_ctrl->bitmap[level]);
		HAL_EXIT_CRITICAL();
		acoral_spin_unlock(&acoral_mem_ctrl->lock);
		return;
	}
	index=num>>1+level;
	while(level<max_level){
		cur=index/32;
		if(!acoral_get_bit(index,acoral_mem_ctrl->bitmap[level])){
			acoral_set_bit(index,acoral_mem_ctrl->bitmap[level]);
			if(acoral_mem_ctrl->free_cur[level]<0||cur<acoral_mem_ctrl->free_cur[level]){
				acoral_mem_ctrl->free_list[level][cur]=acoral_mem_ctrl->free_cur[level];	
				acoral_mem_ctrl->free_cur[level]=cur;	
			}
			break;
		}
		/*有个伙伴是空闲的，向上级回收*/
		acoral_clear_bit(index,acoral_mem_ctrl->bitmap[level]);
		if(cur==acoral_mem_ctrl->free_cur[level])
			acoral_mem_ctrl->free_cur[level]=acoral_mem_ctrl->free_list[level][cur];	
		level++;
		if(level<max_level-1)
			index=index>>1;
	}
	HAL_EXIT_CRITICAL();
	acoral_spin_unlock(&acoral_mem_ctrl->lock);
#ifdef CFG_TEST_MEM
	buddy_scan();
#endif
}
