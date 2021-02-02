#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
void delay_nms(unsigned int ms);
int main(void)
{
	int fd_led=0,fd_beep=0,fd_key=0;
	unsigned char buf = 0,ch=0;
    fd_led = open("/dev/led_device", O_RDWR);
	if (fd_led < 0)
	{
		printf("open dev led device failure!\n");
		return -1;
	}
	printf("open dev led device success!\n");
	fd_beep = open("/dev/beep_device", O_RDWR);
	if (fd_beep < 0)
	{
		printf("open dev beep device failure!\n");
		return -1;
	}
	printf("open dev beep device success!\n");
	fd_key = open("/dev/key_device", O_RDWR);
	if (fd_key < 0)
	{
		printf("open dev key device failure!\n");
		return -1;
	}
	printf("open dev key device success!\n");
	while (1)
	{
		buf++;
		if (buf > 5)
		{
			buf = 1;
		}
		write(fd_led , &buf, 1);
		read(fd_key, &ch, 1);
		if ((ch == 1) || (ch == 2) || (ch == 3))
		{
			ioctl(fd_beep, 1);
		}
		else
		{
			ioctl(fd_beep, 0);
		}
		if(ch == 4)
		{
		    goto p;
		}
		delay_nms(999999);
	}
	p:
	close(fd_led);
	close(fd_beep);
	close(fd_key);
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

