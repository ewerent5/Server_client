#pragma once


#ifndef TCP_LIENT_H
#define TCP_LIENT_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <vector>
#pragma comment(lib, "Ws2_32.lib")



class TCPClient {
public:
    TCPClient();
    ~TCPClient();

    bool setup(const std::string& address, int port);
    bool Send(const std::string& data);
    std::string receive(int size = 4096);
    std::string read();
    void exit();

private:
    SOCKET sock;
    std::string address;
    int port;
    struct sockaddr_in server;
    bool wsaInitialized;
};

#endif