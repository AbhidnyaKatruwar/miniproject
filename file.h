#ifndef STDIO_H
#define STDIO_H
#endif

#ifndef SEEK_SET
#define SEEK_SET   0
#define SEEK_CUR   1
#define SEEK_END   2
#endif
#define	CT_CHAR		3	/* %c conversion */
#define	CT_CCL		4	/* %[...] conversion */
#define	CT_STRING	5	/* %s conversion */
#define	CT_INT		6	/* %[dioupxX] conversion */
#define	CT_FLOAT	7	/* %[efgEFG] conversion */

#define	SIGNOK		0x40	/* +/- is (still) legal */
#define	NDIGITS		0x80	/* no digits detected */
#define	PFXOK		0x100	/* 0x prefix is (still) legal */
#define	NZDIGITS	0x200	/* no zero digits detected */
#define	LONG		0x01	/* l: long or double */
#define	LONGDBL		0x02	/* L: long double */
#define	SHORT		0x04	/* h: short */
#define	SUPPRESS	0x08	/* *: suppress assignment */
#define	POINTER		0x10	/* p: void * (as hex) */
#define	NOSKIP		0x20	/* [ or c: do not skip blanks */
#define	LONGLONG	0x400	/* ll: long long (+ deprecated q: quad) */
#define	INTMAXT		0x800	/* j: intmax_t */
#define	PTRDIFFT	0x1000	/* t: ptrdiff_t */
#define	SIZET		0x2000	/* z: size_t */
#define	SHORTSHORT	0x4000	/* hh: char */
#define	UNSIGNED	0x8000	/* %[oupxX] conversions */

#define _SRD 0x0004
#define _SWR 0x0008
#define _SRW 0x0010
#define _SERR 0x0040
#define _SLBF 0x0001
#define _SNBF 0x0002
#define _SEOF 0x0020

#define BUF       1024


typedef struct file{
	char *ptr;
	int cnt;
	char *base;
	int flag;
	int fd;
	int _w;
	int _r;
  	//handle_t file;
  	int charbuf;
  	int bufsiz;
  	int phndl;
}_FILE;

int _getc(_FILE *); 
int _putc(int ,_FILE *); 
_FILE *f_open(char *, char *);
int fillbuf(_FILE *);
int f_eof(_FILE *);
int f_error(_FILE *);
int _fileno(_FILE *);
int f_getc(_FILE *);
int f_putc(int ,_FILE *);
//int f_scanf(_FILE *fi,char *fmt,...);
//int vf_scanf(_FILE *fp,va_list p ,char *fmt);
//int parsefloat(FILE *fp, char *buf, char *end);
char *f_gets(char *, size_t , _FILE *);
_FILE *fd_open(int , const char *);
_FILE *f_reopen(char *, char *, _FILE *);
int f_seek(_FILE *, long , int );
//u_char *__sccl(char *tab, char *fmt);
size_t f_read(void *, size_t , size_t , _FILE *);
int f_close(_FILE *);
int f_flush(_FILE *);
