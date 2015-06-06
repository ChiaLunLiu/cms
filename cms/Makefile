
DEBUG=-DDEBUG
all:
	gcc -Wl,-rpath,/home/bendog/git/cms/lib -O2 main.c cms.c -o cms -I../include ${DEBUG} -lminimsg -lsimclist -L../lib
clean:
	rm -f cms
