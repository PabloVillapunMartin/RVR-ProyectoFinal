#pragma once

#include <vector>
#include "NetworkServer.h"

class CollisionSystem;
class Manager;
class SDLGame;
class RenderSystem;
class PlayerSystem;
class GameCtrlSystem;
class BulletSystem;
class CollisionSystem;
class BulletPool;
class Transform;

class PiumPiumMasterServer {

public:
    /// <summary>
	/// Constructora por defecto que crea el servidor
	/// </summary>
	PiumPiumMasterServer(char* ip, char* port);
	/// <summary>
	/// Destructora por defecto
	/// </summary>
	virtual ~PiumPiumMasterServer();

	/// <summary>
	/// Bucle ppal del juego
	/// </summary>
	void start();
	/// <summary>
	/// Para parar la la ejecución del bucle ppal
	/// </summary>
	void stop();
	/// <summary>
	/// Crea los muros del juego
	/// </summary>
	void createWalls();

private:
	/// <summary>
	/// Inicializa los sistemas del juego y el servidor
	/// </summary>
	void initGame(char* ip, char* port);
	/// <summary>
	/// Libera los recursos y memoria utilizados
	/// </summary>
	void closeGame();

    /// <summary>
	/// Envía a los jugadores las posiciones actualizadas de todos los gameObjects
	/// </summary>
	void sendObjectPositions();

	/// <summary>
	/// Crea un muro dada una posicion y tamaño
	/// </summary>
	void createSingleWall(int x, int y, int width, int height);

	SDLGame *game_;
	Manager *mngr_;
	bool exit_;
	NetworkServer* net_server;
	BulletPool* pool;

	CollisionSystem *collisionSystem_;
	PlayerSystem *playerSystem_;
	GameCtrlSystem *gameCtrlSystem_;
	BulletSystem *bulletSystem_;

	const static int _WINDOW_WIDTH_ = 640;
	const static int _WINDOW_HEIGHT_ = 480;

};
