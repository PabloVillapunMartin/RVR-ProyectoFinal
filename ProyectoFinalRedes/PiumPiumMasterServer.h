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
class BulletPool;
class Transform;

class PiumPiumMasterServer {

public:
	PiumPiumMasterServer(char* ip, char* port);
	virtual ~PiumPiumMasterServer();

	// from SDLGame
	void start();
	void stop();
	void createWalls();

private:
	void initGame(char* ip, char* port);
	void closeGame();

	void clientGame();
	void serverGame();
	void sendObjectPositions();
	void createSingleWall(int x, int y, int width, int height);

	SDLGame *game_;
	Manager *mngr_;
	bool exit_;
	NetworkServer* net_server;
	BulletPool* pool;

	CollisionSystem *collisionSystem_;
	PlayerSystem *playerSystem_;
	GameCtrlSystem *gameCtrlSystem_;
	BulletSystem *bulletSystem_;

	const static int _WINDOW_WIDTH_ = 640;
	const static int _WINDOW_HEIGHT_ = 480;

};
