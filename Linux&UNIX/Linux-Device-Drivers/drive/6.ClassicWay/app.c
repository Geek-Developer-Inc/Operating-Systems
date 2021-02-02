#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc,char *argv[])
{
	int fp;
	fp =open(argv[1], O_RDWR);
	if(fp < 0)
	{
		printf("open failure\n");
		return -1;
	}
	write(fp,NULL,0);
	read(fp,NULL,0);
	close(fp);
	return 0;
}

  




