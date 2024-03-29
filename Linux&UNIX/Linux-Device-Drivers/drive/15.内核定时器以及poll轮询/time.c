/*     linux2.6注册字符设备模板        定时器      自动创建字符设备文件[led设备]、[key设备]、[beep设备]    */
#include <linux/module.h>     /*    模块相关函数的头文件声明           */
#include <linux/kernel.h>     /*    printk函数的声明头文件             */
#include <linux/fs.h>         /*    关于设备号                         */
#include <linux/cdev.h>       /*    关于设备                           */
#include <linux/device.h>     /*    自动创建设备节点                   */
#include <linux/io.h>         /*    映射地址操作                       */
#include <linux/uaccess.h>    /*    从用户空间把数据复制到内核空间函数 */
#include <linux/interrupt.h>  /*    linux中断                          */
#include <linux/sched.h>      /*    等待队列                           */
#include <linux/gpio.h>       /*    IO口管脚编号                       */
#include <linux/irq.h>        /*    中断号宏定义                       */
#include <linux/wait.h>       /*    初始化等待队列头结构               */
#include <linux/sched.h>      /*    唤醒等待队列                       */
unsigned int* LED_BaseAddress = NULL;
unsigned int* BEEP_BaseAddress = NULL;
unsigned int* KEY_BaseAddress = NULL;
/*    LED灯寄存器地址定义   */
#define GPM4CON (*(volatile unsigned int*)LED_BaseAddress)
#define GPM4DAT (*(volatile unsigned int*)(LED_BaseAddress+1)) 
/*    蜂鸣器寄存器地址定义  */
#define GPD0CON (*(volatile unsigned int*)BEEP_BaseAddress)
#define GPD0DAT (*(volatile unsigned int*)(BEEP_BaseAddress+1)) 
/*    按键寄存器地址定义    */
#define GPX3CON *((volatile unsigned int *)(KEY_BaseAddress))
#define GPX3PUD *((volatile unsigned int *)(KEY_BaseAddress+2)) 
#define GPX3DAT *((volatile unsigned int *)(KEY_BaseAddress+1))
/*    LED状态控制宏定义     */
#define LED1(x) x?(GPM4DAT&=~(0x01<<0)):(GPM4DAT|=0x01<<0)
#define LED2(x) x?(GPM4DAT&=~(0x01<<1)):(GPM4DAT|=0x01<<1)
#define LED3(x) x?(GPM4DAT&=~(0x01<<2)):(GPM4DAT|=0x01<<2)
#define LED4(x) x?(GPM4DAT&=~(0x01<<3)):(GPM4DAT|=0x01<<3)
/*     蜂鸣器状态控制宏定义  */
#define BEEP(x) x?(GPD0DAT|=0x01<<0):(GPD0DAT&=~(0x01<<0))

static dev_t dev;                           /*    第一个设备号(包含了主和次)                 */
static struct cdev cdev;                    /*    定义一个cdev变量,struct cdev 数据机构体    */
static struct class* dev_class;             /*    接收创建一个类的函数的返回值               */
static struct device* device_p;             /*    接收创建一个设备节点的函数的返回值         */
char* device_name[3] = { "LedDrive","KeyDrive","BeepDrive"};/*    字符指针数组   存放设备名  */
struct tag
{
	unsigned int irq;
	unsigned int gpio;
	char name[10];
	unsigned char value;
};
struct tag key[4] = {
		{0,EXYNOS4_GPX3(2),"key1",1},
		{0,EXYNOS4_GPX3(3),"key2",2},
		{0,EXYNOS4_GPX3(4),"key3",3},
		{0,EXYNOS4_GPX3(5),"key4",4}
};
struct timer_list timer;      /*     定义定时器结构体timer_list     */
unsigned char key_value = 0;  /*     全局变量保存按键值             */
unsigned char flage_press = 0;/*     按键按下标志位                 */
static DECLARE_WAIT_QUEUE_HEAD(key_wait); /*     定义并且初始化等待队列头结构    */
irqreturn_t  key_interrupt(int irq, void* data)
{
	struct tag* p = (struct tag*)data;
	key_value = p->value;
	mod_timer(&timer, jiffies + msecs_to_jiffies(10));   /*    修改激活定时器    */
	return 0;
}
void KeyTimer_Function(unsigned long data)               /*    定时器处理函数    */
{
	flage_press = 1;
	wake_up_interruptible(&key_wait);                    /*    唤醒队列等待      */
}
/*   设备文件操作函数集定义   */
int devicedrive_open(struct inode* inode, struct file* fp)
{
	int ret = 0, i = 0, j = 0;
	printk("device drive open success!\n");
	for (i = 0; i < 4; i++)
	{
		key[i].irq = gpio_to_irq(key[i].gpio);
		ret = request_irq(key[i].irq, key_interrupt, IRQ_TYPE_EDGE_FALLING, key[i].name, (void*)& key[i]);
		if (ret < 0)
		{
			printk("request key irq failure!\n");
			for (j = i - 1; j > 0; j--)
			{
				free_irq(key[j].irq, (void*)& key[j]);
			}
			return 0;
		}
	}
	return 0;
}
ssize_t devicedrive_read(struct file* fp, char __user* buff, size_t size, loff_t* offset)
{
	unsigned long ret = 0;
#ifdef DEBUG
	unsigned char key_value = 0;
	if ((GPX3DAT & (0x01 << 2)) == 0)
	{
		while (!(GPX3DAT & (0x01 << 2)));
		key_value = 1;
	}
	if ((GPX3DAT & (0x01 << 3)) == 0)
	{
		while ((GPX3DAT & (0x01 << 3)));
		key_value = 2;
	}
	if ((GPX3DAT & (0x01 << 4)) == 0)
	{
		while (!(GPX3DAT & (0x01 << 4)));
		key_value = 3;
	}
	if ((GPX3DAT & (0x01 << 5)) == 0)
	{
		while (!(GPX3DAT & (0x01 << 5)));
		key_value = 4;
	}
#endif // DEBUG
	if (!flage_press)
	{
		if (fp->f_flags & O_NONBLOCK)
		{
			return -1;
		}
		else
		{
			wait_event_interruptible(key_wait, flage_press);/*     等待    */
		}
	}
	ret = copy_to_user(buff, &key_value, 1);
	if (ret < 0)
	{
		printk("copy_to_user fail!!!\n");
		return 0;
	}
	key_value = 0;
	flage_press = 0;
	printk("device drive read success!\n");
	return 0;
}
ssize_t devicedrive_write(struct file* fp, const char __user* buf, size_t size, loff_t* offset)
{
	int ret = 0;
	unsigned char led_value = 0;
	printk("buf = %s\n", buf);
	ret = copy_from_user(&led_value, buf, 1);/*   从应用层空间把数据复制到内核空间     */
	if (ret < 0)
	{
		printk("copy form user failure!\n");
		return 0;
	}
	if (led_value == 1)
	{
		LED1(1);
		LED2(0);
		LED3(0);
		LED4(0);
	}
	if (led_value == 2)
	{
		LED2(1);
		LED1(0);
		LED3(0);
		LED4(0);
	}
	if (led_value == 3)
	{
		LED3(1);
		LED1(0);
		LED2(0);
		LED4(0);
	}
	if (led_value == 4)
	{
		LED4(1);
		LED1(0);
		LED2(0);
		LED3(0);
	}
	printk("device drive write success!\n");
	return 0;
}
int devicedrive_close(struct inode* inode, struct file* fp)
{
	int i;
	printk("device drive close success!\n");
	for (i = 0; i < 4; i++)
	{
		free_irq(key[i].irq, (void*)& key[i]);
	}
	LED1(0);
	LED2(0);
	LED3(0);
	LED4(0);
	BEEP(0);
	return 0;
}
long devicedrive_unlocked_ioctl(struct file* fp, unsigned int cmd, unsigned long arg)
{
	if (1 == cmd)
	{
		BEEP(1);
	}
	if (0 == cmd)
	{
		BEEP(0);
	}
	return 0;
}
/*       设备文件操作函数集结构体赋值      */
static struct file_operations devicedrive_fops =
{
	.owner = THIS_MODULE,
	.open = devicedrive_open,      /*    对应应用层的open()函数     */
	.read = devicedrive_read,      /*    对应应用层的read()函数     */
	.write = devicedrive_write,    /*    对应应用层的write()函数    */
	.release = devicedrive_close,  /*    对应应用层的close()函数    */
	.unlocked_ioctl = devicedrive_unlocked_ioctl,    /*     驱动层     ioctl函数接口     */
};
#define baseminor 1     /*     动态申请设备号的次设备号的起始次设备号    */
#define count     3     /*     要申请的连续的次设备号数量                */
static __init int DeviceDrive_Init(void)    /*    加载函数               */
{
	int ret = 0,i=0;
	/*      LED灯寄存器地址映射   */
	LED_BaseAddress = (unsigned int*)ioremap(0x110002E0, 0x08);   /*    LED灯初始化配置    */
	GPM4CON &= ~((0x0f << 3 * 4) | (0x0f << 2 * 4) | (0x0f << 1 * 4) | (0x0f << 0 * 4));    /*   寄存器的相应位初始化(清零)   */
	GPM4CON |= ((0x01 << 3 * 4) | (0x01 << 2 * 4) | (0x01 << 1 * 4) | (0x01 << 0 * 4));     /*  输出模式  */
	GPM4DAT |= ((0x01 << 0) | (0x01 << 1) | (0x01 << 2) | (0x01 << 3));
	/*      蜂鸣器虚拟地址映射    */
	BEEP_BaseAddress = (unsigned int*)ioremap(0x114000A0, 0x08);
	GPD0CON &= ~(0x0f << 0);
	GPD0CON |= 0x01 << 0;
	GPD0DAT &= ~(0x01 << 0);
	/*      按键虚拟地址映射      */
	KEY_BaseAddress = (unsigned int*)ioremap(0x11000C60, 0x0C);
	GPX3CON &= ~((0x0f << 2 * 4) | (0x0f << 3 * 4) | (0x0f << 4 * 4) | (0x0f << 5 * 4));         /*     输入模式    */
	GPX3PUD &= ~((0x03 << 2 * 2) | (0x03 << 3 * 2) | (0x03 << 4 * 2) | (0x03 << 5 * 2));
	GPX3PUD |= (0x03 << 2 * 2) | (0x03 << 3 * 2) | (0x03 << 4 * 2) | (0x03 << 5 * 2);            /*     上拉模式    */
	ret = alloc_chrdev_region(&dev, baseminor, count, "MyDeviceDrive");      /*       动态申请一个设备号范围        */
	if (ret < 0)
	{
		printk("alloc chardev region failure!\n");
		return -1;
	}
	printk("major=%d\n", MAJOR(dev));      /*      打印主设备号     */
	printk("minor=%d\n", MINOR(dev));      /*      打印次设备号     */
	printk("alloc chardev region success!\n");
	/*      调用自动创建设备节点函数      */
	dev_class = class_create(THIS_MODULE, "DeviceDrive");    /*     创建设备类指针                 */
	for (i = 0; i < 3; i++)                                  /*     依次创建三个设备节点           */
	{                                            /*    LedDrive节点,KeyDrive节点,BeepDrive节点     */
		device_p = device_create(dev_class, NULL, dev + i, NULL, device_name[i]);
	}
	cdev_init(&cdev, &devicedrive_fops);         /*     核心结构初始化[cdev] [devicedrive_fops]    */
	cdev_add(&cdev, dev, count);                 /*     设备注册函数    count为3                   */
	printk("device drive init success!\n");
	timer.expires = jiffies + msecs_to_jiffies(10);
	timer.function = KeyTimer_Function;
	init_timer(&timer);
	return 0;
}
static __exit void DeviceDrive_Exit(void)        /*    卸载函数      */
{
	int i;
	/*        取消映射地址  	摧毁节点	 删除类		 分离设备	  删除设备号        */
	iounmap(LED_BaseAddress);
	iounmap(BEEP_BaseAddress);
	iounmap(KEY_BaseAddress);
	for (i = 0; i < 4; i++)
	{
		free_irq(key[i].irq, (void*)& key[i]);
	}
	for (i = 0; i < 3; i++)
	{
		device_destroy(dev_class, dev + i); /*       销毁设备节点       */
	}	
	class_destroy(dev_class);               /*       销毁类             */                   
	cdev_del(&cdev);	                    /*       注销设备           */
	unregister_chrdev_region(dev, count);   /*       注销设备号         */
	printk("device drive exit success!\n");
}
module_init(DeviceDrive_Init);      /*     指定加载函数     */
module_exit(DeviceDrive_Exit);      /*     指定卸载函数     */
MODULE_LICENSE("GPL");              /*     声明开源协议     */
