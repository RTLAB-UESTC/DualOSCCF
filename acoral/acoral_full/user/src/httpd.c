#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/sockets.h"
#include <os_file.h>
/*
;*****************************************************************************************************
;* 函数名称 : httpd_thread
;* 描    述 : HTTPD任务
;* 输　	 入 : pdata
;*        
;* 输　	 出 : 无
;*****************************************************************************************************
;*/
acoral_u8 HTTP_GET_STRING[]   =  "GET";
#define HTTP_GET_STRING_LEN   (sizeof(HTTP_GET_STRING)-1)
static acoral_u8 httpd_buf[128];	
void httpd_thread(void *arg)
{
	int s, ei, temp;
    	acoral_32 len;
    	acoral_u8 cmd[4];
	struct sockaddr_in serv, cli;
	acoral_u32 fd;		
	memset(&serv, 0, sizeof(struct sockaddr_in));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(80);	
	serv.sin_addr.s_addr = htons(INADDR_ANY);
	s = socket(AF_INET, SOCK_STREAM, 0);
	ei = bind(s, (struct sockaddr*)&serv, sizeof(struct sockaddr_in));
	ei = listen(s, 5);
	while (1)
	{
		temp = accept(s, (struct sockaddr*)&cli, &len);
		if (temp != -1)
		{
			recv(temp, cmd, HTTP_GET_STRING_LEN, 0);
			cmd[3] = 0;
			fd=acoral_open("www/index.htm",O_RDONLY); 	
			while((len=acoral_read(fd, httpd_buf,128))>0)
				send(temp,httpd_buf, len, 0);
			acoral_close(fd);
		}
		close(temp);
	}
 }

void user_httpd()
{
	acoral_posix_create(NULL,NULL,httpd_thread,0);
}
