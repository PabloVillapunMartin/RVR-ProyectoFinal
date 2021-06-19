#pragma once
#include "System.h"
#include "PiumPiumMasterClient.h"
#include "BulletPool.h"


class RenderSystem: public System {
public:
	RenderSystem(PiumPiumMasterClient* piumpium, BulletPool* pool);
	void update() override;
private:
	PiumPiumMasterClient* piumpium;
	BulletPool* pool;
};

