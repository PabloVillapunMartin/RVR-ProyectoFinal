#pragma once
#include <ctime>
#include "Serializable.h"

class Entity;

// Mensajes locales para las entidades del juego
using msgType = std::size_t;

enum MsgId : msgType {
	_TESTING,

	//
	_last_MsgId_
};

class NetworkMessage : public Serializable{
public:
	NetworkMessage(MsgId id) : id(id) {}

	virtual int from_bin(char* data) override;

	virtual void to_bin() {};
	
	MsgId id;
};

struct TestingMessage : public NetworkMessage {
public:
	TestingMessage(int e) : NetworkMessage(_TESTING), win(e) {};

	int from_bin(char* data) override;

	void to_bin() override;

	int win;
};

// struct EatFruit : public Message {
// 	EatFruit(Entity* e) : Message(_EATFRUIT), e(e) {};
// 	Entity* e;
// };
