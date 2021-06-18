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

	struct ShootMessage : public Message {
		ShootMessage(int x, int y, int speed) : Message(_SHOOT_), x(x),y(y),speed(speed) {};
		int x,y,speed;
	};

	struct MoveMessage : public Message {
		MoveMessage(int _x, int _y, int _id) : Message(_MOVE_), x(_x), y(_y), id_go(_id) {};
		int x, y, id_go;
		float rotation;
	};

	struct BulletCollisionMessage : public Message {
		BulletCollisionMessage(int id_bullet_, int id_player_) : Message(_BULLET_COLLISION_), id_bullet(id_bullet_), id_player(id_player_) {}
		int id_player, id_bullet;
	};
}
