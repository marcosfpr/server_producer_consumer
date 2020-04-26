cmake_minimum_required(VERSION 3.10)
project(server)

set (CMAKE_CXX_STANDARD 11)

find_package (Threads)

add_executable (server src/main.cpp src/Server.cpp src/SocketConsumer.cpp src/TcpListener.cpp)

target_link_libraries (server ${CMAKE_THREAD_LIBS_INIT})