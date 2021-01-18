#include <cstdint>
#include <iostream>
#include <string.h>

#include <include/tcp_server.h>

static constexpr char DEF_IP_ADDRESS[] = "127.0.0.1";
static constexpr uint16_t DEF_TCP_PORT = 35000;
static constexpr size_t MAX_SIZE_BUFFER = 255;

int main()
{
    bool isError = false;
    std::vector<char> rxData;
    rxData.reserve(MAX_SIZE_BUFFER);

    // Create Server
    tcp_udp_server::TCP_Server tcpServer(DEF_IP_ADDRESS, DEF_TCP_PORT);

    // Start
    try {
        tcpServer.start();
        std::cout << "starting TCP server on port " << DEF_TCP_PORT << std::endl;
    } catch (...) {
        std::cerr << "error: server init" << std::endl;
        isError = true;
    }

    while (!isError) {
        std::cout << "waiting data..." << std::endl;

        // Прием клиента
        try {
            tcpServer.acceptSocket();
        } catch (...) {
            std::cerr << "error: server accept" << std::endl;
            isError = true;
            continue;
        }

        // Прием данных от клиента
        rxData.resize(MAX_SIZE_BUFFER);
        try {
            tcpServer.receiveData(rxData, MAX_SIZE_BUFFER);
        } catch (...) {
            std::cerr << "error: server receive" << std::endl;
            isError = true;
            continue;
        }

        // Вывод полученных данных
        std::cout << "receive data(size " << rxData.size() << " byte):" << std::endl;
        for (const auto& byte : rxData) {
            std::cout << byte;
        }
        std::cout << std::endl;

        // Отправка эхо клиенту
        try {
            tcpServer.sendData(rxData);
        } catch (...) {
            std::cerr << "error: server receive" << std::endl;
            isError = true;
            continue;
        }

        // Проверка на выход
        static constexpr char END[] = "end";
        static constexpr size_t LENGTH_OF_END = sizeof(END) - 1U;
        if ((rxData.size() == LENGTH_OF_END) && (memcmp(rxData.data(), END, LENGTH_OF_END) == 0)) {
            break;
        }

        rxData.clear();
    }

    tcpServer.stop();
    std::cout << "stop TCP server on port " << DEF_TCP_PORT << std::endl;

    if (isError) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
