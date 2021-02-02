KERN_DIR = /home/work/linux-3.5/
obj-m += passfunction.o
all:
	make -C $(KERN_DIR) M=`pwd` modules
clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers *.markers *.order


