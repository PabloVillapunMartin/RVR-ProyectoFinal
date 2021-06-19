#pragma once
#include <ctime>
#include "Serializable.h"
#include <string>
#include "Socket.h"

class Entity;

using msgType = std::size_t;
enum MsgId : msgType {
	_network_,
	_LOG_IN_CLIENT,
	_CONFIRMATION_LOGIN,
	_START_GAME,
	_CLIENT_READY,
	_UPDATE_CLIENT_PLAYER,
	_UPDATE_GAMEOBJECT,
	_SHOOT_CLIENT,
	_SHOOT_SERVER,
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

//Mensaje de confirmacion del servidor al cliente de logging
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

//Mensaje de actualizacion de input del clinte al servidor
class UpdateClientPlayerMessage : public NetworkMessage {
public:
	UpdateClientPlayerMessage(int goid, int x_, int y_, float rotation_) : NetworkMessage(_UPDATE_CLIENT_PLAYER), x(x_), y(y_), go_id(goid), rotation(rotation_) {};

	UpdateClientPlayerMessage() : NetworkMessage(_UPDATE_CLIENT_PLAYER) {};

	~UpdateClientPlayerMessage(){}

	int from_bin(char* data) override;

	void to_bin() override;

	int x, y, go_id;
	float rotation;
};
//Mensaje de actualizacion de posicion de servidor a clientede un game object
class UpdateGameObjectMessage : public NetworkMessage {
public:
	UpdateGameObjectMessage(int goid, int x_, int y_, int type_, float rotation_, bool active_) : NetworkMessage(_UPDATE_GAMEOBJECT), x(x_), y(y_), 
	go_id(goid), type(type_), rotation(rotation_), active(active_){};

	UpdateGameObjectMessage() : NetworkMessage(_UPDATE_GAMEOBJECT) {};

	~UpdateGameObjectMessage(){}

	int from_bin(char* data) override;

	void to_bin() override;

	int x, y, go_id, type;
	float rotation;
	bool active;
};
//Mensaje de disparo, lo usa el cliente para indicarle al servidor la posicion en la que sea crea
class ShootClientMessage : public NetworkMessage {
public:
	ShootClientMessage(int x_, int y_, float dirX_, float dirY_) : NetworkMessage(_SHOOT_CLIENT), x(x_), y(y_), dirX(dirX_), dirY(dirY_) {};

	ShootClientMessage() : NetworkMessage(_SHOOT_CLIENT) {};

	~ShootClientMessage(){}

	int from_bin(char* data) override;

	void to_bin() override;

	int x, y;
	float dirX, dirY;
};
//Mensaje de disparo, lo usa el servidor para decirle a los clientes que creen un go "bullet"
class ShootServerMessages : public NetworkMessage {
public:
	ShootServerMessages(int x_, int y_) : NetworkMessage(_SHOOT_SERVER), x(x_), y(y_) {};

	ShootServerMessages() : NetworkMessage(_SHOOT_SERVER) {};

	~ShootServerMessages(){}

	int from_bin(char* data) override;

	void to_bin() override;

	int x, y;
};
