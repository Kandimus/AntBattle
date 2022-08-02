#pragma once

#include "Ant.h"

namespace AntBattle {

class AntSolder : public Ant
{
public:
	AntSolder(std::weak_ptr<Player> player);
	virtual ~AntSolder() = default;

	virtual std::string strType() const override { return "solder"; }
	virtual uint32_t cargo() const override { return 0; }
};

};
