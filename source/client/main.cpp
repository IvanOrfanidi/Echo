#include <cstdint>
#include <iostream>
#include <string.h>

#include <include/tcp_client.h>

static constexpr char DEF_IP_ADDRESS[] = "127.0.0.1";
static constexpr uint16_t DEF_TCP_PORT = 35000;
static constexpr size_t MAX_SIZE_BUFFER = 255;

int main()
{
    bool isError = false;
    std::vector<char> txData;
    txData.reserve(MAX_SIZE_BUFFER);
    std::vector<char> rxData;
    rxData.reserve(MAX_SIZE_BUFFER);

    tcp_udp_client::TCP_Client tcpClient(DEF_IP_ADDRESS, DEF_TCP_PORT);
    try {
        tcpClient.start();
        std::cout << "starting TCP client on port " << DEF_TCP_PORT << std::endl;
    } catch (...) {
        std::cerr << "error: client init" << std::endl;
        isError = true;
    }

    while (!isError) {
        std::cout << "please, enter your string:" << std::endl;

        // Считываем строку
        std::string str;
        std::getline(std::cin, str);
        txData.resize(MAX_SIZE_BUFFER);
        try {
            std::copy(str.cbegin(), str.cend(), txData.begin());
        } catch (...) {
            std::cerr << "error: string is too large" << std::endl;
            isError = true;
            continue;
        }
        txData.resize(str.length());

        // Подключение если есть данные
        try {
            tcpClient.connecting();
        } catch (...) {
            std::cerr << "error: client connecting" << std::endl;
            isError = true;
            continue;
        }

        // Отправка данных
        try {
            tcpClient.sendData(txData);
        } catch (...) {
            std::cerr << "error: client send" << std::endl;
            isError = true;
            continue;
        }
        std::cout << "data was sent(size " << txData.size() << " byte):" << std::endl;

        // Прием ответа
        rxData.resize(MAX_SIZE_BUFFER);
        try {
            tcpClient.receiveData(rxData, MAX_SIZE_BUFFER);
        } catch (...) {
            std::cerr << "error: client receive" << std::endl;
            isError = true;
            continue;
        }

        // Проверка ответа
        if (rxData == txData) {
            std::cout << "answer is correct" << std::endl;
        } else {
            std::cerr << "error: answer is not correct" << std::endl;
            isError = true;
            continue;
        }

        // Проверка на выход
        static constexpr char END[] = "end";
        static constexpr size_t LENGTH_OF_END = sizeof(END) - 1U;
        if ((rxData.size() == LENGTH_OF_END) && (memcmp(rxData.data(), END, LENGTH_OF_END) == 0)) {
            break;
        }

        txData.clear();
        rxData.clear();
    }

    tcpClient.stop();
    std::cout << "stop TCP client on port " << DEF_TCP_PORT << std::endl;

    if (isError) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
