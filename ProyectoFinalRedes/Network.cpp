#include "Network.h"

NetworkClient::NetworkClient(const char * direccion, const char * puerto) : socket_(direccion,puerto){

}
NetworkClient::~NetworkClient(){

}

void NetworkClient::start(){

}
void NetworkClient::send(Serializable* message){

}
void NetworkClient::proccessMessages(){

}

void NetworkClient::login(){

}
void NetworkClient::recieve_thread(){
    
}