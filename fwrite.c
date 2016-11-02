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

int write_translated(int fh, char *buf, int len) {
	char *ptr = buf;
	char *end = buf + len;
	int written = 0;
	int rc;

	while (ptr < end) {
		if (*ptr == '\n') {
			if (buf < ptr) {
				rc = write(fh, buf, ptr - buf);
				if (rc < 0)
					 return rc;
				written += rc;
			}

			rc = write(fh, "\r", 1);
			if (rc < 0)
				 return -1;

			buf = ptr;
		}
		ptr++;
	}

	if (buf < end) {
		rc = write(fh, buf, end - buf);
			if (rc < 0) 
				return rc;
			written += rc;
	}

	return written;
}

size_t f_write(void *buffer, size_t size, size_t num, _FILE *stream) {
	char *data;               // Point to where data comes from next
	unsigned total;                 // Total bytes to write
	unsigned count;                 // Num bytes left to write
	unsigned bufsize;               // Size of stream buffer
	unsigned nbytes;                // Number of bytes to write now
	unsigned nwritten;              // Number of bytes written

	// Initialize local vars
	data = (char*)buffer;
	count = total = size * num;
	if (count == 0) 
		return 0;

	if ((stream -> flag & _SOWNBUF ) == 1) {
	// Already has buffer, use its size
		bufsize = stream -> bufsiz;
	} else {
	// Assume will get BUFSIZ buffer
		bufsize = BUF;
	}

	  // Here is the main loop -- we go through here until we're done
	while (count != 0) {
	// If the buffer is big and has room, copy data to buffer
		if ((stream -> flag & _SOWNBUF) && stream -> _r != 0) {
	// How much do we want?
			nbytes = (count < (unsigned) stream -> _r) ? count : stream -> _r;
			memcpy(stream -> ptr, data, nbytes);

	// Update stream and amount of data written
			count -= nbytes;
			stream -> _r -= nbytes;
			stream -> ptr += nbytes;
			data += nbytes;
		} else if (count >= bufsize) {
	// If we have more than bufsize chars to write, write
	// data by calling write with an integral number of
	// bufsiz blocks.  If we reach here and we have a big
	// buffer, it must be full so flush it.
			if (stream -> flag & _SOWNBUF) {
				if (f_flush(stream)) {
			// Error, stream flags set -- we're out of here
					return (total - count) / size;
				}
			}
	
		// Calc chars to write -- (count / bufsize) * bufsize
			nbytes = bufsize ? (count - count % bufsize) : count;
			nwritten = write_translated(stream -> fd, (char *) data, nbytes);
     

			if ((int) nwritten < 0) {
			// Error -- out of here
				stream -> flag |= _SERR;
				return (total - count) / size;
			}
			// Update count and data to reflect write
			count -= nwritten;
			data += nwritten;

			if (nwritten < nbytes) {
			// Error -- out of here
				stream -> flag |= _SERR;
				return (total - count) / size;
			}
			} else {
			// Buffer full and not enough chars to do direct write, so do a flsbuf. 
				if (f_flush( stream) == EOF) {
			// Error or eof, stream flags set by _flsbuf
					return (total - count) / size;
				}
			// Update buffer size
				//data++;
				count--;
				bufsize = stream -> bufsiz > 0 ? stream->bufsiz : 1;
			}
		}
// We finished successfully, so just return num
return nwritten;
}
