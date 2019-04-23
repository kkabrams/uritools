#define _XOPEN_SOURCE 500
#include <string.h>
#include "uri.h"

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

/*
 schemes are case sensitive but cononicals are lower case.
 domain is case insensitive. return it lowercased?
 port is optional and in decimal
 path
 scheme://username:password@domain:port/path?query_string#fragment_id
 mailto:username@domain

 optional stuff:
 scheme, username, password, port, path, query_string, fragment_id
*/

#define AorB(a,b) ((a)?(a):(b))

#define F_SCHEME 1<<0
#define F_USERNAME 1<<1
#define F_PASSWORD 1<<2
#define F_DOMAIN 1<<3
#define F_PORT 1<<4
#define F_PATH 1<<5
#define F_QUERY_STRING 1<<6
#define F_FRAGMENT_ID 1<<7
#define F_WHOLE_URI 1<<8

char *long_opts[]={"scheme","username","password","domain","port","path","query_string","fragment_id","URI",0};
char *short_opts[]={"s","u","k","d","P","p","q","f","U"};

int main(int argc,char *argv[]) {
 char *uri;
 char *line=0;
 short args[256];//this needs to be a short to make room for the F_WHOLE_URI
 int i,j,c=0;
 int size=1024;
 char fixme=0;
 char using_stdin=1;
 char malloced=0;
 struct uri u;
 if(argc > 1) {
  if(!strcmp(argv[1],"--help") || !strcmp(argv[1],"-h")) {
   printf("usage: echo uris | uricut [options]\n");
   printf("usage: uricut [options] uri [options] [uri]\n\n");
   printf("options: \n");
   for(i=0;long_opts[i];i++) {
    printf("        -%s|--%s\n",short_opts[i],long_opts[i]);
   }
   printf("To set default values use environment variables like: CUTURI_[OPTION]\n");
   return 2;
  }
 }
 argv++;
 argc--;
 while(1) {
  u.scheme=0;
  u.username=0;
  u.password=0;
  u.domain=0;
  u.port=0;
  u.path=0;
  u.query_string=0;
  u.fragment_id=0;
  if(!using_stdin) c=0;
  if(argc >= 1) {
   for(;argc>0;argc--,argv++) {
    for(i=0;long_opts[i];i++) {
     if(!strncmp(*argv,"--",2)) {
      if(!strcmp(*argv+2,long_opts[i])) {
       args[c]=1<<i;
       c++;
       break;
      }
     }
    }
    fixme=0;
    if(**argv=='-' && argv[0][1] != '-') {
     for(j=1;argv[0][j];j++) {
      for(i=0;short_opts[i];i++) {
       if(argv[0][j]==*short_opts[i]) {
        args[c]=1<<i;
        c++;
        fixme=1;
       }
      }
     }
    }
    if(fixme) continue;
    if(long_opts[i]) continue;
    //if we get here we are at data instead of flags. work on it.
    line=*argv;
    using_stdin=0;
    argc--;
    argv++;
    break;
   }
  }
  if(!argc && !line) {//if we are out of arguments and it didn't include data
   using_stdin=1;
  }
  if(using_stdin) {
   line=malloc(size+1);
   malloced=1;
   if(!fgets(line,size,stdin)) {
    free(line);
    return 0;
   }
  }
  if(!line) return 0;
  for(i=0;line[i] && line[i] != '\n' && line[i] != '\r';i++);
  line[i]=0;

  uri=strdup(line);
  urifromline(&u,line);

  // printf("scheme://username:password@domain:port/path?query_string#fragment_id\n\n");
  //let's set them to what'll get printed now...

   if(c) {
    for(i=0;i<c;i++) {
     if(args[i]&F_SCHEME) printf("%s\n",AorB(u.scheme,""));
     if(args[i]&F_USERNAME) printf("%s\n",AorB(u.username,""));
     if(args[i]&F_PASSWORD) printf("%s\n",AorB(u.password,""));
     if(args[i]&F_DOMAIN) printf("%s\n",AorB(u.domain,""));
     if(args[i]&F_PORT) printf("%s\n",AorB(u.port,""));
     if(args[i]&F_PATH) printf("%s\n",AorB(u.path,""));
     if(args[i]&F_QUERY_STRING) printf("%s\n",AorB(u.query_string,""));
     if(args[i]&F_FRAGMENT_ID) printf("%s\n",AorB(u.fragment_id,""));
     if(args[i]&F_WHOLE_URI) printf("%s\n",uri);
    }
   } else {
    if(u.scheme)       printf("scheme: %s\n",u.scheme);
    if(u.username)     printf("username: %s\n",u.username);
    if(u.password)     printf("password: %s\n",u.password);
    if(u.domain)       printf("domain: %s\n",u.domain);
    if(u.port)         printf("port: %s\n",u.port);
    if(u.path)         printf("path: %s\n",u.path);
    if(u.query_string) printf("query_string: %s\n",u.query_string);
    if(u.fragment_id)  printf("fragment_id: %s\n",u.fragment_id);
    printf("whole_uri: %s\n",uri);
   }
  free(uri);//this is definitely malloc()d
  if(malloced) {
   free(line);
   malloced=0;
   line=0;
  } else {
   line=0;//???
  }
 }
 return 0;
}
