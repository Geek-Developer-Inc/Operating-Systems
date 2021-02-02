/*      linux2.6注册字符设备     动态申请设备号    调用自动添加设备节点函数    函数接口       */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/interrupt.h>   /*    中断           */
#include <linux/uaccess.h>
#include <linux/kdev_t.h>
#include <linux/sched.h>
#include <linux/wait.h>        /*    等待消息队列   */
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/irq.h>
#include <linux/fs.h>
#include <linux/io.h>
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
/*    蜂鸣器状态控制宏定义  */
#define BEEP(x) x?(GPD0DAT|=0x01<<0):(GPD0DAT&=~(0x01<<0))
static dev_t dev;       /*   存放分配到的第一个设备(包括主次设备号)                     */
static struct cdev cdev;/*   核心结构体                */
static struct class* DeviceClass = NULL;
static struct device* LedDriveDevice = NULL;
static struct device* BeepDriveDevice = NULL;
static struct device* KeyDriveDevice = NULL;
/*   按键中断相关参数结构体   */
struct tag
{
	unsigned int irq; /*   中断编号       */
	unsigned int gpio;/*   存放io口编号   */
	char name[10];    /*   指定中断的名字，注册后会出现在/proc/irq/irq号/name文件夹中出现   */
	unsigned char value;/*   按键值保存   */
};
/*      定义4个按键的信息     */
struct tag key[4] = {
		{0,EXYNOS4_GPX3(2),"key1",1},    /*   EXYNOS4_GPX3(2)   IO口管脚编号   */
		{0,EXYNOS4_GPX3(3),"key2",2},
		{0,EXYNOS4_GPX3(4),"key3",3},    /*   EXYNOS4_GPX3(2)   arch/arm/mach-exynos/include GPIO.h    */
		{0,EXYNOS4_GPX3(5),"key4",4}
};
int flage_press = 0;                             /*     按键标志位              */
static DECLARE_WAIT_QUEUE_HEAD(keyirq_wait);     /*     定义并初始化等待队列头，keyirq_wait不需要定义   */
unsigned char key_value = 0;                     /*     保存键值                */
irqreturn_t keyirq_handler(int irq, void* data)  /*     按键中断服务函数          */
{
	struct tag* p = (struct tag*)data;           /*     还原原来的类型    */
	key_value = p->value;                        /*     保存当前按键值    */
	flage_press = 1;                             /*     每次触发中断，则标志位置1   */
	wake_up_interruptible(&keyirq_wait);         /*     唤醒等待队列，取消阻塞      */
	return 0;
}
int device_open(struct inode* inode, struct file* fp)    /*  对应应用层app.c中的read()函数   */
{
	int ret = 0, i = 0, j = 0;
	for (i = 0; i < 4; i++)                              /*  按键属于外部中断，需要获取中断号   */
	{
		key[i].irq = gpio_to_irq(key[i].gpio);           /*  依次循环给4个按键分别获取中断号    */
		/*   中断注册函数  中断编号   中断服务函数   中断属性，触发方式   指定中断的名字    传递给中断服务函数的数据      */
		ret = request_irq(key[i].irq, keyirq_handler, IRQ_TYPE_EDGE_FALLING, key[i].name, (void*)& key[i]);
		if (ret < 0)          /*   如果中断注册失败，依次注销已经注册成功的中断   */
		{
			printk("request key irq failure!\n");
			for (j = i - 1; j >= 0; j--)
			{
				free_irq(key[j].irq, (void*)& key[j]);    /*     从内核中断链表上删除一个中断结构     */
			}
			return 0;
		}
	}
	printk("device drive open sucees!\n");
	return 0;
}
#ifdef DEBUG /*      本程序使用了按键中断，此段在本程序中被屏蔽       */
/*    按键KEY1扫描函数      */
unsigned char KEY1_GetValue(void)
{
	unsigned char KEY1_Value = 0;
	if ((GPX3DAT & (0x01 << 2)) == 0)
	{
		KEY1_Value++;
	}
	else
	{
		if (KEY1_Value > 50 && KEY1_Value < 100)
		{
			KEY1_Value = 0;
			return 1;
		}
		else if (KEY1_Value > 100 && KEY1_Value < 300)
		{
			KEY1_Value = 0;
			return 2;
		}
		else KEY1_Value = 0;
	}
	return 0;
}
/*     按键KEY2扫描函数        */
unsigned char KEY2_GetValue(void)
{
	unsigned char KEY2_Value = 0;
	if ((GPX3DAT & (0x01 << 3)) == 0)
	{
		KEY2_Value++;
	}
	else
	{
		if (KEY2_Value > 50 && KEY2_Value < 100)
		{
			KEY2_Value = 0;
			return 1;
		}
		else if (KEY2_Value > 100 && KEY2_Value < 300)
		{
			KEY2_Value = 0;
			return 2;
		}
		else KEY2_Value = 0;
	}
	return 0;
}
/*     按键KEY3扫描函数         */
unsigned char KEY3_GetValue(void)
{
	unsigned char KEY3_Value = 0;
	if ((GPX3DAT & (0x01 << 4)) == 0)
	{
		KEY3_Value++;
	}
	else
	{
		if (KEY3_Value > 50 && KEY3_Value < 100)
		{
			KEY3_Value = 0;
			return 1;
		}
		else if (KEY3_Value > 100 && KEY3_Value < 300)
		{
			KEY3_Value = 0;
			return 2;
		}
		else KEY3_Value = 0;
	}
	return 0;
}
/*     按键KEY4扫描函数         */
unsigned char KEY4_GetValue(void)
{
	unsigned char KEY4_Value = 0;
	if ((GPX3DAT & (0x01 << 5)) == 0)
	{
		KEY4_Value++;
	}
	else
	{
		if (KEY4_Value > 50 && KEY4_Value < 100)
		{
			KEY4_Value = 0;
			return 1;
		}
		else if (KEY4_Value > 100 && KEY4_Value < 300)
		{
			KEY4_Value = 0;
			return 2;
		}
		else KEY4_Value = 0;
	}
	return 0;
}
#endif
ssize_t device_read(struct file* fp, char __user* buf, size_t size, loff_t* offset)
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
#endif // DEBUG
	if (!flage_press)       /*   如果按键被按下   */
	{
		if (fp->f_flags & O_NONBLOCK)    /*   判断是否是非阻塞模式    */      
		{
			return -1;
		}
		else              /*  如果是阻塞模式，就阻塞等待按键被按下唤醒    */
		{
			wait_event_interruptible(keyirq_wait, flage_press);//等待
		}
	}
	flage_press = 0;
	ret = copy_to_user(buf, &key_value, 1);
	if (ret < 0)
	{
		printk("copy to user key_value failure!\n");
		return 0;
	}
	key_value = 0;
	return 0;
	return 0;
}
ssize_t device_write(struct file* fp, const char __user* buf, size_t size, loff_t* offset)
{
	int ret = 0;
	unsigned char led_value = 0;
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
	int i = 0;
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
	.unlocked_ioctl = device_unlocked_ioctl,
};
/*   杂项设备注测   屏蔽   */
#ifdef DEBUG
static struct miscdevice misc =
{
	.minor = 255,
	.name = "DeviceDrive",
	.fops = &Device_fops,
};
#endif // DEBUG

static int __init DeviceDrive_Init(void)
{
	int ret = 0;
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
	GPX3PUD |= (0x03 << 2 * 2) | (0x03 << 3 * 2) | (0x03 << 4 * 2) | (0x03 << 5 * 2);           /*     上拉模式     */
/*    杂项设备注册    屏蔽    */
#ifdef DEBUG
	ret = misc_register(&misc);
	if (ret < 0)
	{
		printk("misc register failure!\n");    /*    向内核注册一个杂项字符设备    */
		return -1;
	}
	printk("misc register sucess!\n");
#endif // DEBUG
	ret = alloc_chrdev_region(&dev, 1, 3, "DriverDevice");   /*     动态申请三个设备号范围        */
	if (ret < 0)
	{
		printk("alloc chrdev region defeat!\n");
		return 0;
	}
	printk("major=%d\n", MAJOR(dev));      /*      打印主设备号     */
	printk("minor=%d\n", MINOR(dev));      /*      打印次设备号     */
	printk("alloc chrdev region success!\n");
	DeviceClass = class_create(THIS_MODULE, "DriverDevice"); /*   创建一个类   */
	LedDriveDevice = device_create(DeviceClass, NULL, dev, NULL, "led_device");
	BeepDriveDevice = device_create(DeviceClass, NULL, dev + 1, NULL, "beep_device");
	KeyDriveDevice = device_create(DeviceClass, NULL, dev + 2, NULL, "key_device");
	cdev_init(&cdev, &Device_fops);    /*   初始化核心结构，具体做的是清零核心结构，初始化核心结构的list,kobj,ops成员        */
	cdev_add(&cdev, dev, 3);           /*   注册三个设备         */
	return 0;
}
static void __exit DeviceDrive_Enit(void)
{
	int i = 0;
	for (i = 0; i < 4; i++)
	{
		free_irq(key[i].irq, (void*)& key[i]);
	}
	iounmap(LED_BaseAddress);
	iounmap(BEEP_BaseAddress);
	iounmap(KEY_BaseAddress);
	device_destroy(DeviceClass, dev);    /*    摧毁设备节点    */
	device_destroy(DeviceClass, dev + 1);  /*    摧毁设备节点    */
	device_destroy(DeviceClass, dev + 2);  /*    摧毁设备节点    */
	class_destroy(DeviceClass);          /*    摧毁类          */
	cdev_del(&cdev);                     /*    注销设备        */
	unregister_chrdev_region(dev, 3);    /*    释放设备号      */
	printk("DriverLed_Exit oK\n");
	//misc_deregister(&misc);
}
module_init(DeviceDrive_Init);
module_exit(DeviceDrive_Enit);
MODULE_LICENSE("GPL");

