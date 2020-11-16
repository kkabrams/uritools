#define _POSIX_C_SOURCE 200908
#include <stdio.h>
#include <sys/stat.h>
#include <openssl/md5.h>
#include <string.h>
//#include <sys/types.h>
#include <sys/xattr.h>

#define BYTES_IN_SHORT 2
#define CACHE_DIR "/var/cache/shorten"

int main(int argc,char *argv[]) {
  int i;
  if(argc < 2) return 1;
  unsigned char *p=MD5((unsigned char *)argv[1],strlen(argv[1]),NULL);
  char filepath[4096];//too long, oh well
  char out[256];
  char tmp[3];
  strcpy(out,"");
  for(i=0;i<BYTES_IN_SHORT;i++) {
    snprintf(tmp,sizeof(tmp),"%02x",p[i]);
    strcat(out,tmp);
  }
  printf("%s\n",out);
  snprintf(filepath,sizeof(filepath)-1,"%s/%s",CACHE_DIR,out);
  FILE *fp;
  if((fp=fopen(filepath,"w")) == NULL) {
    fprintf(stderr,"shorten: failed to write a file: %s\n",filepath);
    return 1;
  }
  fprintf(fp,"%s\n",argv[1]);
  //disabled for now because tmpfs can't do extended attributes
  //fprintf(stderr,"fsetxattr return: %d\n",fsetxattr(fileno(fp),"user.mime-type","text/uri-list",strlen("text/uri-list"),0));
  //perror("fsetxattr");
  fchmod(fileno(fp),0664);
  fclose(fp);
}
