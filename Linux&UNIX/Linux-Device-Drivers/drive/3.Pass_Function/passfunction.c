/*       模块传参函数：加载模块时动态传递参数     */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/stat.h>
static int par = 8888;
static char *str = "Are Yuo OK";
static int arr[10] = {0,1,2,3,4};
static int num;
static int __init transfer_init(void)
{
	int i;
	printk("transfer_init\n");
	printk("par = %d\n",par);
	printk("str = %s\n",str);
	for(i=0;i<10;i++)
	{
		printk("%4d",arr[i]);
	}
	printk("\n");
	printk("num = %d\n",num);
	return 0;
}
static void __exit transfer_exit(void)
{
	printk("transfer_exit\n");
}
module_param(par,int,S_IRUGO|S_IWUSR);   /*    传递普通变量参数的函数：par要传递的参数，int为par的类型，S_IRUGO|S_IWUSR为权限    */
module_param(str,charp,S_IRUGO);
module_param_array(arr,int,&num,S_IRUGO);/*    传递数组变量参数的函数：arr要传递的数组，num为传入参数的元素个数，S_IRUGO为权限    */
module_init(transfer_init);
module_exit(transfer_exit);
MODULE_LICENSE("GPL");
/*
       传递数组时数组元素之间只需要用，隔开 
*/

