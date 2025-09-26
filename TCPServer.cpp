#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET serverSocket;
    sockaddr_in serverAddr{};

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "WSAStartup failed!" << std::endl;
        return 1;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Could not create socket!" << std::endl;
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // слушаем все интерфейсы
    serverAddr.sin_port = htons(11999);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed! Error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed! Error: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server listening on port 11999..." << std::endl;

    // список клиентов
    fd_set masterSet, readSet;
    FD_ZERO(&masterSet);
    FD_SET(serverSocket, &masterSet);

    SOCKET maxSocket = serverSocket;

    while (true) {
        readSet = masterSet; // копируем, так как select изменяет set
        int socketCount = select(0, &readSet, nullptr, nullptr, nullptr);

        if (socketCount == SOCKET_ERROR) {
            std::cerr << "select() failed! Error: " << WSAGetLastError() << std::endl;
            break;
        }

        // проверяем все сокеты
        for (SOCKET s = 0; s <= maxSocket; s++) {
            if (FD_ISSET(s, &readSet)) {
                if (s == serverSocket) {
                    // новое подключение
                    sockaddr_in clientAddr{};
                    int clientSize = sizeof(clientAddr);
                    SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);

                    if (clientSocket != INVALID_SOCKET) {
                        FD_SET(clientSocket, &masterSet);
                        if (clientSocket > maxSocket) {
                            maxSocket = clientSocket;
                        }
                        std::cout << "New client connected! Socket: " << clientSocket << std::endl;
                    }
                }
                else {
                    // получаем данные от клиента
                    char buffer[4096];
                    ZeroMemory(buffer, sizeof(buffer));
                    int bytesReceived = recv(s, buffer, sizeof(buffer), 0);

                    if (bytesReceived > 0) {
                        std::string msg(buffer, bytesReceived);
                        std::cout << "Client " << s << " says: " << msg << std::endl;

                        // рассылаем всем клиентам
                        for (SOCKET outSock = 0; outSock <= maxSocket; outSock++) {
                            if (FD_ISSET(outSock, &masterSet)) {
                                if (outSock != serverSocket && outSock != s) {
                                    send(outSock, msg.c_str(), static_cast<int>(msg.size()), 0);
                                }
                            }
                        }
                    }
                    else {
                        // клиент отключился
                        std::cout << "Client " << s << " disconnected." << std::endl;
                        closesocket(s);
                        FD_CLR(s, &masterSet);
                    }
                }
            }
        }
    }

    // очищаем всё
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
