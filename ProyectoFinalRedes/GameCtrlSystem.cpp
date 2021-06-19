#include "GameCtrlSystem.h"


#include "SDLGame.h"
#include "Manager.h"
#include "GameState.h"
#include "BulletIDPlayer.h"


GameCtrlSystem::GameCtrlSystem(NetworkServer* server, PiumPiumMasterClient* client) :
System(ecs::_sys_GameCtrl), //
gameState_(nullptr),		//
net_server(server),			//
client(client)				//
{
}

void GameCtrlSystem::init() {
	Entity *e = mngr_->addEntity();
	gameState_ = e->addComponent<GameState>();
	for(int i = 0; i< 4; i++){
		gameState_->life.push_back(3);
		gameState_->points.push_back(0);
	}
	mngr_->setHandler(ecs::_hdlr_GameStateEntity, e);
}

void GameCtrlSystem::recieve(const msg::Message& msg){
	switch (msg.id)
	{
	//Se recibe de forma local en el servidor
	case msg::_BULLET_COLLISION_ :{
		msg::BulletCollisionMessage info = static_cast<const msg::BulletCollisionMessage&>(msg);
		//Restamos vida en el jugador que ha sido disparado
		gameState_->life[info.id_player]--;
		//En caso de matarlo, el jugador pasará a estar inactivo 
		if(gameState_->life[info.id_player] <= 0)
			mngr_->getGroupEntities(ecs::_grp_Player)[info.id_player]->setActive(false);
		//Sumamos puntos al jugador que ha disparado
		int idPlayer = mngr_->getGroupEntities(ecs::_grp_Bullet)[info.id_bullet]->getComponent<BulletIDPlayer>(ecs::BulletIDPlayer)->idPlayer;
		gameState_->points[idPlayer] += SHOOT_POINTS;
		//Enviamos a los clientes la informacion necesaria
		if(net_server){
			UpdatePlayerStateMessage ms(gameState_->life[0], gameState_->points[0], gameState_->life[1], gameState_->points[1],
			gameState_->life[2], gameState_->points[2], gameState_->life[3], gameState_->points[3]);
			net_server->broadcastMessage(&ms);
		} 
		break;
	} 	
	//Se rcibe de forma local en los clientes
	case msg::_UPDATE_PLAYER_INFO:{
		msg::UpdatePlayerState info = static_cast<const msg::UpdatePlayerState&>(msg);
		//Actualizacion de vidas y puntos
		gameState_->life[0] = info.lives1;
		gameState_->life[1] = info.lives2;
		gameState_->life[2] = info.lives3;
		gameState_->life[3] = info.lives4;

		gameState_->points[0] = info.points1;
		gameState_->points[1] = info.points2;
		gameState_->points[2] = info.points3;
		gameState_->points[3] = info.points4;

		if(gameState_->life[client->getId()] <= 0)
			gameState_->state = GameState::ondeath;
		break;
	}
	default:
		break;
	}
}