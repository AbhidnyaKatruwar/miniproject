project:abhi.o fclose.o fprintf.o file.o fscanf.o fwrite.o  fseek.o files.o fread.o freopen.o fdopen.o fgets.o 
	cc abhi.o fclose.o file.o fprintf.o fscanf.o fwrite.o fseek.o files.o fread.o freopen.o fdopen.o fgets.o -o project
abhi.o:abhi.c file.h
	cc -c -Wall abhi.c
fclose.o:fclose.c file.h
	cc -c -Wall fclose.c
file.o:file.h file.c
	cc -c -Wall file.c
fscanf.o:fscanf.c file.h
	cc -c -Wall fscanf.c
fwrite.o:fwrite.c file.h
	cc -c -Wall fwrite.c
fseek.o:fseek.c file.h
	cc -c -Wall fseek.c
files.o:files.c file.h
	cc -c -Wall files.c
fread.o:fread.c file.h
	cc -c -Wall fread.c
freopen.o:freopen.c file.h
	cc -c -Wall freopen.c
fdopen.o:fdopen.c file.h
	cc -c -Wall fdopen.c
fgets.o:fgets.c file.h
	cc -c -Wall fgets.c
fprintf.o: fprintf.c file.h
	cc -c -Wall fprintf.c
clean:
	rm *.o
