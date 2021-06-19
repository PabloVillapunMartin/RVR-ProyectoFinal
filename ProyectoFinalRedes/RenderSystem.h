#pragma once
#include "System.h"
#include "PiumPiumMasterClient.h"


class RenderSystem: public System {
public:
	RenderSystem(PiumPiumMasterClient* piumpium = nullptr);
	void update() override;
private:
	PiumPiumMasterClient* piumpium;
};

