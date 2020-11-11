/*

Sa se implementeze un program c, folosind apeluri de sistem si functii de biblioteca, care
se apeleaza sub forma: ./executabil
Se va verifica faptul ca exista un argument ce reprezinta calea catre un fisier obisnuit cu
drept de executie pentru grupul utilizatorului. In caz de erori la nr sau tipul argumentelor
se vir afisa mesaje de utilizare corespunzatoare.
Se va crea un proces fiu.Parintele va citi fisierul dat ca argument si de fiecare data
cand gaseste cifra 1, trimite SIGUSR1 catre procesul fiu.
Procesul fiu numara de cate ori primeste semnalul SIGUSR1 si se termina atunci cand
parintele termina de citit fisierul.Inainte de terminare, procesul fiu afiseaza rezultatul
sub forma "Procesul fiu cu PID-il <PID> are rezultatul: <rezultat>." si apoi il trimite
procesului parinte prin codul de retur.
La terminarea procesului fiu, parintele va afisa pe ecran un mesaj de forma
"Procesul fiu cu PID-ul <PID> s-a terminat cu codul <cod>"
Procesul fiu si procesul parinte vor rula obligatoriu in paralel.


*/
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include <signal.h>
#include <fcntl.h>
#include<string.h>
#include <sys/wait.h>

#define SIZE 4094

pid_t child;
int contor=0;

void fatherend()
{
	int status;
	wait(&status);

      printf("\nChild %d ended with code %d\n", child, WEXITSTATUS(status));
		printf("Father ended with code 0\n");
		exit(0);

}

void processtata(char *fisier)
{	int n;
	int fd;
	char *buf;
	struct sigaction father_action;
	memset(&father_action,0x00,sizeof(struct sigaction));

	

	buf=malloc(sizeof(char)*SIZE);
	if((fd=open(fisier,O_RDONLY))<0)
	{
		perror("La deschidere fisier");
		exit(-3);
	}
	else
	{
		
		while((n=read(fd,buf,SIZE)))
		{	
			for(int i=0;i<=n;i++)
				{if(buf[i]=='1')
					if(kill(child,SIGUSR1)<0)
					{
						perror("Eroare la trimiterea SIGUSR1");
						exit(-4);
					}
				usleep(100);	
				}	
		}

	}
	
	


close(fd);
if(n==0)
	if(kill(child,SIGUSR2)<0)
	{
		perror("Eroare la inchidere");
		exit(-5);
	}

free(buf);



fatherend();
}

void count(int sig)
{
	contor++;
	printf("val= %d\n",contor);
}

void endchild(int sig)
{
	printf("Procesul fiu cu PID-il < %d > are rezultatul: < %d >.\n",child,contor);

	exit(contor);
}

void processfiu()
{
	struct sigaction child_process;
	memset(&child_process,0x00,sizeof(struct sigaction));
	child_process.sa_handler=count;

	if(sigaction(SIGUSR1,&child_process,NULL)<0)
	{
		printf("Eriare ka SIGUSR1\n");
		exit(12);
	}

	

	child_process.sa_handler=endchild;
	if(sigaction(SIGUSR2,&child_process,NULL)<0)
	{
		printf("Eriare ka SIGUSR1\n");
		exit(12);
	}

	while(1)
		;

}

int main(int argc, char *argv[])
{
	if(argc!=2)
	{
		printf("Eroare la argumente\n");
		exit(1);
	}

	struct stat info;

	if(stat(argv[1],&info)<0)
	{
		perror("eroare la stat");
		exit(-1);
	}

	if(S_ISREG(info.st_mode) && (info.st_mode & S_IXGRP))
	{
		if((child=fork())<0)
		{
			perror("Eroare la fork");
			exit(-2);
		}
		else
			if(child==0) // fiu
				processfiu();
			

		processtata(argv[1]);
	}	
	else
		printf("NU are drept de executie/ nu e obisnuit \n");


return 0;
}