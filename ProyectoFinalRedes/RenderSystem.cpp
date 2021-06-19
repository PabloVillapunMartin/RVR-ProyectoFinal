#include "RenderSystem.h"

#include <cassert>
#include "AnimatedImageComponent.h"
#include "ecs.h"
#include "Manager.h"
#include "ImageComponent.h"
#include "SDL_macros.h"
#include "SDLGame.h"
#include "Transform.h"
#include "Entity.h"
#include "Manager.h"
#include "SDLGame.h"
#include "Texture.h"
#include "GameState.h"

RenderSystem::RenderSystem() :
		System(ecs::_sys_Render) {
}

void RenderSystem::update() {

	if (mngr_->getHandler(ecs::_hdlr_GameStateEntity)->getComponent<GameState>(ecs::GameState)->state == GameState::inGame ||
		mngr_->getHandler(ecs::_hdlr_GameStateEntity)->getComponent<GameState>(ecs::GameState)->state == GameState::ondeath) {
		for(auto& ent: mngr_->getGroupEntities(ecs::_grp_Player)){
			Transform *tr = ent->getComponent<Transform>(ecs::Transform);
			ImageComponent *iC = ent->getComponent<ImageComponent>(ecs::ImageComponent);
			SDL_Rect dest =	SDL_Rect{tr->position_.getX(), tr->position_.getY(), tr->width_, tr->height_};
			iC->tex_->render(dest, tr->rotation_);
		}
		for(auto& ent: mngr_->getGroupEntities(ecs::_grp_Bullet)){
			if(ent->isActive()){
				Transform *tr = ent->getComponent<Transform>(ecs::Transform);
				ImageComponent *iC = ent->getComponent<ImageComponent>(ecs::ImageComponent);
				SDL_Rect dest =	SDL_Rect{tr->position_.getX(), tr->position_.getY(), tr->width_, tr->height_};
				iC->tex_->render(dest, tr->rotation_);
			}
		}
		if(mngr_->getHandler(ecs::_hdlr_GameStateEntity)->getComponent<GameState>(ecs::GameState)->state == GameState::ondeath){
			Texture* texture = game_->getTextureMngr()->getTexture(Resources::DeathBackGround);
			SDL_Rect rect;
			rect.x = 0; rect.y = 0; rect.w = game_->getWindowWidth(); rect.h = game_->getWindowHeight();
			texture->render(rect);

			texture = game_->getTextureMngr()->getTexture(Resources::WaitingForPlayers);
			rect;
			rect.x = game_->getWindowWidth() / 2; rect.y = 20; rect.w = game_->getWindowWidth() / 3; rect.h = 50;
			texture->render(rect);
		}
	}
	else{
		Texture* texture = game_->getTextureMngr()->getTexture(Resources::WaitingForPlayers);
		SDL_Rect rect;
		rect.x = game_->getWindowWidth() / 2; rect.y = 20; rect.w = game_->getWindowWidth() / 3; rect.h = 50;
		texture->render(rect);
	}
}

