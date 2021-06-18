#pragma once

#include <cstdint>
#include "Component.h"

struct GameState : Component {
	enum State : uint8_t { waiting, inGame };

	GameState() :
		Component(ecs::GameState), //
		state(waiting) //
	{
	}

	State state;
};

