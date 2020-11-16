CFLAGS:=-std=c11 -pedantic -Wall
PREFIX:=/usr/local
CC:=gcc

all: urimatch uricut urijoin uricmp uriunescape uriescape

urimatch: urimatch.c uri.h

uricut: uricut.c uri.h

urijoin: urijoin.c uri.h

uricmp: uricmp.c uri.h

uriunescape: uriunescape.c uri.h

uriescape: uriescape.c uri.h

clean:
	rm -f uricut urimatch uriunescape uriescape uricmp
	rm -f *.o

install: all
	install -t $(PREFIX)/bin urimatch
	install -t $(PREFIX)/bin uricut
	install -t $(PREFIX)/bin urijoin
	install -t $(PREFIX)/bin uricmp
	install -t $(PREFIX)/bin uristart
	install -t $(PREFIX)/bin uriprintf
	install -t $(PREFIX)/bin uriunescape
	install -t $(PREFIX)/bin uriescape
	install -t $(PREFIX)/bin urilaunch
	install -t $(PREFIX)/bin search
	install -t $(PREFIX)/bin getsrv
	install -t $(PREFIX)/bin urititle
	install -t $(PREFIX)/bin urigetline
	install -t $(PREFIX)/bin copy_start_nevermind.sh
	install -t $(PREFIX)/bin choose
	install -t $(PREFIX)/bin query_param
	install -t $(PREFIX)/bin data_handler
	install -t $(PREFIX)/bin unshorten.sh
