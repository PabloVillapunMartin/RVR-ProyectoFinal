#pragma once
#include <ctime>

class Entity;

namespace msg {

using msgType = std::size_t;

enum MsgId : msgType {
	_GAME_START,
	_GAME_OVER,
	_READY,
	_PACMANDEAD,
	_EATFRUIT,
	_NOMOREFOOD,
	_EATSTRAWBERRY,

	//
	_last_MsgId_
};

struct Message {
	Message(MsgId id) :
			id(id) {
	}
	MsgId id;
};

struct GameOver : public Message {
	GameOver(bool e) : Message(_GAME_OVER), win(e) {};
	bool win;
};
struct EatFruit : public Message {
	EatFruit(Entity* e) : Message(_EATFRUIT), e(e) {};
	Entity* e;
};

}
