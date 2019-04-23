#define _XOPEN_SOURCE 500
#include <string.h>
#include <stdio.h>
#include "uri.h"

int main(int argc,char *argv[]) {
  struct uri *u=malloc(sizeof(struct uri));
  int i;
  char key[1024];//fuck if I know
  char *value;
  memset(u,0,sizeof(struct uri));
  char *parts[]={"scheme:","username:","password:","domain:","port:","path:","query_string:","fragment_id:"};
  while(fgets(key,sizeof(key),stdin)) {
    if(strchr(key,'\n')) *strchr(key,'\n')=0;
    if((value=strchr(key,' '))) {
      *value=0;
      value++;
//      fprintf(stderr,"read: %s %s\n",key,value);
      for(i=0;i<8;i++) {
        if(!strcmp(key,parts[i])) {
          if(u->A[i]) free(u->A[i]);
          u->A[i]=strdup(value);
          break;
        }
      }
      if(i==8) {
        fprintf(stderr,"urijoin: unknown uri portion: '%s'\n",key);
        //return 1;//bail early instead of pretending it makes sense.
      }
    } else {
      fprintf(stderr,"urijoin: malformed line. missing a space: '%s'\n",key);
    }
  }
  printf("%s\n",linefromuri(u));
  return 0;
}
