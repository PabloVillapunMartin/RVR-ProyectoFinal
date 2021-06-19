#pragma once

#include <cstdint>
#include "Component.h"
#include <vector>

struct GameState : Component {
	enum State : uint8_t { waiting, inGame , ondeath};

	GameState() :
		Component(ecs::GameState), //
		state(waiting) //
	{
	}

	State state;
	std::vector<int> life;
	std::vector<int> points;
};

