CFLAGS=-std=c99 -pedantic -Wall
PREFIX=/usr/local
CC=gcc

all:
	$(CC) -Wall -o matchurl matchurl.c url.c
	$(CC) -Wall -o cuturl cuturl.c url.c

clean:
	rm -f matchurl
	rm -f cuturl

install:
	cp -f matchurl $(PREFIX)/bin/matchurl
	cp -f cuturl $(PREFIX)/bin/cuturl
