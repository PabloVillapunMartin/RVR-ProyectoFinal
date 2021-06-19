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
		_UPDATE_PLAYER_INFO,

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
		ShootMessage(int x_, int y_, float dirX_, float dirY_, int idP) : Message(_SHOOT_), x(x_), y(y_), idPlayer(idP), dirX(dirX_), dirY(dirY_) {};
		int x,y,idPlayer;
		float dirX, dirY;
	};

	struct MoveMessage : public Message {
		MoveMessage(int _x, int _y, int _id, float rotation_) : Message(_MOVE_), x(_x), y(_y), id_go(_id), rotation(rotation_) {};
		int x, y, id_go;
		float rotation;
	};

	struct BulletCollisionMessage : public Message {
		BulletCollisionMessage(int id_bullet_, int id_player_) : Message(_BULLET_COLLISION_), id_bullet(id_bullet_), id_player(id_player_) {}
		int id_player, id_bullet;
	};
	
	struct UpdatePlayerState : public Message {
		UpdatePlayerState(int lives1_, int points1_, int lives2_, int points2_, int lives3_, int points3_, int lives4_, int points4_) : 
		Message(_UPDATE_PLAYER_INFO), lives1(lives1_), points1(points1_), lives2(lives2_), points2(points2_),
	 	lives3(lives3_), points3(points3_), lives4(lives4_), points4(points4_) {}
		
		
		int lives1, points1;
		int lives2, points2;
		int lives3, points3;
		int lives4, points4;
	};
}
