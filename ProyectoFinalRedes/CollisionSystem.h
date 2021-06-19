#pragma once

#include "System.h"

class Transform;

class CollisionSystem: public System {
public:
	CollisionSystem();

	void update() override;
	void fixCollision(Transform* &player, Transform* &wall);

};

