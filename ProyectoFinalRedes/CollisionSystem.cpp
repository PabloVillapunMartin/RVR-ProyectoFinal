#include "CollisionSystem.h"

#include "Collisions.h"
#include "GameState.h"
#include "Manager.h"
#include "Transform.h"
#include "BulletIDPlayer.h"


CollisionSystem::CollisionSystem(BulletPool* pool) :
System(ecs::_sys_Collisions),
pool(pool)
{
}

void CollisionSystem::update() {

	auto gameState = mngr_->getHandler(ecs::_hdlr_GameStateEntity)->getComponent<GameState>(ecs::GameState);
	if ( gameState->state != GameState::inGame)
		return;
	//Miramos la colisión del player con cada una de las entidades del juego
	for (int i = 0; i < mngr_->getGroupEntities(ecs::_grp_Player).size(); i++) {

		Entity* player = mngr_->getGroupEntities(ecs::_grp_Player)[i];
		if(player->isVisible()){

			Transform* ptr = player->getComponent<Transform>(ecs::Transform);

			//Comprobacion con las balas
			for (int j = 0; j < pool->getPool().size(); j++) {

				Entity* bullet = pool->getPool()[j];
				Transform* etr = bullet->getComponent<Transform>(ecs::Transform);
				BulletIDPlayer* idP = bullet->getComponent<BulletIDPlayer>(ecs::BulletIDPlayer);
				if(bullet->isVisible() && idP->idPlayer != i){
					if (Collisions::collides(ptr->position_, ptr->width_, ptr->height_,
							etr->position_, etr->width_, etr->height_)) {
						mngr_->send<msg::BulletCollisionMessage>(j,i);
					}
				}
			}

			//Comprobacion con paredes
			for(auto& wall : mngr_->getGroupEntities(ecs::_grp_Walls)){
				Transform* wallTr = wall->getComponent<Transform>(ecs::Transform);

				if(Collisions::collides(ptr->position_, ptr->width_, ptr->height_,
							wallTr->position_, wallTr->width_, wallTr->height_)){
					fixCollision(ptr, wallTr);
				}
			}
		}
	}
	//Comprobamos las balas con cada una de las paredes
	for (int i = 0; i < pool->getPool().size(); i++) {

		Entity* bullet = pool->getPool()[i];
		Transform* ptr = bullet->getComponent<Transform>(ecs::Transform);
		if(bullet->isVisible()){
			//Comprobacion con paredes
			for(auto& wall : mngr_->getGroupEntities(ecs::_grp_Walls)){
				Transform* wallTr = wall->getComponent<Transform>(ecs::Transform);

				if(Collisions::collides(ptr->position_, ptr->width_, ptr->height_,
					wallTr->position_, wallTr->width_, wallTr->height_)){
					pool->deleteBullet(bullet);
				}
			}
		}
	}
}

void colisionDerecha(Transform* &player, Transform* &wall){
	int offset = abs((player->position_.getX() + player->width_) - wall->position_.getX());
	player->position_.setX(player->position_.getX() - offset);
}

void colisionIzquierda(Transform* &player, Transform* &wall){
	int offset = abs(player->position_.getX() - (wall->position_.getX() + wall->width_));
	player->position_.setX(player->position_.getX() + offset);
}

void colisionArriba(Transform* &player, Transform* &wall){
	int offset = abs((player->position_.getY() + player->height_) - wall->position_.getY());
	player->position_.setY(player->position_.getY() - offset);
}

void colisionAbajo(Transform* &player, Transform* &wall){
	int offset = abs(player->position_.getY() - (wall->position_.getY() + wall->height_));
	player->position_.setY(player->position_.getY() + offset);
}

void colisionEsquinaSupDer(Transform* &player, Transform* &wall){
	int offsetAncho = abs((wall->position_.getX() + wall->width_) - player->position_.getX());
	int offsetAlto = abs((player->position_.getY() + player->height_) - (wall->position_.getY()));

	if(offsetAlto < offsetAncho) player->position_.setY(player->position_.getY() - offsetAlto);
	else player->position_.setX(player->position_.getX() + offsetAncho);
}

void colisionEsquinaSupIzq(Transform* &player, Transform* &wall){
	int offsetAncho = abs((player->position_.getX() + player->width_) - wall->position_.getX());
	int offsetAlto = abs((player->position_.getY() + player->height_) - (wall->position_.getY()));

	if(offsetAlto < offsetAncho) player->position_.setY(player->position_.getY() - offsetAlto);
	else player->position_.setX(player->position_.getX() - offsetAncho);
}

void colisionEsquinaInfDer(Transform* &player, Transform* &wall){
	int offsetAncho = abs((wall->position_.getX() + wall->width_) - player->position_.getX());
	int offsetAlto = abs((wall->position_.getY() + wall->height_) - player->position_.getY());

	if(offsetAlto < offsetAncho) player->position_.setY(player->position_.getY() + offsetAlto);
	else player->position_.setX(player->position_.getX() + offsetAncho);
}

void colisionEsquinaInfIzq(Transform* &player, Transform* &wall){
	int offsetAncho = abs((player->position_.getX() + player->width_) - wall->position_.getX());
	int offsetAlto = abs((wall->position_.getY() + wall->height_) - player->position_.getY());

	if(offsetAlto < offsetAncho) player->position_.setY(player->position_.getY() + offsetAlto);
	else player->position_.setX(player->position_.getX() - offsetAncho);
}

void CollisionSystem::fixCollision(Transform* &player, Transform* &wall){

	//colision horizontal
	if(wall->position_.getY() <= player->position_.getY() && (wall->position_.getY() + wall->height_) >= (player->position_.getY() + player->height_)){
		//Derecha
		if(player->position_.getX() <= wall->position_.getX()) colisionDerecha(player, wall);
		//Izquierda
		else colisionIzquierda(player, wall);
	}
	//colision vertical
	else if(wall->position_.getX() < player->position_.getX() && (wall->position_.getX() + wall->width_) > (player->position_.getX() + player->width_)){
		//Arriba
		if(player->position_.getY() <= wall->position_.getY()) colisionArriba(player, wall);
		//Abajo
		else colisionAbajo(player, wall);
	}
	else{	//Esquinas
		//Parte derecha
		if(player->position_.getX() > wall->position_.getX()){
			//Arriba
			if(player->position_.getY() < wall->position_.getY()){
				colisionEsquinaSupDer(player, wall);
			}
			//Abajo
			else{
				colisionEsquinaInfDer(player, wall);
			}
		}
		//Parte izquierda
		else{
			//Arriba
			if(player->position_.getY() < wall->position_.getY()){
				colisionEsquinaSupIzq(player, wall);
			}
			//Abajo
			else{
				colisionEsquinaInfIzq(player, wall);
			}
		}
	}
}