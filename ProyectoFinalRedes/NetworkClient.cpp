#include "NetworkClient.h"
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
    LoginClientMessage* loginMessage = new LoginClientMessage(playerName);
    std::cout << "Enviando mensaje login tipo " << (size_t)loginMessage->id << '\n';
    socket_.send(*loginMessage, socket_);
    delete loginMessage;
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
