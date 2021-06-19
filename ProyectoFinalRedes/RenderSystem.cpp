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

RenderSystem::RenderSystem(PiumPiumMasterClient* piumpium, BulletPool* pool) :
System(ecs::_sys_Render),
piumpium(piumpium),
pool(pool)
{
}

void RenderSystem::update() {
	GameState* gamestate =  mngr_->getHandler(ecs::_hdlr_GameStateEntity)->getComponent<GameState>(ecs::GameState );
	//------------------------------------------------inGame & onDeath------------------------------------------
	if (gamestate->state == GameState::inGame || gamestate->state == GameState::ondeath || gamestate->state == GameState::win) {
		//Dibujamos el suelo
		Texture* texture = game_->getTextureMngr()->getTexture(Resources::Suelo);
		SDL_Rect rect;
		rect.x = 0; rect.y = 0; rect.w = game_->getWindowWidth(); rect.h = game_->getWindowHeight();
		texture->render(rect);
		//Dibujamos las entidades
		for(auto& ent: mngr_->getGroupEntities(ecs::_grp_Player)){
			if(ent->isVisible()){
				Transform *tr = ent->getComponent<Transform>(ecs::Transform);
				ImageComponent *iC = ent->getComponent<ImageComponent>(ecs::ImageComponent);
				SDL_Rect dest =	SDL_Rect{tr->position_.getX(), tr->position_.getY(), tr->width_, tr->height_};
				iC->tex_->render(dest, tr->rotation_);
			}
		}
		for(auto& ent: pool->getPool()){
			if(ent->isVisible()){
				Transform *tr = ent->getComponent<Transform>(ecs::Transform);
				ImageComponent *iC = ent->getComponent<ImageComponent>(ecs::ImageComponent);
				SDL_Rect dest =	SDL_Rect{tr->position_.getX(), tr->position_.getY(), tr->width_, tr->height_};
				iC->tex_->render(dest, tr->rotation_);
			}
		}
		for(auto& ent: mngr_->getGroupEntities(ecs::_grp_Walls)){
			Transform *tr = ent->getComponent<Transform>(ecs::Transform);
			ImageComponent *iC = ent->getComponent<ImageComponent>(ecs::ImageComponent);
			SDL_Rect dest =	SDL_Rect{tr->position_.getX(), tr->position_.getY(), tr->width_, tr->height_};
			iC->tex_->render(dest, tr->rotation_);
		}
		//Vidas del jugador
		for(int i = 0; i < gamestate->life[piumpium->getId()]; i++){
			Texture* texture = game_->getTextureMngr()->getTexture(Resources::Vida);
			SDL_Rect rect;
			int offset = 10; 
			rect.x = game_->getWindowWidth() - offset - 32 * (i + 1); rect.y = 20; rect.w = 32; rect.h = 28;
			texture->render(rect);
		}
		//Puntuaciones de los jugadores
		for(int i = 0; i < 4; i++){
			string player = "Player " + std::to_string(i + 1) + ": " + std::to_string(gamestate->points[i]);

			Texture name;
			name.loadFromText(SDLGame::instance()->getRenderer(), player, SDLGame::instance()->getFontMngr()->getFont(Resources::BEAR24), SDL_Color({255, 255, 255, 255}));

			SDL_Rect rect;
			int offset = 10; 
			rect.x = game_->getWindowWidth() - offset - 100; rect.y = 58 + offset + 20 * (i+1); rect.w = 100; rect.h = 20;
			name.render(rect);
		}

		//-------------------------------------------------- OnDeath -------------------------------------
		if(gamestate->state == GameState::ondeath){
			Texture* texture = game_->getTextureMngr()->getTexture(Resources::DeathBackGround);
			SDL_Rect rect;
			rect.x = 0; rect.y = 0; rect.w = game_->getWindowWidth(); rect.h = game_->getWindowHeight();
			texture->render(rect);

			texture = game_->getTextureMngr()->getTexture(Resources::GameOver);
			rect;
			rect.x = game_->getWindowWidth() / 2 - game_->getWindowWidth() / 4; rect.y = 0; rect.w = game_->getWindowWidth() / 2; rect.h = 50;

			texture->render(rect);
		}
		//-------------------------------------------------- OnWin -----------------------------------------
		if(gamestate->state == GameState::win){
			Texture* texture = game_->getTextureMngr()->getTexture(Resources::DeathBackGround);
			SDL_Rect rect;
			rect.x = 0; rect.y = 0; rect.w = game_->getWindowWidth(); rect.h = game_->getWindowHeight();
			texture->render(rect);

			texture = game_->getTextureMngr()->getTexture(Resources::Winner);
			rect;
			rect.x = game_->getWindowWidth() / 2 - game_->getWindowWidth() / 4; rect.y = 0; rect.w = game_->getWindowWidth() / 2; rect.h = 50;

			texture->render(rect);
		}
	}
	//--------------------------------------------- Waiting for players ---------------------------------------
	else{
		Texture* texture = game_->getTextureMngr()->getTexture(Resources::Waiting);
		SDL_Rect rect;
		rect.x = 0; rect.y = 0; rect.w = game_->getWindowWidth(); rect.h = game_->getWindowHeight();
		texture->render(rect);
	}
}

