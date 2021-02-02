/*
 * Copyright (c) 2010 Samsung Electronics Co., Ltd.
 *              http://www.samsung.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//   ./a.out 1.bin 2.bin 14k
 //将1.bin的数读取出来添加和校验写入的2.bin
int main (int argc, char *argv[])
{
	FILE		*fp;
	unsigned char	src;
	char		*Buf, *a;
	int		BufLen;
	int		nbytes, fileLen;
	unsigned int	checksum = 0;
	int		i;

	if (argc != 4)
	{
		//./mkbl2 led.bin bl2.bin 14336
		printf("Usage: mkbl1 <source file> <destination file> <size> \n");
		return -1;
	}

	BufLen = atoi(argv[3]);//是把字符串转换成整型数的一个函数 "14336" -->14336（14*1024=14336）
	Buf = (char *)malloc(BufLen);//为 Buf 开辟空间
	memset(Buf, 0x00, BufLen);//初始化 Buf

	fp = fopen(argv[1], "rb"); //以只读的方式打开 led.bin
	if( fp == NULL)
	{
		printf("source file open error\n");
		free(Buf);
		return -1;
	}

	fseek(fp, 0L, SEEK_END); //移动文件指针到最后
	fileLen = ftell(fp); //读取当前文件指针，得到是文件长度
	fseek(fp, 0L, SEEK_SET);//移动文件指针到开头

	if ( fileLen >  (BufLen-16))//这里是随便写一个数，判断不要大于14K代码
	{
		printf("Usage: unsupported size,size more than 14K \n");
		free(Buf);
		fclose(fp);
		return -1;
	}

	nbytes = fread(Buf, 1, BufLen, fp); //  //读取1.bin到开辟的内存 如果程序代码小于 14K

	

	fclose(fp); //关闭文件
	
	//循环累加前面 14K-4 字节
	for(i = 0;i < (14 * 1024) - 4;i++)
	{
		checksum += (unsigned char)(Buf[i]); //典型的偶校验
	}
	*(unsigned int*)(Buf+i) = checksum; //校验 将得到和校验存放到内存末尾

	fp = fopen(argv[2], "wb");  //打开一个用于写入的二进制文件 bl2.bin 
	if (fp == NULL)
	{
		printf("destination file open error\n");
		free(Buf);
		return -1;
	}

	a = Buf;
	nbytes	= fwrite( a, 1, BufLen, fp);//将led.bin文件里边的数据以及效验位写入到bl2.bin

	if ( nbytes != BufLen )//判断写入的长度是否和buflen相等
	{
		printf("destination file write error\n");
		free(Buf);
		fclose(fp);
		return -1;
	}

	free(Buf);
	fclose(fp);

	return 0;
}
