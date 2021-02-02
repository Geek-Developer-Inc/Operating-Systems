#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/io.h>
unsigned int  *BaseAddr= NULL;
#define GPM4CON 	*((volatile unsigned int *)(BaseAddr))
#define GPM4DAT 	*((volatile unsigned int *)(BaseAddr +1))
int misc_register_open (struct inode *inode, struct file *fp)
{
	printk("misc_register open is ok\n");
	GPM4DAT &=~(0XF<<0);
	return 0;
}
ssize_t misc_register_read(struct file *fp, char __user *buf, size_t size, loff_t *offset)
{
    printk("DriverLed_read OK\n");
    return 0; 
}
ssize_t misc_register_writer(struct file *fp, const char __user *buf, size_t size, loff_t *offset)
{
    printk("DriverLed_writer OK\n");
    return 0; 
}
int  misc_register_close(struct inode *inode, struct file *fp)
{
	printk("misc_register close is ok\n");
	GPM4DAT |=(0XF<<0);
	return 0;
}
struct file_operations misc_register_fops = {
	.owner = THIS_MODULE,
	.open = misc_register_open,                    /*   模块引用，任何时候都赋值THIS_MODULE       */ 
	.read = misc_register_read,
	.write = misc_register_writer,
	.release = misc_register_close,
};
struct miscdevice  misc = {
	.minor = 255,                            /*    可以直接指定，一般为255表示系统自动分配    */
	.name = "misc_register",
	.fops = &misc_register_fops,
};
static int __init misc_register_device_init(void)
{
	int ret;
	ret = misc_register(&misc);
	if(ret <0)
	{
		printk("misc register error!\n");
		return -1;
	}
	printk("misc register sucess!\n");
	BaseAddr=(unsigned int  *) ioremap(0x110002E0, 0x08);
	GPM4CON &= ~((0x0f << 3 * 4) | (0x0f << 2 * 4) | (0x0f << 1 * 4) | (0x0f << 0 * 4));
	GPM4CON |= ((0x01 << 3 * 4) | (0x01 << 2 * 4) | (0x01 << 1 * 4) | (0x01 << 0 * 4));
	GPM4DAT |=((0x01 << 0)|(0x01 << 1)|(0x01 << 2)|(0x01 << 3));
	return 0;
}
static void __exit misc_register_device_exit(void)
{
	iounmap(BaseAddr);
	misc_deregister(&misc);
	printk("misc register exit\n");
}
module_init(misc_register_device_init);
module_exit(misc_register_device_exit);
MODULE_LICENSE("GPL");

