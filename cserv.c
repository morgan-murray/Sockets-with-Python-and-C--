#include <stdio.h>
#include <stdlib.h>
#include  <errno.h>
#include <sys/socket.h>
#include <sys/un.h>

// Simple server

int main(){

  int s;
  int os;

  struct sockaddr_un name,cli_addr;
  
  // create socket
  if((s = socket(PF_UNIX,SOCK_STREAM,0)) < 0){
    perror("socket");
    exit(EXIT_FAILURE);
  }
  unlink("/tmp/sock_test");
  name.sun_family = AF_FILE;
  strcpy(name.sun_path,"/tmp/sock_test");
  int size = sizeof(name.sun_family) + strlen(name.sun_path) + 1;

  if(bind(s, (struct sockaddr *)&name, size) < 0){
    perror("bind");
    exit(EXIT_FAILURE);
  }

  // listen and accept a connection
  if (listen(s,5) < 0){
    perror("listen");
    exit(EXIT_FAILURE);
  }

  int len;
  int conn = accept(s,(struct sockaddr *)&cli_addr,&len);
  if (conn < 0 ){
    perror("accept");
    exit(EXIT_FAILURE);
  }

  FILE *fp = fdopen(conn,"r");
    
  int ii = 4;
  char c;

  // Read 4 bytes from socket
  while(ii > 0){ 
    c = fgetc(fp);
    putchar(c);
    ii--;
  }
    
  close(s);
  unlink("/tmp/sock_test");  
  printf("\n");
  return 0;
}
    
  
