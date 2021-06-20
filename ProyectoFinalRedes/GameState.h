#pragma once

#include <cstdint>
#include "Component.h"
#include <vector>

struct GameState : Component {
	enum State : uint8_t { waiting, inGame , ondeath, win};

	GameState() :
		Component(ecs::GameState), //
		state(waiting) //
	{
	}

	State state;
	std::vector<int> life;		//array de vidas donde en cada posicion se encuentra la vida de un jugador
	std::vector<int> points;	//array de puntos donde en cada posicion se encuentra los puntos de un jugador
};

