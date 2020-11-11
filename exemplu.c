#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#define SIZE 4096
char c;
int cntr;
pid_t child[10];
int lp,vs;

void printpoint()
{	
	while(1)
		
			
	printf(".");
}

void endfather(int sig)
{	

int status;

for(int l=2;l<lp;l++)
{wait(&status);
 printf("\nChild ended with code %d\n", WEXITSTATUS(status));}

printf("Father ended %d\n",getpid());
exit(0);


}

int main(int argc, char * argv[])
{	lp=argc;
	char *buf = malloc (sizeof(char) * SIZE);
	if(argc <3)
	{
		printf("Eroare la argumente\n");
		exit(1);
	}
	if(strlen(argv[1])>1)
		printf("%s nu e char",argv[1]);
	else
	c=*argv[1];

	for(int i=2;i<argc;i++)
	{

		if((child[cntr]=fork())<0)
		{
			perror("La creerea unui nou proces");
			exit(-1);
		}
		else
			if(child[cntr]==0) //proces fiu
			{
				struct stat info;
				if(stat(argv[i],&info)<0)
				{
					perror("Eroare la fc stat");
					exit(2);
				}
				
					if(S_ISREG(info.st_mode))
					{
						int fd;
						if((fd=open(argv[i],O_RDONLY))<0)
						{
							printf("Eroare la deschidere fisier\n");
							exit(4);
						}
						else
						{	int n;
							int vctr=0;
							while((n=read(fd,buf,SIZE))>0)
							{
								for(int k=0;k<n;k++)
									if(buf[k]==c)
										vctr++;
							}
							if(close(fd)<0)
								{printf("Eroare la inchidere fisier\n");}
							printf("Procesul %d cu Fisier %s are %d de %c\n",getpid(),argv[i],vctr,c);
						}
						
						
					}
					else
						printf("Fisierul nu e normal %s\n ",argv[i]);
					
				
				exit(0);
			}
			
				



	}
	struct sigaction action;
					memset(&action,0x00,sizeof(struct sigaction));
					action.sa_handler = endfather;
					if(sigaction(SIGCHLD,&action,NULL)<0)
					{
						printf("Eroare la SIGCLD\n");
						exit(6);
					}
						


					printpoint();

	free(buf);
	return 0;
}