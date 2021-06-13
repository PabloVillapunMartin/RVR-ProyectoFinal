#include "Network.h"
#include <mutex>

std::mutex mut;
std::queue<Serializable*> messages_;
//========================================= CLIENT =============================================

NetworkClient::NetworkClient(const char * direccion, const char * puerto, char* _playerName) : socket_(direccion,puerto), playerName(_playerName){

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
    mut.lock();
    mut.unlock();
}

void NetworkClient::login(){
    std::cout << "[Client] Login\n";
    LoginClientMessage loginMessage(playerName);

    socket_.send(loginMessage, socket_);
}

void NetworkClient::recieve_thread(){
    while(true){
        NetworkMessage nm;
        char* msData;

        socket_.recv(nm, msData);

        mut.lock();
        switch (nm.id)
        {
        case MsgId::_CONFIRMATION_LOGIN :{
            ConfirmationLoginMessage* confirmationLogin = new ConfirmationLoginMessage();
            confirmationLogin->from_bin(msData);
            messages_.push(confirmationLogin);
            std::cout << "Mensaje de confirmacion de login\n";
            break;
        }
        case MsgId::_START_GAME :{
            StartGameMessage* startGame = new StartGameMessage();
            startGame->from_bin(msData);
            messages_.push(startGame);
            std::cout << "Mensaje de start game\n";
            break;
        }
        default:
            std::cout << "Mensaje no identificado\n";
            break;
        }
        mut.unlock();
    }
}

//================================ SERVER =======================================

void NetworkServer::start(){
    std::cout << "[Server] Starting server\n";
    incomingMessagesThread_ = std::thread(&NetworkServer::recieve_thread, this);
}

void NetworkServer::proccessMessages(){
    mut.lock();
    mut.unlock();
}

void NetworkServer::addClient(Socket* clientSocket, LoginClientMessage* msg){
    if(isAlreadyRegistered(clientSocket)) return;

    std::cout << "[Server] Adding a new client: " << msg->name_ << '\n';
    std::unique_ptr<Socket> loginRequest(new Socket(*clientSocket));
    clients.emplace_back(std::move(loginRequest));

    messages_.push(msg);

    ConfirmationLoginMessage confirmationMessage(clients.size() - 1);
    socket.send(confirmationMessage, *clientSocket);
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
        Socket* clientSock;

        socket.recv(nm, clientSock, msData);

        mut.lock();
        switch (nm.id)
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
        mut.unlock();
    }
}