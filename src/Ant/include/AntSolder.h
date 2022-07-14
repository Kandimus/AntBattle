#pragma once

#include "Ant.h"

namespace AntBattle {

class AntSolder : public Ant
{
public:
	AntSolder() = default;
	virtual ~AntSolder() = default;

	virtual std::string strType() const override { return "solder"; }
};

};
