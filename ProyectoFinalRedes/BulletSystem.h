#pragma once
#include "System.h"
#include "Entity.h"
#include "BulletPool.h"

class NetworkServer;

class BulletSystem : public System
{
public:
	BulletSystem(NetworkServer* net_, BulletPool* pool) : System(ecs::_sys_Bullets), net(net_), pool(pool) {}
	~BulletSystem() {}

	void shoot(Vector2D pos, Vector2D vel, double width, double height, int id);
		
	void update() override;
	void recieve(const msg::Message& msg) override;
private:

	NetworkServer* net;
	BulletPool* pool;

};

