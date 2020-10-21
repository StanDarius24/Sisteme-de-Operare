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

void printerror(char *p)
{
	printf("eroare la functia %s\n",p);
}

int main(int argv, char * argc[])
{
	int fd1;
	char *buffer;
	buffer = malloc(sizeof(char) * sizeo);
	if(	(fd1=open(argc[1],O_RDONLY)) == -1)
	{	printf("%s",argc[1]);	
		printerror("deschidere fisier");
		exit(2);
	}
	struct stat st;
	if(stat(argc[1],&st)<0)
	{
		printerror("eroare la stat");
		exit(4);
	}

	if(!S_ISREG(st.st_mode))
	{
		printerror("fisierul nu e simplu");
		exit(5);
	}
	int p;
	int contorUpper=0,contorLower=0,contorCifra=0,contorLitera=0,contorDim=0;
	if((p=read(fd1,buffer,sizeo))!=0)
	{
		for(int i=0;i<p;i++)
			{	
				if(isupper(buffer[i]))
					{contorUpper++;
						if(buffer[i]==*argc[3])
								contorLitera++;}
				else
					if(islower(buffer[i]))
						{contorLower++;
							if(buffer[i]==*argc[3])
								contorLitera++;}
					else
						if(isdigit(buffer[i]))
							contorCifra++;					
					contorDim++;}

	
	}
	else
	{
		printerror("citirea fisierului");
		exit(3);
	}

	close(fd1);
	int fd2;
	if(	(fd2=creat(argc[2],S_IRWXU)) == -1)
	{	printf("%s",argc[2]);	
		printerror("deschidere fisier");
		exit(5);
	}
	if(stat(argc[2],&st)<0)
	{
		printerror("eroare la stat");
		exit(6);
	}

	if(!S_ISREG(st.st_mode))
	{
		printerror("fisierul nu e simplu");
		exit(7);
	}
	free(buffer);
	char *scrie=malloc(sizeof(char) * sizeo);
	strcpy(scrie,"");
	int x=sprintf(scrie,"numar litere mici : %d\nnumar litere mari : %d\nnumar cifre : %d\nnumar aparitii caractere : %d\ndimensiune fisier : %d\n",contorLower,contorUpper,contorCifra,contorLitera,contorDim);

	if(write(fd2,scrie,x)<0)
	{
		printerror("eroare la scriere");
		exit(8);
	}
	free(scrie);
	close(fd1);
	close(fd2);
	return 0;
}