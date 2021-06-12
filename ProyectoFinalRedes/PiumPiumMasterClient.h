#pragma once

#include <vector>


class CollisionSystem;
class Manager;
class SDLGame;
class RenderSystem;

class PiumPiumMasterClient {

public:
	PiumPiumMasterClient();
	virtual ~PiumPiumMasterClient();

	// from SDLGame
	void start();
	void stop();

private:
	void initGame();
	void closeGame();
    bool checkInput();

	SDLGame *game_;
	Manager *mngr_;
	bool exit_;
    int clientId_;

	RenderSystem *renderSystem_;

	const static int _WINDOW_WIDTH_ = 640;
	const static int _WINDOW_HEIGHT_ = 480;

};
