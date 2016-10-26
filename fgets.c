
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
	unsigned char *p, *t;
	

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
