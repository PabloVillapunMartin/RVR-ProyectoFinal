#pragma once

#include <cstdint>
#include "Component.h"
//Guarda el id del player que dispara la bala
struct BulletIDPlayer : Component {

	BulletIDPlayer() :
		Component(ecs::BulletIDPlayer), //
		idPlayer()
	{
	}

	int idPlayer;
};

