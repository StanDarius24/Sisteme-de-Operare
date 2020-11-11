/*
Sa se implementeze un program C, folosind apeluri sistem si functii de
 biblioteca, care se apeleaza sub forma: ./executabil c new_c file.
Se va verifica faptul ca exista 3 argumente ce reprezinta doua caractere 
alfanumerice si calea catre un fisier obisnuit. In caz de erori 
la numarul sau tipul argumentelor, se vor afisa mesaje de utilizare 
corespunzatoare.
Se va crea un proces fiu care inlocuieste fiecare aparitie a caracterului 
c in fisierul file cu caracterul new_c intr-o singura trecere, 
pe masura ce citeste fisierul. Parintele va afisa in fiecare secunda “Tic-tac”.

Inainte de terminare, procesul fiu afiseaza rezultatul sub forma “S-au facut
 <n> inlocuiri.” si apoi trimite numarul de inlocuiri procesului 
parinte prin codul de retur.
La terminarea procesului fiu, parintele va afisa pe ecran un mesaj de forma 
"Procesul fiu cu PID-ul <PID> s-a terminat cu codul <cod>" si apoi
 se termina si el.
Procesul fiu si procesul parinte vor rula obligatoriu in paralel.

Se vor trata cazurile de eroare pentru apelurile sistem folosite.
Functiile de biblioteca permise sunt DOAR pentru formatări (printf),
 verificări de tip de caracter si operatii asupra sirurilor de caractere.
Nota: Pentru a incetini ritmul in care fiul face inlocuiri, se poate 
folosi apelul usleep(100000) in timpul citirii fisierului.
Exemplu de apelare:
.executabil f F file
Daca file contine:
abcdefgabcdefg
Rezulta 2 inlocuiri si file va avea continutul:
abcdeFgabcdeFg
*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<signal.h>
#include<sys/wait.h>
char c,c_new;
#define size 4096
pid_t child;
void usage(char *s)
{
	printf("Eroare la %s\n",s);
}

void processchild(char *fisier)
{	int nrinloc=0;
	int fd;
	char *buf = malloc(sizeof(char) * size);
	if((fd=open(fisier,O_RDWR))<0)
	{
		usage("deschidere fisier");
		exit(6);
	}
	else
	{
		int n;
		n=read(fd,buf,size);		
			for(int i=0;i<n;i++)
				if(buf[i]==c)
					{buf[i]=c_new;
						nrinloc++;}

		
		if(lseek(fd,SEEK_SET,0)<0)
		{
			usage("lseek");
			exit(7);
		}

		if(write(fd,buf,n)<0)
		{
			usage("scriere");
			exit(8);
		}



	}
close(fd);
free(buf);

printf("Child %d ended succesfully %d changes \n",getpid(),nrinloc);
exit(nrinloc);

}

void tictac()
{
	while(1)
		{	printf("tic-tac");
	//usleep(1000);
	}
}
void fatherend(int sig)
{
	int status;
	wait(&status);

	if(WIFEXITED(status))
     printf("\nChild ended with code %d\n", WEXITSTATUS(status));

	printf("Father ended\n");
	exit(0);

}

void processfather()
{
	struct sigaction action_father;
	memset(&action_father,0x00,sizeof(struct sigaction));
	action_father.sa_handler=fatherend;

	if(sigaction(SIGCHLD,&action_father,NULL)<0)
	{
		perror("Eroare la SIGCHLD");
		exit(-1);
	}

	tictac();
}


int main(int argc, char *argv[])
{
	struct stat info;
	if(argc!=4)
	{
		usage("nr de argumente");
		exit(1);
	}
	else
	{
		if(strlen(argv[1])!=1 || strlen(argv[2])!=1)
		{
			usage("cele 2 caractere sunte text");
			exit(2);
		}
		else
		{
			c=*argv[1];
			c_new=*argv[2];
			
			if(stat(argv[3],&info)<0)
			{
				usage("functia stat");
				exit(3);
			}
			else
			if(!S_ISREG(info.st_mode))
			{
				usage("Fisierul nu este unul obisnuit");
				exit(4);
			}
		}
	}

	if((child=fork())<0)
	{
		usage("creere proces");
		exit(5);
	}
	else
		if(child==0)
		{
			processchild(argv[3]);
		}

processfather();


return 0;
}