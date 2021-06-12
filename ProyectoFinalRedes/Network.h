#include <thread>
#include <queue>

#include "Socket.h"
#include "Serializable.h"
// +-----------------------+
// | Clase para el cliente |
// +-----------------------+
class NetworkClient {
public:
    NetworkClient(const char * direccion, const char * puerto);
    ~NetworkClient();

    //Inicializa el cliente en el servidor y crea el hilo para recibir mensajes
    void start();
    //Envia mensajes al servidor
    void send(Serializable* message);
    void proccessMessages();

private:
    void login();
    void recieve_thread();

    Socket socket_;
    std::queue<Serializable*> messages_;
};

// +------------------------+
// | Clase para el servidor |
// +------------------------+
class NetworkServer {
public:
 public:
    NetworkServer(const char * s, const char * p): socket(s, p)
    {
        socket.bind();
    };

    void proccessMessages();

private:
    void addClient(Socket* clientSocket, Serializable* message);
    void removeClient(Socket* clientSocket, Serializable* message);
    void broadcastMessage(Socket* clientSocket, Serializable* message);

    void recieve_thread();


    //Lista de clientes conectados al servidor de Chat, representados por su socket
    std::vector<std::unique_ptr<Socket>> clients;

    //Socket del servidor
    Socket socket;
};
