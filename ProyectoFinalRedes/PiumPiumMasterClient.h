#pragma once

#include <vector>
#include "NetworkClient.h"


class CollisionSystem;
class Manager;
class SDLGame;
class RenderSystem;
class GameCtrlSystem;
class BulletPool;
class WallInfo;

class PiumPiumMasterClient {

public:
	PiumPiumMasterClient();
	virtual ~PiumPiumMasterClient();

	// from SDLGame
	void start(char* ip, char* port, char* playerName);
	void stop() { exit_ = true; };

	
	void createPlayer(int x, int y);
	void createWall(WallInfo* ms);
	void updateGO(int x, int y, float rot, int id, int type, bool active);

	inline void setID(int id){ idClient_ = id;}
	inline int getId(){ return idClient_;}
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
	BulletPool* pool;

	int idClient_;
	const static int _WINDOW_WIDTH_ = 640;
	const static int _WINDOW_HEIGHT_ = 480;

	//----- Input -------
	bool up, down, right, left;
};
