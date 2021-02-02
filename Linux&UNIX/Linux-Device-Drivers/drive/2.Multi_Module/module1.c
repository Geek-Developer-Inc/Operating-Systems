/*
     多模块编成示例程序，模块1内定义两个函数
     加载时先加载模块1程序，卸载时先卸载模块2程序  
*/

#include <linux/kernel.h>
#include <linux/module.h>
void module_function1(void)
{
	printk("module1:module_function1\n");
}
void module_function2(void)
{
	printk("module1:module_function2\n");
}
static __init int module_drive_init(void)
{
	printk("module1:this is module_drive_init\n");
	return 0;
}
static __exit void module_drive_exit(void)
{
	printk("module1:this is module_drive_exit\n");
}
module_init(module_drive_init);
module_exit(module_drive_exit);
EXPORT_SYMBOL(module_function1);    /*   模块内的函数如果要提供给其他模块使用，必须使用EXPORT_SYMBOL()导出    */
EXPORT_SYMBOL(module_function2);
MODULE_LICENSE("GPL");




