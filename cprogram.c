#include <stdio.h>
#include <stddef.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>

int make_socket();
void write_socket(void *conn);
void read_socket(void *conn);

int main(){

  register int sock = make_socket();
  int conn;
  struct sockaddr_un cli_sadd;
  pthread_t tread,twrite;
  int len;

  if( listen(sock,5) < 0){
    perror("listen error");
    exit(EXIT_FAILURE);
  }
  
  if(( conn = accept (sock, (struct sockaddr *)&cli_sadd, &len)) < 0){
    
    perror("accept error");
    exit(EXIT_FAILURE);
  }

  // Possible bug if both try to access conn at the same time?
  int ret = pthread_create(&tread,NULL,(void *)read_socket,(void *)&conn);
  int rot = pthread_create(&twrite,NULL,(void *)write_socket,(void *)&conn);

  pthread_join(twrite,NULL);
  return 0;
}
  
void read_socket(void *c){
  
  int *conn = (int*)c;
  FILE *fp = fdopen(*conn,"r");
  char ch;


  while((ch = fgetc(fp)) != EOF)
    putchar(ch);

  pthread_exit(0);
}

void write_socket(void *s){

  int stop = 0;
  char buf[500];
  int *sock_fd = (int*) s;
  char quit='q';

  printf("Please type a message:\n");
  while( stop == 0 ){
    
    fgets(buf,sizeof(buf),stdin);
    if(buf[0]==quit) break;
    send(*sock_fd,buf,strlen(buf),0);
    printf("\nSent message***\n %s ***through socket. ",buf);
    printf("Please type a message:\n");
  }

  pthread_exit(0);
}

int make_socket(){

  struct sockaddr_un name;
  int sock_fd;

  sock_fd = socket(PF_UNIX,SOCK_STREAM,0);
  
  if(sock_fd < 0){
    
    perror("Socket");
    exit(EXIT_FAILURE);
  }

  name.sun_family = AF_FILE;
  strcpy(name.sun_path,"/tmp/sock_test");
  int size = sizeof(name.sun_family) + strlen(name.sun_path) + 1;

  if( bind(sock_fd, (struct sockaddr *) &name, size) < 0 ){
    
    perror("Bind");
    exit(EXIT_FAILURE);
  }

  return sock_fd;
}
  
  
  




