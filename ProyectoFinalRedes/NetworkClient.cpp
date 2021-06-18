#include "NetworkClient.h"
#include "NetworkMessages.h"
#include "PiumPiumMasterClient.h"
#include "SDLGame.h"
#include "Manager.h"
#include "GameState.h"

#include <mutex>

std::mutex mut;
std::queue<NetworkMessage*> messages_;
//========================================= CLIENT =============================================

NetworkClient::NetworkClient(const char * direccion, const char * puerto, char* _playerName, PiumPiumMasterClient* game) : 
socket_(direccion,puerto), 
playerName(_playerName),
gameClient(game)
{

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
    while(!messages_.empty()){
        NetworkMessage* msg = messages_.front(); messages_.pop();
        switch ((size_t)msg->id)
        {
        case MsgId::_CONFIRMATION_LOGIN:{
            ConfirmationLoginMessage* login = static_cast<ConfirmationLoginMessage*>(msg);
            gameClient->setID(login->gameObjectID); 
            std::cout << "[Client] Login confirmed\n";
            ClientReadyMessage newMessage;
            send(&newMessage);
            std::cout << "[Client] Waiting for the players...\n";
            break;
        }   
        case MsgId::_START_GAME :{
            std::cout << "[Client] Game has initialised\n";
            StartGameMessage* start = static_cast<StartGameMessage*>(msg);
            gameClient->createGO(start->x1,start->y1,0,0);
            gameClient->createGO(start->x2,start->y2,1,0);
            gameClient->createGO(start->x3,start->y3,2,0);
            gameClient->createGO(start->x4,start->y4,3,0);

	        SDLGame::instance()->getManager()->getHandler(ecs::_hdlr_GameStateEntity)->getComponent<GameState>(ecs::GameState)->state = GameState::inGame;

            break;
        }
        
        default:
            break;
        }
        delete msg;
    }
    mut.unlock();
}

void NetworkClient::login(){
    LoginClientMessage* loginMessage = new LoginClientMessage(playerName);
    std::cout << "[Client] Sending login message of type " << (size_t)loginMessage->id << '\n';
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
            break;
        }
        case MsgId::_START_GAME :{
            StartGameMessage* startGame = new StartGameMessage();
            startGame->from_bin(msData);
            messages_.push(startGame);
            std::cout << "[CLient] Start GAme message\n";
            break;
        }
        default:
            std::cout << "[Client] Unknown message received\n";
            break;
        }
        mut.unlock();
    }
}
