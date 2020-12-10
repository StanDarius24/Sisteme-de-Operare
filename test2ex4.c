/*

Sa se implementeze:
A) un program C, folosind apeluri sistem si functii de biblioteca
care se apeleaza sub forma ./executabil <character> <fisier>,
unde fisier este o cale catre un fisier obisnuit.
Programul verifica faptul ca a primit doua argumente; in caz contrar, afiseaza un mesaj
de eroare ce indica utilizarea corecta.
Procesul parinte creeaza doua procese fiu, citeste <fisier> si trimite doar caracterele
diferite de <caracter> catre primul fiu. Procesele fiu lanseaza in executie OBLIGATORIU
comenzi pentru a realiza filtrarea acelor linii care sunt mesaje valide. Un mesaj corect
conţine doar litere, cifre sau caracterul spaţiu, fara a avea vreo litera imediat dupa o cifra.
La terminarea fiecarui proces fiu, in caz de eroare, parintele afiseaza un mesaj de forma:
" Procesul fiu cu PID-ul <PID> s-a incheiat cu codul de eroare <cod>".
La final parintele trimite catre iesirea standard de eroare de cate ori a intalnit <caracter>
(primit ca argument ca si “caracter interzis”).
Procesele fiu si procesul parinte vor rula obligatoriu in paralel.
Se vor trata cazurile de eroare pentru apelurile sistem folosite.
Toate mesajele de eroare se scriu / trimit catre iesirea standard de eroare.
Functiile de biblioteca permise sunt DOAR pentru formatări (printf, sprintf)
şi operatii asupra sirurilor de caractere.

B) un shell script care se apeleaza sub forma bash script <caracter>
Script-ul verifica faptul ca a primit un argument; in caz contrar,
 va afisa un mesaj de utilizare.
Se citesc siruri de caractere ce reprezinta nume de fisiere
(NU trebuie verificat daca sunt fisiere) de la tastatură (stdin) până la la EOF (Ctrl+D).
Pentru fiecare nume de fisier, se executa programul de la punctul A) cu <caracter>
si numele de fisier citit ca argumente, numarand cate cuvinte contin liniile filtrate de program.

La final se va afisa pe ecran numarul maxim de cuvinte gasite in fisiere.

*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#define SIZE 4096

int main(int argc, char *argv[])
{
	int pfd1[2];    //  parinte -> fiu1
	int pfd2[2];	//fiu1 -> fiu2

	if(argc!=3)
	{
		printf("Eroare la nr de argumente\nUsage %s caracter fisier\n",argv[0]);
		exit(1);
	}

	if(pipe(pfd1)<0)
	{
		printf("Eroare la creerea pipe-ului parinte->fiu\n");
		exit(6);
	}
	if(pipe(pfd2)<0)
	{
		printf("Eroare la creerea pipe-ului fiu1->fiu2\n");
		exit(7);
	}

	pid_t child;

	for(int i=0;i<2;i++)
	{
		if((child=fork())<0)
		{
			printf("Eroare la creerea procesului %d",i+1);
			exit(2);
		}
		else
			if(child==0 && i==0 )
			{
				close(pfd1[1]);
				close(pfd2[0]);
				dup2(pfd1[0],0);
				dup2(pfd2[1],1);
				execlp("grep","grep","[A-Za-z0-9 ]",NULL);
				printf("Eroare la executarea exec in fiu1\n");
				close(pfd1[0]);
				close(pfd2[1]);
				exit(-1);
			}
			else
				if(child==0 && i==1)
				{
					close(pfd1[0]);
					close(pfd1[1]);
					close(pfd2[1]);
					dup2(pfd2[0],0);

					//execlp("wc","wc","-w",NULL);
					execlp("grep","grep","-v","[a-zA-Z][0-9]",NULL);

					printf("Eroare la exec in fiu2\n");
					close(pfd2[0]);
					exit(-1);

				}

	}
	close(pfd1[0]);
	close(pfd2[0]);
	close(pfd2[1]);

	int fd;

	if((fd=open(argv[2],O_RDONLY))<0)
	{
		printf("Eroare la deschiderea fisierului\n");
		exit(8);
	}

	char *fis=malloc(sizeof(char)*SIZE);
	int n,kl=0;
	while((n=read(fd,fis,SIZE)))
	{	
		for(int i=0;i<n;i++)
			if(fis[i]!=argv[1][0])
				{	char sw=fis[i];
					
					if(write(pfd1[1],&sw,1)<0)
					{
						printf("Eroare la scrierea in pipe de la parinte -> fiu1\n");
						exit(10);
					}
				}
				else
				kl++;

	}
	free(fis);
	close(fd);
	close(pfd1[1]);
	pid_t wpid;
	int status;
	printf("\n");
	for(int i=0;i<2;i++)
	{
		wpid=wait(&status);

		if(WIFEXITED(status))
			printf("Procesul fiu cu PID-ul %d s-a incheiat cu codul de eroare %d\n",wpid,WEXITSTATUS(status));
		else
			printf("Child ended abnormally\n");
	}
	printf("\n");
	dup2(3,2);
	for(int i=0;i<kl;i++)
		printf("Caracter interzis");
	printf("\n");
	exit(0);


	return 0;
}