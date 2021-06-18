#pragma once

#include <vector>
#include "NetworkClient.h"


class CollisionSystem;
class Manager;
class SDLGame;
class RenderSystem;
class GameCtrlSystem;

class PiumPiumMasterClient {

public:
	PiumPiumMasterClient();
	virtual ~PiumPiumMasterClient();

	// from SDLGame
	void start(char* ip, char* port, char* playerName);
	void stop();

	inline void setID(int id){ idClient_ = id;}
	void createGO(int x, int y, int id, int type);

private:
	void initGame();
	void closeGame();
    bool checkInput();

	SDLGame *game_;
	Manager *mngr_;
	bool exit_;
	NetworkClient* net_client;

	RenderSystem *renderSystem_;
	GameCtrlSystem *gameCtrlSystem_;

	int idClient_;
	const static int _WINDOW_WIDTH_ = 640;
	const static int _WINDOW_HEIGHT_ = 480;

};
