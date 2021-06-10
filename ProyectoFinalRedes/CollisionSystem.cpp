#include "CollisionSystem.h"

#include "Collisions.h"
//#include "GameState.h"
#include "Manager.h"
#include "Transform.h"


CollisionSystem::CollisionSystem() :
		System(ecs::_sys_Collisions) {
}

void CollisionSystem::update() {

	//auto gameState = mngr_->getHandler(ecs::_hdlr_GameStateEntity)->getComponent<GameState>(ecs::GameState);
	//if ( gameState->state_ != GameState::RUNNING)
	//	return;

	//auto ptr = mngr_->getHandler(ecs::_hdlr_PacManEntity)->getComponent<Transform>(ecs::Transform);

	//// collision with food
	//for (auto &e : mngr_->getGroupEntities(ecs::_grp_Food)) {
	//	auto etr = e->getComponent<Transform>(ecs::Transform);
	//	if (Collisions::collides(ptr->position_, ptr->width_, ptr->height_,
	//			etr->position_, etr->width_, etr->height_)) {
	//			mngr_->send<msg::EatFruit>(e);
	//	}
	//}

	//// collision with ghosts
	//for (auto &e : mngr_->getGroupEntities(ecs::_grp_Ghost)) {
	//	auto etr = e->getComponent<Transform>(ecs::Transform);
	//	if (Collisions::collides(ptr->position_, ptr->width_, ptr->height_,
	//			etr->position_, etr->width_, etr->height_)) {
	//			if(!mngr_->getHandler(ecs::_hdlr_GameStateEntity)->getComponent<Bonus>(ecs::Bonus)->active)
	//				mngr_->send<msg::Message>(msg::_PACMANDEAD);
	//			else //MUERE EL FANTASMA CON MENSAJE
	//			break;
	//	}
	//}

	//// collision with strawberry
	//Transform* tr = mngr_->getHandler(ecs::_hdlr_strawberry)->getComponent<Transform>(ecs::Transform);
	//if (tr != nullptr) {
	//	if (Collisions::collides(ptr->position_, ptr->width_, ptr->height_,
	//		tr->position_, tr->width_, tr->height_)) {
	//		mngr_->send<msg::Message>(msg::_EATSTRAWBERRY);
	//	}
	//}

}
