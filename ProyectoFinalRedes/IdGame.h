#pragma once

#include "Component.h"

struct IdGame: Component {
	IdGame(int id) : Component(ecs::IdGame), id(id)
	{
	}
    int id;
};

