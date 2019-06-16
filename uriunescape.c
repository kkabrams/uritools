#include "uri.h"
#include <unistd.h>

int main(int argc,char *argv[]) {
  int n;
  int len;
  char buf[4096];
  if(argc > 1) {
   for(argv++,argc--;argc;argc--,argv++) {
    len=uriunescape(*argv,*argv);
    write(1,*argv,len);
    if(argc-1) write(1," ",1);
   }
  } else {
   while((n=read(0,buf,sizeof(buf)-1)) > 0) {
    buf[n]=0;
    len=uriunescape(buf,buf);
    write(1,buf,len);
   }
  }
  return 0;
}
