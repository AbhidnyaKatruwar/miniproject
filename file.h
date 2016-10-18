#ifndef STDIO_H
#define STDIO_H
#endif

#ifndef SEEK_SET
#define SEEK_SET   0
#define SEEK_CUR   1
#define SEEK_END   2
#endif

#define _SRD 0x0004
#define _SWR 0x0008
#define _SRW 0x0010
#define _SEOF 0x0020
#define _SERR 0x0040
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
 
#define	SIGNOK		0x40	/* +/- is (still) legal */
#define	NDIGITS		0x80	/* no digits detected */
#define	PFXOK		0x100	/* 0x prefix is (still) legal */
#define	NZDIGITS	0x200	/* no zero digits detected */

/*
 * Conversion types.
 */
#define	CT_CHAR		0	/* %c conversion */
#define	CT_CCL		1	/* %[...] conversion */
#define	CT_STRING	2	/* %s conversion */
#define	CT_INT		3	/* %[dioupxX] conversion */
#define	CT_FLOAT	4	/* %[efgEFG] conversion */


//typedef int64_t f_pos_t;

//#define L_tmpnam       256
//#define FILENAME_MAX   256
//#define EOF            (-1)
#ifndef BUFSIZ
#define BUFSIZ         1024
#endif
typedef struct file{
	char *ptr;
	int _r;
	int _w;
	char *base;
	int flag;
	int fd;
	//fpos_t offset;
  	//handle_t file;
  	int charbuf;
  	int bufsiz;
  	int phndl;
}_FILE;


/*
#define stdin   __getstdfile(0)
#define stdout  __getstdfile(1)
#define stderr  __getstdfile(2)

#define _IORD           0x0001
#define _IOWR           0x0002

#define _IOFBF          0x0000
#define _IOLBF          0x0040
#define _IONBF          0x0004

#define _IOOWNBUF       0x0008
#define _IOEXTBUF       0x0100
#define _IOTMPBUF       0x1000

#define _IOEOF          0x0010
#define _IOERR          0x0020
#define _IOSTR          0x0040
#define _IORW           0x0080

#define _IOCRLF         0x8000
#endif
#ifdef  __cplusplus
extern "C" {
#endif*/

#define _getc(p) (--p -> _r >= 0 ? *p -> ptr++ : fillbuf(p))

_FILE *f_open(char *, char *);
int f_scanf(_FILE *,char *,...);
int vf_scanf(_FILE *,va_list ,char *);
int f_close(_FILE *);
static int parsefloat(FILE *, char *, char *);
int fillbuf(_FILE *);

