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
	delete net_client;
	closeGame();
}

void PiumPiumMasterClient::initGame() {

	game_ = SDLGame::init("PiumPiumMaster", _WINDOW_WIDTH_, _WINDOW_HEIGHT_);

	// create the manager
	mngr_ = new Manager(game_);
	game_->setManager(mngr_);
	renderSystem_ = mngr_->addSystem<RenderSystem>();
	gameCtrlSystem_ = mngr_->addSystem<GameCtrlSystem>(nullptr, this);

}

void PiumPiumMasterClient::closeGame() {
	delete mngr_;
}

bool PiumPiumMasterClient::checkInput() {
    auto ih = InputHandler::instance();
    ih->update();
    int x = 0, y = 0;
	
	//Si hay algún evento de teclado o mouse
	if (ih->keyDownEvent() || ih->mouseMotionEvent() || ih->mouseButtonEvent()) {
		//Si el jugador sale de la app
		if (ih->isKeyDown(SDLK_ESCAPE)) {
			exit_ = true;
			return false;
		}
		if (mngr_->getHandler(ecs::_hdlr_GameStateEntity)->getComponent<GameState>(ecs::GameState)->state == GameState::inGame &&
			mngr_->getGroupEntities(ecs::_grp_Player).size() == 4) {

			Vector2D posMouse = ih->getMousePos();
			Vector2D posPlayer = mngr_->getGroupEntities(ecs::_grp_Player)[idClient_]->getComponent<Transform>(ecs::Transform)->position_;
			//Calculamos la direccion a la que mira el player
			Vector2D dir = (posMouse - posPlayer).normalize();

			float rot = atan(dir.getY() / dir.getX()) * 180 / PI;
			
			//Check de movimiento arriba y abajo

			if (ih->isKeyDown(SDLK_w))  	y = -1 ;

			if (ih->isKeyDown(SDLK_d)) 		x = 1; 

			if (ih->isKeyDown(SDLK_a)) 		x = -1; 
			
			if (ih->isKeyDown(SDLK_s))		y = 1;

			//Check de disparo de bala
			if(ih->getMouseButtonState(InputHandler::MOUSEBUTTON::LEFT)){
				ShootClientMessage ms;

				ms.dirX = dir.getX();		ms.dirY = dir.getY();
				ms.x = posPlayer.getX();	ms.y = posPlayer.getY();

				net_client->send(&ms);
			}

			//Mensaje update de input
			UpdateClientPlayerMessage ms;
			ms.go_id = idClient_;	ms.x = x; 	ms.y = y;	ms.rotation = rot - 90.0f;
			if ( posMouse.getX() > posPlayer.getX() ) ms.rotation += 180.0f;

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

void PiumPiumMasterClient::createPlayer(int x, int y){

	Entity* ent = mngr_->addEntity();

	Transform* tr = ent->addComponent<Transform>();
	tr->position_ = {x,y};
	ent->addComponent<ImageComponent>(game_->getTextureMngr()->getTexture(Resources::Player));
	ent->addToGroup(ecs::_grp_Player);
	tr->width_= 32; tr->height_ = 32;
}
void PiumPiumMasterClient::createBullet(int x, int y){
	Vector2D pos = {x,y};
	Entity* e = mngr_->addEntity<BulletPool>(pos, Vector2D(), 16, 16);
	if (e != nullptr) {
		e->setActive(true);
		e->addToGroup(ecs::_grp_Bullet);
		//game_->getAudioMngr()->playChannel(Resources::Bullet, 0, 1);
	}

}
void PiumPiumMasterClient::updateGO(int x, int y, float rot, int id, int type, bool active){
	if(type == 0){
		if(mngr_->getGroupEntities(ecs::_grp_Player).size() == 4){
			Entity* ent = mngr_->getGroupEntities(ecs::_grp_Player)[id];
			Transform* tr = ent->getComponent<Transform>(ecs::Transform);
			tr->position_ = {x, y};
			tr->rotation_ = rot;
		}
	}
	else{
		Entity* ent = mngr_->getGroupEntities(ecs::_grp_Bullet)[id];
		Transform* tr = ent->getComponent<Transform>(ecs::Transform);
		tr->position_ = {x, y};
		tr->rotation_ = rot;
		ent->setActive(active);
	}	
}
void PiumPiumMasterClient::initPoolBullets(){
	BulletPool::init(40);
}