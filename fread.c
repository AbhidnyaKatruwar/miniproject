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


size_t f_read(void *buffer, size_t size, size_t num, _FILE *stream) {
  char *data;                     // Point to where should be read next
  unsigned total;                 // Total bytes to read
  unsigned count;                 // Num bytes left to read
  unsigned bufsize;               // Size of stream buffer
  unsigned nbytes;                // How much to read now
  unsigned nread;                 // How much we did read
  int c;                          // A temp char

  // Initialize local variables
  data = buffer;

	if ((count = total = size * num) == 0) 
		return 0;
	if ((stream -> flag & _SOWNBUF) == 1 ) {
// Already has buffer, use its size
		bufsize = stream -> bufsiz;
	} else {
// Assume will get BUFSIZ buffer
	bufsize = BUF;
	}

 // Here is the main loop -- we go through here until we're done
	while (count != 0) {
	// if the buffer exists and has characters, copy them to user buffer
		if (stream -> _r != 0) {
		// How much do we want?
			nbytes = (count < (unsigned) stream -> _r) ? count : stream -> _r;
			memcpy(data, stream -> ptr, nbytes);
		// Update stream and amount of data read
			count -= nbytes;
			stream -> _r -= nbytes;
			stream -> ptr += nbytes;
			data += nbytes;
		} else if (count >= bufsize) {
			nbytes = bufsize ? count - count % bufsize : count;
			nread = read(stream -> fd, data, nbytes);
			if (nread == 0) {
		// End of file -- out of here
				stream -> flag |= _SEOF;
				return (total - count) / size;
			} else if ((int) nread < 0) {
			// Error -- out of here
				stream -> flag |= _SERR;
				return (total - count) / size;
			}
	
		// Update count and data to reflect read
			count -= nread;
			data += nread;
		} else {
		// Less than bufsize chars to read, so call filbuf to fill buffer
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
	// We finished successfully, so just return num
  	return	nread;
}		
