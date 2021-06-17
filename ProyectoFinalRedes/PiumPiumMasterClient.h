#pragma once

#include <vector>
#include "NetworkClient.h"


class CollisionSystem;
class Manager;
class SDLGame;
class RenderSystem;

class PiumPiumMasterClient {

public:
	PiumPiumMasterClient();
	virtual ~PiumPiumMasterClient();

	// from SDLGame
	void start(char* ip, char* port, char* playerName);
	void stop();

private:
	void initGame();
	void closeGame();
    bool checkInput();

	SDLGame *game_;
	Manager *mngr_;
	bool exit_;
	NetworkClient* net_client;

	RenderSystem *renderSystem_;

	const static int _WINDOW_WIDTH_ = 640;
	const static int _WINDOW_HEIGHT_ = 480;

};
