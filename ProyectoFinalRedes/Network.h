#pragma once

#include <thread>
#include <queue>

#include "Socket.h"
#include "NetworkMessages.h"
#include "Serializable.h"


// +-----------------------+
// | Clase para el cliente |
// +-----------------------+
class NetworkClient {
public:
    NetworkClient(const char * direccion, const char * puerto, char* playerName);
    ~NetworkClient();

    //Inicializa el cliente en el servidor y crea el hilo para recibir mensajes
    void start();
    //Envia mensajes al servidor
    void send(Serializable* message);
    void proccessMessages();

private:
    void login();
    void recieve_thread();

    char* playerName;
    Socket socket_;
    std::thread incomingMessagesThread_;
};

// +------------------------+
// | Clase para el servidor |
// +------------------------+
class NetworkServer {
public:
    NetworkServer(const char * s, const char * p): socket(s, p)
    {
        socket.bind();
    };
    
    void start();
    void proccessMessages();

private:
    void addClient(Socket* clientSocket, LoginClientMessage* message);
    void removeClient(Socket* clientSocket);
    void broadcastMessage(Socket* clientSocket, Serializable* message);
    bool isAlreadyRegistered(Socket* client);

    void recieve_thread();

    //Lista de clientes conectados al servidor de Chat, representados por su socket
    std::vector<std::unique_ptr<Socket>> clients;

    //Socket del servidor
    Socket socket;

    //Thread de la lectura de mensajes de clientes
    std::thread incomingMessagesThread_;
};
