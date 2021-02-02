/**********************************************************************************************/
/**********************************************************************************************/
/**                       Linux2.6方式注册字符设备节点，静态申请设备号                       **/
/**                       读取按键的键值      虚拟地址映射    多点驱动                       **/
/**                       设备节点名称：KeyDevice                                            **/
/**********************************************************************************************/
/**********************************************************************************************/

/*********************************************/
/*                  头文件区                 */
/*********************************************/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/io.h>
/*********************************************/
/*                定义寄存器地址             */
/*********************************************/
unsigned int* BaseAddress = NULL;
#define GPX3CON *((volatile unsigned int *)(BaseAddress))
#define GPX3PUD *((volatile unsigned int *)(BaseAddress+2)) 
#define GPX3DAT *((volatile unsigned int *)(BaseAddress+1))
/*********************************************/
/*                  变量定义区               */
/*********************************************/
static dev_t dev;                                  /*      存放第一个分配到的设备的设备号(主设备号+次设备号)    */
static unsigned int major = 255;                   /*      主设备号        指定静态申请设备号的主设备号         */
static unsigned int minor = 0;                     /*      次设备号        指定静态申请设备号的次设备号         */
static struct cdev cdev;                           /*      需要初始化的核心结构指针                             */
static struct class* Keydrive_class = NULL;        /*      设备的类        类指针                               */
static struct device* Key_device = NULL;           /*      存放设备节点自动创建函数的返回值                     */

/*********************************************/
/*                  文件操作指针函数集       */
/*********************************************/
int key_open(struct inode* inode, struct file* fp)
{
	printk("Key open sucess!\n");
	return 0;
}
ssize_t key_read(struct file* fp, char __user* buf, size_t size_t, loff_t* offset)
{
	static unsigned char key_value = 0;
	if (!(GPX3DAT & (1 << 2)))
	{
		while (!(GPX3DAT & (1 << 2)));
		key_value = 1;
		printk("Key %d\n", key_value);
	}
	if (!(GPX3DAT & (1 << 3)))
	{
		while (!(GPX3DAT & (1 << 3)));
		key_value = 2;
		printk("Key %d\n", key_value);
	}
	if (!(GPX3DAT & (1 << 4)))
	{
		while (!(GPX3DAT & (1 << 4)));
		key_value = 3;
		printk("Key %d\n", key_value);
	}
	if (!(GPX3DAT & (1 << 5)))
	{
		while (!(GPX3DAT & (1 << 5)));
		key_value = 4;
		printk("Key %d\n", key_value);
	}
	printk("Key read sucess!\n");
	return 0;
}
ssize_t key_write(struct file* fp, const char __user* buf, size_t size, loff_t* offset)
{
	printk("Key write sucess!\n");
	return 0;
}
int key_release(struct inode* inode, struct file* fp)
{
	printk("Key close sucess!\n");
	return 0;
}
static struct file_operations key_fops =
{
	.open = key_open,
	.read = key_read,
	.write = key_write,
	.release = key_release,     /*    相当于close      */
};
char* name[4] = { "KEY1","KEY2","KEY3","KEY4" };/*    字符指针数组   设备节点名称     */
/**********************************************************************************************************************************************/
/**                                         按键与寄存器地址对应关系                                                                         **/
/**                             GPX3CON    GPX3_2，GPX3_3，GPX3_4，GPX3_5                                                                    **/
/**			                    GPX3PUD    GPX3_4，GPX3_6，GPX3_8，GPX3_10                                                                   **/
/**		                        GPX3DAT    GPX3_2，GPX3_3，GPX3_4，GPX3_5                                                                    **/
/**                    GPX3CON配置为输入模式， GPX3PUD上拉，GPX3DAT读取寄存器的相应位电平                                                    **/
/**********************************************************************************************************************************************/
static int __init KeyDeviceDrive_Init(void)
{
	int i = 0,ret=0;
	BaseAddress=(unsigned int*)ioremap(0x11000C60, 0x0C);
	GPX3CON &= ~((0x0f << 2 * 4) | (0x0f << 3 * 4) | (0x0f << 4 * 4) | (0x0f << 5 * 4));         /*     输入模式    */
	GPX3PUD &= ~((0x03 << 2 * 2) | (0x03 << 3 * 2) | (0x03 << 4 * 2) | (0x03 << 5 * 2));
	GPX3PUD |= (0x03 << 2 * 2) | (0x03 << 3 * 2) | (0x03 << 4 * 2) | (0x03 << 5 * 2);            /*     上拉模式    */
	dev = MKDEV(major, minor);
	ret = register_chrdev_region(dev, 4, "KeyDevice");
	if (ret < 0)
	{
		printk("misc register failure!\n");
		return -1;
	}
	printk("misc register success!\n");
	Keydrive_class = class_create(THIS_MODULE, "KeyDevice");       /*   创建一个类        */
	for (i = 0; i < 4; i++)
	{
		Key_device = device_create(Keydrive_class, NULL, dev+i, NULL, name[i]);
	}
	cdev_init(&cdev, &key_fops);
	cdev_add(&cdev, dev, 4);
	return 0;
}
static void __exit  KeyDeviceDrive_Exit(void)
{
	int j = 0;
	iounmap(BaseAddress);
	for (j = 0; j < 4; j++)
	{
		device_destroy(Keydrive_class, dev + j);
	}
	class_destroy(Keydrive_class);
	cdev_del(&cdev);                     /*      注销设备       */
	unregister_chrdev_region(dev, 0);    /*      释放设备号     */
	printk("DriverLed_Exit oK\n");
}
module_init(KeyDeviceDrive_Init);
module_exit(KeyDeviceDrive_Exit);
MODULE_LICENSE("GPL");
