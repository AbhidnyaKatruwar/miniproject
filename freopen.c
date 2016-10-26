#include<stdio.h>
#include"file.h"

_FILE *f_reopen(char *filename, char *mode, _FILE *stream) { 
  if (stream -> fd > 0) 
	f_close(stream);
  return f_open(filename, mode) ;
}
  
