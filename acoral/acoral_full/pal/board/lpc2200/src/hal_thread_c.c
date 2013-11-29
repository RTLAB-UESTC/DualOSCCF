#include"acoral.h"
void hal_stack_init(acoral_u32 **stk,void (*route)(),void (*exit)(),void *args){
	hal_ctx_t *ctx=(hal_ctx_t *)*stk;	
	ctx--;
	ctx=(hal_ctx_t *)((acoral_u32 *)ctx+1);
	ctx->r0=(acoral_u32)args;
	ctx->r1=1;
	ctx->r2=2;
	ctx->r3=3;
	ctx->r4=4;
	ctx->r5=5;
	ctx->r6=6;
	ctx->r7=7;
	ctx->r8=8;
	ctx->r9=9;
	ctx->r10=10;
	ctx->r11=11;
	ctx->r12=12;
	ctx->lr=(acoral_u32)exit;
	ctx->pc=(acoral_u32)route;
	ctx->cpsr=0x0000001fL;
    *stk=(acoral_u32 *)ctx;
 
}
