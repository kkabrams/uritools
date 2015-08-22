#ifndef URL_H
#define URL_H
struct url {
 char *scheme;
 char *username;
 char *password;
 char *domain;
 char *port;
 char *path;
 char *query_string;
 char *fragment_id;
};

void urlfromline(struct url *u,char *line);
#endif
