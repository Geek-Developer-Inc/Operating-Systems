#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/io.h>

unsigned int *LED_BaseAddress=NULL;
unsigned int *BEEP_BaseAddress = NULL;
unsigned int *KEY_BaseAddress = NULL;
/*    LED灯寄存器地址定义   */
#define GPM4CON (*(volatile unsigned int*)LED_BaseAddress)
#define GPM4DAT (*(volatile unsigned int*)(LED_BaseAddress+1)) 
/*    蜂鸣器寄存器地址定义   */
#define GPD0CON (*(volatile unsigned int*)BEEP_BaseAddress)
#define GPD0DAT (*(volatile unsigned int*)(BEEP_BaseAddress+1)) 
/*    按键寄存器地址定义     */
#define GPX3CON *((volatile unsigned int *)(KEY_BaseAddress))
#define GPX3PUD *((volatile unsigned int *)(KEY_BaseAddress+2)) 
#define GPX3DAT *((volatile unsigned int *)(KEY_BaseAddress+1))

#define LED1(x) x?(GPM4DAT&=~(0x01<<0)):(GPM4DAT|=0x01<<0)
#define LED2(x) x?(GPM4DAT&=~(0x01<<1)):(GPM4DAT|=0x01<<1)
#define LED3(x) x?(GPM4DAT&=~(0x01<<2)):(GPM4DAT|=0x01<<2)
#define LED4(x) x?(GPM4DAT&=~(0x01<<3)):(GPM4DAT|=0x01<<3)
#define BEEP(x) x?(GPD0DAT|=0x01<<0):(GPD0DAT&=~(0x01<<0))

int device_open(struct inode* inode, struct file* fp)
{
	printk("device drive open sucees!\n");
	return 0;
}
ssize_t device_read(struct file* fp, char __user* buf, size_t size, loff_t* offset)
{
	unsigned long ret = 0;
	unsigned char key_value = 0;
	if ((GPX3DAT & (0x01 << 2)) == 0)
	{
		while (!(GPX3DAT & (0x01 << 2)));
		key_value = 1;
	}
	if ((GPX3DAT & (0x01 << 3)) == 0)
	{
		while (!(GPX3DAT & (0x01 << 3)));
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
	ret = copy_to_user(buf, &key_value, 1);
	if (ret < 0)
	{
		printk("copy to user key_value failure!\n");
		return 0;
	}
	key_value = 0;
	return 0;
}
ssize_t device_write(struct file* fp, const char __user* buf, size_t size, loff_t* offset)
{
	int ret=0;
	unsigned char led_value=0;
	printk("buf = %s\r\n", buf);
	ret = copy_from_user(&led_value, buf, 1);/*   从应用层空间把数据复制到内核空间     */
	if (ret < 0)
	{
		printk("copy form user failure\n");
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
	
	return 0;
}
int device_close(struct inode* inode, struct file* fp)
{
	LED1(0);
	LED2(0);
	LED3(0);
	LED4(0);
	BEEP(0);
	return 0;
}
long device_unlocked_ioctl(struct file* fp, unsigned int cmd, unsigned long arg)
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
static struct file_operations Device_fops =
{
	.owner = THIS_MODULE,
	.open = device_open,
	.read = device_read,
	.write = device_write,
	.release = device_close,
	.unlocked_ioctl=device_unlocked_ioctl,
};
static struct miscdevice misc =
{
	.minor = 255,
	.name = "DeviceDrive",
	.fops = &Device_fops,
};
static int __init DeviceDrive_Init(void)
{
	int ret = 0;
	ret = misc_register(&misc);
	if (ret < 0)
	{
		printk("misc register failure!\n");    /*    向内核注册一个杂项字符设备    */
		return -1;
	}
	printk("misc register sucess!\n");
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
	return 0;
}
static void __exit DeviceDrive_Enit(void)
{
	iounmap(LED_BaseAddress);
	iounmap(BEEP_BaseAddress);
	iounmap(KEY_BaseAddress);
	misc_deregister(&misc);
}
module_init(DeviceDrive_Init);
module_exit(DeviceDrive_Enit);
MODULE_LICENSE("GPL");
