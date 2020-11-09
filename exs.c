#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<signal.h>
#include<sys/wait.h>
#include<string.h>
#include<unistd.h>
pid_t child;
double a,r,cntr;
char cc;
int i,prts;
int kk;
void usage(char *s)
{
	printf("Eroare la %s\n",s);
}

void calculate()
{
	while(1)
		{a=a+1/r;
cntr++;}
}

void childprint(int sig)
{	prts++;
	printf("val=%lf iteratie=%lf \n",a,cntr);
if(prts==2)
	exit(4);
}

void procesfiu()
{
struct sigaction child_action;
memset(&child_action,0x00,sizeof(struct sigaction));
child_action.sa_handler=childprint;
if(sigaction(SIGUSR1,&child_action,NULL)<0)
{
	perror("Eroare la primire SIGUSR1");
	exit(-1);
}
calculate();
}
void printeaza()
{	
	int hg=0;
	while(1)
	{
		hg++;
		if(hg==1000)
		{
		printf("%c",cc);
		hg=0;
		}
	}
}
void fatherends(int sig)
{
	int status;
	wait(&status);
	printf("father %d ended succesfully\n",getpid());
	exit(0);
}

void changechar(int sig)
{	

	if(kill(child,SIGUSR1)<0)
	{
		printf("eroare la sigusr1\n");
		exit(6);
	}	
	if(kk==0)
		cc='*';
	else
		cc='-';
		kk++;
		alarm(1);
}

void procestata()
{
	struct sigaction process_father;
	memset(&process_father,0x00,sizeof(struct sigaction));
	process_father.sa_handler=fatherends;
	if(sigaction(SIGCHLD,&process_father,NULL)<0)
	{
		perror("eroare la sigchld tata");
		exit(-1);
	}
	process_father.sa_handler=changechar;
	if(sigaction(SIGALRM,&process_father,NULL)<0)
	{
		perror("eroare la sigalarm tata");
		exit(-1);
	}

	printeaza();
}

int main(int argc, char *argv[])
{
	
	cc='+';	
	alarm(1);
	if(argc!=3)
	{
		usage("Parametrii");
		exit(1);
	}
	

	
	a=atof(argv[1]);
	r=atof(argv[2]);
	
	if((child=fork())<0)
	{
		perror("Eroare la creerea procesului");
		exit(-1);
	}
	else
		if(child==0)
		{


			procesfiu();
		}
		
	procestata();
		

return 0;
	
}