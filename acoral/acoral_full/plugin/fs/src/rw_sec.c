#define IN_RWSEC
#include "../include/fs.h"
Disk_cache DiskCache[MAX_DISK_CACHES];      /* 磁盘cache */

/*********************************************************************************************************
** 函数名称: CacheInit
** 功能描述: 初始化磁盘cache
**
** 输　入: 无
**
** 输　出: 无
**         
** 全局变量: DiskCache
** 调用模块: 无
********************************************************************************************************/
        void CacheInit(void)
{
    acoral_u16 i;
    
    for (i = 0; i < MAX_DISK_CACHES; i++)
    {
        DiskCache[i].Drive = EMPTY_DRIVE;   /* cache空闲 */
        DiskCache[i].Flag = 0;              /* 扇区无需回写 */
        DiskCache[i].RW_ID = 0;             /* 访问纪录 */
        DiskCache[i].RW_ID--;               /* 访问纪录 */
        DiskCache[i].SecIndex = 0;          /* 缓冲的扇区号  */
    }
}


/*********************************************************************************************************
** 函数名称: CloseSec
** 功能描述: 解锁指定cache（允许切换出内存）
**
** 输　入: Drive：逻辑驱动器号
**        Index：缓冲的扇区号
** 输　出: 无
**         
** 全局变量: DiskCache
** 调用模块: 无
********************************************************************************************************/
        void CloseSec(acoral_u8 Drive, acoral_u32 Index)
{
    Drive = Drive;
    Index = Index;
}

/*********************************************************************************************************
** 函数名称: CacheWriteBack2
** 功能描述: 把指定扇区写回逻辑盘
**
** 输　入: Index：cache索引
**
** 输　出: 无
**         
** 全局变量: DiskCache
** 调用模块: GetDiskInfo
********************************************************************************************************/
        void CacheWriteBack2(acoral_u16 Index)
{
    Disk_RW_Parameter Pa;
    Disk_Info * Disk;

    Disk = GetDiskInfo(DiskCache[Index].Drive);     // 获取逻辑盘信息
    if (Disk != NULL)
    if (Disk->DiakCommand != NULL)
    {
        DiskCache[Index].Flag &= ~CACHE_WRITED;     // cache 不再需要回写
        /* 传递参数 */
        Pa.Drive = DiskCache[Index].Drive;               
        Pa.SectorIndex = DiskCache[Index].SecIndex;
        Pa.RsvdForLow = Disk->RsvdForLow;
        Pa.Buf = DiskCache[Index].buf;
        
        Disk->DiakCommand(DISK_WRITE_SECTOR, &Pa);  /* 调用底层驱动写扇区 */ 
    }
}

/*********************************************************************************************************
** 函数名称: CacheWriteBack
** 功能描述: 把指定cache写回逻辑盘
**
** 输　入: Drive：逻辑驱动器号
**        Index：缓冲的扇区号
** 输　出: 无
**         
** 全局变量: DiskCache
** 调用模块: CacheWriteBack2
********************************************************************************************************/
       void CacheWriteBack(acoral_u8 Drive, acoral_u32 Index)
{
    acoral_u16 i;
    
    for (i = 0; i < MAX_DISK_CACHES; i++)
    {
        if (DiskCache[i].Drive == Drive)
        if (DiskCache[i].SecIndex == Index)
        {
            if ((DiskCache[i].Flag & CACHE_WRITED) != 0)
            {
                CacheWriteBack2(i);
            }
            break;
        }
    }
}

/*********************************************************************************************************
** 函数名称: AllCacheWriteBack
** 功能描述: 把所有已改变的扇区写回逻辑盘
**
** 输　入: 无
**
** 输　出: 无
**         
** 全局变量: DiskCache
** 调用模块: CacheWriteBack2
********************************************************************************************************/
       void AllCacheWriteBack(void)
{
    acoral_u16 i;

    for (i = 0; i < MAX_DISK_CACHES; i++)
    {
        if ((DiskCache[i].Flag & CACHE_WRITED) != 0)
        {
            CacheWriteBack2(i);
        }
    }
}

/*********************************************************************************************************
** 函数名称: GetCache
** 功能描述: 获取一个cache
**
** 输　入: 无
**
** 输　出: cache索引
**         
** 全局变量: DiskCache
** 调用模块: 无
********************************************************************************************************/
        acoral_u16 GetCache(void)
{
    acoral_u16 Max;
    acoral_u16 i,j;

    Max = 0;
    j = 0;
    j--;
    for (i = 0; i < MAX_DISK_CACHES; i++)
    {
        if (Max <= DiskCache[i].RW_ID)
        {
            Max = DiskCache[i].RW_ID;
            j = i;
        }
        if (Max == (acoral_u16)(-1))
        {
            break;
        }
    }
    if (j < MAX_DISK_CACHES)
    {
        if (DiskCache[j].Drive != EMPTY_DRIVE)
        if ((DiskCache[j].Flag & CACHE_WRITED) != 0)
        {
            CacheWriteBack2(j);
        }
    }
    return j;
}

/*********************************************************************************************************
** 函数名称: OpenSec
** 功能描述: 为逻辑盘上的一个扇区打开一个cache并锁定
**
** 输　入: Drive：磁盘号
**        Index：扇区号
** 输　出: 指向指定扇区数据的指针
**         
** 全局变量: DiskCache
** 调用模块: 无
********************************************************************************************************/
        acoral_u8 *OpenSec(acoral_u8 Drive, acoral_u32 Index)
{
    acoral_u16 i;
    acoral_u8 *Rt;
    Disk_Info *Disk;
   
    /* 增加访问间隔 */
    for (i = 0; i < MAX_DISK_CACHES; i++)
    {
        if (DiskCache[i].Drive != EMPTY_DRIVE)
        {
            if (DiskCache[i].RW_ID < (acoral_u16)(-1))
            {
                DiskCache[i].RW_ID++;
            }
        }
    }

    /* 看扇区是否已经缓存 */
    Rt = NULL;
    for (i = 0; i < MAX_DISK_CACHES; i++)
    {
        if (DiskCache[i].Drive == Drive )
        if (DiskCache[i].SecIndex == Index)
        {
            Rt = DiskCache[i].buf;
            DiskCache[i].RW_ID = 0;
            break;
        }
    }
    
    /* Rt == NULL则未缓存 */
    if (Rt == NULL)
    {
        Disk = GetDiskInfo(Drive);
        if (Disk != NULL)
        if (Disk->SecPerDisk > Index)
        {
            i = GetCache();                     /* 获取cache */
            if (i < MAX_DISK_CACHES)
            {
                /* 初始化cache  */
                DiskCache[i].Drive = Drive;     
                DiskCache[i].RW_ID = 0;
                DiskCache[i].Flag = 0;
                DiskCache[i].SecIndex = Index;
                Rt = DiskCache[i].buf;
                for (i = 0; i < Disk->BytsPerSec; i++)
                {
                    Rt[i] = 0;
                }
            }
        }
    }
    return Rt;
}

acoral_u8 ReadSuperSec(acoral_u8 Drive,acoral_u8 *buf)
{
	Disk_RW_Parameter Pa;
	Disk_Info *Disk;
        Disk = GetDiskInfo(Drive);    
	Pa.Drive=Drive;
	Pa.Buf=buf;
        if(Disk->DiakCommand(DISK_READ_SUPER_SECTOR, &Pa) == DISK_READ_OK)
        {
                return RETURN_OK;
	}
	return SECTOR_READ_ERR;
}

/*********************************************************************************************************
** 函数名称: ReadSec
** 功能描述: 从逻辑盘读扇区
**
** 输　入: Disk：逻辑盘信息
**        Index：扇区号
** 输　出: TRUE:成功
**         FALSE:失败
** 全局变量: DiskCache
** 调用模块: GetDiskInfo
********************************************************************************************************/
        acoral_u8 ReadSec(acoral_u8 Drive, acoral_u32 Index)
{
    acoral_u16 i;
    Disk_RW_Parameter Pa;
    Disk_Info *Disk;
    acoral_u8 Rt;
    
    for (i = 0; i < MAX_DISK_CACHES; i++)
    {
        if (DiskCache[i].Drive == Drive)
        if (DiskCache[i].SecIndex == Index)
        {
            if ((DiskCache[i].Flag & CACHE_READED) != 0)
            {
                return RETURN_OK;
            }
            break;
        }
    }
    Rt = SECTOR_NOT_IN_CACHE;
    if (i < MAX_DISK_CACHES)
    {
        Disk = GetDiskInfo(Drive);    

        DiskCache[i].Flag |= CACHE_READED;

        /* 从磁盘读取扇区数据 */
        Pa.Drive = Drive;
        Pa.SectorIndex = Index;
        Pa.RsvdForLow = Disk->RsvdForLow;
        Pa.Buf = DiskCache[i].buf;
        Rt = NOT_FIND_DISK;
        if (Disk->DiakCommand != NULL)
        {

            Rt = SECTOR_READ_ERR;
            if (Disk->DiakCommand(DISK_READ_SECTOR, &Pa) == DISK_READ_OK)
            {
                return RETURN_OK;
            }
        }
    }
    return Rt;
}

/*********************************************************************************************************
** 函数名称: WriteSec
** 功能描述: 说明指定逻辑盘的指定一个扇区被改写
**
** 输　入: Disk：逻辑盘信息
**        Index：扇区号
** 输　出: 无
**         
** 全局变量: DiskCache
** 调用模块: 无
********************************************************************************************************/
        void WriteSec(acoral_u8 Drive, acoral_u32 Index)
{
    acoral_u16 i;
    
    for (i = 0; i < MAX_DISK_CACHES; i++)
    {
        if (DiskCache[i].Drive == Drive)
        if (DiskCache[i].SecIndex == Index)
        {
            DiskCache[i].Flag |= (CACHE_WRITED | CACHE_READED);
            break;
        }
    }
}

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
