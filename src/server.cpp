#include "server.h"
#include "TQCommandLine.h"

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void NetworkBuff::setSocket(int socket) {
    this->socket = socket;
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

    auto readMethod = listOfRead.front();

    int bytesread = readMethod(socket, &inputBuffer[0], static_cast<int>(readsize));

    if(bytesread == 0) {
        return traits_type::eof();
    }

    if(bytesread <0) {
        return traits_type::eof();
    }
    setg(&inputBuffer[0], &inputBuffer[0], &inputBuffer[bytesread]);
    return traits_type::to_int_type(*gptr());
}

NetworkBuff::NetworkBuff() {
    srand (time(NULL));
    std::cout<< (int)(rand()) << "  buffer index" << std::endl;
}

void NetworkBuff::setReadMethod(std::function<int(int, char *, size_t)> readMethod) {
    listOfRead.push_back(readMethod);
    std::cout<< listOfRead.size() << "  " << std::endl;
}

void* SocketServer::gameThread(void *param)
{
    pthread_detach(pthread_self());

    Params params = *((Params*)param);

    int sockt = *params.socket;
    NetworkBuff *networkBuff = params.buffor;

    networkBuff->setReadMethod( [](auto fd,auto buf,auto nbyte) -> int {
        return read(fd, &buf[0], static_cast<int>(nbyte));
    } );

    auto bufferCin = std::cin.rdbuf();
    auto bufferCout = std::cout.rdbuf();
    std::cout.rdbuf(networkBuff);
    std::cin.rdbuf(networkBuff);

    std::string line;
    auto *tqCommandLine = new TQCommandLine([networkBuff, &sockt](){
        networkBuff->setSocket(sockt);
    });
    tqCommandLine->invitation();
    bool endGame = true;
    while (endGame) {
        std::cout << std::endl << ">";

        getline(std::cin, line);

        endGame = !tqCommandLine->analyzeCommand(line);
    }

    close(sockt);
    std::cout.rdbuf(bufferCout);
    std::cin.rdbuf(bufferCin);
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

    socketBuffer = new NetworkBuff();

    listen(sockfd,5);

    clilen = sizeof(cli_addr);

    while((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen))){
      std::cout << "player connected" << std::endl;
      Params params = {&newsockfd, &*socketBuffer};
      pthread_t t;
      pthread_create(&t, 0, gameThread, &params);
    }

    close(sockfd);
}
