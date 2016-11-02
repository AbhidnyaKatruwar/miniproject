#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "file.h"

long f_tell(_FILE *stream) {
	  long filepos;
	  if (stream -> _r < 0)
		 stream -> _r = 0;    
	  if ((filepos = tell(stream -> fd)) < 0L)
		 return -1;

	  if (stream -> flag & _SRD) {
		    filepos -= stream -> _r;
	  } else if (stream -> flag & _SWR) {
		    filepos += (stream -> ptr - stream -> base);
	  }

	  return filepos;
}
