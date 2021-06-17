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

