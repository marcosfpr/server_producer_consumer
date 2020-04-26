#include <iostream>
#include <cstring>
#include <sstream>

#include "SocketConsumer.hpp"
#include "TcpListener.hpp"

namespace pcr
{
    void SocketConsumer::handle(int socketId) {

        char buf[MAX_BUFFER_SIZE];

        int bytesReceived = 0;

        do{

            memset(buf, 0, MAX_BUFFER_SIZE);

            bytesReceived = recv(socketId, buf, MAX_BUFFER_SIZE, 0);
#ifdef DEBUG
            pcr::coutMutex.lock();
            std::cout << "Bytes received from client " << socketId << " : " << buf << "(" << bytesReceived << ")" << std::endl;
            std::flush(std::cout);
            pcr::coutMutex.unlock();
#endif
            std::ostringstream ss;
            ss << "Ok client " << socketId << "!";
            net::TcpListener::sendMessage(socketId, ss.str());

        }while(bytesReceived - 2 > 0);

        closesocket(socketId);
    }
};
