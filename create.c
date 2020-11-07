#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include <dirent.h>
#include <limits.h>


void usage(char *c)
{
	printf("Eroare la %s\n",c);
}

int main(int argc, char *argv[])
{

	char *text=malloc(sizeof(char)*150);

	strcpy(text,"#include<stdio.h>\n#include<stdlib.h>\n#include<sys/types.h>\n#include<sys/stat.h>\n#include<fcntl.h>\n#include<unistd.h>\n#include<string.h>\n#include<dirent.h>\n#include<limits.h>");

	if(argc!=2)
	{
		usage("Argumente");
		exit(1);

	}

	int fd;

	if((fd=open(argv[1],O_CREAT | O_WRONLY | O_EXCL ,S_IRWXU))<0)
	{
		usage("deschidere fisier");
		exit(2);

	}
	else
	{
		if(write(fd,text,strlen(text))<0)
		{
			usage("la scrierea in fisier");
			exit(3);
		}
	}


close(fd);
free(text);
	return 1;

}