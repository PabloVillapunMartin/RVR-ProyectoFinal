#include "BulletSystem.h"
#include "BulletPool.h"
#include "Manager.h"
#include "GameState.h"
#include "NetworkMessages.h"
#include "NetworkServer.h"


//Crea una nueva bala dada la posicion y la velocidad a la que ha de ir
void BulletSystem::shoot(Vector2D pos, Vector2D vel, double width, double height, int id)
{
	Entity* e = mngr_->addEntity<BulletPool>(pos, vel, width, height, id);
	if (e != nullptr) {
		e->setActive(true);
		e->addToGroup(ecs::_grp_Bullet);
		//game_->getAudioMngr()->playChannel(Resources::Bullet, 0, 1);
	}
}


void BulletSystem::update()
{
	if (mngr_->getHandler(ecs::_hdlr_GameStateEntity)->getComponent<GameState>(ecs::GameState)->state == GameState::inGame) {
		for (auto& o : mngr_->getGroupEntities(ecs::_grp_Bullet)) {
			if (o->isActive()) {
				Transform* t = o->getComponent<Transform>(ecs::Transform);
				t->position_.set(Vector2D(t->position_.getX() + t->velocity_.getX(), t->position_.getY() + t->velocity_.getY())); //suma la velocidad a la bala

				if (t->position_.getX() > game_->getWindowWidth() || t->position_.getX() < 0 ||		//Si se sale por los laterales
					t->position_.getY() > game_->getWindowHeight() || t->position_.getY() < 0) {	//Si se sale por arriba o abajo
					o->setActive(false);
				}
			}
		}
	}

}

void BulletSystem::recieve(const msg::Message& msg){
	switch (msg.id)
	{
	case msg::_SHOOT_ :{
		msg::ShootMessage info = static_cast<const msg::ShootMessage&>(msg);
		shoot(Vector2D(info.x,info.y), Vector2D(info.dirX,info.dirY), 16, 16, info.idPlayer);
		ShootServerMessages ms(info.x,info.y);
		net->broadcastMessage(&ms);
		break;
	}
	case msg::_BULLET_COLLISION_ :{
		msg::BulletCollisionMessage info = static_cast<const msg::BulletCollisionMessage&>(msg);
		mngr_->getGroupEntities(ecs::_grp_Bullet)[info.id_bullet]->setActive(false);
		break;
	}
	default:
		break;
	}
}