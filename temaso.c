#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
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



int main(int argc, char *argv[])
{
	int pfd1[2];
	int pfd2[2];

	if(argc!=2)
	{
		printf("Eroare la nr de argumente\nUsage %s director\n",argv[0]);
		exit(1);
	}
	pid_t child;

	if(pipe(pfd1)<0)
	{
		printf("Eroare la creerea pipe-ului\n");
		exit(2);
	}

	if(pipe(pfd2)<0)
	{
		printf("Eroare la creerea pipe-ului\n");
		exit(3);
	}

// pt pipe-uri 0 citire 1 scriere !

	for(int i=0;i<3;i++)
	{

		if((child=fork())<0)
		{
			printf("Eroare la creerea procesului fiu\n");
			exit(3);
		}
		else
			if(child==0 && i==0)
			{
				close(pfd1[0]);
				close(pfd2[0]);
				close(pfd2[1]);
				
				dup2(pfd1[1],1);
				execlp("ls","ls","-al",argv[1],NULL);
				close(pfd1[1]);
				printf("Eroare la execlp in fiu1\n");
				exit(-1);
			}
			else
				if(child==0 && i==1)
				{
					close(pfd1[1]);
					close(pfd2[0]);
					sleep(3);
					
					dup2(pfd1[0],0);
					dup2(pfd2[1],1);

					execlp("grep","grep","^-",NULL);
					printf("Eroare la exec in fiu2\n");

					close(pfd1[0]);
					close(pfd2[1]);
					exit(-1);
				}
				else
					if(child==0 && i==2)
					{	sleep(5);
						close(pfd1[0]);
						close(pfd1[1]);
						close(pfd2[1]);
						
						dup2(pfd2[0],0);
						execlp("wc","wc","-l",NULL);
						printf("Eroare la exec in fiu3\n");
						close(pfd2[0]);
						exit(-1);
					}

	}
close(pfd1[0]);
close(pfd1[1]);
close(pfd1[1]);
close(pfd2[1]);
int info;
for(int i=0;i<3;i++)
{
	child=wait(&info);

	if(WIFEXITED(info))
		printf("Child %d ended with code %d\n",child,WEXITSTATUS(info));
	else
		printf("Child ended abnormally :(\n");
}
printf("Father ended\n");
return 0;
}