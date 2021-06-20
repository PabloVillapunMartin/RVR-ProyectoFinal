#pragma once

#include "System.h"
#include "NetworkServer.h"
#include "PiumPiumMasterClient.h"
#include "BulletPool.h"
struct GameState;

class GameCtrlSystem: public System {
public:
	/// <summary>
	/// Constructora
	/// </summary>
	GameCtrlSystem(NetworkServer* server, BulletPool* pool, PiumPiumMasterClient* client = nullptr);

	/// <summary>
	/// Ininializa el GameState
	/// </summary>
	void init() override;

	/// <summary>
	/// Procesa mensajes locales
	/// </summary>
	void recieve(const msg::Message& msg) override;

private:
	const int SHOOT_POINTS = 10;	//puntuación al acertar un disparo

	GameState *gameState_;			//componente de gameState
	NetworkServer* net_server;		//servidor
	PiumPiumMasterClient* client;	//juego del cliente
	BulletPool* pool;				//pool de balas
};

