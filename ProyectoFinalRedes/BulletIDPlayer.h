#pragma once

#include <cstdint>
#include "Component.h"

struct BulletIDPlayer : Component {

	BulletIDPlayer() :
		Component(ecs::GameState), //
		idPlayer()
	{
	}

	int idPlayer;
};

