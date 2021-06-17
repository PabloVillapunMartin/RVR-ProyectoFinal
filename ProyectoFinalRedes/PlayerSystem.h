#pragma once

#include <vector>
#include "System.h"

class PlayerSystem: public System {
public:
	PlayerSystem();

    virtual void init();
	virtual void recieve(const msg::Message& msg);

    std::vector<Entity*> players;
};

