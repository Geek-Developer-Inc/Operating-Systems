KERN_DIR = /home/work/linux-3.5/
obj-m += Led_linux26.o
all:
	make -C $(KERN_DIR) M=`pwd` modules
	arm-linux-gcc app.c -o app
clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers *.markers *.order app

