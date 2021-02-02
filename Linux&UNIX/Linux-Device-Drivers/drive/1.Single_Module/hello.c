#include <linux/kernel.h>          /*      模块相关函数的头文件                */
#include <linux/module.h>          /*      printk函数的声明头文件             */
static int __init hello_init(void)    /*   _init 修饰函数，表示该函数仅做初始化工作，执行完成后，占用的内存就会被释放        */    
{
	printk("hello_init is call\n");
	printk("jia zai han shu chu shi hua\n");
	return 0;
}
static void __exit hello_exit(void)   /*   __exit：修饰函数，表示该函数仅做初始化工作，执行完成后，占用的内存就会被释放声明    */
{
	printk("hello_module is end\n");
	printk("xie zai han shu chu shi hua\n");
}
module_init(hello_init);            /*        加载函数声明            */
module_exit(hello_exit);            /*        卸载函数声明            */
MODULE_LICENSE("GPL");              /*        开源协议声明            */
/*
        在虚拟机终端当前目录下输入：make
        在串口助手终端的相同目录下输入：insmod hello.ko
        在串口助手终端的输入:lsmod 可查看加载状态
        在串口助手终端的输入：rmmod hello.ko可卸载该模块         
*/


