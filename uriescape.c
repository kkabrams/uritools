#include "uri.h"
#include <stdio.h>

int main(int argc,char *argv[]) {
  int len;
  char *out;
  if(argc < 2) return 1;
  len=uriescapelength(argv[1],strlen(argv[1]));
  out=malloc(len+1);
  uriescape(argv[1],out,len);
  out[len]=0;
  printf("%s\n",out);
  return 0;
}
