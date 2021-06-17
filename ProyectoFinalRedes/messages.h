#pragma once
#include <ctime>

class Entity;

// Mensajes locales para las entidades del juego
namespace msg {

	using msgType = std::size_t;

	enum MsgId : msgType {
		_SHOOT_,
		_MOVE_,
		_BULLET_COLLISION_,

		//
		_last_MsgId_
	};

	struct Message {
		Message(MsgId id) :
				id(id) {
		}
		MsgId id;
	};

	struct Shoot : public Message {
		Shoot(int x, int y, int speed) : Message(_SHOOT_), x(x),y(y),speed(speed) {};
		int x,y,speed;
	};

	struct Move : public Message {
		Move(int _x, int _y, int _id) : Message(_MOVE_), x(_x), y(_y), id(_id) {};
		int x, y, id;
	};

	struct BulletCollision : public Message {
		BulletCollision(int id_bullet_, int id_player_) : Message(_BULLET_COLLISION_), id_bullet(id_bullet_), id_player(id_player_) {}
		int id_player, id_bullet;
	};
}
