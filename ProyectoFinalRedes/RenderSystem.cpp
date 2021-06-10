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

RenderSystem::RenderSystem() :
		System(ecs::_sys_Render) {
}

void RenderSystem::drawAnimated(Entity *e) {
	// Transform *tr = e->getComponent<Transform>(ecs::Transform);
	// AnimatedImageComponent *img = e->getComponent<AnimatedImageComponent>(
	// 		ecs::AnimatedImageComponent);
	// const auto &sprite = img->getSprite(game_->getTime());
	// SDL_Rect dest =	RECT(tr->position_.getX(), tr->position_.getY(), tr->width_, tr->height_);
	// sprite.first->render(dest, tr->rotation_, sprite.second);
}

void RenderSystem::update() {
	for(auto& ent: mngr_->getEntities()){
		Transform *tr = ent->getComponent<Transform>(ecs::Transform);
		ImageComponent *iC = ent->getComponent<ImageComponent>(ecs::ImageComponent);
		SDL_Rect dest =	SDL_Rect{tr->position_.getX(), tr->position_.getY(), tr->width_, tr->height_};
		iC->tex_->render(dest);
	}
}

