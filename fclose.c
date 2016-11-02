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


int f_close(_FILE *fp){

	int r;
	if (fp -> flag == 0) {	/* not open! */
		perror("can not close:\n");
		return (EOF);
	}
	r = fp -> flag & (_SWR | _SRD | _SRW) ? f_flush(fp) : 0;
	if(close(fp -> fd) < 0) {	/* not open! */
		perror("can not close:\n");
		return (EOF);
	}
	fp->flag = 0;		/* Release this FILE for reuse. */
	fp->_r = fp->_w = 0;
	/* Mess up if reaccessed. */
	return (r);
}

int f_flush(_FILE *fp){
	char *p;
	int n;
	if((fp -> flag & (_SWR | _SRD | _SRW)) == 0){
		perror ("cann't flush:\n");
		return EOF;
	}
	p = fp -> base;
	if(p == NULL)
		return 0;
	n = fp ->ptr - p;
	fp -> ptr = p;
	fp -> _w = write(fp -> fd,fp -> ptr ,n);
	if(--fp -> _w < 0){	
		if(fp -> _w == -1)
			fp -> flag |= _SEOF;
		else
			fp ->flag |= _SERR;
	fp -> _w =0;
	return EOF;
	}
	return (unsigned char) *fp -> ptr++;
}

