#ifndef CSERVER_SOCKETCONSUMER_HPP
#define CSERVER_SOCKETCONSUMER_HPP

#include "Resource.hpp"
#include "TcpListener.hpp"

namespace pcr{
    class SocketConsumer : public pcr::ConsumeHandler<int>{
    public:
        void handle(int socketId) override;
    };
};

#endif //CSERVER_SOCKETCONSUMER_HPP
