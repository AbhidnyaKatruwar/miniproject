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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "file.h"

/*
 * Read at most n-1 characters from the given file.
 * Stop when a newline has been read, or the count runs out.
 * Return first argument, or NULL if no characters were read.
 */
char *f_gets(char *buf, size_t n, _FILE *fp){
	size_t len;
	char *s;
	char *p, *t;
	

	if (n == 0)		
		return (NULL);

	s = buf;
	n--;			/* leave space for NULL */
	while (n != 0) {
		/*
		 * If the buffer is empty, refill it.
		 */
		if ((len = fp -> _r) <= 0) {
			if (fillbuf(fp) == EOF) {
				/* EOF/error: stop with partial or no line */
					return (NULL);
				break;
			}
			len = fp -> _r;
		}
		p = fp -> ptr;

		/*
		 * Scan through at most n bytes of the current buffer,
		 * looking for '\n'.  If found, copy up to and including
		 * newline, and stop.  Otherwise, copy entire chunk
		 * and loop.
		 */
		if (len > n)
			len = n;
		t = memchr((void *)p, '\n', len);
		if (t != NULL) {
			len = ++t - p;
			fp -> _r -= len;
			fp -> ptr = t;
			(void)memcpy((void *)s, (void *)p, len);
			s[len] = 0;
			return (buf);
		}
		fp -> _r -= len;
		fp -> ptr += len;
		(void)memcpy((void *)s, (void *)p, len);
		s += len;
		n -= len;
	}
	*s = 0;
	return (buf);
}

int f_puts(char *string, _FILE *stream) {
  int len;
  int written;
  len = strlen(string);
  if (stream->flag & _SNBF) {
    char buf[BUFSIZ];
    stbuf(stream, buf, BUFSIZ);
    written = f_write(string, 1, len, stream);
    ftbuf(stream);
  } else 
	{
    written = f_write(string, 1, len, stream);
  }

  return written == len ? 0 : EOF;
}
int stbuf(_FILE *stream, char *buf, int bufsiz) {
  stream -> base = stream->ptr = buf;
  stream -> _r = bufsiz;
  stream -> flag |= (_SWR | _STMPBUF | _SEXTBUF);

  return 0;
}

void ftbuf(_FILE *stream) {
  if (stream->flag & _STMPBUF) {
    f_flush(stream);
    stream -> flag &= ~(_SEXTBUF | _STMPBUF);
    stream -> bufsiz = 0;
    stream -> base = stream -> ptr = NULL;
  }
}

