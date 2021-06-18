#include "NetworkServer.h"
#include "SDLGame.h"
#include "Manager.h"
#include "messages.h"
#include "GameState.h"
#include <mutex>

std::mutex mutServer;
std::queue<NetworkMessage*> messagesServer_;
//================================ SERVER =======================================

void NetworkServer::start(){
    std::cout << "[Server] Starting server\n";
    playersReady = 0;
    incomingMessagesThread_ = std::thread(&NetworkServer::recieve_thread, this);
}

void NetworkServer::proccessMessages(){
    mutServer.lock();
    while(!messagesServer_.empty()){
        NetworkMessage* msg = messagesServer_.front(); messagesServer_.pop();

        switch ((size_t)msg->id)
        {
            case MsgId::_CLIENT_READY :{
                if(playersReady < 4) playersReady ++;
                std::cout << "[Server] Players ready to play: "<< playersReady << "\n";

                if(playersReady == 2){
                    StartGameMessage startGame(40, 40, 600, 40, 40, 440, 600, 440);
                    broadcastMessage(&startGame);
                    SDLGame::instance()->getManager()->getHandler(ecs::_hdlr_GameStateEntity)->getComponent<GameState>(ecs::GameState)->state = GameState::inGame;
                }
                break;
            }
            case MsgId::_LOG_IN_CLIENT : {
                LoginClientMessage* loginClientMessage = static_cast<LoginClientMessage*>(msg);
                addClient(loginClientMessage->clientSocket, loginClientMessage);
                break;
            }
            case MsgId::_UPDATE_CLIENT_PLAYER :{
                UpdateClientPlayerMessage* ms = static_cast<UpdateClientPlayerMessage*>(msg);
                SDLGame::instance()->getManager()->send<msg::MoveMessage>(ms->x, ms->y, ms->go_id, ms->rotation);
                //std::cout << "UpdateClient info: " << ms->go_id << " | " << ms->x << ", " << ms->y << " | " << ms->rotation << '\n';
                break;
            }
            default:
                break;
        }
        delete msg;
    }
    mutServer.unlock();
}

void NetworkServer::addClient(Socket* clientSocket, LoginClientMessage* msg){
    if(isAlreadyRegistered(clientSocket)) return;

    std::cout << "[Server] Adding a new client: " << msg->name_ << '\n';
    std::unique_ptr<Socket> loginRequest(clientSocket);
    clients.emplace_back(std::move(loginRequest));
    
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

void NetworkServer::broadcastMessage(Serializable* message){
    for(int i = 0; i < clients.size(); ++i){
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
        //std::cout << "Mensaje recibido de tipo " << (size_t)nm.id << "\n"; 

        mutServer.lock();
        switch ((size_t)nm.id)
        {
        case MsgId::_LOG_IN_CLIENT : {
            LoginClientMessage* loginClientMessage = new LoginClientMessage();
            loginClientMessage->from_bin(msData);
            loginClientMessage->clientSocket = clientSock;

            messagesServer_.push(loginClientMessage);
            break;
        }
        case MsgId::_CLIENT_READY :{
            std::cout << "Client ready\n";
            ClientReadyMessage* clientReadyMessage = new ClientReadyMessage();
            clientReadyMessage->from_bin(msData);

            messagesServer_.push(clientReadyMessage);
            break;
        }
        case MsgId::_UPDATE_CLIENT_PLAYER :{
            UpdateClientPlayerMessage* Message = new UpdateClientPlayerMessage();
            Message->from_bin(msData);

            messagesServer_.push(Message);
            break;
        }
        default:
            break;
        }
        mutServer.unlock();
        free(msData);
    }
}