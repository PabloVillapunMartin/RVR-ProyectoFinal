#pragma once
#include "ecs.h"
#include "Entity.h"
#include "Manager.h"
#include "Transform.h"
#include "SDLGame.h"
#include "ImageComponent.h"
#include "BulletIDPlayer.h"

#include <queue>

class BulletPool {
public:
	BulletPool(std::size_t n) :
		pool_() {
		for (int i = 0; i < n; i++) {
			Entity* e = SDLGame::instance()->getManager()->addEntity();
			Transform* tr = e->addComponent<Transform>();
			tr->height_ = 8; tr->width_ = 8;
			e->addComponent<ImageComponent>(SDLGame::instance()->getTextureMngr()->getTexture(Resources::Bullet));
			e->addComponent<BulletIDPlayer>();
			e->setVisible(false);
			notUsed_.push(e);
			pool_.push_back(e);
		}
	}
	~BulletPool(){
		
	}

	Entity* getBullet(){
		if(!notUsed_.empty()){
			Entity* e  = notUsed_.front(); notUsed_.pop();
			e->setVisible(true);
			return e;
		}
		else return nullptr;
	}
	void deleteBullet(Entity* e){
		if(e->isVisible()){
			notUsed_.push(e);
			e->setVisible(false);
		}
	}
	std::vector<Entity*>& getPool(){
		return pool_;
	}
	int getNotUsed(){
		return notUsed_.size();
	}

private:
	std::vector<Entity*> pool_;
	std::queue<Entity*> notUsed_;
};