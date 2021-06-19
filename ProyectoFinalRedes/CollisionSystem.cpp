#include "CollisionSystem.h"

#include "Collisions.h"
#include "GameState.h"
#include "Manager.h"
#include "Transform.h"
#include "BulletIDPlayer.h"


CollisionSystem::CollisionSystem() :
		System(ecs::_sys_Collisions) {
}

void CollisionSystem::update() {

	auto gameState = mngr_->getHandler(ecs::_hdlr_GameStateEntity)->getComponent<GameState>(ecs::GameState);
	if ( gameState->state != GameState::inGame)
		return;

	// collision with food
	for (int i = 0; i < mngr_->getGroupEntities(ecs::_grp_Player).size(); i++) {

		Entity* player = mngr_->getGroupEntities(ecs::_grp_Player)[i];
		if(player->isActive()){

			Transform* ptr = player->getComponent<Transform>(ecs::Transform);

			//Comprobacion con las balas
			for (int j = 0; j < mngr_->getGroupEntities(ecs::_grp_Bullet).size(); j++) {

				Entity* bullet = mngr_->getGroupEntities(ecs::_grp_Bullet)[i];
				Transform* etr = bullet->getComponent<Transform>(ecs::Transform);
				BulletIDPlayer* idP = bullet->getComponent<BulletIDPlayer>(ecs::BulletIDPlayer);
				if(bullet->isActive() && idP->idPlayer != i){
					if (Collisions::collides(ptr->position_, ptr->width_, ptr->height_,
							etr->position_, etr->width_, etr->height_)) {
							mngr_->send<msg::BulletCollisionMessage>(j,i);
					}
				}
			}
		}
	}
}
