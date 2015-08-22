CFLAGS=-std=c99 -pedantic -Wall
PREFIX=/usr/local
CC=gcc

all:
	$(CC) -o matchurl matchurl.c url.c
	$(CC) -o cuturl cuturl.c url.c

clean:
	rm -f matchurl
	rm -f cuturl

install:
	cp -f matchurl $(PREFIX)/bin/matchurl
	cp -f cuturl $(PREFIX)/bin/cuturl
