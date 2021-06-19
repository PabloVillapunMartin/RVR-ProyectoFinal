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
	game_ = SDLGame::init("PiumPiumMasterServer", _WINDOW_WIDTH_, _WINDOW_HEIGHT_);

	// create the manager
	mngr_ = new Manager(game_);
	game_->setManager(mngr_);

	net_server = new NetworkServer(ip, port);
	net_server->start();
	// // create the systems
	playerSystem_=mngr_->addSystem<PlayerSystem>();
	renderSystem_ = mngr_->addSystem<RenderSystem>();
	collisionSystem_ = mngr_->addSystem<CollisionSystem>();
	BulletPool::init(40);
	gameCtrlSystem_ = mngr_->addSystem<GameCtrlSystem>(net_server);
	bulletSystem_ = mngr_->addSystem<BulletSystem>(net_server);

	// pacmanSystem_ = mngr_->addSystem<PacManSystem>();
	// audioSystem = mngr_->addSystem<AudioSystem>();
	// strawberrySystem = mngr_->addSystem<StrawberrySystem>();
}

void PiumPiumMasterServer::closeGame() {
	delete mngr_;
}
void PiumPiumMasterServer::sendObjectPositions(){
	if (mngr_->getHandler(ecs::_hdlr_GameStateEntity)->getComponent<GameState>(ecs::GameState)->state == GameState::inGame) {
		for(int i = 0; i <  mngr_->getGroupEntities(ecs::_grp_Player).size(); i++){
			Transform* tr = mngr_->getGroupEntities(ecs::_grp_Player)[i]->getComponent<Transform>(ecs::Transform);
			UpdateGameObjectMessage update(i, tr->position_.getX(), tr->position_.getY(), 0, tr->rotation_,mngr_->getGroupEntities(ecs::_grp_Player)[i]->isActive());
			net_server->broadcastMessage(&update);
		}
		for(int i = 0; i <  mngr_->getGroupEntities(ecs::_grp_Bullet).size(); i++){
			Transform* tr = mngr_->getGroupEntities(ecs::_grp_Bullet)[i]->getComponent<Transform>(ecs::Transform);
			UpdateGameObjectMessage update(i, tr->position_.getX(), tr->position_.getY(), 1, tr->rotation_, mngr_->getGroupEntities(ecs::_grp_Bullet)[i]->isActive());
			net_server->broadcastMessage(&update);
		}
	}
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

