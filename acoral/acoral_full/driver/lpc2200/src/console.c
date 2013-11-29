#include"acoral.h"
#define put_char(ch) uart_write(ch)
acoral_dev_ops_t console_ops;
void uart_write(acoral_u8 ch){
    U0THR = ch;						// 发送数据
    while( (U0LSR&0x40)==0 );			// 等待数据发送完毕
}

acoral_32 console_write(void *data,acoral_size size,acoral_size offet,acoral_time tm_out){
	acoral_u32 i;
  	acoral_char p;
	for(i=0;i<size;i++){
		p=*((acoral_char *)data+i);
		if(p=='\n')
			put_char('\r');
		put_char(p);
	}
}

void uart_init(){	
    acoral_16 Fdiv;
    PINSEL0 = (PINSEL0 & (~0x0F)) | 0x05;	// 不影响其它管脚连接,设置I/O连接到UART0
    U0LCR = 0x83;							// DLAB = 1，可设置波特率
    Fdiv = (Fpclk / 16) / CFG_BAUD_RATE;				// 设置波特率
    U0DLM = Fdiv / 256;							
    U0DLL = Fdiv % 256;						
    U0LCR = 0x03;
}
void console_init(){
	console_ops.write=console_write;
	console_ops.open=NULL;
  	acoral_drv_register(&console_ops,"console");
	uart_init();
}
