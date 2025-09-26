#include "TCPClient.h"

TCPClient::TCPClient() {
    sock = INVALID_SOCKET;
    port = 0;
    address = "";
    wsaInitialized = false;
}

TCPClient::~TCPClient() {
    exit();
}

bool TCPClient::setup(const std::string& addr, int prt) {
    WSADATA wsa;
    if (!wsaInitialized) {
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
            std::cerr << "WSAStartup failed!" << std::endl;
            return false;
        }
        wsaInitialized = true;
    }

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Could not create socket! Error: " << WSAGetLastError() << std::endl;
        return false;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(prt);

    // Пробуем распарсить как IP
    if (inet_pton(AF_INET, addr.c_str(), &server.sin_addr) <= 0) {
        // Если не IP — пробуем DNS
        struct addrinfo hints {}, * result = nullptr;
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        if (getaddrinfo(addr.c_str(), nullptr, &hints, &result) != 0) {
            std::cerr << "Failed to resolve hostname!" << std::endl;
            return false;
        }

        struct sockaddr_in* ipv4 = (struct sockaddr_in*)result->ai_addr;
        server.sin_addr = ipv4->sin_addr;
        freeaddrinfo(result);
    }

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        std::cerr << "Connect failed! Error: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        sock = INVALID_SOCKET;
        return false;
    }

    return true;
}

bool TCPClient::Send(const std::string& data) {
    if (sock == INVALID_SOCKET) return false;

    int sent = send(sock, data.c_str(), static_cast<int>(data.size()), 0);
    if (sent == SOCKET_ERROR) {
        std::cerr << "Send failed! Error: " << WSAGetLastError() << std::endl;
        return false;
    }
    return true;
}

std::string TCPClient::receive(int size) {
    if (sock == INVALID_SOCKET) return "";

    std::vector<char> buffer(size, 0);
    int bytes = recv(sock, buffer.data(), size - 1, 0);
    if (bytes == SOCKET_ERROR) {
        std::cerr << "Receive failed! Error: " << WSAGetLastError() << std::endl;
        return "";
    }

    return std::string(buffer.data(), bytes);
}

std::string TCPClient::read() {
    if (sock == INVALID_SOCKET) return "";

    std::string reply;
    char buffer[1];
    while (true) {
        int bytes = recv(sock, buffer, 1, 0);
        if (bytes <= 0) break;
        reply += buffer[0];
        if (buffer[0] == '\n') break;
    }
    return reply;
}

void TCPClient::exit() {
    if (sock != INVALID_SOCKET) {
        closesocket(sock);
        sock = INVALID_SOCKET;
    }
    if (wsaInitialized) {
        WSACleanup();
        wsaInitialized = false;
    }
}
