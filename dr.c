#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<dirent.h>
#include<limits.h>
#define BUFFSIZE 4096
void usage(char *s)
{
	printf("eroare la %s",s);
}

void parcurgere(char *numedir, int nivel)
{
DIR *dir;
struct dirent *direct;
struct stat info;
char *nume=malloc(sizeof(char)*100);
char *buff=malloc(sizeof(char)*BUFFSIZE);
char *cale=malloc(sizeof(char)*100);
if(!(dir=opendir(numedir)))
{
	usage("deschiderea direcotrului");
	exit(2);
}

	while((direct=readdir(dir))>0)
	{
		strcpy(nume,direct->d_name);

		if( strcmp(nume, ".")==0 || strcmp(nume, "..")==0 )
			continue;

		snprintf(cale,99,"%s/%s",numedir,nume);

		if(lstat(cale,&info)<0)
		{
			usage("stat");
			exit(3);

		}

		if(S_ISREG(info.st_mode))
		{	int fd1;
			if((fd1=open(cale,O_RDONLY))<0)
			{
				usage("deschidere fisier din director");
				exit(4);
			}
			else
			{	printf("%s\n\n",nume);
				
				while(read(fd1,buff,BUFFSIZE)>0)
				{
					printf("%s",buff);
				}
				printf("\n");
			}
			close(fd1);
		}
		else
			if(S_ISDIR(info.st_mode))
			{
				parcurgere(cale,nivel+1);
			}



	}


closedir(dir);
}


int main(int argc, char *argv[])
{
	if(argc!=2)
	{
		usage("Argumente");
		exit(1);
	}

	parcurgere(argv[1],1);

	return 0;
}