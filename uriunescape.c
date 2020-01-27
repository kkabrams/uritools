#include "uri.h"
#include <unistd.h>
#include <stdio.h>

#define BUFSIZE 4096

int main(int argc,char *argv[]) {
  int i=0,j=0;
  int len;
  char buf[BUFSIZE];
  char buf2[BUFSIZE];
  int blen;
  int hack;
  if(argc > 1) {
    for(argv++,argc--;argc;argc--,argv++) {
      len=uriunescape(*argv,*argv);
      write(1,*argv,len);
      if(argc-1) write(1," ",1);
    }
  } else {
    while((blen=read(0,buf+hack,BUFSIZE-hack-1)) > 0) {
      buf[blen+hack]=0;
      blen=strlen(buf);
      hack=0;
      for(i=0;i<2;i++) {//2 being max length of a %XX thing. and we're starting at the last.
        if(buf[blen-i-1] == '%') {//we're looping from the end first...
          buf[blen-i-1] = 0;//zero out the %
          hack=i+1;
          break;
        }
      }
      len=uriunescape(buf,buf2);//uriunescape wants null terminated strings
      write(1,buf2,len);
      if(hack) {
        buf[0]='%';
        for(j=0;j<hack;j++) {//move the entity to the start of the string
          buf[j+1]=buf[blen-(hack-1-j)];
        }
      }
    }
  }
  return 0;
}
