/* run using ./server <port> */
#include "http_server.hh"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>

#include <pthread.h>

void error(char *msg) {
  perror(msg);
  exit(1);
}

void *start_function(void *arg) {
  int newsockfd = *((int *) arg);
  int n;
  string temp;
  char buffer[1024];
  char escape[8] = "EXIT\n";

  while (true)
  {
    /* read message from client */
    
    bzero(buffer, 1024);
    n = read(newsockfd, buffer, 1023);


    if (strcmp(buffer, escape) == 0 | strcmp(buffer, "") == 0) break;
    
    if (n < 0)
      error("ERROR reading from socket");
    printf("Here is the HTTP Request:\n%s", buffer);

    /* send reply to client */
    string FromClient(buffer);
    HTTP_Response FromServer = *handle_request(FromClient);
    temp = FromServer.get_string();
    char result[1023];
    strcpy(result, temp.c_str());

    n = write(newsockfd, result, strlen(result));
    if (n < 0)
      error("ERROR writing to socket");
  }
  return 0;
}

int main(int argc, char *argv[]) {
  int sockfd, newsockfd, portno;
  socklen_t clilen;
  struct sockaddr_in serv_addr, cli_addr;
  int n;
  pthread_t thread_id;


  if (argc < 2) {
    fprintf(stderr, "ERROR, no port provided\n");
    exit(1);
  }

  /* create socket */

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error("ERROR opening socket");

  /* fill in port number to listen on. IP address can be anything (INADDR_ANY)
   */

  bzero((char *)&serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  /* bind socket to this port number on this machine */

  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR on binding");

  while(true){
    /* listen for incoming connection requests */

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    /* accept a new request, create a newsockfd */

    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0)
      error("ERROR on accept");

    if ( pthread_create(&thread_id, NULL, start_function, (void*)&newsockfd) < 0 )
    {
        perror("could not create thread");
        return 1;
    }
  }
  pthread_join(thread_id, NULL);

  return 0;
}
