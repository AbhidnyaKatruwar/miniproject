#include<stdio.h>
#include "file.h"
int main()
{
	char c[60] = "you only live once\nbut if u do it right,once is enough. ";
	char d[10],v[10];
	int fd,ch;
	_FILE *fi;
	fi = f_open("abhidnya","w+");
	f_write(c,1,55,fi);
	f_puts("\ngood luck ",fi);
	f_seek(fi,0,SEEK_SET);
	f_read(d,1,10,fi);
	printf("fread : %s\n",d);
	f_seek(fi,0,SEEK_SET);
	f_scanf(fi,"%s",v);
	printf("fscanf : %s\n",v);
	f_seek(fi,0,SEEK_SET);
	f_scanf(fi,"%[\n]",v);
	printf("fscanf with format [new line]: %s\n",v);
	f_seek(fi,0,SEEK_SET);
	f_scanf(fi,"%[^\n]",v);
	printf("fscanf with format [tab new line]: %s\n",v);
	fd = _fileno(fi);
	printf("file discriptor : %d \n",fd);	
	f_seek(fi,0,SEEK_END);
	f_putc((int)d[0],fi);
	f_seek(fi,-1,SEEK_CUR);
	ch = f_getc(fi);
	printf("fgetc : %c\n",(char)ch);
	f_close(fi);
}
