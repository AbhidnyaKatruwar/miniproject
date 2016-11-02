/*****************************************************************************
 * Copyright (C) Abhidnya R. K.  abhidnyark008@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

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
			oflags = O_CREAT;
			break;	
		case 'a':/*open for appending*/
			sflag = _SWR;
			flags = O_WRONLY;
			oflags = O_CREAT | O_APPEND;
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
	
	if ((fd = open(file, flags | oflags, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)) < 0) {
		fp -> flag = 0;	/* release */
		perror("error in opening file:\n");
		return (NULL);
	}
	if (oflags & O_APPEND) 
		lseek(fd,0,SEEK_END);
	fp -> flag = sflag;
	fp -> _w = 0;
	fp -> ptr = fp -> base = NULL; 
	fp -> fd = fd;
	fp -> _r = 0;
	return (fp);
}
int fillbuf(_FILE *fp){
	int bufsize;
	if((fp -> flag & (_SRD | _SWR | _SRW)) == 0)
		return EOF;

	bufsize =(fp-> flag & _SNBF) ? 1 : BUF;
	if(bufsize == 1){
		fp -> flag |= _SNBF;
		fp -> bufsiz = 1;
	}
	else{
		fp -> flag |= _SOWNBUF;
		fp -> bufsiz = BUF;
	}	
	if(fp -> base == NULL){
		fp -> base = (char *)malloc(bufsize);
		if(fp -> base == NULL)
			return EOF;
	}
	fp -> ptr = fp -> base;
	fp -> _r = read(fp -> fd,fp -> ptr,bufsize);
	
	//fp -> base = fp -> ptr;
	if(--fp -> _r < 0){
		if(fp -> _r == -1)
			fp -> flag |= _SEOF;
		else
			fp ->flag |= _SERR;
	fp -> _r =0;
	return EOF;
	}
	return (unsigned char) *fp -> ptr++;
}
void getbuf(_FILE *stream) {
  // Try to get a buffer
  if ((stream -> base = malloc(BUF)) != NULL) {
    // Got a buffer
    stream->flag |= _SOWNBUF;
    stream->bufsiz = BUF;
  } else {
    // Did NOT get a buffer - use single char buffering.
    stream -> flag |= _SNBF;
    stream -> base = (char *)malloc(1);
    stream -> bufsiz = 1;
  }

  stream -> ptr = stream -> base;
  stream -> _r = 0;
}

