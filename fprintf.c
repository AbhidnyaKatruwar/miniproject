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
#include <stdio.h>
#include<inttypes.h>
#include<limits.h>
#if __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#include <math.h>
#endif
#include"file.h"

int f_printf(_FILE *stream,char *fmt, ...){
	int ret;
	va_list ap;
	va_start(ap, fmt);
	ret = vf_printf(stream, fmt, ap);
	va_end(ap);
	return (ret);
}

int __sprint(_FILE *fp, __suio *uio){
	 int err;

	if (uio -> uio_resid == 0) {
		uio -> uio_iovcnt = 0;
		return (0);
	}
	err = __sfvwrite(fp, uio);
	uio -> uio_resid = 0;
	uio -> uio_iovcnt = 0;
	return (err);
}

int __sfvwrite(_FILE *fp,__suio *uio){
	size_t len;
	char *p;
	__siov *iov;
	int w,s;
	char *nl;
	int nlknown,nldist;

	if((len = uio -> uio_resid) == 0)
		return (0);
	if(!(fp -> flag & (_SWR |_SRW)))
		return EOF;
	iov = uio -> uio_iov;
	p = iov -> iov_base;
	len = iov -> iov_len;
	iov++;
#define GETIOV(extra_work)\
	while(len == 0){\
		extra_work;\
		p = iov -> iov_base;\
		len = iov -> iov_len;\
		iov++;\
	}

	if(fp -> flag & _SNBF)
		getbuf(fp);
	else{
		nlknown = 0;
		nldist = 0;
		do{
			GETIOV(nlknown = 0)
			if(!nlknown){
				nl = memchr((void *)p,'\n',len);
				nldist = nl ? nl + 1 - p : len + 1;
				nlknown = 1;
			}
			s = MIN(len,nldist);
			w = fp -> _w + fp -> bufsiz;
			if(fp -> ptr > fp ->  base && s > w){
				COPY(w);
				fp -> ptr += w;
				if(f_flush(fp))
					goto err;
			}
			else if(s >= (w = fp -> bufsiz)){
				//w = (*fp -> _w)(fp,p,w);
				if(w <= 0)
					goto err;
			}
			else {
				w = s;
				COPY(w);
				fp -> _w -= w;
				fp -> ptr += w;
			}
			if((nldist -= w) == 0){
				if(f_flush(fp))
					goto err;
				nlknown = 0;
			}
			p += w;
			len -= w;
		}while((uio -> uio_resid -= w) != 0);
	}
	return 0;
err:
	fp -> flag |= _SERR;
	return(EOF);
}

int __sbprintf(_FILE *fp,char *fmt,va_list ap){
	int ret;
	_FILE *fake;
	unsigned char buf[BUFSIZ];

	/* copy the important variables */
	fake -> flag = fp -> flag & ~_SNBF;
	fake -> fd = fp -> fd;
	//fake._cookie = fp->_cookie;
	fake -> _w = fp->_w;

	/* set up the buffer */
	fake -> base = fake ->ptr = buf;
	fake -> bufsiz = fake -> _w = sizeof(buf);
	
	/* do the work, then copy any error status */
	ret = vf_printf(fake, fmt, ap);
	if (ret >= 0 && f_flush(fake))
		ret = EOF;
	if (fake -> flag & _SERR)
		fp -> flag |= _SERR;
	return (ret);
}
char *__ultoa(u_long val,char *endp,int base,int octzero,char *xdigs){
	char *cp = endp;
	long sval;

	/*
	 * Handle the three cases separately, in the hope of getting
	 * better/faster code.
	 */
	switch (base) {
		case 10:
			if (val < 10) {	/* many numbers are 1 digit */
				*--cp = to_char(val);
				return (cp);
			}
		
			if (val > LONG_MAX) {
				*--cp = to_char(val % 10);
				sval = val / 10;
			} else
				sval = val;
			do {
				*--cp = to_char(sval % 10);
				sval /= 10;
			} while (sval != 0);
			break;

		case 8:
			do {
				*--cp = to_char(val & 7);
				val >>= 3;
			} while (val);
			if (octzero && *cp != '0')
				*--cp = '0';
			break;

		case 16:
			do {
				*--cp = xdigs[val & 15];
				val >>= 4;
			} while (val);
			break;

		default:			/* oops */
			abort();
		}
	return (cp);
}
/* Identical to __ultoa, but for quads. */
char *__uqtoa(u_quad_t val, char *endp,int base, int octzero,char *xdigs){
	char *cp = endp;
	quad_t sval;

	/* quick test for small values; __ultoa is typically much faster */
	/* (perhaps instead we should run until small, then call __ultoa?) */
	if (val <= ULONG_MAX)
		return (__ultoa((u_long)val, endp, base, octzero, xdigs));
	switch (base) {
		case 10:
			if (val < 10) {
				*--cp = to_char(val % 10);
				return (cp);
			}
			if (val > QUAD_MAX) {
				*--cp = to_char(val % 10);
				sval = val / 10;
			} else
			sval = val;
			do {
				*--cp = to_char(sval % 10);
				sval /= 10;
			} while (sval != 0);
			break;
	
		case 8:
			do {
				*--cp = to_char(val & 7);
				val >>= 3;
			} while (val);
			if (octzero && *cp != '0')
				*--cp = '0';
			break;

		case 16:
			do {
				*--cp = xdigs[val & 15];
				val >>= 4;
			} while (val);
			break;

		default:
			abort();
		}
	return (cp);
}




int vf_printf(_FILE *fp,char *fmt0,va_list ap){
	char *fmt;	/* format string */
	int ch;	/* character from fmt */
	int n, n2;	/* handy integer (short term usage) */
	char *cp;	/* handy char pointer (short term usage) */
	__siov *iovp;/* for PRINT macro */
	int flags;	/* flags as above */
	int ret;		/* return value accumulator */
	int width;		/* width from format (%8d), or 0 */
	int prec;		/* precision from format (%.3d), or -1 */
	char sign;		/* sign prefix (' ', '+', '-', or \0) */

	u_long	ulval = 0;	/* integer arguments %[diouxX] */
	u_quad_t uqval = 0;	/* %q integers */
	int base;		/* base for [diouxX] conversion */
	int dprec;		/* a copy of prec if [diouxX], 0 otherwise */
	int realsz;		/* field size expanded by dprec, sign, etc */
	int size;		/* size of converted field or string */
	int prsize;             /* max size of printed field */
	char *xdigs = NULL;	/* digits for [xX] conversion */
#define NIOV 8
	__suio uio;	/* output information: summary */
	__siov iov[NIOV];/* ... and individual io vectors */
	char buf[BUFF];		/* space for %c, %[diouxX], %[eEfgG] */
	char ox[2];		/* space for 0x hex-prefix */
        void **argtable;        /* args, built due to positional arg */
        void *statargtable [STATIC_ARG_TBL_SIZE];
        int nextarg;            /* 1-based argument index */
        va_list orgap;          /* original argument pointer */

	/*
	 * Choose PADSIZE to trade efficiency vs. size.  If larger printf
	 * fields occur frequently, increase PADSIZE and make the initialisers
	 * below longer.
	 */
#define	PADSIZE	16		/* pad chunk size */
	static char blanks[PADSIZE] =
	 {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
	static char zeroes[PADSIZE] =
	 {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};

	/*
	 * BEWARE, these `goto error' on error, and PAD uses `n'.
	 */
#define	PRINT(ptr, len) { \
	iovp->iov_base = (ptr); \
	iovp->iov_len = (len); \
	uio.uio_resid += (len); \
	iovp++; \
	if (++uio.uio_iovcnt >= NIOV) { \
		if (__sprint(fp, &uio)) \
			goto error; \
		iovp = iov; \
	} \
}
#define	PAD(count, with) { \
	if ((n = (count)) > 0) { \
		while (n > PADSIZE) { \
			PRINT(with, PADSIZE); \
			n -= PADSIZE; \
		} \
		PRINT(with, n); \
	} \
}
#define	FLUSH() { \
	if (uio.uio_resid && __sprint(fp, &uio)) \
		goto error; \
	uio.uio_iovcnt = 0; \
	iovp = iov; \
}

        /*
         * Get the argument indexed by nextarg.   If the argument table is
         * built, use it to get the argument.  If its not, get the next
         * argument (and arguments must be gotten sequentially).
         */
#define GETARG(type) ((argtable != NULL) ? *((type*)(argtable[nextarg++])) : (nextarg++, va_arg(ap, type)))

	/*
	 * To extend shorts properly, we need both signed and unsigned
	 * argument extraction methods.
	 */
#define	SARG() (flags&LONGINT ? GETARG(long) : flags&SHORTINT ? (long)(short)GETARG(int) : (long)GETARG(int))
#define	UARG() (flags&LONGINT ? GETARG(u_long) : flags&SHORTINT ? (u_long)(u_short)GETARG(int) : (u_long)GETARG(u_int))

        /*
         * Get * arguments, including the form *nn$.  Preserve the nextarg
         * that the argument can be gotten once the type is determined.
         */
#define GETASTER(val) \
        n2 = 0; \
        cp = fmt; \
        while (is_digit(*cp)) { \
                n2 = 10 * n2 + to_digit(*cp); \
                cp++; \
        } \
        if (*cp == '$') { \
            	int hold = nextarg; \
                if (argtable == NULL) { \
                        argtable = statargtable; \
                        __find_arguments (fmt0, orgap, &argtable); \
                } \
                nextarg = n2; \
                val = GETARG (int); \
                nextarg = hold; \
                fmt = ++cp; \
        } else { \
		val = GETARG (int); \
        }
        

	if ((fp -> flag & (_SWR | _SRW)) == 0) {
		/* FUNLOCKFILE(fp); */
		return (EOF);
	}

	/* optimise fprintf(stderr) (and other unbuffered Unix files) */
	if ((fp -> flag & (_SNBF|_SWR|_SRW)) == (_SNBF|_SWR) && fp -> fd >= 0) {
		return (__sbprintf(fp, fmt0, ap));
	}

	fmt = (char *)fmt0;
        argtable = NULL;
        nextarg = 1;
       // orgap = ap;
	uio.uio_iov = iovp = iov;
	uio.uio_resid = 0;
	uio.uio_iovcnt = 0;
	ret = 0;

	/*
	 * Scan the format for conversions (`%' character).
	 */
	for (;;) {
		for (cp = fmt; (ch = *fmt) != '\0' && ch != '%'; fmt++)
			/* void */;
		if ((n = fmt - cp) != 0) {
			if ((unsigned)ret + n > INT_MAX) {
				ret = EOF;
				goto error;
			}
			PRINT(cp, n);
			ret += n;
		}
		if (ch == '\0')
			goto done;
		fmt++;		/* skip over '%' */

		flags = 0;
		dprec = 0;
		width = 0;
		prec = -1;
		sign = '\0';

rflag:		ch = *fmt++;
	switch (ch) {
			case ' ':
			/*
			 * ``If the space and + flags both appear, the space
			 * flag will be ignored.''
			 *	-- ANSI X3J11
			 */
				if (!sign)
					sign = ' ';
				goto rflag;
			case '*':
			/*
			 * ``A negative field width argument is taken as a
			 * - flag followed by a positive field width.''
			 *	-- ANSI X3J11
			 * They don't exclude field widths read from args.
			 */
				GETASTER (width);
				if (width >= 0)
					goto rflag;
				width = -width;
			/* FALLTHROUGH */
			case 'c':
				*(cp = buf) = GETARG(int);
				size = 1;
				sign = '\0';
				break;
			case 'D':
				flags |= LONGINT;
				/*FALLTHROUGH*/
			case 'd':
			case 'i':
				if (flags & QUADINT) {
					uqval = GETARG(quad_t);
					if ((quad_t)uqval < 0) {
						uqval = -uqval;
						sign = '-';
					}
				} else {
					ulval = SARG();
					if ((long)ulval < 0) {
						ulval = -ulval;
						sign = '-';
					}
				}
				base = 10;
				printf("int:\n");
				goto number;
					goto nosign;
nosign:				sign = '\0';
				/*
				 * ``... diouXx conversions ... if a precision is
				 * specified, the 0 flag will be ignored.''
				 *	-- ANSI X3J11
				 */
number:				printf("in number");
				if ((dprec = prec) >= 0)
					flags &= ~ZEROPAD;
	
				/*
				 * ``The result of converting a zero value with an
				 * explicit precision of zero is no characters.''
				 *	-- ANSI X3J11
				 */
				printf("in number");
				cp = buf + BUF;
				if (flags & QUADINT) {
					if (uqval != 0 || prec != 0)
						cp = __uqtoa(uqval, cp, base,
						    flags & ALT, xdigs);
				} else {
					if (ulval != 0 || prec != 0)
						cp = __ultoa(ulval, cp, base,
						    flags & ALT, xdigs);
				}
				size = buf + BUF - cp;
				break;
			case 's':
				if ((cp = GETARG(char *)) == NULL)
					cp = "(null)";
				if (prec >= 0) {
				/*
				 * can't use strlen; can only look for the
				 * NUL in the first `prec' characters, and
				 * strlen() will go further.
				 */
					char *p = memchr(cp, 0, (size_t)prec);
	
					if (p != NULL) {
						size = p - cp;
						if (size > prec)
							size = prec;
					} else
						size = prec;
				} else
					size = strlen(cp);
				sign = '\0';
				break;

			default:
				if (ch == '\0')
					goto done;
				/* pretend it was %c with argument ch */
				cp = buf;
				*cp = ch;
				size = 1;
				sign = '\0';
				break;
		}
		realsz = dprec > size ? dprec : size;
			if (sign)
				realsz++;
			else if (flags & HEXPREFIX)
				realsz += 2;

			prsize = width > realsz ? width : realsz;
			if ((unsigned)ret + prsize > INT_MAX) {
				ret = EOF;
				goto error;
			}
	
			/* right-adjusting blank padding */
			if ((flags & (LADJUST|ZEROPAD)) == 0)
				PAD(width - realsz, blanks);
	
			/* prefix */
			if (sign) {
				PRINT(&sign, 1);
			} else if (flags & HEXPREFIX) {
				ox[0] = '0';
				ox[1] = ch;
				PRINT(ox, 2);
			}
	
			/* right-adjusting zero padding */
			if ((flags & (LADJUST|ZEROPAD)) == ZEROPAD)
				PAD(width - realsz, zeroes);
	
			/* leading zeroes from decimal precision */
			PAD(dprec - size, zeroes);
	
			/* the string or number proper */

		/* left-adjusting padding (always blank) */
		if (flags & LADJUST)
			PAD(width - realsz, blanks);

		/* finally, adjust ret */
		ret += prsize;

		FLUSH();	/* copy out the I/O vectors */
	printf("flush done");	
	}
done:
	FLUSH();
error:
	if (f_error(fp))
		ret = EOF;
	
        if ((argtable != NULL) && (argtable != statargtable))
                free (argtable);
	return (ret);
	/* NOTREACHED */

}
void __find_arguments (char *fmt0, va_list ap, void ***argtable){
	register char *fmt;	/* format string */
	register int ch;	/* character from fmt */
	register int n, n2;	/* handy integer (short term usage) */
	register char *cp;	/* handy char pointer (short term usage) */
	register int flags;	/* flags as above */
	unsigned char *typetable; /* table of types */
	unsigned char stattypetable [STATIC_ARG_TBL_SIZE];
	int tablesize;		/* current size of type table */
	int tablemax;		/* largest used index in table */
	int nextarg;		/* 1-based argument index */
	int width;
	/*
	 * Add an argument type to the table, expanding if necessary.
	 */
#define ADDTYPE(type) \
	((nextarg >= tablesize) ? \
		__grow_type_table(nextarg, &typetable, &tablesize) : 0, \
	typetable[nextarg++] = type, \
	(nextarg > tablemax) ? tablemax = nextarg : 0)

#define	ADDSARG() \
	((flags&LONGINT) ? ADDTYPE(T_LONG) : \
		((flags&SHORTINT) ? ADDTYPE(T_SHORT) : ADDTYPE(T_INT)))

#define	ADDUARG() \
	((flags&LONGINT) ? ADDTYPE(T_U_LONG) : \
		((flags&SHORTINT) ? ADDTYPE(T_U_SHORT) : ADDTYPE(T_U_INT)))

	/*
	 * Add * arguments to the type array.
	 */
#define ADDASTER() \
	n2 = 0; \
	cp = fmt; \
	while (is_digit(*cp)) { \
		n2 = 10 * n2 + to_digit(*cp); \
		cp++; \
	} \
	if (*cp == '$') { \
		int hold = nextarg; \
		nextarg = n2; \
		ADDTYPE (T_INT); \
		nextarg = hold; \
		fmt = ++cp; \
	} else { \
		ADDTYPE (T_INT); \
	}
	fmt = (char *)fmt0;
	typetable = stattypetable;
	tablesize = STATIC_ARG_TBL_SIZE;
	tablemax = 0; 
	nextarg = 1;
	memset (typetable, T_UNUSED, STATIC_ARG_TBL_SIZE);

	/*
	 * Scan the format for conversions (`%' character).
	 */
	for (;;) {
		for (cp = fmt; (ch = *fmt) != '\0' && ch != '%'; fmt++)
			/* void */;
		if (ch == '\0')
			goto done;
		fmt++;		/* skip over '%' */

		flags = 0;
		width = 0;

rflag:		ch = *fmt++;
	switch (ch) {
		case ' ':
		case '#':
			goto rflag;
		case '*':
			ADDASTER ();
			goto rflag;
		case 'd':
		case 'i':
			if (flags & QUADINT) {
				ADDTYPE(T_QUAD);
			} else {
				ADDSARG();
			}
			break;
			break;
		default:	/* "%?" prints ?, unless ? is NUL */
			if (ch == '\0')
				goto done;
			break;
		}
	}
done:
	/*
	 * Build the argument table.
	 */
	if (tablemax >= STATIC_ARG_TBL_SIZE) {
		*argtable = (void **)
		    malloc (sizeof (void *) * (tablemax + 1));
	}

	(*argtable) [0] = NULL;
	for (n = 1; n <= tablemax; n++) {
		switch (typetable [n]) {
		    case T_UNUSED:
			(argtable) [n] =  (void *)va_arg (ap, int);
			break;
		  
		    case T_INT:
			(*argtable) [n] = (void *) va_arg (ap, int);
			break;
		    case T_U_INT:
			(*argtable) [n] = (void *) va_arg (ap, unsigned int);
			break;
		    case TP_INT:
			(*argtable) [n] = (void *) va_arg (ap, int *);
			break;
		     case TP_CHAR:
			(*argtable) [n] = (void *) va_arg (ap, char *);
			break;
		    case TP_VOID:
			(*argtable) [n] = (void *) va_arg (ap, void *);
			break;
		}
	}

	if ((typetable != NULL) && (typetable != stattypetable))
		free (typetable);
}
void __grow_type_table (int nextarg,unsigned char **typetable,int *tablesize)
	
{
	unsigned char *oldtable = *typetable;
	int newsize = *tablesize * 2;

	if (*tablesize == STATIC_ARG_TBL_SIZE) {
		*typetable = (unsigned char *)
		    malloc (sizeof (unsigned char) * newsize);
		bcopy (oldtable, *typetable, *tablesize);
	} else {
		*typetable = (unsigned char *)
		    realloc (typetable, sizeof (unsigned char) * newsize);

	}
	memset (&typetable [*tablesize], T_UNUSED, (newsize - *tablesize));

	*tablesize = newsize;
}

	
