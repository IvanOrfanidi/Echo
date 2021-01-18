#pragma once

#include <iostream>
#include <string.h>

#include <project_version.h>

static constexpr char DEF_IP_ADDRESS[] = "127.0.0.1";
static constexpr char DEF_TCP_PORT[] = "35000";
static constexpr char DEF_UDP_PORT[] = "35100";
static constexpr size_t MAX_SIZE_BUFFER = 255;

static constexpr char END[] = "end";
static constexpr size_t LENGTH_OF_END = sizeof(END) - 1U;

// IP and Port
struct Config {
    std::string ip;
    std::string port;
};

inline Config GetConfigurationFromCommands(int argc, char* argv[])
{
    Config config;
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "-a" || arg == "--address") {
            config.ip = argv[++i];
        } else if (arg == "-p" || arg == "--port") {
            config.port = argv[++i];
        } else if (arg == "-v" || arg == "--version") {
            std::cout << PROJECT_VERSION << std::endl;
            exit(EXIT_SUCCESS);
        } else {
            std::cerr << "error: unknown command '" << arg << "'" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    return config;
}