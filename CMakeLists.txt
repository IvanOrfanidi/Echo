# Version CMake
cmake_minimum_required(VERSION 3.10)

# Project name
project(echo VERSION 1.0.1)

configure_file(
    project_version.h.in ${CMAKE_CURRENT_BINARY_DIR}/project_version.h
)

# Option
set(CMAKE_CXX_STANDARD 17)

# Compiler-specific options
if((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    add_compile_options(
        -Werror

        -Wall
        -Wextra
        -Wpedantic

        -Wcast-align
        -Wcast-qual
        -Wenum-compare
        -Wfloat-equal
        -Wnon-virtual-dtor
        -Woverloaded-virtual
        -Wredundant-decls
        -Wsign-promo
    )
endif()


# Source files
file(GLOB_RECURSE SERVER
    source/server/*.cpp
)
file(GLOB_RECURSE CLIENT
    source/client/*.cpp
)

# Set build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

# Compiling target
add_executable(${PROJECT_NAME}_server ${SERVER})
add_executable(${PROJECT_NAME}_client ${CLIENT})

# Link lib TCP/UDP server, client
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/build/tcp_udp_server)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/build/tcp_udp_client)

target_include_directories(${PROJECT_NAME}_server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${PROJECT_NAME}_client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

if(UNIX)
    target_link_libraries(${PROJECT_NAME}_server "${PROJECT_SOURCE_DIR}/build/tcp_udp_server/lib/lib_tcp_udp_server.a")
    target_link_libraries(${PROJECT_NAME}_client "${PROJECT_SOURCE_DIR}/build/tcp_udp_client/lib/lib_tcp_udp_client.a")
endif()
if(WIN32)
    target_link_libraries(${PROJECT_NAME}_server PRIVATE "${PROJECT_SOURCE_DIR}/build/tcp_udp_server/lib/lib_tcp_udp_server.a")
    target_link_libraries(${PROJECT_NAME}_client PRIVATE "${PROJECT_SOURCE_DIR}/build/tcp_udp_client/lib/lib_tcp_udp_client.a")
    target_link_libraries(${PROJECT_NAME}_server PRIVATE wsock32 ws2_32)
    target_link_libraries(${PROJECT_NAME}_client PRIVATE wsock32 ws2_32)
    set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
endif()
