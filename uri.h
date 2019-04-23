#ifndef uri_H
#define uri_H

//#define _XOPEN_SOURCE 500 //for strdup
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

//uri_reserved = gen-delims / sub-delims
#define pe_gen_delims ":/?#[]@"
#define pe_sub_delims "!$&'()*+,;="
//char *pe_reserved[]=pe_gen_delims "" pe_sub_delims; 
#define pe_ALPHA "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define pe_DIGIT "0123456789"
#define pe_HPUT "-._~"
//char *pe_unreserved[]=pe_ALPHA "" pe_DIGIT "" pe_HPUT;

unsigned char rfc3086_percent_encoding[256];

#define isxdigit(a) ((a >= 'a' && a <= 'f') || (a >= '0' && a <= '9') || (a >= 'A' && a <= 'F'))
#define toupper(a) ((a >= 'a' && a <= 'z')?a-' ':a)

char *uri_reserved={
  pe_gen_delims
  pe_sub_delims
  pe_ALPHA
  pe_DIGIT
  pe_HPUT
};

int uriescapelength(char *in,int len) {
  int rlen=0;//be sure to add one to this return value if you plan on putting a null byte at the end.
  int i;
  for(i=0;i<len;i++) {
    rlen+=strchr(uri_reserved,in[i])?1:3;
  }
  return rlen;
}

// make sure your out char * has enough space! use uriescapelength for it.
void uriescape(char *in,char *out,int len) {
  int i;
  int j;
  for(i=0,j=0;i<len;i++) {
    if(strchr(uri_reserved,in[i])) {
      out[j]=in[i];
      j++;
    } else {
      out[j]='%';
      j++;
      out[j]="0123456789ABCDEF"[(in[i] >> 4 & 0x15)];
      j++;
      out[j]="0123456789ABCDEF"[(in[i] % 16)];
      j++;
    }
  }
}

int uriunescape(char *in,char *out) {
 char *o=out;
 char *t;
 char a,b;
 char *s=in;
 if(!strchr(s,'%')) memmove(out,in,strlen(in));
 while((t=strchr(s,'%'))) {
  if(t-s) {//if there are actually bytes to copy.
   memmove(o,s,t-s);
   o+=(t-s);
   s+=(t-s);
  }
  if(isxdigit(t[1]) && isxdigit(t[2])) {
   s+=3;//skip the %XX
   a=toupper(t[1]);
   b=toupper(t[2]);
   *o=((a-'0'<10 ? a-'0' : a-'A'+10) << 4) + (b-'0'<10 ? b-'0' : b-'A'+10); 
   o++;
  } else {
   s++;//skip just the %. the next character might be a % //TODO: look up what the "right" thing to do here is.
   *o='%';
   o++;
  }
 }
 //copy the last part.
 memmove(o,s,strlen(s));
 o[strlen(s)]=0;
 return o+strlen(s)-out;
}

struct uri {//warning. it is technically undefined behavior to set one half of a union then use the other half.
  union {
    char *A[8];
    struct {
      union { char *s;char *scheme; };
      union { char *u;char *username; };
      union { char *k;char *password; };
      union { char *d;char *domain; };
      union { char *P;char *port; };
      union { char *p;char *path; };
      union { char *q;char *query_string; };
      union { char *f;char *fragment_id; };
    };
  };
};

//returns 0 on success, returns a byte with bits set for non-matching pieces.
unsigned int uricmp(struct uri *a,struct uri *b) {
  int i;
  int ret=0;
  for(i=0;i<8;i++) {
    if(a->A[i] && !b->A[i]) ret |=(1<<(i+8));//we have a's but not b's
    if(!a->A[i] && b->A[i]) ret |=(1<<(i+16));
    //for testing if(!a->A[i] && !b->A[i]) ret |=(1<<(i+24));//no problem here. both empty.
    if(a->A[i] && b->A[i]) {
      if(strcmp(a->A[i],b->A[i])) {
        ret|=(1<<i);
      }
    }
  }
  return ret;
}

char *linefromuri(struct uri *u) {
  char *line=malloc(2048);//fuck if I know
  strcpy(line,"");
  if(u->scheme) {
    strcat(line,u->scheme);
  }
  if(u->scheme && u->domain) {
    strcat(line,"://");
  }
  if(u->scheme && !u->domain) {
    strcat(line,":");
  }
  if(u->username && u->domain) {
    strcat(line,u->username);
  }
  if(u->password && u->username && u->domain) {//we /should/ only have a password if there's a username AND domain
    strcat(line,":");
    strcat(line,u->password);
  }
  if(u->username && u->domain) {
    strcat(line,"@");
  }
  if(u->domain) {
    strcat(line,u->domain);
  }
  if(u->port && u->domain) { //port only makes sense if there's a domain
    strcat(line,":");
    strcat(line,u->port);
  }
  if(u->path && u->scheme && !u->domain) {
    strcat(line,u->path);
  }
  if(u->path && u->scheme && u->domain) {
    if(*u->path != '/') {
      strcat(line,"/");
    }
    strcat(line,u->path);
    //path must start with / if we have domain.
  }
  if(u->query_string) {
    strcat(line,"?");
    strcat(line,u->query_string);
  }
  if(u->fragment_id) {
    strcat(line,"#");
    strcat(line,u->fragment_id);
  }
  return line;
}

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

//should it be a dick about what characters are allowed?
//should it just try to ignore weird shit?

//return 0 on fail //not sure what this means.
//return 1 on success
int urifromline(struct uri *u,char *line) {
  //these first two are easy. the rest... not so much.
  char *t;
//  memset(u,0,sizeof(struct uri)); //this function shouldn't do this.
  if((u->fragment_id=strchr(line,'#'))) {
    *u->fragment_id=0;
    u->fragment_id++;
  }
  if((u->query_string=strchr(line,'?'))) {
    *u->query_string=0;
    u->query_string++;
  }
  //now we have scheme, user, pass, domain, port, and path. maybe.
  //what character can we split on now? : is a terrible choice.
  // how about /? first / is either a separator between scheme
  //could find the first non-scheme character.
  //so we might have... scheme://user:pass@host:port/path
  //or... user:pass@host:port/path ?
  //we need to do this based on /s
  // we're either going to find the scheme and authority separator
  // or we're going to find the start of a path.
  //there: scheme:/path, scheme://host (empty path), or scheme:path/morepath
  //or...  should we do paths without
  //scheme must start with a-z
/*  if(*line == '/' && *(line+1) != '/') { //we have a relative path. /like:this.maybe
    u->path=line;
    return;//we're done. nothing else to do.
  }
  if(*line == '.') { //we have a relative path like: ./derp or ../merp
    u->path=line;
    return;//we're done here. nothing else to do.
  }*/
  //let's see if this starts with a scheme
  if(strchr(line,':') && ((*line >= 'a' && *line <= 'z') || (*line >= 'A' && *line <= 'Z'))) {
    for(u->scheme=strchr(line,':')-1;u->scheme > line;u->scheme--) {
      if((*u->scheme >= 'a' && *u->scheme <= 'z') ||
         (*u->scheme >= 'A' && *u->scheme <= 'Z') ||
         (*u->scheme >= '0' && *u->scheme <= '9') ||
         *u->scheme == '+' || *u->scheme == '-' || *u->scheme == '.') {
        //this is still a scheme.
      } else {
        break;
      }
    }
    if(u->scheme == line) {//we got through the for loop alright. line starts with a scheme.
      line=strchr(line,':');
      *line=0;
      line++;
      for(t=u->scheme;*t;t++) {
        if(*t >= 'A' && *t <= 'Z') *t+=' ';
      }
    }
  }

  //copy-pasted from above the scheme strip attempt.
  if(*line == '/' && *(line+1) != '/') { //we have a relative path. /like:this.maybe
    u->path=line;
    return 1;//we're done. nothing else to do.
  }
  if(*line == '.') { //we have a relative path like: ./derp or ../merp
    u->path=line;
    return 1;//we're done here. nothing else to do.
  }

  if(*line == '/' && line[1] == '/') {//we have an authority section.
    //let's left-shift this shit over until the third /
    for(t=line+1;*(t+1) && *(t+1) != '/';t++) {
      *t=*(t+1);
    }
    *t=0;
    u->path=t+1;//if there was a /, path points at it and the stuff after.
    //if there wasn't a /, it points at a null byte. so "empty"
    u->username=line+1;
  } else {
    //we're an authority section without a // I guess.
    //or we're a path
    if(u->scheme) u->path=line;
    else u->username=line;//if we have a scheme we're not a //-less authority
  }

  if(u->username) {//this contains all of the authority.
    if((u->domain=strchr(u->username,'@'))) {//we have user@host at least.
      *u->domain=0;
      u->domain++;
    } else {//this isn't really a username. it is the domain.
      u->domain=u->username;
      u->username=0;
    }
  }
  //if we still have u->username we try to split to user and password
  if(u->username) {
    if((u->password=strchr(u->username,':'))) {
      *u->password=0;
      u->password++;
    }
  }
  if(u->domain) {
    if((u->port=strchr(u->domain,']')) && *u->domain == '[') {//this is an IPv6 host
      *u->port=0;
      u->port++;
      if(*u->port == ':') {
        *u->port=0;
        u->port++;//if it ends up being empty, whatever. that's a URI like: http://host:/path
      }
    } else { //we're safe to split port off at :
      if((u->port=strchr(u->domain,':'))) {
        *u->port=0;
        u->port++;
      } //there isn't a port. leave it unset.
    }
  }
  //I dunno.
  return 1;
}

#endif
