/*     早期经典方式注册字符设备方法，模拟在/dev/目录下自动创建设备文件     */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
static int major = 0;                                         /*       用于接收主设备号      */
static struct class* DeviceClass = NULL;
static struct device* Way_Device = NULL;
int Classic_open(struct inode* inode, struct file* fp)
{
	printk("hello_open ok\n");
	return 0;
}
ssize_t Classic_read(struct file* fp, char __user* buf, size_t size, loff_t* offset)
{
	printk("hello_read ok\n");
	return 0;

}
ssize_t Classic_write(struct file* fp, const char __user* buf, size_t size, loff_t* offset)
{
	printk("hello_write ok\n");
	return 0;

}
int Classic_close(struct inode* inode, struct file* fp)
{
	printk("hello_close ok\n");
	return 0;

}
struct file_operations Classic_fops =
{
   .owner = THIS_MODULE,
   .open = Classic_open,
   .read = Classic_read,
   .write = Classic_write,
   .release = Classic_close,
};
static int __init Classic_register_init(void)
{
	major = register_chrdev(0, "Classic_Chrdev", &Classic_fops);     /*     注册一个早期经典标准字符设备；0表示由内核自动分配一个可以用的主设备号；设备名       */
	if (major < 0)
	{
		printk("register chrdev failure\n");
		return -1;
	}
	major=MKDEV(major, 0);                                                 /*      已知主次设备号合成完整设备号     */
	printk("register chrdev ok\n");
	printk("major = %d\n", major);
	DeviceClass = class_create(THIS_MODULE, "DriverClassic");
	Way_Device = device_create(DeviceClass, NULL, major, NULL, "Classic_device");
	return 0;
}
static void __exit Classic_registet_exit(void)
{
	device_destroy(DeviceClass, major);    /*      摧毁设备节点   */
	class_destroy(DeviceClass);            /*      摧毁类         */
	unregister_chrdev(major, "Classic_Chrdev");                      /*        注销一个已经存在标准字符设备         */
}
module_init(Classic_register_init);
module_exit(Classic_registet_exit);
MODULE_LICENSE("GPL");






