;@**************************************************************
;@ MMU Cache/TLB/etc on/off functions
;@**************************************************************
R1_I	EQU		1<<12
R1_C	EQU		1<<2
R1_A	EQU		1<<1
R1_M	EQU   	1
BUS_M	EQU 	3<<30
;R1		EQU		0X1007
    
    
    CODE32
    AREA    |.text|, CODE, READONLY
    
    
    EXPORT MMU_EnableICache
MMU_EnableICache
   mrc p15,0,r0,c1,c0,0
   orr r0,r0,#R1_I
   mcr p15,0,r0,c1,c0,0
   mov pc ,lr

;@**************************************************************
;@void MMU_DisableICache(void)
;@**************************************************************
  EXPORT MMU_DisableICache
MMU_DisableICache
   mrc p15,0,r0,c1,c0,0
   bic r0,r0,#R1_I
   mcr p15,0,r0,c1,c0,0
   mov pc ,lr

;@**************************************************************
;@void MMU_EnableDCache(void)
;@**************************************************************
   EXPORT MMU_EnableDCache
MMU_EnableDCache
   mrc p15,0,r0,c1,c0,0
   orr r0,r0,#R1_C
   mcr p15,0,r0,c1,c0,0
   mov pc ,lr

;@**************************************************************
;@void MMU_DisableDCache(void)
;@**************************************************************
  	EXPORT MMU_DisableDCache
MMU_DisableDCache
   mrc p15,0,r0,c1,c0,0
   bic r0,r0,#R1_C
   mcr p15,0,r0,c1,c0,0
   mov pc ,lr

;@**************************************************************
;@void MMU_EnableAlignFault(void)
;@**************************************************************
   EXPORT MMU_EnableAlignFault
MMU_EnableAlignFault
   mrc p15,0,r0,c1,c0,0
   orr r0,r0,#R1_A
   mcr p15,0,r0,c1,c0,0
   mov pc ,lr

;@**************************************************************
;@void MMU_DisableAlignFault(void)
;@**************************************************************
   EXPORT MMU_DisableAlignFault
MMU_DisableAlignFault
   mrc p15,0,r0,c1,c0,0
   bic r0,r0,#R1_A
   mcr p15,0,r0,c1,c0,0
   mov pc ,lr

;@**************************************************************
;@void MMU_EnableMMU(void)
;@**************************************************************
   EXPORT MMU_EnableMMU
MMU_EnableMMU
   mrc p15,0,r0,c1,c0,0
   orr r0,r0,#R1_M
   mcr p15,0,r0,c1,c0,0
   mov pc ,lr

;@**************************************************************
;;@void MMU_DisableMMU(void)
;@**************************************************************
   EXPORT MMU_DisableMMU
MMU_DisableMMU
   mrc p15,0,r0,c1,c0,0
   bic r0,r0,#R1_M
   mcr p15,0,r0,c1,c0,0
   mov pc ,lr

;@**************************************************************
;@ Set TTBase
;@ ro=TTBase
;@void MMU_SetTTBase(int base)
;@**************************************************************
   EXPORT MMU_SetTTBase
MMU_SetTTBase
   mcr p15,0,r0,c2,c0,0
   mov pc ,lr

;@**************************************************************
;@ Set Domain
;@void MMU_SetDomain(int domain)
;@ro=domain
;@**************************************************************
   EXPORT MMU_SetDomain
MMU_SetDomain
   mcr p15,0,r0,c3,c0,0
   mov pc ,lr

;@**************************************************************
;@ ICache/DCache functions
;@void MMU_InvalidateIDCache(void)
;@**************************************************************
   EXPORT MMU_InvalidateIDCache
MMU_InvalidateIDCache
   mcr p15,0,r0,c7,c7,0
   mov pc ,lr

;@**************************************************************
;@void MMU_InvalidateICache(void)
;@**************************************************************
   EXPORT MMU_InvalidateICache
MMU_InvalidateICache
   mcr p15,0,r0,c7,c5,0
   mov pc ,lr

;@**************************************************************
;@void MMU_InvalidateICacheMVA(U32 mva)
;@r0=mva
;@**************************************************************
   EXPORT MMU_InvalidateICacheMVA
MMU_InvalidateICacheMVA
   mcr p15,0,r0,c7,c5,1
   mov pc ,lr

;@**************************************************************
;@void MMU_PrefetchICacheMVA(U32 mva)
;@r0=mva
;@**************************************************************
   EXPORT MMU_PrefetchICacheMVA
MMU_PrefetchICacheMVA
   mcr p15,0,r0,c7,c13,1
   mov pc ,lr

;@**************************************************************
;@void MMU_InvalidateDCache(void)
;@**************************************************************
   EXPORT MMU_InvalidateDCache
MMU_InvalidateDCache
   mcr p15,0,r0,c7,c6,0
   mov pc ,lr

;@**************************************************************
;@void MMU_InvalidateDCacheMVA(U32 mva)
;@**************************************************************
   EXPORT MMU_InvalidateDCacheMVA
MMU_InvalidateDCacheMVA
   ;@r0=mva
   mcr p15,0,r0,c7,c6,1
   mov pc ,lr

;@**************************************************************
;@void MMU_CleanDCacheMVA(U32 mva)
;@**************************************************************
   EXPORT MMU_CleanDCacheMVA
MMU_CleanDCacheMVA
   ;@r0=mva
   mcr p15,0,r0,c7,c10,1
   mov pc ,lr

;@**************************************************************
;@void MMU_CleanInvalidateDCacheMVA(U32 mva)
;@r0=mva
;@**************************************************************
   EXPORT MMU_CleanInvalidateDCacheMVA
MMU_CleanInvalidateDCacheMVA
   mcr p15,0,r0,c7,c14,1
   mov pc ,lr

;@**************************************************************
;;@void MMU_CleanDCacheIndex(U32 index)
;@r0=index
;@**************************************************************
   EXPORT MMU_CleanDCacheIndex
MMU_CleanDCacheIndex
   mcr p15,0,r0,c7,c10,2
   mov pc ,lr

;@**************************************************************
;@void MMU_CleanInvalidateDCacheIndex(U32 index)
;@r0=index
;@**************************************************************
   EXPORT MMU_CleanInvalidateDCacheIndex
MMU_CleanInvalidateDCacheIndex
   mcr p15,0,r0,c7,c14,2
   mov pc ,lr

;@**************************************************************
;@void MMU_WaitForInterrupt(void)
;@**************************************************************
   EXPORT MMU_WaitForInterrupt
MMU_WaitForInterrupt
   mcr p15,0,r0,c7,c0,4
   mov pc ,lr

;@**************************************************************
;@ TLB functions
;@voic MMU_InvalidateTLB(void)
;@**************************************************************
   EXPORT MMU_InvalidateTLB
MMU_InvalidateTLB
   mcr p15,0,r0,c8,c7,0
   mov pc ,lr

;@**************************************************************
;@void MMU_InvalidateITLB(void)
;@**************************************************************
   EXPORT MMU_InvalidateITLB
MMU_InvalidateITLB
   mcr p15,0,r0,c8,c5,0
   mov pc ,lr

;@**************************************************************
;@void MMU_InvalidateITLBMVA(U32 mva)
;@ro=mva
;@**************************************************************
   EXPORT MMU_InvalidateITLBMVA
MMU_InvalidateITLBMVA
   mcr p15,0,r0,c8,c5,1
   mov pc ,lr

;@**************************************************************
;@void MMU_InvalidateDTLB(void)
;@**************************************************************
	EXPORT MMU_InvalidateDTLB
MMU_InvalidateDTLB
	mcr p15,0,r0,c8,c6,0
	mov pc ,lr

;@**************************************************************
;@ Process ID
;@void MMU_SetProcessId(U32 pid)
;@r0= pid
;@**************************************************************
   EXPORT MMU_SetProcessId
MMU_SetProcessId
   mcr p15,0,r0,c13,c0,0
   mov pc ,lr

