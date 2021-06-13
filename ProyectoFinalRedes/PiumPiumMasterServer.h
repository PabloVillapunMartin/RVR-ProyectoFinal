#pragma once

#include <vector>
#include "Network.h"

class CollisionSystem;
class Manager;
class SDLGame;
class RenderSystem;

class PiumPiumMasterServer {

public:
	PiumPiumMasterServer();
	virtual ~PiumPiumMasterServer();

	// from SDLGame
	void start(char* ip, char* port);
	void stop();

private:
	void initGame();
	void closeGame();

	void clientGame();
	void serverGame();

	SDLGame *game_;
	Manager *mngr_;
	bool exit_;
	NetworkServer* net_server;
	NetworkClient* net_client;

	CollisionSystem *collisionSystem_;
	RenderSystem *renderSystem_;

	const static int _WINDOW_WIDTH_ = 640;
	const static int _WINDOW_HEIGHT_ = 480;

};
