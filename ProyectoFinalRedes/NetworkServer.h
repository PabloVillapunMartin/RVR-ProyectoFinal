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
    /// <summary>
	/// Contructora
	/// </summary>
    NetworkServer(const char * s, const char * p, PiumPiumMasterServer* masterServer): socket(s, p), playersReady(0), gameServer(masterServer)
    {
        socket.bind();
    };
    /// <summary>
	/// Destructora
	/// </summary>
    ~NetworkServer();

    /// <summary>
	/// Inicializa el servidor
	/// </summary>
    void start();
    /// <summary>
	/// Procesa los mensajes de la cola dependiendo de su tipo
	/// </summary>
    void proccessMessages();
    /// <summary>
	/// Envía el mensaje message a todos los clientes
	/// </summary>
    void broadcastMessage(Serializable* message);

private:
    /// <summary>
	/// Añade un cliente al vector
	/// </summary>
    void addClient(Socket* clientSocket, LoginClientMessage* message);
    /// <summary>
	/// Elimina un cliente del vector
	/// </summary>
    void removeClient(Socket* clientSocket);
    /// <summary>
	/// Comprueba si el socket ya está registrado
	/// </summary>
    bool isAlreadyRegistered(Socket* client);

    /// <summary>
	/// Recibe los mensajes de los clientes
	/// </summary>
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
