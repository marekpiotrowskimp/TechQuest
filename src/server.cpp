#include "server.h"
#include "TQCommandLine.h"

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

std::streambuf::int_type NetworkBuff::overflow (int_type c) {
    if (c != traits_type::eof()) {
        c = std::toupper(static_cast<char>(c),getloc());
        char buffer[1];
        buffer[0] = static_cast<char>(c);
        if (socket>=0) {
            send(socket, buffer, 1, 0);
        }
        if (putchar(c) == traits_type::eof()) {
            return traits_type::eof();
        }
    }
    return c;
}

std::streambuf::int_type NetworkBuff::underflow(){
    const int readsize = 30;

    if(gptr() < egptr()) {
        return traits_type::to_int_type(*gptr());
    }
    inputBuffer.clear();
    inputBuffer.resize(readsize + 1);

    int bytesread = read(socket, &inputBuffer[0], static_cast<int>(readsize));

    if(bytesread == 0) {
        return traits_type::eof();
    }

    if(bytesread <0) {
        return traits_type::eof();
    }
    setg(&inputBuffer[0], &inputBuffer[0], &inputBuffer[bytesread]);
    return traits_type::to_int_type(*gptr());
}

NetworkBuff::NetworkBuff(int socket) {
    this->socket = socket;
}

void SocketServer::gameThread(void *param)
{
    pthread_detach(pthread_self());

    Params params = *((Params*)param);

    int sockt = *params.socket;
    NetworkBuff networkBuff = NetworkBuff(sockt);

    std::istream cin = std::istream(&networkBuff);
    std::ostream cout = std::ostream(&networkBuff);

    std::string line;
    auto tqCommandLine = new TQCommandLine([&cout](std::basic_stringstream<char>& print) {
        cout << print.str();
        print.str(std::string());
    });
    tqCommandLine->invitation();
    bool endGame = true;
    while (endGame) {
        cout << std::endl << ">";

        getline(cin, line);

        endGame = !tqCommandLine->analyzeCommand(line);
    }

    close(sockt);
    pthread_exit(NULL);
}

void SocketServer::server() {

    int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

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
      std::cout << "player connected" << std::endl;
      Params params = {&newsockfd};
      pthread_t t;
      pthread_create(&t, 0, (void *(*)(void *))gameThread, &params);
    }

    close(sockfd);
}
