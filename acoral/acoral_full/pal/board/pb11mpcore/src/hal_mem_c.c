#include"acoral.h"

#include<type.h>
#include"hal_brd_cfg.h"



#define _MMUTT_STARTADDRESS     0x80000000

#define DESC_SEC                        (0x2|(1<<4))
#define CB                                      (3<<2)  //cache_on, write_back
#define CNB                                     (2<<2)  //cache_on, write_through
#define NCB                                     (1<<2)  //cache_off,WR_BUF on
#define NCNB                            (0<<2)  //cache_off,WR_BUF off
#define AP_RW                           (3<<10) //supervisor=RW, user=RW
#define AP_RO                           (2<<10) //supervisor=RW, user=RO

#define DOMAIN_FAULT            (0x0)
#define DOMAIN_CHK                      (0x1)
#define DOMAIN_NOTCHK           (0x3)
#define DOMAIN0                         (0x0<<5)
#define DOMAIN1                         (0x1<<5)

#define DOMAIN0_ATTR            (DOMAIN_CHK<<0)
#define DOMAIN1_ATTR            (DOMAIN_FAULT<<2)

#define RW_CB                           (AP_RW|DOMAIN0|CB|DESC_SEC)
#define RW_CNB                          (AP_RW|DOMAIN0|CNB|DESC_SEC)
#define RW_NCNB                         (AP_RW|DOMAIN0|NCNB|DESC_SEC)
#define RW_FAULT                        (AP_RW|DOMAIN1|NCNB|DESC_SEC)


void mmu_setttbase(register acoral_u32 i)
{
        asm ("mcr       p15, 0, %0, c2, c0, 0": :"r" (i));
}

void mmu_set_domain(register acoral_u32 i)
{
        asm ("mcr       p15, 0, %0, c3, c0, 0": :"r" (i));
}

void mmu_enable()
{
        register acoral_u32 i;

        /* read control register */
        asm ("mrc       p15, 0, %0, c1, c0, 0":"=r" (i));

        i |= 0x1;

        /* write back to control register */
        asm ("mcr       p15, 0, %0, c1, c0, 0": :"r" (i));
}

void mmu_disable()
{
        register acoral_u32 i;

        /* read control register */
        asm ("mrc       p15, 0, %0, c1, c0, 0":"=r" (i));

        i &= ~0x1;

        /* write back to control register */
        asm ("mcr       p15, 0, %0, c1, c0, 0": :"r" (i));
}

void mmu_enable_icache()
{
        register acoral_u32 i;

        /* read control register */
        asm ("mrc       p15, 0, %0, c1, c0, 0":"=r" (i));

        i |= (1 << 12);

        /* write back to control register */
        asm ("mcr       p15, 0, %0, c1, c0, 0": :"r" (i));
}

void mmu_enable_dcache()
{
        register acoral_u32 i;

        /* read control register */
        asm ("mrc       p15, 0, %0, c1, c0, 0":"=r" (i));

        i |= (1 << 2);

        /* write back to control register */
        asm ("mcr       p15, 0, %0, c1, c0, 0": :"r" (i));
}

void mmu_disable_icache()
{
        register acoral_u32 i;

        /* read control register */
        asm ("mrc       p15, 0, %0, c1, c0, 0":"=r" (i));

        i &= ~(1 << 12);

        /* write back to control register */
        asm ("mcr       p15, 0, %0, c1, c0, 0": :"r" (i));
}

void mmu_disable_dcache()
{
        register acoral_u32 i;

        /* read control register */
        asm ("mrc       p15, 0, %0, c1, c0, 0":"=r" (i));

        i &= ~(1 << 2);

        /* write back to control register */
        asm ("mcr       p15, 0, %0, c1, c0, 0": :"r" (i));
}

void mmu_enable_alignfault()
{
        register acoral_u32 i;

        /* read control register */
        asm ("mrc       p15, 0, %0, c1, c0, 0":"=r" (i));

        i |= (1 << 1);

        /* write back to control register */
        asm ("mcr       p15, 0, %0, c1, c0, 0": :"r" (i));
}

void mmu_disable_alignfault()
{
        register acoral_u32 i;

        /* read control register */
        asm ("mrc       p15, 0, %0, c1, c0, 0":"=r" (i));

        i &= ~(1 << 1);

        /* write back to control register */
        asm ("mcr       p15, 0, %0, c1, c0, 0": :"r" (i));
}

void mmu_clean_invalidated_cache_index(int index)
{
        asm ("mcr       p15, 0, %0, c7, c14, 2": :"r" (index));
}

void mmu_invalidate_tlb()
{
        asm ("mcr       p15, 0, %0, c8, c7, 0": :"r" (0));
}

void mmu_invalidate_dcache()
{
        asm ("mcr       p15, 0, %0, c7, c14, 0": :"r" (0));
}

void mmu_invalidate_icache()
{
        asm ("mcr       p15, 0, %0, c7, c5, 0": :"r" (0));
}

void mmu_setmtt(acoral_u32 vaddrStart, acoral_u32 vaddrEnd, acoral_u32 paddrStart, acoral_u32 attr)
{
    volatile acoral_u32 *pTT;
    volatile int i, nSec;

    pTT = (acoral_u32 *)_MMUTT_STARTADDRESS + (vaddrStart>>20);
    nSec = (vaddrEnd>>20) - (vaddrStart>>20);
    for (i=0; i<=nSec; i++)
    {
                *pTT = attr |(((paddrStart>>20)+i)<<20);
                pTT++;
    }
}

void mmu_init(void)
{
        int i;

        mmu_disable_dcache();
        mmu_disable_icache();
        mmu_invalidate_dcache();
        mmu_invalidate_icache();

        // icache may be turned on here.
        mmu_enable_icache();

        mmu_disable();
        mmu_invalidate_tlb();

        // clear all page tables
        for (i=0; i<(4*1024); i++)
        {
                *(volatile unsigned int *)(_MMUTT_STARTADDRESS + i*4) = 0;
        }
        mmu_setmtt(0x00000000, 0xFFFFFFFF, 0x00000000,RW_CB);         // bank0
       // mmu_setmtt(0x0c000000, 0x10000000, 0x0c000000, RW_CNB);         // stepping-stone
        //mmu_setmtt(0x70000000, 0x80000000, 0x70000000, RW_NCNB);        // SFR

	//mmu_setmtt(0x00000000, 0x00100000, 0x01000000,RW_CB);
	//mmu_setmtt(0x06000000,0xFFFFFFFF,0x06000000,RW_CB);

        mmu_setttbase(_MMUTT_STARTADDRESS);

        // DOMAIN1: no_access, DOMAIN0,2~15=client(AP is checked)
        mmu_set_domain(0x55555550|DOMAIN1_ATTR|DOMAIN0_ATTR);

        mmu_enable_alignfault();

        mmu_enable();

        // ICache enable
       // mmu_enable_icache();
        // DCache should be turned on after mmu is turned on.
       // mmu_enable_dcache();
}


void hal_mem_init(){
       mmu_init();
       //MMU_init();
}


void hal_scu_enable(void)
{
	acoral_u32 scu_ctrl;
	scu_ctrl = rSCUCTRL;
	if (!(scu_ctrl & 1)) {
		/* not enabled yet */
		scu_ctrl |= 1;
		rSCUCTRL=scu_ctrl;
	}
}


