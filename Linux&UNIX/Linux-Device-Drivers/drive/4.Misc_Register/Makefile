# Makefile 2.6 模板
obj-m += misc_register.o

#在使用时kdir要修改为你自己的内核的顶级目录
KDIR:=/home/work/linux-3.5/

all:
	make -C $(KDIR) M=$(PWD) modules
#-C 选项的作用是指将当前工作目录转移到你所指定的位置。
#“M=”选项的作用是，当用户需要以某个内核为基础编译一个外部模块的话，
#需要在make modules 命令中加入“M=dir”，
#程序会自动到你所指定的dir目录中查找模块源码，将其编译，生成KO文件。
clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers *.markers *.order 
#使用make命令编译模块，由于makefile文件无法正确的处理带空格的路径名，
#确保路径没有空格符
#该命令是make modules命令的扩展，-C选项的作用是指将当前的工作目录转移到制定的 目录，即（KDIR）目录，程序到（shellpwd）当前目录查找模块源码，将其编译，生成.ko文件
