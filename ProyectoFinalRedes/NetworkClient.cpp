#include "NetworkClient.h"
#include "NetworkMessages.h"
#include "PiumPiumMasterClient.h"
#include "SDLGame.h"
#include "Manager.h"
#include "GameState.h"

#include <mutex>

std::mutex mut;
std::queue<NetworkMessage*> messages_;
bool running;
//========================================= CLIENT =============================================

NetworkClient::~NetworkClient(){
    mut.lock();
    running = false;
    mut.unlock();
    incomingMessagesThread_.join();

    while(!messages_.empty()){
        delete messages_.front();
        messages_.pop();
    }
}

NetworkClient::NetworkClient(const char * direccion, const char * puerto, char* _playerName, PiumPiumMasterClient* game) : 
socket_(direccion,puerto), 
playerName(_playerName),
gameClient(game)
{

}

void NetworkClient::start(){
    running = true;
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
            std::cout << "[Client] Login confirmed " << login->gameObjectID << "\n";
            ClientReadyMessage newMessage;
            send(&newMessage);
            std::cout << "[Client] Waiting for the players...\n";
            break;
        }   
        case MsgId::_START_GAME :{
            std::cout << "[Client] Game has initialised\n";
            StartGameMessage* start = static_cast<StartGameMessage*>(msg);
            gameClient->createPlayer(start->x1,start->y1);
            gameClient->createPlayer(start->x2,start->y2);
            gameClient->createPlayer(start->x3,start->y3);
            gameClient->createPlayer(start->x4,start->y4);

	        SDLGame::instance()->getManager()->getHandler(ecs::_hdlr_GameStateEntity)->getComponent<GameState>(ecs::GameState)->state = GameState::inGame;

            break;
        }
        case MsgId::_UPDATE_GAMEOBJECT: {
            UpdateGameObjectMessage* ms = static_cast<UpdateGameObjectMessage*>(msg);
            gameClient->updateGO(ms->x, ms->y, ms->rotation, ms->go_id, ms->type,ms->active);
            break;
        }
        case MsgId::_UPDATE_PLAYER_INFO: {
            UpdatePlayerStateMessage* ms = static_cast<UpdatePlayerStateMessage*>(msg);
            SDLGame::instance()->getManager()->send<msg::UpdatePlayerState>(ms->lives1, ms->points1, ms->lives2, ms->points2,
             ms->lives3, ms->points3,ms->lives4, ms->points4);
            break;
        }
        case MsgId::_WALL_INFO: {
            WallInfo* ms = static_cast<WallInfo*>(msg);
            gameClient->createWall(ms);
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
        case MsgId::_UPDATE_GAMEOBJECT: {
            UpdateGameObjectMessage* ms = new UpdateGameObjectMessage();
            ms->from_bin(msData);
            messages_.push(ms);
            break;
        }
        case MsgId::_UPDATE_PLAYER_INFO: {
            UpdatePlayerStateMessage* ms = new UpdatePlayerStateMessage();
            ms->from_bin(msData);
            messages_.push(ms);
            break;
        }
        case MsgId::_WALL_INFO: {
            WallInfo* ms = new WallInfo();
            ms->from_bin(msData);
            messages_.push(ms);
            break;
        }
        case MsgId::_SERVER_CLOSED: {
            std::cout << "[Client] Server has been closed\n";
            gameClient->stop();
            running = false;
            break;
        }
        default:
            std::cout << "[Client] Unknown message received of type " << (size_t)nm.id << "\n";
            break;
        }

        if(!running){
            mut.unlock();
            free(msData);
            break;
        }

        mut.unlock();
        free(msData);
    }
}
