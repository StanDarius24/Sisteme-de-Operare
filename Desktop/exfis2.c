#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/syscall.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<ctype.h>
#define sizeo 4096
void printerori(char *s)
{
	printf("Eroare la %s\n",s);
}

int main(int argc, char * argv[])
{
int fd;
if((fd=open(argv[1],O_RDONLY))<0)
{
	printerori("deschidere");
	exit(2);
}
char *buf=malloc(sizeof(char)*sizeo);

if(read(fd,buf,100)<0)
{
	printerori("citire nume");
	exit(3);

}
printf("File name: %s\n",buf);
strcpy(buf,"");
if(read(fd,buf,8)<0)
{
	printerori("citire nume");
	exit(3);

}

printf("Owner num id: %s\n",buf);
strcpy(buf,"");
if(read(fd,buf,8)<0)
{
	printerori("citire nume");
	exit(3);

}

printf("Group num id: %s\n",buf);
strcpy(buf,"");
if(read(fd,buf,8)<0)
{
	printerori("citire nume");
	exit(3);

}

printf("File mode: %s\n",buf);
strcpy(buf,"");
if(read(fd,buf,12)<0)
{
	printerori("citire nume");
	exit(3);

}

printf("Size in bytes: %s\n",buf);
close(fd);
free(buf);
return 0;
}