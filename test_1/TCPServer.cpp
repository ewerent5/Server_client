#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr {}, clientAddr{};
    int clientSize = sizeof(clientAddr);

    // Инициализация WinSock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "WSAStartup failed! Error: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // Создаем сокет
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Could not create socket! Error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Настройка адреса
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = InetPton(AF_INET, L"127.0.0.1", &serverAddr.sin_addr); // только localhost
    serverAddr.sin_port = htons(11999);

    // Привязка
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed! Error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Ожидание подключений
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed! Error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server listening on 127.0.0.1:11999 ..." << std::endl;

    // Принимаем клиента
    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed! Error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Client connected!" << std::endl;

    // Основной цикл
    char buffer[4096];
    while (true) {
        ZeroMemory(buffer, sizeof(buffer));

        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            std::string msg(buffer, bytesReceived);
            std::cout << "Received: " << msg << std::endl;

            std::string response = "OK: " + msg;
            send(clientSocket, response.c_str(), static_cast<int>(response.size()), 0);
        }
        else if (bytesReceived == 0) {
            std::cout << "Client disconnected." << std::endl;
            break;
        }
        else {
            std::cerr << "Recv failed! Error: " << WSAGetLastError() << std::endl;
            break;
        }
    }

    // Закрываем соединения
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    std::cout << 2222;
    std::cin.get();
    return 0;
}
