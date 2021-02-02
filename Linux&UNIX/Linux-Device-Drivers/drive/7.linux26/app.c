#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main(int argc,char *argv[])
{
	int fd;
	while(1)
	{
	   fd = open(argv[1],O_RDWR);
	   if(fd < 0)
	   {
		 perror("open defeat\n");
		 return -1;
	   }
	   sleep(2);
	   read(fd,NULL,0);
	   write(fd,NULL,0);
	   close(fd);
	   sleep(2);
	}
	return 0;
}

