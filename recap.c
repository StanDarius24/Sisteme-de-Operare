#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

//2 argumente fisier de citit, fisier de scris
//se va scrie textul pe dos din primul in al 2-lea
//o sa sara la inceput si o sa puna o semnatura
#define BUFFSIZE 4096

void usage(char *s)
{
	printf("Eroare la %s\n",s);
}

int main(int argc, char *argv[])
{
int fd1,fd2;
char *p=malloc(sizeof(char)*100);
char *buf=malloc(sizeof(char)*BUFFSIZE);
char *w=malloc(sizeof(char)*7);
if(argc!=3)
{
	snprintf(p,99,"argumente %s",argv[0]);
	usage(p);
	exit(1);
}

if((fd1=open(argv[1],O_RDONLY))<0)
{
	snprintf(p,99,"deschiderea fisier %s",argv[1]);
	usage(p);
	exit(2);
}

if((fd2=open(argv[2],O_WRONLY | O_CREAT | O_EXCL,S_IRWXU))<0)
{
	snprintf(p,99,"deschiderea fisier %s",argv[2]);
	usage(p);
	exit(3);
}
int val;
char c;
while((val=read(fd1,buf,BUFFSIZE))>0)
{
for(int i=0;i<=val/2;i++)
{
	c=buf[i];
	buf[i]=buf[val-i];
	buf[val-i]=c;
}
if(write(fd2,buf,val)<0)
{

	usage("scriere in fisier");
	exit(4);
}
if(lseek(fd2,0,SEEK_SET)<0)
{
	usage("pozitionare");
	exit(5);
}

strcpy(w,"darius");
if(write(fd2,w,7)<0)
{
	usage("scrierea a 2-a");
	exit(6);
}}

if(val<0)
{
	usage("citire fisier");
	exit(5);
}

close(fd2);
close(fd1);
free(p);
free(buf);
free(w);
return 0;
}