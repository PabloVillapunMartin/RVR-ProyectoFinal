#pragma once

#include <vector>


class CollisionSystem;
class Manager;
class SDLGame;
class RenderSystem;

class PiumPiumMaster {

public:
	PiumPiumMaster();
	virtual ~PiumPiumMaster();

	// from SDLGame
	void start();
	void stop();

private:
	void initGame();
	void closeGame();

	SDLGame *game_;
	Manager *mngr_;
	bool exit_;


	CollisionSystem *collisionSystem_;
	RenderSystem *renderSystem_;

	const static int _WINDOW_WIDTH_ = 640;
	const static int _WINDOW_HEIGHT_ = 480;

};
