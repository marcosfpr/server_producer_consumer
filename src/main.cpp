#include "Server.hpp"
/*
  Main TCP-Server.
 */
int main(int argc, char *argv[])
{
    unsigned int buffer_size = 100;    // n° de sockets que podem ficar na fila para conectar ao server

    ClientConnectedServer handler;   // handler que será executado após o cliente se conectar ao servidor.
                                     // Nesta implementação, o socket é apenas inserido no buffer. (Producer);

    Server* server = Server::getInstance("127.0.0.1", 54011, buffer_size, &handler);

    server->run();

    Server::destroy();

    return 0;
}
