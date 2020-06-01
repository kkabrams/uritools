#include "uri.h"
#include <stdio.h>

int main(int argc,char *argv[]) {
  int len;
  unsigned char *out;
  if(argc < 2) return 1;
  len=uriescapelength((unsigned char *)argv[1],strlen(argv[1]));
  out=malloc(len+1);
  uriescape((unsigned char *)argv[1],out,strlen(argv[1]));
  out[len]=0;
  printf("%s\n",out);
  return 0;
}
