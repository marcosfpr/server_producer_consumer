#ifndef CSERVER_SERVER_HPP
#define CSERVER_SERVER_HPP


#include <string>
#include <thread>
#include <memory>
#include <utility>

#include "TcpListener.hpp"
#include "Resource.hpp"
#include "SocketConsumer.hpp"

#define SERVER_THREADS std::thread::hardware_concurrency() * 10

class ClientConnectedServer: public net::ClientConnectedHandler{
    void handle(int socketId) override;
};

class Server{
public:

    static Server* getInstance(std::string ipAddress = "127.0.0.1", int port = 8080, unsigned int size_b = 100,
            ClientConnectedServer* handler = nullptr);

    static void destroy();

    void run();
    pcr::Producer<int> getProducer() const;

    virtual ~Server();

protected:
    Server(std::string ipAddress, int port, unsigned int size_b, ClientConnectedServer* handler);

private:
    Server(const Server& s);
    Server& operator=(const Server& s);

    net::TcpListener tcpListener_;
    std::shared_ptr<pcr::Buffer<int>> buffer_;
    pcr::Producer<int> producer_;

    static Server* instance_;
};


#endif //CSERVER_SERVER_HPP
