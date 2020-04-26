#include <thread>

#include "TcpListener.hpp"

namespace net{

  TcpListener::TcpListener(std::string ipAddress, int port, ClientConnectedHandler* handler)
    : ipAddress_(ipAddress), port_(port), handler_(handler) {}

  TcpListener::~TcpListener(){
    closesocket(this->listener_);
    cleanUp();
  }
  
  void TcpListener::sendMessage(int clientSocket, std::string msg){
    send(clientSocket, msg.c_str(), msg.size() + 1, 0);
  }

    // Initialize Socket
  bool TcpListener::startSocket(){
#ifdef _WIN32
    WSAData data;
    WORD ver = MAKEWORD(2,2);

    int wsInit = WSAStartup(ver, &data);

    return wsInit == 0;
#else
    return true;
#endif
  }

    // Receive loop
  void TcpListener::runListener(){

    this->listener_ = createSocket();

    while (true) {
      int client = waitConnection();

      if(client != SOCKET_ERROR){
	    std::thread new_thread(&ClientConnectedHandler::handle, this->handler_, client);
	    new_thread.detach();
      }
    }
  }
    
    // Cleanup
  void TcpListener::cleanUp(){
#ifdef _WIN32
        WSACleanup();
#endif
  }
    
    // Create a socket
  int TcpListener::createSocket(){
    int listening = socket(AF_INET, SOCK_STREAM, 0);

    if (listening != SOCKET_ERROR) {

      struct sockaddr_in hint;
      hint.sin_family = AF_INET;
      hint.sin_addr.s_addr = INADDR_ANY;
      hint.sin_port = htons(this->port_);
      inet_pton(AF_INET, this->ipAddress_.c_str(), &hint.sin_addr);
      

      int bindOk = bind(listening, (struct sockaddr*) &hint, sizeof(hint));

      if (bindOk != SOCKET_ERROR){
        int listenOk = listen(listening, 3);
        if (listenOk == SOCKET_ERROR){
          return SOCKET_ERROR;
        }
      }
      else{
        return SOCKET_ERROR;
      }
    }
    return listening;
  }
    
    // Wait for a connection
  int TcpListener::waitConnection(){
    int client = accept(this->listener_, NULL, NULL);
    return client;
  }
   
};
