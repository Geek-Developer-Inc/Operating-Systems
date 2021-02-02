led.bin: led.o
	arm-linux-ld -Ttext 0x02023400 -o led.elf $^ #将 led.o 链接生成.elf
	arm-linux-objcopy -O binary led.elf led.bin #将 led.elf 转换成 led.bin
#汇编语言文件编译使用
%.o : %.S
	arm-linux-gcc -o $@ $< -c #把.S汇编文件编译生.o  ( -o $@意思就是编译生成的文件用上面的%.o来命名 $<   #指依赖关系中的第一项（%.c）)
#C语言文件编译使用
%.o : %.c
	arm-linux-gcc -o $@ $< -c #把.c文件编译成.o文件
clean:
	rm *.o *.elf *.bin *.dis -f