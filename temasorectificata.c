/*
prog dir

Fiul 1 transmite intrarile din <dir> catre fiul 2, care filtreaza liniile ce contin
 fisiere obisnuite (^-) si le transmite catre fiul 3.

Al treilea fiu numara liniile primite.

Parintele si fiii se executa in paralel!

Parintele afiseaza ... pana la term tuturor fiilor si afiseaza codurile de retur pe masura 
e se termina fiecare fiu.



Fiecare fiu executa cate o comanda cu argumente si redirecteaza stdin si stdout.

Recomandare: daca se termina procesele fiu prea repede sau aproape deodata, se poate pune 
un sleep(3) in al doilea fiu inainte de executia comenzii, si un sleep(5) in al treilea, 
pentru a ne asigura ca se termina pe rand.
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

int pfd1[2];
int pfd2[2];

void procesfiu1(char *director)   // fiu1 -> fiu2 prin pfd1
{
	close(pfd1[0]);
	close(pfd2[0]);
	close(pfd2[1]);

	dup2(pfd1[1],1);
	printf("child1\n");
	execlp("ls","ls","-al",director,NULL);

	printf("Eroare la fc exec\n");
	exit(9);
}

void procesfiu2()		// fiu1 -> fiu2 prin pfd1 , fiu2 -> fiu3 prin pfd2
{
	close(pfd1[1]);
	close(pfd2[0]);
	sleep(3);
	dup2(pfd1[0],0);
	dup2(pfd2[1],1);
	printf("child2\n");
	execlp("grep","grep", "^-" , "-E" , NULL);

	printf("Eroare la exec\n");
	exit(10);
}

void procesfiu3()
{
	close(pfd1[0]);
	close(pfd1[1]);
	close(pfd2[1]);
	sleep(5);
	dup2(pfd2[0],0);
printf("child3\n");
	execlp("wc","wc","-l","NULL");

	printf("Eroare la exec\n");

	exit(11);
}
int contor;

void takechild(int sig)
{
	contor++;
	
	int status;
	wait(&status);
	if(WIFEXITED(status))
	     printf("\nChild ended with code %d\n",WEXITSTATUS(status));
	  else
	     printf("\nChild ended abnormally\n");

	 if(contor==2)
	 {
	 	printf("Father ended succesfully\n");
	 	exit(0);
	 }
}

void procestata()
{
	close(pfd1[0]);
	close(pfd1[1]);
	close(pfd2[1]);
	close(pfd2[0]);
	struct sigaction fatherprocess;
	memset(&fatherprocess,0x00,sizeof(sigaction));
	fatherprocess.sa_handler=takechild;
	if(sigaction(SIGCHLD,&fatherprocess,NULL)<0)
	{
		printf("Eroare la SIGCHLD\n");
		exit(11);
	}
	printf(".");
	while(1)
		;
}

int main(int argc, char *argv[])
{	

	
	pid_t child;

	if(argc!=2)
	{
		printf("Eroare la numarul de argumente\n Usage : %s <director> \n",argv[0]);
		exit(1);
	}
	struct stat stat;

	if(pipe(pfd1)<0)
	{
		printf("Eroare la creerea pipe 1\n");
		exit(5);
	}
	if(pipe(pfd2)<0)
	{
		printf("Eroare la creerea pipe 2\n");
		exit(6);
	}

	if(lstat(argv[1],&stat)<0)
	{
		printf("Eroare la lstat pentru director\n");
		exit(2);
	}

	if(!S_ISDIR(stat.st_mode))
	{
		printf("Fisierul transmis %s nu este director\n",argv[1]);
		exit(3);
	}

	for(int i=0;i<3;i++)
	{

		if((child=fork())<0)
		{
			printf("Eroare la creerea procesului\n");
			exit(4);
		}
		else
			if(i==0 && child==0)
				procesfiu1(argv[1]);
			else
				if(i==1 && child==0)
					procesfiu2();
				else
					if(i==2 && child==0)
						procesfiu3();


	}
	procestata();

	
	


return 0;
}