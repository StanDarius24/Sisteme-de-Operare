/*
Sa se scrie un program C care se apeleaza sub forma:
<program> <director> <director-nou> <br-intreg>
Programul va parcurge nerecursiv directorul <director> primit ca si argument
Pentru fiecare fisier cu permisiune de read si write pentru owner
se va crea cate un proces fiu care:
-va procesa eficient in intregime informatia din fisierul respectiv
-va crea o copie a fisierului in directorul <director-nou>
-va clona continutul fisierului astfel incat fiecare cuvant sa fie pe o linie
noua.
De fiecare data cand se va adauga un caracter '\n', procesul fiu va trimite
un semnal <nr-intreg> catre parinte.Procesul se va termina cu un cod de iesire
ce reprezinta dimensiunea in bytes a fisierului procesat.
Parintele va contoriza de cate ori a primit acel semnal si va afisa la finalul
executiei.
Pentru fiecare copil parintele va afiasa o linie de forma:
"Copil<PID> <Cod iesire>"
*/
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#define SIZE 4096
int contor;
pid_t child;
void count(int sig)
{
contor++;
}

void endfather(int sig)
{
	int status;

	wait(&status);

	if(WIFEXITED(status))
	{
		printf("child %d ended with value %d\n",status,WIFEXITED(status));
	}

	printf("Father ended %d\n",contor);
	exit(0);
}

void processfather()
{
	struct sigaction fatheraction;
	memset(&fatheraction,0x00,sizeof(struct sigaction));
	fatheraction.sa_handler=count;
	if(sigaction(SIGUSR1,&fatheraction,NULL)<0)
	{
		printf("Eroare la SIGUSR1 child\n");
		exit(9);
	}

	fatheraction.sa_handler=endfather;
	if(sigaction(SIGCHLD,&fatheraction,NULL)<0)
	{
		printf("Eroare la terminarea proces fiu");
		exit(10);
	}

	while(1)
		;


}


int main(int argc, char *argv[])
{	struct stat info;
	char *buf = malloc(sizeof(char)*SIZE);
	char *fname=malloc(sizeof(char)*350);
	char *name = malloc(sizeof(char)*20);
	char *sname=malloc(sizeof(char)*350);
	if(argc!=4)
	{
		printf("Eroare la argumente\n");
		exit(1);
	}

	

	DIR *director;

	if((director=opendir(argv[1]))<0)
	{
		printf("Eroare la deschiderea directorului\n");
		exit(2);
	}

	struct dirent *dir;
	if(mkdir(argv[2], S_IRWXU)<0)
	{
		printf("Eroare la creerea director\n");
		exit(-1);
	}

	if((child=fork())<0)
					{
						printf("Eroare la creere proces\n");
						exit(-1);

					}
					else
						if(child==0)
					{ int n;


	while((dir=readdir(director)))
	{	
		
		name=dir->d_name;
		
		if( (strcmp(name,".")== 0) || (strcmp(name,"..")==0) )
			continue;


		if(stat(name,&info)<0)
		{
			printf("Eroare la stat\n");
			exit(1);
		}
		printf("%s\n",name);
		if((info.st_mode & S_IRUSR) && (info.st_mode & S_IWUSR))
		{
			snprintf(sname,30,"%s/%s",argv[1],name);
			int fd,fd2;
			if((fd=open(sname,O_RDONLY))<0)
			{
				printf("Eroare la deschidere fis\n");
				exit(3);
			}
			

			while((n=read(fd,buf,SIZE))>0)
			{	
						struct sigaction action_child;
						memset(&action_child,0x00,sizeof(struct sigaction));
						
				
						snprintf(fname,30,"%s/%s",argv[2],name);
						printf("fname %s \n",fname);
						printf("%s\n",buf);
			
						for(int i=0;i<n;i++)
							if(buf[i] == ' ')
							{
								buf[i]='\n';
								if(kill(getppid(),SIGUSR1)<0)
								{
									printf("Eroare la trimitrea SIGUSR1\n");
									exit(9);
								}
								else
									{printf("signal sended\n");}
							}

						if((fd2=open(fname,O_WRONLY | O_CREAT | O_EXCL, S_IRWXU))<0)
						{
							printf("Eroare la creere fisier\n");

							exit(4);
						}

							if(write(fd2,buf,n)<0)
							{
								printf("Eroare la scriere in fisier\n");
								exit(5);
							}
						close(fd2);
						
						
					}	

					
					
			close(fd);

		}



	}
exit(n);
}
processfather();

	
	
	free(buf);
	closedir(director);
free(fname);
						
free(sname);

free(name);

	return 0;
}