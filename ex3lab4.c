#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<dirent.h>
#include<limits.h>
#define size 4
void Error(char *s)
{
	printf("Eroare la %s\n",s);
}

int main(int argc, char *argv[])
{
	if(argc!=3)
	{
		Error("la argumente");
		exit(1);
	}

	int fd1,fd2;
	
	char *buf=malloc(sizeof(char)*size);
	

		if((fd1=open(argv[1],O_RDONLY))<0)
		{
			Error("deschiderea fis in 4 bytes");
			exit(2);
		}
		int i=0;
		int val=32;
		int maxim=-32, minim=32, valmedie=0;
		while(read(fd1,buf,size))
		{	
			val=atoi(buf);

			valmedie=valmedie+val;
			i++;
	
			if(val>maxim)
				maxim=val;

			if(val<minim)
				minim=val;

		}
		valmedie=valmedie/i;
		if(close(fd1)!=0)
		{
			Error("inchidere fis");
			exit(3);
		}
		printf("%d %d %d\n",minim,maxim,valmedie);
	

free(buf);
return 0;
}