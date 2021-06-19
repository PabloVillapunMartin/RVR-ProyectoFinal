#pragma once

#include "System.h"
#include "BulletPool.h"

class Transform;

class CollisionSystem: public System {
public:
	CollisionSystem(BulletPool* pool);

	void update() override;
	void fixCollision(Transform* &player, Transform* &wall);
private:
	BulletPool* pool;
};

