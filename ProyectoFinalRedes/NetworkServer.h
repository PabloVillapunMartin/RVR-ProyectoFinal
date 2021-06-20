#pragma once

#include <thread>
#include <queue>

#include "Socket.h"
#include "NetworkMessages.h"
#include "Serializable.h"

class PiumPiumMasterServer;

// +------------------------+
// | Clase para el servidor |
// +------------------------+
class NetworkServer {
public:
    NetworkServer(const char * s, const char * p, PiumPiumMasterServer* masterServer): socket(s, p), playersReady(0), gameServer(masterServer)
    {
        socket.bind();
    };

    ~NetworkServer();
    
    void start();
    void proccessMessages();
    void broadcastMessage(Serializable* message);

private:
    void addClient(Socket* clientSocket, LoginClientMessage* message);
    void removeClient(Socket* clientSocket);
    bool isAlreadyRegistered(Socket* client);

    void recieve_thread();

    //Referencia del juego
    PiumPiumMasterServer* gameServer;

    //Lista de clientes conectados al servidor de Chat, representados por su socket
    std::vector<std::unique_ptr<Socket>> clients;

    //Socket del servidor
    Socket socket;

    //Thread de la lectura de mensajes de clientes
    std::thread incomingMessagesThread_;

    int playersReady;
};
