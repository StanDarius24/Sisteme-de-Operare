#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<dirent.h>
#include<limits.h>

void printare(int proces,int c)
{
	for(int i=0;i<c;i++)
		printf("%d : %d\n",proces,i);
}
void procesfiufiu()
{

printare(getpid(),100);
}

void procesfiu()
{
	pid_t fiufiu,fs;
	int status;

	if((fiufiu=fork())<0)
	{
		printf("eroare la fiul fiului\n");
		exit(2);
	}
	else
		if(fiufiu==0)
		{
			procesfiufiu();
			printf("tata : %d, fiu : %d\n",getppid(),getpid());
			printf("proces fiu fiu terminat\n");
			exit(3);
		}
		else
		{
			printare(getpid(),100);
			fs=wait(&status);
			printf("tata : %d, fiu : %d\n",getppid(),getpid());
			printf("proces fiu terminat\n");
		}
exit(4);
}

void procestata()
{	pid_t wais;
	int status;
	printare(getpid(),100);
	wais=wait(&status);

}

int main(int argc, char *argv[])
{
	pid_t child;
	
	if((child=fork())<0)
	{
		printf("eroare la creere proces\n");
		exit(1);
	}
	if(child==0)
		procesfiu();
	else
		procestata();
	
	
	printf("proces tata terminat %d\n",getpid());
	
	

	return 0;
}
