CFLAGS=-std=c99 -pedantic -Wall
PREFIX=/usr/local
CC=gcc

all: cuturl matchurl

matchurl: matchurl.c url.h

cuturl: cuturl.c url.h

clean:
	rm -f matchurl
	rm -f cuturl
	rm -f *.o

install: all
	install matchurl $(PREFIX)/bin/matchurl
	install cuturl $(PREFIX)/bin/cuturl
	install start $(PREFIX)/bin/start
	install printfurl $(PREFIX)/bin/printfurl
