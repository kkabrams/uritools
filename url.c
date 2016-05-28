#include <netdb.h>
#include <string.h>

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

#include "url.h"

/*
struct url {
 char *scheme;
 char *username;
 char *password;
 char *domain;
 char *port;
 char *path;
 char *query_string;
 char *fragment_id;
}
*/

void urlfromline(struct url *u,char *line) {
 int i;
  //split at first single / into line and path
  for(i=0;line[i];i++) {
   if(line[i] == '/' && line[i+1] == '/') {
    i++;
    continue;
   }
   if(line[i] == '/') {
    line[i]=0;
    u->path=line+i+1;
    break;
   }
  }
  if(u->path) {
   if(strchr(u->path,'?')) {
    u->query_string=strchr(u->path,'?');
    *u->query_string=0;
    u->query_string++;
   }
  }

  if(u->query_string) {
   if(strchr(u->query_string,'#')) {
    u->fragment_id=strchr(u->query_string,'#');
    *u->fragment_id=0;
    u->fragment_id++;
   }
  }

  if(strstr(line,"://")) {
   u->scheme=line;
   u->domain=strstr(line,"://");
   *u->domain=0;
   u->domain+=3;
  } else {
   u->domain=line;
  }

  if(u->domain) {
   if(strchr(u->domain,'@')) {
    u->username=u->domain;
    u->domain=strchr(u->domain,'@');
    *u->domain=0;
    u->domain++;
   }
  }

  if(u->username) {
   if(strchr(u->username,':')) {
    u->password=strchr(u->username,':');
    *u->password=0;
    u->password++;
   }
  }

  if(u->domain) {
   if(strchr(u->domain,']')) {//the end of an IPv6 address
    if(strchr(strchr(u->domain,']'),':')) {
     u->port=strchr(strchr(u->domain,']'),':');
     if(u->port[1] == '?') {//for magnet links
      u->port=0;
     } else {
      *u->port=0;
      u->port++;
     }
    }
   } else {
    if(strchr(u->domain,':')) {
     u->port=strchr(u->domain,':');
     if(u->port[1] == '?') {//for magnet links
      u->port=0;
     } else {
      *u->port=0;
      u->port++;
     }
    }
   }
  }

  if(u->domain) {//for magnet links.
   if(strchr(u->domain,'?')) {
    u->query_string=strchr(u->domain,'?');
    *u->query_string=0;
    u->query_string++;
   }
  }

  if(strchr(u->domain,':') && !strchr(u->domain,']')) {//for scheme:?query_string
   u->scheme=u->domain;
   *strchr(u->scheme,':')=0;
   u->domain=0;
  }

  if(!u->scheme && u->username) {//for mailto:
   u->scheme=u->username;
   u->username=u->password;
   u->password=0;
  }
}

#define AorB(a,b) ((a)?(a):(b))

void magic_and_defaults(struct url *u) {
  struct servent *serv;
  char sport[10];
  u->scheme=AorB(u->scheme,AorB(getenv("URL_SCHEME"),"DEFAULT"));
  u->username=AorB(u->username,AorB(getenv("URL_USERNAME"),"DEFAULT"));
  u->password=AorB(u->password,AorB(getenv("URL_PASSWORD"),"DEFAULT"));
  u->domain=AorB(u->domain,AorB(getenv("URL_DOMAIN"),"DEFAULT"));
  serv=getservbyname(u->scheme,strcmp(u->scheme,"udp")?"tcp":"udp");//gets default port for the scheme. http -> 80
  if(serv) snprintf(sport,sizeof(sport)-1,"%d",ntohs(serv->s_port));
  u->port=AorB(u->port,AorB(getenv("URL_PORT"),(serv?sport:"DEFAULT")));
  u->path=AorB(u->path,AorB(getenv("URL_PATH"),"DEFAULT"));
  u->query_string=AorB(u->query_string,AorB(getenv("URL_QUERY_STRING"),"DEFAULT"));
  u->fragment_id=AorB(u->fragment_id,AorB(getenv("URL_FRAGMENT_ID"),"DEFAULT"));
}
