#include "acoral_link.h"

void send_ipi(unsigned int cmd)
{
	iowrite32(cmd, ipi_base);
}

int find_and_set_bit_appMemInfo(volatile unsigned long *bitmap)
{
    int nr;
    nr = find_first_zero_bit((void *)bitmap, sizeof(int)*8);
    if(nr >= sizeof(int)*8)
        return -1;
    set_bit(nr, bitmap);
    return nr;
}

int free_bit_appMemInfo(int nr, unsigned long *bitmap)
{
    clear_bit(nr, bitmap);
    return 0;
}

int   timeval_subtract(char *str,   struct   timeval*   x,struct   timeval*   y)  
{  
    struct timeval result;
     //int   nsec;  
     if(x->tv_sec>y->tv_sec)  
        return   -1;  
                       
    if((x->tv_sec==y->tv_sec) && (x->tv_usec>y->tv_usec))  
        return   -1;  
                               
    result.tv_sec = (y->tv_sec-x->tv_sec);  
    result.tv_usec = (y->tv_usec-x->tv_usec);  
                                               
    if(result.tv_usec<0)  
    {  
        result.tv_sec--;
        result.tv_usec+=1000000;  
                                                                                        }  
    printk("%s:%ld.%ld\n", str, result.tv_sec, result.tv_usec);
    return   0;  
}     

 int acoral_link_app(App_Info appInfo)
{
	struct file *fp;            //file pointer
	mm_segment_t old_fs;        
    loff_t pos = 0;
	void __iomem *add;          //temp address
	unsigned int  file_len = 0; //file length
	int index;                  //which appMemInfo

    struct   timeval   start,stop;

    //App_para *para; 
    WORD_b *app_blk = 0;        //store app.o
    WORD_b *para_blk = 0;       //store app para struct
    WORD_b *ret_blk = 0;        //store app return value

    do_gettimeofday(&start);
    //App_Info appInfo = appInfo_t;
    //printk("filename:%s\n", appInfo.filename);
    //printk("para:%x\n", appInfo.para);
    //printk("ret:%x\n", appInfo.ret);
    //printk("parasize:%d\n",appInfo.parasize);
    //printk("acoral enter\n");
    fp = filp_open(appInfo.filename, O_RDONLY , 0644);
   	if (IS_ERR(fp)) {
         printk("create file error\n");
        return -1;
     }
    old_fs = get_fs();      //get_fs是取得当前的地址访问限制值
    set_fs(KERNEL_DS);  //set_fs是设置当前的地址访问限制值  KERNEL_DS会跳过地址检查
	
	 file_len = vfs_llseek(fp, 0, SEEK_END);//get file length
     
     /*find free appMemInfo struct*/
    local_irq_disable();
    while((index = find_and_set_bit_appMemInfo(appMemInfo_bitmap)) < 0);
    local_irq_enable();    
    printk("index: %d\n", index);


	app_blk = AllocBuddy(AllList, file_len);
    if(app_blk == 0)
    {
        printk("alloc error\n");
        return -1;
    }
    para_blk = AllocBuddy(AllList, appInfo.parasize);
    if(para_blk == 0)
    {
        printk("para_blk error\n");
        return -1;
    }
    ret_blk = AllocBuddy(AllList, appInfo.retsize);
    if(ret_blk == 0)
    {
        printk("ret_blk error\n");
        return -1;
    }
    
    //translate the virtuel address to physical address
    memcpy(para_blk->addr, appInfo.para, appInfo.parasize);
	appMemInfo[index].addr = app_blk->addr - first_blk_phy + ALLOC_MEM_START;//virt_to_bus(blk->addr);
    appMemInfo[index].para = para_blk->addr - first_blk_phy + ALLOC_MEM_START;//virt_to_bus(para_blk->addr);
    appMemInfo[index].ret = ret_blk->addr - first_blk_phy + ALLOC_MEM_START;
    appMemInfo[index].com_index = index;    /////which com
    appMemInfo[index].prio = appInfo.prio;
/*
    memcpy(&appMemInfo[1], &appMemInfo[0], sizeof(App_Mem_Info));
    memcpy(&appMemInfo[2], &appMemInfo[0], sizeof(App_Mem_Info));

    acoral_tasks = 3;
    appMemInfo[0].task_start = 0;
    appMemInfo[1].task_start = 150;
    appMemInfo[2].task_start = 300;
    */
   // printk("blk_addr:%x\n", appMemInfo[index].addr);
    //printk("para: %x\n", appMemInfo[index].para);
    //printk("ret: %x\n", appMemInfo[index].ret);
    printk("prio:%d\n", appMemInfo[index].prio);

    //para = (App_para *)(para_blk->addr);
    //printk("%d : %d\n", para->para1, para->para2);
	add = (void __iomem *)(app_blk->addr);
	if(vfs_read(fp, add, file_len, &pos) != file_len)
	{
		printk("vfs_read err\n");
		return -1;
	}
	
     //printk("read file length: %d\n", file_len);

     filp_close(fp, NULL);
     set_fs(old_fs);         //恢复地址访问限制值

     do_gettimeofday(&stop);
    timeval_subtract("para_time",&start,&stop); 
	send_ipi(ACORAL_IPI_INT3_CPU1_APP);
   // wait_for_completion(&memInfo[index].com);
    //init_completion(&acoral_com);
    wait_for_completion(&acoral_com[index]);

    //printk("result:%d ret: %d\n", *(ret_blk->addr), *(int *)appInfo.ret);
    //printk("retadd:%x\n", appInfo.ret);

    do_gettimeofday(&start);
    memset(&appMemInfo[index], 0, sizeof(App_Mem_Info));
    if(copy_to_user(appInfo.ret, ret_blk->addr, appInfo.retsize)) 
        return -EFAULT;
   // free_bit_appMemInfo(index, appMemInfo_bitmap);
    FreeBuddy(AllList, app_blk);
    FreeBuddy(AllList, para_blk);
    FreeBuddy(AllList, ret_blk);
    do_gettimeofday(&stop);
    timeval_subtract("trans_ret_time",&start,&stop); 
	//return appMemInfo[index].ret;;
    return 0;
}


void clear_appMemInfo(App_Mem_Info *memInfo)
{
    memset(memInfo, 0, sizeof(App_Mem_Info));
}

int init_Mem_Info(void)
{
    int i;
    for(i = 0; i < MAX_APP_MEM; i++)
    {
        //appMemInfo[i].addr = 0; 
        //appMemInfo[i].para = 0;
        //appMemInfo[i].ret = 0;
        //appMemInfo[i].com_index = 0;
        //clear_appMemInfo(&appMemInfo[i]);
        memset(&appMemInfo[i], 0, sizeof(App_Mem_Info));
        init_completion(&acoral_com[i]);
    }
    return 0;
}

int init_buddy(void)
{
    InitBuddy(AllList);
    return AddMemBuddy(AllList, (unsigned char *)SHARE_MEM_START, (unsigned char *)SHARE_MEM_END);
}

/*translation acoral.bin to */
int trans_acoral(void __iomem* io_base)
{
	struct file *fp;
	mm_segment_t old_fs;
    loff_t pos = 0;
	void __iomem *add;
	
	
    printk("acoral enter\n");
    fp = filp_open("/acoral.bin", O_RDONLY , 0644);
   	if (IS_ERR(fp)) {
         printk("create file error\n");
        return -1;
     }
     old_fs = get_fs();      //get_fs是取得当前的地址访问限制值
     set_fs(KERNEL_DS);  //set_fs是设置当前的地址访问限制值  KERNEL_DS会跳过地址检查
    
	
	 file_len = vfs_llseek(fp, 0, SEEK_END);

	add = (void __iomem *)(io_base);
	if(vfs_read(fp, add, file_len, &pos) != file_len)
	{
		printk("vfs_read err\n");
		return -1;
	}
	
     printk("read file length: %d\n", file_len);

     filp_close(fp, NULL);
     set_fs(old_fs);         //恢复地址访问限制值
	return 0;
}


/*set some register to start acoral*/
int startAcoral(void)
{
	void __iomem *add;
	acoral_base = ioremap_nocache(ACORAL_BASE, ACORAL_MAX - ACORAL_BASE);
	if(!acoral_base)
	{
		printk("ioremap acoral_base error!\n");
		return -1;
	}
	
	
	
	flags_base = ioremap_nocache(BRD_FLAGS_RES, 4);
	if(!flags_base)
	{	
		printk("ioremap flags_base error!\n");
		return -1;
	}

	ipi_base = ioremap_nocache(IPI_RES, 4);
	if(!ipi_base)
	{
		printk("ioremap ipi_base error!\n");
		return -1;
	}
	

	if(trans_acoral(acoral_base) < 0)
	{
		printk("trans_acoral error\n");
		return -1;
	}
	//printmem(acoral_base, 1024 );

    //clean app_flag
    //app_flag_res_base = ioremap_nocache(APP_FLAG_RES, 12);
    //add = (void __iomem *)(app_flag_res_base);
    //memset_io(add, 0x0, 12);

	add = (void __iomem *)(flags_base);
	iowrite32(ACORAL_BASE, add);
	
	add = (void __iomem *)(ipi_base);
	iowrite32(ACORAL_IPI_INT1_CPU3, add);
	return 0;
}

int init_mem(void)
{
	unsigned char *mem_start, *alloc_start;
    share_mem_base = ioremap_nocache(SHARE_MEM_START, SHARE_MEM_SIZE);
    if(!share_mem_base)
        return -1;
	mem_start = (unsigned char *)share_mem_base;
    //set_bit(4, mem_start);
    sharemem_devp->mem = mem_start;//(unsigned char *)(share_mem_base);
    sharemem_devp->ipi_flag = (unsigned int *)(ipi_base);
    //memset(sharemem_devp->mem, 0, 12);  //clean 12 bytes
	InitBuddy(AllList);
    appMemInfo_bitmap = (unsigned long *)mem_start;
    acoral_com_index = (int *)(mem_start + sizeof(int));
    *appMemInfo_bitmap = 0;
    appMemInfo = (App_Mem_Info *)(mem_start + 0x400);

    alloc_start = mem_start + ALLOC_MEM_SHIFT;     //from 1M
    init_Mem_Info();
    AddMemBuddy(AllList, alloc_start, mem_start + SHARE_MEM_SIZE);
    if(AllList[0].first)
        first_blk_phy = AllList[0].first->addr ;//- alloc_start;//ALLOC_MEM_SHIFT;
    //AddMemBuddy(AllList, SHARE_MEM_START+MAX_APP_MEM*sizeof(App_Mem_Info), SHARE_MEM_START + SHARE_MEM_SIZE);
   // PrintMem(AllList);
    return 0;
}

/*open file function*/
int sharemem_open(struct inode *inode, struct file *filp)
{
  /*将设备结构体指针赋给文件私有数据指针*/
  struct sharemem_dev *dev;
  
  dev = container_of(inode->i_cdev,struct sharemem_dev,cdev);  
  filp->private_data = dev;  
  return 0;
}
/*release function*/
int sharemem_release(struct inode *inode, struct file *filp)
{
  return 0;
}

/* ioctl function*/
static int sharemem_ioctl(struct inode *inodep, struct file *filp, unsigned
  int cmd, unsigned long arg)
{
    struct sharemem_dev *dev = filp->private_data;/*get device pointor*/
     App_Info appInfo;
    void __user *argp;
  switch (cmd)
  {
    case MEM_CLEAR:
      memset(dev->mem, 0, SHARE_MEM_SIZE);      
      printk(KERN_INFO "sharemem is set to zero\n");
      break;
    case SEND_IPI:
      *dev->ipi_flag = ACORAL_IPI_INT3_APP; 
      printk(KERN_INFO "send ipi to acoral\n");
      break;
	case ACORAL_LINK_APP:

      argp = (void __user *)arg;
      if(copy_from_user(&appInfo, (App_Info *)argp, sizeof(App_Info)))
          return -EFAULT;
	 return (int)acoral_link_app(appInfo);
    default:
      return  - EINVAL;
  }
  return 0;
}

/*read function*/
static ssize_t sharemem_read(struct file *filp, char __user *buf, size_t size,
  loff_t *ppos)
{
  unsigned long p =  *ppos;
  unsigned int count = size;
  int ret = 0;
  struct sharemem_dev *dev = filp->private_data; /*get device pointor*/
    
  printk("count = %d\n", count);
  /*analyse and get the valid length*/
  if (p >= SHARE_MEM_SIZE)
    return count ?  - ENXIO: 0;
  if (count > SHARE_MEM_SIZE - p)
    count = SHARE_MEM_SIZE - p;

  printk("count = %d\n", count);
  /*kernel space -> user space*/
  if (copy_to_user(buf, (void*)(dev->mem + p), count))
  {
    ret =  - EFAULT;
  }
  else
  {
    *ppos += count;
    ret = count;
    
    printk(KERN_INFO "read %d bytes(s) from %ld\n", count, p);
  }

  return ret;
}

/*write function*/
static ssize_t sharemem_write(struct file *filp, const char __user *buf,
  size_t size, loff_t *ppos)
{
  unsigned long p =  *ppos;
  unsigned int count = size;
  int ret = 0;
  struct sharemem_dev *dev = filp->private_data; /*get device pointor*/
  
  /*analyse and get the valid length*/
  if (p >= SHARE_MEM_SIZE)
    return count ?  - ENXIO: 0;
  if (count > SHARE_MEM_SIZE - p)
    count = SHARE_MEM_SIZE - p;
    
  /*user space -> kernel space*/
  if (copy_from_user(dev->mem + p, buf, count))
    ret =  - EFAULT;
  else
  {
    *ppos += count;
    ret = count;
    
    printk(KERN_INFO "written %d bytes(s) from %ld\n", count, p);
  }

  return ret;
}

/* seek file function */
static loff_t sharemem_llseek(struct file *filp, loff_t offset, int orig)
{
  loff_t ret = 0;
  switch (orig)
  {
    case 0:   /*according to the start location of file*/
      if (offset < 0)
      {
        ret =  - EINVAL;
        break;
      }
      if ((unsigned int)offset > SHARE_MEM_SIZE)
      {
        ret =  - EINVAL;
        break;
      }
      filp->f_pos = (unsigned int)offset;
      ret = filp->f_pos;
      break;
    case 1:   /*according to the current location of file*/
      if ((filp->f_pos + offset) > SHARE_MEM_SIZE)
      {
        ret =  - EINVAL;
        break;
      }
      if ((filp->f_pos + offset) < 0)
      {
        ret =  - EINVAL;
        break;
      }
      filp->f_pos += offset;
      ret = filp->f_pos;
      break;
    default:
      ret =  - EINVAL;
      break;
  }
  return ret;
}

/*file operation construction*/
static const struct file_operations sharemem_fops =
{
  .owner = THIS_MODULE,
  .llseek = sharemem_llseek,
  .read = sharemem_read,
  .write = sharemem_write,
  .ioctl = sharemem_ioctl,
  .open = sharemem_open,
  .release = sharemem_release,
};

/*initalize and register the cdev*/
static void sharemem_setup_cdev(struct sharemem_dev *dev, int index)
{
  int err, devno = MKDEV(sharemem_major, index);

  cdev_init(&dev->cdev, &sharemem_fops);
  dev->cdev.owner = THIS_MODULE;
  dev->cdev.ops = &sharemem_fops;
  err = cdev_add(&dev->cdev, devno, 1);
  if (err)
    printk(KERN_NOTICE "Error %d adding LED%d", err, index);
}

/*device driver module loader function*/
int sharemem_init(void)
{
  int result;
  dev_t devno = MKDEV(sharemem_major, 0);


  printk("init_mem ok\n");

  /*start acoral*/
  if(startAcoral() < 0)
  {
      printk("start acoral error\n");
      return -1;
  }

  printk("startAcoral ok\n");
  /* apply for device number*/
  if (sharemem_major)
    result = register_chrdev_region(devno, 1, "sharemem");
  else  /* dynamic apply*/
  {
    result = alloc_chrdev_region(&devno, 0, 1, "sharemem");
    sharemem_major = MAJOR(devno);
  }  
  if (result < 0)
    return result;

  printk("region ok\n");
    
  /*dynamic apply for the memory of device construction*/
  sharemem_devp = kmalloc(sizeof(struct sharemem_dev), GFP_KERNEL);
  if (!sharemem_devp)    /*application failed*/
  {
    result =  - ENOMEM;
    goto fail_malloc;
  }
  printk("kmalloc ok\n");
  memset(sharemem_devp, 0, sizeof(struct sharemem_dev)); 
  if(init_mem() < 0)
  {
      printk("init_mem error\n");
      return -1;
  }
  sharemem_setup_cdev(sharemem_devp, 0);
  return 0;

  fail_malloc: unregister_chrdev_region(devno, 1);
  return result;
}

/*module unload function*/
void sharemem_exit(void)
{
  cdev_del(&(sharemem_devp->cdev));  /*logout cdev*/   
  
  kfree(sharemem_devp);     /*release the memory of device construction*/
  unregister_chrdev_region(MKDEV(sharemem_major, 0), 1); /*release the device number*/

  iounmap(acoral_base);
  iounmap(flags_base);
  iounmap(ipi_base);
  iounmap(share_mem_base);

  printk("exit acoral_link\n");
}

MODULE_AUTHOR("Song Baohua");
//MODULE_LICENSE("Dual BSD/GPL");

MODULE_LICENSE("GPL");

module_param(sharemem_major, int, S_IRUGO);

module_init(sharemem_init);
module_exit(sharemem_exit);
