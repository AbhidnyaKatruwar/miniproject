#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include "file.h"

_FILE *f_open(char *file,char *mode){
	_FILE *fp;
	fp =malloc(sizeof(_FILE));
	int fd;
	int sflag ,flags, oflags;
	 sflag = flags = oflags = 0;	

	switch(*mode++){
		case 'r':/*open for reading*/
			sflag = _SRD;
			flags = O_RDONLY;
			oflags = 0;
			break;	
		case 'w':/*open for writing*/
			sflag = _SWR;
			flags = O_WRONLY;
			oflags = O_CREAT| S_IRUSR | S_IWUSR ;
			break;	
		case 'a':/*open for appending*/
			sflag = _SWR;
			flags = O_WRONLY;
			oflags = O_CREAT | O_APPEND| S_IRUSR | S_IWUSR;
			break;	
		default:/*invalid*/
			errno = EINVAL;
			flags = 0;
			return NULL;			
			break;	
	}
	if(sflag == 0)
	 	return NULL;
	if(*mode == '+' || (*mode == 'b' && mode[1] == '+')){
		sflag = _SRW;
		flags = O_RDWR;
	}
	
	if ((fd = open(file, flags | oflags)) < 0) {
		fp->flag = 0;	/* release */
		perror("error in opening file:\n");
		return (NULL);
	}
	if (oflags & O_APPEND) 
		lseek(fd,0,SEEK_END);
	/*if (sflag & (_SRD | _SWR) ) 
		fp -> bufsiz = sizeof(file);*/
	fp -> flag = sflag;
	fp -> _w = 0;
	fp ->ptr = fp -> base = NULL; 
	fp -> fd = fd;
	fp -> _r = 0;
	return (fp);
}
int fillbuf(_FILE *fp){
	int bufsize,i;
	if((fp -> flag &(_SRD | _SWR | _SRW)) == 0)
		return EOF;

	bufsize =(fp-> flag & _SNBF) ? 1 : BUF;
	if(fp -> base == NULL){
		fp -> base = (char *)malloc(bufsize);
		if(fp -> base == NULL)
			return EOF;
	}
	fp -> ptr = fp -> base;
	fp -> _r = read(fp -> fd,fp -> ptr,bufsize);
	for(i = 0; i < fp -> _r ; i++)
		printf("%c ",*fp -> base++);
	fp -> base = fp -> ptr;
	if(--fp -> _r < 0){
		if(fp -> _r == -1)
			fp -> flag |= _SEOF;
		else
			fp ->flag |= _SERR;
	fp -> _r =0;
	return EOF;
	}
	return (unsigned char) *fp -> ptr;
}
