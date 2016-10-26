#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "file.h"
size_t f_read(void *buffer, size_t size, size_t num, _FILE *stream) {
  char *data;                     // Point to where should be read next
  size_t total;                 // Total bytes to read
  size_t count;                 // Num bytes left to read
  unsigned bufsize;               // Size of stream buffer
  size_t nbytes;                // How much to read now
  size_t nread;                 // How much we did read
  int c,i;                          // A temp char

  if ((count = total = size * num) == 0) 
		return 0;
  data = (char *)buffer ;
  data = (char *)malloc(total);


  // Here is the main loop -- we go through here until we're done
  while (count != 0) {
    // if the buffer exists and has characters, copy them to user buffer
    /*if (anybuf(stream) && stream->cnt != 0) {
      // How much do we want?
      nbytes = (count < (unsigned) stream->cnt) ? count : stream->cnt;
      memcpy(data, stream->ptr, nbytes);

      // Update stream and amount of data read
      count -= nbytes;
      stream->cnt -= nbytes;
      stream->ptr += nbytes;
      data += nbytes;
    } else*/ 
	if (count >= bufsize) {
		
      		nread = read(stream -> fd, data, nbytes);
		
		printf("prog \n");
      		if (nread == 0) {
        	// End of file -- out of here
        		stream->flag = _SEOF;
       			 return (total - count) / size;
     		 } 
		else if ((int) nread < 0) {
	        // Error -- out of here
		        stream->flag = _SERR;
		        return (total - count) / size;
	      	}
		else if(nread < count){
			stream->flag = _SEOF;
       			 return (nread);
		}
	      // Update count and data to reflect read
	      count -= nread;
	      data += nread;
      }
      else {
      // Less than bufsize chars to read, so call filbuf to fill buffer
     	 if ((c = fillbuf(stream)) == EOF) {
        // Error or eof, stream flags set by filbuf
        return (total - count) / size;
      }

      // filbuf returned a char -- store it
      *data++ = (char) c;
      count--;

      // Update buffer size
      bufsize = stream->bufsiz;
    }
  }

  // We finished successfully, so just return no. of read characters
  return nread;
}



