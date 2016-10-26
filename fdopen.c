#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include "file.h"

_FILE *fd_open(int fd, const char *mode) {
	_FILE *stream; 
  	int streamflag;

  	switch (*mode) {
    		case 'r':
		      streamflag = _SRD;
      			break;

    		case 'w':
      			streamflag = _SWR;
    			 break;

    		case 'a':
      			streamflag = _SWR;
      			break;
  
    		default:
      			errno = EINVAL;
     			return NULL;
  	}

  	while (*++mode) {
    		switch (*mode) {
      			case '+':
       			 	streamflag |= _SRW;
        			streamflag &= ~(_SRD | _SWR);
        			break;
    		}
  	}

 	stream = malloc(sizeof(_FILE));
 	if (!stream) {
    		errno = ENFILE;
    		return NULL;
  	}

	stream -> flag = streamflag;
	stream -> cnt = 0;
  	stream -> base = stream -> ptr = NULL;
  	stream -> fd = fd;
  
  return stream;
}

