#pragma once

#include "System.h"
#include "NetworkServer.h"
#include "PiumPiumMasterClient.h"

struct GameState;

class GameCtrlSystem: public System {
public:

	GameCtrlSystem(NetworkServer* server, PiumPiumMasterClient* client = nullptr);

	void init() override;
	void recieve(const msg::Message& msg) override;

private:
	const int SHOOT_POINTS = 10;
	GameState *gameState_;
	NetworkServer* net_server;
	PiumPiumMasterClient* client;
};

