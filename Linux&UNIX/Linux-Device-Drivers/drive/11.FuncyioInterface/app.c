#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
void delay_nms(unsigned int ms);
int main(void)
{
	int fd=0;
	unsigned char buf = 0,ch=0;
    fd = open("/dev/DeviceDrive", O_RDWR);
	if (fd < 0)
	{
		printf("open dev DeviceDrive failure!\n");
		return -1;
	}
	printf("open dev DeviceDrive success!\n");
	while (1)
	{
		buf++;
		if (buf > 5)
		{
			buf = 1;
		}
		write(fd, &buf, 1);
		read(fd, &ch, 1);
		if ((ch == 1) || (ch == 2) || (ch == 3))
		{
			ioctl(fd, 1);
		}
		else
		{
			ioctl(fd, 0);
		}
		if(ch == 4)
		{
		    goto p;
		}
		delay_nms(999999);
	}
	p:
	close(fd);
	return 0;
}
void delay_nms(unsigned int ms)
{
	unsigned int i = 0, j = 0;
	for (i = ms; i > 0; i--)
	{
		for (j = 123; j > 0; j--)
		{
			;
		}
	}
}

