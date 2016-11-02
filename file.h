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
#include <string.h>
#include <errno.h>
#include <unistd.h>

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

#define T_UNUSED	0
#define T_SHORT		1
#define T_U_SHORT	2
#define TP_SHORT	3
#define T_INT		4
#define T_U_INT		5
#define TP_INT		6
#define T_LONG		7
#define T_U_LONG	8
#define TP_LONG		9
#define T_QUAD		10
#define T_U_QUAD	11
#define TP_QUAD		12
#define T_DOUBLE	13
#define T_LONG_DOUBLE	14
#define TP_CHAR		15
#define TP_VOID		16

#define	DEFPREC		6


#define	BUFF		68
#define STATIC_ARG_TBL_SIZE 8           /* Size of static argument table. */

/*
 * Flags used during conversion.
 */
#define MAXEXPDIG  6
#define	ALT		0x001		/* alternate form */
#define	HEXPREFIX	0x002		/* add 0x or 0X prefix */
#define	LADJUST		0x004		/* left adjustment */
//#define	LONGDBL		0x008		/* long double */
#define	LONGINT		0x010		/* long integer */
#define	QUADINT		0x020		/* quad integer */
#define	SHORTINT	0x040		/* short integer */
#define	ZEROPAD		0x080		/* zero (as opposed to blank) pad */
#define FPT		0x100		/* Floating point number */

#define _SRD 0x0004
#define _SWR 0x0008
#define _SRW 0x0010
#define _SERR 0x0040
#define _SLBF 0x0001
#define _SNBF 0x0002
#define _SEOF 0x0020
#define _SOWNBUF 0x0030
#define _STMPBUF 0x0040
#define _SEXTBUF 0x0050

#define BUF 1024

#define QUAD_MAX 56
#define MIN(a ,b)  ((a) < (b) ? (a) : (b))
#define COPY(n) (void)memcpy((void *)fp -> ptr,(void *)p,(size_t)(n))

/*
 * Macros for converting digits to letters and vice versa
 */
#define	to_digit(c)	((c) - '0')
#define is_digit(c)	((unsigned)to_digit(c) <= 9)
#define	to_char(n)	((n) + '0')

typedef struct file{
	char *ptr;
	int cnt;
	char *base;
	int flag;
	int fd;
	int _w;
	int _r;
  	int charbuf;
  	int bufsiz;
  	int phndl;
}_FILE;

typedef struct siov{
	char * iov_base;
	size_t iov_len;
}__siov;


typedef struct suio{
	int uio_resid;
	int uio_iovcnt;
	struct __siov *uio_iov;
}__suio;



int	__sprint(_FILE *,__suio *);
int	__sbprintf (_FILE *, char *, va_list);
char *	__ultoa(u_long, char *, int, int, char *);
char *	__uqtoa(u_quad_t, char *, int, int, char *);
void	__find_arguments (char *, va_list, void ***);
void	__grow_type_table __P((int, unsigned char **, int *));

int f_printf(_FILE *stream,char *fmt, ...);
int __sfvwrite(_FILE *fp,__suio *uio);
int vf_printf(_FILE *fp,char *fmt0,va_list ap);
int _getc(_FILE *); 
int _putc(int ,_FILE *); 
_FILE *f_open(char *, char *);
size_t f_write(void *, size_t , size_t , _FILE *);
void getbuf(_FILE *);
int fillbuf(_FILE *);
int f_eof(_FILE *);
int f_error(_FILE *);
int _fileno(_FILE *);
int f_getc(_FILE *);
int f_putc(int ,_FILE *);
int stbuf(_FILE *, char *, int );
void ftbuf(_FILE *);
int f_puts(char *, _FILE *);
int f_scanf(_FILE *,char *,...);
int vf_scanf(_FILE *,va_list  ,char *);
int parsefloat(FILE *, char *, char *);
char *f_gets(char *, size_t , _FILE *);
_FILE *fd_open(int , const char *);
_FILE *f_reopen(char *, char *, _FILE *);
int f_seek(_FILE *, long , int );
u_char *__sccl(char *, char *);
int _ungetc(int , _FILE *);
size_t f_read(void *, size_t , size_t , _FILE *);
int f_close(_FILE *);
int f_flush(_FILE *);
