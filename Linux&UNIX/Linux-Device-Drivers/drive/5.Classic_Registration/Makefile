KERN_DIR = /home/work/linux-3.5
all:
	make -C $(KERN_DIR) M=`pwd` modules
	arm-linux-gcc app.c -o app
	#cp hello *.ko /xyd/rootfs/work
clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers *.markers *.order app
obj-m := registration.o

