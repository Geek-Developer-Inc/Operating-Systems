#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
int main(int argc,char *argv[])
{
      int fd;
      fd=open("/dev/example",O_RDWR);
      if(fd<0)
      {
           perror("open");
           return -1;
      }
      read(fd,NULL,0);
      write(fd,NULL,0);
      close(fd);
      return 0;  
}
