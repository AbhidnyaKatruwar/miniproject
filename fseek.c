#include<stdio.h>
#include<errno.h>
#include <sys/types.h>
#include <unistd.h>
#include"file.h"


int f_seek(_FILE *stream, long offset, int whence) {
	if (whence != SEEK_SET && whence != SEEK_CUR && whence != SEEK_END) {
		errno = EINVAL;
    		return -1;
  	}

 
  	stream -> flag &= ~_SEOF;
  
  
 	 if (whence == SEEK_CUR && (stream -> flag & _SRD)) 
		offset -= stream -> cnt;
  
  
 	 f_flush(stream);

 	 if (stream->flag & _SRW) 
		stream->flag &= ~(_SWR | _SRD);

 	 // Seek to the desired location and return
return lseek(stream -> fd, offset, whence) < 0 ? -1 : 0;
}
