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

	
	void createPlayer(int x, int y);
	void createBullet(int x, int y);
	void initPoolBullets();
	void createWalls();
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

	int idClient_;
	const static int _WINDOW_WIDTH_ = 640;
	const static int _WINDOW_HEIGHT_ = 480;

};
