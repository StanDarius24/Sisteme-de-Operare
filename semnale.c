#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
int n=0;
pid_t child=0;
void processchildwrites(int sig)
{
	printf("child count:%d\n",n++);
}

void processchildends(int sig)
{
	printf("Process child ends!\n");
	exit(0);
}

void processchild()
{
	struct sigaction action_child;
	memset(&action_child,0x00,sizeof(struct sigaction));
	action_child.sa_handler=SIG_IGN;
  if (sigaction(SIGINT, &action_child, NULL) < 0)
    {
      perror("Process b ignore SIGINT");
      exit(-1);
    }
	action_child.sa_handler=processchildwrites;
	if(sigaction(SIGUSR1,&action_child,NULL)<0)
	{
		perror("Sigusr1 nu a reusit");
		exit(-1);
	}
	action_child.sa_handler=processchildends;
	if(sigaction(SIGUSR2,&action_child,NULL)<0)
	{
		perror("SIGUSR2 nu a reusit");
		exit(-1);
	}
	while(1)
		;
}

void processfatherend(int sig)
{
	
	int status;

	if(kill(child,SIGUSR2)<0)
	{
		printf("Nu s-a trimis SIGUSR2 catre fiu");
		exit(3);
	}

	wait(&status);
 printf("Child ended with code %d\n", WEXITSTATUS(status));

 printf("Father ended successful\n");
 exit(0);

}

void processtata()
{	
	int i;
	struct sigaction action_father;
	memset(&action_father,0x00,sizeof(struct sigaction));
	action_father.sa_handler=processfatherend;
	if(sigaction(SIGINT,&action_father,NULL)<0)
	{
		perror("Proces IGN neignorat");
		exit(-1);
	}
	
	for(i=0;;i++)
	{
		usleep(1000);
		
		if(i%10==0)
		
			if(kill(child,SIGUSR1)<0)
			{
				printf("Trimitere semnal SIGUSR1 catre fiu");
				exit(2);
			}

		
	}

}

int main()
{
	struct sigaction action_ignore;
	memset(&action_ignore,0x00,sizeof(struct sigaction));
	action_ignore.sa_handler=SIG_IGN;

	if(sigaction(SIGUSR1,&action_ignore,NULL)<0)
	{
		perror("Eroare");
		exit(-1);
	}
	if(sigaction(SIGUSR2,&action_ignore,NULL)<0)
	{
		perror("Eroare");
		exit(-1);
	}

	if((child=fork())<0)
	{
		printf("Eroare la creerea processchild\n");
		exit(1);
	}
	else
		if(child==0)
		{	
			
			processchild();
			exit(0);
		}
		else
		{
			processtata();
		}




	return 0;
}