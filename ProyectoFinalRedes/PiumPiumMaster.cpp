#include "PiumPiumMaster.h"

#include <assert.h>
#include <string.h>

#include "ImageComponent.h"
#include "Transform.h"
#include "InputHandler.h"
#include "SDL_macros.h"
#include "RenderSystem.h"
#include "Manager.h"
#include "SDLGame.h"

using namespace std;

PiumPiumMaster::PiumPiumMaster() :
		game_(nullptr), //
		mngr_(nullptr), //
		exit_(false) {
	initGame();
}

PiumPiumMaster::~PiumPiumMaster() {
	closeGame();
}



void PiumPiumMaster::initGame() {

	game_ = SDLGame::init("PiumPiumMaster", _WINDOW_WIDTH_, _WINDOW_HEIGHT_);

	// Initialize the pool, for the rest of factories it is not needed,
	// we will be using DefFactory for simplicity (e.g., in addEntity,
	// addComponnet, send)
	//
	// GhostsPool::init(21);        // for caching ghost entities
	// FoodPool::init(22);          // for caching food entities

	// create the manager
	mngr_ = new Manager(game_);

	// // create the systems
	// ghostsSystem_ = mngr_->addSystem<GhostsSystem>();
	// foodSystem_ = mngr_->addSystem<FoodSystem>();
	// pacmanSystem_ = mngr_->addSystem<PacManSystem>();
	renderSystem_ = mngr_->addSystem<RenderSystem>();
	// collisionSystem_ = mngr_->addSystem<CollisionSystem>();
	// gameCtrlSystem_ = mngr_->addSystem<GameCtrlSystem>();
	// audioSystem = mngr_->addSystem<AudioSystem>();
	// strawberrySystem = mngr_->addSystem<StrawberrySystem>();
}

void PiumPiumMaster::closeGame() {
	delete mngr_;
}

void PiumPiumMaster::start(char* mode, char* ip, char* port) {
	exit_ = false;
	auto ih = InputHandler::instance();
	
	std::string _mode = mode;
	if(_mode == "server"){
		net_server = new NetworkServer(ip, port);
		net_server->start();
	}
	else{
		net_client = new NetworkClient(ip, port);
		net_client->start();
	} 

	//Entity* e = mngr_->addEntity();
	//e->addComponent<ImageComponent*>(ecs::ImageComponent);
	//e->addComponent<Transform*>(ecs::Transform);

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

		// gameCtrlSystem_->update();
		// ghostsSystem_->update();
		// pacmanSystem_->update();
		// foodSystem_->update();
		// collisionSystem_->update();
		// renderSystem_->update();
		// audioSystem->update();

		// this is needed for sending the messages!
		mngr_->flushMessages();

		SDL_RenderPresent(game_->getRenderer());

		Uint32 frameTime = game_->getTime() - startTime;
		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}
}

