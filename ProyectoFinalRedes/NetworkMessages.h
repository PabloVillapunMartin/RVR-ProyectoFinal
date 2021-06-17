#pragma once
#include <ctime>
#include "Serializable.h"
#include <string>
#include "Socket.h"

class Entity;

using msgType = std::size_t;
enum MsgId : msgType {
	_network_,
	_CONFIRMATION_LOGIN,
	_LOG_IN_CLIENT,
	_START_GAME,
	_CLIENT_READY,
	_UPDATE_CLIENT_PLAYER,
	_UPDATE_GAMEOBJECT,
	_SHOOT,
	//
	_last_MsgId_
};

//Clase padre de los mensajes de red
class NetworkMessage : public Serializable{
public:
	NetworkMessage(MsgId id) : id(id) {};

	NetworkMessage() : id(_network_) {};

	~NetworkMessage(){};

	virtual int from_bin(char* data) override;

	virtual void to_bin() {};
	
	MsgId id;
};

//Mensaje de log de cliente al servidor
class LoginClientMessage : public NetworkMessage {
public:
	LoginClientMessage(char* name) : NetworkMessage(_LOG_IN_CLIENT), name_(name) {};

	LoginClientMessage() : NetworkMessage(_LOG_IN_CLIENT) {};

	~LoginClientMessage(){ 
	};

	int from_bin(char* data) override;

	void to_bin() override;

	std::string name_;
	const int SIZE_NAME = 20;

	Socket* clientSocket;
};

//Mensaje de confirmacion del servidor al cliente
class ConfirmationLoginMessage : public NetworkMessage {
public:
	ConfirmationLoginMessage(int id) : NetworkMessage(_CONFIRMATION_LOGIN), gameObjectID(id) {};

	ConfirmationLoginMessage() : NetworkMessage(_CONFIRMATION_LOGIN) {};

	~ConfirmationLoginMessage(){}

	int from_bin(char* data) override;

	void to_bin() override;

	int gameObjectID;
};

//Mensaje de inicio de juego del servidor al cliente
class StartGameMessage : public NetworkMessage {
public:
	StartGameMessage(int _x1, int _y1, int _x2, int _y2, int _x3, int _y3, int _x4, int _y4) : NetworkMessage(_START_GAME), x1(_x1),y1(_y1),
	 x2(_x2),y2(_y2),
	 x3(_x3),y3(_y3), 
	 x4(_x4),y4(_y4) {};

	StartGameMessage() : NetworkMessage(_START_GAME) {};

	~StartGameMessage(){}

	int from_bin(char* data) override;

	void to_bin() override;
	//Posiciones de los jugadores
	int x1, y1;
	int x2, y2;
	int x3, y3;
	int x4, y4;
};

//Mensaje de confirmacion del cliente al servidor
class ClientReadyMessage : public NetworkMessage {
public:
	ClientReadyMessage(int id) : NetworkMessage(_CLIENT_READY) {};

	ClientReadyMessage() : NetworkMessage(_CLIENT_READY) {};

	~ClientReadyMessage(){}

	int from_bin(char* data) override;

	void to_bin() override;
};