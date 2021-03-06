#include "PiumPiumMasterServer.h"

#include <assert.h>
#include <string.h>

#include "ImageComponent.h"
#include "Transform.h"
#include "InputHandler.h"
#include "SDL_macros.h"
#include "RenderSystem.h"
#include "Manager.h"
#include "SDLGame.h"
#include "BulletPool.h"
#include "PlayerSystem.h"
#include "GameState.h"
#include "GameCtrlSystem.h"
#include "BulletSystem.h"
#include "CollisionSystem.h"

using namespace std;

PiumPiumMasterServer::PiumPiumMasterServer(char* ip, char* port) :
		game_(nullptr), //
		mngr_(nullptr), //
		exit_(false) {
	initGame(ip,port);
}

PiumPiumMasterServer::~PiumPiumMasterServer() {
	delete net_server;
	closeGame();
}


void PiumPiumMasterServer::initGame(char* ip, char* port) {
	game_ = SDLGame::init("PiumPiumMasterServer", _WINDOW_WIDTH_, _WINDOW_HEIGHT_, true);

	// create the manager
	mngr_ = new Manager(game_);
	game_->setManager(mngr_);
	
	pool = new BulletPool(40);

	net_server = new NetworkServer(ip, port, this);
	net_server->start();
	// // create the systems
	playerSystem_=mngr_->addSystem<PlayerSystem>();
	collisionSystem_ = mngr_->addSystem<CollisionSystem>(pool);
	gameCtrlSystem_ = mngr_->addSystem<GameCtrlSystem>(net_server, pool);
	bulletSystem_ = mngr_->addSystem<BulletSystem>(net_server, pool);
}

void PiumPiumMasterServer::createWalls(){
	//---------------Pared arriba-----------------------
	createSingleWall(0, -10, _WINDOW_WIDTH_, 10);
	//---------------Pared abajo-------------------------
	createSingleWall(0, _WINDOW_HEIGHT_, _WINDOW_WIDTH_, 10);
	//---------------Pared derecha-----------------------
	createSingleWall(-10, 0, 10, _WINDOW_HEIGHT_);
	//---------------Pared izquierda---------------------
	createSingleWall(_WINDOW_WIDTH_, 0, 10, _WINDOW_HEIGHT_);
	//----------------Muro interior derecha---------------------
	createSingleWall(425, 200, 140, 30);
	//----------------Muro interior izquierda-------------------
	createSingleWall(105, 200, 140, 30);
	//----------------Muro interior abajo-----------------------
	createSingleWall(310, 70, 20, 140);
	//----------------Muro interior arriba----------------------
	createSingleWall(310, 350, 20, 140);
}

void PiumPiumMasterServer::closeGame() {
	delete mngr_;
}
void PiumPiumMasterServer::sendObjectPositions(){
	if (mngr_->getHandler(ecs::_hdlr_GameStateEntity)->getComponent<GameState>(ecs::GameState)->state == GameState::inGame) {
		//Actualizamos los jugadores
		for(int i = 0; i <  mngr_->getGroupEntities(ecs::_grp_Player).size(); i++){
			Entity* player = mngr_->getGroupEntities(ecs::_grp_Player)[i]; 
			Transform* tr = player->getComponent<Transform>(ecs::Transform);
			UpdateGameObjectMessage update(i, tr->position_.getX(), tr->position_.getY(), 0, tr->rotation_,player->isVisible());
			net_server->broadcastMessage(&update);
		}
		//Actualizamos las balas
		for(int i = 0; i < pool->getPool().size(); i++){
			Transform* tr = pool->getPool()[i]->getComponent<Transform>(ecs::Transform);
			UpdateGameObjectMessage update(i, tr->position_.getX(), tr->position_.getY(), 1, tr->rotation_, pool->getPool()[i]->isVisible());
			net_server->broadcastMessage(&update);
		}
	}
}

void PiumPiumMasterServer::createSingleWall(int x, int y, int width, int height){
	Entity* ent = mngr_->addEntity();

	Transform* tr = ent->addComponent<Transform>();
	tr->position_ = {x, y};
	tr->height_ = height;
	tr->width_ = width;
	
	ent->addToGroup(ecs::_grp_Walls);
	ent->setVisible(true);

	WallInfo ms(tr->position_.getX(), tr->position_.getY(), tr->height_,tr->width_);
	net_server->broadcastMessage(&ms);
}

void PiumPiumMasterServer::start() {
	exit_ = false;
	auto ih = InputHandler::instance();
	
	while (!exit_) {
		Uint32 startTime = game_->getTime();
		SDL_RenderClear(game_->getRenderer());
		SDL_SetRenderDrawColor(game_->getRenderer(), COLOR(0x003B00FF));

		ih->update();
		if (ih->keyDownEvent()) {
			if (ih->isKeyDown(SDLK_ESCAPE)) {
				exit_ = true;
				std::cout << "[Server] Closing server\n";
				break;
			}
		}

		mngr_->refresh();

		bulletSystem_->update();
		collisionSystem_->update();

		// this is needed for sending the messages!
		net_server->proccessMessages();
		mngr_->flushMessages();

		sendObjectPositions();

		SDL_RenderPresent(game_->getRenderer());

		Uint32 frameTime = game_->getTime() - startTime;
		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}
}

