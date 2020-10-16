#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fstream>
#include <streambuf>
#include <exception>
#include <iostream>
#include <vector>


class NetworkBuff : public std::streambuf { ;
    std::vector<std::function<int(int, char *, size_t)>> listOfRead;
public:
    std::string inputBuffer;
    NetworkBuff();
    void setSocket(int socket);
    void setReadMethod(std::function<int(int, char *, size_t)> readMethod);

protected:
    virtual int_type overflow (int_type c);
    virtual std::streambuf::int_type underflow();

private:
    int socket = -1;
};

class SocketServer {
    NetworkBuff *socketBuffer;
    static void* gameThread(void *param);
public:
    void server();
};

struct Params {
    int *socket;
    NetworkBuff *buffor;
};