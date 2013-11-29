#define IN_DISK
#include "../include/fs.h"
Disk_Info DiskInfo[MAX_DRIVES];     //逻辑盘信息

/*********************************************************************************************************
** 函数名称: DiskInit
** 功能描述: 初始化逻辑盘管理模块
**
** 输　入: 无
**
** 输　出: 无
**         
** 全局变量: DiskInfo
** 调用模块: 无
********************************************************************************************************/
        void DiskInit(void)
{
    acoral_u8 i;
    
    for (i = 0; i < MAX_DRIVES; i++)
    {
        DiskInfo[i].Drive = EMPTY_DRIVE;    // 没有分配
        DiskInfo[i].FATType = 0xff;         // 无效的文件系统
        DiskInfo[i].SecPerClus = 0;         // 每簇扇区数
        DiskInfo[i].NumFATs = 0;            // FAT表数
        DiskInfo[i].DevId= -1;
        DiskInfo[i].SecPerDisk = 0xffffffff;// 逻辑驱动器包含扇区数
        DiskInfo[i].BytsPerSec = 0;         // 每扇区字节数
        DiskInfo[i].RootDirTable = 0;       // 根目录开始扇区号（FAT32为开始簇号）
        DiskInfo[i].RootSecCnt = 0;         // 根目录占用扇区数
        DiskInfo[i].FATStartSec = 0;        // FAT表开始扇区号
        DiskInfo[i].FATSecCnt = 0;          // 每个FAT占用扇区数
        DiskInfo[i].DataStartSec = 0;       // 数据区开始扇区号
        DiskInfo[i].PathClusIndex = 0;      // 当前目录
        DiskInfo[i].DiakCommand = NULL;     // 驱动程序
    }
    CacheInit();                //初始化磁盘cache
    CurrentDrive=MAX_DRIVES;
}

/*********************************************************************************************************
** 函数名称: GetDiskInfo
** 功能描述: 获取指定逻辑盘信息
**
** 输　入: 逻辑盘号
**
** 输　出: 指向逻辑盘信息的结构体指针
**         
** 全局变量: DiskInfo
** 调用模块: 无
********************************************************************************************************/
        Disk_Info * GetDiskInfo(acoral_u8 Drive)
{
    Disk_Info *Rt;
    Rt = NULL;
    if (Drive < MAX_DRIVES)
    {
        if (DiskInfo[Drive].Drive != EMPTY_DRIVE)
        {
            Rt = DiskInfo + Drive;
        }
    }
    return Rt;
}

/*********************************************************************************************************
** 函数名称: GetEmptyDiskInfoAddr
** 功能描述: 获取空闲逻辑盘
**
** 输　入: 无
**
** 输　出: 指向逻辑盘信息的结构体指针
**         
** 全局变量: DiskInfo
** 调用模块: 无
********************************************************************************************************/
        Disk_Info * GetEmptyDiskInfoAddr(void)
{
    acoral_u8 i;
    Disk_Info *Rt;
    
    Rt = NULL;
    for (i = 0; i < MAX_DRIVES; i++)
    {
        if (DiskInfo[i].Drive == EMPTY_DRIVE)     // 逻辑盘号空闲
        {
            Rt = DiskInfo + i;
            DiskInfo[i].Drive = i;
            break;
        }
    }
    return Rt;
}

/*********************************************************************************************************
** 函数名称: AddFileDriver
** 功能描述: 增加一个底层驱动程序
**
** 输　入: DiakCommand：驱动程序接口函数
**
** 输　出: 无
**         
** 全局变量: DiskInfo
** 调用模块: 无
********************************************************************************************************/
static acoral_u8 Buf[512];
        void AddFileDriver(acoral_u16  (* DiakCommand)(acoral_u8 Cammand, void *Parameter),acoral_char *devname)
{
    acoral_u8 i;

    acoral_u16 temp;
    acoral_u32 temp1;
    
    if (DiakCommand == NULL)        // 参数无效退出
    {
        return;
    }

    if (DiakCommand(DISK_INIT, devname) == DISK_INIT_NOT_OK)   // 底层驱动初始化不成功退出
    {
        return;
    }
    
    /* 重新获取逻辑盘信息 */
    for (i = 0; i < MAX_DRIVES; i++)
    {
        if (DiskInfo[i].Drive != EMPTY_DRIVE)       /* 逻辑盘已分配 */
        {
        	/*加载驱动时将CurrentDrive设为改驱动对应的设备*/
        	CurrentDrive=i;
        	/*将设备ID存入设备结构体中*/
           /*	if(DiskInfo[i].DevId==ACORAL_DEV_ERR_ID)
				DiskInfo[i].DevId=acoral_dev_open(devname);
            *///Buf = OpenSec(DiskInfo[i].Drive, 0);    /* 打开扇区0（保留扇区） */
            ReadSuperSec(i,Buf);
            if (Buf != NULL)
            {
                /* 每扇区字节数 */
                DiskInfo[i].BytsPerSec = Buf[11] | (Buf[12] << 8);
                /* 每簇扇区数 */
                DiskInfo[i].SecPerClus = Buf[13];
                /* FAT开始扇区号 */
                DiskInfo[i].FATStartSec = Buf[14] | (Buf[15] << 8);
                /* FAT表个数 */
                DiskInfo[i].NumFATs = Buf[16];
                /* 根目录占扇区数 */ 
                DiskInfo[i].RootSecCnt = ((Buf[17] | (Buf[18] << 8)) * 32 + 
                                           DiskInfo[i].BytsPerSec - 1) /
                                           DiskInfo[i].BytsPerSec;
                /* 逻辑盘（卷）占扇区数 */
                temp = Buf[19] | (Buf[20] << 8);
                if (temp != 0)
                {
                    DiskInfo[i].SecPerDisk = temp;
                }
                else
                {
                    DiskInfo[i].SecPerDisk = Buf[32] | (Buf[33] << 8);
                    DiskInfo[i].SecPerDisk |= (((acoral_u32)Buf[34] << 16) | ((acoral_u32)Buf[35] << 24));
                }
                /* FAT表占用扇区数 */
                temp = Buf[22] | (Buf[23] << 8);
                if (temp != 0)
                {
                    DiskInfo[i].FATSecCnt = temp;
                }
                else
                {
                    DiskInfo[i].FATSecCnt = Buf[36] | (Buf[37] << 8);
                    DiskInfo[i].FATSecCnt |= (((acoral_u32)Buf[38] << 16) | (acoral_u32)(Buf[39] << 24));
                }
                
                /* 当前目录为根目录 */
                DiskInfo[i].PathClusIndex = 0;
            }
            /* 根目录开始扇区号 */
            DiskInfo[i].RootDirTable = DiskInfo[i].NumFATs * DiskInfo[i].FATSecCnt + 
                                       DiskInfo[i].FATStartSec;
            /* 数据区开始扇区号 */
            DiskInfo[i].DataStartSec = DiskInfo[i].RootDirTable + DiskInfo[i].RootSecCnt;
            temp1 = DiskInfo[i].SecPerDisk - DiskInfo[i].DataStartSec;
            temp1 = temp1 / DiskInfo[i].SecPerClus;
            DiskInfo[i].ClusPerData = temp1;
            /* 判断是FAT12、FAT16还是FAT32 */
            if (temp1 < 4085)
            {
                DiskInfo[i].FATType = FAT12;
            }
            else if (temp1 < 65525)
            {
                DiskInfo[i].FATType = FAT16;
            }
            else
            {
                DiskInfo[i].FATType = FAT32;
                /* FAT32 RootDirTable为开始簇号 */
                DiskInfo[i].RootDirTable = Buf[44] | (Buf[45] << 8);
                DiskInfo[i].RootDirTable |= (((acoral_u32)Buf[46] << 16) | ((acoral_u32)Buf[47] << 24));
                DiskInfo[i].PathClusIndex = DiskInfo[i].RootDirTable;
            }
            //CloseSec(DiskInfo[i].Drive, 0);     /* 关闭扇区 */
        }
    }
}

/*********************************************************************************************************
** 函数名称: RemoveFileDriver
** 功能描述: 删除一个底层驱动程序
**
** 输　入: Drive:逻辑驱动器号
**
** 输　出: 无
**         
** 全局变量: DiskInfo
** 调用模块: 无
********************************************************************************************************/
        void RemoveFileDriver(acoral_u8 Drive)
{
    Disk_RW_Parameter Pa;
    
    if (Drive < MAX_DRIVES)
    if (DiskInfo[Drive].Drive != EMPTY_DRIVE)
    {
        AllCacheWriteBack();
        Pa.Drive = Drive;
        Pa.RsvdForLow = DiskInfo[Drive].RsvdForLow;
        DiskInfo[Drive].DiakCommand(DISK_CLOSE, &Pa);

        DiskInfo[Drive].Drive = EMPTY_DRIVE;    // 没有分配
        DiskInfo[Drive].FATType = 0xff;         // 无效的文件系统
        DiskInfo[Drive].SecPerClus = 0;         // 每簇扇区数
        DiskInfo[Drive].NumFATs = 0;            // FAT表数
        DiskInfo[Drive].DevId=-1;
        DiskInfo[Drive].SecPerDisk = 0xffffffff;// 逻辑驱动器包含扇区数
        DiskInfo[Drive].BytsPerSec = 0;         // 每扇区字节数
        DiskInfo[Drive].RootDirTable = 0;       // 根目录开始扇区号（FAT32为开始簇号）
        DiskInfo[Drive].RootSecCnt = 0;         // 根目录占用扇区数
        DiskInfo[Drive].FATStartSec = 0;        // FAT表开始扇区号
        DiskInfo[Drive].FATSecCnt = 0;          // 每个FAT占用扇区数
        DiskInfo[Drive].DataStartSec = 0;       // 数据区开始扇区号
        DiskInfo[Drive].PathClusIndex = 0;      // 当前目录
        DiskInfo[Drive].DiakCommand = NULL;     // 驱动程序
    }
}


/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
