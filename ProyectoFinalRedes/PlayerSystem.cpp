#include "PlayerSystem.h"

#include "Collisions.h"
#include "Manager.h"
#include "Transform.h"
#include "ImageComponent.h"
#include "Resources.h"

PlayerSystem::PlayerSystem() :
		System(ecs::_sys_Player) 
	{
	}

void PlayerSystem::init(){
	for(int i = 0; i < 4; ++i){
		Entity* ent = mngr_->addEntity();
		Transform* tr = ent->addComponent<Transform>();
		tr->height_ = 32; tr->width_= 32;
		ent->addComponent<ImageComponent>(game_->getTextureMngr()->getTexture(Resources::Player));
		players.push_back(ent);
		ent->addToGroup(ecs::_grp_Player);
	}

	Transform* tr = players[0]->getComponent<Transform>(ecs::Transform);
	tr->position_= {40,40};

	tr = players[1]->getComponent<Transform>(ecs::Transform);
	tr->position_= {600,40};

	tr = players[2]->getComponent<Transform>(ecs::Transform);
	tr->position_= {40,440};

	tr = players[3]->getComponent<Transform>(ecs::Transform);
	tr->position_= {600,440};

}

void PlayerSystem::recieve(const msg::Message& msg){
	switch (msg.id)
	{
	case msg::_MOVE_:{
		msg::MoveMessage info = static_cast<const msg::MoveMessage&>(msg);
		updatePlayerClient(info);
		break;
	}
	default:
		break;
	}
}

void PlayerSystem::updatePlayerClient(msg::MoveMessage const& ms){

	Transform* tr = players[ms.id_go]->getComponent<Transform>(ecs::Transform);
	tr->position_ = {tr->position_.getX() + ms.x, tr->position_.getY() + ms.y};
	tr->rotation_ = ms.rotation;
}