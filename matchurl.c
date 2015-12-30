#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "url.h"

#define LINE_LENGTH 1024

int main(int argc,char *argv[]) {
 int i;
 struct url u;
 char *line=malloc(LINE_LENGTH);
 char copy[LINE_LENGTH];
 if(argc < 3) {
  printf("usage: matchurl [-][s|u|k|d|D|P|p|q|f] [string]\n");
  printf("scheme://username:password@domain:port/path?query_string#fragment_id\n");
  printf("s://u:k@d:P/p?q#f\n");
  printf("The D flag is special. it matches its argument against the last bytes of the input url's domain.\n");
  printf("This allows matching of subdomains, like `echo epoch.ano | matchurl -D ano` would match.\n");
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
   if(argv[i][0] == '-') argv[i]++;
   switch(argv[i][0]) {
    case 's': if(u.scheme       && !strcmp(u.scheme,argv[i+1]))       printf("%s\n",copy); break;
    case 'u': if(u.username     && !strcmp(u.username,argv[i+1]))     printf("%s\n",copy); break;
    case 'k': if(u.password     && !strcmp(u.password,argv[i+1]))     printf("%s\n",copy); break;
    case 'd': if(u.domain       && !strcmp(u.domain,argv[i+1]))       printf("%s\n",copy); break;
    case 'P': if(u.port         && !strcmp(u.port,argv[i+1]))         printf("%s\n",copy); break;
    case 'p': if(u.path         && !strcmp(u.path,argv[i+1]))         printf("%s\n",copy); break;
    case 'q': if(u.query_string && !strcmp(u.query_string,argv[i+1])) printf("%s\n",copy); break;
    case 'f': if(u.fragment_id  && !strcmp(u.fragment_id,argv[i+1]))  printf("%s\n",copy); break;
    case 'D':
     if(u.domain && strlen(u.domain) >= strlen(argv[i+1]) && !strncmp(u.domain+strlen(u.domain)-strlen(argv[i+1]),argv[i+1],strlen(argv[i+1]))) 
      printf("%s\n",copy);
     break;
    default:
     printf("unknown url part letter! '%c'\n",argv[i][0]);
     return 0;
   }
  }
 }
 return 0;
}

