#include<stdio.h>
#include"file.h"



int _getc(_FILE *stream){

	 return (--stream -> cnt >= 0 ? (unsigned char) *stream -> ptr++ : fillbuf(stream));

}
int _putc(int x,_FILE *stream){

	 return (--stream -> cnt >= 0 ? *stream -> ptr++ = (x) : f_flush(stream));

}
int f_eof(_FILE *stream){

	return ((stream -> flag & _SEOF) != 0 );
}
	
int f_error(_FILE *stream){

	return ((stream -> flag & _SERR) != 0);
}

int _fileno(_FILE *stream){

	return stream -> fd ;
}

int f_getc(_FILE *stream){
	
	return (_getc(stream));
}
int f_putc(int c,_FILE *stream){
	return (_putc(c,stream);
}


