#include "NetworkServer.h"
#include <mutex>

std::mutex mutServer;
std::queue<Serializable*> messagesServer_;
//================================ SERVER =======================================

void NetworkServer::start(){
    std::cout << "[Server] Starting server\n";
    incomingMessagesThread_ = std::thread(&NetworkServer::recieve_thread, this);
}

void NetworkServer::proccessMessages(){
    mutServer.lock();
    mutServer.unlock();
}

void NetworkServer::addClient(Socket* clientSocket, LoginClientMessage* msg){
    if(isAlreadyRegistered(clientSocket)) return;

    std::cout << "[Server] Adding a new client: " << msg->name_ << '\n';
    std::unique_ptr<Socket> loginRequest(clientSocket);
    clients.emplace_back(std::move(loginRequest));
    
    messagesServer_.push(msg);

    ConfirmationLoginMessage confirmationMessage(clients.size() - 1);
    socket.send(confirmationMessage, *clients[clients.size()-1].get());
    //piumpium->addClient(); ->se añade el go de un cliente(con sus componentes)
}

void NetworkServer::removeClient(Socket* clientSocket){
    std::vector<std::unique_ptr<Socket>>::iterator it = clients.begin();

    while(it != clients.end()){
        if(*it->get() == *clientSocket) break;
        it++;
    } 

    if(it != clients.end()) clients.erase(it);
}

void NetworkServer::broadcastMessage(Socket* clientSocket, Serializable* message){
    for(int i = 0; i < clients.size(); ++i){
        if(*clients[i].get() == *clientSocket) continue;    //No enviar al cliente origen del mensaje
        socket.send(*message, *clients[i]);
    }
}

bool NetworkServer::isAlreadyRegistered(Socket* client){
    for(int i = 0; i < clients.size(); ++i){
        if(*clients[i].get() == *client) return true;
    }
    return false;
}

void NetworkServer::recieve_thread(){
    while(true){
        NetworkMessage nm;
        char* msData;
        Socket* clientSock = nullptr;
    
        socket.recv(nm, clientSock, msData);
        std::cout << "Mensaje recibido de tipo " << (size_t)nm.id << "\n"; 

        mutServer.lock();
        switch ((size_t)nm.id)
        {
        case MsgId::_LOG_IN_CLIENT : {
            LoginClientMessage* loginClientMessage = new LoginClientMessage();
            loginClientMessage->from_bin(msData);
            addClient(clientSock, loginClientMessage);
            break;
        }
        case MsgId::_CLIENT_READY :
            std::cout << "Client ready\n";
            break;
        default:
            break;
        }
        mutServer.unlock();
    }
}