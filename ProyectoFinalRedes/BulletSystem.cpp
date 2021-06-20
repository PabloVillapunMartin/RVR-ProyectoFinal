#include "BulletSystem.h"
#include "Manager.h"
#include "GameState.h"
#include "NetworkMessages.h"
#include "NetworkServer.h"


//Crea una nueva bala dada la posicion y la velocidad a la que ha de ir
void BulletSystem::shoot(Vector2D pos, Vector2D vel, double width, double height, int id)
{
	Entity* e = pool->getBullet();	//pide una bala
	if (e != nullptr) {
		//Actualiza sus parámetros con la información dada
		e->setVisible(true);
		Transform* t = e->getComponent<Transform>(ecs::Transform);
		t->position_ = pos; 	t->velocity_ = vel;
		t->height_ = height; 	t->width_=width;
		BulletIDPlayer* b = e->getComponent<BulletIDPlayer>(ecs::BulletIDPlayer);
		b->idPlayer = id;
	}
}


void BulletSystem::update()
{
	//Si el estado es ingame
	if (mngr_->getHandler(ecs::_hdlr_GameStateEntity)->getComponent<GameState>(ecs::GameState)->state == GameState::inGame) {
		for (auto& o : pool->getPool()) {
			if (o->isVisible()) {	//si la bala esta activa
				Transform* t = o->getComponent<Transform>(ecs::Transform);
				t->position_.set(Vector2D(t->position_.getX() + t->velocity_.getX(), t->position_.getY() + t->velocity_.getY())); //suma la velocidad a la bala

				if (t->position_.getX() > game_->getWindowWidth() || t->position_.getX() < 0 ||		//Si se sale por los laterales
					t->position_.getY() > game_->getWindowHeight() || t->position_.getY() < 0) {	//Si se sale por arriba o abajo
					pool->deleteBullet(o);
				}
			}
		}
	}

}

void BulletSystem::recieve(const msg::Message& msg){
	switch (msg.id)
	{
	case msg::_SHOOT_ :{	//Mensaje de disparo
		msg::ShootMessage info = static_cast<const msg::ShootMessage&>(msg);
		shoot(Vector2D(info.x,info.y), Vector2D(info.dirX,info.dirY), 8, 8, info.idPlayer);
		break;
	}
	case msg::_BULLET_COLLISION_ :{		//Si la bala ha colisionado
		msg::BulletCollisionMessage info = static_cast<const msg::BulletCollisionMessage&>(msg);
		pool->deleteBullet(pool->getPool()[info.id_bullet]);	//desactivamos la bala
		break;
	}
	default:
		break;
	}
}