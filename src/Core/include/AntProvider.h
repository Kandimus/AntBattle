#pragma once

#include <stdint.h>
#include <memory>

#include "Ant.h"
#include "ApiVersion.h"

namespace AntBattle {

class Player;

class Provide {
public:
	static std::shared_ptr<Ant> newAnt(std::weak_ptr<Player> player, AntClass ant_class);
};

};
