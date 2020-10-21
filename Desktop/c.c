#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>

#define SIZE 4096

int main (int argc,char *argv[])
{
	int p;
	int fd, ct=0;
	char buffer[SIZE];
	if(argc!=2)
	{
		perror("eroare, nu este un singur argument");
		exit(1);
	}
	
	int status=stat(argv[1],buffer);
	if(S_ISREG(status)!=0)
	{
		printf("it s not a reg file");
		exit(3);
	}
	if(status!=0)
	{
		perror("eroare, fisier neobisnuit");
		exit(2);
	}
	
	if(fd=open(argv[1],O_RDONLY)<0)
	{
		perror("eroare, nu s-a putut deschide fisierul");
			exit(3);	
	}
	
	while((p=read(fd,buffer,SIZE))>0)
	{	
		for(int i=0;i<p;i++)
			if(isupper(buffer[i])!=0)
				ct++;
		
	}
	close(fd);
	printf("%d",ct);
	return 0;
	
}