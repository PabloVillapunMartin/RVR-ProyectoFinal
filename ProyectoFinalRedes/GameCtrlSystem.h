#pragma once

#include "System.h"

struct GameState;

class GameCtrlSystem: public System {
public:

	GameCtrlSystem();

	void init() override;

private:
	GameState *gameState_;
};

