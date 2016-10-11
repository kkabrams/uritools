CFLAGS=-std=c99 -pedantic -Wall
PREFIX=/usr/local
CC=gcc

all: cuturl matchurl

matchurl: matchurl.c url.o

cuturl: cuturl.c url.o

clean:
	rm -f matchurl
	rm -f cuturl

install: all
	cp -f matchurl $(PREFIX)/bin/matchurl
	cp -f cuturl $(PREFIX)/bin/cuturl
