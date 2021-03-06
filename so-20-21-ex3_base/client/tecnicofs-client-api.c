#include "tecnicofs-client-api.h"
char commandSuccess[10]="SUCCESS";
char commandFail[10]="FAIL";

int setSockAddrUn(char *path, struct sockaddr_un *addr) {

  if (addr == NULL)
    return 0;

  bzero((char *)addr, sizeof(struct sockaddr_un));
  addr->sun_family = AF_UNIX;
  strcpy(addr->sun_path, path);

  return SUN_LEN(addr);
}

int tfsCreate(char *path, char nodeType) {

  char command[MAX_INPUT_SIZE];
  int receive;

  sprintf(command,"c %s %c", path, nodeType);

  if (sendto(sockfd, command, strlen(command)+1, 0, (struct sockaddr *) &serv_addr, servlen) < 0) {
    perror("client: sendto error");
    return -1;
  }

  if (recvfrom(sockfd, (void*) &receive, sizeof(&receive), 0, 0, 0) < 0) {
    perror("client: recvfrom error");
    return -1;
  } 

  return receive;

}

int tfsDelete(char *path) {

  char command[MAX_INPUT_SIZE];
  int receive;

  sprintf(command,"d %s", path);

  if (sendto(sockfd, command, strlen(command)+1, 0, (struct sockaddr *) &serv_addr, servlen) < 0) {
    perror("client: sendto error");
    return -1;
  } 

  if (recvfrom(sockfd, (void*) &receive, sizeof(&receive), 0, 0, 0) < 0) {
    perror("client: recvfrom error");
    return -1;
  } 

  return receive;

}

int tfsMove(char *from, char *to) {

  char command[MAX_INPUT_SIZE];
  int receive;

  sprintf(command,"m %s %s", from, to);

  if (sendto(sockfd, command, strlen(command)+1, 0, (struct sockaddr *) &serv_addr, servlen) < 0) {
    perror("client: sendto error");
    return -1;
  } 

  if (recvfrom(sockfd, (void*) &receive, sizeof(&receive), 0, 0, 0) < 0) {
    perror("client: recvfrom error");
    return -1;
  } 

  return receive;

}

int tfsLookup(char *path) {

  char command[MAX_INPUT_SIZE];
  int receive;

  sprintf(command,"l %s", path);

  if (sendto(sockfd, command, strlen(command)+1, 0, (struct sockaddr *) &serv_addr, servlen) < 0) {
    perror("client: sendto error");
    return -1;
  } 

  if (recvfrom(sockfd, (void*) &receive, sizeof(&receive), 0, 0, 0) < 0) {
    perror("client: recvfrom error");
    return -1;
  } 

  return receive;

}

int tfsPrint(char *path) {

  char command[MAX_INPUT_SIZE];
  int receive;

  sprintf(command,"p %s", path);

  if (sendto(sockfd, command, strlen(command)+1, 0, (struct sockaddr *) &serv_addr, servlen) < 0) {
    perror("client: sendto error");
    return -1;
  } 

  if (recvfrom(sockfd, (void*) &receive, sizeof(&receive), 0, 0, 0) < 0) {
    perror("client: recvfrom error");
    return -1;
  } 

  return receive;

}

int tfsMount(char * sockPath) {

  if ((sockfd = socket(AF_UNIX, SOCK_DGRAM, 0) ) < 0) {
      perror("client: can't open socket");
      return -1;
  }

  servlen = setSockAddrUn (sockPath, &serv_addr);

  if (bind(sockfd, (struct sockaddr *) &serv_addr, servlen) != -1) {
    perror("client: socket does not exist");
    return -1;
  }

  if (sendto(sockfd, "t", strlen("t")+1, 0, (struct sockaddr *) &serv_addr, servlen) < 0) {
    perror("client: sendto error");
    return -1;
  } 

  unlink(nameclient);
  clilen = setSockAddrUn (nameclient, &client_addr);
  if (bind(sockfd, (struct sockaddr *) &client_addr, clilen) < 0) {
    perror("client: bind error");
    return -1;
  } 

  return 0;
}

int tfsUnmount() {
  if(close(sockfd)){
    perror("client: unmount error");
    return -1;
  }

  if(unlink(nameclient)){
    perror("client: unmount error");
    return -1;
  }

  return 0;
}
