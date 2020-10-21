#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<ctype.h>
#include<sys/syscall.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include<string.h>
#include<unistd.h>
#define sizeo 4096
void printareeroare(char *s)
{
	printf("EROARE LA %s\n",s);
}
  int fd;
void parcurgeredir(char *numedir, int *dim, int nivel,char *fst)
{
DIR *dir;
struct dirent *in;
char *nume;
char cale[100]="",spatii[100];
struct stat info;
memset(spatii,' ',2*nivel);
spatii[2*nivel]='\0';
if(!(dir=opendir(numedir)))
{
	printareeroare("Deschidere director");
	exit(2);
}
else
{
    
  printf("%s%s\n",spatii,numedir);
}

while((in=readdir(dir))>0)
{ 
    nume=in->d_name;
  if(strcmp(nume,".") == 0 || strcmp(nume,"..") == 0)
      continue;

  snprintf(cale,sizeof(cale),"%s/%s",numedir,nume);

  if(lstat(cale,&info)<0)
  {
    printareeroare("la lstat");
    exit(3);
  }

    if(S_ISDIR(info.st_mode))    
      parcurgeredir(cale,dim,nivel+1,fst);
  else
    if(S_ISLNK(info.st_mode))
     {
      snprintf(fst,300,"%s : %d\n",nume,info.st_mode);
      if(write(fd,fst,sizeof(fst))<0)
  {
    printareeroare("la scriere");
    exit(7);
  }
    }
    else
    {
      printf("%s%s\n",spatii,nume);

    }
  
}
strcpy(numedir,"");
closedir(dir);
}


int main(int argc, char *argv[])
{
	int *a;
  int b=0;
  a=&b;
  char *f=malloc(sizeof(char)*300);

  if((fd=creat(argv[2],S_IRWXU)) <0)
  {
    printareeroare("deschidere fisier scriere");
    exit(5);
  }
  struct stat st;
  if(stat(argv[2],&st)<0)
  {
    printareeroare("la stat ");
    exit(6);
  }
	parcurgeredir(argv[1],a,0,f);
  printf("%s\n",f);
  
  close(fd);
  free(f);
	return 0;
}