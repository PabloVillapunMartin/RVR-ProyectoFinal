#include "Network.h"

//========================================= CLIENT =============================================

NetworkClient::NetworkClient(const char * direccion, const char * puerto) : socket_(direccion,puerto){

}

NetworkClient::~NetworkClient(){
    while(!messages_.empty()){
        delete messages_.front();
        messages_.pop();
    }
}

void NetworkClient::start(){
    incomingMessagesThread_ = std::thread(&NetworkClient::recieve_thread, this);
    login();
}

void NetworkClient::send(Serializable* message){
    socket_.send(*message, socket_);
}

void NetworkClient::proccessMessages(){

}

void NetworkClient::login(){
    std::cout << "[Client] Login\n";
    TestingMessage loginMessage = TestingMessage(5);
    socket_.send(loginMessage, socket_);
}

void NetworkClient::recieve_thread(){
    while(true){
        TestingMessage* ms = new TestingMessage(3);

        socket_.recv(*ms);

        std::cout << "[Client] Message recv: " << ms->win << '\n';
    }
}

//================================ SERVER =======================================

void NetworkServer::start(){
    std::cout << "[Server] Starting server\n";
    incomingMessagesThread_ = std::thread(&NetworkServer::recieve_thread, this);
}

void NetworkServer::proccessMessages(){

}

void NetworkServer::addClient(Socket* clientSocket, Serializable* message){
    if(isAlreadyRegistered(clientSocket)) return;

    std::cout << "[Server] Adding a new client\n";
    std::unique_ptr<Socket> loginRequest(new Socket(*clientSocket));
    clients.emplace_back(std::move(loginRequest));
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
        TestingMessage ms = TestingMessage(0);

        socket.recv(ms);

        std::cout << "[Server] Message recv: " << ms.win << '\n';
    }
}