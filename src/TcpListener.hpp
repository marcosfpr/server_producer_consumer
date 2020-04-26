#ifndef CSERVER_TCPLISTENER_HPP_
#define CSERVER_TCPLISTENER_HPP_

#include <string>

#ifdef _WIN32

#include <ws2tcpip.h> // winsock functions
#include <winsock2.h>
extern "C" {
    WINSOCK_API_LINKAGE  INT WSAAPI inet_pton( INT Family, PCSTR pszAddrString, PVOID pAddrBuf);
    WINSOCK_API_LINKAGE  PCSTR WSAAPI inet_ntop(INT  Family, PVOID pAddr, PSTR pStringBuf, size_t StringBufSize);
}

#else

#include <sys/socket.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define closesocket close

#endif

#define MAX_BUFFER_SIZE 4096
#define SOCKET_ERROR -1

namespace net{

    class ClientConnectedHandler{
    public:
        virtual void handle(int socketId) = 0;
    };

    class TcpListener{
    public:

        TcpListener(std::string ipAddress, int port, ClientConnectedHandler* handler);
        virtual ~TcpListener();

        static void sendMessage(int clientSocket, std::string msg);

        // Initialize Socket
        bool startSocket();

        // Receive loop
        void runListener();

        // Cleanup
        void cleanUp();

    private:

        TcpListener(const TcpListener& tcpListener); // block copy constructor
        TcpListener& operator=(const TcpListener& tcpListener); // block copy constructor

        // Create a socket
        int createSocket();

        // Wait for a connection
        int waitConnection();

        std::string ipAddress_;
        int port_, listener_;
        ClientConnectedHandler* handler_;

    };
  
};


#endif // CSERVER_TCPLISTENER_HPP_
