#include "uri.h"
#include <stdio.h>

int main(int argc,char *argv[]) {
  int i;
  int ret;
  struct uri *a=malloc(sizeof(struct uri));
  struct uri *b=malloc(sizeof(struct uri));
  if(argc < 3) {
    fprintf(stderr,"usage: uricmp uri1 uri2\n");//we didn't ask for usage so it goes to stderr
    return 1;
  }
  urifromline(a,argv[1]);
  urifromline(b,argv[2]);
  for(i=0;i<8;i++) {
    printf("%s ? %s\n",a->A[i],b->A[i]);
  }
  printf("%08x\n",ret=uricmp(a,b));
  return ret > 0 ? 2 : 0;
}
