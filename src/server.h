
#ifndef TECHQUEST_SERVER_H
#define TECHQUEST_SERVER_H

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
public:
    std::string inputBuffer;
    NetworkBuff(int socket);

protected:
    virtual int_type overflow (int_type c);
    virtual std::streambuf::int_type underflow();

private:
    int socket = -1;
};

class SocketServer {
    NetworkBuff *socketBuffer;
    static void gameThread(void *param);
public:
    void server();
};

struct Params {
    int *socket;
};

#endif