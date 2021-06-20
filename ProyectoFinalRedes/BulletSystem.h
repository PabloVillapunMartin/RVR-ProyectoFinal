#pragma once
#include "System.h"
#include "Entity.h"
#include "BulletPool.h"

class NetworkServer;
/// <summary>
/// Sistema que controla las balas del juego
/// </summary>
class BulletSystem : public System
{
public:
/// <summary>
	/// Constructora
	/// </summary>
	BulletSystem(NetworkServer* net_, BulletPool* pool) : System(ecs::_sys_Bullets), net(net_), pool(pool) {}
	~BulletSystem() {}

	/// <summary>
	/// Crea una bala con la información dada dentro de la pool de balas
	/// </summary>
	void shoot(Vector2D pos, Vector2D vel, double width, double height, int id);
	
	/// <summary>
	/// Actualiza la posición de las balas en caso de estar activas
	/// </summary>
	void update() override;

	/// <summary>
	/// Procesa mensajes locals
	/// </summary>
	void recieve(const msg::Message& msg) override;
private:

	NetworkServer* net;		//servidor
	BulletPool* pool;		//pool de balas

};

