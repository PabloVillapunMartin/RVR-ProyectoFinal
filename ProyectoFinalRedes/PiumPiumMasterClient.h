#pragma once

#include <vector>
#include "NetworkClient.h"


class CollisionSystem;
class Manager;
class SDLGame;
class RenderSystem;
class GameCtrlSystem;
class BulletPool;
class WallInfo;

class PiumPiumMasterClient {

public:
    /// <summary>
	/// Constructora por defecto
	/// </summary>
	PiumPiumMasterClient();
	/// <summary>
	/// Destrcutora por defecto
	/// </summary>
	virtual ~PiumPiumMasterClient();

	/// <summary>
	/// Inicializa la clase y se coneca al servidor
	/// </summary>
	void start(char* ip, char* port, char* playerName);

	/// <summary>
	/// Para la condición del bucle ppal
	/// </summary>
	void stop() { exit_ = true; };

	/// <summary>
	/// Crea un player dada una posicion
	/// </summary>
	void createPlayer(int x, int y);

	/// <summary>
	/// Crea un muro dada la información de un mensaje
	/// </summary>
	void createWall(WallInfo* ms);

	/// <summary>
	/// Actualiza un gameobject dada la posicion, rotacion, id(de la entidad)
	/// el tipo(bala o jugador) y si está activo o no
	/// </summary>
	void updateGO(int x, int y, float rot, int id, int type, bool active);

	inline void setID(int id){ idClient_ = id;}
	inline int getId(){ return idClient_;}
private:
    /// <summary>
	/// Crea los sistemas referentes al juego
	/// </summary>
	void initGame();
	/// <summary>
	/// Limpia los recursos generados por SDL
	/// </summary>
	void closeGame();
	/// <summary>
	/// Comprueba el input del jugador e informa al servidor
	/// </summary>
    bool checkInput();

	NetworkClient* net_client;
	SDLGame *game_;
	Manager *mngr_;

	RenderSystem *renderSystem_;
	GameCtrlSystem *gameCtrlSystem_;
	BulletPool* pool;

	bool exit_;
	int idClient_;
	const static int _WINDOW_WIDTH_ = 640;
	const static int _WINDOW_HEIGHT_ = 480;

	//----- Input -------
	bool up, down, right, left;
};
