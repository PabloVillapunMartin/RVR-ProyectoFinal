#include "PiumPiumMasterClient.h"

#include <assert.h>

#include "ImageComponent.h"
#include "Transform.h"
#include "InputHandler.h"
#include "SDL_macros.h"
#include "RenderSystem.h"
#include "Manager.h"
#include "SDLGame.h"
#include "BulletPool.h"

using namespace std;

PiumPiumMasterClient::PiumPiumMasterClient() :
		game_(nullptr), //
		mngr_(nullptr), //
		exit_(false) {
	initGame();
}

PiumPiumMasterClient::~PiumPiumMasterClient() {
	closeGame();
}

void PiumPiumMasterClient::initGame() {

	game_ = SDLGame::init("PiumPiumMaster", _WINDOW_WIDTH_, _WINDOW_HEIGHT_);

	// create the manager
	mngr_ = new Manager(game_);
	//game_->setManager(mngr_);
	renderSystem_ = mngr_->addSystem<RenderSystem>();

}

void PiumPiumMasterClient::closeGame() {
	delete mngr_;
}

bool PiumPiumMasterClient::checkInput() {
    auto ih = InputHandler::instance();
    ih->update();
    int x = 0, y = 0;

    if (ih->keyDownEvent()) {
        if (ih->isKeyDown(SDLK_ESCAPE)) {
            exit_ = true;
            return false;
        }
        if (ih->isKeyDown(SDLK_w))         
            y = -1 ;

        if (ih->isKeyDown(SDLK_d)) 
            x = 1; 

        if (ih->isKeyDown(SDLK_a)) 
            x = -1; 
        
        if (ih->isKeyDown(SDLK_s)) 

        if(ih->getMouseButtonState(InputHandler::MOUSEBUTTON::LEFT)){
            //Mensaje de bala
        }

        //Mensaje de movimientoJugador(x,y,rotacion, gameObject);
        
    }
    return true;
}
void PiumPiumMasterClient::start(char* ip, char* port, char* playerName) {
	exit_ = false;
	
	net_client = new NetworkClient(ip, port, playerName, this);
	net_client->start();

	while (!exit_) {
		Uint32 startTime = game_->getTime();
		SDL_RenderClear(game_->getRenderer());
		SDL_SetRenderDrawColor(game_->getRenderer(), COLOR(0x003B00FF));

        if(!checkInput()) break;

		mngr_->refresh();

		renderSystem_->update();

		net_client->proccessMessages();
		mngr_->flushMessages();

		SDL_RenderPresent(game_->getRenderer());

		Uint32 frameTime = game_->getTime() - startTime;
		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}
}


