#pragma once
#include "System.h"



class RenderSystem: public System {
public:
	RenderSystem();
	void update() override;

private:
	void drawAnimated(Entity *e);
};

