#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
int main(int argc,char *argv[])
{
	char buf[1024]={0};
	int ret;
	int fp;
	fp = open(argv[1],O_RDWR);
	if(fp < 0)
	{
		printf("open error\n");
		return -1;
	}
	read(fp,buf,1024);
	ret = write(fp,buf,sizeof(buf));
	if(ret < 0)
	{
		printf("write error\n");
		return -1;
	}
	close(fp);
	return 0;
}

