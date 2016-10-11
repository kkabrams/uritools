#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "url.h"

#define LINE_LENGTH 1024

int match(char negate,char *part,char *arg) {
 if(negate) {
  if(part == 0) return 1;//we found that the part isn't here!
 } else {
  if(part) {
   if(!strcmp(part,arg)) return 1;
  }
 }
 return 0;
}

int main(int argc,char *argv[]) {
 int i;
 int ret=1;
 struct url u;
 char negate=0;
 char *line=malloc(LINE_LENGTH);
 char copy[LINE_LENGTH];
 if(argc < 2) {
  printf("usage: matchurl [-][n][s|u|k|d|D|P|p|q|f] [string]\n");
  printf("scheme://username:password@domain:port/path?query_string#fragment_id\n");
  printf("s://u:k@d:P/p?q#f\n");
  printf("The D flag is special. it matches its argument against the last bytes of the input url's domain.\n");
  printf("This allows matching of subdomains, like `echo epoch.ano | matchurl -D ano` would match.\n");
  printf("the 'n' flag can be put before any of the other flags to check for a missing.\n");
  return 1;
 }
 while(fgets(line,LINE_LENGTH-1,stdin)) {
  if(strchr(line,'\r')) *strchr(line,'\r')=0;
  if(strchr(line,'\n')) *strchr(line,'\n')=0;
  strcpy(copy,line);
  memset(&u,0,sizeof(u));
  urlfromline(&u,line);
  //use the character in argv[1] to match stdin against argv[2]. if match print whole line.
  for(i=1;i<argc;i+=2) {
   if(negate) {i--;}//we didn't really need to go that far.
   negate=0;
   if(argv[i][0] == '-') argv[i]++;
   if(argv[i][0] == 'n') {argv[i]++; negate=1; }//heh.
   switch(argv[i][0]) {
    case 's': if(match(negate,u.scheme,argv[i+1]))       { printf("%s\n",copy); ret=0;} break;
    case 'u': if(match(negate,u.username,argv[i+1]))     { printf("%s\n",copy); ret=0;} break;
    case 'k': if(match(negate,u.password,argv[i+1]))     { printf("%s\n",copy); ret=0;} break;
    case 'd': if(match(negate,u.domain,argv[i+1]))       { printf("%s\n",copy); ret=0;} break;
    case 'P': if(match(negate,u.port,argv[i+1]))         { printf("%s\n",copy); ret=0;} break;
    case 'p': if(match(negate,u.path,argv[i+1]))         { printf("%s\n",copy); ret=0;} break;
    case 'q': if(match(negate,u.query_string,argv[i+1])) { printf("%s\n",copy); ret=0;} break;
    case 'f': if(match(negate,u.fragment_id,argv[i+1]))  { printf("%s\n",copy); ret=0;} break;
    case 'D': //not sure how to look for a missing one of these. it'd be like d.
     if(u.domain && argv[i+1] && strlen(u.domain) >= strlen(argv[i+1]) && !strncmp(u.domain+strlen(u.domain)-strlen(argv[i+1]),argv[i+1],strlen(argv[i+1]))) 
      printf("%s\n",copy);
      ret=0;
     break;
    default:
     printf("unknown url part letter! '%c'\n",argv[i][0]);
     return ret;
   }
  }
 }
 return ret;
}

