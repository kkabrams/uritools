#include "uri.h"
#include <unistd.h>
#include <stdio.h>

int main(int argc,char *argv[]) {
  int i;
  int len;

  char doh[2];
  int hack=0;

  char buf[16];
  char buf2[16];
  if(argc > 1) {
   for(argv++,argc--;argc;argc--,argv++) {
    len=uriunescape(*argv,*argv);
    write(1,*argv,len);
    if(argc-1) write(1," ",1);
   }
  } else {
   while(fgets(buf+hack,sizeof(buf)-1-hack,stdin)) {//this is a bad idea. first chunk could end inside of a sequence we could decode. [first %] [09 second]
    hack=0;
    for(i=0;i<2;i++) {//2 being max length of a %XX thing.
     if(buf[strlen(buf)-1-i] == '%') {
      for(j=0;j<i;j++) {
        doh[j] = buf[strlen(buf)-1-j];//save it for when we need to fix it back.
      }
      buf[strlen(buf)-1-i] = 0;//zero out the %
      hack=2;
      len=uriunescape(buf,buf2);
      write(1,buf2,len);
     //we end with a %[single char]
      buf[0]='%';
      buf[1]=doh;
     }
    }
    if(!hack) {//we're good.
      len=uriunescape(buf,buf2);
      write(1,buf2,len);
    }
   }
  }
  return 0;
}
