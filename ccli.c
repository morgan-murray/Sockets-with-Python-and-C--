#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>


int main(){

  int s = socket(AF_UNIX,SOCK_STREAM,0);

  struct sockaddr_un name;
  name.sun_family = AF_UNIX;
  strcpy(name.sun_path,"/tmp/sock_test");

  int l = sizeof(name.sun_family) + strlen(name.sun_path);
  
  if(connect(s,(struct sockaddr *)&name,l) < 0){
    
    perror("client connection");
    exit(EXIT_FAILURE);
  }
  

  FILE *fp = fdopen(s,"r");

  char mes[5];
  
  strcpy(mes,"mess");

  send(s,mes,strlen(mes),0);

  close(s);
  return 0;
}
