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
#include "IdGame.h"

using namespace std;

PiumPiumMasterServer::PiumPiumMasterServer() :
		game_(nullptr), //
		mngr_(nullptr), //
		exit_(false) {
	initGame();
}

PiumPiumMasterServer::~PiumPiumMasterServer() {
	closeGame();
}



void PiumPiumMasterServer::initGame() {
	game_ = SDLGame::init("PiumPiumMasterServer", _WINDOW_WIDTH_, _WINDOW_HEIGHT_);

	// create the manager
	mngr_ = new Manager(game_);
	game_->setManager(mngr_);

	// // create the systems
	playerSystem_=mngr_->addSystem<PlayerSystem>();
	renderSystem_ = mngr_->addSystem<RenderSystem>();

	//BulletPool::init(40);
	// ghostsSystem_ = mngr_->addSystem<GhostsSystem>();
	// foodSystem_ = mngr_->addSystem<FoodSystem>();

	// pacmanSystem_ = mngr_->addSystem<PacManSystem>();
	// collisionSystem_ = mngr_->addSystem<CollisionSystem>();
	// gameCtrlSystem_ = mngr_->addSystem<GameCtrlSystem>();
	// audioSystem = mngr_->addSystem<AudioSystem>();
	// strawberrySystem = mngr_->addSystem<StrawberrySystem>();
}

void PiumPiumMasterServer::closeGame() {
	delete mngr_;
}
void PiumPiumMasterServer::sendObjectPositions(){
	for(auto& players: mngr_->getGroupEntities(ecs::_grp_Player)){
		Transform* tr = players->getComponent<Transform>(ecs::Transform);
		UpdateGameObjectMessage update(players->getComponent<IdGame>(ecs::IdGame)->id, tr->position_.getX(), tr->position_.getY(), tr->rotation_);
		net_server->broadcastMessage(&update);
	}
}
void PiumPiumMasterServer::start(char* ip, char* port) {
	exit_ = false;
	auto ih = InputHandler::instance();
	
	net_server = new NetworkServer(ip, port);
	net_server->start();

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

		// collisionSystem_->update();

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

