/* run client using: ./client localhost <server_port> */
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cstring>

#include <netdb.h>
using namespace std;
void error(char *msg) {
  perror(msg);
  exit(0);
}

int main(int argc, char *argv[]) {
  int sockfd, portno, n;

  struct sockaddr_in serv_addr;
  struct hostent *server;
  char escape[8] = "EXIT\n";

  char buffer[1024];
  if (argc < 3) {
    fprintf(stderr, "usage %s hostname port\n", argv[0]);
    exit(0);
  }

  /* create socket, get sockfd handle */

  portno = atoi(argv[2]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error("ERROR opening socket");

  /* fill in server address in sockaddr_in datastructure */

  server = gethostbyname(argv[1]);
  if (server == NULL) {
    fprintf(stderr, "ERROR, no such host\n");
    exit(0);
  }
  bzero((char *)&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
  serv_addr.sin_port = htons(portno);

  /* connect to server */
  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR connecting");

  while (true) {
    /* ask user for input */
    printf("Please enter the message / Type \"EXIT\": ");
    bzero(buffer, 1024);
    fgets(buffer, 1023, stdin);

    /* send user message to server */

    n = write(sockfd, buffer, strlen(buffer));

    if (n < 0)
      error("ERROR writing to socket");

    if (strcmp(buffer, escape) == 0) break;
    
    bzero(buffer, 1024);

    /* read reply from server */

    n = read(sockfd, buffer, 1023);

    // cout << strlen(buffer) << endl;
    // cout << buffer;

    if (n < 0)
      error("ERROR reading from socket");
    printf("\nServer response: \n%s\n\n\n", buffer);
  }

  return 0;
}
