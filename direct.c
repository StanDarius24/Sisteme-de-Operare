#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include <dirent.h>
#include <limits.h>
#define BUFFSIZE 4096
// primul argument un director, cautare recursiva in directoare
// daca e un fisier text se salveaza textul, la final se
// scrie textul salvat intr-un fisier citit argument
// si se creaza o legatura simbolica intre directoare si fisierele pe
// care le are directorul.
void usage(char *c)
{
	
	printf("Eroare la %s\n",c);

}


void parcurgeredir(char *numedir, int nivel,char *fisscr)
{
	DIR *director;
	struct dirent *dir;
	struct stat info;
	int fd1,fd2,n;
	char *buf=malloc(sizeof(char)*BUFFSIZE);
	char *name=malloc(sizeof(char)*30);
	char *cale=malloc(sizeof(char)*30);
if(!(director=opendir(numedir)))
	{
		usage("deschidere director principal");
		exit(2);
	}

while((dir=readdir(director))>0)
{
strcpy(name,dir->d_name);

snprintf(cale,29,"%s/%s",numedir,name);

if(strcmp(name, ".") == 0 || strcmp(name, "..") == 0 )
	continue;

if(lstat(cale,&info)<0)
{
	usage("functia stat");
	exit(3);
}

if(S_ISDIR(info.st_mode))
	{	
		parcurgeredir(cale,nivel+1,fisscr);
	}
else
	if(S_ISREG(info.st_mode))
	{	
		if((fd1=open(cale,O_RDONLY))<0)
		{	
			usage("deschidere fisier while");
			exit(6);
		}
		else
		{
			while((n=read(fd1,buf,BUFFSIZE))>0)
			{
				if((fd2=open(fisscr,O_WRONLY | O_APPEND,S_IRWXU))<0)
				{
					usage("deschidere in fis de scriere");
					exit(4);
				}
				else
				{
					if(write(fd2,buf,n)<0)
					{
						usage("scriere in fis");
						exit(5);
					}

				}
				close(fd2);
			}
			
		close(fd1);
		}


	}



}


closedir(director);
free(name);

free(buf);
}


int main(int argc, char *argv[])
{

	
	if(argc!=3)
	{
		usage("argumente");
		exit(1);
	}

	parcurgeredir(argv[1],1,argv[2]);

	return 0;
}