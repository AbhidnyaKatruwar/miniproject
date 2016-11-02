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

