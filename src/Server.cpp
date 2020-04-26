#include "Server.hpp"

#include "Resource.hpp"
#include "SocketConsumer.hpp"

#include <vector>

Server* Server::instance_ = nullptr;

Server* Server::getInstance(std::string ipAddress, int port, unsigned int size_b, ClientConnectedServer* handler) {
    if(Server::instance_ == nullptr){
        Server::instance_ = new Server(ipAddress, port, size_b, handler);
    }
    return Server::instance_;
}

void Server::destroy() {
    delete instance_;
    instance_ = 0;
}

Server::Server(std::string ipAddress, int port, unsigned int size_b, ClientConnectedServer* handler)
    : tcpListener_(ipAddress, port, handler), producer_(nullptr) {
    this->buffer_ = std::make_shared<pcr::Buffer<int>>(size_b);
    this->producer_ = pcr::Producer<int>(this->buffer_);
}

Server::~Server() {
    this->tcpListener_.cleanUp();
}

void Server::run(){
    pcr::SocketConsumer consumer_handler;

    std::vector<pcr::Consumer<int>> consumers;
    consumers.resize(SERVER_THREADS, pcr::Consumer<int>(this->buffer_, &consumer_handler));

    std::vector<std::thread> consumer_pool;
    for(auto it = consumers.begin(); it != consumers.end(); ++it){
        pcr::Consumer<int> cons = *it;
        consumer_pool.push_back(std::thread(&pcr::Consumer<int>::consume, &cons));
    }

    if (tcpListener_.startSocket()){
        tcpListener_.runListener();
    }

    for(auto it = consumer_pool.begin(); it != consumer_pool.end(); ++it){
        std::thread* thr = &(*it);
        thr->join();
    }
}

pcr::Producer<int> Server::getProducer() const{
    return producer_;
}

void ClientConnectedServer::handle(int p) {
    Server* server = Server::getInstance();
    server->getProducer().produce(p);
}

