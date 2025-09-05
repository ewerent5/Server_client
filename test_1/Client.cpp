#include <csignal>
#include "TCPClient.h"


TCPClient tcp;

void sig_exit(int) {
    tcp.exit();
    exit(0);
}

int main() {
    signal(SIGINT, sig_exit);

    if (!tcp.setup("127.0.0.1", 11999)) {
        std::cerr << "Failed to connect to server" << std::endl;
        return -1;
    }

    while (true) {
        srand((unsigned)time(nullptr));
        tcp.Send(std::to_string(rand() % 25000));

        std::string rec = tcp.receive();
        if (!rec.empty()) {
            std::cout << "Server Response: " << rec << std::endl;
        }
        Sleep(100); // Windows Sleep (мс)
    }

    std::cin.get();

    return 0;
}