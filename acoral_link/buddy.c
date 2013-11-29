#include "buddy.h"
#include <linux/slab.h>
//#define NULL 0

/*Init FreeList*/
int InitBuddy(FreeList *avail)
{
    int i; 
    for(i = 0; i <= M_VAL; i++)
    {
        avail[i].nodesize = MIN_BLK<<i;
        avail[i].first = NULL;
    }
    return 0;
}

/*add memory to FreeList*/
/*split all memory to each FreeList,so the block address is not line location*/
/*the AllocBuddy can fastly get block*/
int AddMemBuddy(struct FreeList *avail, unsigned char* addr_start, unsigned char * addr_end)
{
    int i;
    WORD_b *blk;
    unsigned char* addr;
    if(!avail || !addr_start)
        return INVALID_PARA;
    if(addr_end - addr_start < MIN_BLK)
        return INVALID_MEM_SIZE;
    //addr_start &= ~(4-1);
    //addr_end &= ~(4-1);
    //addr_end -= (addr_end - addr_start)%MIN_BLK;
    i = 0;
    addr = addr_start;
    while(addr_end-addr>= MIN_BLK)
    {
        if(addr_end - addr < avail[i].nodesize) //if left memory < avail[i].nodesize,then i = 0,and split memory restart
        {
            i = 0;
            continue;
        }
        
        blk = (WORD_b *)kmalloc(sizeof(WORD_b), GFP_KERNEL);
        if(!blk)
            return MALLOC_MEM_ERR;
        blk->tag = 0;
        blk->kval = i;
        blk->addr = addr;
        InsertWORD_b(avail, blk);
        addr += avail[i].nodesize;              //increase addr
        i++;                                    //add leval
        if(i > M_VAL)                           //i > 6
            i = 0;
    }
    
    return 0;
}
/*
WORD_b* FindBuddy(WORD_b *p)
{
    int k = p->kval;
    unsigned int size = 1<<k;
    unsigned int size_to = 1<<(k+1); 
    if((p%size_to) == 0)
        return p + size;
    else
        return p - size;
}
*/
int     InsertWORD_b(struct FreeList *avail, WORD_b *pa)
{
    unsigned int size;
    int k = pa->kval;
    WORD_b *pnext;

    //printk("kval:%d\n", k);
    while(k <= M_VAL)
    {
       // printk("k <= M_VAL\n");
        pnext = avail[k].first;
        if(!pnext)                      //if the avail[k].first = NULL
        {
            //printk("pnext = 0\n");
            avail[k].first = pa;        //insert to the first block
            pa->rlink = pa;
            pa->llink = pa;
            return 0;
        }
        size = avail[k].nodesize;       //if not
        //find the prior block,order block from little to big with address 
        for(;pa->addr < pnext->addr && pnext->rlink!=avail[k].first; pnext = pnext->rlink);
        if(pnext->addr+size == pa->addr && k < M_VAL) //left buddy is addr_sequence
        {
            //delete right
            pnext->llink->rlink = pnext->rlink;
            pnext->rlink->llink = pnext->llink;
            //printk("right\n");
            kfree(pa);
            
            pa = pnext;
            pa->kval+=1;                    //increase size
            k++;
            continue;
        }
        if((pa->addr+size) == pnext->rlink->addr && k < M_VAL) //right buddy is addr_sequence
        {
            //delete left
            pnext = pnext->rlink;
            pnext->llink->rlink = pnext->rlink;
            pnext->rlink->llink = pnext->llink;
            //printk("left\n");
            kfree(pnext);
            
            pa->kval+=1;                    //increase size
            k++;
            continue;
        }
        
        
        pa->rlink = pnext->rlink;           //connot merge,insert to the right place
        pa->llink = pnext;
        pnext->rlink = pa;
        pa->rlink->llink = pa;
        //printk("find ok\n");
        return 0;
    }
    return 0;
}

WORD_b* AllocBuddy(struct FreeList *avail, unsigned int needsize)
{
    int i, k;
    WORD_b *pa, *pi;
    WORD_b *pre, *suc;
    //find the right block,if bigger ,split it
    for(k = 0; k <= M_VAL && (avail[k].nodesize < needsize || !avail[k].first); ++k);
    if(k > M_VAL)
        return NULL;
    else
    {
        pa = avail[k].first;
        pre = pa->llink;
        suc = pa->rlink;
        if(pa == suc)
        {
            avail[k].first = NULL;
            //printk("one blk\n");
        }
        else
        {
            //printk("not one\n");
            pre->rlink = suc;
            suc->llink = pre;
            avail[k].first = suc;
        }
        //split the bigger block
        for(i = 1; k-i >=0 && avail[k-i].nodesize >= needsize+1; ++i)
        {
            pi = (WORD_b *)kmalloc(sizeof(WORD_b), GFP_KERNEL);
            if(!pi)
            {
                printk("malloc pi error\n");
                return NULL;
            }
            pi->addr = pa->addr + avail[k-i].nodesize;
            pi->tag = 0;
            pi->kval = k-i;
            //printk("insert %x\n",pi->addr);
            InsertWORD_b(avail, pi);        //split it and insert to the right place
        }
        pa->tag = 1;                        //not free
        pa->kval = k-(--i);
    }
    return pa;
}

int FreeBuddy(struct FreeList *avail, WORD_b *pfree)
{
    InsertWORD_b(avail, pfree);
    return 0;
}

void PrintMem(FreeList *avail)
{
    int i;
    WORD_b *blk;
    for(i = 0; i <= M_VAL; i++)
    {
        blk = avail[i].first;
        printk("size:%x\n", avail[i].nodesize);
        while(blk && blk->rlink !=avail[i].first)
        {
            printk("%x\n", blk->addr);
            blk = blk->rlink; 
        }
        printk("\n");
    }
}
