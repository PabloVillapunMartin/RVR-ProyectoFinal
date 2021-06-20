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
		exit_(false),
		up(false),
		down(false),
		left(false), 
		right(false),
		pool(nullptr) {
	initGame();
}

PiumPiumMasterClient::~PiumPiumMasterClient() {
	delete net_client;
	closeGame();
}

void PiumPiumMasterClient::initGame() {

	game_ = SDLGame::init("PiumPiumMaster", _WINDOW_WIDTH_, _WINDOW_HEIGHT_, true);

	// create the managerB
	mngr_ = new Manager(game_);
	game_->setManager(mngr_);
	pool = new BulletPool(40);
	renderSystem_ = mngr_->addSystem<RenderSystem>(this, pool);
	gameCtrlSystem_ = mngr_->addSystem<GameCtrlSystem>(nullptr, pool, this);
}

void PiumPiumMasterClient::closeGame() {
	delete mngr_;
	delete pool;
}

bool PiumPiumMasterClient::checkInput() {
    auto ih = InputHandler::instance();
    ih->update();
	
	//Si hay algún evento de teclado o mouse
	//if (ih->keyDownEvent() || ih->mouseMotionEvent() || ih->mouseButtonEvent()) {
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

			if (ih->isKeyDown(SDLK_w))  	up = true;
			else if(ih->isKeyUp(SDLK_w))	up = false;

			if (ih->isKeyDown(SDLK_s)) 		down = true;
			else if(ih->isKeyUp(SDLK_s))	down = false;

			if (ih->isKeyDown(SDLK_a)) 		left = true;
			else if(ih->isKeyUp(SDLK_a))	left = false;
			
			if (ih->isKeyDown(SDLK_d))		right = true;
			else if(ih->isKeyUp(SDLK_d))	right = false;

			//Check de disparo de bala
			if(ih->getMouseButtonState(InputHandler::MOUSEBUTTON::LEFT)){
				ShootClientMessage ms;

				ms.dirX = dir.getX();		ms.dirY = dir.getY();
				ms.x = posPlayer.getX();	ms.y = posPlayer.getY();
				ms.idPlayer = idClient_;

				net_client->send(&ms);
			}

			int x = 0;
			int y = 0;

			if(up) y -= 1;
			if(down) y += 1;
			if(left) x -= 1;
			if(right) x += 1;

			//Mensaje update de input
			int speed = 2;
			UpdateClientPlayerMessage ms;
			ms.go_id = idClient_;	ms.x = x; 	ms.y = y;	ms.rotation = rot - 180.0f;
			if ( posMouse.getX() > posPlayer.getX() ) ms.rotation += 180.0f;

			net_client->send(&ms);
		}	
	//}
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
	ent->setVisible(true);
	tr->width_= 32; tr->height_ = 32;

}

void PiumPiumMasterClient::createWall(WallInfo* ms){
	Entity* ent = mngr_->addEntity();

	Transform* tr = ent->addComponent<Transform>();
	tr->position_ = {ms->x, ms->y};
	tr->height_ = ms->height;
	tr->width_ = ms->width;
	
	if(ms->width > ms->height)
		ent->addComponent<ImageComponent>(game_->getTextureMngr()->getTexture(Resources::Muro2));
	else ent->addComponent<ImageComponent>(game_->getTextureMngr()->getTexture(Resources::Muro));
	
	ent->addToGroup(ecs::_grp_Walls);
	ent->setVisible(true);
}

void PiumPiumMasterClient::updateGO(int x, int y, float rot, int id, int type, bool active){
	if(type == 0){
		if(mngr_->getGroupEntities(ecs::_grp_Player).size() == 4){
			Entity* ent = mngr_->getGroupEntities(ecs::_grp_Player)[id];
			Transform* tr = ent->getComponent<Transform>(ecs::Transform);
			tr->position_ = {x, y};
			tr->rotation_ = rot;
			ent->setVisible(active);
		}
	}
	else{
		if(pool){
			Entity* ent = pool->getPool()[id];
			Transform* tr = ent->getComponent<Transform>(ecs::Transform);
			tr->position_ = {x, y};
			tr->rotation_ = rot; 
			if(active == false) 
				pool->deleteBullet(ent);
			else 
				ent->setVisible(active);
		}
	}	
}
