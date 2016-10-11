CFLAGS=-std=c99 -pedantic -Wall
PREFIX=/usr/local
CC=gcc

matchurl: matchurl.c
cuturl: cuturl.c

all: matchurl cuturl

clean:
	rm -f matchurl
	rm -f cuturl

install: all
	cp -f matchurl $(PREFIX)/bin/matchurl
	cp -f cuturl $(PREFIX)/bin/cuturl
