#include <stdio.h>
#include "url.h"

int main(int argc,char *argv[]) {
  int len;
  char *out;
  if(argc < 2) return 1;
  len=urlescapelength(argv[1],strlen(argv[1]));
  out=malloc(len+1);
  urlescape(argv[1],out,len);
  out[len]=0;
  printf("%s\n",out);
  return 0;
}
