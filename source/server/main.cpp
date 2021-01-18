#include <cstdint>
#include <iostream>
#include <string.h>

#include <common.h>
#include <include/tcp_server.h>

int main(int argc, char* argv[])
{
    bool isError = false;
    std::vector<char> rxData;
    rxData.reserve(MAX_SIZE_BUFFER);

    Config config = GetConfigurationFromCommands(argc, argv);
    if (config.ip.length() == 0) {
        std::cout << "Please enter a valid IP address of the TCP server(default 127.0.0.1):" << std::endl;
        std::getline(std::cin, config.ip);
        if (config.ip.length() == 0) {
            config.ip = DEF_IP_ADDRESS;
        }
    }
    if (config.port.length() == 0) {
        std::cout << "Please enter a valid port of the TCP server(default 35000):" << std::endl;
        std::getline(std::cin, config.port);
        if (config.port.length() == 0) {
            config.port = DEF_TCP_PORT;
        }
    }

    // Создаем TCP сервер
    tcp_udp_server::TCP_Server tcpServer(config.ip.data(), atoi(config.port.data()));

    // Старт TCP сервера
    try {
        tcpServer.start();
        std::cout << "starting TCP server on port " << config.port << std::endl;
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
        if ((rxData.size() == LENGTH_OF_END) && (memcmp(rxData.data(), END, LENGTH_OF_END) == 0)) {
            break;
        }

        rxData.clear();
    }

    tcpServer.stop();
    std::cout << "stop TCP server on port " << config.port << std::endl;

    if (isError) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
