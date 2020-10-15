#include "server.h"

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void* echo(void *param)
{
    char buf[1024];
    pthread_detach(pthread_self());
    int sockt = *((int *)param);
    bool endOfSocket = false;
    while( read(sockt, buf, 1023) > 0 && !endOfSocket) {
        std::string commandStr(buf);
        if (commandStr == "END") endOfSocket = true;
//        printf("Server received %s\n", buf);
//        printf("Server sending it back\n");
//        std::string image = getPicture("Welcome.txt");
//        write(sockt, image.c_str(), strlen(image.c_str()));
//        image = getPicture("TechQuest.txt");
//        write(sockt, image.c_str(), strlen(image.c_str()));
    }

    close(sockt);
    pthread_exit(NULL);
}

void server() {

  int sockfd, newsockfd, portno;
  socklen_t clilen;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;

  sockfd =  socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
     error("ERROR opening socket");

  bzero((char *) &serv_addr, sizeof(serv_addr));

  portno = 8080; //atoi(argv[1]);
  serv_addr.sin_family = AF_INET;

  serv_addr.sin_addr.s_addr = INADDR_ANY;

  serv_addr.sin_port = htons(portno);

  if (bind(sockfd, (struct sockaddr *) &serv_addr,
           sizeof(serv_addr)) < 0)
           error("ERROR on binding");

  listen(sockfd,5);

  clilen = sizeof(cli_addr);

  while((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen))){
    pthread_t t;
    pthread_create(&t, 0, echo, &newsockfd);
  }

  close(sockfd);
}
