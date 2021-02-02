/*
    多模块编成的模块2程序,此文件中的函数调用了模块1中定义的函数   
*/
#include <linux/kernel.h>
#include <linux/module.h>
extern void module_function1(void);          /*   模块1中定义的函数1声明   */
extern void module_function2(void);
static int __init module_drive2_init(void)
{
	module_function1();
	printk(KERN_INFO"module2:module_drive2_init\n");
	return 0;
}
static void __exit module_drive2_exit(void)
{
	module_function2();
	printk(KERN_INFO"module2:module_drive2_exit\n");
}
module_init(module_drive2_init);
module_exit(module_drive2_exit);
MODULE_LICENSE("GPL");

