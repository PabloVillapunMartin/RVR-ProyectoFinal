#pragma once
#include "ecs.h"
#include "Entity.h"
#include "ImageComponent.h"
#include "ObjectPool.h"
#include "Singleton.h"
#include "Transform.h"
#include "Manager.h"


class BulletPool : public Singleton<BulletPool> {
	friend Singleton<BulletPool>;
public:
	virtual ~BulletPool() {
	}

	template<typename ...Targs>
	inline static Entity* construct(Targs&&...args) {
		return BulletPool::instance()->construct_(std::forward<Targs>(args)...);
	}

	inline static void destroy(Entity* p) {
		BulletPool::instance()->destroy_(p);
	}

	inline Entity* construct_(Vector2D pos, Vector2D vel, double w, double h) {
		Entity* e = pool_.getObj();
		if (e != nullptr) {
			e->setActive(true);
			Transform* tr = e->getComponent<Transform>(ecs::Transform);
			tr->velocity_.set(vel);
			tr->position_.set(pos);
			tr->width_ = w;
			tr->height_ = h;
		}
		return e;
	}

	inline void destroy_(Entity* p) {
		pool_.relObj(p);
	}

private:
	BulletPool() :
		BulletPool(10) {
	}
	BulletPool(std::size_t n) :
		pool_(n) {
		for (Entity* e : pool_.getPool()) {
			e->addComponent<Transform>();
			e->addComponent<ImageComponent>(SDLGame::instance()->getTextureMngr()->getTexture(Resources::Bullet));
		}
	}

	ObjectPool<Entity> pool_;
};