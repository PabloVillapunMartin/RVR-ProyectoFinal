#pragma once

#include <vector>
#include "NetworkServer.h"

class CollisionSystem;
class Manager;
class SDLGame;
class RenderSystem;
class PlayerSystem;
class GameCtrlSystem;
class BulletSystem;
class CollisionSystem;

class PiumPiumMasterServer {

public:
	PiumPiumMasterServer(char* ip, char* port);
	virtual ~PiumPiumMasterServer();

	// from SDLGame
	void start();
	void stop();

private:
	void initGame(char* ip, char* port);
	void closeGame();
	void createWalls();

	void clientGame();
	void serverGame();
	void sendObjectPositions();

	SDLGame *game_;
	Manager *mngr_;
	bool exit_;
	NetworkServer* net_server;

	CollisionSystem *collisionSystem_;
	RenderSystem *renderSystem_;
	PlayerSystem *playerSystem_;
	GameCtrlSystem *gameCtrlSystem_;
	BulletSystem *bulletSystem_;

	const static int _WINDOW_WIDTH_ = 640;
	const static int _WINDOW_HEIGHT_ = 480;

};
