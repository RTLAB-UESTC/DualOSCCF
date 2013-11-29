#include "../include/common.h"
#include "../include/blockheap.h"
#include "../include/rect.h"
#include "../include/invalidregion.h"
#include "../include/clipregion.h"
#include "../include/message.h"
#include "../include/hdc.h"
#include "../include/caret.h"
#include "../include/winnc.h"
#include "../include/winbase.h"
#include "../include/window.h"
#include "../include/lguiapp.h"

static screeninfo _lGUI_vInfo;
extern int _lGUI_iFrameWidth;
extern int _lGUI_iFrameHeight;
static int _lGUI_iFrameBuffer;
WndCaptureMouse tmp;
PWndCaptureMouse _lGUI_wndCaptureMouse=&tmp;
extern unsigned char* _lGUI_pFrameBuffer;
extern int _lGUI_iLineSize;
extern int _lGUI_iBufferSize;
extern int _lGUI_iBytesPerPixel;
extern int _lGUI_iBytesDataType;
extern char* _lGUI_pShm;	
acoral_u32 SCREEN_HEIGHT,SCREEN_WIDTH;
void PortSetCurStat(PlGUIAppStat stat){
	acoral_cur_thread->data=malloc(sizeof(win_data));
	((win_data *)acoral_cur_thread->data)->curStat=stat;
	((win_data *)acoral_cur_thread->data)->curWin=NULL;
}

PlGUIAppStat PortGetCurStat(thread_t id){
	acoral_thread_t *thread;
	thread=(acoral_thread_t *)acoral_get_res_by_id(id);
	return ((win_data *)thread->data)->curStat;
}

void PortExit(){
	free(acoral_cur_thread->data);
}

BOOL port_shm_server_init(){
	_lGUI_pShm=bmalloc(SHMEM_SIZE);
	return true;
}

BOOL port_shm_server_exit(){
	bfree(_lGUI_pShm);
	return true;
}

BOOL port_shm_client_exit(){
	return true;
}

BOOL port_shm_client_init(){
	return true;
}

void PortServerExit(){
	
}

void PortClientExit(){
	PortExit();
}

void 
PortUnInitFrameBuffer(){
#ifdef CFG_GUI_EMU
	close(_lGUI_iFrameBuffer);
#endif
}

BOOL 
PortInitFrameBuffer()
{
#ifdef CFG_GUI_EMU
	_lGUI_iFrameBuffer = open ("/dev/fb0", O_RDWR);
	if(!_lGUI_iFrameBuffer){
		printerror("open framebuffer return error.");
		return FALSE;
	}
	ioctl (_lGUI_iFrameBuffer, FBIOGET_VSCREENINFO, &_lGUI_vInfo);

	_lGUI_iFrameHeight=_lGUI_vInfo.yres;
	_lGUI_iFrameWidth=_lGUI_vInfo.xres;

	_lGUI_iLineSize = _lGUI_vInfo.xres * _lGUI_vInfo.bits_per_pixel / 8;
	_lGUI_iBufferSize = _lGUI_iLineSize * _lGUI_vInfo.yres;

	_lGUI_vInfo.xoffset = 0;
	_lGUI_vInfo.yoffset = 0;
	ioctl (_lGUI_iFrameBuffer, FBIOPAN_DISPLAY, &_lGUI_vInfo);
	_lGUI_pFrameBuffer =mmap (NULL, _lGUI_iBufferSize, PROT_READ | PROT_WRITE, MAP_SHARED, _lGUI_iFrameBuffer, 0);
	if(!_lGUI_pFrameBuffer){
		printerror("mmap return error.");
		return false;
	}
	_lGUI_iBytesPerPixel = _lGUI_vInfo.bits_per_pixel / 8;
	if (_lGUI_iBytesPerPixel == 3)
		_lGUI_iBytesDataType = 4;
	else
		_lGUI_iBytesDataType = _lGUI_iBytesPerPixel;
	
	return true;
#else
	acoral_id screen_id;
	screen_id=acoral_dev_open("screen");
	if(screen_id==ACORAL_DEV_ERR_ID){
		acoral_prints("open screen error\n");
	}
	acoral_dev_config(screen_id,CMD_SCREEN_INFO,&_lGUI_vInfo,0);
	_lGUI_iFrameHeight=_lGUI_vInfo->yres;
	_lGUI_iFrameWidth=_lGUI_vInfo->xres;
	_lGUI_iLineSize = _lGUI_vInfo->xres * _lGUI_vInfo->bits_per_pixel / 8;
	_lGUI_iBufferSize = _lGUI_iLineSize * _lGUI_vInfo->yres;
	_lGUI_iBytesPerPixel = _lGUI_vInfo->bits_per_pixel / 8;
	if (_lGUI_iBytesPerPixel == 3)
		_lGUI_iBytesDataType = 4;
	else
		_lGUI_iBytesDataType = _lGUI_iBytesPerPixel;

	_lGUI_pFrameBuffer=_lGUI_vInfo->screen_buf;
	SCREEN_HEIGHT=_lGUI_iFrameHeight;
	SCREEN_WIDTH=_lGUI_iFrameWidth;
	return true;
#endif
}

void PortTimerAttach(void (*route)()){
}

