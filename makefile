project: file.o fclose.o fread.o fscanf.o ftell.o  fdopen.o fgets.o files.o freopen.o fseek.o fwrite.o abhi.o
	cc file.o fclose.o fread.o fscanf.o ftell.o fdopen.o fgets.o files.o freopen.o fseek.o fwrite.o abhi.o -o project

file.o:file.h file.c
	cc -c file.c
fclose.o : fclose.c file.h
	cc -c fclose.c
fread.o: fread.c file.h
	cc -c fread.c
fscanf.o:fscanf.c file.h
	cc -c fscanf.c
ftell.o: ftell.c file.h
	cc -c ftell.c
fdopen.o:fdopen.c file.h
	cc -c fdopen.c
fgets.o: fgets.c file.h
	cc -c fgets.c
files.o:files.c file.h
	cc -c files.c
freopen.o:freopen.c file.h
	cc -c freopen.c
fseek.o:fseek.c file.h
	cc -c fseek.c
fwrite.o:fwrite.c file.h
	cc -c fwrite.c
abhi.o:abhi.c file.h
	cc -c abhi.c
