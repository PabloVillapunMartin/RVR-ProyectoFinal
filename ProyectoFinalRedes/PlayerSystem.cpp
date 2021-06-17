#include "PlayerSystem.h"

#include "Collisions.h"
#include "Manager.h"
#include "Transform.h"
#include "ImageComponent.h"
#include "Resources.h"
#include "IdGame.h"

PlayerSystem::PlayerSystem() :
		System(ecs::_sys_Player) 
	{
	}

void PlayerSystem::init(){
	for(int i = 0; i < 4; ++i){
		Entity* ent = mngr_->addEntity();
		Transform* tr = ent->addComponent<Transform>();
		ent->addComponent<ImageComponent>(game_->getTextureMngr()->getTexture(Resources::Bullet));
		//ent->addComponent<IdGame>(ent->getEntityMngr()->getIdCount());
		players.push_back(ent);
		mngr_->addToGroup(ecs::_grp_Player, ent);
	}

}

void PlayerSystem::recieve(const msg::Message& msg){
	switch (msg.id)
	{
	case msg::_MOVE_:{
		//Movimiento jugador
		break;
	}
	case msg::_SHOOT_:{
		//Disparo jugador
		break;
	}	
	case msg::_BULLET_COLLISION_:{
		//Colision bala jugador
		break;
	}
	default:
		break;
	}
}
