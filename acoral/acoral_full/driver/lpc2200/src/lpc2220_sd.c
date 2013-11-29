#include"acoral.h"
acoral_dev_ops_t sd_ops;
typedef struct{
acoral_u32 physic_blocksz;
}raw_disk_info;

acoral_32 sd_write_fat_sector(void *data,acoral_size size,acoral_size offset,acoral_time tm_out){
	acoral_u32 sdblock_num=size/512;
	acoral_u32 sdblock_index=offset/512;
	acoral_32 ret;
	acoral_u8 *write_data=(acoral_u8 *)data;
	for(;sdblock_num>0;sdblock_num--)
	{
		ret=SD_WriteBlock(sdblock_index, write_data);
		sdblock_index++;
		write_data=write_data+512;
	}
	return ret;
}

acoral_32 sd_read_fat_sector(void *data,acoral_size size,acoral_size offset,acoral_time tm_out){
	acoral_u32 sdblock_num=size/512;
	acoral_u32 sdblock_index=offset/512;
	acoral_32 ret;
	acoral_u8 *read_data=(acoral_u8 *)data;	
	for(;sdblock_num>0;sdblock_num--)
	{
		ret=SD_ReadBlock(sdblock_index, read_data);
		sdblock_index++;
		read_data=read_data+512;
	}
	return ret;
}

acoral_32 sd_config(acoral_u32 cmd,void *data,acoral_time time_out)
{
	switch(cmd){
		case GET_DISK_INFO:
			break;
		default:
			return -1;
	}
	return 0;
			
}
void sd_open(){
	SD_Initialize();
}

/*void uart_init(){	
    acoral_16 Fdiv;
    PINSEL0 = (PINSEL0 & (~0x0F)) | 0x05;	// 不影响其它管脚连接,设置I/O连接到UART0
    U0LCR = 0x83;							// DLAB = 1，可设置波特率
    Fdiv = (Fpclk / 16) / CFG_BAUD_RATE;				// 设置波特率
    U0DLM = Fdiv / 256;							
    U0DLL = Fdiv % 256;						
    U0LCR = 0x03;
}*/
void sd_init(){
	sd_ops.write=sd_write_fat_sector;
	sd_ops.read=sd_read_fat_sector;
	sd_ops.open=sd_open;
	sd_ops.config=sd_config;
	sd_ops.close=NULL;
  	acoral_drv_register(&sd_ops,"disk");
	//uart_init();
}

DRV_INIT(sd_init)
