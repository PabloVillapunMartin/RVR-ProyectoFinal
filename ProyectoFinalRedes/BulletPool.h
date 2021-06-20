#pragma once
#include "ecs.h"
#include "Entity.h"
#include "Manager.h"
#include "Transform.h"
#include "SDLGame.h"
#include "ImageComponent.h"
#include "BulletIDPlayer.h"

#include <queue>
//Pool de balas
class BulletPool {
public:
	/// <summary>
	/// Constructora con n siendo el número de de cargas
	/// </summary>
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
	/// <summary>
	/// Destructora
	/// </summary>
	~BulletPool(){
		
	}
	/// <summary>
	/// Devulve una entidad que no esté siendo usada
	/// </summary>
	Entity* getBullet(){
		if(!notUsed_.empty()){
			Entity* e  = notUsed_.front(); notUsed_.pop();
			e->setVisible(true);
			return e;
		}
		else return nullptr;
	}
	/// <summary>
	/// En caso de que la entiedad esté activada, pone la entidad
	/// desactvada y la met en la cola de desactivadas
	/// </summary>
	void deleteBullet(Entity* e){
		if(e->isVisible()){
			notUsed_.push(e);
			e->setVisible(false);
		}
	}
	/// <summary>
	/// Devuelve la pool de balas
	/// </summary>
	std::vector<Entity*>& getPool(){
		return pool_;
	}
	/// <summary>
	/// Devuelve el numero de balas no usadas
	/// </summary>
	int getNotUsed(){
		return notUsed_.size();
	}

private:
	std::vector<Entity*> pool_;		//pool donde se encuentran todas las balas
	std::queue<Entity*> notUsed_;	//cola donde se encuentran las balas no usadas
};