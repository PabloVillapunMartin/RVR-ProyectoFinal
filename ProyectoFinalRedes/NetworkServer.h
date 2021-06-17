#pragma once

#include <thread>
#include <queue>

#include "Socket.h"
#include "NetworkMessages.h"
#include "Serializable.h"

// +------------------------+
// | Clase para el servidor |
// +------------------------+
class NetworkServer {
public:
    NetworkServer(const char * s, const char * p): socket(s, p), playersReady(0)
    {
        socket.bind();
    };
    
    void start();
    void proccessMessages();

private:
    void addClient(Socket* clientSocket, LoginClientMessage* message);
    void removeClient(Socket* clientSocket);
    void broadcastMessage(Serializable* message);
    bool isAlreadyRegistered(Socket* client);

    void recieve_thread();

    //Lista de clientes conectados al servidor de Chat, representados por su socket
    std::vector<std::unique_ptr<Socket>> clients;

    //Socket del servidor
    Socket socket;

    //Thread de la lectura de mensajes de clientes
    std::thread incomingMessagesThread_;

    int playersReady;
};
