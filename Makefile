CFLAGS:=-std=c11 -pedantic -Wall
PREFIX:=/usr/local
CC:=gcc

all: uricut urimatch uriunescape uriescape uricmp

urimatch: urimatch.c uri.h

uricut: uricut.c uri.h

uricmp: uricmp.c uri.h

uriunescape: uriunescape.c uri.h

uriescape: uriescape.c uri.h

clean:
	rm -f uricut urimatch uriunescape uriescape uricmp
	rm -f *.o

install: all
	install urimatch $(PREFIX)/bin/urimatch
	install uricut $(PREFIX)/bin/uricut
	install uricmp $(PREFIX)/bin/uricmp
	install uristart $(PREFIX)/bin/uristart
	install uriprintf $(PREFIX)/bin/uriprintf
	install uriunescape $(PREFIX)/bin/uriunescape
	install uriescape $(PREFIX)/bin/uriescape
