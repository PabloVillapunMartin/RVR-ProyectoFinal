#pragma once

#include <thread>
#include <queue>

#include "Socket.h"
#include "Serializable.h"

class NetworkMessages;
class PiumPiumMasterClient;

// +-----------------------+
// | Clase para el cliente |
// +-----------------------+
class NetworkClient {
public:
    NetworkClient(const char * direccion, const char * puerto, char* playerName, PiumPiumMasterClient* game);
    ~NetworkClient();

    /// <summary>
	/// Inicializa el cliente en el servidor y crea el hilo para recibir mensajes
	/// </summary>
    void start();
    /// <summary>
	/// Envía el mensaje message al servidor
	/// </summary>
    void send(Serializable* message);
    /// <summary>
	/// Procesa los mensajes dependiendo de su tipo
	/// </summary>
    void proccessMessages();

private:
    /// <summary>
	/// Realiza la petición de log al servidor
	/// </summary>
    void login();
    /// <summary>
	/// Recibe los mensajes del servidor
	/// </summary>
    void recieve_thread();

    char* playerName;   //nombre del jugador                  

    PiumPiumMasterClient* gameClient;       //clase del juego

    Socket socket_;                         //socket del cliente-servidor
    std::thread incomingMessagesThread_;    //hilo para recibir mensajes
};

