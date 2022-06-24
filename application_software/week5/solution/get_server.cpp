/* run using ./server <port> */
#include "http_server.hh"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cstring>
#include <netinet/in.h>
#include <queue>
#include <pthread.h>

#define N 512

int no_clients = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;
pthread_cond_t c_FULL = PTHREAD_COND_INITIALIZER;

void error(char *msg) {
  perror(msg);
  exit(1);
}

void *start_function(void *arg) {
  int n;
  string temp;
  char buffer[1024], responsebuffer[1024];
  char escape[8] = "EXIT\n";

  while (true)
  {
    pthread_mutex_lock(&m);
    while (no_clients == 0)
      pthread_cond_wait(&c, &m);
    if (no_clients == -1){
      pthread_mutex_unlock(&m);
      break;
    }
    int newsockfd = (*((queue<int> *) arg)).front();
    (*((queue<int> *) arg)).pop();
    no_clients -= 1;
    pthread_cond_signal(&c_FULL);
    // cout << "thread dealing " << newsockfd << " " << no_clients << endl;
    pthread_mutex_unlock(&m);

    while (true)
    {
    /* read message from client */
    
      bzero(buffer, 1024);
      n = read(newsockfd, buffer, 1023);

      if (strcmp(buffer, escape) == 0 | strcmp(buffer, "") == 0) break;
      if (n < 0)
        error("ERROR reading from socket");
      // printf("\nHere is the HTTP Request: %s", buffer);

      /* send reply to client */
      string FromClient(buffer);
      HTTP_Response *FromServer = handle_request(FromClient);
      temp = FromServer->get_string();
      delete FromServer;

      strcpy(responsebuffer, temp.c_str());
      // cout << responsebuffer << endl;

      n = write(newsockfd, responsebuffer, strlen(responsebuffer));
      if (n < 0)
        error("ERROR writing to socket");
    }
  }
  return 0;
}

int main(int argc, char *argv[]) {
  int sockfd, newsockfd, portno;
  queue<int> client_newsockfd;
  socklen_t clilen;
  struct sockaddr_in serv_addr, cli_addr;
  int n;
  pthread_t thread_id[N];


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

  for(int i = 0; i < N; i++){
    if ( pthread_create(&thread_id[i], NULL, start_function, (void*)&client_newsockfd) < 0 )
    {
        perror("could not create thread");
        return 1;
    }
  }
  
  while(true){
    /* listen for incoming connection requests */

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    /* accept a new request, create a newsockfd */

    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0)
      error("ERROR on accept");

    pthread_mutex_lock(&m);
    while (no_clients == N)
      pthread_cond_wait(&c_FULL, &m);
    no_clients += 1;
    client_newsockfd.push(newsockfd);
    // cout << "added to queue " << newsockfd << " " << no_clients << endl;
    pthread_cond_signal(&c); 
    pthread_mutex_unlock(&m);
  }

  while(no_clients > 0)
  {
    /* code */
  }
  
  no_clients = -1;
  cout << no_clients << endl;

  for(int i = 0; i < N-1; i++) pthread_cond_signal(&c); 
  for(int i = 0; i < N; i++) pthread_join(thread_id[i], NULL); 

  pthread_exit(NULL);
  return 0;
}
