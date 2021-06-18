#include "PiumPiumMasterClient.h"

#include <assert.h>
#include <math.h>

#include "ImageComponent.h"
#include "Transform.h"
#include "InputHandler.h"
#include "SDL_macros.h"
#include "RenderSystem.h"
#include "Manager.h"
#include "SDLGame.h"
#include "BulletPool.h"
#include "GameCtrlSystem.h"
#include "GameState.h"
#include "NetworkMessages.h"

#define PI 3.14159265

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
	game_->setManager(mngr_);
	renderSystem_ = mngr_->addSystem<RenderSystem>();
	gameCtrlSystem_ = mngr_->addSystem<GameCtrlSystem>();

}

void PiumPiumMasterClient::closeGame() {
	delete mngr_;
}

bool PiumPiumMasterClient::checkInput() {
    auto ih = InputHandler::instance();
    ih->update();
    int x = 0, y = 0;
	if (mngr_->getHandler(ecs::_hdlr_GameStateEntity)->getComponent<GameState>(ecs::GameState)->state == GameState::inGame) {
	
		if (ih->keyDownEvent() || ih->mouseMotionEvent() || ih->mouseButtonEvent()) {
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
				y = 1;

			if(ih->getMouseButtonState(InputHandler::MOUSEBUTTON::LEFT)){
				//Mensaje de bala
			}

			Vector2D posMouse = ih->getMousePos();
			Vector2D dir = posMouse - mngr_->getGroupEntities(ecs::_grp_Player)[idClient_]->getComponent<Transform>(ecs::Transform)->position_;
			float rot = atan(dir.getY() / dir.getX()) * 180 / PI;
			UpdateClientPlayerMessage ms;
			ms.go_id = idClient_;
			ms.x = x; ms.y = y;
			ms.rotation = rot - 90;
			net_client->send(&ms);
		}
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

void PiumPiumMasterClient::createGO(int x, int y, int id, int type){
	Entity* ent = mngr_->addEntity();

	Transform* tr = ent->addComponent<Transform>();
	tr->position_ = {x,y};

	ent->addComponent<IdGame>(ent->getEntityMngr()->getIdCount());
	if(type == 0){
		ent->addComponent<ImageComponent>(game_->getTextureMngr()->getTexture(Resources::Player));
		ent->addToGroup(ecs::_grp_Player);
		tr->width_= 32; tr->height_ = 32;
	}
	else{
		ent->addComponent<ImageComponent>(game_->getTextureMngr()->getTexture(Resources::Bullet));
		ent->addToGroup(ecs::_grp_Bullet);
		tr->width_= 16; tr->height_ = 16;
	}
}
void PiumPiumMasterClient::updateGO(int x, int y, float rot, int id){
	//Players
	if(id < 4){
		if(mngr_->getGroupEntities(ecs::_grp_Player).size() == 4){
			Transform* tr = mngr_->getGroupEntities(ecs::_grp_Player)[id]->getComponent<Transform>(ecs::Transform);
			tr->position_ = {x, y};
			tr->rotation_ = rot;
		}
	}
}
