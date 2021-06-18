#pragma once
#include "System.h"
#include "Entity.h"

class NetworkServer;

class BulletSystem : public System
{
public:
	BulletSystem(NetworkServer* net_) : System(ecs::_sys_Bullets), net(net_) {}
	~BulletSystem() {}

	void shoot(Vector2D pos, Vector2D vel, double width, double height);
		
	void update() override;
	void recieve(const msg::Message& msg) override;
private:

	NetworkServer* net;

};

