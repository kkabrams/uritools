#include <unistd.h>
#include "url.h"

int main(int argc,char *argv[]) {
  int len;
  for(argv++,argc--;argc;argc--,argv++) {
   len=urlunescape(*argv,*argv);
   write(1,*argv,len);
   if(argc-1) write(1," ",1);
  }
  return 0;
}
