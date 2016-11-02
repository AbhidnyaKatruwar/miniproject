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
