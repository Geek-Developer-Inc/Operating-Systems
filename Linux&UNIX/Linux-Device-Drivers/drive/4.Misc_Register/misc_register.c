#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
int example_open(struct inode *inode,struct file *fp)
{
    printk("example open\n");
    return 0;
}
ssize_t example_read(struct file *fp,char _user *buf,size_t size,loff_t *offset)
{
    printk("example read\n");
    return 0;
}
ssize_t example_write(struct file *fp,const char _user *buf,size_t size,loff_t *offset)
{
     printk("example write\n");
     return 0;
}
int example_close(struct inode *inode,struct file *fp)
{
     printk("example close\n");
     return 0;
}
const struct file_operation example_fops={
     .owner=THIS_MODULE,
     .open=example_open,
     .read=example_read,
     .write=example_write,
     .release=example_close,
};
struct miscdevice  misc = {
	.minor = 255,               /*   可以直接指定，一般为255表示系统自动分配   */
	.name = "example",
	.fops = &example_fops,
};

/*

struct miscdevice  {
	int minor;
	const char *name;
	const struct file_operations *fops;
	struct list_head list;
	struct device *parent;
	struct device *this_device;
	const char *nodename;
	umode_t mode;
};

*/

static _init int misc_register_init(void)
{
    misc_register(&misc);
    printk("misc_register_init\n");
    return 0;
} 
static _exit void misc_register_exit(void)
{
    misc_deregister(&misc);
    printk("misc_register_exit\n");
} 
module_init(misc_register_init);
module_exit(misc_register_exit);
MODULE_LICENSE("GPL");

