#include <linux/module.h> 
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
static dev_t dev;       /*   存放分配到的第一个设备(包括主次设备号)                     */
static struct cdev cdev;/*   核心结构体                */
static struct class *DeviceClass=NULL;
static struct device *LED_Device=NULL;
unsigned int *LedBassAddr=NULL;
#define GPM4CON (*(volatile unsigned int*)LedBassAddr)
#define GPM4DAT (*(volatile unsigned int*)(LedBassAddr+1))
char *name[4]={"LED1","LED2","LED3","LED4"};/*  字符指针数组:设备节点名称     */
int DriverLed_open(struct inode *inode, struct file *fp)
{
   printk("DriverLed_open OK\n");
   GPM4DAT &=~((0x1 << 0)|(0x1 << 1)|(0x1 << 2)|(0x1 << 3));
   return 0;
}

ssize_t DriverLed_read(struct file *fp, char __user *buf, size_t size, loff_t *offset)
{
    printk("DriverLed_read OK\n");
    return 0; 
}
ssize_t DriverLed_writer(struct file *fp, const char __user *buf, size_t size, loff_t *offset)
{
    printk("DriverLed_writer OK\n");
    return 0; 
}
int DriverLed_ralease(struct inode *inode, struct file *fp)
{
    printk("DriverLed_ralease OK\n");
	GPM4DAT |=((0x1 << 0)|(0x1 << 1)|(0x1 << 2)|(0x1 << 3));
    return 0;     
}

static struct file_operations Driver_LED_fops=
{
	 .open=DriverLed_open,
	 .read=DriverLed_read,
	 .write=DriverLed_writer,
	 .release=DriverLed_ralease,    
};
static int __init DriverLed_Init(void)
{
    int ret;
    LedBassAddr= (unsigned int *)ioremap(0x110002E0,0x18);   /* 映射地址  */
	//IO口初始化
    GPM4CON &= ~((0xf << 3 * 4) | (0xf << 2 * 4) | (0xf << 1 * 4) | (0xf << 0 * 4));
	GPM4CON |= ((0x1 << 3 * 4) | (0x1 << 2 * 4) | (0x1 << 1 * 4) | (0x1 << 0 * 4));
	GPM4DAT |=((0x1 << 0)|(0x1 << 1)|(0x1 << 2)|(0x1 << 3));
	ret=alloc_chrdev_region(&dev, 1, 1, "DriverLed");   /*     动态申请一个设备号范围                */
	if(ret<0)
	{
	    printk("alloc chardev region defeat\n");
		return -1;
	}
	printk("alloc chardev region success\n");
	printk("major=%d\n",MAJOR(dev));           /*   打印主设备号         */
	printk("minor=%d\n",MINOR(dev));           /*   打印次设备号         */ 
    DeviceClass=class_create(THIS_MODULE,"DriverLed");       /*   创建一个类        */
    LED_Device=device_create(DeviceClass, NULL,dev, NULL, "my_device");
	cdev_init(&cdev, &Driver_LED_fops);    /*   初始化核心结构，具体做的是清零核心结构，初始化核心结构的list,kobj,ops成员        */
	cdev_add(&cdev,dev,1);           /*   注册一个设备         */
	return 0;	
}
static void __exit DriverLed_Exit(void) 
{
	 iounmap(LedBassAddr);           /* 取消映射地址    */
     device_destroy(DeviceClass,dev);  /* 摧毁设备节点      */
	 class_destroy(DeviceClass);   /* 摧毁类   */
     cdev_del(&cdev);     /*    注销设备        */
	 unregister_chrdev_region(dev,1);    /*   释放设备号        */
	 printk("DriverLed_Exit oK\n");
}
module_init(DriverLed_Init);
module_exit(DriverLed_Exit);
MODULE_LICENSE("GPL");



