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
 //��1.bin������ȡ������Ӻ�У��д���2.bin
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

	BufLen = atoi(argv[3]);//�ǰ��ַ���ת������������һ������ "14336" -->14336��14*1024=14336��
	Buf = (char *)malloc(BufLen);//Ϊ Buf ���ٿռ�
	memset(Buf, 0x00, BufLen);//��ʼ�� Buf

	fp = fopen(argv[1], "rb"); //��ֻ���ķ�ʽ�� led.bin
	if( fp == NULL)
	{
		printf("source file open error\n");
		free(Buf);
		return -1;
	}

	fseek(fp, 0L, SEEK_END); //�ƶ��ļ�ָ�뵽���
	fileLen = ftell(fp); //��ȡ��ǰ�ļ�ָ�룬�õ����ļ�����
	fseek(fp, 0L, SEEK_SET);//�ƶ��ļ�ָ�뵽��ͷ

	if ( fileLen >  (BufLen-16))//���������дһ�������жϲ�Ҫ����14K����
	{
		printf("Usage: unsupported size,size more than 14K \n");
		free(Buf);
		fclose(fp);
		return -1;
	}

	nbytes = fread(Buf, 1, BufLen, fp); //  //��ȡ1.bin�����ٵ��ڴ� ����������С�� 14K

	

	fclose(fp); //�ر��ļ�
	
	//ѭ���ۼ�ǰ�� 14K-4 �ֽ�
	for(i = 0;i < (14 * 1024) - 4;i++)
	{
		checksum += (unsigned char)(Buf[i]); //���͵�żУ��
	}
	*(unsigned int*)(Buf+i) = checksum; //У�� ���õ���У���ŵ��ڴ�ĩβ

	fp = fopen(argv[2], "wb");  //��һ������д��Ķ������ļ� bl2.bin 
	if (fp == NULL)
	{
		printf("destination file open error\n");
		free(Buf);
		return -1;
	}

	a = Buf;
	nbytes	= fwrite( a, 1, BufLen, fp);//��led.bin�ļ���ߵ������Լ�Ч��λд�뵽bl2.bin

	if ( nbytes != BufLen )//�ж�д��ĳ����Ƿ��buflen���
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
