CFLAGS=-std=c99 -pedantic -Wall
PREFIX=/usr/local
CC=gcc

all: cuturl matchurl linturl

matchurl: matchurl.c url.h

cuturl: cuturl.c url.h

linturl: linturl.c url.h

clean:
	rm -f matchurl
	rm -f cuturl
	rm -f linturl
	rm -f *.o

install: all
	cp -f matchurl $(PREFIX)/bin/matchurl
	cp -f cuturl $(PREFIX)/bin/cuturl
	cp -f linturl $(PREFIX)/bin/linturl
