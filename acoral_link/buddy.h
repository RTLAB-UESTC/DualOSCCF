#ifndef     BUDDY_H
#define     BUDDY_H


#define     INVALID_PARA        4   //parameter is invalid
#define     INVALID_MEM_SIZE    5   //mem size error
#define     MALLOC_MEM_ERR      6   //malloc mem error

#define     MIN_BLK     1024        //minimum size of blk 
#define     M_VAL       12           //leval max size is MIN_BLK * 2~6
typedef struct WORD_b{
    struct WORD_b      *llink;      //left pointer
    struct WORD_b      *rlink;      //right pointer
    int         tag;                //if free tag=0,else tag=1
    int         kval;               //blcok size=(MIN_BLK<<kval)
    unsigned char *addr;            //mem block address
}WORD_b;

typedef struct FreeList{
    int         nodesize;           //list each block size
    WORD_b      *first;             //point to the first block
}FreeList;

//struct FreeList AllList[M_VAL+1];

void PrintMem(FreeList *avail);

//WORD_b* FindBuddy(WORD_b *p);
int     InsertWORD_b(struct FreeList *avail, WORD_b *pa);
WORD_b* AllocBuddy(struct FreeList *avail, unsigned int needsize);
int FreeBuddy(struct FreeList *avail, WORD_b *pfree);
int InitBuddy(struct FreeList *avail);
int AddMemBuddy(FreeList *avail, unsigned char* addr_start, unsigned char* addr_end);
#endif
